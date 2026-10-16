/* Host model only. Production driver functions are inserted by test_idma_controlled.py. */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

typedef __u64 u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uintptr_t dma_addr_t;
typedef uintptr_t phys_addr_t;
struct device { int index; };
struct page { int unused; };
struct miscdevice { struct device *parent; };
struct mutex { int unused; };
struct iommu_domain { int unused; };
#define BIT_ULL(n) (1ULL << (n))
#define __iomem
#define __user
#define WRITE_ONCE(dst, src) ((dst) = (src))
#define READ_ONCE(src) (*(volatile __typeof__(src) *)&(src))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define dev_crit(dev, ...) ((void)(dev))
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define PAGE_SIZE 4096U
#define offset_in_page(ptr) ((uintptr_t)(ptr) & (PAGE_SIZE - 1))
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL
#define GFP_KERNEL 0
#define kzalloc(n, flags) calloc(1, n)
#define kfree(ptr) free(ptr)

/* PRODUCTION_TYPES */

struct riscv_iommu_hpm_session { int unused; };
struct riscv_iommu_hpm_sample { u64 iotlb_miss[2], s1_ptw[2], s2_ptw[2]; };

struct fake_dma {
    u64 regs[32], latency, due, next_id, done, submits;
    uintptr_t submitted_src[4096];
    bool pending, zero_id, batch, stop;
    u8 *cpu_src, *cpu_dst, *memory_src, *memory_dst;
    size_t buffer_size;
};

static struct fake_dma hw[2];
static struct device devices[2] = {{0}, {1}};
static struct idma_device idmas[2];
static struct mutex idma_experiment_lock;
static struct riscv_iommu_hpm_session session;
static u64 now = 1000, interrupt_at = UINT64_MAX;
static unsigned allocations, frees, unmaps, hpm_releases, hpm_acquires;
static bool idma_observe, idma_path_observe;
static u64 idma_observation_sequence;
static unsigned obs_arms, obs_reports, obs_mmio;
static bool controlled_mode, controlled_probe_miss, controlled_flush_fail;
static bool controlled_abort_hold, controlled_cancel_v, controlled_cancel_a;
static bool controlled_drain_fail, controlled_issue_a, controlled_hold_a;
static bool controlled_no_terminal, controlled_raw_error;
static unsigned controlled_gos, controlled_aborts, controlled_clears;

static void dev_info(struct device *dev, const char *fmt, ...)
{
    (void)dev; (void)fmt; obs_reports++;
}

static struct page *idma_buffer_page(void *ptr)
{
    return (struct page *)((uintptr_t)ptr & ~(uintptr_t)(PAGE_SIZE - 1));
}
static phys_addr_t page_to_phys(struct page *page) { return (uintptr_t)page; }

static void local_flush_icache_all(void)
{
    assert(!hw[0].pending && !hw[1].pending);
}

static phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, unsigned long iova)
{
    (void)domain;
    return iova + PAGE_SIZE;
}

static void dev_err(struct device *dev, const char *format, int id, u32 did,
                    size_t offset, size_t touched_end, size_t size, u32 chunk,
                    unsigned src, unsigned src_after, unsigned expected_src,
                    unsigned dst, unsigned dst_after, unsigned expected_dst,
                    unsigned long src_iova, unsigned long dst_iova,
                    const phys_addr_t *src_pa, const phys_addr_t *dst_pa,
                    const phys_addr_t *src_pte_pa, const phys_addr_t *dst_pte_pa)
{
    (void)dev; (void)format; (void)id; (void)did; (void)offset; (void)touched_end;
    (void)size; (void)chunk; (void)src; (void)src_after; (void)expected_src;
    (void)dst; (void)dst_after; (void)expected_dst; (void)src_iova; (void)dst_iova;
    (void)src_pa; (void)dst_pa; (void)src_pte_pa; (void)dst_pte_pa;
}

static void advance(u64 delta)
{
    int i;

    now += delta;
    for (i = 0; i < 2; i++) {
        struct fake_dma *h = &hw[i];
        if (h->pending && h->due != UINT64_MAX && now >= h->due) {
            u8 *dst = (void *)(uintptr_t)h->regs[IDMA_DST_ADDR_REG_OFFSET / 8];
            u8 *src = (void *)(uintptr_t)h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
            if (controlled_drain_fail && i == 0)
                continue;
            memcpy(dst, src, h->regs[IDMA_NUM_BYTES_REG_OFFSET / 8]);
            h->done = h->next_id;
            h->pending = false;
        }
    }
}

static u64 ktime_get_ns(void) { advance(1000); return now; }
static bool signal_pending(void *unused) { (void)unused; return now >= interrupt_at; }
static void cpu_relax(void) { advance(1000); }
static void cond_resched(void) { advance(1000); }

static struct fake_dma *mmio(void *address, size_t *offset)
{
    int i;
    for (i = 0; i < 2; i++) {
        uintptr_t p = (uintptr_t)address, base = (uintptr_t)hw[i].regs;
        if (p >= base && p < base + sizeof(hw[i].regs)) {
            *offset = p - base;
            return &hw[i];
        }
    }
    assert(!"invalid MMIO address");
    return NULL;
}

static void controlled_launcher_reset(void)
{
    memset(&hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8], 0, 0x50);
    hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8] = 0x49444d414c4e4301ULL;
    controlled_clears++;
}

static void controlled_launcher_prep(u64 value)
{
    hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 1;
    hw[0].regs[IDMA_LAUNCHER_DATA_OFFSET / 8] = 0;
    hw[0].regs[IDMA_LAUNCHER_INDEX_OFFSET / 8] = 0;
    (void)value;
}

static void controlled_launcher_go(void)
{
    controlled_gos++;
    /* V is lane 0, A is lane 1 when requested. */
    if (!controlled_abort_hold) {
        if (!controlled_cancel_v) {
            hw[0].pending = true;
            hw[0].due = now + hw[0].latency;
        }
        if (controlled_issue_a && !controlled_cancel_a) {
            hw[1].pending = true;
            hw[1].due = now + hw[1].latency;
        }
    }
    hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 3;
}

static void controlled_launcher_abort(void)
{
    controlled_aborts++;
    if (controlled_abort_hold && !controlled_no_terminal) {
        hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 5;
    } else if (!controlled_no_terminal) {
        hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 5;
    }
}

static void writeq(u64 value, void *address)
{
    size_t offset;
    struct fake_dma *h = mmio(address, &offset);

    if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
        if (offset == IDMA_LAUNCHER_CONTROL_OFFSET) {
            if (value & 8)
                controlled_launcher_reset();
            else if (value & 1)
                controlled_launcher_prep(value);
            else if (value & 2)
                controlled_launcher_go();
            else if (value & 4)
                controlled_launcher_abort();
        }
        h->regs[offset / 8] = value;
        return;
    }
    if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
        if (offset == IDMA_BATCH_CONTROL_OFFSET && value == 2) {
            h->stop = true;
            return;
        }
        h->regs[offset / 8] = value;
        return;
    }
    if (offset >= IDMA_OBS_MAGIC_OFFSET) {
        obs_mmio++;
        if (offset == IDMA_OBS_CONTROL_OFFSET) {
            if (value == 4)
                value = 0;
            else if (value == 2)
                value = 4;
            else {
                assert(hpm_acquires == hpm_releases + 1 && !hw[0].pending && !hw[1].pending);
                obs_arms++;
                value = 1;
            }
        }
        h->regs[offset / 8] = value;
        return;
    }
    assert(!h->pending);
    h->regs[offset / 8] = value;
}

static u64 readq(void *address)
{
    size_t offset;
    struct fake_dma *h = mmio(address, &offset);

    advance(1000);
    if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
        if (offset == IDMA_LAUNCHER_DATA_OFFSET) {
            unsigned int index = h->regs[IDMA_LAUNCHER_INDEX_OFFSET / 8];
            static u64 words[IDMA_E3_CONTROLLED_WORDS];
            unsigned int i;

            for (i = 0; i < IDMA_E3_CONTROLLED_WORDS; i++)
                words[i] = 0;
            words[0] = 0x49444d414c4e4301ULL;
            words[1] = controlled_raw_error;
            words[2] = 4;
            words[3] = now;
            words[4] = now;
            words[5] = now + 1;
            words[6] = now + 2;
            words[7] = now + 3;
            words[8] = now + 1;
            words[9] = now + 4;
            words[10] = now + 5;
            words[11] = 1;
            words[12] = controlled_issue_a ? 2 : 0;
            words[13] = 1;
            words[14] = controlled_issue_a ? 2 : 0;
            words[15] = controlled_issue_a ? 3 : 1;
            words[16] = controlled_issue_a ? 3 : 1;
            words[17] = (controlled_cancel_v ? 1 : 0) | (controlled_cancel_a ? 2 : 0);
            words[24] = controlled_cancel_v ? 0 : 1;
            words[25] = controlled_issue_a && !controlled_cancel_a ? 1 : 0;
            words[26] = controlled_cancel_v ? 0 : 1;
            words[27] = controlled_issue_a && !controlled_cancel_a ? 1 : 0;
            words[28] = controlled_issue_a ? 1 : 0;
            words[29] = 0;
            words[30] = 0;
            return words[index];
        }
        return h->regs[offset / 8];
    }
    if (offset >= IDMA_BATCH_MAGIC_OFFSET)
        return h->regs[offset / 8];
    if (offset >= IDMA_OBS_MAGIC_OFFSET) {
        obs_mmio++;
        if (offset == IDMA_OBS_CONTROL_OFFSET)
            return h->regs[offset / 8] & 3 ? 4 : 4;
        if (offset == IDMA_OBS_DATA_OFFSET)
            return 0;
        return h->regs[offset / 8];
    }
    if (offset == IDMA_NEXT_ID_REG_OFFSET) {
        assert(!h->pending);
        h->submitted_src[h->submits++] = h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
        if (h == &hw[0] && (hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] & 1) &&
            !(hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] & 2)) {
            h->next_id++;
            return h->zero_id ? 0 : h->next_id;
        }
        h->pending = true;
        h->next_id++;
        h->due = h->zero_id || h->latency == UINT64_MAX ? UINT64_MAX : now + h->latency;
        return h->zero_id ? 0 : h->next_id;
    }
    if (offset == IDMA_DONE_REG_OFFSET)
        return h->done;
    if (offset == IDMA_ACTIVE_CYCLES_REG_OFFSET)
        return now;
    return h->regs[offset / 8];
}

#define readq_poll_timeout(address, value, condition, sleep_us, timeout_us) ({ \
    u64 limit = now + (u64)(timeout_us) * NSEC_PER_USEC; \
    int result = -ETIMEDOUT; \
    (void)(sleep_us); \
    do { \
        (value) = readq(address); \
        if (condition) { result = 0; break; } \
        advance(NSEC_PER_MSEC); \
    } while (now < limit); \
    result; \
})

static int copy_from_user(void *dst, const void *src, size_t n) { memcpy(dst, src, n); return 0; }
static int copy_to_user(void *dst, const void *src, size_t n) { memcpy(dst, src, n); return 0; }
static int mutex_lock_interruptible(struct mutex *m) { (void)m; return 0; }
static void mutex_unlock(struct mutex *m) { (void)m; }
static const void *memchr_inv(const void *ptr, int c, size_t n)
{
    const unsigned char *p = ptr;
    while (n--) {
        if (*p != (unsigned char)c)
            return p;
        p++;
    }
    return NULL;
}
static bool idma_any_quarantined(void)
{
    return idmas[0].quarantined || idmas[1].quarantined;
}
static struct idma_device *idma_get_instance(u32 id)
{
    return id < 2 ? &idmas[id] : NULL;
}
static void idma_put_instance(struct idma_device *d) { (void)d; }
static void idma_iommu_unmap_range(struct iommu_domain *d, unsigned long iova,
                                   size_t size, size_t page)
{
    (void)d; (void)iova; (void)size; (void)page;
    unmaps++;
}
static void dma_free_attrs(struct device *d, size_t n, void *ptr, dma_addr_t dma,
                           unsigned attrs)
{
    (void)n; (void)dma; (void)attrs;
    assert(!hw[d->index].pending);
    frees++;
    free(ptr);
}

static u64 idma_pattern(struct idma_stream *s, size_t word);
static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
                               u32 pages, u32 chunk,
                               struct idma_experiment_stats *stats)
{
    size_t i;
    s->idma = idma;
    s->dev = idma->miscdev.parent;
    s->stats = stats;
    s->size = (size_t)pages * PAGE_SIZE;
    s->chunk = chunk;
    s->src = malloc(s->size);
    s->dst = malloc(s->size);
    assert(s->src && s->dst);
    allocations += 2;
    s->src_iova = (uintptr_t)s->src;
    s->dst_iova = (uintptr_t)s->dst;
    s->mapped_src = s->mapped_dst = true;
    for (i = 0; i < s->size / sizeof(u64); i++) {
        ((u64 *)s->src)[i] = idma_pattern(s, i);
        ((u64 *)s->dst)[i] = ~idma_pattern(s, i);
    }
    return 0;
}

static int riscv_iommu_hpm_acquire(struct device *v, struct device *a,
                                   struct riscv_iommu_hpm_session **s)
{
    (void)v; (void)a;
    hpm_acquires++;
    *s = &session;
    return 0;
}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *s,
                                 struct riscv_iommu_hpm_sample *sample)
{
    (void)s;
    memset(sample, 0, sizeof(*sample));
    advance(1000);
}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *s)
{
    assert(s == &session);
    hpm_releases++;
}
static int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *s)
{
    assert(s == &session);
    assert(hpm_acquires == hpm_releases + 1);
    assert(!hw[0].pending && !hw[1].pending);
    return controlled_flush_fail ? -EIO : 0;
}

/* PRODUCTION_FUNCTIONS */

static struct idma_experiment controlled_input(void)
{
    struct idma_experiment e = {
        .abi_version = IDMA_EXPERIMENT_ABI,
        .struct_size = sizeof(e),
        .aggressor_id = 1,
        .victim_pages = 1,
        .aggressor_pages = 1,
        .chunk_bytes = 64,
        .run_timeout_ms = 100,
        .victim_transfers = 1,
    };
    int i;
    for (i = 0; i < 2; i++) {
        idmas[i].id = i;
        idmas[i].did = 10 + i;
        idmas[i].reg = hw[i].regs;
        idmas[i].miscdev.parent = &devices[i];
        hw[i].latency = 20000;
    }
    return e;
}

int main(int argc, char **argv)
{
    struct idma_e3_controlled c;
    struct idma_experiment e;
    const char *name;
    long ret;

    assert(argc == 2);
    name = argv[1];
    e = controlled_input();
    hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC_V6;
    hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8] = 0x49444d414c4e4301ULL;

    memset(&c, 0, sizeof(c));
    c.pair.abi_version = 1;
    c.pair.struct_size = sizeof(c.pair);
    c.pair.request = e;

    if (!strcmp(name, "invalid_before_mmio")) {
        c.pair.abi_version = 0;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EINVAL);
        assert(!allocations && !obs_mmio && !controlled_gos);
        return 0;
    }
    if (!strcmp(name, "cold_solo")) {
        c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
        c.prep_mode = 0;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == 0 || ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "hot_pair")) {
        c.prep_mode = 1;
        c.pair_mode = 1;
        controlled_mode = true;
        controlled_issue_a = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == 0 || ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "probe_miss_no_go")) {
        c.prep_mode = 0;
        controlled_probe_miss = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EIO);
        assert(!controlled_gos);
        return 0;
    }
    if (!strcmp(name, "flush_failure")) {
        controlled_flush_fail = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "cancel_v_no_bytes")) {
        controlled_cancel_v = true;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == 0 || ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "abort_partial_hold")) {
        controlled_abort_hold = true;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret != 0);
        return 0;
    }
    if (!strcmp(name, "raw_failure_evidence")) {
        controlled_raw_error = true;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EIO);
        return 0;
    }
    assert(!"unknown test");
    return 1;
}
