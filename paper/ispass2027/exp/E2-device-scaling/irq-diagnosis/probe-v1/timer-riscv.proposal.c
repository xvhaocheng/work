// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
 *
 * All RISC-V systems have a timer attached to every hart.  These timers can
 * either be read from the "time" and "timeh" CSRs, and can use the SBI to
 * setup events, or directly accessed using MMIO registers.
 */

#define pr_fmt(fmt) "riscv-timer: " fmt

#include <linux/acpi.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/cpu.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/sched_clock.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/io-64-nonatomic-lo-hi.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>
#include <clocksource/timer-riscv.h>
#include <asm/smp.h>
#include <asm/hwcap.h>
#include <asm/sbi.h>
#include <asm/timex.h>

static DEFINE_STATIC_KEY_FALSE(riscv_sstc_available);
static bool riscv_timer_cannot_wake_cpu;

struct riscv_timer_probe_stamp {
	u64 cycle;
	u64 instret;
};

#if defined(CONFIG_64BIT) && defined(CONFIG_DEBUG_FS)
/*
 * Bounded E2 diagnostic, disabled by default. This is specific to the fixed
 * single-hart CVA6/OpenSBI platform: CYCLE/INSTRET are native counters and
 * OpenSBI enables their S-mode access in MCOUNTEREN. Do not infer that from
 * rdcycle alone, or enable this on a platform which emulates these CSRs.
 *
 * Only CPU0 with local IRQs disabled writes the state. Control/snapshot run
 * on the sole online CPU0 under a mutex. No IRQ-side locks, allocation, time
 * reads, MMIO reads or printing are introduced by the diagnostic helpers.
 */
#define RISCV_TIMER_PROBE_RECORDS 64
#define RISCV_TIMER_PROBE_SAMPLE_MASK 63
#define RISCV_TIMER_PROBE_HANDLER 1
#define RISCV_TIMER_PROBE_REARM 2

struct riscv_timer_probe_totals {
	u64 count;
	u64 cycles_sum;
	u64 cycles_max;
	u64 instret_sum;
	u64 instret_max;
};

struct riscv_timer_probe_record {
	u64 sequence;
	u64 kind;
	u64 cycle_start;
	u64 cycles;
	u64 instret;
	u64 sample_cycle;
	u64 raw_now;
	u64 delta;
	u64 next_tval;
	unsigned long sip;
	unsigned long sie;
};

static struct riscv_timer_probe_state {
	bool enabled;
	u64 start_cycle;
	u64 stop_cycle;
	u64 event_sequence;
	u64 records_written;
	struct riscv_timer_probe_totals handler;
	struct riscv_timer_probe_totals rearm;
	struct riscv_timer_probe_record records[RISCV_TIMER_PROBE_RECORDS];
} riscv_timer_probe;
static DEFINE_MUTEX(riscv_timer_probe_mutex);

static __always_inline bool riscv_timer_probe_active(void)
{
	if (likely(!READ_ONCE(riscv_timer_probe.enabled)))
		return false;
	if (raw_smp_processor_id() || num_online_cpus() != 1 ||
	    cpuid_to_hartid_map(0) || !irqs_disabled())
		return false;
	return true;
}

static __always_inline void
riscv_timer_probe_begin(struct riscv_timer_probe_stamp *stamp)
{
	stamp->cycle = csr_read(CSR_CYCLE);
	stamp->instret = csr_read(CSR_INSTRET);
}

static void riscv_timer_probe_end(const struct riscv_timer_probe_stamp *stamp,
				  unsigned int kind, u64 raw_now, u64 delta,
				  u64 next_tval)
{
	struct riscv_timer_probe_state *p = &riscv_timer_probe;
	struct riscv_timer_probe_totals *totals;
	struct riscv_timer_probe_record *record;
	u64 instret = csr_read(CSR_INSTRET) - stamp->instret;
	u64 cycles = csr_read(CSR_CYCLE) - stamp->cycle;
	u64 sequence = p->event_sequence + 1;
	u64 sample_cycle = 0;
	unsigned long sip = 0, sie = 0;
	bool new_max, sample;

	totals = kind == RISCV_TIMER_PROBE_HANDLER ? &p->handler : &p->rearm;
	new_max = cycles > totals->cycles_max;
	sample = new_max || !((totals->count + 1) & RISCV_TIMER_PROBE_SAMPLE_MASK);
	/* Sample pending state before probe bookkeeping can consume the slack. */
	if (sample) {
		sample_cycle = csr_read(CSR_CYCLE);
		sip = csr_read(CSR_IP);
		sie = csr_read(CSR_IE);
	}
	p->event_sequence = sequence;
	totals->count++;
	totals->cycles_sum += cycles;
	totals->cycles_max = max(totals->cycles_max, cycles);
	totals->instret_sum += instret;
	totals->instret_max = max(totals->instret_max, instret);
	/* Every 64th event per kind plus new maxima, not a distribution sample. */
	if (!sample)
		return;
	record = &p->records[p->records_written++ &
			     (RISCV_TIMER_PROBE_RECORDS - 1)];
	record->sequence = sequence;
	record->kind = kind;
	record->cycle_start = stamp->cycle;
	record->cycles = cycles;
	record->instret = instret;
	record->sample_cycle = sample_cycle;
	record->raw_now = raw_now;
	record->delta = delta;
	record->next_tval = next_tval;
	record->sip = sip;
	record->sie = sie;
}

/* Keep probe stack objects and their stack protector off the disabled path. */
static noinline void
riscv_timer_probe_handler(struct clock_event_device *evdev)
{
	struct riscv_timer_probe_stamp stamp;

	riscv_timer_probe_begin(&stamp);
	evdev->event_handler(evdev);
	riscv_timer_probe_end(&stamp, RISCV_TIMER_PROBE_HANDLER, 0, 0, 0);
}

static noinline void riscv_timer_probe_rearm(u64 raw_now, unsigned long delta,
					     u64 next_tval)
{
	struct riscv_timer_probe_stamp stamp;

	riscv_timer_probe_begin(&stamp);
	sbi_set_timer(next_tval);
	riscv_timer_probe_end(&stamp, RISCV_TIMER_PROBE_REARM,
			      raw_now, delta, next_tval);
}

static int riscv_timer_probe_control_get(void *unused, u64 *value)
{
	int cpu = get_cpu();
	int ret = 0;

	if (cpu || num_online_cpus() != 1 || !cpu_online(0) ||
	    cpuid_to_hartid_map(0))
		ret = -EOPNOTSUPP;
	else
		*value = READ_ONCE(riscv_timer_probe.enabled);
	put_cpu();
	return ret;
}

static int riscv_timer_probe_control_set(void *unused, u64 value)
{
	unsigned long flags;
	int ret = 0;
	int cpu;

	if (value > 1)
		return -EINVAL;
	mutex_lock(&riscv_timer_probe_mutex);
	cpus_read_lock();
	cpu = get_cpu();
	if (cpu || num_online_cpus() != 1 || !cpu_online(0) ||
	    cpuid_to_hartid_map(0)) {
		ret = -EOPNOTSUPP;
		goto out;
	}
	if (value && READ_ONCE(riscv_timer_probe.enabled)) {
		ret = -EBUSY;
		goto out;
	}
	/* The disabled state has no IRQ writers; reset outside IRQ-off time. */
	if (value)
		memset(&riscv_timer_probe, 0, sizeof(riscv_timer_probe));
	local_irq_save(flags);
	if (value) {
		riscv_timer_probe.start_cycle = csr_read(CSR_CYCLE);
		WRITE_ONCE(riscv_timer_probe.enabled, true);
	} else if (READ_ONCE(riscv_timer_probe.enabled)) {
		WRITE_ONCE(riscv_timer_probe.enabled, false);
		riscv_timer_probe.stop_cycle = csr_read(CSR_CYCLE);
	}
	local_irq_restore(flags);
out:
	put_cpu();
	cpus_read_unlock();
	mutex_unlock(&riscv_timer_probe_mutex);
	return ret;
}
DEFINE_DEBUGFS_ATTRIBUTE(riscv_timer_probe_control_fops,
			 riscv_timer_probe_control_get,
			 riscv_timer_probe_control_set, "%llu\n");

static int riscv_timer_probe_show(struct seq_file *m, void *unused)
{
	struct riscv_timer_probe_state *p = m->private;
	u64 first = p->records_written > RISCV_TIMER_PROBE_RECORDS ?
		p->records_written - RISCV_TIMER_PROBE_RECORDS : 0;
	u64 n;

	seq_printf(m, "version=1 cpu=0 enabled=0 start_cycle=%llu stop_cycle=%llu\n",
		   p->start_cycle, p->stop_cycle);
	seq_puts(m, "counter_contract=fixed_CVA6_8aac_OpenSBI_e8d_native_S_mode\n");
	seq_puts(m, "sampling=every_64th_per_kind_event_or_new_cycle_max ring=64; not_CDF\n");
	seq_puts(m, "timing=handler_includes_nested_rearm_probe; costs_unmeasured\n");
	seq_printf(m, "handler count=%llu cycles_sum=%llu cycles_max=%llu instret_sum=%llu instret_max=%llu\n",
		   p->handler.count, p->handler.cycles_sum, p->handler.cycles_max,
		   p->handler.instret_sum, p->handler.instret_max);
	seq_printf(m, "rearm count=%llu cycles_sum=%llu cycles_max=%llu instret_sum=%llu instret_max=%llu\n",
		   p->rearm.count, p->rearm.cycles_sum, p->rearm.cycles_max,
		   p->rearm.instret_sum, p->rearm.instret_max);
	seq_printf(m, "events=%llu records_written=%llu\n",
		   p->event_sequence, p->records_written);
	seq_puts(m, "sequence,kind,cycle_start,cycles,instret,sample_cycle,raw_now,delta,next_tval,sip,sie\n");
	for (n = first; n < p->records_written; n++) {
		struct riscv_timer_probe_record *r =
			&p->records[n & (RISCV_TIMER_PROBE_RECORDS - 1)];

		seq_printf(m, "%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%lx,%lx\n",
			   r->sequence, r->kind, r->cycle_start, r->cycles,
			   r->instret, r->sample_cycle, r->raw_now, r->delta, r->next_tval,
			   r->sip, r->sie);
	}
	return 0;
}

static int riscv_timer_probe_open(struct inode *inode, struct file *file)
{
	struct riscv_timer_probe_state *snapshot;
	int ret;

	mutex_lock(&riscv_timer_probe_mutex);
	cpus_read_lock();
	if (num_online_cpus() != 1 || !cpu_online(0) ||
	    cpuid_to_hartid_map(0)) {
		ret = -EOPNOTSUPP;
		goto out;
	}
	if (READ_ONCE(riscv_timer_probe.enabled)) {
		ret = -EBUSY;
		goto out;
	}
	snapshot = kmemdup(&riscv_timer_probe, sizeof(*snapshot), GFP_KERNEL);
	if (!snapshot) {
		ret = -ENOMEM;
		goto out;
	}
	ret = single_open(file, riscv_timer_probe_show, snapshot);
	if (ret)
		kfree(snapshot);
out:
	cpus_read_unlock();
	mutex_unlock(&riscv_timer_probe_mutex);
	return ret;
}

static int riscv_timer_probe_release(struct inode *inode, struct file *file)
{
	struct seq_file *m = file->private_data;

	kfree(m->private);
	return single_release(inode, file);
}

static const struct file_operations riscv_timer_probe_snapshot_fops = {
	.owner = THIS_MODULE,
	.open = riscv_timer_probe_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = riscv_timer_probe_release,
};

static int __init riscv_timer_probe_init(void)
{
	struct dentry *dir;

	if (!of_machine_is_compatible("eth,ariane-bare-dev") ||
	    static_branch_unlikely(&riscv_sstc_available))
		return 0;
	dir = debugfs_create_dir("riscv_timer_probe", NULL);
	if (IS_ERR(dir))
		return PTR_ERR(dir);
	debugfs_create_file("control", 0600, dir, NULL,
			    &riscv_timer_probe_control_fops);
	debugfs_create_file("snapshot", 0400, dir, NULL,
			    &riscv_timer_probe_snapshot_fops);
	return 0;
}
late_initcall(riscv_timer_probe_init);
#else
static inline bool riscv_timer_probe_active(void)
{
	return false;
}

static inline void riscv_timer_probe_handler(struct clock_event_device *evdev)
{
	evdev->event_handler(evdev);
}

static inline void riscv_timer_probe_rearm(u64 raw_now, unsigned long delta,
					   u64 next_tval)
{
	sbi_set_timer(next_tval);
}
#endif

static int riscv_clock_next_event(unsigned long delta,
		struct clock_event_device *ce)
{
	u64 raw_now = get_cycles64();
	u64 next_tval = raw_now + delta;

	csr_set(CSR_IE, IE_TIE);
	if (static_branch_likely(&riscv_sstc_available)) {
#if defined(CONFIG_32BIT)
		csr_write(CSR_STIMECMP, next_tval & 0xFFFFFFFF);
		csr_write(CSR_STIMECMPH, next_tval >> 32);
#else
		csr_write(CSR_STIMECMP, next_tval);
#endif
	} else if (riscv_timer_probe_active()) {
		riscv_timer_probe_rearm(raw_now, delta, next_tval);
	} else {
		sbi_set_timer(next_tval);
	}

	return 0;
}

static unsigned int riscv_clock_event_irq;
static DEFINE_PER_CPU(struct clock_event_device, riscv_clock_event) = {
	.name			= "riscv_timer_clockevent",
	.features		= CLOCK_EVT_FEAT_ONESHOT,
	.rating			= 100,
	.set_next_event		= riscv_clock_next_event,
};

/*
 * It is guaranteed that all the timers across all the harts are synchronized
 * within one tick of each other, so while this could technically go
 * backwards when hopping between CPUs, practically it won't happen.
 */
static unsigned long long riscv_clocksource_rdtime(struct clocksource *cs)
{
	return get_cycles64();
}

static u64 notrace riscv_sched_clock(void)
{
	return get_cycles64();
}

static struct clocksource riscv_clocksource = {
	.name		= "riscv_clocksource",
	.rating		= 400,
	.mask		= CLOCKSOURCE_MASK(64),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
	.read		= riscv_clocksource_rdtime,
#if IS_ENABLED(CONFIG_GENERIC_GETTIMEOFDAY)
	.vdso_clock_mode = VDSO_CLOCKMODE_ARCHTIMER,
#else
	.vdso_clock_mode = VDSO_CLOCKMODE_NONE,
#endif
};

static int riscv_timer_starting_cpu(unsigned int cpu)
{
	struct clock_event_device *ce = per_cpu_ptr(&riscv_clock_event, cpu);

	ce->cpumask = cpumask_of(cpu);
	ce->irq = riscv_clock_event_irq;
	if (riscv_timer_cannot_wake_cpu)
		ce->features |= CLOCK_EVT_FEAT_C3STOP;
	clockevents_config_and_register(ce, riscv_timebase, 100, 0x7fffffff);

	enable_percpu_irq(riscv_clock_event_irq,
			  irq_get_trigger_type(riscv_clock_event_irq));
	return 0;
}

static int riscv_timer_dying_cpu(unsigned int cpu)
{
	disable_percpu_irq(riscv_clock_event_irq);
	return 0;
}

void riscv_cs_get_mult_shift(u32 *mult, u32 *shift)
{
	*mult = riscv_clocksource.mult;
	*shift = riscv_clocksource.shift;
}
EXPORT_SYMBOL_GPL(riscv_cs_get_mult_shift);

/* called directly from the low-level interrupt handler */
static irqreturn_t riscv_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evdev = this_cpu_ptr(&riscv_clock_event);

	csr_clear(CSR_IE, IE_TIE);
	if (riscv_timer_probe_active())
		riscv_timer_probe_handler(evdev);
	else
		evdev->event_handler(evdev);

	return IRQ_HANDLED;
}

static int __init riscv_timer_init_common(void)
{
	int error;
	struct irq_domain *domain;
	struct fwnode_handle *intc_fwnode = riscv_get_intc_hwnode();

	domain = irq_find_matching_fwnode(intc_fwnode, DOMAIN_BUS_ANY);
	if (!domain) {
		pr_err("Failed to find irq_domain for INTC node [%pfwP]\n",
		       intc_fwnode);
		return -ENODEV;
	}

	riscv_clock_event_irq = irq_create_mapping(domain, RV_IRQ_TIMER);
	if (!riscv_clock_event_irq) {
		pr_err("Failed to map timer interrupt for node [%pfwP]\n", intc_fwnode);
		return -ENODEV;
	}

	error = clocksource_register_hz(&riscv_clocksource, riscv_timebase);
	if (error) {
		pr_err("RISCV timer registration failed [%d]\n", error);
		return error;
	}

	sched_clock_register(riscv_sched_clock, 64, riscv_timebase);

	error = request_percpu_irq(riscv_clock_event_irq,
				    riscv_timer_interrupt,
				    "riscv-timer", &riscv_clock_event);
	if (error) {
		pr_err("registering percpu irq failed [%d]\n", error);
		return error;
	}

	if (riscv_isa_extension_available(NULL, SSTC)) {
		pr_info("Timer interrupt in S-mode is available via sstc extension\n");
		static_branch_enable(&riscv_sstc_available);
	}

	error = cpuhp_setup_state(CPUHP_AP_RISCV_TIMER_STARTING,
			 "clockevents/riscv/timer:starting",
			 riscv_timer_starting_cpu, riscv_timer_dying_cpu);
	if (error)
		pr_err("cpu hp setup state failed for RISCV timer [%d]\n",
		       error);

	return error;
}

static int __init riscv_timer_init_dt(struct device_node *n)
{
	int cpuid, error;
	unsigned long hartid;
	struct device_node *child;

	error = riscv_of_processor_hartid(n, &hartid);
	if (error < 0) {
		pr_warn("Invalid hartid for node [%pOF] error = [%lu]\n",
			n, hartid);
		return error;
	}

	cpuid = riscv_hartid_to_cpuid(hartid);
	if (cpuid < 0) {
		pr_warn("Invalid cpuid for hartid [%lu]\n", hartid);
		return cpuid;
	}

	if (cpuid != smp_processor_id())
		return 0;

	child = of_find_compatible_node(NULL, NULL, "riscv,timer");
	if (child) {
		riscv_timer_cannot_wake_cpu = of_property_read_bool(child,
					"riscv,timer-cannot-wake-cpu");
		of_node_put(child);
	}

	return riscv_timer_init_common();
}

TIMER_OF_DECLARE(riscv_timer, "riscv", riscv_timer_init_dt);

#ifdef CONFIG_ACPI
static int __init riscv_timer_acpi_init(struct acpi_table_header *table)
{
	return riscv_timer_init_common();
}

TIMER_ACPI_DECLARE(aclint_mtimer, ACPI_SIG_RHCT, riscv_timer_acpi_init);

#endif
