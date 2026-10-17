/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _LINUX_RISCV_IOMMU_LEAF_H
#define _LINUX_RISCV_IOMMU_LEAF_H

#include <linux/errno.h>
#include <linux/kconfig.h>
#include <linux/string.h>
#include <linux/types.h>

struct iommu_domain;

struct riscv_iommu_leaf_info {
	u64 raw_pte;
	phys_addr_t phys;
	unsigned long leaf_iova_base;
	u32 leaf_shift;
};

/*
 * Read a normal leaf of the current driver's positive-aperture, RV64
 * single-stage domain. Svnapot/Svpbmt and reserved PTE encodings are rejected.
 * The caller keeps the domain and its mappings alive and excludes modification
 * of the queried range for the whole walk. This is not a concurrent page-table
 * snapshot or an IOTLB query; it neither maps, invalidates nor drains anything.
 * Errors zero *out when out is non-NULL. A NULL out returns -EINVAL.
 */
#if IS_REACHABLE(CONFIG_RISCV_IOMMU)
int riscv_iommu_query_leaf(struct iommu_domain *domain, unsigned long iova,
			   struct riscv_iommu_leaf_info *out);
#else
static inline int
riscv_iommu_query_leaf(struct iommu_domain *domain, unsigned long iova,
		       struct riscv_iommu_leaf_info *out)
{
	if (!out)
		return -EINVAL;
	memset(out, 0, sizeof(*out));
	return -EOPNOTSUPP;
}
#endif

#endif /* _LINUX_RISCV_IOMMU_LEAF_H */
