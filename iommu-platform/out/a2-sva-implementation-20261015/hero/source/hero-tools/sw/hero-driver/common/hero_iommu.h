// SPDX-License-Identifier: GPL-2.0 OR Apache-2.0
#pragma once
#include <linux/iommu.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mutex.h>

#define HERO_IOVA_BASE 0x80000000ULL
#define HERO_IOVA_SIZE 0x40000000ULL
#define HERO_BYPASS_OFFSET 0x200000000ULL

struct hero_iommu_region {
    struct list_head list;
    const void *owner;
    u64 user_addr, length, iova;
    struct page **pages;
    unsigned long nr_pages, base_mapped, bypass_mapped;
    bool quarantined;
};

/* Every entry point requires the device-wide iommu_lock. */
int hero_iommu_region_add(struct device *dev, struct iommu_domain *domain,
                         struct list_head *regions, const void *owner,
                         u64 user_addr, u64 iova, u64 length);
int hero_iommu_region_remove(struct device *dev, struct iommu_domain *domain,
                            struct list_head *regions, const void *owner,
                            u64 user_addr, u64 iova, u64 length);
int hero_iommu_release_owner(struct device *dev, struct iommu_domain *domain,
                             struct list_head *regions, const void *owner);
int hero_iommu_cache_sync(struct device *dev, struct list_head *regions,
                          const void *owner, u64 user_addr, u64 iova,
                          u64 length, bool for_cpu);

/* Platform acceptance boundary. Default implementations fail closed.
 * quiesce: stop ALL domain publishers, drain accepted DMA read/write requests,
 * wait for final write responses, and hold them stopped until resume().
 * invalidate_complete: complete all outstanding page-table updates and context/
 * IOTLB invalidations, using a hardware completion receipt with timeout/error
 * propagation. iommu_unmap() or queue head advancement alone is NOT sufficient.
 * cache_sync: complete CPU/private-cache and LLC clean/invalidate for every
 * pinned page, covering the physical bypass alias. No asynchronous flush.
 * resume: only release a successful quiesce; cannot create work by itself.
 * These functions must be idempotent and must not re-enter the device mutex.
 */
int hero_platform_quiesce(struct device *dev);
int hero_platform_invalidate_complete(struct device *dev,
                                      struct iommu_domain *domain);
int hero_platform_cache_sync(struct device *dev, struct page **pages,
                             unsigned long nr_pages, bool for_cpu);
void hero_platform_resume(struct device *dev);
