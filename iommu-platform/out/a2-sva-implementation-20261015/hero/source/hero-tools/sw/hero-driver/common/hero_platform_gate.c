// SPDX-License-Identifier: GPL-2.0 OR Apache-2.0
#include <linux/errno.h>
#include "hero_iommu.h"

/* Replace this translation unit only after the pinned hardware contract is
 * implemented and accepted. No module parameter can bypass this boundary. */
int hero_platform_quiesce(struct device *dev) { return -EOPNOTSUPP; }
int hero_platform_invalidate_complete(struct device *dev,
                                      struct iommu_domain *domain)
{ return -EOPNOTSUPP; }
int hero_platform_cache_sync(struct device *dev, struct page **pages,
                             unsigned long nr_pages, bool for_cpu)
{ return -EOPNOTSUPP; }
void hero_platform_resume(struct device *dev) { }
