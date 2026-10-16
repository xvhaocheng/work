/* Host ownership test: directly includes unchanged production translation unit. */
#include "kernel_stubs.h"
#define _RISCV_IOMMU_H_
#ifndef IO_PGTABLE_SOURCE
#define IO_PGTABLE_SOURCE "/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/io_pgtable.c"
#endif
#include IO_PGTABLE_SOURCE

struct allocation { void *ptr; unsigned freed; };
static struct allocation allocs[128];
static size_t nalloc;
static unsigned failures, checks, cases;
static const char *case_name;

static unsigned long get_zeroed_page(gfp_t gfp)
{
 void *p = NULL;
 (void)gfp;
 assert(nalloc < sizeof(allocs) / sizeof(allocs[0]));
 assert(posix_memalign(&p, PAGE_SIZE, PAGE_SIZE) == 0);
 memset(p, 0, PAGE_SIZE);
 allocs[nalloc++] = (struct allocation){p, 0};
 return (unsigned long)p;
}
static void free_page(unsigned long ptr)
{
 for (size_t i = 0; i < nalloc; i++) {
  if ((unsigned long)allocs[i].ptr == ptr) { allocs[i].freed++; return; }
 }
 fprintf(stderr, "%s: attempted free of unknown address %#lx\n", case_name, ptr);
 failures++;
}
static unsigned free_count(const void *p)
{
 for (size_t i = 0; i < nalloc; i++) if (allocs[i].ptr == p) return allocs[i].freed;
 abort();
}
static void expect(const char *what, unsigned got, unsigned want)
{
 checks++;
 if (got != want) { fprintf(stderr, "%s: %s: got %u expected %u\n", case_name, what, got, want); failures++; }
}
static void begin(const char *name)
{
 assert(nalloc == 0); case_name = name; cases++;
}
static void end(void)
{
 for (size_t i = 0; i < nalloc; i++) free(allocs[i].ptr);
 nalloc = 0;
}
static pmd_t table(pmd_t *p) { return (virt_to_pfn(p) << _PAGE_PFN_SHIFT) | _PAGE_TABLE; }
static pmd_t leaf(void *p) { return (virt_to_pfn(p) << _PAGE_PFN_SHIFT) | _PAGE_PRESENT | _PAGE_READ | _PAGE_WRITE; }
static void destroy(struct riscv_iommu_domain *d)
{
 io_pgtable_riscv_init_fns.free(&d->pgtbl);
 expect("root retained by io-pgtable destructor", free_count(d->pgd_root), 0);
 free_page((unsigned long)d->pgd_root);
 expect("root released exactly once by domain owner", free_count(d->pgd_root), 1);
}
static struct riscv_iommu_domain domain(void)
{
 struct riscv_iommu_domain d = {.domain.type = IOMMU_DOMAIN_UNMANAGED};
 d.pgd_root = (pmd_t *)get_zeroed_page(0);
 return d;
}
int main(void)
{
 struct riscv_iommu_domain d;
 pmd_t *mid, *low, *low2;
 void *data, *data2;
 begin("empty root");
 d = domain(); destroy(&d); end();

 begin("entry zero absent with nonzero leaf");
 d = domain(); mid = (pmd_t *)get_zeroed_page(0); low = (pmd_t *)get_zeroed_page(0); data = (void *)get_zeroed_page(0);
 d.pgd_root[7] = table(mid); mid[23] = table(low); low[113] = leaf(data);
 destroy(&d); expect("middle table freed", free_count(mid), 1); expect("leaf table freed", free_count(low), 1); expect("data page retained", free_count(data), 0); end();

 begin("owned empty middle table");
 d = domain(); mid = (pmd_t *)get_zeroed_page(0); d.pgd_root[5] = table(mid);
 destroy(&d); expect("empty middle table freed", free_count(mid), 1); end();

 begin("nested tables with entry zero leaf");
 d = domain(); mid = (pmd_t *)get_zeroed_page(0); low = (pmd_t *)get_zeroed_page(0); data = (void *)get_zeroed_page(0);
 d.pgd_root[0] = table(mid); mid[0] = table(low); low[0] = leaf(data);
 destroy(&d); expect("middle table freed", free_count(mid), 1); expect("leaf table freed", free_count(low), 1); expect("data page retained", free_count(data), 0); end();

 begin("huge root leaf is a data page");
 d = domain(); data = (void *)get_zeroed_page(0); d.pgd_root[19] = leaf(data);
 destroy(&d); expect("huge data backing retained", free_count(data), 0); end();

 begin("two sparse leaf-table branches");
 d = domain(); mid = (pmd_t *)get_zeroed_page(0); low = (pmd_t *)get_zeroed_page(0); low2 = (pmd_t *)get_zeroed_page(0); data = (void *)get_zeroed_page(0); data2 = (void *)get_zeroed_page(0);
 d.pgd_root[255] = table(mid); mid[1] = table(low); mid[511] = table(low2); low[17] = leaf(data); low2[511] = leaf(data2);
 destroy(&d); expect("middle freed", free_count(mid), 1); expect("first leaf table freed", free_count(low), 1); expect("second leaf table freed", free_count(low2), 1); expect("first data retained", free_count(data), 0); expect("second data retained", free_count(data2), 0); end();

 begin("actual map-unmap then empty leaf-table destruction");
 d = domain(); io_pgtable_riscv_init_fns.alloc(&d.pgtbl.cfg, &d);
 for (unsigned i = 0; i < 4; i++) {
  size_t mapped = 0;
  void *page = (void *)get_zeroed_page(0);
  int ret = d.pgtbl.ops.map_pages(&d.pgtbl.ops, 0x12345000UL + i * PAGE_SIZE, (phys_addr_t)(uintptr_t)page, PAGE_SIZE, 1, IOMMU_WRITE, 0, &mapped);
  expect("real map return", (unsigned)ret, 0); expect("real mapped size", (unsigned)mapped, PAGE_SIZE);
  expect("real translation identity", d.pgtbl.ops.iova_to_phys(&d.pgtbl.ops, 0x12345000UL + i * PAGE_SIZE) == (phys_addr_t)(uintptr_t)page, 1);
 }
 expect("real unmap size", (unsigned)d.pgtbl.ops.unmap_pages(&d.pgtbl.ops, 0x12345000UL, PAGE_SIZE, 4, NULL), 4 * PAGE_SIZE);
 expect("real translation gone", d.pgtbl.ops.iova_to_phys(&d.pgtbl.ops, 0x12345000UL) == 0, 1);
 mid = pfn_to_virt(__page_val_to_pfn(d.pgd_root[(0x12345000UL >> 30) & 511]));
 low = pfn_to_virt(__page_val_to_pfn(mid[(0x12345000UL >> 21) & 511]));
 destroy(&d); expect("unmapped middle table freed", free_count(mid), 1); expect("unmapped leaf table freed", free_count(low), 1);
 for (size_t i = 0; i < nalloc; i++) if (allocs[i].ptr != d.pgd_root && allocs[i].ptr != mid && allocs[i].ptr != low) expect("unmapped data page retained", allocs[i].freed, 0);
 end();
 printf("%u cases, %u ownership/translation checks, %u failures\n", cases, checks, failures);
 return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
