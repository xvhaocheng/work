// SPDX-License-Identifier: GPL-2.0
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/iommu.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
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
#define IDMA_XFER_TIMEOUT_US 2000000
#define IDMA_MAX_DEVS 8
#define IDMA_MAX_PAGES 4096U
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U

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
	size_t size, offset, next_offset, touched_end;
	u32 chunk;
	u64 transfer_id, deadline_ns, last_complete_ns;
	bool mapped_src, mapped_dst, in_flight, retain;
	struct idma_experiment_stats *stats;
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
	s->stats->quarantined = 1;
	dev_crit(s->dev, "experiment DMA not safely drained; buffers retained until reboot\n");
}

static int idma_submit(struct idma_stream *s, bool measured)
{
	u64 now;

	s->offset = s->next_offset;
	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
	s->transfer_id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
	if (!s->transfer_id) {
		/* Without a valid ID we cannot prove that buffers are no longer used. */
		idma_quarantine(s);
		return -EIO;
	}
	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
	now = ktime_get_ns();
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
	return 0;
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
	s->in_flight = false;
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
		u8 expected = i < s->touched_end ? value : (u8)~value;
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

static long idma_run_experiment(struct idma_device *victim,
			       struct idma_experiment __user *uarg)
{
	struct idma_experiment e;
	struct idma_stream streams[2] = {};
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	u64 active_before[2], active_after[2];
	u64 start, end = 0, deadline, stamp;
	int ret, drain_ret, verify_ret;

	if (copy_from_user(&e, uarg, sizeof(e)))
		return -EFAULT;
	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
	e.victim_did = victim->did;
	e.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_validate(&e);
	if (ret)
		goto out_copy;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_copy;
	if (idma_any_quarantined()) {
		ret = -EIO;
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
	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
	if (ret)
		goto out_cleanup;
	if (a) {
		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
					  e.chunk_bytes, &e.aggressor);
		if (ret)
			goto out_cleanup;
	}
	ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
	if (ret)
		goto out_cleanup;
	e.hpm_configured = 1;
	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
	if (a) {
		ret = idma_submit(a, false);
		if (ret)
			goto out_drain;
	}
	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
	if (ret)
		goto out_drain;
	/* Every measured V task starts at the same address after warmup. */
	v->next_offset = 0;
	v->last_complete_ns = 0;
	if (a)
		a->last_complete_ns = 0;
	e.victim.in_flight_at_start = v->in_flight;
	e.aggressor.in_flight_at_start = a && a->in_flight;
	stamp = ktime_get_ns();
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e.snapshot_before_ns = start - stamp;
	e.measurement_started = 1;
	ret = idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
	e.victim_elapsed_ns = end - start;
	e.measurement_complete = !ret;
	e.victim.in_flight_at_end = v->in_flight;
	e.aggressor.in_flight_at_end = a && a->in_flight;
	/* No more A refills after V's final observed completion. */
	idma_snapshot(hpm, v, a, &after, active_after);
	e.snapshot_after_ns = ktime_get_ns() - end;
	idma_snapshot_delta(&e.victim, 0, &before, &after,
			    active_before[0], active_after[0]);
	if (a)
		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
				    active_before[1], active_after[1]);
 out_drain:
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
	riscv_iommu_hpm_release(hpm);
	if (!ret) {
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
	if (a)
		idma_stream_cleanup(a);
	idma_stream_cleanup(v);
 out_put:
	idma_put_instance(aggressor);
 out_unlock:
	mutex_unlock(&idma_experiment_lock);
 out_copy:
	e.status = ret;
	if (copy_to_user(uarg, &e, sizeof(e)))
		return -EFAULT;
	return ret;
}

static long idma_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct idma_device *idma = fp->private_data;
	struct idma_experiment_caps caps = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_SW_REFILL,
		.device_id = idma->id,
		.did = idma->did,
		.max_pages = IDMA_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	switch (cmd) {
	case IDMA_IOC_EXPERIMENT_CAPS:
		if (riscv_iommu_hpm_supported(idma->miscdev.parent))
			caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
		return copy_to_user((void __user *)arg, &caps, sizeof(caps)) ? -EFAULT : 0;
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

	idma_unregister_instance(idma_dev);
	misc_deregister(&idma_dev->miscdev);

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
