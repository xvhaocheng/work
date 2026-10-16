// SPDX-License-Identifier: GPL-2.0
#include <linux/capability.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/iommu.h>
#include <linux/iopoll.h>
#include <linux/irqflags.h>
#include <linux/kernel.h>
#include <linux/preempt.h>
#include <linux/ktime.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/riscv_iommu_hpm.h>
#include <linux/sched/signal.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <uapi/linux/idma.h>
#include <asm/cacheflush.h>

MODULE_DESCRIPTION("CVA6 iDMA versioned two-device experiment driver");
MODULE_AUTHOR("Manuel Rodriguez <manuel.cederog@gmail.com>");
MODULE_ALIAS("idma");
MODULE_LICENSE("GPL v2");

#define IDMA_SRC_ADDR_REG_OFFSET 0x00
#define IDMA_DST_ADDR_REG_OFFSET 0x08
#define IDMA_NUM_BYTES_REG_OFFSET 0x10
#define IDMA_CONF_REG_OFFSET 0x18
#define IDMA_STATUS_REG_OFFSET 0x20
#define IDMA_NEXT_ID_REG_OFFSET 0x28
#define IDMA_DONE_REG_OFFSET 0x30
#define IDMA_ACTIVE_CYCLES_REG_OFFSET 0x50
#define IDMA_OBS_MAGIC_OFFSET 0x58
#define IDMA_OBS_CONTROL_OFFSET 0x60
#define IDMA_OBS_TARGET_OFFSET 0x68
#define IDMA_OBS_INDEX_OFFSET 0x70
#define IDMA_OBS_DATA_OFFSET 0x78
#define IDMA_OBS_MAGIC 0x49444d414f425301ULL
#define IDMA_OBS_MAGIC_V2 0x49444d414f425302ULL
#define IDMA_OBS_MAGIC_V3 0x49444d414f425303ULL
#define IDMA_OBS_MAGIC_V4 0x49444d414f425304ULL
#define IDMA_OBS_MAGIC_V5 0x49444d414f425305ULL
#define IDMA_OBS_MAGIC_V6 0x49444d414f425306ULL
#define IDMA_OBS_MAGIC_V7 0x49444d414f425307ULL
#define IDMA_OBS_MAGIC_V8 0x49444d414f425308ULL
#define IDMA_OBS_MAGIC_V9 0x49444d414f425309ULL /* P4M/composition, no path/E3 */
#define IDMA_E3_MAGIC 0x49444d4145330001ULL
#define IDMA_E3_MAGIC_V2 0x49444d4145330002ULL
#define IDMA_OBS_VALUES 34
#define IDMA_P4M_BANK_BASE 448
#define IDMA_P4M_BANK_MAGIC 0x49444d4150344d01ULL
#define IDMA_BATCH_MAGIC_OFFSET 0x80
#define IDMA_BATCH_CONTROL_OFFSET 0x88
#define IDMA_BATCH_TARGET_OFFSET 0x90
#define IDMA_BATCH_PAGES_OFFSET 0x98
#define IDMA_BATCH_COMPLETED_OFFSET 0xa0
#define IDMA_BATCH_ISSUED_OFFSET 0xa8
#define IDMA_BATCH_MAGIC 0x49444d4142544301ULL
#define IDMA_XFER_TIMEOUT_US 2000000
#define IDMA_MAX_DEVS 8
#define IDMA_MAX_PAGES 4096U
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U

/* DMA0 launcher register block; separate from observer registers. */
#define IDMA_LAUNCHER_MAGIC_OFFSET 0xb0
#define IDMA_LAUNCHER_CONTROL_OFFSET 0xb8
#define IDMA_LAUNCHER_INDEX_OFFSET 0xc0
#define IDMA_LAUNCHER_DATA_OFFSET 0xc8
#define IDMA_LAUNCHER_MAGIC 0x49444d414c4e4301ULL
#define IDMA_LAUNCHER_PREP 1ULL
#define IDMA_LAUNCHER_PREP_HAS_A 0x100ULL
#define IDMA_LAUNCHER_GO 2ULL
#define IDMA_LAUNCHER_ABORT 4ULL
#define IDMA_LAUNCHER_CLEAR 8ULL

/* BatchPort gap slots carved out before launcher/global decode. */
#define IDMA_BATCH_GAP_OFFSET 0xd0
#define IDMA_BATCH_GAP_MAGIC_OFFSET 0xd8
#define IDMA_BATCH_GAP_MAGIC 0x49444d4147415001ULL

#define IDMA_LAUNCHER_STATUS_PREP BIT_ULL(0)
#define IDMA_LAUNCHER_STATUS_RUNNING BIT_ULL(1)
#define IDMA_LAUNCHER_STATUS_FROZEN BIT_ULL(2)
#define IDMA_LAUNCHER_STATUS_ABORTED BIT_ULL(3)
#define IDMA_LAUNCHER_HELD_SHIFT 8
#define IDMA_LAUNCHER_ACTIVE_SHIFT 12
#define IDMA_LAUNCHER_ERROR_SHIFT 32
#define IDMA_LAUNCHER_HELD_MASK (0xfULL << IDMA_LAUNCHER_HELD_SHIFT)
#define IDMA_LAUNCHER_ACTIVE_MASK (0xfULL << IDMA_LAUNCHER_ACTIVE_SHIFT)
#define IDMA_LAUNCHER_ERROR_MASK (0xffffffffULL << IDMA_LAUNCHER_ERROR_SHIFT)

#define IDMA_CONTROLLED_STAGE_FLUSH 1
#define IDMA_CONTROLLED_STAGE_APRIME 2
#define IDMA_CONTROLLED_STAGE_VPRIME 3
#define IDMA_CONTROLLED_STAGE_VPROBE 4
#define IDMA_CONTROLLED_STAGE_PREPARE 5
#define IDMA_CONTROLLED_STAGE_RUNNING 6
#define IDMA_CONTROLLED_STAGE_DRAIN 7
#define IDMA_CONTROLLED_STAGE_COMPLETE 8

/* Shared AXI identity of the A read selected by the IOMMU. */
#define IDMA_CONTROLLED_A_SELECT_ID 4U
#define IDMA_CONTROLLED_A_SELECT_DID 11U

struct idma_device {
	struct miscdevice miscdev;
	char name[16];
	int id;
	u32 did;
	void __iomem *reg;
	bool quarantined;
};

struct idma_stream {
	struct idma_device *idma;
	struct device *dev;
	struct iommu_domain *domain;
	void *src, *dst;
	dma_addr_t src_dma, dst_dma;
	unsigned long src_iova, dst_iova;
	size_t size, offset, next_offset, touched_end, touched_tail;
	u32 chunk;
	u64 transfer_id, deadline_ns, last_complete_ns;
	bool mapped_src, mapped_dst, in_flight, retain;
	bool batch_configured, batch_started, reverse;
	bool composition_trace;
	struct idma_experiment_stats *stats;
	struct idma_e3_diag_lane *diagnostic;
};

struct idma_batch_start {
	bool attempted;
	bool has_a;
	bool a_triggered;
	bool v_triggered;
	u32 v_did, a_did;
	u64 a_id, v_id;
	u64 a_begin_ns, a_end_ns;
	u64 v_begin_ns, v_end_ns;
	u64 irq_span_ns;
};

static DEFINE_MUTEX(idma_devices_lock);
static struct idma_device *idma_devices[IDMA_MAX_DEVS];
/* All experiment preparation, IOVA mappings and DMA execution are serialized. */
static DEFINE_MUTEX(idma_experiment_lock);

static int idma_reserve_instance_id(void)
{
	int id;

	mutex_lock(&idma_devices_lock);
	for (id = 0; id < IDMA_MAX_DEVS; id++) {
		if (!idma_devices[id]) {
			idma_devices[id] = ERR_PTR(-EBUSY);
			mutex_unlock(&idma_devices_lock);
			return id;
		}
	}
	mutex_unlock(&idma_devices_lock);

	return -ENOSPC;
}

static void idma_publish_instance(struct idma_device *idma_dev)
{
	mutex_lock(&idma_devices_lock);
	if (idma_dev->id >= 0 && idma_dev->id < IDMA_MAX_DEVS)
		idma_devices[idma_dev->id] = idma_dev;
	mutex_unlock(&idma_devices_lock);
}

static void idma_unregister_instance(struct idma_device *idma_dev)
{
	mutex_lock(&idma_devices_lock);
	if (idma_dev->id >= 0 && idma_dev->id < IDMA_MAX_DEVS &&
	    (idma_devices[idma_dev->id] == idma_dev ||
	     IS_ERR(idma_devices[idma_dev->id])))
		idma_devices[idma_dev->id] = NULL;
	mutex_unlock(&idma_devices_lock);
}

static struct idma_device *idma_get_instance(u32 id)
{
	struct idma_device *idma_dev;

	if (id >= IDMA_MAX_DEVS)
		return NULL;

	mutex_lock(&idma_devices_lock);
	idma_dev = idma_devices[id];
	if (IS_ERR(idma_dev))
		idma_dev = NULL;
	if (idma_dev)
		get_device(idma_dev->miscdev.parent);
	mutex_unlock(&idma_devices_lock);

	return idma_dev;
}

static void idma_put_instance(struct idma_device *idma_dev)
{
	if (idma_dev)
		put_device(idma_dev->miscdev.parent);
}

static int idma_open(struct inode *inode, struct file *fp)
{
	struct miscdevice *miscdev = fp->private_data;

	fp->private_data = container_of(miscdev, struct idma_device, miscdev);
	return 0;
}

static int idma_release(struct inode *inode, struct file *fp)
{
	return 0;
}


static bool idma_iova_range_free(struct iommu_domain *domain,
				 unsigned long iova, size_t size)
{
	size_t off;

	for (off = 0; off < size; off += PAGE_SIZE) {
		if (iommu_iova_to_phys(domain, iova + off))
			return false;
	}

	return true;
}

static int idma_find_iova(struct iommu_domain *domain, size_t size,
			  unsigned long *src_iova, unsigned long *dst_iova)
{
	unsigned long start = domain->geometry.aperture_start;
	unsigned long end = domain->geometry.aperture_end;
	unsigned long iova;
	size_t span = size * 2;

	if (end < start || span < size || span - 1 > end - start)
		return -EBUSY;

	iova = ALIGN_DOWN(end - span + 1, SZ_2M);
	for (;;) {
		if (!idma_iova_range_free(domain, iova, span)) {
			if (iova < start + SZ_2M)
				break;
			iova -= SZ_2M;
			continue;
		}

		*src_iova = iova;
		*dst_iova = iova + size;
		return 0;
	}

	return -EBUSY;
}

static int idma_iommu_map_range(struct iommu_domain *domain,
				unsigned long iova, phys_addr_t phys,
				size_t size, size_t pgsize)
{
	size_t off;
	int ret;

	for (off = 0; off < size; off += pgsize) {
		ret = iommu_map(domain, iova + off, phys + off, pgsize,
				IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
		if (ret) {
			while (off) {
				off -= pgsize;
				iommu_unmap(domain, iova + off, pgsize);
			}
			return ret;
		}
	}

	return 0;
}

static void idma_iommu_unmap_range(struct iommu_domain *domain,
				   unsigned long iova, size_t size,
				   size_t pgsize)
{
	size_t off;

	for (off = 0; off < size; off += pgsize)
		iommu_unmap(domain, iova + off, pgsize);
}


static int idma_read_did(struct platform_device *pdev, u32 *did)
{
	struct of_phandle_args args;
	int ret;

	ret = of_parse_phandle_with_args(pdev->dev.of_node, "iommus",
					 "#iommu-cells", 0, &args);
	if (ret)
		return ret;
	if (args.args_count != 1 || args.args[0] >= BIT(24))
		ret = -EINVAL;
	else
		*did = args.args[0];
	of_node_put(args.np);
	return ret;
}

static struct page *idma_buffer_page(void *ptr)
{
	return is_vmalloc_addr(ptr) ? vmalloc_to_page(ptr) : virt_to_page(ptr);
}

/* Each byte distinguishes the DMA instance; words also encode position. */
static u64 idma_pattern(struct idma_stream *s, size_t word)
{
	return (0xa53c691df027b84eULL ^
		((u64)s->idma->id * 0x0101010101010101ULL) ^
		((u64)word * 0x9e3779b97f4a7c15ULL));
}

static void idma_stream_cleanup(struct idma_stream *s)
{
	if (!s->dev || s->retain)
		return;
	if (s->mapped_dst)
		idma_iommu_unmap_range(s->domain, s->dst_iova, s->size, PAGE_SIZE);
	if (s->mapped_src)
		idma_iommu_unmap_range(s->domain, s->src_iova, s->size, PAGE_SIZE);
	if (s->dst)
		dma_free_attrs(s->dev, s->size, s->dst, s->dst_dma,
			       DMA_ATTR_FORCE_CONTIGUOUS);
	if (s->src)
		dma_free_attrs(s->dev, s->size, s->src, s->src_dma,
			       DMA_ATTR_FORCE_CONTIGUOUS);
}

/*
 * Partial-failure safe: fields are assigned as resources are acquired, and the
 * caller always runs idma_stream_cleanup() on this stream afterwards, which
 * tolerates src-only, dst-only and unmapped states.
 */
static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	struct page *src_page, *dst_page;
	size_t i;
	int ret;

	s->idma = idma;
	s->dev = idma->miscdev.parent;
	s->stats = stats;
	s->domain = iommu_get_domain_for_dev(s->dev);
	if (!s->domain || s->domain->type == IOMMU_DOMAIN_IDENTITY)
		return -EOPNOTSUPP;
	s->size = (size_t)pages * PAGE_SIZE;
	s->chunk = chunk;
	s->src = dma_alloc_attrs(s->dev, s->size, &s->src_dma, GFP_KERNEL,
				 DMA_ATTR_FORCE_CONTIGUOUS);
	if (!s->src)
		return -ENOMEM;
	s->dst = dma_alloc_attrs(s->dev, s->size, &s->dst_dma, GFP_KERNEL,
				 DMA_ATTR_FORCE_CONTIGUOUS);
	if (!s->dst)
		return -ENOMEM;
	for (i = 0; i < s->size / sizeof(u64); i++) {
		((u64 *)s->src)[i] = idma_pattern(s, i);
		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
	}
	src_page = idma_buffer_page(s->src);
	dst_page = idma_buffer_page(s->dst);
	if (!src_page || !dst_page)
		return -EFAULT;
	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
	if (ret)
		return ret;
	ret = idma_iommu_map_range(s->domain, s->src_iova,
				   page_to_phys(src_page), s->size, PAGE_SIZE);
	if (ret)
		return ret;
	s->mapped_src = true;
	ret = idma_iommu_map_range(s->domain, s->dst_iova,
				   page_to_phys(dst_page), s->size, PAGE_SIZE);
	if (ret)
		return ret;
	s->mapped_dst = true;
	return 0;
}

static void idma_quarantine(struct idma_stream *s)
{
	s->retain = true;
	WRITE_ONCE(s->idma->quarantined, true);
	if (s->stats)
		s->stats->quarantined = 1;
	dev_crit(s->dev, "experiment DMA not safely drained; buffers retained until reboot\n");
}

static void idma_submit_config(struct idma_stream *s)
{
	if (s->diagnostic && s->batch_started)
		s->diagnostic->launch_begin_ns = ktime_get_ns();
	s->offset = s->next_offset;
	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
}

static void idma_submit_commit(struct idma_stream *s, u64 id, bool measured, u64 now)
{
	s->transfer_id = id;
	if (s->diagnostic) {
		if (s->batch_started)
			s->diagnostic->launch_end_ns = now;
		else
			s->diagnostic->warm_submitted++;
	}
	if (s->last_complete_ns && measured) {
		u64 gap = now - s->last_complete_ns;

		s->stats->refill_gap_ns_sum += gap;
		s->stats->refill_gap_ns_max = max(s->stats->refill_gap_ns_max, gap);
		s->stats->refill_gap_samples++;
	}
	s->last_complete_ns = 0;
	s->deadline_ns = now + (u64)IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;
	s->in_flight = true;
	s->next_offset += s->chunk;
	if (s->next_offset == s->size)
		s->next_offset = 0;
	if (measured)
		s->stats->submitted++;
}

static int idma_submit(struct idma_stream *s, bool measured)
{
	u64 now, id;

	idma_submit_config(s);
	id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
	if (!id) {
		/*
		 * Without a valid ID we cannot prove that buffers are no longer
		 * used.  A failed submit must not keep the previous transfer ID,
		 * because the drain path matches DONE against transfer_id.
		 */
		s->transfer_id = 0;
		idma_quarantine(s);
		return -EIO;
	}
	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
	now = ktime_get_ns();
	idma_submit_commit(s, id, measured, now);
	return 0;
}

static bool idma_composition_trace;
module_param_named(composition_trace, idma_composition_trace, bool, 0644);
MODULE_PARM_DESC(composition_trace, "Trace composition stages (default false)");

static void idma_comp_stage(bool enabled, struct device *dev, const char *stage,
			    int ret)
{
	if (enabled)
		dev_info(dev, "idma_comp stage=%s ret=%d\n", stage, ret);
}

/*
 * Bounded software batch start.  Both register sets are configured before any
 * NEXT_ID trigger.  Only the ordered A-then-V trigger reads and their software
 * timestamps run with IRQs and preemption disabled.  That section is
 * non-sleeping and never covers commit bookkeeping, polling, drain,
 * quarantine, allocation, logging or cond_resched.
 *
 * The deadline and the pending signal are checked twice: once before the
 * configuration writes and again inside the protected section before the first
 * trigger.  A request that expires in between is dropped without triggering
 * either device, without keeping either untriggered batch_started claim and
 * without keeping their launch stamps.
 *
 * a_triggered/v_triggered mean the corresponding NEXT_ID read was executed,
 * even when it returned zero.  a_id/v_id carry the raw readback (zero on
 * failure).  irq_span_ns is the ktime bracket around the protected trigger
 * reads; the real IRQ-off interval extends a bounded few instructions on both
 * sides and is not a cycle-accurate or interrupt-latency measurement.
 *
 * Failure ownership matches the ordinary batch path: a stream whose trigger
 * read produced a valid ID is committed even when the other stream failed, so
 * the caller can still stop and drain it; a stream whose trigger read returned
 * zero is quarantined, and an untriggered V is released instead.
 */
static int idma_batch_start(struct idma_stream *v, struct idma_stream *a,
			    u64 deadline, u64 *end_ns,
			    struct idma_batch_start *start)
{
	unsigned long flags;
	u64 a_id = 0, v_id = 0;
	int ret;

	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}

	/*
	 * Ownership and configuration are outside the protected trigger section.
	 * Mark batch_started before the trigger so failure handling keeps the
	 * same STOP/drain retention ownership as the ordinary batch path.
	 */
	if (a) {
		a->batch_started = true;
		idma_submit_config(a);
	}
	v->batch_started = true;
	idma_submit_config(v);

	start->attempted = true;
	idma_comp_stage(v->composition_trace, v->dev, "pair-enter", 0);
	preempt_disable();
	local_irq_save(flags);

	/*
	 * Configuration consumed time: recheck both conditions before the first
	 * NEXT_ID read so an expired or interrupted request never triggers either
	 * device.
	 */
	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out_untriggered;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out_untriggered;
	}

	if (a) {
		start->a_begin_ns = ktime_get_ns();
		a_id = readq(a->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
		start->a_end_ns = ktime_get_ns();
		start->a_triggered = true;
		start->a_id = a_id;
		if (!a_id) {
			/*
			 * V was only configured, never triggered: release its
			 * ownership and stamps, then log A's quarantine with
			 * interrupts restored.
			 */
			local_irq_restore(flags);
			preempt_enable();
			v->batch_started = false;
			if (v->diagnostic) {
				v->diagnostic->launch_begin_ns = 0;
				v->diagnostic->launch_end_ns = 0;
			}
			idma_quarantine(a);
			ret = -EIO;
			goto out;
		}
	}

	start->v_begin_ns = ktime_get_ns();
	v_id = readq(v->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
	start->v_end_ns = ktime_get_ns();
	start->v_triggered = true;
	start->v_id = v_id;

	/* Commit bookkeeping never runs with IRQs or preemption disabled. */
	local_irq_restore(flags);
	preempt_enable();
	idma_comp_stage(v->composition_trace, v->dev, "pair-return", 0);

	if (!v_id) {
		/*
		 * A was triggered with a valid ID: publish it before V is
		 * quarantined so the caller can still stop and drain A.
		 */
		if (a)
			idma_submit_commit(a, a_id, false, start->a_end_ns);
		idma_quarantine(v);
		ret = -EIO;
		goto out;
	}

	if (a)
		idma_submit_commit(a, a_id, false, start->a_end_ns);
	idma_submit_commit(v, v_id, false, start->v_end_ns);

	start->irq_span_ns = start->v_end_ns -
		(a ? start->a_begin_ns : start->v_begin_ns);
	return 0;

out_untriggered:
	/* Nothing was triggered: drop both ownership claims and their stamps. */
	if (a) {
		a->batch_started = false;
		if (a->diagnostic) {
			a->diagnostic->launch_begin_ns = 0;
			a->diagnostic->launch_end_ns = 0;
		}
	}
	v->batch_started = false;
	if (v->diagnostic) {
		v->diagnostic->launch_begin_ns = 0;
		v->diagnostic->launch_end_ns = 0;
	}
	local_irq_restore(flags);
	preempt_enable();

out:
	if (start->attempted) {
		if (start->a_triggered && start->v_triggered)
			start->irq_span_ns = start->v_end_ns - start->a_begin_ns;
		else if (start->a_triggered)
			start->irq_span_ns = start->a_end_ns - start->a_begin_ns;
		else if (start->v_triggered)
			start->irq_span_ns = start->v_end_ns - start->v_begin_ns;
	}
	*end_ns = ktime_get_ns();
	return ret;
}

/*
 * Log contract (version 1), emitted only after the observation finisher and
 * all error aggregation.  Timestamps are software ktime brackets around the
 * protected trigger reads; they are not hardware issue timestamps, cycle
 * accurate or an interrupt-latency proof.  a_triggered/v_triggered mean the
 * corresponding NEXT_ID read was attempted; a_id/v_id are the raw readbacks
 * and may be zero on failure.  a_to_v_ns is valid only when both A and V
 * trigger reads were attempted.  A start that failed after configuration but
 * before any trigger reports attempted=1 with both trigger flags zero and
 * irq_span_ns=0.
 */
static void idma_batch_start_log(struct device *dev,
				 const struct idma_batch_start *start,
				 u64 sequence, int error)
{
	u64 a_to_v_ns = 0;

	if (start->has_a && start->a_triggered && start->v_triggered)
		a_to_v_ns = start->v_begin_ns - start->a_begin_ns;
	dev_info(dev,
		 "idma_batch_start version=1 seq=%llu method=irq_pair v_did=%u a_did=%u attempted=%u has_a=%u a_triggered=%u v_triggered=%u a_id=%llu v_id=%llu a_begin_ns=%llu a_end_ns=%llu v_begin_ns=%llu v_end_ns=%llu irq_span_ns=%llu a_to_v_ns=%llu error=%d\n",
		 sequence, start->v_did, start->a_did, start->attempted,
		 start->has_a, start->a_triggered, start->v_triggered,
		 start->a_id, start->v_id, start->a_begin_ns, start->a_end_ns,
		 start->v_begin_ns, start->v_end_ns, start->irq_span_ns,
		 a_to_v_ns, error);
}

/* 1: completion observed; 0: pending; negative: failed. Never refills itself. */
static int idma_poll(struct idma_stream *s, bool measured)
{
	u64 done;

	if (!s->in_flight)
		return 0;
	done = readq(s->idma->reg + IDMA_DONE_REG_OFFSET);
	if (done != s->transfer_id)
		return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;
	s->last_complete_ns = ktime_get_ns();
	if (s->diagnostic && !s->batch_started)
		s->diagnostic->warm_completed++;
	s->in_flight = false;
	if (!s->batch_configured)
		s->touched_end = max(s->touched_end, s->offset + s->chunk);
	if (measured) {
		s->stats->completed++;
		s->stats->completed_bytes += s->chunk;
	}
	return 1;
}

static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
			   u64 count, u64 deadline, bool measured, u64 *end_ns)
{
	u64 completed = 0;
	u32 spins = 0;
	int ret;

	if (!count)
		return 0;
	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}
	ret = idma_submit(v, measured);
	if (ret)
		goto out;
	for (;;) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			break;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		ret = idma_poll(v, measured);
		if (ret < 0)
			break;
		if (ret == 1) {
			if (++completed == count) {
				/* Capture V's end before polling/refilling A or taking snapshots. */
				*end_ns = v->last_complete_ns;
				return 0;
			}
			ret = idma_submit(v, measured);
			if (ret)
				break;
		}
		if (a) {
			ret = idma_poll(a, measured);
			if (ret < 0)
				break;
			if (ret == 1) {
				ret = idma_submit(a, measured);
				if (ret)
					break;
			}
		}
		if (!(++spins & 1023))
			cond_resched();
		cpu_relax();
	}
 out:
	*end_ns = ktime_get_ns();
	return ret;
}

static int idma_drain(struct idma_stream *s)
{
	u64 done;
	int ret;

	if (!s->in_flight)
		return s->retain ? -EIO : 0;
	ret = readq_poll_timeout(s->idma->reg + IDMA_DONE_REG_OFFSET, done,
				 done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);
	if (ret) {
		idma_quarantine(s);
		return ret;
	}
	s->in_flight = false;
	if (s->diagnostic && !s->batch_started)
		s->diagnostic->warm_drained++;
	if (!s->batch_configured)
		s->touched_end = max(s->touched_end, s->offset + s->chunk);
	return 0;
}

static int idma_verify(struct idma_stream *s)
{
	size_t i;

	if (s->retain)
		return -EIO;
	/* Verify untouched bytes as well, and source integrity independently. */
	for (i = 0; i < s->size; i++) {
		u64 pattern = idma_pattern(s, i / sizeof(u64));
		u8 value = ((u8 *)&pattern)[i % sizeof(u64)];
		u8 expected = (i < s->touched_end ||
			       i >= s->size - s->touched_tail) ? value : (u8)~value;
		u8 src = READ_ONCE(((u8 *)s->src)[i]);
		u8 dst = READ_ONCE(((u8 *)s->dst)[i]);

		if (src != value || dst != expected) {
			phys_addr_t src_pa, dst_pa, src_pte_pa, dst_pte_pa;
			u8 src_after, dst_after;

			/*
			 * On this CVA6, FENCE.I also invalidates the WT data cache.
			 * Probe only after failure and safe drain; preserve the failure.
			 * This is not a generic DMA cache-coherency fix.
			 */
			local_flush_icache_all();
			src_after = READ_ONCE(((u8 *)s->src)[i]);
			dst_after = READ_ONCE(((u8 *)s->dst)[i]);

			src_pa = page_to_phys(idma_buffer_page(s->src + i)) +
				 offset_in_page(s->src + i);
			dst_pa = page_to_phys(idma_buffer_page(s->dst + i)) +
				 offset_in_page(s->dst + i);
			src_pte_pa = iommu_iova_to_phys(s->domain, s->src_iova + i);
			dst_pte_pa = iommu_iova_to_phys(s->domain, s->dst_iova + i);

			s->stats->data_errors++;
			dev_err(s->dev,
				"experiment data mismatch: id=%d did=%u offset=%zu touched_end=%zu size=%zu chunk=%u src=%02x src_after_fence_i=%02x expected_src=%02x dst=%02x dst_after_fence_i=%02x expected_dst=%02x src_iova=%#lx dst_iova=%#lx src_pa=%pa dst_pa=%pa src_pte_pa=%pa dst_pte_pa=%pa\n",
				s->idma->id, s->idma->did, i, s->touched_end,
				s->size, s->chunk, src, src_after, value,
				dst, dst_after, expected,
				s->src_iova + i, s->dst_iova + i, &src_pa,
				&dst_pa, &src_pte_pa, &dst_pte_pa);
			return -EILSEQ;
		}
	}
	return 0;
}

/* Batch mode still uses DEFAULT read/write copies. STOP never cancels AXI. */
static int idma_batch_configure(struct idma_stream *s, u64 count)
{
	void __iomem *base = s->idma->reg;

	if (readq(base + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC)
		return -EOPNOTSUPP;
	if (readq(base + IDMA_BATCH_CONTROL_OFFSET) & 2)
		return -EBUSY;
	writeq(0, base + IDMA_BATCH_CONTROL_OFFSET);
	writeq(count, base + IDMA_BATCH_TARGET_OFFSET);
	writeq(s->size / PAGE_SIZE, base + IDMA_BATCH_PAGES_OFFSET);
	writeq(1, base + IDMA_BATCH_CONTROL_OFFSET);
	s->batch_configured = true;
	s->next_offset = 0;
	return readq(base + IDMA_BATCH_CONTROL_OFFSET) == 1 ? 0 : -EIO;
}

static void idma_batch_stop(struct idma_stream *s)
{
	if (s && s->batch_configured && s->batch_started)
		writeq(2, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
}

static int idma_batch_collect(struct idma_stream *s)
{
	u64 status, issued, completed;

	if (!s || !s->batch_configured)
		return 0;
	if (s->retain)
		return -EIO;
	status = readq(s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
	if (status & 2) {
		idma_quarantine(s);
		return -EIO;
	}
	if (s->batch_started) {
		issued = readq(s->idma->reg + IDMA_BATCH_ISSUED_OFFSET);
		completed = readq(s->idma->reg + IDMA_BATCH_COMPLETED_OFFSET);
		s->stats->submitted = issued;
		s->stats->completed = completed;
		s->stats->completed_bytes = completed * PAGE_SIZE;
		if (s->reverse)
			s->touched_tail = max(s->touched_tail,
				      (size_t)min(completed, (u64)(s->size / PAGE_SIZE)) * PAGE_SIZE);
		else
			s->touched_end = max(s->touched_end,
				     (size_t)min(completed, (u64)(s->size / PAGE_SIZE)) * PAGE_SIZE);
		if (issued != completed || issued > IDMA_EXPERIMENT_MAX_TRANSFERS)
			return -EIO;
	}
	return status >> 32 ? -EIO : 0;
}

static void idma_batch_disable(struct idma_stream *s)
{
	if (s && s->batch_configured && !s->retain)
		writeq(0, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
}

static int idma_batch_window(struct idma_stream *v, struct idma_stream *a,
			     u64 deadline, u64 *end_ns,
			     struct idma_batch_start *start)
{
	u32 spins = 0;
	int ret;

	ret = idma_batch_start(v, a, deadline, end_ns, start);
	if (ret)
		return ret;
	v->deadline_ns = deadline;
	for (;;) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			break;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		ret = idma_poll(v, false);
		if (ret < 0)
			break;
		if (ret == 1) {
			*end_ns = v->last_complete_ns;
			return 0;
		}
		if (!(++spins & 1023))
			cond_resched();
		cpu_relax();
	}
	*end_ns = ktime_get_ns();
	return ret;
}

static void idma_snapshot(struct riscv_iommu_hpm_session *hpm,
			  struct idma_stream *v, struct idma_stream *a,
			  struct riscv_iommu_hpm_sample *sample, u64 active[2])
{
	riscv_iommu_hpm_read(hpm, sample);
	active[0] = readq(v->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
	active[1] = a ? readq(a->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET) : 0;
}

static void idma_snapshot_delta(struct idma_experiment_stats *stats, int n,
			       const struct riscv_iommu_hpm_sample *before,
			       const struct riscv_iommu_hpm_sample *after,
			       u64 active_before, u64 active_after)
{
	stats->active_cycles = active_after - active_before;
	stats->iotlb_miss = after->iotlb_miss[n] - before->iotlb_miss[n];
	stats->s1_ptw = after->s1_ptw[n] - before->s1_ptw[n];
	stats->s2_ptw = after->s2_ptw[n] - before->s2_ptw[n];
}

static int idma_validate(const struct idma_experiment *e)
{
	if (PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e))
		return -EPROTO;
	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	if (!e->victim_pages || e->victim_pages > IDMA_MAX_PAGES ||
	    !e->aggressor_pages || e->aggressor_pages > IDMA_MAX_PAGES ||
	    !e->chunk_bytes || e->chunk_bytes > PAGE_SIZE ||
	    PAGE_SIZE % e->chunk_bytes || !e->victim_transfers ||
	    e->victim_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    e->warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    !e->run_timeout_ms || e->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	return 0;
}

static bool idma_any_quarantined(void)
{
	bool quarantined = false;
	int i;

	mutex_lock(&idma_devices_lock);
	for (i = 0; i < IDMA_MAX_DEVS; i++)
		if (!IS_ERR_OR_NULL(idma_devices[i]) && idma_devices[i]->quarantined)
			quarantined = true;
	mutex_unlock(&idma_devices_lock);
	return quarantined;
}

/* Disabled by default: ordinary E1 works with both old and observation BITs. */
static bool idma_observe;
module_param_named(observe, idma_observe, bool, 0644);
MODULE_PARM_DESC(observe, "Collect passive E4 hardware timing (requires observation BIT)");
static bool idma_path_observe = true;
module_param_named(path_observe, idma_path_observe, bool, 0644);
MODULE_PARM_DESC(path_observe, "Require v3 per-transaction path timing in batch experiments");

struct idma_observation {
	struct idma_device *control;
	u64 sequence, e3_magic;
	bool armed, batch, path, e3, delay128, queue_pair, controlled, p4m;
};

static u64 idma_observation_sequence;

/* Called with the experiment lock, before preparing or submitting any DMA. */
static int idma_observation_prepare(struct idma_observation *o, bool enabled)
{
	u64 magic;

	if (!enabled)
		return 0;
	o->sequence = ++idma_observation_sequence;
	o->control = idma_get_instance(0);
	if (!o->control)
		return -ENODEV;
	magic = readq(o->control->reg + IDMA_OBS_MAGIC_OFFSET);
	/* V9 retains window accounting but physically omits the old path timers. */
	o->path = magic != IDMA_OBS_MAGIC_V9 &&
		(o->p4m || o->e3 || (o->batch && READ_ONCE(idma_path_observe)));
	if (o->p4m && magic != IDMA_OBS_MAGIC_V9 &&
	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
		return -EOPNOTSUPP;
	if (o->controlled && magic != IDMA_OBS_MAGIC_V6 &&
	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
		return -EOPNOTSUPP;
	if (o->queue_pair && magic != IDMA_OBS_MAGIC_V5 &&
	    magic != IDMA_OBS_MAGIC_V6 && magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
		return -EOPNOTSUPP;
	o->e3_magic = (magic == IDMA_OBS_MAGIC_V5 || magic == IDMA_OBS_MAGIC_V6 ||
		       magic == IDMA_OBS_MAGIC_V8 || magic == IDMA_OBS_MAGIC_V7) ? IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
	if (o->e3 && magic != IDMA_OBS_MAGIC_V4 &&
	    magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V6 &&
	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
		return -EOPNOTSUPP;
	if (o->control->did != 10 ||
	    (magic != IDMA_OBS_MAGIC_V9 && magic != IDMA_OBS_MAGIC_V8 &&
	     magic != IDMA_OBS_MAGIC_V7 && magic != IDMA_OBS_MAGIC_V6 &&
	     magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V4 &&
	     magic != IDMA_OBS_MAGIC_V3 &&
	     (o->path || (magic != IDMA_OBS_MAGIC_V2 && (o->batch || magic != IDMA_OBS_MAGIC)))))
		return -EOPNOTSUPP;
	/* Clear previous frozen results; never take ownership of an active session. */
	if (readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) & 3)
		return -EBUSY;
	writeq(4, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
	return readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) ? -EIO : 0;
}

static int idma_observation_arm(struct idma_observation *o,
				struct idma_device *v, struct idma_device *a, u64 count)
{
	u64 command = 1 | (o->batch ? BIT_ULL(25) : 0) | (o->path ? BIT_ULL(26) : 0);

	if (o->p4m) {
		if (o->e3 || o->controlled || o->delay128)
			return -EINVAL;
		command |= BIT_ULL(30);
	}
	if (o->e3)
		command |= BIT_ULL(27) | (o->delay128 ? BIT_ULL(28) : 0);
	if (o->controlled)
		command |= BIT_ULL(29);

	if (!o->control)
		return 0;
	if (o->controlled && count != 1)
		return -EINVAL;
	if ((u32)v->id >= 4 || v->did != 10U + v->id ||
	    (a && ((u32)a->id >= 4 || a->did != 10U + a->id)))
		return -EOPNOTSUPP;
	command |= (u64)v->id << 8;
	if (a)
		command |= BIT_ULL(24) | ((u64)a->id << 16);
	writeq(count, o->control->reg + IDMA_OBS_TARGET_OFFSET);
	writeq(command, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
	o->armed = true;
	return readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) == 1 ? 0 : -EIO;
}

/* Drain/verify first. Frozen reads and logging are outside all timed windows. */
static int idma_observation_finish(struct idma_observation *o,
				   const struct idma_experiment *e, int run_error)
{
	u64 values[IDMA_OBS_VALUES], path[16][8], status;
	void __iomem *base;
	bool valid;
	int i;

	if (!o->control || !o->armed)
		return 0;
	base = o->control->reg;
	status = readq(base + IDMA_OBS_CONTROL_OFFSET);
	if (status & 3) {
		writeq(2, base + IDMA_OBS_CONTROL_OFFSET);
		status = readq(base + IDMA_OBS_CONTROL_OFFSET);
	}
	for (i = 0; i < IDMA_OBS_VALUES; i++) {
		writeq(i, base + IDMA_OBS_INDEX_OFFSET);
		values[i] = readq(base + IDMA_OBS_DATA_OFFSET);
	}
	valid = !run_error && status == 4 &&
		values[7] == e->victim_transfers && values[8] == values[7] &&
		values[9] == (o->batch ? 1 : values[7]) &&
		values[0] == values[1] + values[2] + values[3] &&
		values[0] == values[10] + values[11] + values[12] + values[13];
	/* Opening stock + accepted requests must equal returns + closing stock. */
	for (i = 0; i < 2; i++) {
		u64 opening = values[30 + i], closing = values[32 + i];

		if ((opening >> 16) + values[14 + 6 * i] !=
		    (closing >> 16) + values[16 + 6 * i] ||
		    (opening & 0xffff) + values[15 + 6 * i] !=
		    (closing & 0xffff) + values[17 + 6 * i])
			valid = false;
	}
	if (o->path) {
		int lane, stage, field;

		for (i = 0; i < 16; i++)
			for (field = 0; field < 8; field++) {
				writeq(64 + i * 8 + field, base + IDMA_OBS_INDEX_OFFSET);
				path[i][field] = readq(base + IDMA_OBS_DATA_OFFSET);
			}
		for (lane = 0; lane < 4; lane++) {
			for (stage = 0; stage < 4; stage++) {
				u64 *p = path[lane * 4 + stage];

				/* Opening + entered = completed + closing, including carry-in. */
				if (p[5] + p[0] != p[1] + p[2] + p[6] ||
				    p[4] > p[3] || (!p[1] && (p[3] || p[4])) ||
				    (stage < 3 && p[1] + p[2] != path[lane * 4 + stage + 1][0]))
					valid = false;
				/* All V transactions must be wholly inside the batch window. */
				if (lane < 2 && (p[5] || p[6] || p[2] ||
				    p[1] != values[14 + lane] || p[7] != p[3]))
					valid = false;
				if (lane >= 2 && e->aggressor_did == IDMA_EXPERIMENT_NO_AGGRESSOR)
					for (field = 0; field < 8; field++)
						if (p[field])
							valid = false;
			}
		}
		if (!values[14] || !values[15])
			valid = false;
	}
	if (o->batch) {
		u64 read_delay;

		writeq(34, base + IDMA_OBS_INDEX_OFFSET);
		read_delay = readq(base + IDMA_OBS_DATA_OFFSET);
		valid = valid && values[2] == 0;
		dev_info(o->control->miscdev.parent,
			 "idma_batch_obs seq=%llu part=time valid=%u status=%#llx v_did=%u a_did=%u cycles=%llu h=%llu gap=%llu hmax=%llu gapmax=%llu issued=%llu done=%llu batch_polled=%llu done_to_read=%llu\n",
			 o->sequence, valid, status, e->victim_did, e->aggressor_did,
			 values[0], values[1], values[3], values[4], values[6],
			 values[7], values[8], values[9], read_delay);
	} else {
		dev_info(o->control->miscdev.parent,
			 "idma_obs seq=%llu part=time valid=%u status=%#llx v_did=%u a_did=%u cycles=%llu h=%llu p=%llu r=%llu hmax=%llu pmax=%llu rmax=%llu issued=%llu done=%llu polled=%llu\n",
			 o->sequence, valid, status, e->victim_did, e->aggressor_did,
			 values[0], values[1], values[2], values[3], values[4], values[5],
			 values[6], values[7], values[8], values[9]);
	}
	dev_info(o->control->miscdev.parent,
		 "%s seq=%llu part=axi_v neither=%llu v_only=%llu a_only=%llu both=%llu ar=%llu aw=%llu rlast=%llu b=%llu ar_wait=%llu aw_wait=%llu start=%#llx end=%#llx\n",
		 (o->batch ? "idma_batch_obs" : "idma_obs"), o->sequence,
		 values[10], values[11], values[12], values[13],
		 values[14], values[15], values[16], values[17], values[18],
		 values[19], values[30], values[32]);
	dev_info(o->control->miscdev.parent,
		 "%s seq=%llu part=axi_a ar=%llu aw=%llu rlast=%llu b=%llu ar_wait=%llu aw_wait=%llu start=%#llx end=%#llx shared_ar=%llu shared_aw=%llu shared_ar_wait=%llu shared_aw_wait=%llu\n",
		 (o->batch ? "idma_batch_obs" : "idma_obs"), o->sequence,
		 values[20], values[21], values[22], values[23],
		 values[24], values[25], values[31], values[33], values[26],
		 values[27], values[28], values[29]);
	if (o->path)
		for (i = 0; i < 16; i++)
			dev_info(o->control->miscdev.parent,
				 "idma_path_obs seq=%llu role=%c channel=%c stage=%u valid=%u entered=%llu full=%llu carry=%llu sum=%llu max=%llu start=%llu end=%llu area=%llu\n",
				 o->sequence, i < 8 ? 'v' : 'a', (i / 4) % 2 ? 'w' : 'r',
				 i % 4, valid, path[i][0], path[i][1], path[i][2],
				 path[i][3], path[i][4], path[i][5], path[i][6], path[i][7]);
	return valid ? 0 : -EIO;
}

static int idma_e3_finish(struct idma_observation *o, const struct idma_experiment *e, u64 *words)
{
	u64 n = o->controlled ? 1 : e->victim_transfers * 2;
	unsigned int i, lane, stage;
	bool valid;

	if (!o->armed)
		return -EIO;
	for (i = 0; i < IDMA_E3_WORDS; i++) {
		writeq(192 + i, o->control->reg + IDMA_OBS_INDEX_OFFSET);
		words[i] = readq(o->control->reg + IDMA_OBS_DATA_OFFSET);
	}
	valid = words[0] == o->e3_magic && !words[1] && words[2] <= 1 &&
		words[3] == (o->delay128 ? 128 : 0);
	for (lane = 0; lane < 4; lane++)
		for (stage = 0; stage < 4; stage++) {
			u64 *p = words + 96 + lane * 32 + stage * 8;

			if (p[5] + p[0] != p[1] + p[2] + p[6] ||
			    p[4] > p[3] || (!p[1] && (p[3] || p[4])) ||
			    (stage < 3 && p[1] + p[2] != p[8]))
				valid = false;
			if (lane < 2 && (p[1] != n || p[2] || p[5] || p[6] || p[7] != p[3]))
				valid = false;
		}
	for (lane = 0; lane < 2; lane++) {
		u64 *p = words + 224 + lane * 16;

		if (p[13] + p[0] != p[1] + p[11] || p[1] != p[2] + p[3])
			valid = false;
		if (!lane && (p[0] != n || p[1] != n || p[2] != n || p[3] || p[11] || p[13]))
			valid = false;
	}
	if (words[17] + words[21] != n || words[18] + words[22] != n)
		valid = false;
	/* Full V writes have the same AW/B endpoints as v3 path stage 3. */
	writeq(123, o->control->reg + IDMA_OBS_INDEX_OFFSET);
	if (words[233] != readq(o->control->reg + IDMA_OBS_DATA_OFFSET))
		valid = false;
	if (words[0] == IDMA_E3_MAGIC_V2) {
		if (words[25] < words[26] || words[26] != words[27] + words[28] ||
		    words[29] > words[27] || words[30] < words[27] || words[31] > 1 ||
		    words[2] != !!words[27])
			valid = false;
		if (words[2]) {
			if (words[62] != (words[12] > 2) || words[63] != min(words[12], 2ULL))
				valid = false;
			for (i = 0; i < min(words[12], 2ULL); i++) {
				u64 *p = words + 46 + i * 8;

				if (!p[0] || (p[1] != 0 && p[1] != 4) ||
				    p[2] != (p[1] == 0 ? 10 : 11) || p[4] > 1 || p[7] > 1 ||
				    p[5] > p[6] || p[6] < words[41] || p[6] > words[70] ||
				    (i && p[6] < words[52]))
					valid = false;
			}
		}
	}
	if (words[2]) {
		if (words[13] != 3 || words[7] != words[3] || words[5] != words[4] + 1 + words[3] ||
		    words[6] < words[5] || words[8] > 2 || words[14] != 2 || !words[15] ||
		    words[34] != 11 || words[66] != 10 || words[44] != 0 || words[76] > 1 ||
		    (words[36] & 63) != 63 || words[68] != 127)
			valid = false;
		for (lane = 0; lane < 2; lane++) {
			u64 *p = words + 32 + lane * 32;

			if (!p[0])
				valid = false;
			for (i = 5; i < 11; i++)
				if ((p[4] & BIT_ULL(i - 4)) && p[i + 1] < p[i])
					valid = false;
		}
	}
	return valid ? 0 : -EIO;
}

/* Called only after both devices have drained; mappings remain allocated. */
static void idma_stream_reseed(struct idma_stream *s)
{
	size_t i;

	if (!s)
		return;
	for (i = 0; i < s->size / sizeof(u64); i++) {
		((u64 *)s->src)[i] = idma_pattern(s, i);
		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
	}
	s->offset = s->next_offset = s->touched_end = s->touched_tail = 0;
	s->transfer_id = s->deadline_ns = s->last_complete_ns = 0;
	s->batch_configured = s->batch_started = false;
}

/* ------------------------------------------------------------------------- */
/* Controlled finite E3 round                                                */
/* ------------------------------------------------------------------------- */

static bool idma_launcher_terminal(u64 status)
{
	return (status & IDMA_LAUNCHER_STATUS_FROZEN) &&
	       !(status & IDMA_LAUNCHER_STATUS_RUNNING) &&
	       !(status & IDMA_LAUNCHER_HELD_MASK) &&
	       !(status & IDMA_LAUNCHER_ACTIVE_MASK);
}

static u64 idma_launcher_held(u64 status)
{
	return (status & IDMA_LAUNCHER_HELD_MASK) >> IDMA_LAUNCHER_HELD_SHIFT;
}

static u64 idma_launcher_active(u64 status)
{
	return (status & IDMA_LAUNCHER_ACTIVE_MASK) >> IDMA_LAUNCHER_ACTIVE_SHIFT;
}

static int idma_launcher_read_words(struct idma_device *launcher, u64 *words)
{
	unsigned int i;

	if (readq(launcher->reg + IDMA_LAUNCHER_MAGIC_OFFSET) != IDMA_LAUNCHER_MAGIC)
		return -EOPNOTSUPP;
	for (i = 0; i < IDMA_E3_CONTROLLED_WORDS; i++) {
		writeq(i, launcher->reg + IDMA_LAUNCHER_INDEX_OFFSET);
		words[i] = readq(launcher->reg + IDMA_LAUNCHER_DATA_OFFSET);
	}
	return 0;
}

/* Observer indexed word; only valid once the observer is frozen. */
static u64 idma_observer_indexed(struct idma_observation *o, unsigned int index)
{
	if (!o->control)
		return 0;
	writeq(index, o->control->reg + IDMA_OBS_INDEX_OFFSET);
	return readq(o->control->reg + IDMA_OBS_DATA_OFFSET);
}

static void idma_p4m_clear_outputs(struct idma_p4m *p)
{
	memset(&p->experiment.status, 0,
	       sizeof(p->experiment) - offsetof(struct idma_experiment, status));
	p->sequence = 0;
	p->observer_status = 0;
	memset(p->observation, 0, sizeof(p->observation));
	memset(p->window, 0, sizeof(p->window));
}

static int idma_p4m_validate(struct idma_device *victim,
			     const struct idma_p4m *p)
{
	const struct idma_experiment *e = &p->experiment;
	bool has_a;

	if (victim->id != 0 || victim->did != 10)
		return -EINVAL;
	if (memchr_inv(p->reserved, 0, sizeof(p->reserved)))
		return -EINVAL;
	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e))
		return -EPROTO;
	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	has_a = e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
	if (has_a && e->aggressor_id != 1)
		return -EINVAL;
	if (!has_a && p->aggressor_gap_cycles)
		return -EINVAL;
	if (e->chunk_bytes != PAGE_SIZE)
		return -EINVAL;
	if (!e->victim_transfers || e->victim_transfers > IDMA_BATCH_MAX_TRANSFERS)
		return -EINVAL;
	if (e->warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS)
		return -EINVAL;
	return 0;
}

static int idma_p4m_validate_cfg(struct idma_device *victim,
				 const struct idma_p4m *p,
				 const struct idma_research2_config *cfg)
{
	struct idma_p4m tmp = *p;
	u32 gap = p->aggressor_gap_cycles;
	bool has_a = p->experiment.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
	int ret;

	if (!cfg || cfg->required_role != IDMA_RESEARCH2_METHOD_ROLE)
		return -EINVAL;
	if (gap && cfg->policy_mode != 0)
		return -EINVAL;
	if (gap && (!has_a ||
	    (gap != 500 && gap != 1000 && gap != 2000 && gap != 4000)))
		return -EINVAL;
	tmp.aggressor_gap_cycles = 0;
	ret = idma_p4m_validate(victim, &tmp);
	if (ret)
		return ret;
	return 0;
}

static int idma_gap_preflight(struct idma_device *victim,
			      struct idma_device *aggressor)
{
	if (readq(victim->reg + IDMA_BATCH_GAP_MAGIC_OFFSET) != IDMA_BATCH_GAP_MAGIC)
		return -EOPNOTSUPP;
	if (aggressor &&
	    readq(aggressor->reg + IDMA_BATCH_GAP_MAGIC_OFFSET) != IDMA_BATCH_GAP_MAGIC)
		return -EOPNOTSUPP;
	return 0;
}

static int idma_gap_program_readback(struct idma_stream *s, u32 gap)
{
	void __iomem *base = s->idma->reg;
	u64 readback;

	if (readq(base + IDMA_BATCH_GAP_MAGIC_OFFSET) != IDMA_BATCH_GAP_MAGIC)
		return -EOPNOTSUPP;
	writeq((u64)gap, base + IDMA_BATCH_GAP_OFFSET);
	readback = readq(base + IDMA_BATCH_GAP_OFFSET);
	if (readback != gap)
		return -EIO;
	/* A sticky backend error latched before launch must stop the run. */
	if (readq(base + IDMA_BATCH_CONTROL_OFFSET) >> 32)
		return -EIO;
	return 0;
}

static int idma_p4m_finish(struct idma_observation *o,
			   const struct idma_experiment *e,
			   struct idma_p4m *p, int run_error)
{
	u64 flags, done, first, last, start_id, end_id;
	bool has_a = e->aggressor_did != IDMA_EXPERIMENT_NO_AGGRESSOR;
	bool valid = !run_error;
	unsigned int i;

	if (!o->control || !o->armed)
		return run_error ? run_error : -EIO;
	p->sequence = o->sequence;
	p->observer_status = readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET);
	for (i = 0; i < IDMA_P4M_OBS_WORDS; i++)
		p->observation[i] = idma_observer_indexed(o, i);
	for (i = 0; i < IDMA_P4M_WORDS; i++)
		p->window[i] = idma_observer_indexed(o, IDMA_P4M_BANK_BASE + i);

	/* Frozen record: owned sequence, exact frozen status, live counter. */
	if (!p->sequence || p->observer_status != 4)
		valid = false;
	if (p->window[0] != IDMA_P4M_BANK_MAGIC)
		valid = false;
	if (!p->window[1] || p->window[1] != p->observation[0])
		valid = false;
	flags = p->window[15];
	if (!(flags & BIT_ULL(1)))
		valid = false;
	if (flags & ~(BIT_ULL(0) | BIT_ULL(1) | BIT_ULL(2)))
		valid = false;
	if (!!(flags & BIT_ULL(0)) != has_a)
		valid = false;
	if (p->window[12])
		valid = false;
	if (p->observation[7] != e->victim_transfers ||
	    p->observation[8] != e->victim_transfers)
		valid = false;
	if (p->window[13] != e->victim_transfers ||
	    p->window[14] != e->victim_transfers)
		valid = false;

	/* Every batch identifier must stay inside the encodable range. */
	done = p->window[3];
	first = p->window[10];
	last = p->window[11];
	start_id = p->window[8];
	end_id = p->window[9];
	if (p->window[2] > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    done > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    start_id > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    end_id > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    first > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    last > IDMA_EXPERIMENT_MAX_TRANSFERS)
		valid = false;

	if (!has_a) {
		for (i = 2; i <= 12; i++)
			if (p->window[i])
				valid = false;
		if (flags & (BIT_ULL(0) | BIT_ULL(2)))
			valid = false;
	} else {
		if (p->window[4] > 1 || p->window[5] > 1 ||
		    p->window[6] > 1 || p->window[7] > 1)
			valid = false;
		if (p->window[4] + p->window[2] !=
		    p->window[3] + p->window[5])
			valid = false;
		if (p->window[6] > p->window[4] ||
		    p->window[6] > p->window[3])
			valid = false;
		if (!!(flags & BIT_ULL(2)) !=
		    (p->window[4] && !p->window[7]))
			valid = false;
		/* An opening edge issue hands off a single stock entry. */
		if (p->window[7] && p->window[4] != 1)
			valid = false;
		/* Strict carry retires a carried entry only when work was done. */
		if (!!p->window[6] !=
		    (!!(flags & BIT_ULL(2)) && done > 0))
			valid = false;
		if (!!p->window[4] != !!p->window[8])
			valid = false;
		if (!!p->window[5] != !!p->window[9])
			valid = false;
		if (done) {
			if (!first || !last)
				valid = false;
			if (last != first + done - 1)
				valid = false;
			/* The batch opens on the opening stock's first ID. */
			if (p->window[4] && first != start_id)
				valid = false;
			/* The closing stock continues past the last retired ID. */
			if (p->window[5] && end_id != last + 1)
				valid = false;
		} else if (p->window[4]) {
			/* Nothing retired: the carried entry closes unchanged. */
			if (p->window[5] != 1 || end_id != start_id ||
			    first || last)
				valid = false;
		} else if (first || last) {
			valid = false;
		}
	}
	return valid ? 0 : -EIO;
}

static int idma_controlled_validate(struct idma_device *victim,
				    struct idma_e3_controlled *c, bool *has_a)
{
	struct idma_experiment *e = &c->pair.request;
	bool a;

	if (PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (c->pair.abi_version != 1 || c->pair.struct_size != sizeof(c->pair) ||
	    c->pair.reserved)
		return -EINVAL;
	if (c->prep_mode > 1 || c->pair_mode > 1 ||
	    memchr_inv(c->reserved, 0, sizeof(c->reserved)))
		return -EINVAL;
	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e) ||
	    e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	a = e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
	if (a && e->aggressor_id != 1)
		return -EINVAL;
	/* Hot priming always needs an aggressor lane; cold Solo has none. */
	if (!a && c->prep_mode)
		return -EINVAL;
	if (victim->id != 0 || victim->did != 10)
		return -EINVAL;
	if (e->victim_pages != 1 || e->aggressor_pages != 1)
		return -EINVAL;
	if (e->chunk_bytes != 64 || e->victim_transfers != 1 ||
	    e->warmup_transfers || !e->run_timeout_ms || e->run_timeout_ms > 60000)
		return -EINVAL;
	if (c->pair_mode == 1) {
		if (c->pair.first_delay)
			return -EINVAL;
	} else if (c->pair.first_delay != 0 && c->pair.first_delay != 128) {
		return -EINVAL;
	}
	*has_a = a;
	return 0;
}

/*
 * Preconditions are checked before any mutating write. The launcher is only
 * CLEARed/ABORTed later, and only once ownership is established in the formal
 * path. Foreign prepared/active launchers, observers and backends are refused.
 */
static int idma_controlled_environment(struct idma_device *victim,
				       struct idma_device *aggressor)
{
	u64 status;

	if (readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_OBS_MAGIC_V6 &&
	    readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_OBS_MAGIC_V7 &&
	    readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_OBS_MAGIC_V8)
		return -EOPNOTSUPP;
	if (readq(victim->reg + IDMA_LAUNCHER_MAGIC_OFFSET) != IDMA_LAUNCHER_MAGIC)
		return -EOPNOTSUPP;
	if (readq(victim->reg + IDMA_OBS_CONTROL_OFFSET) & 3)
		return -EBUSY;
	status = readq(victim->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (status & (IDMA_LAUNCHER_STATUS_PREP | IDMA_LAUNCHER_STATUS_RUNNING |
		      IDMA_LAUNCHER_HELD_MASK | IDMA_LAUNCHER_ACTIVE_MASK |
		      IDMA_LAUNCHER_ERROR_MASK))
		return -EBUSY;
	if (readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC ||
	    (readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET) & 2))
		return -EBUSY;
	if (aggressor &&
	    (readq(aggressor->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC ||
	     (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET) & 2)))
		return -EBUSY;
	if (readq(victim->reg + IDMA_STATUS_REG_OFFSET) ||
	    (aggressor && readq(aggressor->reg + IDMA_STATUS_REG_OFFSET)))
		return -EBUSY;
	/* All idle and owned: disable the backend batch engines with readback. */
	writeq(0, victim->reg + IDMA_BATCH_CONTROL_OFFSET);
	if (readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET))
		return -EIO;
	if (aggressor) {
		writeq(0, aggressor->reg + IDMA_BATCH_CONTROL_OFFSET);
		if (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET))
			return -EIO;
	}
	return 0;
}

/*
 * One finite priming task: offset 0, CPU flush, single submit + bounded drain,
 * FENCE.I, verify this stream's own src/dst, then reseed this stream.  The HPM
 * lane is explicit; statistics are kept separate from the formal statistics.
 */
static int idma_controlled_prep_task(struct idma_stream *s, int lane, bool probe,
				     struct idma_experiment_stats *stats,
				     struct riscv_iommu_hpm_session *hpm,
				     u64 deadline)
{
	struct riscv_iommu_hpm_sample before, after;
	u64 active_before, active_after;
	int ret;

	if (s->in_flight || s->retain)
		return -EBUSY;
	if (ktime_get_ns() >= deadline)
		return -ETIMEDOUT;
	if (signal_pending(current))
		return -EINTR;
	memset(stats, 0, sizeof(*stats));
	s->stats = stats;
	s->offset = s->next_offset = s->touched_end = s->touched_tail = 0;
	s->in_flight = false;
	s->transfer_id = s->deadline_ns = s->last_complete_ns = 0;

	riscv_iommu_hpm_read(hpm, &before);
	active_before = readq(s->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);

	local_flush_icache_all();
	ret = idma_submit(s, false);
	if (!ret) {
		stats->submitted = 1;
		ret = idma_drain(s);
	}

	active_after = readq(s->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
	riscv_iommu_hpm_read(hpm, &after);

	stats->active_cycles = active_after - active_before;
	stats->iotlb_miss = after.iotlb_miss[lane] - before.iotlb_miss[lane];
	stats->s1_ptw = after.s1_ptw[lane] - before.s1_ptw[lane];
	stats->s2_ptw = after.s2_ptw[lane] - before.s2_ptw[lane];
	stats->completed = ret ? 0 : 1;
	stats->completed_bytes = ret ? 0 : s->chunk;

	if (ret)
		return ret;
	/* S2 walk is never expected for a single-level mapping. */
	if (stats->s2_ptw)
		return -EIO;
	/* The probe must actually copy while both V translations are hot. */
	if (probe && (stats->iotlb_miss || stats->s1_ptw))
		return -EIO;

	local_flush_icache_all();
	ret = idma_verify(s);
	if (ret)
		return ret;

	/* Reset this stream so the next phase starts from offset 0 again. */
	idma_stream_reseed(s);
	local_flush_icache_all();
	return 0;
}

/*
 * Accounting only: every reserved lane must be cancelled (with zero actual
 * issues) or actually completed.  Nothing may complete that was not reserved.
 */
static int idma_controlled_account(struct idma_stream *v, struct idma_stream *a,
				   u8 reserved_mask, u64 *words)
{
	u64 expected = a ? 3 : 1;
	u64 issued = words[15], done = words[16], cancel = words[17];
	unsigned int lane;

	if (words[0] != IDMA_LAUNCHER_MAGIC || !idma_launcher_terminal(words[2]) ||
	    words[19] != expected || words[28] != !!a || words[29] || words[30] ||
	    words[31] || (reserved_mask & ~expected) || issued != done ||
	    (done & cancel) || (done | cancel) != reserved_mask ||
	    words[18] != reserved_mask)
		return -EIO;
	for (lane = 0; lane < 2; lane++) {
		struct idma_stream *stream = lane ? a : v;
		u64 n = !!(issued & BIT(lane));

		if (words[24 + lane] != n || words[26 + lane] != n)
			return -EIO;
		if (n) {
			if (!stream || !stream->transfer_id ||
			    words[11 + lane] != stream->transfer_id ||
			    words[13 + lane] != stream->transfer_id)
				return -EIO;
		} else if (words[11 + lane] || words[13 + lane]) {
			return -EIO;
		}
	}
	return 0;
}

/*
 * Only called once the launcher proved terminal.  Cancelled reservations get
 * in_flight cleared with no touched bytes; actual completions claim exactly
 * one chunk at offset 0.
 */
static void idma_controlled_apply(struct idma_stream *v, struct idma_stream *a, u64 *words)
{
	u8 done = (u8)(words[16] & 0x3);
	u8 cancel = (u8)(words[17] & 0x3);

	if (done & 1) {
		v->in_flight = false;
		v->touched_end = v->offset + v->chunk;
	} else if (cancel & 1) {
		v->in_flight = false;
	}
	if (a) {
		if (done & 2) {
			a->in_flight = false;
			a->touched_end = a->offset + a->chunk;
		} else if (cancel & 2) {
			a->in_flight = false;
		}
	}
}

static int idma_controlled_validate_success(struct idma_stream *v,
					    struct idma_stream *a, u64 *words)
{
	u8 expected = a ? 3 : 1;

	if (idma_controlled_account(v, a, expected, words) || words[1] ||
	    (words[2] & IDMA_LAUNCHER_STATUS_ABORTED) ||
	    words[15] != expected || words[16] != expected || words[17])
		return -EIO;
	if (!words[6] || words[6] > words[7] || words[7] > words[9])
		return -EIO;
	if (a) {
		if (!words[4] || words[4] > words[8] || words[8] > words[5] ||
		    words[5] >= words[6] || words[10] < words[8] ||
		    words[20] != IDMA_CONTROLLED_A_SELECT_ID ||
		    words[21] != IDMA_CONTROLLED_A_SELECT_DID ||
		    words[22] != a->src_iova || words[23])
			return -EIO;
	} else if (words[4] || words[5] || words[8] || words[10] ||
		   words[20] || words[21] || words[22] || words[23]) {
		return -EIO;
	}
	return 0;
}

/*
 * Single conservative cleanup.  Never CLEARs or ABORTs an unowned launcher;
 * never frees a lane whose terminal/accounting proof failed.  Raw words are
 * copied on every reachable path; local words are zero-initialized.
 */
static int idma_controlled_cleanup(struct idma_device *launcher,
				   struct idma_stream *v, struct idma_stream *a,
				   struct idma_experiment *e,
				   struct idma_e3_controlled_round *round,
				   u8 reserved_mask, bool launcher_owned,
				   int first_error)
{
	u64 words[IDMA_E3_CONTROLLED_WORDS];
	u64 status;
	int ret = first_error;
	int ret2;
	bool terminal;

	memset(words, 0, sizeof(words));

	if (!launcher_owned) {
		memcpy(round->launcher, words, sizeof(round->launcher));
		return ret;
	}

	/* Never CLEAR before terminal proof; ABORT only a launcher we own. */
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (!idma_launcher_terminal(status)) {
		writeq(IDMA_LAUNCHER_ABORT, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
		ret2 = readq_poll_timeout(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET,
					  status, idma_launcher_terminal(status), 0,
					  IDMA_XFER_TIMEOUT_US);
		if (ret2) {
			if (!ret)
				ret = ret2;
		}
	}
	terminal = idma_launcher_terminal(status);

	/* Preserve live raw evidence even if terminal proof timed out. */
	ret2 = idma_launcher_read_words(launcher, words);
	memcpy(round->launcher, words, sizeof(round->launcher));
	if (ret2 || !terminal) {
		if (!ret)
			ret = ret2 ? ret2 : -ETIMEDOUT;
		goto quarantine;
	}

	/* Preserve independent actual activity counts on every failure path. */
	e->victim.submitted = words[24];
	e->victim.completed = words[26];
	e->victim.completed_bytes = words[26] * v->chunk;
	if (a) {
		e->aggressor.submitted = words[25];
		e->aggressor.completed = words[27];
		e->aggressor.completed_bytes = words[27] * a->chunk;
	}

	if (idma_controlled_account(v, a, reserved_mask, words)) {
		if (!ret)
			ret = -EIO;
		goto quarantine;
	}

	idma_controlled_apply(v, a, words);

	if (!ret) {
		ret2 = idma_controlled_validate_success(v, a, words);
		if (ret2) {
			ret = ret2;
			goto quarantine;
		}
	}
	return ret;

quarantine:
	if (v->mapped_src || v->mapped_dst)
		idma_quarantine(v);
	if (a && (a->mapped_src || a->mapped_dst))
		idma_quarantine(a);
	return ret;
}

/*
 * Formal round: PREP/reserve/arm/GO/poll.  Ownership is marked before the PREP
 * write; every failure after that runs the common cleanup.  The HPM window
 * spans GO -> both lanes drained, which is wider than the software V done time.
 */
static int idma_controlled_formal(struct idma_device *launcher,
				  struct idma_stream *v, struct idma_stream *a,
				  struct idma_observation *o,
				  struct idma_experiment *e,
				  struct idma_e3_controlled_round *round,
				  struct riscv_iommu_hpm_session *hpm,
				  u64 deadline, u8 *reserved_mask, bool *owned)
{
	struct riscv_iommu_hpm_sample before, after;
	u64 active_before[2], active_after[2];
	u64 status, arm_stamp, start = 0, end, drain_start, v_done = 0;
	u8 reserved = 0;
	int ret, poll_ret;
	bool started = false;

	v->stats = &e->victim;
	if (a)
		a->stats = &e->aggressor;
	memset(&e->victim, 0, sizeof(e->victim));
	if (a)
		memset(&e->aggressor, 0, sizeof(e->aggressor));
	if (v->in_flight || v->retain || (a && (a->in_flight || a->retain))) {
		ret = -EBUSY;
		goto out;
	}
	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}

	v->offset = v->next_offset = v->touched_end = v->touched_tail = 0;
	v->in_flight = false;
	v->transfer_id = v->deadline_ns = v->last_complete_ns = 0;
	if (a) {
		a->offset = a->next_offset = a->touched_end = a->touched_tail = 0;
		a->in_flight = false;
		a->transfer_id = a->deadline_ns = a->last_complete_ns = 0;
	}

	/* Ownership starts at the CLEAR/PREP writes. */
	*owned = true;
	writeq(IDMA_LAUNCHER_CLEAR, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (status & ~IDMA_LAUNCHER_STATUS_FROZEN) {
		ret = -EIO;
		goto out;
	}
	writeq(IDMA_LAUNCHER_PREP | (a ? IDMA_LAUNCHER_PREP_HAS_A : 0),
	       launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (!(status & IDMA_LAUNCHER_STATUS_PREP) ||
	    (status & IDMA_LAUNCHER_ERROR_MASK))
	{
		ret = -EIO;
		goto out;
	}

	ret = idma_submit(v, false);
	if (ret)
		goto out;
	reserved |= 1;
	*reserved_mask = reserved;
	if (a) {
		ret = idma_submit(a, false);
		if (ret)
			goto out;
		reserved |= 2;
		*reserved_mask = reserved;
	}

	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if ((status & IDMA_LAUNCHER_ERROR_MASK) ||
	    idma_launcher_held(status) != (a ? 3U : 1U) ||
	    idma_launcher_active(status) != (a ? 3U : 1U))
	{
		ret = -EIO;
		goto out;
	}

	ret = idma_observation_arm(o, v->idma, a ? a->idma : NULL, 1);
	if (ret)
		goto out;

	arm_stamp = ktime_get_ns();
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e->measurement_started = 1;
	started = true;
	e->victim.in_flight_at_start = v->in_flight;
	if (a)
		e->aggressor.in_flight_at_start = a->in_flight;
	e->snapshot_before_ns = start - arm_stamp;
	round->stage = IDMA_CONTROLLED_STAGE_RUNNING;

	writeq(IDMA_LAUNCHER_GO, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (status & IDMA_LAUNCHER_ERROR_MASK)
	{
		ret = -EIO;
		goto out;
	}

	ret = 0;
	for (;;) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			break;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		poll_ret = idma_poll(v, false);
		if (poll_ret < 0) {
			ret = poll_ret;
			break;
		}
		if (poll_ret == 1 && !v_done)
			v_done = v->last_complete_ns;
		if (a) {
			poll_ret = idma_poll(a, false);
			if (poll_ret < 0) {
				ret = poll_ret;
				break;
			}
		}
		status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
		if (status & IDMA_LAUNCHER_ERROR_MASK) {
			ret = -EIO;
			break;
		}
		/* Terminal can become visible after the earlier V DONE poll.
		 * Keep polling until that completion has also reached the observer. */
		if (idma_launcher_terminal(status) && !v->in_flight &&
		    (!a || !a->in_flight))
			break;
		cpu_relax();
	}

out:
	drain_start = ktime_get_ns();
	if (!ret)
		round->stage = IDMA_CONTROLLED_STAGE_DRAIN;
	ret = idma_controlled_cleanup(launcher, v, a, e, round,
				      *reserved_mask, *owned, ret);
	*owned = false;
	e->drain_ns = ktime_get_ns() - drain_start;
	if (started) {
		if (v_done)
			e->victim_elapsed_ns = v_done - start;
		end = ktime_get_ns();
		idma_snapshot(hpm, v, a, &after, active_after);
		e->snapshot_after_ns = ktime_get_ns() - end;
		idma_snapshot_delta(&e->victim, 0, &before, &after,
				    active_before[0], active_after[0]);
		if (a)
			idma_snapshot_delta(&e->aggressor, 1, &before, &after,
					    active_before[1], active_after[1]);
	}
	return ret;
}

static int idma_controlled_one_round(struct idma_device *launcher,
				     struct idma_stream *v, struct idma_stream *a,
				     struct idma_observation *o,
				     struct idma_experiment *e,
				     struct idma_e3_controlled_round *round,
				     u64 *e3_words,
				     struct riscv_iommu_hpm_session **hpm,
				     bool hot, u64 timeout_ms)
{
	u64 deadline = ktime_get_ns() + timeout_ms * NSEC_PER_MSEC;
	bool owned = false;
	u8 reserved_mask = 0;
	int ret, sub;

	round->flush_status = -EINPROGRESS;
	round->stage = IDMA_CONTROLLED_STAGE_FLUSH;
	local_flush_icache_all();
	ret = riscv_iommu_hpm_flush_s1(*hpm);
	round->flush_status = ret;
	if (ret)
		goto finish;

	round->stage = IDMA_CONTROLLED_STAGE_APRIME;
	memset(&round->prime[1], 0, sizeof(round->prime[1]));
	if (a) {
		if (hot) {
			ret = idma_controlled_prep_task(a, 1, false, &round->prime[1],
							*hpm, deadline);
			if (ret)
				goto finish;
		} else {
			a->offset = a->next_offset = a->touched_end = a->touched_tail = 0;
			a->in_flight = false;
			a->transfer_id = a->deadline_ns = a->last_complete_ns = 0;
		}
	}

	round->stage = IDMA_CONTROLLED_STAGE_VPRIME;
	ret = idma_controlled_prep_task(v, 0, false, &round->prime[0], *hpm, deadline);
	if (ret)
		goto finish;

	round->stage = IDMA_CONTROLLED_STAGE_VPROBE;
	ret = idma_controlled_prep_task(v, 0, true, &round->probe, *hpm, deadline);
	if (ret)
		goto finish;

	round->prep_complete = 1;
	round->stage = IDMA_CONTROLLED_STAGE_PREPARE;
	ret = idma_controlled_formal(launcher, v, a, o, e, round, *hpm,
				     deadline, &reserved_mask, &owned);

	if (!ret) {
		local_flush_icache_all();
		sub = idma_verify(v);
		if (sub)
			ret = sub;
		if (a && !ret) {
			sub = idma_verify(a);
			if (sub)
				ret = sub;
		}
	}

finish:
	if (*hpm) {
		riscv_iommu_hpm_release(*hpm);
		*hpm = NULL;
	}
	if (o->control) {
		if (o->armed) {
			sub = idma_observation_finish(o, e, ret);
			if (sub && !ret)
				ret = sub;
			round->v_hw_cycles = idma_observer_indexed(o, 0);
			sub = idma_e3_finish(o, e, e3_words);
			if (sub && !ret)
				ret = sub;
		} else {
			idma_observation_finish(o, e, ret);
		}
		round->observer_magic = readq(o->control->reg + IDMA_OBS_MAGIC_OFFSET);
	}
	if (!round->prep_complete) {
		e->victim.data_errors = round->prime[0].data_errors + round->probe.data_errors;
		if (a)
			e->aggressor.data_errors = round->prime[1].data_errors;
	}
	e->victim.quarantined = v->retain;
	if (a)
		e->aggressor.quarantined = a->retain;
	e->victim.in_flight_at_end = v->in_flight;
	if (a)
		e->aggressor.in_flight_at_end = a->in_flight;
	if (!ret) {
		round->stage = IDMA_CONTROLLED_STAGE_COMPLETE;
		e->measurement_complete = 1;
	}
	e->status = ret;
	return ret;
}

static long idma_run_e3_controlled(struct idma_device *victim, void __user *uarg)
{
	struct idma_e3_controlled *c;
	struct idma_experiment e;
	struct idma_stream streams[2] = {};
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_device *aggressor = NULL, *launcher;
	struct idma_observation o;
	struct riscv_iommu_hpm_session *hpm = NULL;
	bool has_a = false;
	unsigned int round;
	long ret;

	c = kzalloc(sizeof(*c), GFP_KERNEL);
	if (!c)
		return -ENOMEM;
	if (copy_from_user(c, uarg, sizeof(*c))) {
		ret = -EFAULT;
		goto out_free;
	}
	/* Clear all outputs while preserving request inputs, DIDs and modes. */
	memset(&c->pair.status, 0, sizeof(c->pair) - offsetof(struct idma_e3_pair, status));
	memset(c->controlled, 0, sizeof(c->controlled));

	ret = idma_controlled_validate(victim, c, &has_a);
	if (ret)
		goto out_copy;
	e = c->pair.request;
	e.victim_did = 10;
	e.aggressor_did = has_a ? 11 : IDMA_EXPERIMENT_NO_AGGRESSOR;

	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_copy;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto out_unlock;
	}
	if (has_a) {
		aggressor = idma_get_instance(1);
		if (!aggressor) {
			ret = -ENODEV;
			goto out_unlock;
		}
		if (aggressor->did != 11) {
			ret = -EINVAL;
			goto out_put;
		}
		a = &streams[1];
	}
	launcher = victim;
	ret = idma_controlled_environment(victim, aggressor);
	if (ret)
		goto out_put;

	ret = idma_stream_prepare(v, victim, 1, 64, &e.victim);
	if (ret)
		goto out_cleanup;
	if (a) {
		ret = idma_stream_prepare(a, aggressor, 1, 64, &e.aggressor);
		if (ret)
			goto out_cleanup;
	}
	c->pair.mapping[0] = v->src_iova;
	c->pair.mapping[1] = v->dst_iova;
	c->pair.mapping[2] = page_to_phys(idma_buffer_page(v->src));
	c->pair.mapping[3] = page_to_phys(idma_buffer_page(v->dst));
	if (a) {
		c->pair.mapping[4] = a->src_iova;
		c->pair.mapping[5] = a->dst_iova;
		c->pair.mapping[6] = page_to_phys(idma_buffer_page(a->src));
		c->pair.mapping[7] = page_to_phys(idma_buffer_page(a->dst));
	}

	for (round = 0; round < 2; round++) {
		bool hot = c->prep_mode == 1;

		/* Reset every output field so round 2 cannot inherit round 1. */
		memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
		e.victim_did = victim->did;
		e.aggressor_did = has_a ? aggressor->did : IDMA_EXPERIMENT_NO_AGGRESSOR;
		e.hpm_configured = 0;
		memset(&e.victim, 0, sizeof(e.victim));
		memset(&e.aggressor, 0, sizeof(e.aggressor));

		memset(&o, 0, sizeof(o));
		o.batch = true;
		o.e3 = true;
		o.controlled = true;
		o.delay128 = c->pair_mode == 1 ? false :
			(round ? c->pair.first_delay == 0
			       : c->pair.first_delay == 128);

		c->pair.rounds_attempted++;
		ret = idma_observation_prepare(&o, true);
		if (ret) {
			e.status = ret;
			c->pair.result[round] = e;
			c->pair.status = ret;
			if (o.control) {
				idma_put_instance(o.control);
				o.control = NULL;
			}
			break;
		}
		c->pair.sequence[round] = o.sequence;

		ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
		if (ret) {
			e.status = ret;
			c->pair.result[round] = e;
			c->pair.status = ret;
			idma_observation_finish(&o, &e, ret);
			idma_put_instance(o.control);
			o.control = NULL;
			break;
		}
		e.hpm_configured = 1;

		ret = idma_controlled_one_round(launcher, v, a, &o, &e,
						&c->controlled[round],
						c->pair.observation[round],
						&hpm, hot, e.run_timeout_ms);
		/* one_round released hpm and set e.status. */
		c->pair.result[round] = e;
		if (ret) {
			c->pair.status = ret;
			if (o.control) {
				idma_put_instance(o.control);
				o.control = NULL;
			}
			break;
		}
		c->pair.rounds_completed++;
		if (o.control) {
			idma_put_instance(o.control);
			o.control = NULL;
		}
		idma_stream_reseed(v);
		if (a)
			idma_stream_reseed(a);
		local_flush_icache_all();
	}
	if (hpm)
		riscv_iommu_hpm_release(hpm);

out_cleanup:
	if (a)
		idma_stream_cleanup(a);
	idma_stream_cleanup(v);
out_put:
	idma_put_instance(aggressor);
out_unlock:
	mutex_unlock(&idma_experiment_lock);
out_copy:
	c->pair.status = ret;
	if (copy_to_user(uarg, c, sizeof(*c)))
		ret = -EFAULT;
out_free:
	kfree(c);
	return ret;
}

static int idma_research_begin(struct idma_device *d, struct idma_research *r);
static int idma_research_capture(struct idma_device *d, struct idma_research *r, int error);
static int idma_research_quiet(struct idma_device *d, u64 deadline);
static int idma_research2_preflight(struct idma_device *d,
				    const struct idma_research2_config *cfg);
static int idma_research2_pre_arm(struct idma_device *d,
				  const struct idma_research2_config *cfg);
static int idma_research2_method_freeze(struct idma_device *d,
					const struct idma_research2_config *cfg,
					int error);
static void idma_research2_method_abort(struct idma_device *d,
					const struct idma_research2_config *cfg,
					int error);
static int idma_research_capture_ext(struct idma_device *d,
				     struct idma_research *r,
				     const struct idma_research2_config *cfg,
				     int error);

/* New, independent hardware bank. All calls hold idma_experiment_lock. */
#define IDMA_COMPOSITION_BASE (512U + 0xd20U)

static u64 idma_composition_read(struct idma_device *d, unsigned int word)
{
	writeq(IDMA_COMPOSITION_BASE + word, d->reg + IDMA_OBS_INDEX_OFFSET);
	return readq(d->reg + IDMA_OBS_DATA_OFFSET);
}

static int idma_composition_configure(struct idma_device *d, u32 mode, u32 did,
				      u64 *generation)
{
	u64 old, now, status, config = ((u64)did << 8) | mode;

	if (idma_composition_read(d, 0) != IDMA_COMPOSITION_MAGIC ||
	    idma_composition_read(d, 1) != IDMA_COMPOSITION_ABI)
		return -EOPNOTSUPP;
	old = idma_composition_read(d, 3);
	if (old == U64_MAX)
		return -EOVERFLOW;
	if (idma_composition_read(d, 5) != 1)
		return -EBUSY;
	writeq(IDMA_COMPOSITION_BASE + 2, d->reg + IDMA_OBS_INDEX_OFFSET);
	writeq(config, d->reg + IDMA_OBS_DATA_OFFSET);
	/* Same-clock atomic clear/config/ack: ordered read observes that edge. */
	status = idma_composition_read(d, 4);
	now = idma_composition_read(d, 3);
	if (status == 1)
		return -EBUSY;
	if (status || now != old + 1 || idma_composition_read(d, 2) != config)
		return -EIO;
	*generation = now;
	return 0;
}

static int idma_composition_capture(struct idma_device *d,
				    struct idma_composition *p, int error)
{
	u64 config = ((u64)p->protected_did << 8) | p->mode;
	unsigned int i;

	for (i = 0; i < IDMA_COMPOSITION_WORDS; i++)
		p->raw[i] = idma_composition_read(d, i);
	p->raw_captured = 1;
	if (error)
		return error;
	if (p->raw[0] != IDMA_COMPOSITION_MAGIC || p->raw[1] != 1 ||
	    p->raw[2] != config || p->raw[3] != p->generation ||
	    p->raw[6] != 4 || p->raw[9] || p->raw[10] != config ||
	    p->raw[11] != p->generation || !p->raw[7] ||
	    p->raw[8] != p->p4m.window[1] || p->raw[34] != 3)
		return -EIO;
	for (i = 12; i <= 18; i += 3)
		if (p->raw[i] != p->raw[i + 1] + p->raw[i + 2])
			return -EIO;
	if (p->raw[24] > p->raw[8] || p->raw[25] ||
	    (!p->victim_gap_cycles && p->raw[24]) ||
	    (p->victim_gap_cycles && p->p4m.experiment.victim_transfers > 1 &&
	     !p->raw[24]) || p->raw[30] || p->raw[32] || p->raw[33])
		return -EIO;
	/* Reserved words are an ABI check, not experimental outcome filtering. */
	if (p->raw[35])
		return -EIO;
	for (i = 40; i < 64; i++)
		if (p->raw[i])
			return -EPROTO;
	return 0;
}

static int idma_composition_restore(struct idma_device *d,
				    struct idma_composition *p)
{
	u64 deadline = ktime_get_ns() + NSEC_PER_SEC;

	/* Never force a cache clear across an unfinished DMA or response. */
	while (idma_composition_read(d, 5) != 1) {
		if (ktime_get_ns() >= deadline)
			return -ETIMEDOUT;
		cond_resched();
	}
	return idma_composition_configure(d, 0, 10, &p->restored_generation);
}

static long idma_run_session2(struct idma_device *victim,
			      struct idma_experiment *request, bool batch, struct idma_e3_pair *pair,
			      struct idma_e3_diag_round *diagnostic, bool queue_pair,
			      struct idma_p4m *p4m, struct idma_research *research,
			      const struct idma_research2_config *cfg,
			      struct idma_composition *composition)
{
	struct idma_experiment e = *request;
	struct idma_stream streams[2] = {};
	struct idma_observation observation = { .batch = batch, .e3 = !!pair, .queue_pair = queue_pair,
		.delay128 = pair && pair->first_delay == 128, .p4m = !!p4m };
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_batch_start batch_start = {};
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	struct idma_e3_diag_round *diag = NULL;
	u64 active_before[2], active_after[2];
	u64 start, end = 0, deadline, stamp;
	int ret, drain_ret, verify_ret, observation_ret, p4m_ret;
	unsigned int round = 0;
	bool comp_trace = composition != NULL &&
		READ_ONCE(idma_composition_trace);

	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
	e.victim_did = victim->did;
	e.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_validate(&e);
	if (!ret && batch && (e.chunk_bytes != PAGE_SIZE ||
			      e.victim_transfers > IDMA_BATCH_MAX_TRANSFERS))
		ret = -EINVAL;
	if (ret)
		goto out_copy;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_copy;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto out_unlock;
	}
	if (cfg) {
		ret = idma_research2_preflight(victim, cfg);
		if (ret)
			goto out_unlock;
	}
	if (e.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) {
		aggressor = idma_get_instance(e.aggressor_id);
		if (!aggressor) {
			ret = -ENODEV;
			goto out_unlock;
		}
		if (aggressor == victim || aggressor->did == victim->did) {
			ret = -EINVAL;
			goto out_put;
		}
		e.aggressor_did = aggressor->did;
		a = &streams[1];
	}
	batch_start.v_did = victim->did;
	batch_start.a_did = a ? aggressor->did : IDMA_EXPERIMENT_NO_AGGRESSOR;
	batch_start.has_a = !!a;
	if (p4m) {
		ret = idma_gap_preflight(victim, aggressor);
		if (ret)
			goto out_put;
	}
	ret = idma_observation_prepare(&observation, batch || READ_ONCE(idma_observe));
	if (ret)
		goto out_put;
	if (batch && (readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC ||
	    (aggressor && readq(aggressor->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC))) {
		ret = -EOPNOTSUPP;
		goto out_put;
	}
	if (batch && ((readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) ||
	    (aggressor && (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET) & 2)))) {
		ret = -EBUSY;
		goto out_put;
	}
	if (composition) {
		idma_comp_stage(comp_trace, victim->miscdev.parent,
				"pre-config", 0);
		ret = idma_composition_configure(victim, composition->mode,
				composition->protected_did, &composition->generation);
		idma_comp_stage(comp_trace, victim->miscdev.parent,
				"post-config", ret);
		if (ret)
			goto out_put;
		composition->configured = 1;
		composition->restore_status = -EINPROGRESS;
	}
	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
	if (ret)
		goto out_cleanup;
	v->composition_trace = comp_trace;
	if (a) {
		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
					  e.chunk_bytes, &e.aggressor);
		if (ret)
			goto out_cleanup;
		a->reverse = cfg && cfg->reverse;
	}
	if (pair) {
		pair->mapping[0] = v->src_iova; pair->mapping[1] = v->dst_iova;
		pair->mapping[2] = page_to_phys(idma_buffer_page(v->src));
		pair->mapping[3] = page_to_phys(idma_buffer_page(v->dst));
		if (a) {
			pair->mapping[4] = a->src_iova; pair->mapping[5] = a->dst_iova;
			pair->mapping[6] = page_to_phys(idma_buffer_page(a->src));
			pair->mapping[7] = page_to_phys(idma_buffer_page(a->dst));
		}
	}
 run_round:
	batch_start.attempted = false;
	batch_start.a_triggered = false;
	batch_start.v_triggered = false;
	batch_start.a_id = 0;
	batch_start.v_id = 0;
	batch_start.a_begin_ns = batch_start.a_end_ns = 0;
	batch_start.v_begin_ns = batch_start.v_end_ns = 0;
	batch_start.irq_span_ns = 0;
	if (diagnostic) {
		diag = &diagnostic[round];
		v->diagnostic = &diag->lane[0];
		if (a)
			a->diagnostic = &diag->lane[1];
	}
	if (pair)
		pair->rounds_attempted++;
	ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
	if (ret)
		goto out_cleanup;
	e.hpm_configured = 1;
	/*
	 * Platform workaround: this CVA6 has no DMA snoop of the WT data cache.
	 * Its FENCE.I drains CPU writes and invalidates D-cache as well as I-cache.
	 * Hand initialized buffers to DMA before warmup; never flush per transfer.
	 */
	local_flush_icache_all();
	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
	if (diag) {
		idma_snapshot(hpm, v, a, &before, active_before);
		diag->warm_begin_ns = ktime_get_ns();
	}
	idma_comp_stage(v->composition_trace, v->dev, "warm-start", 0);
	if (a && (!batch || e.warmup_transfers)) {
		ret = idma_submit(a, false);
		if (ret)
			goto out_drain;
	}
	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
	if (diag) {
		diag->warm_end_ns = ktime_get_ns();
		diag->lane[0].warm_in_flight = v->in_flight;
		diag->lane[0].warm_last_offset = v->offset;
		diag->lane[0].warm_next_offset = v->next_offset;
		if (a) {
			diag->lane[1].warm_in_flight = a->in_flight;
			diag->lane[1].warm_last_offset = a->offset;
			diag->lane[1].warm_next_offset = a->next_offset;
		}
	}
	idma_comp_stage(v->composition_trace, v->dev, "warm-done", ret);
	if (ret)
		goto out_drain;
	/* Every measured V task starts at the same address after warmup. */
	v->next_offset = 0;
	v->last_complete_ns = 0;
	if (a)
		a->last_complete_ns = 0;
	if (batch) {
		/* Software warmup is drained before enabling the autonomous mode. */
		if (a) {
			ret = idma_drain(a);
			if (ret)
				goto out_drain;
		}
		if (diag) {
			diag->warm_drained_ns = ktime_get_ns();
			idma_snapshot(hpm, v, a, &after, active_after);
			idma_snapshot_delta(&diag->lane[0].warm_hpm, 0, &before, &after,
					    active_before[0], active_after[0]);
			if (a)
				idma_snapshot_delta(&diag->lane[1].warm_hpm, 1, &before, &after,
						    active_before[1], active_after[1]);
			diag->warm_complete = 1;
		}
		ret = idma_batch_configure(v, e.victim_transfers);
		if (!ret && a)
			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS);
		idma_comp_stage(v->composition_trace, v->dev,
				"batch-config-done", ret);
		if (ret)
			goto out_drain;
	}
	if (p4m) {
		ret = idma_gap_program_readback(v, composition ? composition->victim_gap_cycles : 0);
		if (!ret && a)
			ret = idma_gap_program_readback(a,
							p4m->aggressor_gap_cycles);
		idma_comp_stage(v->composition_trace, v->dev, "gap-done", ret);
		if (ret)
			goto out_drain;
	}
	e.victim.in_flight_at_start = v->in_flight;
	e.aggressor.in_flight_at_start = a && a->in_flight;
	if (diag)
		diag->arm_begin_ns = ktime_get_ns();
	if (research) {
		research->mapping[0] = v->src_iova;
		research->mapping[1] = v->dst_iova;
		research->mapping[2] = a ? a->src_iova : 0;
		research->mapping[3] = a ? a->dst_iova : 0;
		ret = idma_research_begin(victim, research);
		if (ret)
			goto out_drain;
	}
	if (cfg) {
		ret = idma_research2_pre_arm(victim, cfg);
		if (ret)
			goto out_drain;
	}
	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
	idma_comp_stage(v->composition_trace, v->dev, "arm-done", ret);
	if (ret)
		goto out_drain;
	stamp = ktime_get_ns();
	if (diag)
		diag->arm_end_ns = stamp;
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e.snapshot_before_ns = start - stamp;
	e.measurement_started = 1;
	idma_comp_stage(v->composition_trace, v->dev, "prewindow", 0);
	ret = batch ? idma_batch_window(v, a, deadline, &end, &batch_start) :
		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
	e.victim_elapsed_ns = end - start;
	e.measurement_complete = !ret;
	idma_comp_stage(v->composition_trace, v->dev, "window-return", ret);
	e.victim.in_flight_at_end = v->in_flight;
	e.aggressor.in_flight_at_end = a && a->in_flight;
	/* Stop autonomous A after V observation; hardware window is already frozen. */
	if (batch) {
		idma_batch_stop(a);
		if (ret)
			idma_batch_stop(v);
	}
	/* No more software A refills after V's final observed completion. */
	idma_snapshot(hpm, v, a, &after, active_after);
	e.snapshot_after_ns = ktime_get_ns() - end;
	idma_snapshot_delta(&e.victim, 0, &before, &after,
			    active_before[0], active_after[0]);
	if (a)
		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
				    active_before[1], active_after[1]);
	idma_comp_stage(v->composition_trace, v->dev, "snapshot-after", ret);
 out_drain:
	idma_comp_stage(v->composition_trace, v->dev, "drain-start", ret);
	if (batch) {
		idma_batch_stop(v);
		idma_batch_stop(a);
	}
	if (cfg && ret)
		idma_research2_method_abort(victim, cfg, ret);
	stamp = ktime_get_ns();
	drain_ret = idma_drain(v);
	if (drain_ret && !ret)
		ret = drain_ret;
	if (a) {
		drain_ret = idma_drain(a);
		if (drain_ret && !ret)
			ret = drain_ret;
	}
	e.drain_ns = ktime_get_ns() - stamp;
	if (batch) {
		drain_ret = idma_batch_collect(v);
		if (drain_ret && !ret)
			ret = drain_ret;
		drain_ret = idma_batch_collect(a);
		if (drain_ret && !ret)
			ret = drain_ret;
		if (!ret && e.victim.completed != e.victim_transfers)
			ret = -EIO;
	}
	idma_comp_stage(v->composition_trace, v->dev, "drain-end", ret);
	riscv_iommu_hpm_release(hpm);
	if (!ret) {
		/* Both lanes are drained: reacquire CPU visibility before any check. */
		local_flush_icache_all();
		verify_ret = idma_verify(v);
		if (verify_ret)
			ret = verify_ret;
		if (a) {
			verify_ret = idma_verify(a);
			if (verify_ret && !ret)
				ret = verify_ret;
		}
	}
 out_cleanup:
	observation_ret = idma_observation_finish(&observation, &e, ret);
	if (observation_ret && !ret)
		ret = observation_ret;
	idma_comp_stage(v->composition_trace, v->dev, "obs-finish", ret);
	if (p4m) {
		p4m_ret = idma_p4m_finish(&observation, &e, p4m, ret);
		if (p4m_ret && !ret)
			ret = p4m_ret;
	}
	if (research && research->stage) {
		int research_ret = idma_research_quiet(victim, ktime_get_ns() + NSEC_PER_SEC);

		if (research_ret) {
			if (!v->retain)
				idma_quarantine(v);
			if (a && !a->retain)
				idma_quarantine(a);
			if (!ret)
				ret = research_ret;
		}
		if (!research_ret && cfg) {
			int freeze_ret = idma_research2_method_freeze(victim, cfg, ret);

			if (freeze_ret && !ret)
				ret = freeze_ret;
		}
		research_ret = cfg ? idma_research_capture_ext(victim, research, cfg, ret) :
				     idma_research_capture(victim, research, ret);

		if (research_ret && !ret)
			ret = research_ret;
	}
	if (batch) {
		idma_batch_disable(v);
		idma_batch_disable(a);
	}
	if (composition && composition->configured) {
		int capture_ret = idma_composition_capture(victim, composition, ret);

		if (!ret)
			ret = capture_ret;
		composition->restore_status = idma_composition_restore(victim, composition);
		if (!ret)
			ret = composition->restore_status;
		idma_comp_stage(v->composition_trace, v->dev, "restored",
				composition->restore_status);
	}
	if (pair) {
		if (observation.armed) {
			observation_ret = idma_e3_finish(&observation, &e, pair->observation[round]);
			if (observation_ret && !ret)
				ret = observation_ret;
		}
		e.status = ret;
		pair->result[round] = e;
		pair->sequence[round] = observation.sequence;
		if (batch)
			idma_batch_start_log(victim->miscdev.parent, &batch_start,
					     observation.sequence, ret);
		if (!ret)
			pair->rounds_completed++;
		if (!ret && round == 0) {
			round = 1;
			idma_stream_reseed(v);
			idma_stream_reseed(a);
			memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
			e.victim_did = victim->did;
			e.aggressor_did = a ? aggressor->did : IDMA_EXPERIMENT_NO_AGGRESSOR;
			idma_put_instance(observation.control);
			observation = (struct idma_observation) { .batch = true, .e3 = true,
				.delay128 = (!diagnostic || queue_pair) && pair->first_delay == 0,
				.queue_pair = queue_pair };
			ret = idma_observation_prepare(&observation, true);
			if (ret)
				goto out_cleanup;
			goto run_round;
		}
	} else if (batch) {
		idma_batch_start_log(victim->miscdev.parent, &batch_start,
				     observation.sequence, ret);
	}
	if (a)
		idma_stream_cleanup(a);
	idma_stream_cleanup(v);
 out_put:
	idma_put_instance(observation.control);
	idma_put_instance(aggressor);
 out_unlock:
	mutex_unlock(&idma_experiment_lock);
 out_copy:
	e.status = ret;
	*request = e;
	return ret;
}

static long idma_run_session(struct idma_device *victim,
			     struct idma_experiment *request, bool batch, struct idma_e3_pair *pair,
			     struct idma_e3_diag_round *diagnostic, bool queue_pair,
			     struct idma_p4m *p4m, struct idma_research *research)
{
	return idma_run_session2(victim, request, batch, pair, diagnostic,
				 queue_pair, p4m, research, NULL, NULL);
}

static long idma_run_p4m(struct idma_device *victim, void __user *uarg)
{
	struct idma_p4m p;
	long ret;

	if (copy_from_user(&p, uarg, sizeof(p)))
		return -EFAULT;
	idma_p4m_clear_outputs(&p);
	ret = idma_p4m_validate(victim, &p);
	if (ret)
		goto out_copy;
	ret = idma_run_session(victim, &p.experiment, true, NULL, NULL, false,
			       &p, NULL);
out_copy:
	p.experiment.status = ret;
	if (copy_to_user(uarg, &p, sizeof(p)))
		ret = -EFAULT;
	return ret;
}

static long idma_run_composition(struct idma_device *victim, void __user *uarg)
{
	struct idma_composition *p;
	struct idma_experiment *e;
	long ret;

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	p = memdup_user(uarg, sizeof(*p));
	if (IS_ERR(p))
		return PTR_ERR(p);
	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_composition, status));
	idma_p4m_clear_outputs(&p->p4m);
	e = &p->p4m.experiment;
	ret = -EPROTO;
	if (p->abi_version != IDMA_COMPOSITION_ABI || p->struct_size != sizeof(*p))
		goto out;
	ret = -EINVAL;
	if (p->mode > 3 || (p->protected_did != 10 && p->protected_did != 11) ||
	    (p->victim_gap_cycles != 0 && p->victim_gap_cycles != 1000 &&
	     p->victim_gap_cycles != 4000) ||
	    memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
	    p->p4m.aggressor_gap_cycles || e->victim_pages != 1 ||
	    (e->aggressor_pages != 1 && e->aggressor_pages != 8 && e->aggressor_pages != 16) ||
	    e->victim_transfers > 4096 || e->warmup_transfers > 4096)
		goto out;
	ret = idma_p4m_validate(victim, &p->p4m);
	if (!ret)
		ret = idma_run_session2(victim, e, true, NULL, NULL, false,
				       &p->p4m, NULL, NULL, p);
 out:
	p->status = ret;
	e->status = ret;
	if (copy_to_user(uarg, p, sizeof(*p)))
		ret = -EFAULT;
	kfree(p);
	return ret;
}

static long idma_run_experiment_mode(struct idma_device *victim,
				     struct idma_experiment __user *uarg, bool batch)
{
	struct idma_experiment e;
	long ret;

	if (copy_from_user(&e, uarg, sizeof(e)))
		return -EFAULT;
	ret = idma_run_session(victim, &e, batch, NULL, NULL, false, NULL, NULL);
	return copy_to_user(uarg, &e, sizeof(e)) ? -EFAULT : ret;
}

static long idma_run_e3_pair(struct idma_device *victim, void __user *uarg, unsigned int mode)
{
	bool diagnostic = mode != 0, queue_pair = mode == 2;
	struct idma_e3_pair *p;
	struct idma_e3_diag *d;
	size_t size = diagnostic ? sizeof(*d) : sizeof(*p);
	long ret;

	p = kzalloc(size, GFP_KERNEL);
	d = (struct idma_e3_diag *)p;
	if (!p)
		return -ENOMEM;
	if (copy_from_user(p, uarg, size)) {
		ret = -EFAULT;
		goto out_free;
	}
	memset(&p->status, 0, size - offsetof(struct idma_e3_pair, status));
	if (p->abi_version != 1 || p->struct_size != sizeof(*p) || p->reserved ||
	    (p->first_delay != 0 && ((diagnostic && !queue_pair) || p->first_delay != 128)) || victim->id != 0 ||
	    (p->request.aggressor_id != 1 && p->request.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) ||
	    p->request.victim_pages != 1 ||
	    (p->request.aggressor_pages != 1 && p->request.aggressor_pages != 8) ||
	    p->request.chunk_bytes != 4096 || p->request.victim_transfers > 4096 ||
	    p->request.warmup_transfers > 4096 || p->request.run_timeout_ms > 60000) {
		ret = -EINVAL;
	} else {
		struct idma_experiment e = p->request;

		ret = idma_run_session(victim, &e, true, p, diagnostic ? d->diagnostic : NULL, queue_pair, NULL, NULL);
	}
	p->status = ret;
	if (copy_to_user(uarg, p, size))
		ret = -EFAULT;
 out_free:
	kfree(p);
	return ret;
}

static long idma_run_experiment(struct idma_device *victim,
				struct idma_experiment __user *uarg)
{
	return idma_run_experiment_mode(victim, uarg, false);
}

#include "idma_research.h"
#include "idma_sva_task.h"

static long idma_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct idma_device *idma = fp->private_data;
	struct idma_experiment_caps caps = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_SW_REFILL | IDMA_EXPERIMENT_CAP_HW_BATCH |
				IDMA_EXPERIMENT_CAP_E3_PAIR | IDMA_EXPERIMENT_CAP_E3_DIAG |
				IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR |
				IDMA_EXPERIMENT_CAP_E3_CONTROLLED |
				IDMA_EXPERIMENT_CAP_P4M | IDMA_EXPERIMENT_CAP_RESEARCH,
		.device_id = idma->id,
		.did = idma->did,
		.max_pages = IDMA_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	switch (cmd) {
	case IDMA_IOC_SVA_CAPS:
		return idma_sva_caps_ioctl(idma, (void __user *)arg);
	case IDMA_IOC_SVA_RUN:
		return idma_sva_run_ioctl(idma, (void __user *)arg);
	case IDMA_IOC_RUN_RESEARCH:
		return idma_run_research(idma, (void __user *)arg);
	case IDMA_IOC_RUN_RESEARCH2:
		return idma_run_research2(idma, (void __user *)arg);
	case IDMA_IOC_RESEARCH2_CAPS:
		return idma_research2_caps_ioctl(idma, (void __user *)arg);
	case IDMA_IOC_RUN_E3_CONTROLLED:
		return idma_run_e3_controlled(idma, (void __user *)arg);
	case IDMA_IOC_RUN_COMPOSITION:
		return idma_run_composition(idma, (void __user *)arg);
	case IDMA_IOC_RUN_P4M:
		return idma_run_p4m(idma, (void __user *)arg);
	case IDMA_IOC_RUN_E3_QUEUE_PAIR:
		return idma_run_e3_pair(idma, (void __user *)arg, 2);
	case IDMA_IOC_RUN_E3_DIAG:
		return idma_run_e3_pair(idma, (void __user *)arg, 1);
	case IDMA_IOC_RUN_E3_PAIR:
		return idma_run_e3_pair(idma, (void __user *)arg, 0);
	case IDMA_IOC_EXPERIMENT_CAPS:
		if (riscv_iommu_hpm_supported(idma->miscdev.parent))
			caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
		return copy_to_user((void __user *)arg, &caps, sizeof(caps)) ? -EFAULT : 0;
	case IDMA_IOC_RUN_BATCH:
		return idma_run_experiment_mode(idma, (struct idma_experiment __user *)arg, true);
	case IDMA_IOC_RUN_EXPERIMENT:
		return idma_run_experiment(idma, (struct idma_experiment __user *)arg);
	default:
		return -ENOTTY;
	}
}

static const struct file_operations idma_fops = {
	.owner = THIS_MODULE,
	.open = idma_open,
	.release = idma_release,
	.unlocked_ioctl = idma_ioctl,
};

static int idma_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct idma_device *idma_dev;
	struct resource *res;
	int ret;

	idma_dev = devm_kzalloc(dev, sizeof(*idma_dev), GFP_KERNEL);
	if (!idma_dev)
		return -ENOMEM;

	idma_dev->id = idma_reserve_instance_id();
	if (idma_dev->id < 0)
		return idma_dev->id;

	snprintf(idma_dev->name, sizeof(idma_dev->name), "idma%d", idma_dev->id);

	idma_dev->miscdev.fops = &idma_fops;
	idma_dev->miscdev.parent = dev;
	idma_dev->miscdev.minor = MISC_DYNAMIC_MINOR;
	idma_dev->miscdev.name = idma_dev->name;

	idma_dev->reg = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(idma_dev->reg)) {
		ret = dev_err_probe(dev, PTR_ERR(idma_dev->reg),
				    "could not map iDMA registers\n");
		goto err_unregister_id;
	}

	ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(64));
	if (ret) {
		dev_err(dev, "no suitable DMA mask\n");
		goto err_unregister_id;
	}

	ret = idma_read_did(pdev, &idma_dev->did);
	if (ret)
		goto err_unregister_id;
	dev_set_drvdata(dev, idma_dev);

	ret = misc_register(&idma_dev->miscdev);
	if (ret) {
		dev_err(dev, "could not register misc device\n");
		goto err_unregister_id;
	}

	idma_publish_instance(idma_dev);
	dev_info(dev, "registered %s\n", idma_dev->name);

	return 0;

err_unregister_id:
	idma_unregister_instance(idma_dev);
	return ret;
}

static int idma_remove(struct platform_device *pdev)
{
	struct idma_device *idma_dev = dev_get_drvdata(&pdev->dev);

	/* Do not release devm MMIO while a synchronous task owns the devices. */
	mutex_lock(&idma_experiment_lock);
	idma_unregister_instance(idma_dev);
	misc_deregister(&idma_dev->miscdev);
	mutex_unlock(&idma_experiment_lock);

	return 0;
}

static void idma_shutdown(struct platform_device *pdev)
{
}

static const struct of_device_id idma_of_match[] = {
	{ .compatible = "zdl,idma" },
	{ }
};
MODULE_DEVICE_TABLE(of, idma_of_match);

static struct platform_driver idma_driver = {
	.driver = {
		.name = "zdl,idma",
		.of_match_table = idma_of_match,
		.suppress_bind_attrs = true,
	},
	.probe = idma_probe,
	.remove = idma_remove,
	.shutdown = idma_shutdown,
};
module_driver(idma_driver, platform_driver_register,
	      platform_driver_unregister);
