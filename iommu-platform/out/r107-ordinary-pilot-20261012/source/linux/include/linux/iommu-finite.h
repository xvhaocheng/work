/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _LINUX_IOMMU_FINITE_H
#define _LINUX_IOMMU_FINITE_H

#include <linux/errno.h>
#include <linux/kconfig.h>
#include <linux/types.h>

struct device;

/* iova and pa are the actual leaf bases, not the queried byte address. */
struct iommu_finite_leaf {
	u64 iova, pa, bytes, pte;
	u64 tables[5]; /* Actual page-table page PAs, root first. */
	u32 table_count, reserved;
};

struct iommu_finite_allocation {
	void *cpu_addr;
	dma_addr_t handle;
	size_t size;
};

/*
 * Private finite-owner interface, not a general DMA allocation validator.
 * The caller owns the HPM session, keeps device/domain binding stable, and
 * excludes every other map/unmap or DMA user of these allocation extents.
 * The domain must have exactly this one endpoint. Domain->lock does NOT
 * serialize the legacy map/unmap callbacks.
 *
 * items must be the caller's exact kernel-owned successful dma_alloc_attrs
 * records from GFP_KERNEL allocations, with FORCE_CONTIGUOUS or the normal
 * remap fallback. No user-supplied kernel pointer, direct/coherent override,
 * atomic pool, NO_KERNEL_MAPPING allocation, or foreign allocation is valid.
 * Atomic-pool provenance is a caller precondition: this kernel has no public
 * non-destructive membership query, and the helper does not pretend otherwise.
 *
 * Before release, the caller has proved all owned DONE IDs and global quiet.
 * checked_sync must publish the PTE stores and complete a checked host-S1
 * invalidation/fence, without issuing DMA, changing binding or releasing HPM.
 * A nonzero result retains ALL IOVA reservations and backing, even if PTEs
 * were partially/fully revoked. Quarantine the owner; do not retry or remap.
 * Records are never rewritten; on success they describe RELEASED resources.
 */
/* dma-iommu is built in; a modular RISC-V provider is not supported here. */
#if IS_BUILTIN(CONFIG_RISCV_IOMMU)
int riscv_iommu_finite_leaf(struct device *dev, unsigned long iova,
			    struct iommu_finite_leaf *out);
int iommu_dma_free_finite_checked(struct device *dev,
				  struct iommu_finite_allocation *items,
				  unsigned int nr,
				  int (*checked_sync)(void *), void *sync_data);
#else
static inline int riscv_iommu_finite_leaf(struct device *dev,
					  unsigned long iova, struct iommu_finite_leaf *out)
{
	return -EOPNOTSUPP;
}

static inline int iommu_dma_free_finite_checked(struct device *dev,
						struct iommu_finite_allocation *items,
						unsigned int nr,
						int (*checked_sync)(void *), void *sync_data)
{
	return -EOPNOTSUPP;
}
#endif

#endif /* _LINUX_IOMMU_FINITE_H */
