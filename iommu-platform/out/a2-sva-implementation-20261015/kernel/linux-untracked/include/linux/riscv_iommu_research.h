/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _LINUX_RISCV_IOMMU_RESEARCH_H
#define _LINUX_RISCV_IOMMU_RESEARCH_H

#include <linux/types.h>
#include <linux/kconfig.h>
#include <linux/err.h>
#include <linux/errno.h>

struct device;
struct iommu_domain;
struct mm_struct;

struct riscv_iommu_research_certificate {
	u64 epoch;
	u64 posted;
	u64 cqb;
	u64 marker_phys;
	u64 marker_value;
	u32 token;
	u32 first;
	u32 head;
	u32 count;
	u32 cqcsr;
};

#if IS_REACHABLE(CONFIG_RISCV_IOMMU)
bool riscv_iommu_research_supported(struct device *dev0, struct device *dev1);

/*
 * Restricted current-platform experiment; requires iommu.research=1.
 * mm == NULL creates an ordinary unmanaged shadow domain. Otherwise borrow
 * the live plain-Sv39 root (mmget, never free its pages) without PASID/IOPF.
 * Caller keeps mmap_read_lock and pinned pages over the entire synchronous
 * episode, all DMA drain and successful context restoration. No asynchronous
 * use, changing mappings, notifier replacement, generic SVA or PR/PW drain.
 * Free an attached domain, or a formerly attached research domain without
 * a later successful restoration barrier, is refused and retains its mm.
 */
struct iommu_domain *riscv_iommu_research_domain_alloc(struct device *dev,
						     struct mm_struct *mm);
/* Returns an error without freeing anything if restore is not certified. */
int riscv_iommu_research_domain_release(struct iommu_domain *domain);

/*
 * Caller already owns DID10/11 exclusively and has proved ALL translation,
 * PTW/result/spill and R/W/B obligations quiescent; every other source is
 * stopped. This function does not establish that precondition. sync_cache
 * must synchronously publish all CPU stores and acquire the complete 8-byte
 * marker on the accepted single-hart CVA6 cache profile. It runs before tail
 * publication and before every token read; it must not post CQ commands.
 * Exclusive CQ posting and error recovery span all five contiguous entries.
 * A post-publication failure permanently poisons CQ and retains the marker:
 * caller MUST retain domain/mm/pages/owner and must not unlock borrowed mm
 * for teardown. A success proves the preceding invalidations and marker B,
 * not the sentinel's decode, absence of late FQ errors or DMA drain.
 */
int riscv_iommu_research_barrier(struct device *dev0, struct device *dev1,
				void (*sync_cache)(void *), void *sync_arg,
				struct riscv_iommu_research_certificate *cert);

#else
static inline bool riscv_iommu_research_supported(struct device *dev0,
						 struct device *dev1)
{
	return false;
}

static inline struct iommu_domain *
riscv_iommu_research_domain_alloc(struct device *dev, struct mm_struct *mm)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline int
riscv_iommu_research_domain_release(struct iommu_domain *domain)
{
	return -EOPNOTSUPP;
}

static inline int
riscv_iommu_research_barrier(struct device *dev0, struct device *dev1,
			   void (*sync_cache)(void *), void *sync_arg,
			   struct riscv_iommu_research_certificate *cert)
{
	return -EOPNOTSUPP;
}
#endif

#endif
