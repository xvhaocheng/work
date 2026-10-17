// SPDX-License-Identifier: GPL-2.0
/* Host model only: real production functions with explicit MM/page-table stubs. */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <linux/riscv_iommu_leaf.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef unsigned int gfp_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long prot; } pgprot_t;
typedef pmd_t pgd_t;
#define pmd_val(x) ((x).pmd)
#define pte_val(x) ((x).pte)
#define __pmd(x) ((pmd_t){(x)})
#define __pte(x) ((pte_t){(x)})
#define __pgprot(x) ((pgprot_t){(x)})
#define PAGE_SHIFT 12U
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PTRS_PER_PMD 512U
#define BITS_PER_LONG 64U
static unsigned int test_pgdir_shift = 30;
#define PGDIR_SHIFT test_pgdir_shift
#define BIT_ULL(n) (1ULL << (n))
#define GENMASK_ULL(h, l) ((~0ULL << (l)) & (~0ULL >> (63-(h))))
#define IS_ALIGNED(x, a) (!((x) & ((a)-1)))
#define CONFIG_64BIT 1
#define IS_ENABLED(x) (x)
#define _PAGE_PRESENT 1UL
#define _PAGE_READ 2UL
#define _PAGE_WRITE 4UL
#define _PAGE_EXEC 8UL
#define _PAGE_USER 16UL
#define _PAGE_GLOBAL 32UL
#define _PAGE_ACCESSED 64UL
#define _PAGE_DIRTY 128UL
#define _PAGE_LEAF (_PAGE_READ | _PAGE_WRITE | _PAGE_EXEC)
#define _PAGE_BASE (_PAGE_PRESENT | _PAGE_ACCESSED | _PAGE_USER)
#define _PAGE_TABLE _PAGE_PRESENT
#define _PAGE_PFN_SHIFT 10U
#define _PAGE_PFN_MASK GENMASK_ULL(53, 10)
#define __page_val_to_pfn(v) (((v) & _PAGE_PFN_MASK) >> _PAGE_PFN_SHIFT)
#define pte_pfn(p) __page_val_to_pfn(pte_val(p))
#define pfn_to_phys(pfn) ((phys_addr_t)(pfn) << PAGE_SHIFT)
#define phys_to_pfn(phys) ((unsigned long)(phys) >> PAGE_SHIFT)
#define virt_to_pfn(ptr) ((unsigned long)(ptr) >> PAGE_SHIFT)
#define pfn_to_virt(pfn) ((void *)((unsigned long)(pfn) << PAGE_SHIFT))
#define pmd_present(p) (pmd_val(p) & (_PAGE_PRESENT | _PAGE_GLOBAL | _PAGE_LEAF))
#define pmd_leaf(p) (pmd_present(p) && (pmd_val(p) & _PAGE_LEAF))
#define pmd_none(p) (!pmd_val(p))
#define pte_present(p) (pte_val(p) & (_PAGE_PRESENT | _PAGE_GLOBAL))
#define set_pmd(p, v) (*(p) = (v))
#define set_pte(p, v) (*(p) = (v))
#define IOMMU_READ 1U
#define IOMMU_WRITE 2U
#define IOMMU_DOMAIN_DMA 1U
#define IOMMU_DOMAIN_DMA_FQ 2U
#define IOMMU_DOMAIN_UNMANAGED 3U
#define IOMMU_DOMAIN_IDENTITY 4U
#define IOMMU_DOMAIN_BLOCKED 5U
#define IOMMU_DOMAIN_SVA 6U
#define GFP_KERNEL 0U
#define container_of(p, t, m) ((t *)((char *)(p) - offsetof(t, m)))
#define io_pgtable_to_domain(p) container_of((p), struct riscv_iommu_domain, pgtbl)
#define io_pgtable_ops_to_domain(p) io_pgtable_to_domain(container_of((p), struct io_pgtable, ops))
#define iommu_domain_to_riscv(p) container_of(p, struct riscv_iommu_domain, domain)
static unsigned long read_count;
#define READ_ONCE(x) (read_count++, (x))

struct iommu_domain_ops { int dummy; };
struct iommu_domain {
	unsigned int type;
	const struct iommu_domain_ops *ops;
	struct { unsigned long aperture_start, aperture_end; } geometry;
};
struct io_pgtable_ops { int dummy; };
struct io_pgtable { struct io_pgtable_ops ops; };
struct riscv_iommu_domain {
	struct iommu_domain domain;
	struct io_pgtable pgtbl;
	void *iommu;
	bool g_stage;
	struct riscv_iommu_domain *nested;
	unsigned int mode;
	pgd_t *pgd_root;
};
struct iommu_iotlb_gather { size_t bytes; };
static const struct iommu_domain_ops riscv_iommu_domain_ops;
static const struct iommu_domain_ops foreign_ops = {1};
static u64 satp_mode = 8ULL << 60;

static struct { void *p; bool freed; } pages[256];
static unsigned int nr_pages, freed_pages, checks, query_count;
static unsigned long get_zeroed_page(gfp_t gfp)
{
	(void)gfp;
	assert(nr_pages < 256);
	void *p = aligned_alloc(PAGE_SIZE, PAGE_SIZE);
	assert(p);
	memset(p, 0, PAGE_SIZE);
	pages[nr_pages++].p = p;
	return (unsigned long)p;
}
static void free_page(unsigned long pointer)
{
	for (unsigned int i = 0; i < nr_pages; i++)
		if (pages[i].p == (void *)pointer) {
			assert(!pages[i].freed);
			pages[i].freed = true;
			freed_pages++;
			free(pages[i].p);
			return;
		}
	assert(!"freeing an unallocated table");
}
static bool pfn_valid(unsigned long pfn)
{
	for (unsigned int i = 0; i < nr_pages; i++)
		if (!pages[i].freed && virt_to_pfn(pages[i].p) == pfn)
			return true;
	return false;
}
#define virt_addr_valid(p) pfn_valid(virt_to_pfn(p))
static pte_t pfn_pte(unsigned long pfn, pgprot_t prot)
{
	return __pte((pfn << _PAGE_PFN_SHIFT) | prot.prot);
}
static void iommu_iotlb_gather_add_page(struct iommu_domain *d,
		struct iommu_iotlb_gather *g, unsigned long iova, size_t size)
{
	(void)d; (void)iova;
	g->bytes += size;
}

/* CONSTANTS */
/* FUNCTIONS */

static u64 digest(void)
{
	u64 hash = 1469598103934665603ULL;
	for (unsigned int i = 0; i < nr_pages; i++) {
		if (pages[i].freed) continue;
		const unsigned char *bytes = pages[i].p;
		for (size_t j = 0; j < PAGE_SIZE; j++) hash = (hash ^ bytes[j])*1099511628211ULL;
	}
	return hash;
}
static int query(struct iommu_domain *d, unsigned long iova,
		 struct riscv_iommu_leaf_info *out)
{
	u64 before = digest();
	unsigned int alloc_before = nr_pages, free_before = freed_pages;
	unsigned long reads_before = read_count;
	assert(query_count < 100000);
	int ret = riscv_iommu_query_leaf(d, iova, out);
	assert(digest() == before && nr_pages == alloc_before && freed_pages == free_before);
	assert(read_count - reads_before <= 5);
	query_count++;
	return ret;
}
static void rejected(struct iommu_domain *d, unsigned long iova, int expected)
{
	struct riscv_iommu_leaf_info out;
	memset(&out, 0xa5, sizeof(out));
	assert(query(d, iova, &out) == expected);
	struct riscv_iommu_leaf_info zero = {};
	assert(!memcmp(&out, &zero, sizeof(out)));
	checks++;
}
static struct riscv_iommu_domain domain_new(unsigned int shift)
{
	test_pgdir_shift = shift;
	satp_mode = (u64)(8 + (shift - 30)/9) << 60;
	struct riscv_iommu_domain d = {
		.domain = {.type = IOMMU_DOMAIN_DMA, .ops = &riscv_iommu_domain_ops,
			.geometry = {0, (1UL << (shift + 8))-1}},
		.iommu = (void *)1, .mode = satp_mode >> 60,
		.pgd_root = (void *)get_zeroed_page(0),
	};
	return d;
}
static void host_teardown(void)
{
	/* Test-only end-of-domain reclamation includes baseline's orphan tables. */
	for (unsigned int i = 0; i < nr_pages; i++) if (!pages[i].freed) free(pages[i].p);
	memset(pages, 0, sizeof(pages));
	nr_pages = freed_pages = 0;
}
static void map(struct riscv_iommu_domain *d, unsigned long iova,
		phys_addr_t phys, size_t pgsize, size_t nr, int prot)
{
	size_t mapped = 0;
	assert(!riscv_iommu_map_pages(&d->pgtbl.ops, iova, phys, pgsize, nr, prot, 0, &mapped));
	assert(mapped == pgsize*nr);
}
static void unmap(struct riscv_iommu_domain *d, unsigned long iova, size_t pgsize, size_t nr)
{
	struct iommu_iotlb_gather gather = {};
	assert(riscv_iommu_unmap_pages(&d->pgtbl.ops, iova, pgsize, nr, &gather) == pgsize*nr);
	assert(gather.bytes == pgsize*nr);
}
static void range_check(struct riscv_iommu_domain *d, unsigned long iova,
			phys_addr_t phys, unsigned int shift, size_t bytes)
{
	struct riscv_iommu_leaf_info out;
	for (size_t off = 0; off < bytes; off += PAGE_SIZE) {
		assert(!query(&d->domain, iova + off, &out));
		assert(out.leaf_shift == shift && out.phys == phys + off);
		assert(out.leaf_iova_base == ((iova + off) & ~((1UL << shift)-1)));
		assert((out.raw_pte & (_PAGE_PRESENT | _PAGE_READ)) == (_PAGE_PRESENT | _PAGE_READ));
		assert(riscv_iommu_iova_to_phys(&d->pgtbl.ops, iova+off) == phys+off);
	}
	assert(!query(&d->domain, iova + bytes-1, &out));
	assert(out.phys == phys + bytes-1 && out.leaf_shift == shift);
	checks++;
}

int main(void)
{
	const unsigned long iova = 0x400000;
	const phys_addr_t phys = 0x80000000;
	const size_t two_m = 1UL << 21;
	for (unsigned int levels = 30; levels <= 48; levels += 9) {
		fprintf(stderr, "PROGRESS paging shift=%u begin\n", levels);
		struct riscv_iommu_domain d = domain_new(levels);
		rejected(&d.domain, iova, -ENOENT);
		map(&d, iova, phys, PAGE_SIZE, 512, IOMMU_READ | IOMMU_WRITE);
		range_check(&d, iova, phys, 12, two_m);
		fprintf(stderr, "PROGRESS 4K range queries=%u\n", query_count);
		rejected(&d.domain, iova+two_m, -ENOENT);
		unmap(&d, iova, PAGE_SIZE, 512);
		rejected(&d.domain, iova, -ENOENT);
		map(&d, iova, phys, two_m, 1, IOMMU_READ);
		range_check(&d, iova, phys, 21, two_m);
		fprintf(stderr, "PROGRESS 2M range queries=%u\n", query_count);
		unmap(&d, iova, two_m, 1);
		host_teardown();
	}
	fprintf(stderr, "PROGRESS rejection cases\n");
	struct riscv_iommu_domain d = domain_new(30);
	rejected(NULL, iova, -EOPNOTSUPP);
	struct iommu_domain *foreign = calloc(1, sizeof(*foreign));
	assert(foreign); foreign->ops = &foreign_ops;
	rejected(foreign, iova, -EOPNOTSUPP); free(foreign);
	assert(query(&d.domain, iova, NULL) == -EINVAL); checks++;
	for (unsigned int type = IOMMU_DOMAIN_IDENTITY; type <= IOMMU_DOMAIN_SVA; type++) {
		d.domain.type = type; rejected(&d.domain, iova, -EOPNOTSUPP);
	}
	d.domain.type = IOMMU_DOMAIN_DMA;
	d.g_stage = true; rejected(&d.domain, iova, -EOPNOTSUPP); d.g_stage = false;
	d.nested = &d; rejected(&d.domain, iova, -EOPNOTSUPP); d.nested = NULL;
	d.iommu = NULL; rejected(&d.domain, iova, -EOPNOTSUPP); d.iommu = (void *)1;
	d.mode++; rejected(&d.domain, iova, -EOPNOTSUPP); d.mode--;
	pgd_t *root = d.pgd_root;
	d.pgd_root = NULL; rejected(&d.domain, iova, -EOPNOTSUPP);
	d.pgd_root = (void *)0x1000; rejected(&d.domain, iova, -EFAULT);
	d.pgd_root = (void *)((char *)root+8); rejected(&d.domain, iova, -EFAULT); d.pgd_root = root;
	d.domain.geometry.aperture_start = PAGE_SIZE; rejected(&d.domain, 0, -ERANGE);
	d.domain.geometry.aperture_start = 0;
	rejected(&d.domain, d.domain.geometry.aperture_end+1, -ERANGE);
	d.domain.geometry.aperture_end = ULONG_MAX; rejected(&d.domain, 1UL << 38, -ERANGE);
	d.domain.geometry.aperture_end = (1UL << 38)-1;
	map(&d, iova, phys, PAGE_SIZE, 1, IOMMU_READ | IOMMU_WRITE);
	unsigned int leaf_shift = 0;
	pte_t *leaf = riscv_iommu_pt_walk_fetch((pmd_t *)d.pgd_root, iova, PGDIR_SHIFT, true, &leaf_shift);
	assert(leaf && leaf_shift == 12);
	u64 original = pte_val(*leaf);
	*leaf = __pte(original & ~_PAGE_PRESENT); rejected(&d.domain, iova, -ENOENT);
	for (unsigned int bit = 54; bit < 64; bit++) {
		*leaf = __pte(original | BIT_ULL(bit)); rejected(&d.domain, iova, -EOPNOTSUPP);
	}
	*leaf = __pte((original | _PAGE_WRITE) & ~_PAGE_READ); rejected(&d.domain, iova, -EINVAL);
	*leaf = __pte(_PAGE_TABLE); rejected(&d.domain, iova, -EINVAL);
	*leaf = __pte(original);
	pmd_t *parent = &((pmd_t *)root)[0];
	u64 old_parent = pmd_val(*parent);
	for (unsigned int flag = _PAGE_USER; flag <= _PAGE_DIRTY; flag *= 2) {
		if (flag == _PAGE_GLOBAL) continue;
		*parent = __pmd(old_parent | flag); rejected(&d.domain, iova, -EINVAL);
	}
	*parent = __pmd(_PAGE_TABLE); rejected(&d.domain, iova, -EFAULT);
	*parent = __pmd(old_parent);
	unmap(&d, iova, PAGE_SIZE, 1);
	map(&d, iova, phys, two_m, 1, IOMMU_READ | IOMMU_WRITE);
	leaf = riscv_iommu_pt_walk_fetch((pmd_t *)root, iova, PGDIR_SHIFT, true, &leaf_shift);
	original = pte_val(*leaf);
	*leaf = __pte(original + (1UL << _PAGE_PFN_SHIFT)); rejected(&d.domain, iova, -EINVAL);
	*leaf = __pte(original);
	for (unsigned int type = IOMMU_DOMAIN_DMA_FQ; type <= IOMMU_DOMAIN_UNMANAGED; type++) {
		d.domain.type = type; range_check(&d, iova, phys, 21, two_m);
		fprintf(stderr, "PROGRESS 2M range queries=%u\n", query_count);
	}
	d.domain.type = IOMMU_DOMAIN_DMA;
	unmap(&d, iova, two_m, 1);
	map(&d, 0, phys, 1UL << 30, 1, IOMMU_READ);
	struct riscv_iommu_leaf_info out;
	assert(!query(&d.domain, (1UL << 30)-1, &out));
	assert(out.leaf_shift == 30 && out.phys == phys+(1UL << 30)-1); checks++;
	unmap(&d, 0, 1UL << 30, 1);
	host_teardown();

	fprintf(stderr, "PROGRESS alternation\n");
	d = domain_new(30);
	unsigned int live[6];
	for (unsigned int cycle = 0; cycle < 3; cycle++) {
		map(&d, iova, phys, PAGE_SIZE, 512, IOMMU_READ | IOMMU_WRITE);
		range_check(&d, iova, phys, 12, two_m);
		fprintf(stderr, "PROGRESS 4K range queries=%u\n", query_count);
		unmap(&d, iova, PAGE_SIZE, 512); rejected(&d.domain, iova, -ENOENT);
		live[2*cycle] = nr_pages - freed_pages;
		map(&d, iova, phys, two_m, 1, IOMMU_READ | IOMMU_WRITE);
		range_check(&d, iova, phys, 21, two_m);
		fprintf(stderr, "PROGRESS 2M range queries=%u\n", query_count);
		unmap(&d, iova, two_m, 1); rejected(&d.domain, iova, -ENOENT);
		live[2*cycle+1] = nr_pages - freed_pages;
	}
	assert(live[0] == 3 && live[1] == 3 && live[2] == 4 && live[3] == 4 && live[4] == 5 && live[5] == 5);
	unsigned int frees_before = freed_pages;
	riscv_iommu_pt_walk_free(&d.pgd_root[0], 21, false);
	assert(freed_pages == frees_before); /* Existing empty-table guard, not repaired. */
	checks++;
	printf("BASELINE_ALTERNATION_LIVE_TABLES=%u,%u,%u,%u,%u,%u; empty_table_free_delta=%u\n",
	       live[0],live[1],live[2],live[3],live[4],live[5],freed_pages-frees_before);
	host_teardown();
	printf("PASS %u finite cases; %u read-only production queries; all query table digests unchanged\n", checks, query_count);
	return 0;
}
