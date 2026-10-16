/* Software PTE encoding model; the production walk and lookup are inserted. */
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t pmd_t;
typedef uint64_t pte_t;
typedef uint64_t phys_addr_t;
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))
#define PGDIR_SHIFT 30
#define PTRS_PER_PMD 512
#define IOMMU_DOMAIN_IDENTITY 1
#define pmd_val(x) (x)
#define __page_val_to_pfn(x) ((x) >> 10)
#define pte_pfn(x) ((x) >> 10)
#define pfn_to_phys(x) ((phys_addr_t)(x) << PAGE_SHIFT)
#define pfn_to_virt(x) ((void *)(uintptr_t)((x) << PAGE_SHIFT))
#define pmd_leaf(x) ((x) & 0x0e)
#define pmd_none(x) (!(x))
#define pte_present(x) ((x) & 1)
struct riscv_iommu_domain {
    struct { int type; } domain;
    pmd_t *pgd_root;
};
struct io_pgtable_ops { struct riscv_iommu_domain *owner; };
#define io_pgtable_ops_to_domain(ops) ((ops)->owner)

/* PRODUCTION_FUNCTIONS */

static unsigned cases, failures;
static void expect(struct io_pgtable_ops *ops, unsigned long iova,
                   phys_addr_t expected, const char *label)
{
    phys_addr_t actual = riscv_iommu_iova_to_phys(ops, iova);
    cases++;
    if (actual != expected) {
        failures++;
        fprintf(stderr, "%s: iova=%lx actual=%" PRIx64 " expected=%" PRIx64 "\n",
                label, iova, actual, expected);
    }
}

int main(void)
{
    pmd_t *root = aligned_alloc(PAGE_SIZE, PAGE_SIZE);
    pmd_t *middle = aligned_alloc(PAGE_SIZE, PAGE_SIZE);
    pmd_t *leaf = aligned_alloc(PAGE_SIZE, PAGE_SIZE);
    struct riscv_iommu_domain domain = { .pgd_root = root };
    struct io_pgtable_ops ops = { .owner = &domain };
    const unsigned long base = 0x3fffe00000UL;
    const unsigned long offsets[] = { 0, 1, 0x390, 0xfff };
    unsigned i, j;

    if (!root || !middle || !leaf) return 2;
    memset(root, 0, PAGE_SIZE);
    memset(middle, 0, PAGE_SIZE);
    memset(leaf, 0, PAGE_SIZE);
    root[(base >> 30) & 511] = (((uintptr_t)middle >> 12) << 10) | 1;
    middle[(base >> 21) & 511] = (((uintptr_t)leaf >> 12) << 10) | 1;
    /* Exact source/destination physical pages reported by the FPGA diagnostic. */
    const phys_addr_t physical[] = { 0x82d10000, 0x82d49000 };
    for (j = 0; j < 2; j++) {
        leaf[j] = ((physical[j] >> 12) << 10) | 7;
        for (i = 0; i < sizeof(offsets)/sizeof(offsets[0]); i++)
            expect(&ops, base + j*PAGE_SIZE + offsets[i], physical[j] + offsets[i], "4KiB");
    }
    expect(&ops, base + 2*PAGE_SIZE + 0x390, 0, "unmapped leaf");
    expect(&ops, 0x1000, 0, "unmapped root");
    leaf[0] &= ~1UL;
    expect(&ops, base, 0, "invalid leaf");
    middle[(base >> 21) & 511] = ((0x82000000UL >> 12) << 10) | 7;
    expect(&ops, base + 0x12390, 0x82012390, "2MiB interior");
    expect(&ops, base + 0x1fffff, 0x821fffff, "2MiB last byte");
    root[(base >> 30) & 511] = ((0x80000000UL >> 12) << 10) | 7;
    expect(&ops, base + 0x390, 0x80000000UL + ((base + 0x390) & ((1UL << 30)-1)), "1GiB interior");
    expect(&ops, (base | ((1UL << 30)-1)), 0xbfffffff, "1GiB last byte");
    domain.domain.type = IOMMU_DOMAIN_IDENTITY;
    expect(&ops, base + 0x390, base + 0x390, "identity");
    free(leaf); free(middle); free(root);
    printf("%u production software lookup cases, %u failures (no hardware claim)\n", cases, failures);
    return failures ? 1 : 0;
}
