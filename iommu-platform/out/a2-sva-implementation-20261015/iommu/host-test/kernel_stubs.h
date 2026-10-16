#ifndef A2_KERNEL_STUBS_H
#define A2_KERNEL_STUBS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
typedef unsigned int gfp_t;
typedef uint64_t phys_addr_t;
typedef uint64_t pmd_t;
typedef uint64_t pte_t;
typedef uint64_t pgprot_t;
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PGDIR_SHIFT 30
#define PTRS_PER_PMD 512
#define GFP_KERNEL 0
#define _PAGE_PFN_SHIFT 10
#define _PAGE_PRESENT 1UL
#define _PAGE_TABLE _PAGE_PRESENT
#define _PAGE_READ 2UL
#define _PAGE_WRITE 4UL
#define _PAGE_EXEC 8UL
#define _PAGE_DIRTY 128UL
#define _PAGE_BASE (_PAGE_PRESENT | 64UL)
#define SZ_4K (1UL << 12)
#define SZ_2M (1UL << 21)
#define SZ_1G (1UL << 30)
#define IOMMU_PAGE_SIZE_4K SZ_4K
#define IOMMU_PAGE_SIZE_2M SZ_2M
#define IOMMU_PAGE_SIZE_1G SZ_1G
#define IOMMU_PAGE_SIZE_512G (1UL << 39)
#define IOMMU_DOMAIN_BLOCKED 1
#define IOMMU_DOMAIN_IDENTITY 2
#define IOMMU_DOMAIN_UNMANAGED 3
#define IOMMU_WRITE 2
#define WARN_ON(x) ({ bool w = !!(x); if (w) fprintf(stderr, "WARN_ON %s:%d\n", __FILE__, __LINE__); w; })
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
#define pmd_val(x) (x)
#define pmd_present(x) (!!((x) & _PAGE_PRESENT))
#define pmd_leaf(x) (!!((x) & (_PAGE_READ | _PAGE_WRITE | _PAGE_EXEC)))
#define pmd_none(x) (!(x))
#define __page_val_to_pfn(x) ((x) >> _PAGE_PFN_SHIFT)
#define pfn_to_virt(x) ((void *)(uintptr_t)((x) << PAGE_SHIFT))
#define virt_to_pfn(x) ((uintptr_t)(x) >> PAGE_SHIFT)
#define phys_to_pfn(x) ((x) >> PAGE_SHIFT)
#define pfn_to_phys(x) ((phys_addr_t)(x) << PAGE_SHIFT)
#define __pmd(x) (x)
#define __pte(x) (x)
#define __pgprot(x) (x)
#define set_pmd(p,x) (*(p) = (x))
#define set_pte(p,x) (*(p) = (x))
#define pfn_pte(pfn,prot) (((uint64_t)(pfn) << _PAGE_PFN_SHIFT) | (prot))
#define pte_present(x) pmd_present(x)
#define pte_pfn(x) __page_val_to_pfn(x)
#define printk(...) ((void)0)
struct iommu_domain { unsigned int type; };
struct iommu_iotlb_gather { unsigned unused; };
struct io_pgtable_ops {
 int (*map_pages)(struct io_pgtable_ops *, unsigned long, phys_addr_t, size_t, size_t, int, gfp_t, size_t *);
 size_t (*unmap_pages)(struct io_pgtable_ops *, unsigned long, size_t, size_t, struct iommu_iotlb_gather *);
 phys_addr_t (*iova_to_phys)(struct io_pgtable_ops *, unsigned long);
};
struct iommu_flush_ops {
 void (*tlb_flush_all)(void *);
 void (*tlb_flush_walk)(unsigned long, size_t, size_t, void *);
 void (*tlb_add_page)(struct iommu_iotlb_gather *, unsigned long, size_t, void *);
};
struct io_pgtable_cfg { size_t pgsize_bitmap; unsigned ias, oas; const struct iommu_flush_ops *tlb; };
struct io_pgtable { struct io_pgtable_cfg cfg; struct io_pgtable_ops ops; };
struct riscv_iommu_domain { struct iommu_domain domain; struct io_pgtable pgtbl; pmd_t *pgd_root; };
struct io_pgtable_init_fns { struct io_pgtable *(*alloc)(struct io_pgtable_cfg *, void *); void (*free)(struct io_pgtable *); };
static inline void iommu_iotlb_gather_add_page(struct iommu_domain *d, struct iommu_iotlb_gather *g, unsigned long iova, size_t size) {}
static unsigned long get_zeroed_page(gfp_t gfp);
static void free_page(unsigned long ptr);
#endif
