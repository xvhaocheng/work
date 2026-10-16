/*
 * Host model for the bounded software batch-start control in
 * linux/drivers/misc/idma.c.
 *
 * Production helpers are inserted verbatim at the PRODUCTION_TYPES /
 * PRODUCTION_FUNCTIONS markers.  Everything else is a host stub: kernel MMIO,
 * ktime, signal, IRQ/preempt flags and logging.  This proves software control
 * flow only; it is not FPGA, RTL, IOTLB, cache or timing acceptance data.
 */
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

typedef __u64 u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uintptr_t dma_addr_t;
typedef uintptr_t phys_addr_t;

struct device { int index; };
struct page { int unused; };
struct miscdevice { struct device *parent; };
struct mutex { int unused; };
struct iommu_domain { int unused; };

#define BIT_ULL(n) (1ULL << (n))
#define __iomem
#define __user
#define WRITE_ONCE(dst, src) ((dst) = (src))
#define READ_ONCE(src) (*(volatile __typeof__(src) *)&(src))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define PAGE_SIZE 4096U
#define offset_in_page(ptr) ((uintptr_t)(ptr) & (PAGE_SIZE - 1))
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL
#define GFP_KERNEL 0
#define kzalloc(n, flags) calloc(1, (n))
#define kfree(ptr) free(ptr)

/* PRODUCTION_TYPES */

struct fake_dma {
    u64 regs[32];
    u64 next_id;
    bool zero_id;
    bool pending;
};

static struct fake_dma hw[2];
static struct device devices[2] = {{0}, {1}};
static struct idma_device idmas[2];
static u64 now = 1000;
static bool signal_flag;
/*
 * irq_disabled_depth models the CPU interrupt state, driver_irq_depth models
 * only this driver's local_irq_save()/local_irq_restore() window.  Keeping them
 * apart lets a case start with IRQs already disabled by the caller.
 */
static unsigned irq_disabled_depth, driver_irq_depth, preempt_disabled_depth;
static bool irq_pending, irq_serviced;
static u64 irq_service_ns, last_restore_ns;
/* Case knobs. */
static bool inject_irq_on_a_trigger;
static bool no_commit_before_v_trigger;
static struct idma_stream *commit_watch;
static unsigned stall_after_conf_write, signal_after_conf_write;
static unsigned conf_writes, log_calls, crit_calls;
static u64 stall_ns = 2 * NSEC_PER_MSEC;
static struct idma_e3_diag_lane diag_lane[2];

enum mmio_op { MMIO_WRITE, MMIO_READ };
struct mmio_event { enum mmio_op op; int dev; size_t offset; u64 value; };
static struct mmio_event mmio_events[256];
static unsigned mmio_count;

static void record_mmio(enum mmio_op op, int dev, size_t offset, u64 value)
{
    assert(mmio_count < 256);
    mmio_events[mmio_count].op = op;
    mmio_events[mmio_count].dev = dev;
    mmio_events[mmio_count].offset = offset;
    mmio_events[mmio_count].value = value;
    mmio_count++;
}

static struct fake_dma *mmio(void *address, int *dev, size_t *offset)
{
    int i;

    for (i = 0; i < 2; i++) {
        uintptr_t p = (uintptr_t)address;
        uintptr_t base = (uintptr_t)hw[i].regs;

        if (p >= base && p < base + sizeof(hw[i].regs)) {
            *dev = i;
            *offset = p - base;
            return &hw[i];
        }
    }
    assert(!"invalid MMIO address");
    return NULL;
}

static void writeq(u64 value, void *address)
{
    int dev;
    size_t offset;
    struct fake_dma *h = mmio(address, &dev, &offset);

    /*
     * Submission configuration must never be written while this driver still
     * holds its own trigger window or while preemption is disabled; only the
     * trigger reads belong to the protected section.  A caller may already
     * have global IRQs disabled (irq_disabled_depth > 0) when it enters; that
     * is legal here and is what the irq_restore_paths fixture (c) exercises.
     */
    if (offset == IDMA_SRC_ADDR_REG_OFFSET || offset == IDMA_DST_ADDR_REG_OFFSET ||
        offset == IDMA_NUM_BYTES_REG_OFFSET || offset == IDMA_CONF_REG_OFFSET)
        assert(!driver_irq_depth && !preempt_disabled_depth);

    record_mmio(MMIO_WRITE, dev, offset, value);
    h->regs[offset / 8] = value;

    if (offset == IDMA_CONF_REG_OFFSET) {
        conf_writes++;
        if (stall_after_conf_write && conf_writes == stall_after_conf_write)
            now += stall_ns;
        if (signal_after_conf_write && conf_writes == signal_after_conf_write)
            signal_flag = true;
    }
}

static u64 readq(void *address)
{
    int dev;
    size_t offset;
    struct fake_dma *h = mmio(address, &dev, &offset);
    u64 value;

    if (offset == IDMA_NEXT_ID_REG_OFFSET) {
        /* NEXT_ID is the batch trigger: IRQs and preemption must be off. */
        assert(irq_disabled_depth && preempt_disabled_depth);
        if (inject_irq_on_a_trigger && dev == 1) {
            assert(!irq_serviced);
            irq_pending = true;
        }
        if (inject_irq_on_a_trigger && dev == 0) {
            /*
             * V is still inside the protected section with the injected IRQ
             * pending: it must be neither serviced nor re-enabled yet.
             */
            assert(irq_pending && !irq_serviced);
            assert(irq_disabled_depth && preempt_disabled_depth);
        }
        if (no_commit_before_v_trigger && dev == 0)
            /* A valid A must not be published before the V trigger read. */
            assert(commit_watch && !commit_watch->transfer_id &&
                   !commit_watch->in_flight);
        h->pending = true;
        value = h->zero_id ? 0 : ++h->next_id;
    } else {
        value = h->regs[offset / 8];
    }
    record_mmio(MMIO_READ, dev, offset, value);
    return value;
}

/* A pending interrupt can only be taken once interrupts are enabled again. */
static void irq_deliver_if_enabled(void)
{
    if (irq_pending && !irq_serviced && !irq_disabled_depth) {
        irq_serviced = true;
        irq_pending = false;
        irq_service_ns = now;
    }
}

static u64 ktime_get_ns(void)
{
    now += 1000;
    return now;
}

static bool signal_pending(void *task)
{
    (void)task;
    return signal_flag;
}

#define local_irq_save(flags) do { \
    (flags) = irq_disabled_depth++; \
    driver_irq_depth++; \
} while (0)
#define local_irq_restore(flags) do { \
    assert(irq_disabled_depth == (unsigned long)(flags) + 1); \
    irq_disabled_depth = (unsigned long)(flags); \
    assert(driver_irq_depth); \
    driver_irq_depth--; \
    last_restore_ns = now; \
    irq_deliver_if_enabled(); \
} while (0)
#define preempt_disable() do { preempt_disabled_depth++; } while (0)
#define preempt_enable() do { assert(preempt_disabled_depth); preempt_disabled_depth--; } while (0)

static char log_buffer[1024];

static void dev_info(struct device *dev, const char *fmt, ...)
{
    va_list ap;

    (void)dev;
    va_start(ap, fmt);
    vsnprintf(log_buffer, sizeof(log_buffer), fmt, ap);
    va_end(ap);
    log_calls++;
    /* Only outside this driver's own protected trigger section. */
    assert(!driver_irq_depth && !preempt_disabled_depth);
}

static void dev_crit(struct device *dev, const char *fmt, ...)
{
    (void)dev;
    (void)fmt;
    crit_calls++;
    /*
     * Quarantine logging is legal with a pre-existing caller IRQ-off state,
     * but never while this driver still holds its own window open.
     */
    assert(!driver_irq_depth && !preempt_disabled_depth);
}

/* PRODUCTION_FUNCTIONS */

static void setup_streams(struct idma_stream *v, struct idma_stream *a, bool has_a)
{
    int i;

    memset(v, 0, sizeof(*v));
    memset(a, 0, sizeof(*a));
    for (i = 0; i < 2; i++) {
        idmas[i].id = i;
        idmas[i].did = 10 + i;
        idmas[i].reg = hw[i].regs;
        idmas[i].miscdev.parent = &devices[i];
        idmas[i].quarantined = false;
        hw[i].next_id = 100;
        hw[i].zero_id = false;
        hw[i].pending = false;
    }
    v->idma = &idmas[0];
    v->dev = &devices[0];
    v->size = 4096;
    v->chunk = 4096;
    v->src_iova = 0x1000;
    v->dst_iova = 0x2000;
    if (has_a) {
        a->idma = &idmas[1];
        a->dev = &devices[1];
        a->size = 4096;
        a->chunk = 4096;
        a->src_iova = 0x3000;
        a->dst_iova = 0x4000;
    }
}

static void reset_state(void)
{
    now = 1000;
    signal_flag = false;
    irq_disabled_depth = driver_irq_depth = preempt_disabled_depth = 0;
    irq_pending = irq_serviced = false;
    irq_service_ns = last_restore_ns = 0;
    inject_irq_on_a_trigger = no_commit_before_v_trigger = false;
    commit_watch = NULL;
    stall_after_conf_write = signal_after_conf_write = 0;
    conf_writes = log_calls = crit_calls = 0;
    mmio_count = 0;
    log_buffer[0] = 0;
    memset(hw, 0, sizeof(hw));
    memset(idmas, 0, sizeof(idmas));
    memset(diag_lane, 0, sizeof(diag_lane));
}

int main(int argc, char **argv)
{
    struct idma_stream v, a;
    struct idma_batch_start start;
    u64 end = 0;
    int ret;
    const char *name;

    assert(argc == 2);
    name = argv[1];
    reset_state();

    if (!strcmp(name, "order_both")) {
        unsigned first_trigger = (unsigned)-1, second_trigger = (unsigned)-1;
        unsigned a_conf = (unsigned)-1, v_conf = (unsigned)-1, i;

        setup_streams(&v, &a, true);
        a.diagnostic = &diag_lane[1];
        v.diagnostic = &diag_lane[0];
        no_commit_before_v_trigger = true;
        commit_watch = &a;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == 0);
        assert(start.attempted && start.a_triggered && start.v_triggered);
        assert(start.a_id == 101 && start.v_id == 101);
        assert(start.a_begin_ns && start.a_end_ns && start.v_begin_ns && start.v_end_ns);
        assert(start.a_begin_ns <= start.a_end_ns);
        assert(start.a_end_ns <= start.v_begin_ns);
        assert(start.v_begin_ns <= start.v_end_ns);
        assert(start.irq_span_ns == start.v_end_ns - start.a_begin_ns);
        assert(a.in_flight && v.in_flight);
        assert(a.transfer_id == start.a_id && v.transfer_id == start.v_id);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        assert(!a.retain && !v.retain);
        /* Commit bookkeeping and logging stay outside the trigger section. */
        assert(!log_calls && !crit_calls);
        assert(a.diagnostic->launch_end_ns == start.a_end_ns);
        assert(v.diagnostic->launch_end_ns == start.v_end_ns);

        for (i = 0; i < mmio_count; i++) {
            if (mmio_events[i].op == MMIO_READ &&
                mmio_events[i].offset == IDMA_NEXT_ID_REG_OFFSET) {
                first_trigger = i;
                break;
            }
        }
        assert(first_trigger < mmio_count);
        for (i = 0; i < first_trigger; i++) {
            if (mmio_events[i].op == MMIO_WRITE &&
                mmio_events[i].dev == 1 &&
                mmio_events[i].offset == IDMA_SRC_ADDR_REG_OFFSET)
                a_conf = i;
            if (mmio_events[i].op == MMIO_WRITE &&
                mmio_events[i].dev == 0 &&
                mmio_events[i].offset == IDMA_SRC_ADDR_REG_OFFSET)
                v_conf = i;
        }
        /* Index 0 is a real A configuration write, never a "not found" value. */
        assert(a_conf != (unsigned)-1 && v_conf != (unsigned)-1);
        assert(a_conf == 0 && v_conf == 4 && first_trigger == 8);
        assert(a_conf < v_conf && v_conf < first_trigger);
        assert(mmio_events[a_conf].value == a.src_iova);
        assert(mmio_events[v_conf].value == v.src_iova);
        assert(mmio_events[first_trigger].dev == 1);
        second_trigger = first_trigger + 1;
        assert(mmio_count == first_trigger + 2);
        assert(mmio_events[second_trigger].op == MMIO_READ &&
               mmio_events[second_trigger].offset == IDMA_NEXT_ID_REG_OFFSET &&
               mmio_events[second_trigger].dev == 0);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "a_zero_skip_v")) {
        unsigned i;

        setup_streams(&v, &a, true);
        a.diagnostic = &diag_lane[1];
        v.diagnostic = &diag_lane[0];
        hw[1].zero_id = true;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == -EIO);
        assert(start.attempted && start.a_triggered && !start.v_triggered);
        assert(start.a_id == 0);
        assert(start.irq_span_ns == start.a_end_ns - start.a_begin_ns);
        assert(a.retain && idmas[1].quarantined);
        assert(!v.retain && !idmas[0].quarantined);
        assert(a.batch_started);
        /* The untriggered V claim and its launch stamps are released. */
        assert(!v.batch_started);
        assert(!v.diagnostic->launch_begin_ns && !v.diagnostic->launch_end_ns);
        /* A failed trigger publishes no ID and no completion. */
        assert(!a.transfer_id && !a.in_flight);
        assert(a.diagnostic->launch_begin_ns && !a.diagnostic->launch_end_ns);
        /* Quarantine logging only after the protected section has closed. */
        assert(crit_calls == 1 && !log_calls);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        for (i = 0; i < mmio_count; i++)
            assert(!(mmio_events[i].op == MMIO_READ &&
                     mmio_events[i].dev == 0 &&
                     mmio_events[i].offset == IDMA_NEXT_ID_REG_OFFSET));
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "v_zero_after_a")) {
        setup_streams(&v, &a, true);
        a.diagnostic = &diag_lane[1];
        v.diagnostic = &diag_lane[0];
        hw[0].zero_id = true;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == -EIO);
        assert(start.attempted && start.a_triggered && start.v_triggered);
        assert(start.v_id == 0 && start.a_id);
        /*
         * A was triggered with a valid ID and is published before V is
         * retained, so the caller's STOP/drain of A stays safe.
         */
        assert(a.in_flight && !a.retain && a.transfer_id == start.a_id);
        assert(!idmas[1].quarantined);
        assert(a.diagnostic->launch_end_ns == start.a_end_ns);
        /* V never produced an ID: retained, unknown, not in flight. */
        assert(v.retain && idmas[0].quarantined);
        assert(v.batch_started && !v.in_flight && !v.transfer_id);
        assert(v.diagnostic->launch_begin_ns && !v.diagnostic->launch_end_ns);
        assert(crit_calls == 1 && !log_calls);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "solo_timestamps")) {
        setup_streams(&v, &a, false);
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
        start.has_a = false;
        ret = idma_batch_start(&v, NULL, now + 1000000, &end, &start);
        assert(ret == 0);
        assert(start.attempted && !start.a_triggered && start.v_triggered);
        assert(!start.a_begin_ns && !start.a_end_ns);
        assert(start.v_begin_ns && start.v_end_ns);
        assert(start.irq_span_ns == start.v_end_ns - start.v_begin_ns);
        assert(!start.a_id && start.v_id);
        assert(v.in_flight);
        assert(!irq_disabled_depth && !preempt_disabled_depth);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "deadline_no_trigger")) {
        setup_streams(&v, &a, true);
        memset(&start, 0, sizeof(start));
        ret = idma_batch_start(&v, &a, now - 1, &end, &start);
        assert(ret == -ETIMEDOUT);
        assert(!start.attempted && !start.a_triggered && !start.v_triggered);
        assert(!a.batch_started && !v.batch_started);
        assert(!a.in_flight && !v.in_flight);
        assert(!irq_disabled_depth && !preempt_disabled_depth);
        assert(mmio_count == 0);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "signal_no_trigger")) {
        setup_streams(&v, &a, true);
        signal_flag = true;
        memset(&start, 0, sizeof(start));
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == -EINTR);
        assert(!start.attempted && !start.a_triggered && !start.v_triggered);
        assert(!a.batch_started && !v.batch_started);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        assert(mmio_count == 0);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "deadline_after_config")) {
        unsigned i;

        setup_streams(&v, &a, true);
        v.diagnostic = &diag_lane[0];
        a.diagnostic = &diag_lane[1];
        /*
         * The V configuration CONF write is the last write before the trigger
         * section; stalling it must expire the rechecked deadline before
         * either NEXT_ID read happens.
         */
        stall_after_conf_write = 2;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == -ETIMEDOUT);
        assert(start.attempted && !start.a_triggered && !start.v_triggered);
        assert(!start.a_id && !start.v_id && !start.irq_span_ns);
        /* Both untriggered claims and their stamps must be released. */
        assert(!a.batch_started && !v.batch_started);
        assert(!a.diagnostic->launch_begin_ns && !a.diagnostic->launch_end_ns);
        assert(!v.diagnostic->launch_begin_ns && !v.diagnostic->launch_end_ns);
        assert(!a.in_flight && !v.in_flight);
        assert(!a.retain && !v.retain);
        assert(!idmas[0].quarantined && !idmas[1].quarantined);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        assert(!log_calls && !crit_calls);
        assert(conf_writes == 2);
        for (i = 0; i < mmio_count; i++)
            assert(mmio_events[i].offset != IDMA_NEXT_ID_REG_OFFSET);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "signal_after_config")) {
        unsigned i;

        setup_streams(&v, &a, false);
        v.diagnostic = &diag_lane[0];
        signal_after_conf_write = 1;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
        start.has_a = false;
        ret = idma_batch_start(&v, NULL, now + 1000000, &end, &start);
        assert(ret == -EINTR);
        assert(start.attempted && !start.a_triggered && !start.v_triggered);
        assert(!start.v_id && !start.irq_span_ns);
        assert(!v.batch_started);
        assert(!v.diagnostic->launch_begin_ns && !v.diagnostic->launch_end_ns);
        assert(!v.in_flight && !v.retain && !idmas[0].quarantined);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        assert(!log_calls && !crit_calls);
        assert(conf_writes == 1);
        for (i = 0; i < mmio_count; i++)
            assert(mmio_events[i].offset != IDMA_NEXT_ID_REG_OFFSET);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "irq_restore_paths")) {
        /*
         * (a) IRQs start enabled and both triggers are valid.  The IRQ injected
         * during the A trigger read stays pending through the V trigger and is
         * only taken when interrupts are restored.
         */
        setup_streams(&v, &a, true);
        inject_irq_on_a_trigger = true;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == 0);
        assert(start.a_id && start.v_id && start.irq_span_ns);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        assert(irq_serviced && !irq_pending);
        assert(irq_service_ns == last_restore_ns);
        assert(a.in_flight && v.in_flight && !a.retain && !v.retain);

        reset_state();

        /*
         * (b) IRQs start enabled and the A trigger fails: quarantine logging
         * must happen after the protected section has been restored.
         */
        setup_streams(&v, &a, true);
        inject_irq_on_a_trigger = true;
        hw[1].zero_id = true;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == -EIO);
        assert(a.retain && idmas[1].quarantined && !v.retain);
        assert(crit_calls == 1 && !log_calls);
        assert(!irq_disabled_depth && !driver_irq_depth && !preempt_disabled_depth);
        assert(irq_serviced && !irq_pending);
        assert(irq_service_ns == last_restore_ns);

        reset_state();

        /*
         * (c) The caller already had IRQs disabled: the exact prior depth must
         * be restored, the injected IRQ must stay unserviced, and the A
         * quarantine log must still not run while the driver owns its window.
         */
        setup_streams(&v, &a, true);
        inject_irq_on_a_trigger = true;
        hw[1].zero_id = true;
        irq_disabled_depth = 1;
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == -EIO);
        assert(a.retain && idmas[1].quarantined);
        assert(crit_calls == 1 && !driver_irq_depth && !preempt_disabled_depth);
        assert(irq_disabled_depth == 1);
        assert(irq_pending && !irq_serviced && !irq_service_ns);
        printf("PASS %s\n", name);
        return 0;
    }

    if (!strcmp(name, "log_contract")) {
        setup_streams(&v, &a, true);
        memset(&start, 0, sizeof(start));
        start.v_did = 10;
        start.a_did = 11;
        start.has_a = true;
        ret = idma_batch_start(&v, &a, now + 1000000, &end, &start);
        assert(ret == 0);
        idma_batch_start_log(&devices[0], &start, 42, 0);
        assert(log_calls == 1 && !crit_calls);
        assert(strstr(log_buffer, "idma_batch_start version=1 seq=42 method=irq_pair"));
        assert(strstr(log_buffer, "v_did=10 a_did=11 attempted=1 has_a=1 a_triggered=1 v_triggered=1"));
        assert(strstr(log_buffer, "a_id=101 v_id=101"));
        assert(strstr(log_buffer, "irq_span_ns="));
        assert(strstr(log_buffer, "a_to_v_ns="));
        assert(strstr(log_buffer, "error=0"));
        printf("PASS %s\n", name);
        return 0;
    }

    assert(!"unknown test");
    return 1;
}
