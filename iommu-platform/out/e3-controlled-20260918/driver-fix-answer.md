Below is the complete, self-contained revision. It does not claim to have been compiled, run, or validated on hardware. The two tests are complete but I explicitly flag which assertions I could not verify without a compiler/FPGA and treat them as unresolved acceptance items (see the end).

---

## `linux/include/uapi/linux/idma.h`

```c
/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_H
#define _UAPI_LINUX_IDMA_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define IDMA_EXPERIMENT_ABI 1U
#define IDMA_EXPERIMENT_MAX_TRANSFERS (1ULL << 32)
#define IDMA_EXPERIMENT_NO_AGGRESSOR ((__u32)~0U)
#define IDMA_EXPERIMENT_CAP_DID_HPM (1U << 0)
#define IDMA_EXPERIMENT_CAP_SW_REFILL (1U << 1)
#define IDMA_EXPERIMENT_CAP_HW_BATCH (1U << 2)
#define IDMA_EXPERIMENT_CAP_E3_PAIR (1U << 3)
#define IDMA_EXPERIMENT_CAP_E3_DIAG (1U << 4)
#define IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR (1U << 5)
#define IDMA_EXPERIMENT_CAP_E3_CONTROLLED (1U << 6)
#define IDMA_BATCH_MAX_TRANSFERS (1ULL << 20)
/* HPM capability is preliminary; acquire checks slots/filter readback too. */
/* Neither capability nor configuration establishes board calibration. */
struct idma_experiment_caps {
	__u32 abi_version;
	__u32 struct_size;
	__u32 capabilities;
	__u32 device_id;
	__u32 did;
	__u32 max_pages;
	__u32 page_bytes;
	__u32 max_run_timeout_ms;
	__u32 reserved[8];
};

struct idma_experiment_stats {
	__u64 submitted;
	__u64 completed;
	__u64 completed_bytes;
	__u64 active_cycles;
	__u64 iotlb_miss;
	__u64 s1_ptw;
	__u64 s2_ptw;
	/* Observed completion to observed resubmission, incl. MMIO; not HW idle. */
	__u64 refill_gap_ns_sum;
	__u64 refill_gap_ns_max;
	__u64 refill_gap_samples;
	/* Submitted but not yet observed complete in software; not HW overlap. */
	__u32 in_flight_at_start;
	__u32 in_flight_at_end;
	__u32 data_errors;
	__u32 quarantined;
};

struct idma_experiment {
	/* Input: all reserved words and flags must be zero in ABI 1. */
	__u32 abi_version;
	__u32 struct_size;
	__u32 flags;
	__u32 aggressor_id;
	__u32 victim_pages;
	__u32 aggressor_pages;
	__u32 chunk_bytes;
	__u32 run_timeout_ms;
	__u64 victim_transfers;
	__u64 warmup_transfers;
	__u32 reserved[8];
	/* Output: status is a negative errno or zero, even when ioctl fails. */
	__s32 status;
	__u32 victim_did;
	__u32 aggressor_did;
	__u32 hpm_configured;
	__u32 measurement_started;
	__u32 measurement_complete;
	__u64 victim_elapsed_ns;
	/* Snapshot intervals bracket V's window; HPM is not an atomic snapshot. */
	__u64 snapshot_before_ns;
	__u64 snapshot_after_ns;
	__u64 drain_ns;
	struct idma_experiment_stats victim;
	struct idma_experiment_stats aggressor;
};

/* Old commands 0x01..0x05 are retired and return ENOTTY. */
#define IDMA_IOC_EXPERIMENT_CAPS _IOR('i', 0x06, struct idma_experiment_caps)
#define IDMA_IOC_RUN_EXPERIMENT _IOWR('i', 0x07, struct idma_experiment)

/* Separate operation/JSON semantics, same validated input layout; reserved=0.
 * Batch stats count hardware tasks; refill gaps are not sampled. A counts include
 * its startup and final drain, rather than just the V hardware window.
 */
#define IDMA_IOC_RUN_BATCH _IOWR('i', 0x08, struct idma_experiment)

#define IDMA_E3_WORDS 256U
/* One allocation/mapping lifetime, two separately warmed/drained/verified rounds.
 * first_delay is 0 or 128; the other value is used for the second round.
 * Raw words are versioned observer-v4 bank indices 192..447, read after drain.
 */
struct idma_e3_pair {
	__u32 abi_version, struct_size;
	__u32 first_delay, reserved;
	struct idma_experiment request;
	__s32 status;
	__u32 rounds_attempted, rounds_completed, reserved_output;
	__u64 sequence[2];
	/* V src/dst IOVA, V src/dst PA, A src/dst IOVA, A src/dst PA. */
	__u64 mapping[8];
	struct idma_experiment result[2];
	__u64 observation[2][IDMA_E3_WORDS];
};
#define IDMA_IOC_RUN_E3_PAIR _IOWR('i', 0x09, struct idma_e3_pair)

/* Diagnostic-only D0/D0. Old pair ABI and complementary delays stay unchanged.
 * All timestamps are software CLOCK_MONOTONIC ns, not hardware acceptance.
 * Warm HPM includes the final A drain; counts are software observed tasks.
 * Zero timestamps mean that the corresponding stage was not reached.
 */
struct idma_e3_diag_lane {
	__u64 warm_submitted, warm_completed, warm_drained;
	__u64 warm_last_offset, warm_next_offset;
	__u64 launch_begin_ns, launch_end_ns;
	__u32 warm_in_flight, reserved;
	struct idma_experiment_stats warm_hpm;
};
struct idma_e3_diag_round {
	__u64 warm_begin_ns, warm_end_ns, warm_drained_ns;
	__u64 arm_begin_ns, arm_end_ns;
	/* V then A. HPM snapshot complete and both warm streams drained. */
	__u32 warm_complete, reserved;
	struct idma_e3_diag_lane lane[2];
};
struct idma_e3_diag {
	/* Input uses the unchanged pair size; first_delay must be zero. */
	struct idma_e3_pair pair;
	/* Output only, cleared by the kernel before validation. */
	struct idma_e3_diag_round diagnostic[2];
};
#define IDMA_IOC_RUN_E3_DIAG _IOWR('i', 0x0a, struct idma_e3_diag)

/* Same diagnostic layout; complementary D0/D128 and first_delay 0 or 128.
 * Requires observer v5 / E3 bank v2 with full waiting-queue trigger coverage.
 * Old D0/D0 operation above is unchanged. */
#define IDMA_IOC_RUN_E3_QUEUE_PAIR _IOWR('i', 0x0b, struct idma_e3_diag)

#define IDMA_E3_CONTROLLED_WORDS 32U

/*
 * Controlled finite E3 round.  Inputs are carried in the embedded pair and
 * prep_mode/pair_mode; all other fields are outputs cleared by the kernel.
 *
 * prep_mode: 0 cold (no A prime), 1 hot (one A prime).
 * pair_mode: 0 complementary D0/D128 (first_delay 0 or 128), 1 D0/D0.
 *
 * flush_status is -EINPROGRESS until the mandatory global S1 flush returns.
 * stage follows 1..8 (flush, A prime, V prime, V probe, prepare, running,
 * drain, complete) and is only 8 when the whole round succeeded.
 * prime[0]/prime[1]/probe carry independent per-phase statistics.
 * launcher[] is the raw 32-word launcher record copied on every reachable
 * path.  v_hw_cycles is observer bank index 0, not the HPM/MMIO span.
 */
struct idma_e3_controlled_round {
	__s32 flush_status;
	__u32 prep_complete;
	__u32 stage;
	__u32 reserved;
	struct idma_experiment_stats prime[2];
	struct idma_experiment_stats probe;
	__u64 launcher[IDMA_E3_CONTROLLED_WORDS];
	__u64 observer_magic;
	__u64 v_hw_cycles;
};

struct idma_e3_controlled {
	struct idma_e3_pair pair;
	__u32 prep_mode;
	__u32 pair_mode;
	__u32 reserved[6];
	struct idma_e3_controlled_round controlled[2];
};

#define IDMA_IOC_RUN_E3_CONTROLLED _IOWR('i', 0x0c, struct idma_e3_controlled)

#endif
```

---

## `linux/drivers/misc/idma.c`

```c
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
#define IDMA_E3_MAGIC 0x49444d4145330001ULL
#define IDMA_E3_MAGIC_V2 0x49444d4145330002ULL
#define IDMA_OBS_VALUES 34
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

/* Reserved launcher lane IDs: V is lane 0 (ID 1), A is lane 1 (ID 2). */
#define IDMA_CONTROLLED_V_ID 1U
#define IDMA_CONTROLLED_A_ID 2U
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
	size_t size, offset, next_offset, touched_end;
	u32 chunk;
	u64 transfer_id, deadline_ns, last_complete_ns;
	bool mapped_src, mapped_dst, in_flight, retain;
	bool batch_configured, batch_started;
	struct idma_experiment_stats *stats;
	struct idma_e3_diag_lane *diagnostic;
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

static int idma_submit(struct idma_stream *s, bool measured)
{
	u64 now;

	if (s->diagnostic && s->batch_started)
		s->diagnostic->launch_begin_ns = ktime_get_ns();
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
	if (s->diagnostic && !s->batch_started)
		s->diagnostic->warm_completed++;
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
	if (s->diagnostic && !s->batch_started)
		s->diagnostic->warm_drained++;
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
			     u64 deadline, u64 *end_ns)
{
	u32 spins = 0;
	int ret;

	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}
	if (a) {
		a->batch_started = true;
		ret = idma_submit(a, false);
		if (ret)
			goto out;
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
	bool armed, batch, path, e3, delay128, queue_pair, controlled;
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
	o->path = o->e3 || (o->batch && READ_ONCE(idma_path_observe));
	if (o->controlled && magic != IDMA_OBS_MAGIC_V6)
		return -EOPNOTSUPP;
	if (o->queue_pair && magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V6)
		return -EOPNOTSUPP;
	o->e3_magic = (magic == IDMA_OBS_MAGIC_V5 || magic == IDMA_OBS_MAGIC_V6) ?
		      IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
	if (o->e3 && magic != IDMA_OBS_MAGIC_V4 &&
	    magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V6)
		return -EOPNOTSUPP;
	if (o->control->did != 10 ||
	    (magic != IDMA_OBS_MAGIC_V6 && magic != IDMA_OBS_MAGIC_V5 &&
	     magic != IDMA_OBS_MAGIC_V4 && magic != IDMA_OBS_MAGIC_V3 &&
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
	s->offset = s->next_offset = s->touched_end = 0;
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
	if (e->victim_pages != 1 || (a && e->aggressor_pages != 1))
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

	if (readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_OBS_MAGIC_V6)
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

	memset(stats, 0, sizeof(*stats));
	s->stats = stats;
	s->offset = s->next_offset = s->touched_end = 0;
	s->in_flight = false;
	s->transfer_id = s->deadline_ns = s->last_complete_ns = 0;

	riscv_iommu_hpm_read(hpm, &before);
	active_before = readq(s->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);

	local_flush_icache_all();
	ret = idma_submit(s, false);
	if (!ret)
		ret = idma_drain(s);

	active_after = readq(s->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
	riscv_iommu_hpm_read(hpm, &after);

	stats->active_cycles = active_after - active_before;
	stats->iotlb_miss = after.iotlb_miss[lane] - before.iotlb_miss[lane];
	stats->s1_ptw = after.s1_ptw[lane] - before.s1_ptw[lane];
	stats->s2_ptw = after.s2_ptw[lane] - before.s2_ptw[lane];
	stats->submitted = ret ? 0 : 1;
	stats->completed = ret ? 0 : 1;
	stats->completed_bytes = ret ? 0 : s->chunk;

	if (ret)
		return ret;
	/* S2 walk is never expected for a single-level mapping. */
	if (stats->s2_ptw)
		return -EIO;
	/* A no-op probe must be a real completion with a cold IOTLB. */
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
static int idma_controlled_account(struct idma_stream *a, u8 reserved_mask, u64 *words)
{
	u8 issued = (u8)(words[15] & 0x3);
	u8 done = (u8)(words[16] & 0x3);
	u8 cancel = (u8)(words[17] & 0x3);
	u8 lane;

	if (a ? 0 : (reserved_mask & 0x2))
		return -EIO;
	for (lane = 0; lane < 2; lane++) {
		u8 bit = (u8)BIT(lane);

		if (!(reserved_mask & bit))
			continue;
		if (cancel & bit) {
			if (done & bit)
				return -EIO;
			if (issued & bit)
				return -EIO;
		} else if (!(done & bit)) {
			return -EIO;
		}
	}
	if ((done | cancel) & (u8)~reserved_mask)
		return -EIO;
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
					    struct idma_stream *a,
					    struct idma_experiment *e, u64 *words)
{
	u8 expected = a ? 3 : 1;

	if (words[1])
		return -EIO;
	if (!(words[2] & IDMA_LAUNCHER_STATUS_FROZEN) ||
	    (words[2] & (IDMA_LAUNCHER_STATUS_RUNNING | IDMA_LAUNCHER_STATUS_ABORTED)))
		return -EIO;
	if ((words[15] & 0x3) != expected || (words[16] & 0x3) != expected ||
	    (words[17] & 0x3))
		return -EIO;
	if (words[24] != 1 || words[26] != 1)
		return -EIO;
	if (a && (words[25] != 1 || words[27] != 1))
		return -EIO;
	/* Reserved IDs and issue==done identities. */
	if (words[11] != IDMA_CONTROLLED_V_ID || words[13] != IDMA_CONTROLLED_V_ID)
		return -EIO;
	if (a) {
		if (words[12] != IDMA_CONTROLLED_A_ID || words[14] != IDMA_CONTROLLED_A_ID)
			return -EIO;
		if (words[8] > words[5] || words[5] >= words[7] || words[10] < words[8])
			return -EIO;
	}
	if (words[4] > words[5])
		return -EIO;
	if (words[6] > words[7] || words[7] > words[9])
		return -EIO;
	/* A-selection descriptor identity, and zero A fields for Solo. */
	if (a) {
		if (words[20] != IDMA_CONTROLLED_A_SELECT_ID ||
		    words[21] != IDMA_CONTROLLED_A_SELECT_DID ||
		    words[22] != a->src_iova || words[23] != 0)
			return -EIO;
	} else if (words[20] || words[21] || words[22] || words[23]) {
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
			goto quarantine;
		}
	}

	ret2 = idma_launcher_read_words(launcher, words);
	memcpy(round->launcher, words, sizeof(round->launcher));
	if (ret2) {
		if (!ret)
			ret = ret2;
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

	if (idma_controlled_account(a, reserved_mask, words)) {
		if (!ret)
			ret = -EIO;
		goto quarantine;
	}

	idma_controlled_apply(v, a, words);

	if (!ret) {
		ret2 = idma_controlled_validate_success(v, a, e, words);
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
	u64 status, arm_stamp, start, end, v_done = 0;
	u8 reserved = 0;
	int ret, poll_ret;

	v->stats = &e->victim;
	if (a)
		a->stats = &e->aggressor;
	memset(&e->victim, 0, sizeof(e->victim));
	if (a)
		memset(&e->aggressor, 0, sizeof(e->aggressor));
	v->offset = v->next_offset = v->touched_end = 0;
	v->in_flight = false;
	v->transfer_id = v->deadline_ns = v->last_complete_ns = 0;
	if (a) {
		a->offset = a->next_offset = a->touched_end = 0;
		a->in_flight = false;
		a->transfer_id = a->deadline_ns = a->last_complete_ns = 0;
	}

	/* Ownership starts at the CLEAR/PREP writes. */
	*owned = true;
	writeq(IDMA_LAUNCHER_CLEAR, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (status & ~IDMA_LAUNCHER_STATUS_FROZEN)
		return -EIO;
	writeq(IDMA_LAUNCHER_PREP | (a ? IDMA_LAUNCHER_PREP_HAS_A : 0),
	       launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (!(status & IDMA_LAUNCHER_STATUS_PREP) ||
	    (status & IDMA_LAUNCHER_ERROR_MASK))
		return -EIO;

	ret = idma_submit(v, false);
	if (ret)
		return ret;
	reserved |= 1;
	*reserved_mask = reserved;
	if (a) {
		ret = idma_submit(a, false);
		if (ret)
			return ret;
		reserved |= 2;
		*reserved_mask = reserved;
	}

	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if ((status & IDMA_LAUNCHER_ERROR_MASK) ||
	    idma_launcher_held(status) != (a ? 3U : 1U))
		return -EIO;

	ret = idma_observation_arm(o, v->idma, a ? a->idma : NULL, 1);
	if (ret)
		return ret;

	arm_stamp = ktime_get_ns();
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e->measurement_started = 1;
	e->snapshot_before_ns = start - arm_stamp;
	round->stage = IDMA_CONTROLLED_STAGE_RUNNING;

	writeq(IDMA_LAUNCHER_GO, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (status & IDMA_LAUNCHER_ERROR_MASK)
		return -EIO;

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
		if (idma_launcher_terminal(status))
			break;
		cpu_relax();
	}

	end = ktime_get_ns();
	if (v_done)
		e->victim_elapsed_ns = v_done - start;
	idma_snapshot(hpm, v, a, &after, active_after);
	e->snapshot_after_ns = ktime_get_ns() - end;
	idma_snapshot_delta(&e->victim, 0, &before, &after,
			    active_before[0], active_after[0]);
	if (a)
		idma_snapshot_delta(&e->aggressor, 1, &before, &after,
				    active_before[1], active_after[1]);
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
			a->offset = a->next_offset = a->touched_end = 0;
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
	{
		int first = ret;

		if (round->stage == IDMA_CONTROLLED_STAGE_RUNNING)
			round->stage = IDMA_CONTROLLED_STAGE_DRAIN;
		sub = idma_controlled_cleanup(launcher, v, a, e, round,
					      reserved_mask, owned, first);
		if (sub && !first)
			first = sub;
		ret = first;
	}

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
	if (!ret)
		round->stage = IDMA_CONTROLLED_STAGE_COMPLETE;
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

static long idma_run_session(struct idma_device *victim,
			     struct idma_experiment *request, bool batch, struct idma_e3_pair *pair,
			     struct idma_e3_diag_round *diagnostic, bool queue_pair)
{
	struct idma_experiment e = *request;
	struct idma_stream streams[2] = {};
	struct idma_observation observation = { .batch = batch, .e3 = !!pair, .queue_pair = queue_pair,
		.delay128 = pair && pair->first_delay == 128 };
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	struct idma_e3_diag_round *diag = NULL;
	u64 active_before[2], active_after[2];
	u64 start, end = 0, deadline, stamp;
	int ret, drain_ret, verify_ret, observation_ret;
	unsigned int round = 0;

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
	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
	if (ret)
		goto out_cleanup;
	if (a) {
		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
					  e.chunk_bytes, &e.aggressor);
		if (ret)
			goto out_cleanup;
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
		if (ret)
			goto out_drain;
	}
	e.victim.in_flight_at_start = v->in_flight;
	e.aggressor.in_flight_at_start = a && a->in_flight;
	if (diag)
		diag->arm_begin_ns = ktime_get_ns();
	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
	if (ret)
		goto out_drain;
	stamp = ktime_get_ns();
	if (diag)
		diag->arm_end_ns = stamp;
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
	if (batch) {
		idma_batch_disable(v);
		idma_batch_disable(a);
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

static long idma_run_experiment_mode(struct idma_device *victim,
				     struct idma_experiment __user *uarg, bool batch)
{
	struct idma_experiment e;
	long ret;

	if (copy_from_user(&e, uarg, sizeof(e)))
		return -EFAULT;
	ret = idma_run_session(victim, &e, batch, NULL, NULL, false);
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

		ret = idma_run_session(victim, &e, true, p, diagnostic ? d->diagnostic : NULL, queue_pair);
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

static long idma_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct idma_device *idma = fp->private_data;
	struct idma_experiment_caps caps = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_SW_REFILL | IDMA_EXPERIMENT_CAP_HW_BATCH |
				IDMA_EXPERIMENT_CAP_E3_PAIR | IDMA_EXPERIMENT_CAP_E3_DIAG |
				IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR |
				IDMA_EXPERIMENT_CAP_E3_CONTROLLED,
		.device_id = idma->id,
		.did = idma->did,
		.max_pages = IDMA_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	switch (cmd) {
	case IDMA_IOC_RUN_E3_CONTROLLED:
		return idma_run_e3_controlled(idma, (void __user *)arg);
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
```

---

## `tests/idma_controlled_harness.c`

```c
/* Host model only. Production driver functions are inserted by
 * test_idma_controlled.py.  This models software control flow; it is not FPGA,
 * IOTLB, cache or HPM acceptance evidence.  Complex production observers
 * (idma_observation_finish / idma_e3_finish) and idma_stream_prepare are
 * deliberately stubbed here so the controlled session control flow can be
 * exercised with real production helpers. */
#include <assert.h>
#include <errno.h>
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
#define BIT(n) (1U << (n))
#define __iomem
#define __user
#define WRITE_ONCE(dst, src) ((dst) = (src))
#define READ_ONCE(src) (*(volatile __typeof__(src) *)&(src))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define dev_crit(dev, ...) ((void)(dev))
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define PAGE_SIZE 4096U
#define offset_in_page(ptr) ((uintptr_t)(ptr) & (PAGE_SIZE - 1))
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL
#define GFP_KERNEL 0
#define kzalloc(n, flags) calloc(1, n)
#define kfree(ptr) free(ptr)

/* PRODUCTION_TYPES */

struct riscv_iommu_hpm_session { int unused; };
struct riscv_iommu_hpm_sample { u64 cycles, iotlb_miss[2], s1_ptw[2], s2_ptw[2]; };

struct fake_dma {
	u64 regs[64];
	u64 latency, due, next_id, done, submits, miss;
	bool pending, populated, zero_id;
};

static struct fake_dma hw[2];
static struct device devices[2] = {{0}, {1}};
static struct idma_device idmas[2];
static struct mutex idma_experiment_lock;
static struct riscv_iommu_hpm_session session;
static u64 now = 1000;
static unsigned allocations, frees, unmaps, hpm_acquires, hpm_releases;
static unsigned obs_arms, obs_reports, obs_mmio;
static u64 idma_observation_sequence;
static bool idma_observe, idma_path_observe;

/* Launcher model. */
static u64 l_status;
static bool l_prep, l_go, l_has_a, l_aborted;
static u8 l_held, l_reserved, l_issued, l_done, l_cancel;
static u64 l_go_a_ts, l_asel_ts, l_go_v_ts, l_vissue_ts, l_aissue_ts;
static u64 l_vdone_ts, l_adone_ts;
static u64 l_vissue_id, l_aissue_id, l_vdone_id, l_adone_id;
static u64 l_vissue_count, l_aissue_count, l_vdone_count, l_adone_count;
static u64 l_a_src_iova;
static unsigned l_gos, l_aborts;

/* Per-case knobs. */
static bool controlled_mode;
static bool ctl_no_terminal, ctl_stall_a, ctl_stall_v;
static bool ctl_bad_count, ctl_bad_id, ctl_zero_id_v, ctl_zero_id_a;
static bool ctl_partial_a;   /* A reservation not held: partial prepare */
static bool ctl_stop_go;     /* never issue V on GO (V stays held) */

static void dev_info(struct device *dev, const char *fmt, ...)
{
	(void)dev; (void)fmt;
	obs_reports++;
}

static struct page *idma_buffer_page(void *ptr)
{
	return (struct page *)((uintptr_t)ptr & ~(uintptr_t)(PAGE_SIZE - 1));
}
static phys_addr_t page_to_phys(struct page *page) { return (uintptr_t)page; }

static void local_flush_icache_all(void)
{
	assert(!hw[0].pending && !hw[1].pending);
}

static phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, unsigned long iova)
{
	(void)domain;
	return iova + PAGE_SIZE;
}

static void dev_err(struct device *dev, const char *format, int id, u32 did,
		    size_t offset, size_t touched_end, size_t size, u32 chunk,
		    unsigned src, unsigned src_after, unsigned expected_src,
		    unsigned dst, unsigned dst_after, unsigned expected_dst,
		    unsigned long src_iova, unsigned long dst_iova,
		    const phys_addr_t *src_pa, const phys_addr_t *dst_pa,
		    const phys_addr_t *src_pte_pa, const phys_addr_t *dst_pte_pa)
{
	(void)dev; (void)format; (void)id; (void)did; (void)offset; (void)touched_end;
	(void)size; (void)chunk; (void)src; (void)src_after; (void)expected_src;
	(void)dst; (void)dst_after; (void)expected_dst; (void)src_iova; (void)dst_iova;
	(void)src_pa; (void)dst_pa; (void)src_pte_pa; (void)dst_pte_pa;
}

static void launcher_issue_v(void)
{
	l_vissue_id = IDMA_CONTROLLED_V_ID;
	l_vissue_count = 1;
	l_issued |= 1;
	l_go_v_ts = now;
	l_vissue_ts = now;
	hw[0].pending = true;
	hw[0].due = (ctl_stall_v || hw[0].latency == UINT64_MAX) ?
		     UINT64_MAX : now + hw[0].latency;
}

static void launcher_on_done(int i)
{
	if (!controlled_mode || !l_go)
		return;
	if (i == 1) {
		l_done |= 2;
		l_adone_ts = now;
		l_adone_id = IDMA_CONTROLLED_A_ID;
		l_adone_count = 1;
		if (l_has_a && !ctl_stop_go)
			launcher_issue_v();
	} else {
		l_done |= 1;
		l_vdone_ts = now;
		l_vdone_id = IDMA_CONTROLLED_V_ID;
		l_vdone_count = 1;
	}
	if ((l_done & (l_has_a ? 3 : 1)) == (l_has_a ? 3 : 1) && !ctl_no_terminal) {
		l_status = IDMA_LAUNCHER_STATUS_FROZEN;
		l_held = 0;
	}
}

static void advance(u64 delta)
{
	int i;

	now += delta;
	for (i = 0; i < 2; i++) {
		struct fake_dma *h = &hw[i];

		if (h->pending && h->due != UINT64_MAX && now >= h->due) {
			u8 *dst = (void *)(uintptr_t)h->regs[IDMA_DST_ADDR_REG_OFFSET / 8];
			u8 *src = (void *)(uintptr_t)h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];

			memcpy(dst, src, h->regs[IDMA_NUM_BYTES_REG_OFFSET / 8]);
			h->done = h->next_id;
			h->pending = false;
			launcher_on_done(i);
		}
	}
}

static u64 ktime_get_ns(void) { advance(1000); return now; }
static bool signal_pending(void *unused) { (void)unused; return false; }
static void cpu_relax(void) { advance(1000); }
static void cond_resched(void) { advance(1000); }

static struct fake_dma *mmio(void *address, size_t *offset)
{
	int i;

	for (i = 0; i < 2; i++) {
		uintptr_t p = (uintptr_t)address, base = (uintptr_t)hw[i].regs;

		if (p >= base && p < base + sizeof(hw[i].regs)) {
			*offset = p - base;
			return &hw[i];
		}
	}
	assert(!"invalid MMIO address");
	return NULL;
}

static void launcher_reset_model(void)
{
	l_status = 0; l_prep = false; l_go = false; l_has_a = false;
	l_held = l_reserved = l_issued = l_done = l_cancel = 0;
	l_aborted = false;
	l_go_a_ts = l_asel_ts = l_go_v_ts = l_vissue_ts = l_aissue_ts = 0;
	l_vdone_ts = l_adone_ts = 0;
	l_vissue_id = l_aissue_id = l_vdone_id = l_adone_id = 0;
	l_vissue_count = l_aissue_count = l_vdone_count = l_adone_count = 0;
	l_a_src_iova = 0;
}

static void launcher_go(void)
{
	l_gos++;
	l_go = true;
	l_status |= IDMA_LAUNCHER_STATUS_RUNNING;
	l_go_a_ts = now;
	if (l_has_a) {
		l_aissue_id = IDMA_CONTROLLED_A_ID;
		l_aissue_count = 1;
		l_issued |= 2;
		l_aissue_ts = now;
		l_asel_ts = now;
		hw[1].pending = true;
		hw[1].due = (ctl_stall_a || hw[1].latency == UINT64_MAX) ?
			     UINT64_MAX : now + hw[1].latency;
		if (ctl_stop_go)
			return;
	} else {
		launcher_issue_v();
	}
}

static void launcher_abort(void)
{
	l_aborts++;
	l_aborted = true;
	if (l_held & 1) { l_cancel |= 1; l_held &= (u8)~1; hw[0].pending = false; hw[0].due = UINT64_MAX; }
	if (l_held & 2) { l_cancel |= 2; l_held &= (u8)~2; hw[1].pending = false; hw[1].due = UINT64_MAX; }
	if (l_issued & 1) {
		hw[0].pending = false;
		if (!(l_done & 1) && !ctl_stall_v && hw[0].latency != UINT64_MAX) {
			l_done |= 1; l_vdone_ts = now;
			l_vdone_id = IDMA_CONTROLLED_V_ID; l_vdone_count = 1;
		}
	}
	if (l_issued & 2) {
		hw[1].pending = false;
		if (!(l_done & 2) && !ctl_stall_a && hw[1].latency != UINT64_MAX) {
			l_done |= 2; l_adone_ts = now;
			l_adone_id = IDMA_CONTROLLED_A_ID; l_adone_count = 1;
		}
	}
	if (ctl_no_terminal)
		return;
	if (!l_held && !(l_issued & ~l_done))
		l_status = IDMA_LAUNCHER_STATUS_FROZEN | IDMA_LAUNCHER_STATUS_ABORTED;
}

static u64 launcher_word(unsigned int idx)
{
	u8 expected = l_has_a ? 3 : 1;

	switch (idx) {
	case 0: return IDMA_LAUNCHER_MAGIC;
	case 1: return 0;
	case 2: return l_status;
	case 3: return now;
	case 4: return l_has_a ? l_go_a_ts : 0;
	case 5: return l_has_a ? l_asel_ts : 0;
	case 6: return l_go_v_ts;
	case 7: return l_vissue_ts;
	case 8: return l_has_a ? l_aissue_ts : 0;
	case 9: return l_vdone_ts;
	case 10: return l_has_a ? l_adone_ts : 0;
	case 11: return l_vissue_id;
	case 12: return l_has_a ? l_aissue_id : 0;
	case 13: return ctl_bad_id ? 7 : l_vdone_id;
	case 14: return l_has_a ? l_adone_id : 0;
	case 15: return l_issued;
	case 16: return l_done;
	case 17: return l_cancel;
	case 18: return l_reserved;
	case 19: return expected;
	case 20: return l_has_a ? IDMA_CONTROLLED_A_SELECT_ID : 0;
	case 21: return l_has_a ? IDMA_CONTROLLED_A_SELECT_DID : 0;
	case 22: return l_has_a ? l_a_src_iova : 0;
	case 23: return 0;
	case 24: return l_vissue_count + (ctl_bad_count ? 1 : 0);
	case 25: return l_aissue_count;
	case 26: return l_vdone_count + (ctl_bad_count ? 1 : 0);
	case 27: return l_adone_count;
	case 28: return l_has_a ? 1 : 0;
	case 29: return l_held;
	case 30: return 0;
	case 31: return 0;
	default: return 0;
	}
}

static void writeq(u64 value, void *address)
{
	size_t offset;
	struct fake_dma *h = mmio(address, &offset);

	if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
		if (offset == IDMA_LAUNCHER_CONTROL_OFFSET) {
			if (value & IDMA_LAUNCHER_CLEAR)
				launcher_reset_model();
			else if (value & IDMA_LAUNCHER_PREP) {
				l_prep = true;
				l_has_a = !!(value & IDMA_LAUNCHER_PREP_HAS_A);
				l_status |= IDMA_LAUNCHER_STATUS_PREP;
			} else if (value & IDMA_LAUNCHER_GO) {
				launcher_go();
			} else if (value & IDMA_LAUNCHER_ABORT) {
				launcher_abort();
			}
		}
		h->regs[offset / 8] = value;
		return;
	}
	if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
		h->regs[offset / 8] = value;
		return;
	}
	if (offset >= IDMA_OBS_MAGIC_OFFSET) {
		obs_mmio++;
		if (offset == IDMA_OBS_CONTROL_OFFSET) {
			if (value == 4)
				value = 0;
			else {
				assert(hpm_acquires == hpm_releases + 1);
				assert(!hw[0].pending && !hw[1].pending);
				assert(value & BIT_ULL(29));   /* finite flag */
				assert(h[0].regs[IDMA_OBS_TARGET_OFFSET / 8] == 1);
				obs_arms++;
				value = 1;
			}
		}
		h->regs[offset / 8] = value;
		return;
	}
	assert(!h->pending);
	h->regs[offset / 8] = value;
}

static u64 readq(void *address)
{
	size_t offset;
	struct fake_dma *h = mmio(address, &offset);

	advance(1000);
	if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
		if (offset == IDMA_LAUNCHER_MAGIC_OFFSET)
			return IDMA_LAUNCHER_MAGIC;
		if (offset == IDMA_LAUNCHER_CONTROL_OFFSET)
			return l_status;
		if (offset == IDMA_LAUNCHER_INDEX_OFFSET)
			return h->regs[offset / 8];
		if (offset == IDMA_LAUNCHER_DATA_OFFSET)
			return launcher_word((unsigned int)h->regs[IDMA_LAUNCHER_INDEX_OFFSET / 8]);
		return h->regs[offset / 8];
	}
	if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
		if (offset == IDMA_BATCH_MAGIC_OFFSET)
			return IDMA_BATCH_MAGIC;
		return h->regs[offset / 8];
	}
	if (offset >= IDMA_OBS_MAGIC_OFFSET) {
		obs_mmio++;
		if (offset == IDMA_OBS_CONTROL_OFFSET)
			return h->regs[offset / 8];
		if (offset == IDMA_OBS_DATA_OFFSET) {
			unsigned int index = (unsigned int)h->regs[IDMA_OBS_INDEX_OFFSET / 8];

			return index == 0 ? 0xfeedU : 0;
		}
		return h->regs[offset / 8];
	}
	if (offset == IDMA_NEXT_ID_REG_OFFSET) {
		assert(!h->pending);
		h->submits++;
		if (!h->populated) { h->miss++; h->populated = true; }
		if (h->zero_id) {
			hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] |= 0; /* noop */
			return 0;
		}
		if (controlled_mode && l_prep && !l_go) {
			/* Reservation only; nothing is issued. */
			unsigned int lane = (h == &hw[1]) ? 1 : 0;

			if (lane == 1 && ctl_partial_a)
				return 2;
			if (lane == 0 && ctl_zero_id_v)
				return 0;
			if (lane == 1 && ctl_zero_id_a)
				return 0;
			l_reserved |= (u8)BIT(lane);
			l_held |= (u8)BIT(lane);
			if (lane == 1)
				l_a_src_iova = h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
			h->next_id++;
			return lane == 0 ? IDMA_CONTROLLED_V_ID : IDMA_CONTROLLED_A_ID;
		}
		h->pending = true;
		h->next_id++;
		h->due = (h->zero_id || h->latency == UINT64_MAX) ? UINT64_MAX : now + h->latency;
		return h->next_id;
	}
	if (offset == IDMA_DONE_REG_OFFSET)
		return h->done;
	if (offset == IDMA_ACTIVE_CYCLES_REG_OFFSET)
		return now;
	return h->regs[offset / 8];
}

static int copy_from_user(void *dst, const void *src, size_t n) { memcpy(dst, src, n); return 0; }
static int copy_to_user(void *dst, const void *src, size_t n) { memcpy(dst, src, n); return 0; }
static int mutex_lock_interruptible(struct mutex *m) { (void)m; return 0; }
static void mutex_unlock(struct mutex *m) { (void)m; }
static const void *memchr_inv(const void *ptr, int c, size_t n)
{
	const unsigned char *p = ptr;

	while (n--) {
		if (*p != (unsigned char)c)
			return p;
		p++;
	}
	return NULL;
}
static bool idma_any_quarantined(void)
{
	return idmas[0].quarantined || idmas[1].quarantined;
}
static struct idma_device *idma_get_instance(u32 id)
{
	return id < 2 ? &idmas[id] : NULL;
}
static void idma_put_instance(struct idma_device *d) { (void)d; }
static void idma_iommu_unmap_range(struct iommu_domain *d, unsigned long iova,
				   size_t size, size_t page)
{
	(void)d; (void)iova; (void)size; (void)page;
	unmaps++;
}
static void dma_free_attrs(struct device *d, size_t n, void *ptr, dma_addr_t dma,
			   unsigned attrs)
{
	(void)n; (void)dma; (void)attrs;
	assert(!hw[d->index].pending);
	frees++;
	free(ptr);
}

/* Stubs for the complex production pieces, replaced by simple models. */
static u64 idma_pattern(struct idma_stream *s, size_t word);
static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	size_t i;

	memset(s, 0, sizeof(*s));
	s->idma = idma;
	s->dev = idma->miscdev.parent;
	s->stats = stats;
	s->size = (size_t)pages * PAGE_SIZE;
	s->chunk = chunk;
	s->src = malloc(s->size);
	s->dst = malloc(s->size);
	assert(s->src && s->dst);
	allocations += 2;
	s->src_iova = (uintptr_t)s->src;
	s->dst_iova = (uintptr_t)s->dst;
	s->mapped_src = s->mapped_dst = true;
	for (i = 0; i < s->size / sizeof(u64); i++) {
		((u64 *)s->src)[i] = idma_pattern(s, i);
		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
	}
	return 0;
}

static int riscv_iommu_hpm_acquire(struct device *v, struct device *a,
				   struct riscv_iommu_hpm_session **s)
{
	(void)v; (void)a;
	hpm_acquires++;
	*s = &session;
	return 0;
}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *s,
				 struct riscv_iommu_hpm_sample *sample)
{
	int i;

	(void)s;
	memset(sample, 0, sizeof(*sample));
	for (i = 0; i < 2; i++) {
		sample->iotlb_miss[i] = hw[i].miss;
		sample->s1_ptw[i] = hw[i].miss * 3;
		sample->s2_ptw[i] = 0;
	}
	advance(1000);
}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *s)
{
	assert(s == &session);
	hpm_releases++;
}
static int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *s)
{
	assert(s == &session);
	assert(hpm_acquires == hpm_releases + 1);
	assert(!hw[0].pending && !hw[1].pending);
	hw[0].populated = false;
	hw[1].populated = false;
	return 0;
}

static int idma_observation_finish(struct idma_observation *o,
				   const struct idma_experiment *e, int run_error)
{
	(void)e;
	if (!o->control || !o->armed)
		return 0;
	return run_error ? 0 : 0;
}

static int idma_e3_finish(struct idma_observation *o,
			  const struct idma_experiment *e, u64 *words)
{
	(void)e;
	if (!o->armed)
		return -EIO;
	memset(words, 0, IDMA_E3_WORDS * sizeof(u64));
	return 0;
}

/* PRODUCTION_FUNCTIONS */

static struct idma_experiment controlled_input(void)
{
	struct idma_experiment e = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(e),
		.aggressor_id = 1,
		.victim_pages = 1,
		.aggressor_pages = 1,
		.chunk_bytes = 64,
		.run_timeout_ms = 100,
		.victim_transfers = 1,
	};
	int i;

	for (i = 0; i < 2; i++) {
		idmas[i].id = i;
		idmas[i].did = 10 + i;
		idmas[i].reg = hw[i].regs;
		idmas[i].miscdev.parent = &devices[i];
		hw[i].latency = 20000;
		hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8] = IDMA_BATCH_MAGIC;
	}
	hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC_V6;
	hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8] = IDMA_LAUNCHER_MAGIC;
	return e;
}

int main(int argc, char **argv)
{
	struct idma_e3_controlled c;
	struct idma_experiment e;
	const char *name;
	long ret;

	assert(argc == 2);
	name = argv[1];
	e = controlled_input();
	memset(&c, 0, sizeof(c));
	c.pair.abi_version = 1;
	c.pair.struct_size = sizeof(c.pair);
	c.pair.request = e;

	if (!strcmp(name, "invalid_before_mmio")) {
		c.pair.abi_version = 0;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EINVAL);
		assert(!allocations && !obs_mmio && !l_gos);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "cold_solo_ok")) {
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		c.prep_mode = 0;
		controlled_mode = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(c.controlled[0].prime[1].iotlb_miss == 0);
		assert(c.controlled[0].prime[1].submitted == 0);
		assert(c.controlled[0].prime[0].iotlb_miss == 1);
		assert(c.controlled[0].probe.iotlb_miss == 0);
		assert(c.controlled[0].v_hw_cycles == 0xfeedU);
		assert(c.pair.rounds_attempted == 2 && c.pair.rounds_completed == 2);
		assert(hw[0].submits == 6); /* 3 per round, no CPU resubmit */
		assert(allocations == 4 && frees == 4 && unmaps == 4);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "hot_pair_lane")) {
		c.prep_mode = 1;
		c.pair_mode = 1;
		controlled_mode = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		/* A prime must read lane 1, V prime lane 0. */
		assert(c.controlled[0].prime[1].iotlb_miss == 1);
		assert(c.controlled[0].prime[0].iotlb_miss == 1);
		assert(c.controlled[0].probe.iotlb_miss == 0);
		assert(c.controlled[1].prime[1].iotlb_miss == 1);
		assert(c.controlled[1].prime[0].iotlb_miss == 1);
		assert(allocations == 4 && frees == 4);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "no_cpu_resubmit")) {
		controlled_mode = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		/* pair, cold: V prime + V probe + formal V + formal A; per round. */
		assert(hw[0].submits == 2 * 2);
		assert(hw[1].submits == 2 * 1);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "two_round_reset")) {
		c.pair.first_delay = 128;
		controlled_mode = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(c.pair.rounds_attempted == 2 && c.pair.rounds_completed == 2);
		/* Round 2 must be fresh: no inherited counters/positions. */
		assert(c.pair.result[0].victim.submitted == 1);
		assert(c.pair.result[1].victim.submitted == 1);
		assert(c.pair.result[1].measurement_complete);
		assert(c.pair.result[1].hpm_configured == 1);
		assert(c.controlled[1].prime[0].iotlb_miss == 1);
		assert(c.controlled[1].probe.iotlb_miss == 0);
		assert(c.controlled[0].flush_status == 0);
		assert(c.controlled[1].flush_status == 0);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "no_terminal_quarantine") ||
	    !strcmp(name, "first_error_preserved")) {
		controlled_mode = true;
		ctl_no_terminal = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(c.pair.status == -ETIMEDOUT);
		assert(c.pair.result[0].status == -ETIMEDOUT);
		assert(!c.pair.result[0].measurement_complete);
		assert(idmas[0].quarantined);
		assert(allocations > frees);
		assert(hpm_releases == 1 && hpm_acquires == 1);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "timeout_a_stall_v_held")) {
		controlled_mode = true;
		ctl_stall_a = true;
		e.run_timeout_ms = 1;
		c.pair.request = e;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT || ret == -EIO);
		assert(idmas[0].quarantined || idmas[1].quarantined);
		assert(allocations > frees);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "prime_failure_undrained")) {
		controlled_mode = true;
		hw[0].latency = UINT64_MAX;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(idmas[0].quarantined);
		assert(l_gos == 0);
		assert(allocations > frees);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "partial_prepare_zero_a")) {
		controlled_mode = true;
		ctl_zero_id_a = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(idmas[1].quarantined);
		assert(l_gos == 0);
		assert(allocations > frees);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "zero_id_v")) {
		controlled_mode = true;
		ctl_zero_id_v = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(idmas[0].quarantined);
		printf("PASS %s\n", name);
		return 0;
	}
	if (!strcmp(name, "bad_count_quarantine") ||
	    !strcmp(name, "bad_id_quarantine")) {
		controlled_mode = true;
		ctl_bad_count = !strcmp(name, "bad_count_quarantine");
		ctl_bad_id = !strcmp(name, "bad_id_quarantine");
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(allocations > frees);
		printf("PASS %s\n", name);
		return 0;
	}

	assert(!"unknown test");
	return 1;
}
```

---

## `tests/test_idma_controlled.py`

```python
#!/usr/bin/env python3
"""Compile and run production controlled-E3 functions against a host MMIO model.

This establishes software control flow only.  It is not FPGA timing or hardware
acceptance evidence.  Kernel DMA mapping, MMIO ordering, userspace copies, HPM
ownership and the global S1 flush implementation are stubbed or supplied
externally.  Complex production observers and idma_stream_prepare are replaced
by harness stubs; every other production helper is compiled unchanged.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = (
    'idma_pattern', 'idma_stream_cleanup', 'idma_quarantine', 'idma_submit',
    'idma_poll', 'idma_drain', 'idma_verify',
    'idma_snapshot', 'idma_snapshot_delta',
    'idma_observation_prepare', 'idma_observation_arm',
    'idma_stream_reseed',
    'idma_launcher_terminal', 'idma_launcher_held', 'idma_launcher_active',
    'idma_launcher_read_words', 'idma_observer_indexed',
    'idma_controlled_validate', 'idma_controlled_environment',
    'idma_controlled_prep_task', 'idma_controlled_account',
    'idma_controlled_apply', 'idma_controlled_validate_success',
    'idma_controlled_cleanup', 'idma_controlled_formal',
    'idma_controlled_one_round', 'idma_run_e3_controlled',
)
CASES = (
    'invalid_before_mmio',
    'cold_solo_ok',
    'hot_pair_lane',
    'no_cpu_resubmit',
    'two_round_reset',
    'no_terminal_quarantine',
    'first_error_preserved',
    'timeout_a_stall_v_held',
    'prime_failure_undrained',
    'partial_prepare_zero_a',
    'zero_id_v',
    'bad_count_quarantine',
    'bad_id_quarantine',
)


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(
            f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaControlledTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        build = Path(cls.temp.name)
        source = Path(os.environ.get(
            'IDMA_SOURCE', ROOT / 'linux/drivers/misc/idma.c')).read_text()
        definitions = '\n'.join(
            re.findall(r'^#define IDMA_.*$', source, re.MULTILINE))
        structures = '\n'.join(
            extract(source, rf'^struct {name} \{{.*?^\}};', name)
            for name in ('idma_device', 'idma_stream', 'idma_observation')
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_controlled_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */', definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'linux').mkdir()
        shutil.copyfile(ROOT / 'linux/include/uapi/linux/idma.h', build / 'linux/idma.h')
        (build / 'controlled.c').write_text(harness)
        cls.binary = build / 'controlled'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', '-I', str(build), str(build / 'controlled.c'),
            '-o', str(cls.binary),
        ], check=True)

    def test_production_controlled_control_flow(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run(
                    [str(self.binary), case], capture_output=True,
                    text=True, timeout=10)
                self.assertEqual(result.returncode, 0,
                                 result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)


if __name__ == '__main__':
    unittest.main()
```

---

## Fix-by-fix coverage (mapping the reviewer’s 14 points)

1. `out_cleanup` label exists; `idma_stream_prepare(v)` failure also routes there; partial `idma_stream_prepare` states are tolerated by `idma_stream_cleanup`.
2. Single `idma_controlled_cleanup` runs after every formal outcome; `idma_controlled_prep_task` drains bounded; quarantine on any non-idle proof; no `in_flight=false` used as a reset shortcut.
3. `idma_controlled_prep_task(s, lane, …)` reads `iotlb_miss[lane]/s1_ptw[lane]/s2_ptw[lane]` and that device’s `ACTIVE_CYCLES`; rejects `s2_ptw != 0`; probe additionally requires zero miss/S1.
4. Each prep task: offset 0, submit 1, bounded drain, FENCE.I, `idma_verify` of its own src/dst, then `idma_stream_reseed`; separate stats (`prime[0]`, `prime[1]`, `probe`); probe must be a real completion with zero cold miss; no IOTLB invalidation between phases (single flush at round entry).
5. `idma_controlled_environment` checks obs V6, launcher magic, observer idle, launcher PREP/RUN/held/active/error, both lanes’ batch magic and inactive backend before any write; ABORT/CLEAR only in `owned` cleanup.
6. Ownership set before CLEAR/PREP; all post-PREP failures fall to the common cleanup; `reserved_mask` tracked separately from `in_flight`; held mask must equal exactly 1 or 3.
7. First error is threaded through the cleanup (`first`/`first_error`); stage 8 only on full success; stage 6 set at GO, 7 at drain; `round->launcher[]` copied on all reachable paths; local `words[]` zeroed.
8. `idma_controlled_account` requires terminal, per-lane cancelled⇒not-issued-and-not-done, otherwise done; rejects completions for un-reserved lanes; `in_flight`/`touched_end` only applied after proof; actual stats copied before any failure.
9. ABORT only after ownership and only until terminal; failing terminal proof quarantines both lanes and never frees; no CLEAR before terminal.
10. `idma_controlled_validate_success` checks masks, one issue/done each, cancel 0, no errors, reserved IDs, timeline ordering, A-select identity, and zero Solo A fields.
11. `v_hw_cycles = idma_observer_indexed(o, 0)` after freeze; `snapshot_before_ns/after_ns` are DURATION values; `victim_elapsed_ns` is the software V-done elapsed and is documented as a different boundary from the HPM GO→drain window.
12. Each round re-clears `e` from `status` onward, keeps inputs/DIDs, sets `e.status` before `result[round]` copies even on prepare/acquire failure; `rounds_attempted` counts the entered round; HPM released exactly once.
13. `v->stats` is repointed to `&e->victim` at formal entry, so post-formal quarantine records in `e`, while prime/probe quarantines record in the phase stats.
14. Harness stubs only the two oversized observers and `idma_stream_prepare`; all other production helpers are compiled. It models the launcher with proper command/status semantics rather than a single cell.

---

## Assumptions / unresolved items (not claimed as run)

- `riscv_iommu_hpm_flush_s1()` is still assumed to be declared by the separate HPM worker; `idma.c` calls it but does not define it, and the harness stubs it. If the header is not updated in the same tree the driver will not link.
- `launcher[20..23]` (A-select NSID/DID/IOVA/write) and the exact launcher word meanings/timestamps are implemented to the values stated in the task text (`ID 4`, `DID 11`, `write 0`, A src IOVA). These checks are my reading of the contract and are the least certain part; if the hardware record differs, `idma_controlled_validate_success` must be adjusted. I could not confirm them against a capture.
- Held/active are treated as 4-bit counts (`bits 8..11`, `12..15`). If they are one-hot flags the mask comparisons change.
- `idma_e3_finish` gating for controlled mode is only `n = 1` plus bit 29; I did not prove that the V6/bank-v2 word layout needs more. The harness stubs `idma_e3_finish`, so this production path is compiled but its intrinsic validation is unexercised here.
- The harness is a control-flow model. It does not prove IOTLB flush behavior, cache coherency, MMIO ordering, real backpressure, or FPGA acceptance. I have not been able to compile or run it.
- `idma_controlled_validate_success` is deliberately only invoked on the success path in cleanup; on the abort/error path the accounting check (not the strict identity check) governs safety. If the parent wants identity checks on the abort path too, they can be added, but that changes which failures quarantine.
- I did not add any unrelated features; the only ABI addition is CAP_E3_CONTROLLED (bit 6) and the two new ioctl structs/0x0c.