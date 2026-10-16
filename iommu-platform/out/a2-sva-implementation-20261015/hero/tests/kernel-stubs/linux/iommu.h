#pragma once
#include "types.h"
#define IOMMU_READ 1
#define IOMMU_WRITE 2
struct iommu_domain { int unused; };
int iommu_map(struct iommu_domain *domain, unsigned long iova, u64 phys, size_t len, int prot, int flags);
size_t iommu_unmap(struct iommu_domain *domain, unsigned long iova, size_t len);
