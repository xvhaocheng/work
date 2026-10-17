// SPDX-License-Identifier: GPL-2.0
#include <linux/dma-mapping.h>
#include <linux/bitmap.h>
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
#include <linux/overflow.h>
#include <linux/platform_device.h>
#include <linux/riscv_iommu_hpm.h>
#include <linux/sched/signal.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/riscv_iommu_leaf.h>
#include <uapi/linux/idma.h>
#include <uapi/linux/idma_p0.h>
#include <uapi/linux/idma_multi.h>
#include <uapi/linux/idma_pages.h>
#include <uapi/linux/idma_workload.h>
#include <asm/cacheflush.h>
#include "idma-workload.h"

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
#define IDMA_OBS_VALUES 34
#define IDMA_BATCH_MAGIC_OFFSET 0x80
#define IDMA_BATCH_CONTROL_OFFSET 0x88
#define IDMA_BATCH_TARGET_OFFSET 0x90
#define IDMA_BATCH_PAGES_OFFSET 0x98
#define IDMA_BATCH_COMPLETED_OFFSET 0xa0
#define IDMA_BATCH_ISSUED_OFFSET 0xa8
#define IDMA_BATCH_GEOMETRY_OFFSET 0xb0
#define IDMA_BATCH_ROWS_OFFSET 0xb8
#define IDMA_BATCH_PITCH_OFFSET 0xc0
#define IDMA_BATCH_MATRIX_ROWS_OFFSET 0xc8
#define IDMA_BATCH_COLUMNS_OFFSET 0xd0
#define IDMA_BATCH_MAGIC 0x49444d4142544301ULL
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

struct idma_workload_stream {
	const struct idma_workload_geometry *geometry;
	u64 warmup_issued, warmup_completed;
	bool data_verified;
	DECLARE_BITMAP(warmup, IDMA_WORKLOAD_BITMAP_BITS);
	DECLARE_BITMAP(measured, IDMA_WORKLOAD_BITMAP_BITS);
};

struct idma_workload_context {
	struct idma_workload_experiment request;
	struct idma_workload_stream streams[2];
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
	bool batch_configured, batch_started;
	struct idma_experiment_stats *stats;
	/* NULL for all older ABIs; allocated with the private ABI5 request context. */
	struct idma_workload_stream *workload;
};

struct idma_pages_context {
	struct idma_pages_experiment request;
	bool iova_assigned[2];
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

static int idma_stream_allocate(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	size_t i;

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
	return 0;
}

static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	struct page *src_page, *dst_page;
	int ret;

	ret = idma_stream_allocate(s, idma, pages, chunk, stats);
	if (ret)
		return ret;
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

static int idma_pages_verify_buffer(struct idma_stream *s,
				    struct idma_pages_buffer *buffer,
				    size_t pgsize, bool after)
{
	struct riscv_iommu_leaf_info info;
	u32 *verified = after ? &buffer->verified_after_4k_pages :
				&buffer->verified_before_4k_pages;
	unsigned int expected_shift = pgsize == SZ_2M ? 21 : PAGE_SHIFT;
	size_t off;
	int ret;

	for (off = 0; off < s->size; off += PAGE_SIZE) {
		ret = riscv_iommu_query_leaf(s->domain, buffer->iova + off, &info);
		if (ret)
			goto out;
		if (!off) {
			if (after)
				buffer->after_first_pte = info.raw_pte;
			else {
				buffer->before_first_pte = info.raw_pte;
				buffer->first_leaf_iova_base = info.leaf_iova_base;
			}
		}
		if (after)
			buffer->after_last_pte = info.raw_pte;
		else {
			buffer->before_last_pte = info.raw_pte;
			buffer->last_leaf_iova_base = info.leaf_iova_base;
		}
		if (!buffer->leaf_shift_min || info.leaf_shift < buffer->leaf_shift_min)
			buffer->leaf_shift_min = info.leaf_shift;
		buffer->leaf_shift_max = max(buffer->leaf_shift_max, info.leaf_shift);
		/* The explicit COPY mapping requires RISC-V V/R/W (bits 0..2). */
		if (info.leaf_shift != expected_shift || (info.raw_pte & 7) != 7 ||
		    info.phys != buffer->phys + off ||
		    info.leaf_iova_base != ALIGN_DOWN(buffer->iova + off, pgsize)) {
			ret = -EIO;
			goto out;
		}
		(*verified)++;
	}
	/* Check the final byte's superpage offset as well as each 4K base. */
	ret = riscv_iommu_query_leaf(s->domain, buffer->iova + s->size - 1, &info);
	if (!ret && (info.phys != buffer->phys + s->size - 1 ||
	    info.leaf_shift != expected_shift || (info.raw_pte & 7) != 7 ||
	    info.leaf_iova_base != ALIGN_DOWN(buffer->iova + s->size - 1, pgsize)))
		ret = -EIO;
out:
	if (after)
		buffer->after_error = ret;
	else
		buffer->before_error = ret;
	return ret;
}

static int idma_pages_contiguous(void *ptr, size_t bytes, phys_addr_t *phys)
{
	struct page *page;
	size_t off;

	if (!IS_ALIGNED((unsigned long)ptr, PAGE_SIZE))
		return -EINVAL;
	page = idma_buffer_page(ptr);
	if (!page)
		return -EFAULT;
	*phys = page_to_phys(page);
	if (!IS_ALIGNED(*phys, SZ_2M))
		return -EINVAL;
	for (off = 0; off < bytes; off += PAGE_SIZE) {
		page = idma_buffer_page(ptr + off);
		if (!page || page_to_phys(page) != *phys + off)
			return -EFAULT;
	}
	return 0;
}

static int idma_pages_map_buffer(struct idma_stream *s,
				 struct idma_pages_buffer *buffer, size_t pgsize)
{
	size_t off;
	int ret;

	for (off = 0; off < s->size; off += pgsize) {
		ret = iommu_map(s->domain, buffer->iova + off, buffer->phys + off,
				pgsize, IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
		if (ret)
			return ret;
		buffer->mapped_bytes += pgsize;
	}
	return 0;
}

static int idma_pages_prepare(struct idma_stream *s, struct idma_device *idma,
			      struct idma_experiment_stats *stats,
			      struct idma_pages_context *context, unsigned int lane)
{
	struct idma_pages_experiment *p = &context->request;
	struct idma_pages_buffer *buffers = &p->buffers[2 * lane];
	struct riscv_iommu_leaf_info info;
	phys_addr_t phys;
	unsigned int i;
	int ret;

	for (i = 0; i < 2; i++) {
		buffers[i].bytes = IDMA_PAGES_BUFFER_BYTES;
		buffers[i].requested_page_bytes = p->mapping_page_bytes;
	}
	ret = idma_stream_allocate(s, idma, IDMA_PAGES_BUFFER_BYTES / PAGE_SIZE,
				   SZ_4K, stats);
	if (ret)
		return ret;
	for (i = 0; i < 2; i++) {
		phys = 0;
		ret = idma_pages_contiguous(i ? s->dst : s->src, s->size, &phys);
		buffers[i].phys = phys;
		if (ret)
			return ret;
	}
	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
	if (ret)
		return ret;
	/* Reject unsupported query/domain contexts before creating a mapping. */
	ret = riscv_iommu_query_leaf(s->domain, s->src_iova, &info);
	if (ret != -ENOENT)
		return ret ? ret : -EBUSY;
	ret = riscv_iommu_query_leaf(s->domain, s->dst_iova, &info);
	if (ret != -ENOENT)
		return ret ? ret : -EBUSY;
	context->iova_assigned[lane] = true;
	buffers[0].iova = s->src_iova;
	buffers[1].iova = s->dst_iova;
	for (i = 0; i < 2; i++) {
		if (!IS_ALIGNED(buffers[i].iova, SZ_2M))
			return -EINVAL;
		ret = idma_pages_map_buffer(s, &buffers[i], p->mapping_page_bytes);
		if (ret)
			return ret;
		ret = idma_pages_verify_buffer(s, &buffers[i], p->mapping_page_bytes, false);
		if (ret)
			return ret;
		p->verified_before_mask |= BIT(2 * lane + i);
	}
	p->prepared_lane_mask |= BIT(lane);
	return 0;
}

static int idma_pages_cleanup(struct idma_stream *s,
			      struct idma_pages_context *context, unsigned int lane)
{
	struct idma_pages_experiment *p = &context->request;
	struct idma_pages_buffer *buffers = &p->buffers[2 * lane];
	struct riscv_iommu_leaf_info info;
	void **pointers[2] = { &s->src, &s->dst };
	dma_addr_t dma[2] = { s->src_dma, s->dst_dma };
	unsigned int i;
	size_t off, unmapped;
	int ret = 0, next;

	if (!s->dev)
		return 0;
	if (s->in_flight || s->retain) {
		if (!s->retain)
			idma_quarantine(s);
		p->retained_lane_mask |= BIT(lane);
		return -EIO;
	}
	p->quiet_lane_mask |= BIT(lane);
	if (context->iova_assigned[lane]) {
		for (i = 0; i < 2; i++) {
			struct idma_pages_buffer *b = &buffers[i];
			int error = 0;

			for (off = 0; off < b->mapped_bytes; off += p->mapping_page_bytes) {
				unmapped = iommu_unmap(s->domain, b->iova + off,
						       p->mapping_page_bytes);
				if (unmapped <= p->mapping_page_bytes)
					b->unmapped_bytes += unmapped;
				if (unmapped != p->mapping_page_bytes)
					error = -EIO;
			}
			b->empty_after_cleanup = 1;
			for (off = 0; off < s->size; off += PAGE_SIZE) {
				next = riscv_iommu_query_leaf(s->domain, b->iova + off, &info);
				if (next != -ENOENT) {
					b->empty_after_cleanup = 0;
					if (!error)
						error = next ? next : -EIO;
				}
			}
			b->unmap_error = error;
			if (!error)
				p->unmapped_buffer_mask |= BIT(2 * lane + i);
			else if (!ret)
				ret = error;
		}
	}
	if (ret) {
		/* A dangling mapping keeps both original lane allocations alive. */
		idma_quarantine(s);
		p->retained_lane_mask |= BIT(lane);
		return ret;
	}
	for (i = 0; i < 2; i++) {
		if (*pointers[i]) {
			dma_free_attrs(s->dev, s->size, *pointers[i], dma[i],
				       DMA_ATTR_FORCE_CONTIGUOUS);
			*pointers[i] = NULL;
			p->freed_buffer_mask |= BIT(2 * lane + i);
		}
	}
	return 0;
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
	if (s->workload && !s->batch_configured) {
		s->workload->warmup_issued++;
		s->next_offset = idma_workload_offset(s->workload->geometry,
						     s->workload->warmup_issued);
	} else {
		s->next_offset += s->chunk;
		if (s->next_offset == s->size)
			s->next_offset = 0;
	}
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
	if (s->workload && !s->batch_configured)
		s->workload->warmup_completed++;
	if (measured) {
		s->stats->completed++;
		s->stats->completed_bytes += s->chunk;
	}
	return 1;
}

static int idma_run_window_many(struct idma_stream *v,
				struct idma_stream *const aggressors[],
				unsigned int nr_aggressors, u64 count,
				u64 deadline, bool measured, u64 *end_ns)
{
	u64 completed = 0;
	u32 spins = 0;
	unsigned int i;
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
		for (i = 0; i < nr_aggressors; i++) {
			struct idma_stream *a = aggressors[i];

			ret = idma_poll(a, measured);
			if (ret < 0)
				break;
			if (ret == 1) {
				ret = idma_submit(a, measured);
				if (ret)
					break;
			}
		}
		if (ret < 0)
			break;
		if (!(++spins & 1023))
			cond_resched();
		cpu_relax();
	}
 out:
	*end_ns = ktime_get_ns();
	return ret;
}

static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
			   u64 count, u64 deadline, bool measured, u64 *end_ns)
{
	struct idma_stream *aggressors[] = { a };

	return idma_run_window_many(v, aggressors, a ? 1 : 0, count,
				   deadline, measured, end_ns);
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
	if (s->workload && !s->batch_configured)
		s->workload->warmup_completed++;
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
		bool touched = s->workload ?
			(test_bit(i / IDMA_WORKLOAD_GRANULE, s->workload->warmup) ||
			 test_bit(i / IDMA_WORKLOAD_GRANULE, s->workload->measured)) :
			i < s->touched_end;
		u8 expected = touched ? value : (u8)~value;
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
	if (s->workload)
		s->workload->data_verified = true;
	return 0;
}

/* Batch mode still uses DEFAULT read/write copies. STOP never cancels AXI. */
static bool idma_batch_supported(struct idma_device *idma, bool p0)
{
	u64 magic = readq(idma->reg + IDMA_BATCH_MAGIC_OFFSET);

	return magic == IDMA_WORKLOAD_BATCH_MAGIC || magic == IDMA_P0_BATCH_MAGIC ||
	       (!p0 && magic == IDMA_BATCH_MAGIC);
}

static int idma_batch_configure(struct idma_stream *s, u64 count, bool p0)
{
	void __iomem *base = s->idma->reg;

	if (!idma_batch_supported(s->idma, p0))
		return -EOPNOTSUPP;
	if (s->workload && readq(base + IDMA_BATCH_MAGIC_OFFSET) != IDMA_WORKLOAD_BATCH_MAGIC)
		return -EOPNOTSUPP;
	if (readq(base + IDMA_BATCH_CONTROL_OFFSET) & 2)
		return -EBUSY;
	writeq(0, base + IDMA_BATCH_CONTROL_OFFSET);
	if (s->workload) {
		const struct idma_workload_geometry *g = s->workload->geometry;

		writeq(g->geometry, base + IDMA_BATCH_GEOMETRY_OFFSET);
		writeq(g->rows_per_operation, base + IDMA_BATCH_ROWS_OFFSET);
		writeq(g->row_pitch_bytes, base + IDMA_BATCH_PITCH_OFFSET);
		writeq(g->matrix_rows, base + IDMA_BATCH_MATRIX_ROWS_OFFSET);
		writeq(g->columns, base + IDMA_BATCH_COLUMNS_OFFSET);
	}
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
	u64 status, issued, completed, bytes;

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
		if (issued != completed || issued > IDMA_EXPERIMENT_MAX_TRANSFERS)
			return -EIO;
		if (check_mul_overflow(completed, (u64)s->chunk, &bytes))
			return -EOVERFLOW;
		s->stats->completed_bytes = bytes;
		if (s->workload)
			idma_workload_prefix_bitmap(s->workload->measured,
						    s->workload->geometry, completed);
		else
			/* Old ABIs retain their contiguous-prefix validation contract. */
			s->touched_end = max(s->touched_end, (size_t)min(bytes, (u64)s->size));
	}
	return status >> 32 ? -EIO : 0;
}

static void idma_batch_disable(struct idma_stream *s)
{
	if (s && s->batch_configured && !s->retain)
		writeq(0, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
}

static int idma_batch_window_many(struct idma_stream *v,
				  struct idma_stream *const aggressors[],
				  unsigned int nr_aggressors, u64 deadline,
				  u64 *end_ns, struct idma_multi_experiment *multi)
{
	u32 spins = 0;
	unsigned int i;
	int ret;

	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}
	for (i = 0; i < nr_aggressors; i++) {
		struct idma_stream *a = aggressors[i];

		if (multi) {
			if (ktime_get_ns() >= deadline) {
				ret = -ETIMEDOUT;
				goto out;
			}
			if (signal_pending(current)) {
				ret = -EINTR;
				goto out;
			}
			multi->start_attempted_mask |= BIT(i + 1);
			multi->start_ns[i + 1] = ktime_get_ns();
		}
		a->batch_started = true;
		ret = idma_submit(a, false);
		if (ret)
			goto out;
	}
	if (multi) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			goto out;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			goto out;
		}
		multi->start_attempted_mask |= BIT(0);
		multi->start_ns[0] = ktime_get_ns();
	}
	v->batch_started = true;
	ret = idma_submit(v, false);
	if (ret)
		goto out;
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
 out:
	*end_ns = ktime_get_ns();
	return ret;
}

static int idma_batch_window(struct idma_stream *v, struct idma_stream *a,
			     u64 deadline, u64 *end_ns)
{
	struct idma_stream *aggressors[] = { a };

	return idma_batch_window_many(v, aggressors, a ? 1 : 0,
				     deadline, end_ns, NULL);
}

/* STOP/drain every started lane before any buffer can be released. */
static int idma_multi_stop(struct idma_stream streams[], unsigned int nr,
			   struct idma_multi_experiment *multi, bool check_running)
{
	unsigned int i;
	int ret = 0;

	/* Snapshot every live A before issuing any STOP; V's HW window is frozen. */
	if (check_running) {
		for (i = 1; i < nr; i++) {
			struct idma_stream *s = &streams[i];
			u64 status, issued;

			if (!s->batch_started || s->retain) {
				ret = -EIO;
				continue;
			}
			status = readq(s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
			issued = readq(s->idma->reg + IDMA_BATCH_ISSUED_OFFSET);
			multi->pre_stop_status[i] = status;
			multi->pre_stop_issued[i] = issued;
			multi->stop_status_valid_mask |= BIT(i);
			if (!(status & 2) || status >> 32 ||
			    issued >= IDMA_EXPERIMENT_MAX_TRANSFERS)
				ret = -EIO;
		}
	}
	for (i = 0; i < nr; i++) {
		struct idma_stream *s = &streams[i];

		if (!s->batch_configured || !s->batch_started || (multi->stopped_mask & BIT(i)))
			continue;
		multi->stop_ns[i] = ktime_get_ns();
		idma_batch_stop(s); /* STOP never cancels AXI or releases buffers. */
		multi->stopped_mask |= BIT(i);
	}
	return ret;
}

static int idma_multi_drain_collect(struct idma_stream streams[], unsigned int nr,
				    struct idma_multi_experiment *multi)
{
	u64 stamp = ktime_get_ns();
	unsigned int i;
	int ret = 0, next;

	for (i = 0; i < nr; i++) {
		struct idma_stream *s = &streams[i];

		if (!s->dev)
			continue;
		next = idma_drain(s); /* Existing timeout -> quarantine + retain. */
		multi->drain_end_ns[i] = ktime_get_ns();
		if (!next && !s->retain)
			multi->drained_mask |= BIT(i);
		else if (!ret)
			ret = next;
	}
	multi->drain_ns = ktime_get_ns() - stamp;
	for (i = 0; i < nr; i++) {
		struct idma_stream *s = &streams[i];

		next = idma_batch_collect(s); /* Existing error/issued=completed checks. */
		if (!next && s->batch_started &&
		    s->stats->completed_bytes != s->stats->completed * 4096ULL)
			next = -EIO;
		if (next && !ret)
			ret = next;
	}
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
MODULE_PARM_DESC(path_observe, "Require v3/v4/v5 per-transaction path timing in batch experiments");

struct idma_observation {
	struct idma_device *control;
	u64 sequence;
	bool armed, batch, path, hist, hist_v5;
};

static u64 idma_observation_sequence;

static bool idma_observer_has_path(u64 magic)
{
	return magic == IDMA_OBS_MAGIC_V3 || magic == IDMA_OBS_MAGIC_V4 || magic == IDMA_OBS_MAGIC_V5;
}

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
	o->path = o->batch && READ_ONCE(idma_path_observe);
	o->hist = o->path && (magic == IDMA_OBS_MAGIC_V4 || magic == IDMA_OBS_MAGIC_V5);
	o->hist_v5 = o->hist && magic == IDMA_OBS_MAGIC_V5;
	if (o->control->did != 10 ||
	    (!idma_observer_has_path(magic) &&
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

	if (!o->control)
		return 0;
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

/* Frozen, bounded, one-pass reads. No histogram buffer enters a timed window. */
static noinline bool idma_observation_hist_finish(struct idma_observation *o,
						void __iomem *base,
						const u64 path[16][8], bool valid)
{
	char counts[32 * 21]; /* Also retain corrupt 64-bit MMIO values in invalid logs. */
	u64 info;
	u32 overflow;
	int i, bin;

	writeq(192, base + IDMA_OBS_INDEX_OFFSET);
	info = readq(base + IDMA_OBS_DATA_OFFSET);
	overflow = (info >> 32) & 0xffff;
	if (o->hist_v5) {
		valid = valid && (info & 0xffffffffULL) == 0x20202002ULL &&
			!(info >> 56) && ((info >> 48) & 0xff) == 64 && !overflow;
		dev_info(o->control->miscdev.parent,
			 "idma_hist_info seq=%llu valid=%u version=5 schema=%u bins=%u count_bits=%u time_bits=%u depth=%u overflow=%#x raw=%#llx\n",
			 o->sequence, valid, (u32)(info & 0xff), (u32)((info >> 8) & 0xff),
			 (u32)((info >> 16) & 0xff), (u32)((info >> 24) & 0xff),
			 (u32)((info >> 48) & 0xff), overflow, info);
	} else {
		valid = valid && (info & 0xffffffffULL) == 0x20202001ULL &&
			!(info >> 48) && !overflow;
		dev_info(o->control->miscdev.parent,
			 "idma_hist_info seq=%llu valid=%u version=4 schema=%u bins=%u count_bits=%u time_bits=%u overflow=%#x raw=%#llx\n",
			 o->sequence, valid, (u32)(info & 0xff), (u32)((info >> 8) & 0xff),
			 (u32)((info >> 16) & 0xff), (u32)((info >> 24) & 0xff), overflow, info);
	}
	for (i = 0; i < 16; i++) {
		u64 total = 0;
		size_t used = 0;
		bool stage_valid = valid;

		for (bin = 0; bin < 32; bin++) {
			u64 value;

			writeq(256 + i * 32 + bin, base + IDMA_OBS_INDEX_OFFSET);
			value = readq(base + IDMA_OBS_DATA_OFFSET);
			if (value > U32_MAX)
				stage_valid = false;
			else
				total += value;
			used += scnprintf(counts + used, sizeof(counts) - used,
					  "%s%llu", bin ? "," : "", value);
		}
		stage_valid = stage_valid && total == path[i][1];
		dev_info(o->control->miscdev.parent,
			 "idma_path_hist seq=%llu role=%c channel=%c stage=%u valid=%u schema=1 bins=32 full=%llu total=%llu overflow=%u counts=%s\n",
			 o->sequence, i < 8 ? 'v' : 'a', (i / 4) % 2 ? 'w' : 'r',
			 i % 4, stage_valid, path[i][1], total, (overflow >> i) & 1, counts);
		valid = valid && stage_valid;
	}
	return valid;
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
		if (o->hist)
			valid = idma_observation_hist_finish(o, base, path, valid);
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

/* Experiment ABIs share cache handling, locks, stop/drain and validation.
 * ABI adaptation is outside this core; chunk_bytes is V's actual length.
 */
static long idma_run_experiment_mapped(struct idma_device *victim,
		struct idma_experiment *request, bool batch, u32 aggressor_chunk, bool p0,
		struct idma_pages_context *pages, struct idma_workload_context *workload)
{
	struct idma_experiment e = *request;
	struct idma_stream streams[2] = {};
	struct idma_observation observation = { .batch = batch };
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	u64 active_before[2], active_after[2];
	u64 start, end = 0, deadline, stamp;
	int ret, drain_ret, verify_ret, observation_ret;
	unsigned int i, lane;

	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
	e.victim_did = victim->did;
	e.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	/* ABI5 was bounded and expanded by its adapter before entering this core. */
	ret = workload ? (pages || !batch || p0 ? -EINVAL : 0) : idma_validate(&e);
	if (!ret && batch && ((!workload && !p0 && e.chunk_bytes != PAGE_SIZE) ||
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
		if ((pages || workload) && (aggressor->id != 1 || aggressor->did != 11)) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
	}
	ret = idma_observation_prepare(&observation, batch || READ_ONCE(idma_observe));
	if (ret)
		goto out_put;
	if (pages) {
		if (!observation.path) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		pages->request.observer_sequence = observation.sequence;
	}
	if (workload) {
		/* The new bounded geometry requires both the BTC3 producer and the
		 * OBS5/Depth64 observer. Check after clearing, under the same lock.
		 */
		if (!observation.hist_v5 || !observation.path ||
		    readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_WORKLOAD_BATCH_MAGIC ||
		    (aggressor && readq(aggressor->reg + IDMA_BATCH_MAGIC_OFFSET) !=
		     IDMA_WORKLOAD_BATCH_MAGIC)) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		writeq(192, observation.control->reg + IDMA_OBS_INDEX_OFFSET);
		if (readq(observation.control->reg + IDMA_OBS_DATA_OFFSET) !=
		    0x0040000020202002ULL) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		workload->request.observer_sequence = observation.sequence;
		v->workload = &workload->streams[0];
		if (a)
			a->workload = &workload->streams[1];
	}
	if (batch && (!idma_batch_supported(victim, p0) ||
	    (aggressor && !idma_batch_supported(aggressor, p0)))) {
		ret = -EOPNOTSUPP;
		goto out_put;
	}
	if (batch && ((readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) ||
	    (aggressor && (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET) & 2)))) {
		ret = -EBUSY;
		goto out_put;
	}
	ret = pages ? idma_pages_prepare(v, victim, &e.victim, pages, 0) :
		idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
	if (ret)
		goto out_cleanup;
	if (a) {
		ret = pages ? idma_pages_prepare(a, aggressor, &e.aggressor, pages, 1) :
			idma_stream_prepare(a, aggressor, e.aggressor_pages,
					      aggressor_chunk, &e.aggressor);
		if (ret)
			goto out_cleanup;
	}
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
	if (a && (!batch || e.warmup_transfers)) {
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
	if (batch) {
		/* Software warmup is drained before enabling the autonomous mode. */
		if (a) {
			ret = idma_drain(a);
			if (ret)
				goto out_drain;
		}
		ret = idma_batch_configure(v, e.victim_transfers, p0);
		if (!ret && a)
			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS, p0);
		if (ret)
			goto out_drain;
	}
	e.victim.in_flight_at_start = v->in_flight;
	e.aggressor.in_flight_at_start = a && a->in_flight;
	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
	if (ret)
		goto out_drain;
	stamp = ktime_get_ns();
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e.snapshot_before_ns = start - stamp;
	e.measurement_started = 1;
	ret = batch ? idma_batch_window(v, a, deadline, &end) :
		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
	e.victim_elapsed_ns = end - start;
	e.measurement_complete = !ret;
	e.victim.in_flight_at_end = v->in_flight;
	e.aggressor.in_flight_at_end = a && a->in_flight;
	if ((pages || workload) && a && a->batch_started) {
		u64 control = readq(a->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
		u64 issued = readq(a->idma->reg + IDMA_BATCH_ISSUED_OFFSET);

		if (pages) {
			pages->request.pre_stop_A_control = control;
			pages->request.pre_stop_A_issued = issued;
			pages->request.A_status_valid = 1;
		}
		if (!ret && (!(control & 2) || control >> 32 ||
		    issued >= IDMA_EXPERIMENT_MAX_TRANSFERS))
			ret = -EIO;
	}
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
 out_drain:
	if (batch) {
		idma_batch_stop(v);
		idma_batch_stop(a);
	}
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
	riscv_iommu_hpm_release(hpm);
	if (workload)
		for (lane = 0; lane < (a ? 2 : 1); lane++) {
			struct idma_workload_stream *w = &workload->streams[lane];

			idma_workload_prefix_bitmap(w->warmup, w->geometry, w->warmup_completed);
		}
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
	if (workload)
		for (lane = 0; lane < (a ? 2 : 1); lane++) {
			struct idma_workload_stream *w = &workload->streams[lane];
			struct idma_workload_experiment *p = &workload->request;
			struct idma_workload_coverage *coverage = lane ?
				&p->aggressor_coverage : &p->victim_coverage;
			u64 bytes;

			coverage->measured_distinct_bytes =
				(u64)bitmap_weight(w->measured, IDMA_WORKLOAD_BITMAP_BITS) *
				IDMA_WORKLOAD_GRANULE;
			coverage->measured_distinct_pages =
				idma_workload_distinct_pages(w->measured, w->geometry);
			coverage->warmup_distinct_pages =
				idma_workload_distinct_pages(w->warmup, w->geometry);
			if (w->data_verified)
				coverage->verified_union_bytes =
					idma_workload_union_bytes(w->warmup, w->measured);
			if (check_mul_overflow(w->warmup_completed, (u64)w->geometry->row_bytes, &bytes)) {
				bytes = 0;
				if (!ret)
					ret = -EOVERFLOW;
			}
			if (lane) {
				p->aggressor_warmup_descriptors = w->warmup_completed;
				p->aggressor_warmup_bytes = bytes;
			} else {
				p->victim_warmup_descriptors = w->warmup_completed;
				p->victim_warmup_bytes = bytes;
			}
		}
	if (pages && !v->in_flight && !v->retain &&
	    (!a || (!a->in_flight && !a->retain))) {
		for (lane = 0; lane < (a ? 2 : 1); lane++) {
			struct idma_stream *s = &streams[lane];

			if (!s->dev || !s->size || s->retain || s->in_flight)
				continue;
			for (i = 0; i < 2; i++) {
				struct idma_pages_buffer *b = &pages->request.buffers[2 * lane + i];

				if (b->mapped_bytes != s->size)
					continue;
				verify_ret = idma_pages_verify_buffer(s, b,
						pages->request.mapping_page_bytes, true);
				if (!verify_ret)
					pages->request.verified_after_mask |= BIT(2 * lane + i);
				else if (!ret)
					ret = verify_ret;
			}
		}
	}
	observation_ret = idma_observation_finish(&observation, &e, ret);
	if (observation_ret && !ret)
		ret = observation_ret;
	if (batch) {
		idma_batch_disable(v);
		idma_batch_disable(a);
	}
	if (pages) {
		if (a) {
			verify_ret = idma_pages_cleanup(a, pages, 1);
			if (verify_ret && !ret)
				ret = verify_ret;
		}
		verify_ret = idma_pages_cleanup(v, pages, 0);
		if (verify_ret && !ret)
			ret = verify_ret;
	} else {
		if (a)
			idma_stream_cleanup(a);
		idma_stream_cleanup(v);
	}
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

static long idma_run_experiment_core(struct idma_device *victim,
		struct idma_experiment *request, bool batch, u32 aggressor_chunk, bool p0)
{
	return idma_run_experiment_mapped(victim, request, batch, aggressor_chunk, p0, NULL, NULL);
}

static long idma_run_experiment_mode(struct idma_device *victim,
			       struct idma_experiment __user *uarg, bool batch)
{
	struct idma_experiment e;
	long ret;

	if (copy_from_user(&e, uarg, sizeof(e)))
		return -EFAULT;
	ret = idma_run_experiment_core(victim, &e, batch, e.chunk_bytes, false);
	if (copy_to_user(uarg, &e, sizeof(e)))
		return -EFAULT;
	return ret;
}

static int idma_validate_p0(struct idma_device *victim,
			    const struct idma_p0_experiment *e)
{
	if (e->abi_version != IDMA_P0_ABI || e->struct_size != sizeof(*e))
		return -EPROTO;
	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	/* Deliberately bounded to the approved P0 matrix, rather than a general ABI. */
	if (e->geometry != IDMA_P0_GEOMETRY_PACKED_RING || e->victim_pages != 1 ||
	    (e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR && e->aggressor_id != 1) ||
	    (e->aggressor_pages != 1 && e->aggressor_pages != 8) ||
	    (e->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR && e->aggressor_pages != 1) ||
	    (e->victim_chunk_bytes != 64 && e->victim_chunk_bytes != 1024 &&
	     e->victim_chunk_bytes != 4096) || e->aggressor_chunk_bytes != 4096 ||
	    e->victim_transfers != 4096 || e->warmup_transfers != 256 ||
	    !e->run_timeout_ms || e->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	/* Sizes are <=8 pages, V bytes <=2^24, and even A's target*len <=2^44.
	 * Per-stream collection nevertheless checks multiplication before use.
	 */
	return 0;
}

static long idma_run_p0(struct idma_device *victim,
			struct idma_p0_experiment __user *uarg)
{
	struct idma_p0_experiment p;
	struct idma_experiment e = {};
	long ret;

	if (copy_from_user(&p, uarg, sizeof(p)))
		return -EFAULT;
	memset(&p.status, 0, sizeof(p) - offsetof(struct idma_p0_experiment, status));
	p.victim_did = victim->did;
	p.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_validate_p0(victim, &p);
	if (ret)
		goto out;
	/* Translate inputs to the unchanged internal ABI1-shaped request. */
	e.abi_version = IDMA_EXPERIMENT_ABI;
	e.struct_size = sizeof(e);
	e.aggressor_id = p.aggressor_id;
	e.victim_pages = p.victim_pages;
	e.aggressor_pages = p.aggressor_pages;
	e.chunk_bytes = p.victim_chunk_bytes;
	e.run_timeout_ms = p.run_timeout_ms;
	e.victim_transfers = p.victim_transfers;
	e.warmup_transfers = p.warmup_transfers;
	ret = idma_run_experiment_core(victim, &e, true, p.aggressor_chunk_bytes, true);
	/* Named copies avoid relying on equal struct sizes to alias ABI layouts. */
	p.victim_did = e.victim_did;
	p.aggressor_did = e.aggressor_did;
	p.hpm_configured = e.hpm_configured;
	p.measurement_started = e.measurement_started;
	p.measurement_complete = e.measurement_complete;
	p.victim_elapsed_ns = e.victim_elapsed_ns;
	p.snapshot_before_ns = e.snapshot_before_ns;
	p.snapshot_after_ns = e.snapshot_after_ns;
	p.drain_ns = e.drain_ns;
	p.victim = e.victim;
	p.aggressor = e.aggressor;
 out:
	p.status = ret;
	if (copy_to_user(uarg, &p, sizeof(p)))
		return -EFAULT;
	return ret;
}

static long idma_caps_p0(struct idma_device *victim, void __user *uarg)
{
	struct idma_p0_caps caps = {
		.abi_version = IDMA_P0_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_HW_BATCH,
		.device_id = victim->id,
		.did = victim->did,
		.max_pages = IDMA_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
		.batch_magic = IDMA_P0_BATCH_MAGIC,
		.victim_length_mask = IDMA_P0_LENGTH_64 | IDMA_P0_LENGTH_1024 | IDMA_P0_LENGTH_4096,
		.aggressor_length_mask = IDMA_P0_LENGTH_4096,
		.geometry_mask = IDMA_P0_GEOMETRY_MASK_PACKED_RING,
	};

	if (victim->id != 0 || victim->did != 10 || !idma_batch_supported(victim, true))
		return -EOPNOTSUPP;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static long idma_run_experiment(struct idma_device *victim,
			       struct idma_experiment __user *uarg)
{
	return idma_run_experiment_mode(victim, uarg, false);
}

struct idma_multi_context {
	struct idma_stream streams[IDMA_MULTI_MAX_DEVICES];
	struct idma_device *devices[IDMA_MULTI_MAX_DEVICES];
	struct idma_stream *aggressors[IDMA_MULTI_MAX_DEVICES - 1];
	struct idma_observation observation;
	struct riscv_iommu_hpm_multi_sample before, after;
	u64 active_before[IDMA_MULTI_MAX_DEVICES], active_after[IDMA_MULTI_MAX_DEVICES];
};

static int idma_multi_validate(struct idma_device *victim,
			       const struct idma_multi_experiment *multi)
{
	u32 pages, same_total;
	unsigned int i;

	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (multi->abi_version != IDMA_MULTI_ABI || multi->struct_size != sizeof(*multi) ||
	    multi->flags || multi->reserved0 || multi->nr_devices < 2 ||
	    multi->nr_devices > IDMA_MULTI_MAX_DEVICES || multi->path_aggressor_index != 1 ||
	    multi->chunk_bytes != SZ_4K || multi->victim_transfers != 4096 ||
	    multi->warmup_transfers != 256 || !multi->run_timeout_ms ||
	    multi->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS || multi->pages[0] != 1)
		return -EINVAL;
	for (i = 0; i < ARRAY_SIZE(multi->reserved); i++)
		if (multi->reserved[i])
			return -EINVAL;
	pages = multi->pages[1];
	same_total = 6 / (multi->nr_devices - 1);
	if (pages != 1 && pages != 8 && pages != same_total)
		return -EINVAL;
	for (i = 0; i < IDMA_MULTI_MAX_DEVICES; i++) {
		if (i < multi->nr_devices) {
			if (multi->device_id[i] != i || (i && multi->pages[i] != pages))
				return -EINVAL;
		} else if (multi->device_id[i] || multi->pages[i]) {
			return -EINVAL;
		}
	}
	return 0;
}

static void idma_multi_snapshot(struct riscv_iommu_hpm_session *hpm,
				struct idma_multi_context *context,
				struct riscv_iommu_hpm_multi_sample *sample,
				u64 active[], unsigned int nr)
{
	unsigned int i;

	riscv_iommu_hpm_read_multi(hpm, sample);
	for (i = 0; i < nr; i++)
		active[i] = readq(context->devices[i]->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
}

static long idma_run_multi(struct idma_device *victim,
			   struct idma_multi_experiment __user *uarg)
{
	struct idma_multi_experiment multi;
	struct idma_multi_context *context;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct idma_experiment observed = {};
	u64 start, end = 0, stamp, deadline;
	unsigned int i, nr;
	int ret, next;

	if (copy_from_user(&multi, uarg, sizeof(multi)))
		return -EFAULT;
	memset(&multi.status, 0, sizeof(multi) - offsetof(struct idma_multi_experiment, status));
	ret = idma_multi_validate(victim, &multi);
	if (ret)
		goto out_copy;
	nr = multi.nr_devices;
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context) {
		ret = -ENOMEM;
		goto out_copy;
	}
	context->observation.batch = true;
	context->devices[0] = victim;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_free;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto out_put;
	}
	for (i = 0; i < nr; i++) {
		struct idma_device *device;

		if (i)
			context->devices[i] = idma_get_instance(multi.device_id[i]);
		device = context->devices[i];
		if (!device) {
			ret = -ENODEV;
			goto out_put;
		}
		multi.did[i] = device->did;
		if (device->did != 10U + i || !idma_batch_supported(device, false)) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		if (readq(device->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) {
			ret = -EBUSY;
			goto out_put;
		}
		if (i)
			context->aggressors[i - 1] = &context->streams[i];
	}
	ret = idma_observation_prepare(&context->observation, true);
	if (ret)
		goto out_put;
	if (!context->observation.path) {
		ret = -EOPNOTSUPP;
		goto out_put;
	}
	multi.observer_sequence = context->observation.sequence;
	for (i = 0; i < nr; i++) {
		ret = idma_stream_prepare(&context->streams[i], context->devices[i],
					  multi.pages[i], SZ_4K, &multi.stats[i]);
		if (ret)
			goto out_cleanup;
		multi.prepared_mask |= BIT(i);
	}
	ret = riscv_iommu_hpm_acquire_multi((struct device *const []) {
		context->streams[0].dev, context->streams[1].dev,
		context->streams[2].dev, context->streams[3].dev }, nr, &hpm);
	if (ret)
		goto out_cleanup;
	multi.hpm_configured = 1;
	local_flush_icache_all();
	deadline = ktime_get_ns() + (u64)multi.run_timeout_ms * NSEC_PER_MSEC;
	for (i = 1; i < nr; i++) {
		ret = idma_submit(&context->streams[i], false);
		if (ret)
			goto out_drain;
	}
	ret = idma_run_window_many(&context->streams[0], context->aggressors, nr - 1,
				   multi.warmup_transfers, deadline, false, &end);
	if (ret)
		goto out_drain;
	for (i = 0; i < nr; i++) {
		ret = idma_drain(&context->streams[i]);
		if (ret)
			goto out_drain;
		context->streams[i].last_complete_ns = 0;
	}
	context->streams[0].next_offset = 0;
	for (i = 0; i < nr; i++) {
		u64 target = i ? IDMA_EXPERIMENT_MAX_TRANSFERS : multi.victim_transfers;

		ret = idma_batch_configure(&context->streams[i], target, false);
		if (ret)
			goto out_drain;
	}
	for (i = 0; i < nr; i++)
		multi.stats[i].in_flight_at_start = context->streams[i].in_flight;
	ret = idma_observation_arm(&context->observation, victim, context->devices[1],
				   multi.victim_transfers);
	if (ret)
		goto out_drain;
	stamp = ktime_get_ns();
	idma_multi_snapshot(hpm, context, &context->before, context->active_before, nr);
	start = ktime_get_ns();
	multi.snapshot_before_ns = start - stamp;
	multi.measurement_started = 1;
	ret = idma_batch_window_many(&context->streams[0], context->aggressors, nr - 1,
				     deadline, &end, &multi);
	multi.victim_elapsed_ns = end - start;
	multi.measurement_complete = !ret;
	for (i = 0; i < nr; i++)
		multi.stats[i].in_flight_at_end = context->streams[i].in_flight;
	next = idma_multi_stop(context->streams, nr, &multi, !ret);
	if (next && !ret)
		ret = next;
	idma_multi_snapshot(hpm, context, &context->after, context->active_after, nr);
	multi.snapshot_after_ns = ktime_get_ns() - end;
	for (i = 0; i < nr; i++) {
		multi.stats[i].active_cycles = context->active_after[i] - context->active_before[i];
		multi.stats[i].iotlb_miss = context->after.iotlb_miss[i] -
					  context->before.iotlb_miss[i];
		multi.stats[i].s1_ptw = context->after.s1_ptw[i] - context->before.s1_ptw[i];
		multi.stats[i].s2_ptw = context->after.s2_ptw[i] - context->before.s2_ptw[i];
	}
out_drain:
	/* Cover every started lane even after a partial warmup/start or V error. */
	idma_multi_stop(context->streams, nr, &multi, false);
	next = idma_multi_drain_collect(context->streams, nr, &multi);
	if (next && !ret)
		ret = next;
	if (!ret && multi.stats[0].completed != multi.victim_transfers)
		ret = -EIO;
	riscv_iommu_hpm_release(hpm);
	hpm = NULL;
	if (!ret) {
		local_flush_icache_all();
		for (i = 0; i < nr; i++) {
			next = idma_verify(&context->streams[i]);
			if (!next)
				multi.verified_mask |= BIT(i);
			else if (!ret)
				ret = next;
		}
	}
out_cleanup:
	riscv_iommu_hpm_release(hpm);
	observed.victim_transfers = multi.victim_transfers;
	observed.victim_did = multi.did[0];
	observed.aggressor_did = multi.did[1];
	next = idma_observation_finish(&context->observation, &observed, ret);
	if (next && !ret)
		ret = next;
	for (i = nr; i > 0; i--) {
		idma_batch_disable(&context->streams[i - 1]);
		idma_stream_cleanup(&context->streams[i - 1]);
	}
out_put:
	idma_put_instance(context->observation.control);
	for (i = 1; i < nr; i++)
		idma_put_instance(context->devices[i]);
	mutex_unlock(&idma_experiment_lock);
out_free:
	kfree(context);
out_copy:
	multi.status = ret;
	if (copy_to_user(uarg, &multi, sizeof(multi)))
		return -EFAULT;
	return ret;
}

static long idma_caps_multi(struct idma_device *victim, void __user *uarg)
{
	struct idma_multi_caps caps = {
		.abi_version = IDMA_MULTI_ABI,
		.struct_size = sizeof(caps),
		.device_id = victim->id,
		.did = victim->did,
		.max_devices = IDMA_MULTI_MAX_DEVICES,
		.max_pages = IDMA_MULTI_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	if (victim->id != 0 || victim->did != 10 || PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (idma_batch_supported(victim, false))
		caps.capabilities |= IDMA_MULTI_CAP_HW_BATCH;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_MULTI_CAP_ALL_DID_HPM;
	if (idma_observer_has_path(readq(victim->reg + IDMA_OBS_MAGIC_OFFSET)))
		caps.capabilities |= IDMA_MULTI_CAP_SELECTED_PATH;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static int idma_pages_validate(struct idma_device *victim,
			       const struct idma_pages_experiment *p)
{
	if (p->abi_version != IDMA_PAGES_ABI || p->struct_size != sizeof(*p))
		return -EPROTO;
	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT))
		return -EOPNOTSUPP;
	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (p->flags || p->reserved0 || memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
	    (p->mapping_page_bytes != SZ_4K && p->mapping_page_bytes != SZ_2M) ||
	    p->buffer_bytes != IDMA_PAGES_BUFFER_BYTES || p->chunk_bytes != SZ_4K ||
	    p->victim_transfers != 4096 || p->warmup_transfers != 256 ||
	    !p->run_timeout_ms || p->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	return 0;
}

static long idma_run_pages(struct idma_device *victim,
			  struct idma_pages_experiment __user *uarg)
{
	struct idma_pages_context *context;
	struct idma_pages_experiment *p;
	struct idma_experiment *e;
	long ret;

	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;
	p = &context->request;
	if (copy_from_user(p, uarg, sizeof(*p))) {
		ret = -EFAULT;
		goto out_free;
	}
	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_pages_experiment, status));
	ret = idma_pages_validate(victim, p);
	if (ret)
		goto out_copy;
	e = &p->experiment;
	e->abi_version = IDMA_EXPERIMENT_ABI;
	e->struct_size = sizeof(*e);
	e->aggressor_id = 1;
	e->victim_pages = IDMA_PAGES_BUFFER_BYTES / PAGE_SIZE;
	e->aggressor_pages = IDMA_PAGES_BUFFER_BYTES / PAGE_SIZE;
	e->chunk_bytes = p->chunk_bytes;
	e->run_timeout_ms = p->run_timeout_ms;
	e->victim_transfers = p->victim_transfers;
	e->warmup_transfers = p->warmup_transfers;
	ret = idma_run_experiment_mapped(victim, e, true, SZ_4K, false, context, NULL);
out_copy:
	p->status = ret;
	if (copy_to_user(uarg, p, sizeof(*p)))
		ret = -EFAULT;
out_free:
	kfree(context);
	return ret;
}

static long idma_caps_pages(struct idma_device *victim, void __user *uarg)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(victim->miscdev.parent);
	struct idma_pages_caps caps = {
		.abi_version = IDMA_PAGES_ABI,
		.struct_size = sizeof(caps),
		.device_id = victim->id,
		.did = victim->did,
		.kernel_page_bytes = PAGE_SIZE,
		.buffer_bytes = IDMA_PAGES_BUFFER_BYTES,
		.chunk_bytes = SZ_4K,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (idma_batch_supported(victim, false))
		caps.capabilities |= IDMA_PAGES_CAP_BATCH;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_PAGES_CAP_HPM;
	if (idma_observer_has_path(readq(victim->reg + IDMA_OBS_MAGIC_OFFSET)))
		caps.capabilities |= IDMA_PAGES_CAP_PATH;
	if (IS_REACHABLE(CONFIG_RISCV_IOMMU) && IS_ENABLED(CONFIG_64BIT))
		caps.capabilities |= IDMA_PAGES_CAP_LEAF_API;
	if (domain) {
		if (domain->pgsize_bitmap & SZ_4K)
			caps.mapping_page_mask |= IDMA_PAGES_MAP_4K;
		if (domain->pgsize_bitmap & SZ_2M)
			caps.mapping_page_mask |= IDMA_PAGES_MAP_2M;
	}
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static int idma_workload_validate(struct idma_device *victim,
		const struct idma_workload_experiment *p, u64 *count, u64 *warmup)
{
	int ret;

	if (p->abi_version != IDMA_WORKLOAD_ABI || p->struct_size != sizeof(*p))
		return -EPROTO;
	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT) ||
	    PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (p->flags || p->reserved0 || p->reserved1 ||
	    memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
	    p->supply != IDMA_WORKLOAD_SUPPLY_HW_BATCH ||
	    (p->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR && p->aggressor_id != 1) ||
	    !p->run_timeout_ms || p->run_timeout_ms > IDMA_WORKLOAD_MAX_TIMEOUT_MS)
		return -EINVAL;
	ret = idma_workload_geometry_validate(&p->victim_geometry);
	if (ret)
		return ret;
	if (p->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) {
		if (memchr_inv(&p->aggressor_geometry, 0, sizeof(p->aggressor_geometry)))
			return -EINVAL;
	} else {
		ret = idma_workload_geometry_validate(&p->aggressor_geometry);
		if (ret)
			return ret;
	}
	if (!p->victim_operations ||
	    check_mul_overflow(p->victim_operations,
			       (u64)p->victim_geometry.rows_per_operation, count) ||
	    check_mul_overflow(p->victim_warmup_operations,
			       (u64)p->victim_geometry.rows_per_operation, warmup) ||
	    *count > IDMA_WORKLOAD_MAX_DESCRIPTORS ||
	    *warmup > IDMA_WORKLOAD_MAX_WARMUP_DESCRIPTORS)
		return -EINVAL;
	return 0;
}

static long idma_run_workload(struct idma_device *victim,
			      struct idma_workload_experiment __user *uarg)
{
	struct idma_workload_context *context;
	struct idma_workload_experiment *p;
	struct idma_experiment e = {};
	u64 count, warmup;
	long ret;

	/* Both prefix bitmaps live on the heap, outside the kernel stack and the
	 * timed path. No user pointer survives the initial request copy.
	 */
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;
	p = &context->request;
	if (copy_from_user(p, uarg, sizeof(*p))) {
		ret = -EFAULT;
		goto out_free;
	}
	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_workload_experiment, status));
	p->victim_did = victim->did;
	p->aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_workload_validate(victim, p, &count, &warmup);
	if (ret)
		goto out_copy;
	context->streams[0].geometry = &p->victim_geometry;
	context->streams[1].geometry = &p->aggressor_geometry;
	e.abi_version = IDMA_EXPERIMENT_ABI;
	e.struct_size = sizeof(e);
	e.aggressor_id = p->aggressor_id;
	e.victim_pages = p->victim_geometry.ring_pages;
	e.aggressor_pages = p->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR ? 1 :
		p->aggressor_geometry.ring_pages;
	e.chunk_bytes = p->victim_geometry.row_bytes;
	e.run_timeout_ms = p->run_timeout_ms;
	e.victim_transfers = count;
	e.warmup_transfers = warmup;
	ret = idma_run_experiment_mapped(victim, &e, true,
		p->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR ? SZ_4K :
		p->aggressor_geometry.row_bytes, false, NULL, context);
	/* Keep ABI layouts independent; completion counts are actual row COPYs. */
	p->victim_did = e.victim_did;
	p->aggressor_did = e.aggressor_did;
	p->hpm_configured = e.hpm_configured;
	p->measurement_started = e.measurement_started;
	p->measurement_complete = e.measurement_complete;
	p->victim_elapsed_ns = e.victim_elapsed_ns;
	p->snapshot_before_ns = e.snapshot_before_ns;
	p->snapshot_after_ns = e.snapshot_after_ns;
	p->drain_ns = e.drain_ns;
	p->victim = e.victim;
	p->aggressor = e.aggressor;
out_copy:
	p->status = ret;
	if (copy_to_user(uarg, p, sizeof(*p)))
		ret = -EFAULT;
out_free:
	kfree(context);
	return ret;
}

static long idma_caps_workload(struct idma_device *victim, void __user *uarg)
{
	struct idma_workload_caps caps = {
		.abi_version = IDMA_WORKLOAD_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_HW_BATCH,
		.device_id = victim->id,
		.did = victim->did,
		.max_pages = IDMA_WORKLOAD_MAX_PAGES,
		.max_rows_per_op = IDMA_WORKLOAD_MAX_ROWS,
		.max_operation_bytes = IDMA_WORKLOAD_MAX_OPERATION_BYTES,
		.batch_magic = IDMA_WORKLOAD_BATCH_MAGIC,
		.observer_magic = IDMA_WORKLOAD_OBSERVER_MAGIC,
		.geometry_mask = BIT(IDMA_WORKLOAD_LINEAR_RING) |
			BIT(IDMA_WORKLOAD_PAGE_PACKED_RING) | BIT(IDMA_WORKLOAD_TILE_ROWS),
		.row_length_mask = IDMA_WORKLOAD_LENGTH_64 | IDMA_WORKLOAD_LENGTH_256 |
			IDMA_WORKLOAD_LENGTH_1024 | IDMA_WORKLOAD_LENGTH_1536 |
			IDMA_WORKLOAD_LENGTH_4096 | IDMA_WORKLOAD_LENGTH_16384 |
			IDMA_WORKLOAD_LENGTH_65536,
		.path_queue_depth = IDMA_WORKLOAD_PATH_DEPTH,
		.max_run_timeout_ms = IDMA_WORKLOAD_MAX_TIMEOUT_MS,
	};

	/* CAPS is read-only. RUN checks exact INFO2/depth after locked reset. */
	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT) ||
	    PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10 ||
	    readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_WORKLOAD_BATCH_MAGIC ||
	    readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_WORKLOAD_OBSERVER_MAGIC)
		return -EOPNOTSUPP;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static long idma_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct idma_device *idma = fp->private_data;
	struct idma_experiment_caps caps = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_SW_REFILL | IDMA_EXPERIMENT_CAP_HW_BATCH,
		.device_id = idma->id,
		.did = idma->did,
		.max_pages = IDMA_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	switch (cmd) {
	case IDMA_IOC_CAPS_WORKLOAD:
		return idma_caps_workload(idma, (void __user *)arg);
	case IDMA_IOC_RUN_WORKLOAD:
		return idma_run_workload(idma, (struct idma_workload_experiment __user *)arg);
	case IDMA_IOC_CAPS_PAGES:
		return idma_caps_pages(idma, (void __user *)arg);
	case IDMA_IOC_RUN_BATCH_PAGES:
		return idma_run_pages(idma, (struct idma_pages_experiment __user *)arg);
	case IDMA_IOC_CAPS_MULTI:
		return idma_caps_multi(idma, (void __user *)arg);
	case IDMA_IOC_RUN_MULTI_BATCH:
		return idma_run_multi(idma, (struct idma_multi_experiment __user *)arg);
	case IDMA_IOC_CAPS_P0:
		return idma_caps_p0(idma, (void __user *)arg);
	case IDMA_IOC_RUN_BATCH_P0:
		return idma_run_p0(idma, (struct idma_p0_experiment __user *)arg);
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
