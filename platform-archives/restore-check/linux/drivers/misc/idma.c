#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/iommu.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/random.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <uapi/linux/idma.h>

MODULE_DESCRIPTION("CVA6 iDMA benchmark driver");
MODULE_AUTHOR("Manuel Rodriguez <manuel.cederog@gmail.com>");
MODULE_ALIAS("idma");
MODULE_LICENSE("GPL v2");

#define IDMA_SRC_ADDR_REG_OFFSET	0x00
#define IDMA_DST_ADDR_REG_OFFSET	0x08
#define IDMA_NUM_BYTES_REG_OFFSET	0x10
#define IDMA_CONF_REG_OFFSET		0x18
#define IDMA_STATUS_REG_OFFSET		0x20
#define IDMA_NEXT_ID_REG_OFFSET		0x28
#define IDMA_DONE_REG_OFFSET		0x30
#define IDMA_ACTIVE_CYCLES_REG_OFFSET	0x50

#define RISCV_IOMMU_REG_IOCOUNTINH	0x005c
#define RISCV_IOMMU_REG_IOHPMCYCLES	0x0060
#define RISCV_IOMMU_REG_IOHPMCTR_BASE	0x0068
#define RISCV_IOMMU_REG_IOHPMCTR(n)	\
	(RISCV_IOMMU_REG_IOHPMCTR_BASE + ((n) * 8))
#define RISCV_IOMMU_IOCOUNTINH_CY	BIT(0)
#define RISCV_IOMMU_IOHPMCYCLES_MASK	((1ULL << 63) - 1)

#define IDMA_HPM_CTR_IOTLB_MISS		1
#define IDMA_HPM_CTR_S1_PTW		3
#define IDMA_HPM_CTR_S2_PTW		4

#define IDMA_BENCH_MAX_SIZE		SZ_16M
#define IDMA_BENCH_DEFAULT_SIZE		SZ_2M
#define IDMA_BENCH_DEFAULT_CHUNK	256
#define IDMA_BENCH_PAGE_STRIDE		PAGE_SIZE
#define IDMA_XFER_TIMEOUT_US		2000000
#define IDMA_MAX_DEVS			8

struct idma_hpm_sample {
	u64 cycles;
	u64 iotlb_miss;
	u64 s1_ptw;
	u64 s2_ptw;
};

struct idma_device {
	struct miscdevice miscdev;
	struct mutex lock;
	char name[16];
	int id;
	void __iomem *reg;
	void __iomem *iommu_reg;
	bool quarantined;
};

struct idma_bench_ctx {
	struct idma_device *idma_dev;
	struct device *dev;
	struct iommu_domain *domain;
	struct idma_bench_args args;
	unsigned long attrs;
	unsigned long src_iova;
	unsigned long dst_iova;
	dma_addr_t src_dma;
	dma_addr_t dst_dma;
	dma_addr_t bench_src;
	dma_addr_t bench_dst;
	size_t size;
	size_t pgsize;
	size_t stride;
	u32 xfer_chunk;
	u32 chunks_per_page;
	u32 nr_pages;
	u32 nr_random_transfers;
	u32 *transfer_order;
	bool use_dma_api;
	bool mapped_src;
	bool mapped_dst;
	bool random_order;
	bool cleanup_safe;
	void *src;
	void *dst;
	const char *mode;
	const char *pattern;
};

struct idma_transfer {
	dma_addr_t src;
	dma_addr_t dst;
	u64 size;
	u64 id;
	bool valid;
};

struct idma_independent_stream {
	struct idma_bench_ctx *ctx;
	struct idma_transfer xfer;
	ktime_t deadline;
	u64 bytes;
	u32 iteration;
	u32 slot;
	u32 slots;
	u32 done_iters;
	bool in_flight;
	bool finished;
};

static DEFINE_MUTEX(idma_devices_lock);
static struct idma_device *idma_devices[IDMA_MAX_DEVS];
static DEFINE_MUTEX(idma_iova_lock);

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

static void idma_hpm_read(struct idma_device *idma_dev,
			  struct idma_hpm_sample *sample)
{
	memset(sample, 0, sizeof(*sample));

	if (!idma_dev->iommu_reg)
		return;

	sample->cycles = readq(idma_dev->iommu_reg +
			       RISCV_IOMMU_REG_IOHPMCYCLES) &
			 RISCV_IOMMU_IOHPMCYCLES_MASK;
	sample->iotlb_miss = readq_relaxed(idma_dev->iommu_reg +
					   RISCV_IOMMU_REG_IOHPMCTR(IDMA_HPM_CTR_IOTLB_MISS));
	sample->s1_ptw = readq_relaxed(idma_dev->iommu_reg +
				       RISCV_IOMMU_REG_IOHPMCTR(IDMA_HPM_CTR_S1_PTW));
	sample->s2_ptw = readq_relaxed(idma_dev->iommu_reg +
				       RISCV_IOMMU_REG_IOHPMCTR(IDMA_HPM_CTR_S2_PTW));
}

static void idma_hpm_delta(const struct idma_hpm_sample *before,
			   const struct idma_hpm_sample *after,
			   struct idma_hpm_sample *delta)
{
	delta->cycles = (after->cycles - before->cycles) &
			RISCV_IOMMU_IOHPMCYCLES_MASK;
	delta->iotlb_miss = after->iotlb_miss - before->iotlb_miss;
	delta->s1_ptw = after->s1_ptw - before->s1_ptw;
	delta->s2_ptw = after->s2_ptw - before->s2_ptw;
}

static void idma_hpm_init(struct platform_device *pdev,
			  struct idma_device *idma_dev)
{
	struct device *dev = &pdev->dev;
	struct of_phandle_args iommu_args;
	struct resource iommu_res;
	int ret;

	ret = of_parse_phandle_with_args(dev->of_node, "iommus", "#iommu-cells",
					 0, &iommu_args);
	if (ret)
		return;

	ret = of_address_to_resource(iommu_args.np, 0, &iommu_res);
	of_node_put(iommu_args.np);
	if (ret)
		return;

	idma_dev->iommu_reg = devm_ioremap(dev, iommu_res.start,
					   resource_size(&iommu_res));
	if (!idma_dev->iommu_reg) {
		dev_warn(dev, "could not map RISC-V IOMMU HPM registers\n");
		return;
	}

	writel(readl(idma_dev->iommu_reg + RISCV_IOMMU_REG_IOCOUNTINH) &
	       ~RISCV_IOMMU_IOCOUNTINH_CY,
	       idma_dev->iommu_reg + RISCV_IOMMU_REG_IOCOUNTINH);
}

static u64 idma_active_cycles_read(struct idma_device *idma_dev)
{
	return readq_relaxed(idma_dev->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
}

static void idma_fill_test_pattern(void *buf, size_t size)
{
	u8 *p = buf;
	size_t i;

	for (i = 0; i < size; i++)
		p[i] = (u8)(0xa5 ^ i ^ (i >> 8));
}

static struct page *idma_virt_to_page(void *addr)
{
	if (is_vmalloc_addr(addr))
		return vmalloc_to_page(addr);

	return virt_to_page(addr);
}

static int idma_submit_transfer(struct idma_device *idma_dev, dma_addr_t src,
				dma_addr_t dst, u64 size, u64 *transfer_id)
{
	if (unlikely(READ_ONCE(idma_dev->quarantined)))
		return -EIO;

	writeq(src, idma_dev->reg + IDMA_SRC_ADDR_REG_OFFSET);
	writeq(dst, idma_dev->reg + IDMA_DST_ADDR_REG_OFFSET);
	writeq(size, idma_dev->reg + IDMA_NUM_BYTES_REG_OFFSET);
	writeq(0, idma_dev->reg + IDMA_CONF_REG_OFFSET);

	*transfer_id = readq(idma_dev->reg + IDMA_NEXT_ID_REG_OFFSET);
	if (!*transfer_id)
		return -EIO;

	return 0;
}

static int idma_wait_transfer(struct idma_device *idma_dev, dma_addr_t src,
			      dma_addr_t dst, u64 size, u64 transfer_id)
{
	u64 done;
	int ret;

	ret = readq_poll_timeout(idma_dev->reg + IDMA_DONE_REG_OFFSET, done,
				 done == transfer_id, 0, IDMA_XFER_TIMEOUT_US);
	if (ret) {
		dev_err(idma_dev->miscdev.parent,
			"timeout: src=%pad dst=%pad size=%llu id=%llu done=%llu status=%llx\n",
			&src, &dst, (unsigned long long)size,
			(unsigned long long)transfer_id,
			(unsigned long long)done,
			(unsigned long long)readq(idma_dev->reg +
						  IDMA_STATUS_REG_OFFSET));
	}

	return ret;
}

static int idma_start_transfer(struct idma_device *idma_dev, dma_addr_t src,
			       dma_addr_t dst, u64 size)
{
	u64 transfer_id;
	int ret;

	ret = idma_submit_transfer(idma_dev, src, dst, size, &transfer_id);
	if (ret)
		return ret;

	return idma_wait_transfer(idma_dev, src, dst, size, transfer_id);
}

static int idma_start_transfer_range(struct idma_bench_ctx *ctx, u64 *bytes)
{
	size_t off;
	int ret;

	for (off = 0; off < ctx->size; off += ctx->xfer_chunk) {
		size_t len = min_t(size_t, ctx->xfer_chunk, ctx->size - off);

		ret = idma_start_transfer(ctx->idma_dev, ctx->bench_src + off,
					  ctx->bench_dst + off, len);
		if (ret)
			return ret;

		*bytes += len;
	}

	return 0;
}

static void idma_bench_random_order(u32 *order, u32 nr_entries)
{
	u32 i;

	for (i = 0; i < nr_entries; i++)
		order[i] = i;

	if (!nr_entries)
		return;

	for (i = nr_entries - 1; i > 0; i--) {
		u32 j = get_random_u32_below(i + 1);

		swap(order[i], order[j]);
	}
}

static int idma_start_transfer_random_chunks(struct idma_bench_ctx *ctx,
					     u64 *bytes)
{
	u32 i;
	int ret;

	for (i = 0; i < ctx->nr_random_transfers; i++) {
		u32 transfer = ctx->transfer_order[i];
		u32 page = transfer / ctx->chunks_per_page;
		u32 chunk = transfer % ctx->chunks_per_page;
		size_t off = (size_t)page * IDMA_BENCH_PAGE_STRIDE +
			     (size_t)chunk * ctx->xfer_chunk;
		size_t len;

		if (off >= ctx->size)
			continue;

		len = min_t(size_t, ctx->xfer_chunk, ctx->size - off);
		ret = idma_start_transfer(ctx->idma_dev, ctx->bench_src + off,
					  ctx->bench_dst + off, len);
		if (ret)
			return ret;

		*bytes += len;
	}

	return 0;
}

static u32 idma_bench_transfer_slots(const struct idma_bench_ctx *ctx)
{
	if (ctx->random_order)
		return ctx->nr_random_transfers;

	return DIV_ROUND_UP(ctx->size, ctx->xfer_chunk);
}

static bool idma_bench_transfer_at(const struct idma_bench_ctx *ctx,
				   u32 slot, struct idma_transfer *xfer)
{
	size_t off;
	size_t len;

	memset(xfer, 0, sizeof(*xfer));

	if (ctx->random_order) {
		u32 transfer;
		u32 page;
		u32 chunk;

		if (slot >= ctx->nr_random_transfers)
			return false;

		transfer = ctx->transfer_order[slot];
		page = transfer / ctx->chunks_per_page;
		chunk = transfer % ctx->chunks_per_page;
		off = (size_t)page * IDMA_BENCH_PAGE_STRIDE +
		      (size_t)chunk * ctx->xfer_chunk;
	} else {
		off = (size_t)slot * ctx->xfer_chunk;
	}

	if (off >= ctx->size)
		return false;

	len = min_t(size_t, ctx->xfer_chunk, ctx->size - off);
	xfer->src = ctx->bench_src + off;
	xfer->dst = ctx->bench_dst + off;
	xfer->size = len;
	xfer->valid = true;

	return true;
}

static bool idma_bench_verify_random_chunks(const struct idma_bench_ctx *ctx)
{
	const u8 *src = ctx->src;
	const u8 *dst = ctx->dst;
	size_t page_off;
	u32 chunk;

	for (page_off = 0; page_off < ctx->size;
	     page_off += IDMA_BENCH_PAGE_STRIDE) {
		for (chunk = 0; chunk < ctx->chunks_per_page; chunk++) {
			size_t off = page_off + (size_t)chunk * ctx->xfer_chunk;
			size_t len;

			if (off >= ctx->size)
				return true;

			len = min_t(size_t, ctx->xfer_chunk, ctx->size - off);
			if (memcmp(src + off, dst + off, len))
				return false;
		}
	}

	return true;
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

	if (end < start || span < size || span > end - start + 1)
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

static void idma_bench_clear_results(struct idma_bench_args *args)
{
	args->bytes = 0;
	args->cycles = 0;
	args->iotlb_miss = 0;
	args->s1_ptw = 0;
	args->s2_ptw = 0;
	args->dma_active_cycles = 0;
	args->errors = 0;
	args->reserved = 0;
}

static int idma_bench_prepare(struct idma_bench_ctx *ctx,
			      struct idma_device *idma_dev,
			      const struct idma_bench_args *args,
			      u32 xfer_chunk)
{
	struct page *src_page;
	struct page *dst_page;
	phys_addr_t src_phys;
	phys_addr_t dst_phys;
	int ret;

	memset(ctx, 0, sizeof(*ctx));
	ctx->idma_dev = idma_dev;
	ctx->dev = idma_dev->miscdev.parent;
	ctx->domain = iommu_get_domain_for_dev(ctx->dev);
	ctx->args = *args;
	ctx->attrs = DMA_ATTR_FORCE_CONTIGUOUS;
	ctx->cleanup_safe = true;
	idma_bench_clear_results(&ctx->args);

	if (!ctx->domain)
		return -ENODEV;

	if (!ctx->args.iterations)
		ctx->args.iterations = 1;

	ctx->size = ctx->args.size ?: IDMA_BENCH_DEFAULT_SIZE;
	ctx->args.size = ctx->size;
	if (ctx->size > IDMA_BENCH_MAX_SIZE)
		return -EINVAL;

	xfer_chunk = xfer_chunk ?: IDMA_BENCH_DEFAULT_CHUNK;
	if (xfer_chunk > PAGE_SIZE || PAGE_SIZE % xfer_chunk)
		return -EINVAL;
	ctx->xfer_chunk = xfer_chunk;
	ctx->chunks_per_page = PAGE_SIZE / xfer_chunk;

	ctx->random_order = ctx->args.flags & IDMA_BENCH_F_RANDOM;
	ctx->pattern = ctx->random_order ? "random" : "linear";
	ctx->stride = ctx->random_order ? IDMA_BENCH_PAGE_STRIDE : xfer_chunk;
	ctx->nr_pages = DIV_ROUND_UP(ctx->size, IDMA_BENCH_PAGE_STRIDE);
	ctx->nr_random_transfers = ctx->nr_pages * ctx->chunks_per_page;

	ctx->use_dma_api = ctx->args.flags & IDMA_BENCH_F_DMA_API;
	if (ctx->use_dma_api) {
		ctx->mode = "dma";
		ctx->pgsize = PAGE_SIZE;
	} else if (ctx->args.flags & IDMA_BENCH_F_HUGE) {
		if (!IS_ALIGNED(ctx->size, SZ_2M))
			return -EINVAL;
		ctx->mode = "2m";
		ctx->pgsize = SZ_2M;
	} else {
		if (!IS_ALIGNED(ctx->size, PAGE_SIZE))
			return -EINVAL;
		ctx->mode = "4k";
		ctx->pgsize = PAGE_SIZE;
	}

	ctx->src = dma_alloc_attrs(ctx->dev, ctx->size, &ctx->src_dma,
				   GFP_KERNEL, ctx->attrs);
	if (!ctx->src)
		return -ENOMEM;

	ctx->dst = dma_alloc_attrs(ctx->dev, ctx->size, &ctx->dst_dma,
				   GFP_KERNEL, ctx->attrs);
	if (!ctx->dst) {
		ret = -ENOMEM;
		goto err_cleanup;
	}

	memset(ctx->src, 0xa5, ctx->size);
	memset(ctx->dst, 0, ctx->size);

	if (ctx->use_dma_api) {
		ctx->bench_src = ctx->src_dma;
		ctx->bench_dst = ctx->dst_dma;
	} else {
		src_page = idma_virt_to_page(ctx->src);
		dst_page = idma_virt_to_page(ctx->dst);
		if (!src_page || !dst_page) {
			ret = -EFAULT;
			goto err_cleanup;
		}

		src_phys = page_to_phys(src_page);
		dst_phys = page_to_phys(dst_page);
		if (!IS_ALIGNED(src_phys | dst_phys, ctx->pgsize)) {
			ret = -EINVAL;
			goto err_cleanup;
		}

		mutex_lock(&idma_iova_lock);
		ret = idma_find_iova(ctx->domain, ctx->size, &ctx->src_iova,
				     &ctx->dst_iova);
		if (!ret) {
			ret = idma_iommu_map_range(ctx->domain, ctx->src_iova,
						   src_phys, ctx->size,
						   ctx->pgsize);
			if (!ret)
				ctx->mapped_src = true;
		}
		if (!ret) {
			ret = idma_iommu_map_range(ctx->domain, ctx->dst_iova,
						   dst_phys, ctx->size,
						   ctx->pgsize);
			if (!ret)
				ctx->mapped_dst = true;
		}
		mutex_unlock(&idma_iova_lock);
		if (ret)
			goto err_cleanup;

		ctx->bench_src = ctx->src_iova;
		ctx->bench_dst = ctx->dst_iova;
	}

	if (ctx->random_order) {
		ctx->transfer_order = kvmalloc_array(ctx->nr_random_transfers,
						     sizeof(*ctx->transfer_order),
						     GFP_KERNEL);
		if (!ctx->transfer_order) {
			ret = -ENOMEM;
			goto err_cleanup;
		}
		idma_bench_random_order(ctx->transfer_order,
					ctx->nr_random_transfers);
	}

	return 0;

err_cleanup:
	if (ctx->mapped_src || ctx->mapped_dst) {
		mutex_lock(&idma_iova_lock);
		if (ctx->mapped_dst)
			idma_iommu_unmap_range(ctx->domain, ctx->dst_iova,
					       ctx->size, ctx->pgsize);
		if (ctx->mapped_src)
			idma_iommu_unmap_range(ctx->domain, ctx->src_iova,
					       ctx->size, ctx->pgsize);
		mutex_unlock(&idma_iova_lock);
	}
	if (ctx->dst)
		dma_free_attrs(ctx->dev, ctx->size, ctx->dst, ctx->dst_dma,
			       ctx->attrs);
	if (ctx->src)
		dma_free_attrs(ctx->dev, ctx->size, ctx->src, ctx->src_dma,
			       ctx->attrs);

	return ret;
}

static void idma_bench_log_start(const struct idma_bench_ctx *ctx,
				 const char *label)
{
	dev_info(ctx->dev,
		 "%s %s/%s start: size=%zu chunk=%u stride=%zu iter=%u src=%pad dst=%pad\n",
		 label, ctx->mode, ctx->pattern, ctx->size, ctx->xfer_chunk,
		 ctx->stride, ctx->args.iterations, &ctx->bench_src,
		 &ctx->bench_dst);
}

static void idma_bench_log_result(const struct idma_bench_ctx *ctx,
				  const char *label, int ret)
{
	dev_info(ctx->dev,
		 "%s %s/%s: size=%zu chunk=%u stride=%zu iter=%u bytes=%llu cycles=%llu active=%llu iotlb_miss=%llu s1_ptw=%llu s2_ptw=%llu errors=%u ret=%d\n",
		 label, ctx->mode, ctx->pattern, ctx->size, ctx->xfer_chunk,
		 ctx->stride, ctx->args.iterations,
		 (unsigned long long)ctx->args.bytes,
		 (unsigned long long)ctx->args.cycles,
		 (unsigned long long)ctx->args.dma_active_cycles,
		 (unsigned long long)ctx->args.iotlb_miss,
		 (unsigned long long)ctx->args.s1_ptw,
		 (unsigned long long)ctx->args.s2_ptw,
		 ctx->args.errors, ret);
}

static void idma_bench_verify_result(struct idma_bench_ctx *ctx)
{
	if (!(ctx->args.flags & IDMA_BENCH_F_VERIFY))
		return;

	if (ctx->random_order) {
		if (!idma_bench_verify_random_chunks(ctx))
			ctx->args.errors++;
	} else if (memcmp(ctx->src, ctx->dst, ctx->size)) {
		ctx->args.errors++;
	}
}

static int idma_bench_run(struct idma_bench_ctx *ctx, const char *label)
{
	struct idma_hpm_sample before;
	struct idma_hpm_sample after;
	struct idma_hpm_sample delta;
	u64 active_before;
	u64 active_after;
	u64 bytes = 0;
	u32 i;
	u32 done_iters = 0;
	int ret = 0;

	idma_bench_log_start(ctx, label);

	mutex_lock(&ctx->idma_dev->lock);
	active_before = idma_active_cycles_read(ctx->idma_dev);
	idma_hpm_read(ctx->idma_dev, &before);
	for (i = 0; i < ctx->args.iterations; i++) {
		if (ctx->random_order)
			ret = idma_start_transfer_random_chunks(ctx, &bytes);
		else
			ret = idma_start_transfer_range(ctx, &bytes);
		if (ret)
			break;
		done_iters++;
	}
	idma_hpm_read(ctx->idma_dev, &after);
	active_after = idma_active_cycles_read(ctx->idma_dev);
	mutex_unlock(&ctx->idma_dev->lock);

	idma_hpm_delta(&before, &after, &delta);
	ctx->args.iterations = done_iters;
	ctx->args.bytes = bytes;
	ctx->args.cycles = delta.cycles;
	ctx->args.iotlb_miss = delta.iotlb_miss;
	ctx->args.s1_ptw = delta.s1_ptw;
	ctx->args.s2_ptw = delta.s2_ptw;
	ctx->args.dma_active_cycles = active_after - active_before;

	if (!ret)
		idma_bench_verify_result(ctx);

	idma_bench_log_result(ctx, label, ret);

	return ret;
}

static void idma_lock_pair(struct idma_device *a, struct idma_device *b)
{
	if (a->id < b->id) {
		mutex_lock(&a->lock);
		mutex_lock(&b->lock);
	} else {
		mutex_lock(&b->lock);
		mutex_lock(&a->lock);
	}
}

static void idma_unlock_pair(struct idma_device *a, struct idma_device *b)
{
	if (a->id < b->id) {
		mutex_unlock(&b->lock);
		mutex_unlock(&a->lock);
	} else {
		mutex_unlock(&a->lock);
		mutex_unlock(&b->lock);
	}
}

static void idma_lock_multi(struct idma_bench_ctx *ctxs, u32 nr_devs)
{
	int id;
	u32 i;

	for (id = 0; id < IDMA_MAX_DEVS; id++) {
		for (i = 0; i < nr_devs; i++) {
			if (ctxs[i].idma_dev->id == id)
				mutex_lock(&ctxs[i].idma_dev->lock);
		}
	}
}

static void idma_unlock_multi(struct idma_bench_ctx *ctxs, u32 nr_devs)
{
	int id;
	u32 i;

	for (id = IDMA_MAX_DEVS - 1; id >= 0; id--) {
		for (i = 0; i < nr_devs; i++) {
			if (ctxs[i].idma_dev->id == id)
				mutex_unlock(&ctxs[i].idma_dev->lock);
		}
	}
}

static int
idma_bench_run_pair_lockstep(struct idma_bench_ctx *victim_ctx,
			     struct idma_bench_ctx *aggressor_ctx)
{
	struct idma_hpm_sample before;
	struct idma_hpm_sample after;
	struct idma_hpm_sample delta;
	u64 victim_active_before;
	u64 victim_active_after;
	u64 aggressor_active_before;
	u64 aggressor_active_after;
	u64 victim_bytes = 0;
	u64 aggressor_bytes = 0;
	u32 victim_slots = idma_bench_transfer_slots(victim_ctx);
	u32 aggressor_slots = idma_bench_transfer_slots(aggressor_ctx);
	u32 slots = max(victim_slots, aggressor_slots);
	u32 victim_iters = victim_ctx->args.iterations;
	u32 aggressor_iters = aggressor_ctx->args.iterations;
	u32 pair_iters = max(victim_iters, aggressor_iters);
	u32 victim_done_iters = 0;
	u32 aggressor_done_iters = 0;
	u32 i;
	int ret = 0;

	idma_bench_log_start(victim_ctx, "pair lockstep victim");
	idma_bench_log_start(aggressor_ctx, "pair lockstep aggressor");

	idma_lock_pair(victim_ctx->idma_dev, aggressor_ctx->idma_dev);
	victim_active_before = idma_active_cycles_read(victim_ctx->idma_dev);
	aggressor_active_before =
		idma_active_cycles_read(aggressor_ctx->idma_dev);
	idma_hpm_read(victim_ctx->idma_dev, &before);

	for (i = 0; i < pair_iters; i++) {
		bool victim_enabled = i < victim_iters;
		bool aggressor_enabled = i < aggressor_iters;
		u32 slot;

		for (slot = 0; slot < slots; slot++) {
			struct idma_transfer victim_xfer = {};
			struct idma_transfer aggressor_xfer = {};
			int victim_ret = 0;
			int aggressor_ret = 0;

			if (victim_enabled)
				idma_bench_transfer_at(victim_ctx, slot,
						       &victim_xfer);
			if (aggressor_enabled)
				idma_bench_transfer_at(aggressor_ctx, slot,
						       &aggressor_xfer);
			if (!victim_xfer.valid && !aggressor_xfer.valid)
				continue;

			if (aggressor_xfer.valid) {
				ret = idma_submit_transfer(aggressor_ctx->idma_dev,
							   aggressor_xfer.src,
							   aggressor_xfer.dst,
							   aggressor_xfer.size,
							   &aggressor_xfer.id);
			}
			if (!ret && victim_xfer.valid) {
				ret = idma_submit_transfer(victim_ctx->idma_dev,
							   victim_xfer.src,
							   victim_xfer.dst,
							   victim_xfer.size,
							   &victim_xfer.id);
			}

			if (victim_xfer.valid && victim_xfer.id) {
				victim_ret =
					idma_wait_transfer(victim_ctx->idma_dev,
							   victim_xfer.src,
							   victim_xfer.dst,
							   victim_xfer.size,
							   victim_xfer.id);
				if (!victim_ret)
					victim_bytes += victim_xfer.size;
			}
			if (aggressor_xfer.valid && aggressor_xfer.id) {
				aggressor_ret =
					idma_wait_transfer(aggressor_ctx->idma_dev,
							   aggressor_xfer.src,
							   aggressor_xfer.dst,
							   aggressor_xfer.size,
							   aggressor_xfer.id);
				if (!aggressor_ret)
					aggressor_bytes += aggressor_xfer.size;
			}

			if (ret || victim_ret || aggressor_ret) {
				if (!ret)
					ret = victim_ret ? victim_ret : aggressor_ret;
				goto out_measure;
			}
		}

		if (victim_enabled)
			victim_done_iters++;
		if (aggressor_enabled)
			aggressor_done_iters++;
	}

out_measure:
	idma_hpm_read(victim_ctx->idma_dev, &after);
	victim_active_after = idma_active_cycles_read(victim_ctx->idma_dev);
	aggressor_active_after =
		idma_active_cycles_read(aggressor_ctx->idma_dev);
	idma_unlock_pair(victim_ctx->idma_dev, aggressor_ctx->idma_dev);

	idma_hpm_delta(&before, &after, &delta);

	victim_ctx->args.iterations = victim_done_iters;
	victim_ctx->args.bytes = victim_bytes;
	victim_ctx->args.cycles = delta.cycles;
	victim_ctx->args.iotlb_miss = delta.iotlb_miss;
	victim_ctx->args.s1_ptw = delta.s1_ptw;
	victim_ctx->args.s2_ptw = delta.s2_ptw;
	victim_ctx->args.dma_active_cycles =
		victim_active_after - victim_active_before;

	aggressor_ctx->args.iterations = aggressor_done_iters;
	aggressor_ctx->args.bytes = aggressor_bytes;
	aggressor_ctx->args.cycles = delta.cycles;
	aggressor_ctx->args.iotlb_miss = delta.iotlb_miss;
	aggressor_ctx->args.s1_ptw = delta.s1_ptw;
	aggressor_ctx->args.s2_ptw = delta.s2_ptw;
	aggressor_ctx->args.dma_active_cycles =
		aggressor_active_after - aggressor_active_before;

	if (!ret) {
		idma_bench_verify_result(victim_ctx);
		idma_bench_verify_result(aggressor_ctx);
	}

	idma_bench_log_result(victim_ctx, "pair lockstep victim", ret);
	idma_bench_log_result(aggressor_ctx, "pair lockstep aggressor", ret);

	return ret;
}

static void
idma_independent_stream_init(struct idma_independent_stream *stream,
			     struct idma_bench_ctx *ctx)
{
	memset(stream, 0, sizeof(*stream));
	stream->ctx = ctx;
	stream->slots = idma_bench_transfer_slots(ctx);
}

static int
idma_independent_stream_submit_next(struct idma_independent_stream *stream)
{
	struct idma_bench_ctx *ctx = stream->ctx;
	int ret;

	while (stream->iteration < ctx->args.iterations) {
		if (stream->slot >= stream->slots) {
			stream->done_iters++;
			stream->iteration++;
			stream->slot = 0;
			continue;
		}

		if (!idma_bench_transfer_at(ctx, stream->slot++,
					    &stream->xfer))
			continue;

		ret = idma_submit_transfer(ctx->idma_dev, stream->xfer.src,
					   stream->xfer.dst,
					   stream->xfer.size,
					   &stream->xfer.id);
		if (ret)
			return ret;

		stream->deadline =
			ktime_add_us(ktime_get(), IDMA_XFER_TIMEOUT_US);
		stream->in_flight = true;
		return 0;
	}

	stream->finished = true;
	return 0;
}

static int
idma_independent_stream_poll(struct idma_independent_stream *stream,
			     ktime_t now)
{
	struct idma_bench_ctx *ctx = stream->ctx;
	u64 done;

	if (!stream->in_flight)
		return 0;

	done = readq(ctx->idma_dev->reg + IDMA_DONE_REG_OFFSET);
	if (done == stream->xfer.id) {
		stream->bytes += stream->xfer.size;
		stream->in_flight = false;
		return idma_independent_stream_submit_next(stream);
	}

	if (ktime_compare(now, stream->deadline) <= 0)
		return 0;

	dev_err(ctx->dev,
		"timeout: src=%pad dst=%pad size=%llu id=%llu done=%llu status=%llx\n",
		&stream->xfer.src, &stream->xfer.dst,
		(unsigned long long)stream->xfer.size,
		(unsigned long long)stream->xfer.id,
		(unsigned long long)done,
		(unsigned long long)readq(ctx->idma_dev->reg +
					  IDMA_STATUS_REG_OFFSET));

	return -ETIMEDOUT;
}

static bool
idma_independent_stream_drain(struct idma_independent_stream *stream)
{
	int ret;

	if (!stream->in_flight)
		return true;

	ret = idma_wait_transfer(stream->ctx->idma_dev, stream->xfer.src,
				 stream->xfer.dst, stream->xfer.size,
				 stream->xfer.id);
	if (ret) {
		/*
		 * With no hardware cancel/reset, retaining the mappings and
		 * buffers is safer than freeing memory that a wedged DMA may
		 * still access.  Keep the device quarantined until reboot.
		 */
		stream->ctx->cleanup_safe = false;
		WRITE_ONCE(stream->ctx->idma_dev->quarantined, true);
		dev_crit(stream->ctx->dev,
			 "DMA did not drain; device quarantined and benchmark buffers retained\n");
		return false;
	}

	stream->bytes += stream->xfer.size;
	stream->in_flight = false;
	return true;
}

static int
idma_bench_run_pair_independent(struct idma_bench_ctx *victim_ctx,
				struct idma_bench_ctx *aggressor_ctx)
{
	struct idma_independent_stream victim;
	struct idma_independent_stream aggressor;
	struct idma_independent_stream *streams[2] = {
		&victim,
		&aggressor,
	};
	struct idma_hpm_sample before;
	struct idma_hpm_sample after;
	struct idma_hpm_sample delta;
	u64 victim_active_before;
	u64 victim_active_after;
	u64 aggressor_active_before;
	u64 aggressor_active_after;
	u32 first = 0;
	int ret;

	idma_independent_stream_init(&victim, victim_ctx);
	idma_independent_stream_init(&aggressor, aggressor_ctx);

	idma_bench_log_start(victim_ctx, "pair independent victim");
	idma_bench_log_start(aggressor_ctx, "pair independent aggressor");

	idma_lock_pair(victim_ctx->idma_dev, aggressor_ctx->idma_dev);
	victim_active_before = idma_active_cycles_read(victim_ctx->idma_dev);
	aggressor_active_before =
		idma_active_cycles_read(aggressor_ctx->idma_dev);
	idma_hpm_read(victim_ctx->idma_dev, &before);

	if (READ_ONCE(victim_ctx->idma_dev->quarantined) ||
	    READ_ONCE(aggressor_ctx->idma_dev->quarantined)) {
		ret = -EIO;
		goto out_measure;
	}

	/*
	 * Start the aggressor first, as in the lockstep benchmark.  From this
	 * point on each engine is refilled independently as soon as its current
	 * transfer completes.
	 */
	ret = idma_independent_stream_submit_next(&aggressor);
	if (!ret)
		ret = idma_independent_stream_submit_next(&victim);

	while (!ret && (!victim.finished || !aggressor.finished)) {
		ktime_t now = ktime_get();
		u32 scanned;

		for (scanned = 0; scanned < ARRAY_SIZE(streams); scanned++) {
			u32 index = (first + scanned) % ARRAY_SIZE(streams);

			ret = idma_independent_stream_poll(streams[index], now);
			if (ret)
				break;
		}
		first ^= 1;
		cpu_relax();
	}

	if (ret) {
		bool aggressor_drained;
		bool victim_drained;

		/*
		 * Do not unmap/free a buffer while a successfully submitted DMA
		 * may still be using it.  A stream that cannot be drained marks
		 * its context unsafe for cleanup and quarantines that engine.
		 */
		victim_drained = idma_independent_stream_drain(&victim);
		aggressor_drained = idma_independent_stream_drain(&aggressor);
		if (!victim_drained || !aggressor_drained)
			ret = -ETIMEDOUT;
	}

out_measure:
	idma_hpm_read(victim_ctx->idma_dev, &after);
	victim_active_after = idma_active_cycles_read(victim_ctx->idma_dev);
	aggressor_active_after =
		idma_active_cycles_read(aggressor_ctx->idma_dev);
	idma_unlock_pair(victim_ctx->idma_dev, aggressor_ctx->idma_dev);

	idma_hpm_delta(&before, &after, &delta);

	victim_ctx->args.iterations = victim.done_iters;
	victim_ctx->args.bytes = victim.bytes;
	victim_ctx->args.cycles = delta.cycles;
	victim_ctx->args.iotlb_miss = delta.iotlb_miss;
	victim_ctx->args.s1_ptw = delta.s1_ptw;
	victim_ctx->args.s2_ptw = delta.s2_ptw;
	victim_ctx->args.dma_active_cycles =
		victim_active_after - victim_active_before;

	aggressor_ctx->args.iterations = aggressor.done_iters;
	aggressor_ctx->args.bytes = aggressor.bytes;
	aggressor_ctx->args.cycles = delta.cycles;
	aggressor_ctx->args.iotlb_miss = delta.iotlb_miss;
	aggressor_ctx->args.s1_ptw = delta.s1_ptw;
	aggressor_ctx->args.s2_ptw = delta.s2_ptw;
	aggressor_ctx->args.dma_active_cycles =
		aggressor_active_after - aggressor_active_before;

	if (!ret) {
		idma_bench_verify_result(victim_ctx);
		idma_bench_verify_result(aggressor_ctx);
	}

	idma_bench_log_result(victim_ctx, "pair independent victim", ret);
	idma_bench_log_result(aggressor_ctx, "pair independent aggressor",
			      ret);

	return ret;
}

static int idma_bench_run_multi_concurrent(struct idma_bench_ctx *ctxs,
					   u32 nr_devs)
{
	struct idma_hpm_sample before;
	struct idma_hpm_sample after;
	struct idma_hpm_sample delta;
	u64 active_before[IDMA_MULTI_BENCH_MAX_DEVS];
	u64 active_after[IDMA_MULTI_BENCH_MAX_DEVS];
	u64 bytes[IDMA_MULTI_BENCH_MAX_DEVS] = {};
	u32 done_iters[IDMA_MULTI_BENCH_MAX_DEVS] = {};
	u32 iter;
	u32 slots = 0;
	u32 iters = 0;
	u32 i;
	int ret = 0;

	for (i = 0; i < nr_devs; i++) {
		char label[32];

		scnprintf(label, sizeof(label), "multi dev%d",
			  ctxs[i].idma_dev->id);
		idma_bench_log_start(&ctxs[i], label);

		slots = max(slots, idma_bench_transfer_slots(&ctxs[i]));
		iters = max(iters, ctxs[i].args.iterations);
	}

	idma_lock_multi(ctxs, nr_devs);
	for (i = 0; i < nr_devs; i++)
		active_before[i] = idma_active_cycles_read(ctxs[i].idma_dev);
	idma_hpm_read(ctxs[0].idma_dev, &before);

	for (iter = 0; iter < iters; iter++) {
		u32 slot;

		for (slot = 0; slot < slots; slot++) {
			struct idma_transfer xfers[IDMA_MULTI_BENCH_MAX_DEVS] = {};
			bool any_valid = false;
			int wait_ret = 0;

			for (i = 0; i < nr_devs; i++) {
				if (iter >= ctxs[i].args.iterations)
					continue;

				idma_bench_transfer_at(&ctxs[i], slot, &xfers[i]);
				if (xfers[i].valid)
					any_valid = true;
			}

			if (!any_valid)
				continue;

			for (i = 0; i < nr_devs; i++) {
				if (!xfers[i].valid)
					continue;

				ret = idma_submit_transfer(ctxs[i].idma_dev,
							   xfers[i].src,
							   xfers[i].dst,
							   xfers[i].size,
							   &xfers[i].id);
				if (ret)
					break;
			}

			for (i = 0; i < nr_devs; i++) {
				int one_ret;

				if (!xfers[i].valid || !xfers[i].id)
					continue;

				one_ret = idma_wait_transfer(ctxs[i].idma_dev,
							     xfers[i].src,
							     xfers[i].dst,
							     xfers[i].size,
							     xfers[i].id);
				if (!one_ret)
					bytes[i] += xfers[i].size;
				else if (!wait_ret)
					wait_ret = one_ret;
			}

			if (ret || wait_ret) {
				if (!ret)
					ret = wait_ret;
				goto out_measure;
			}
		}

		for (i = 0; i < nr_devs; i++) {
			if (iter < ctxs[i].args.iterations)
				done_iters[i]++;
		}
	}

out_measure:
	idma_hpm_read(ctxs[0].idma_dev, &after);
	for (i = 0; i < nr_devs; i++)
		active_after[i] = idma_active_cycles_read(ctxs[i].idma_dev);
	idma_unlock_multi(ctxs, nr_devs);

	idma_hpm_delta(&before, &after, &delta);

	for (i = 0; i < nr_devs; i++) {
		ctxs[i].args.iterations = done_iters[i];
		ctxs[i].args.bytes = bytes[i];
		ctxs[i].args.cycles = delta.cycles;
		ctxs[i].args.iotlb_miss = delta.iotlb_miss;
		ctxs[i].args.s1_ptw = delta.s1_ptw;
		ctxs[i].args.s2_ptw = delta.s2_ptw;
		ctxs[i].args.dma_active_cycles = active_after[i] -
						  active_before[i];
	}

	if (!ret) {
		for (i = 0; i < nr_devs; i++)
			idma_bench_verify_result(&ctxs[i]);
	}

	for (i = 0; i < nr_devs; i++) {
		char label[32];

		scnprintf(label, sizeof(label), "multi dev%d",
			  ctxs[i].idma_dev->id);
		idma_bench_log_result(&ctxs[i], label, ret);
	}

	return ret;
}

static void idma_bench_cleanup(struct idma_bench_ctx *ctx)
{
	if (!ctx->cleanup_safe) {
		dev_crit(ctx->dev,
			 "retaining benchmark mappings and buffers for quarantined DMA\n");
		return;
	}

	kvfree(ctx->transfer_order);

	if (ctx->mapped_src || ctx->mapped_dst) {
		mutex_lock(&idma_iova_lock);
		if (ctx->mapped_dst)
			idma_iommu_unmap_range(ctx->domain, ctx->dst_iova,
					       ctx->size, ctx->pgsize);
		if (ctx->mapped_src)
			idma_iommu_unmap_range(ctx->domain, ctx->src_iova,
					       ctx->size, ctx->pgsize);
		mutex_unlock(&idma_iova_lock);
	}

	if (ctx->dst)
		dma_free_attrs(ctx->dev, ctx->size, ctx->dst, ctx->dst_dma,
			       ctx->attrs);
	if (ctx->src)
		dma_free_attrs(ctx->dev, ctx->size, ctx->src, ctx->src_dma,
			       ctx->attrs);
}

static long idma_run_once_ioctl(struct idma_device *idma_dev,
				struct idma_once_args __user *uarg)
{
	struct device *dev = idma_dev->miscdev.parent;
	struct idma_once_args args;
	struct idma_hpm_sample before;
	struct idma_hpm_sample after;
	struct idma_hpm_sample delta;
	unsigned long attrs = DMA_ATTR_FORCE_CONTIGUOUS;
	dma_addr_t src_dma;
	dma_addr_t dst_dma;
	u64 active_before;
	u64 active_after;
	void *src = NULL;
	void *dst = NULL;
	size_t size;
	int ret = 0;

	if (copy_from_user(&args, uarg, sizeof(args)))
		return -EFAULT;

	size = args.size;
	args.bytes = 0;
	args.cycles = 0;
	args.iotlb_miss = 0;
	args.s1_ptw = 0;
	args.s2_ptw = 0;
	args.dma_active_cycles = 0;
	args.errors = 0;
	args.reserved = 0;

	if (!size || size > IDMA_BENCH_MAX_SIZE) {
		ret = -EINVAL;
		goto out_copy;
	}

	src = dma_alloc_attrs(dev, size, &src_dma, GFP_KERNEL, attrs);
	if (!src) {
		ret = -ENOMEM;
		goto out_copy;
	}

	dst = dma_alloc_attrs(dev, size, &dst_dma, GFP_KERNEL, attrs);
	if (!dst) {
		ret = -ENOMEM;
		goto free_src;
	}

	idma_fill_test_pattern(src, size);
	memset(dst, 0, size);

	dev_info(dev, "once start: size=%zu src=%pad dst=%pad\n",
		 size, &src_dma, &dst_dma);

	mutex_lock(&idma_dev->lock);
	active_before = idma_active_cycles_read(idma_dev);
	idma_hpm_read(idma_dev, &before);
	ret = idma_start_transfer(idma_dev, src_dma, dst_dma, size);
	idma_hpm_read(idma_dev, &after);
	active_after = idma_active_cycles_read(idma_dev);
	mutex_unlock(&idma_dev->lock);

	idma_hpm_delta(&before, &after, &delta);
	args.cycles = delta.cycles;
	args.iotlb_miss = delta.iotlb_miss;
	args.s1_ptw = delta.s1_ptw;
	args.s2_ptw = delta.s2_ptw;
	args.dma_active_cycles = active_after - active_before;

	if (!ret) {
		args.bytes = size;
		if (memcmp(src, dst, size))
			args.errors++;
	}

	dev_info(dev,
		 "once: size=%zu bytes=%llu cycles=%llu active=%llu iotlb_miss=%llu s1_ptw=%llu s2_ptw=%llu errors=%u ret=%d\n",
		 size,
		 (unsigned long long)args.bytes,
		 (unsigned long long)args.cycles,
		 (unsigned long long)args.dma_active_cycles,
		 (unsigned long long)args.iotlb_miss,
		 (unsigned long long)args.s1_ptw,
		 (unsigned long long)args.s2_ptw,
		 args.errors, ret);

	dma_free_attrs(dev, size, dst, dst_dma, attrs);
free_src:
	dma_free_attrs(dev, size, src, src_dma, attrs);
out_copy:
	if (copy_to_user(uarg, &args, sizeof(args)))
		return -EFAULT;

	return ret;
}

static long idma_run_pair_bench_ioctl(struct idma_device *victim_dev,
				      struct idma_pair_bench_args __user *uarg,
				      bool independent)
{
	struct idma_pair_bench_args args;
	struct idma_bench_ctx victim_ctx;
	struct idma_bench_ctx aggressor_ctx;
	struct idma_device *aggressor_dev = NULL;
	bool has_aggressor;
	bool victim_prepared = false;
	bool aggressor_prepared = false;
	int ret = 0;

	if (copy_from_user(&args, uarg, sizeof(args)))
		return -EFAULT;

	idma_bench_clear_results(&args.victim);
	idma_bench_clear_results(&args.aggressor);
	if (!args.xfer_chunk)
		args.xfer_chunk = IDMA_BENCH_DEFAULT_CHUNK;

	has_aggressor = args.aggressor_id != IDMA_PAIR_AGGRESSOR_NONE;
	if (has_aggressor) {
		aggressor_dev = idma_get_instance(args.aggressor_id);
		if (!aggressor_dev) {
			ret = -ENODEV;
			goto out_copy;
		}
		if (aggressor_dev == victim_dev) {
			ret = -EINVAL;
			goto out_put_aggressor;
		}
	}

	ret = idma_bench_prepare(&victim_ctx, victim_dev, &args.victim,
				 args.xfer_chunk);
	args.victim = victim_ctx.args;
	if (ret)
		goto out_put_aggressor;
	victim_prepared = true;

	if (!has_aggressor) {
		ret = idma_bench_run(&victim_ctx, "pair solo");
		args.victim = victim_ctx.args;
		goto out_cleanup;
	}

	ret = idma_bench_prepare(&aggressor_ctx, aggressor_dev, &args.aggressor,
				 args.xfer_chunk);
	args.aggressor = aggressor_ctx.args;
	if (ret)
		goto out_cleanup;
	aggressor_prepared = true;

	if (independent)
		ret = idma_bench_run_pair_independent(&victim_ctx,
						      &aggressor_ctx);
	else
		ret = idma_bench_run_pair_lockstep(&victim_ctx,
						   &aggressor_ctx);
	args.victim = victim_ctx.args;
	args.aggressor = aggressor_ctx.args;

out_cleanup:
	if (aggressor_prepared)
		idma_bench_cleanup(&aggressor_ctx);
	if (victim_prepared)
		idma_bench_cleanup(&victim_ctx);
out_put_aggressor:
	idma_put_instance(aggressor_dev);
out_copy:
	if (copy_to_user(uarg, &args, sizeof(args)))
		return -EFAULT;

	return ret;
}

static long idma_run_multi_bench_ioctl(struct idma_multi_bench_args __user *uarg)
{
	struct idma_multi_bench_args *args;
	struct idma_bench_ctx *ctxs;
	struct idma_device *idma_devs[IDMA_MULTI_BENCH_MAX_DEVS] = {};
	bool prepared[IDMA_MULTI_BENCH_MAX_DEVS] = {};
	u32 i;
	u32 j;
	int ret = 0;

	BUILD_BUG_ON(IDMA_MULTI_BENCH_MAX_DEVS > IDMA_MAX_DEVS);

	args = memdup_user(uarg, sizeof(*args));
	if (IS_ERR(args))
		return PTR_ERR(args);

	ctxs = kcalloc(IDMA_MULTI_BENCH_MAX_DEVS, sizeof(*ctxs), GFP_KERNEL);
	if (!ctxs) {
		ret = -ENOMEM;
		goto out_copy;
	}

	for (i = 0; i < IDMA_MULTI_BENCH_MAX_DEVS; i++)
		idma_bench_clear_results(&args->devs[i]);
	if (!args->xfer_chunk)
		args->xfer_chunk = IDMA_BENCH_DEFAULT_CHUNK;

	if (!args->nr_devs || args->nr_devs > IDMA_MULTI_BENCH_MAX_DEVS) {
		ret = -EINVAL;
		goto out_copy;
	}

	for (i = 0; i < args->nr_devs; i++) {
		if (args->dev_ids[i] >= IDMA_MAX_DEVS) {
			ret = -EINVAL;
			goto out_put_devs;
		}

		for (j = 0; j < i; j++) {
			if (args->dev_ids[j] == args->dev_ids[i]) {
				ret = -EINVAL;
				goto out_put_devs;
			}
		}

		idma_devs[i] = idma_get_instance(args->dev_ids[i]);
		if (!idma_devs[i]) {
			ret = -ENODEV;
			goto out_put_devs;
		}
	}

	for (i = 0; i < args->nr_devs; i++) {
		ret = idma_bench_prepare(&ctxs[i], idma_devs[i],
					 &args->devs[i], args->xfer_chunk);
		args->devs[i] = ctxs[i].args;
		if (ret)
			goto out_cleanup;
		prepared[i] = true;
	}

	ret = idma_bench_run_multi_concurrent(ctxs, args->nr_devs);
	for (i = 0; i < args->nr_devs; i++)
		args->devs[i] = ctxs[i].args;

out_cleanup:
	for (i = 0; i < IDMA_MULTI_BENCH_MAX_DEVS; i++) {
		if (prepared[i])
			idma_bench_cleanup(&ctxs[i]);
	}
out_put_devs:
	for (i = 0; i < IDMA_MULTI_BENCH_MAX_DEVS; i++)
		idma_put_instance(idma_devs[i]);
out_copy:
	if (copy_to_user(uarg, args, sizeof(*args)))
		ret = -EFAULT;

	kfree(ctxs);
	kfree(args);
	return ret;
}

static long idma_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct idma_device *idma_dev = fp->private_data;

	switch (cmd) {
	case IDMA_IOC_RUN_PAIR_BENCH:
		return idma_run_pair_bench_ioctl(idma_dev,
						 (struct idma_pair_bench_args __user *)arg,
						 false);
	case IDMA_IOC_RUN_ONCE:
		return idma_run_once_ioctl(idma_dev,
					   (struct idma_once_args __user *)arg);
	case IDMA_IOC_RUN_MULTI_BENCH:
		return idma_run_multi_bench_ioctl(
			(struct idma_multi_bench_args __user *)arg);
	case IDMA_IOC_RUN_PAIR_BENCH_INDEPENDENT:
		return idma_run_pair_bench_ioctl(idma_dev,
						 (struct idma_pair_bench_args __user *)arg,
						 true);
	default:
		return -ENOTTY;
	}
}

static const struct file_operations idma_fops = {
	.owner		= THIS_MODULE,
	.open		= idma_open,
	.release	= idma_release,
	.unlocked_ioctl	= idma_ioctl,
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
	mutex_init(&idma_dev->lock);

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

	idma_hpm_init(pdev, idma_dev);
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
