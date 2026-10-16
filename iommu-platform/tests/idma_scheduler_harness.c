/* Host model only. Production driver functions are inserted by test_idma_scheduler.py. */
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
#define NSEC_PER_SEC 1000000000ULL
#define PAGE_SIZE 4096U
#define offset_in_page(ptr) ((uintptr_t)(ptr) & (PAGE_SIZE - 1))
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL
#define GFP_KERNEL 0
#define kzalloc(n, flags) calloc(1, n)
#define kfree(ptr) free(ptr)
#define local_irq_save(flags) do { (flags) = 0; } while (0)
#define local_irq_restore(flags) do { (void)(flags); } while (0)
#define preempt_disable() do { } while (0)
#define preempt_enable() do { } while (0)

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
static bool queue_mode, queue_bank_bad, queue_gate_bad, queue_insert_bad, queue_truncated;
static bool diag_mode, pair_mode, pair_bad, pair_second_bad, pair_trigger, pair_delay;
static bool wrong_source;
static bool idma_observe, obs_bad_sum, obs_wrong_count, obs_stuck, obs_arm_error, obs_bad_axi;
static bool idma_path_observe, path_bad, path_carry, path_stock, path_count;
static bool p4m_mode, p4m_bad_bank, p4m_bad_flags, p4m_bad_conservation;
static bool p4m_bad_counts, p4m_bad_status, p4m_bad_readback, p4m_cap_missing;
static bool p4m_sticky_error, p4m_gap_written;
static u64 p4m_window[16];
static u64 idma_observation_sequence;
static unsigned obs_arms, obs_reports, obs_mmio;
static void dev_info(struct device *dev, const char *fmt, ...) { (void)dev; if (strstr(fmt, "idma_batch_start")) return; assert(hpm_releases && hpm_releases == hpm_acquires); obs_reports++; }
static bool inject_verify_error, corrupt_source, restore_on_flush, diagnostic_mutated;
static unsigned diagnostic_flushes;
static unsigned prepare_flushes, finish_flushes;
static bool cache_shadow, corrupt_backing;
static u8 *diagnostic_mutation_target;
static struct {
    unsigned count;
    int id;
    u32 did, chunk;
    size_t offset, touched_end, size;
    unsigned src, src_after, expected_src, dst, dst_after, expected_dst;
    unsigned long src_iova, dst_iova;
    phys_addr_t src_pa, dst_pa, src_pte_pa, dst_pte_pa;
} diagnostic;

static struct page *idma_buffer_page(void *ptr)
{
    return (struct page *)((uintptr_t)ptr & ~(uintptr_t)(PAGE_SIZE - 1));
}
static phys_addr_t page_to_phys(struct page *page) { return (uintptr_t)page; }
static void local_flush_icache_all(void)
{
    int i;
    assert(!hw[0].pending && !hw[1].pending);
    if (pair_mode) {
        if (hpm_acquires > hpm_releases) prepare_flushes++;
        else finish_flushes++;
        return;
    }
    if (!hpm_releases) {
        assert(!hw[0].submits && !hw[1].submits);
        prepare_flushes++;
        if (cache_shadow) {
            for (i = 0; i < 2; i++) {
                struct fake_dma *h = &hw[i];
                if (!h->cpu_src) continue;
                memcpy(h->memory_src, h->cpu_src, h->buffer_size);
                memcpy(h->memory_dst, h->cpu_dst, h->buffer_size);
            }
        }
        return;
    }
    assert(hpm_releases == 1); /* Outside measurement, after BOTH lanes drain. */
    if (++finish_flushes == 1) {
        if (cache_shadow) {
            for (i = 0; i < 2; i++) {
                struct fake_dma *h = &hw[i];
                if (!h->cpu_src) continue;
                memcpy(h->cpu_src, h->memory_src, h->buffer_size);
                memcpy(h->cpu_dst, h->memory_dst, h->buffer_size);
            }
        }
        return;
    }
    diagnostic_flushes++;
    if (restore_on_flush) {
        assert(diagnostic_mutation_target);
        *diagnostic_mutation_target ^= 0x40;
    }
}
static phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, unsigned long iova)
{
    (void)domain;
    /* Change the failed byte after verify captured it, before dev_err runs. */
    if (diagnostic_mutation_target) {
        *diagnostic_mutation_target ^= 0xff;
        diagnostic_mutation_target = NULL;
        diagnostic_mutated = true;
    }
    return iova + PAGE_SIZE; /* Deliberately differ from the buffer PA. */
}
static void dev_err(struct device *dev, const char *format, int id, u32 did,
                    size_t offset, size_t touched_end, size_t size, u32 chunk,
                    unsigned src, unsigned src_after, unsigned expected_src,
                    unsigned dst, unsigned dst_after, unsigned expected_dst,
                    unsigned long src_iova, unsigned long dst_iova,
                    const phys_addr_t *src_pa, const phys_addr_t *dst_pa,
                    const phys_addr_t *src_pte_pa, const phys_addr_t *dst_pte_pa)
{
    assert(dev->index == id);
    assert(strstr(format, "experiment data mismatch:"));
    diagnostic.count++;
    diagnostic.id = id; diagnostic.did = did;
    diagnostic.offset = offset; diagnostic.touched_end = touched_end;
    diagnostic.size = size; diagnostic.chunk = chunk;
    diagnostic.src = src; diagnostic.expected_src = expected_src;
    diagnostic.src_after = src_after; diagnostic.dst_after = dst_after;
    diagnostic.dst = dst; diagnostic.expected_dst = expected_dst;
    diagnostic.src_iova = src_iova; diagnostic.dst_iova = dst_iova;
    diagnostic.src_pa = *src_pa; diagnostic.dst_pa = *dst_pa;
    diagnostic.src_pte_pa = *src_pte_pa; diagnostic.dst_pte_pa = *dst_pte_pa;
}

static void advance(u64 delta)
{
    int i;
    now += delta;
    for (i = 0; i < 2; i++) {
        struct fake_dma *h = &hw[i];
        if (h->pending && h->due != UINT64_MAX && now >= h->due) {
            struct fake_dma *source = wrong_source ? &hw[1 - i] : h;
            u8 *dst = (void *)(uintptr_t)h->regs[IDMA_DST_ADDR_REG_OFFSET / 8];
            u8 *src = (void *)(uintptr_t)source->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
            if (h->batch) {
                size_t off = (h->regs[IDMA_BATCH_COMPLETED_OFFSET / 8] %
                              h->regs[IDMA_BATCH_PAGES_OFFSET / 8]) * PAGE_SIZE;
                src += off; dst += off;
            }
            if (cache_shadow) {
                dst = h->memory_dst + (dst - h->cpu_dst);
                src = source->memory_src + (src - source->cpu_src);
            }
            memcpy(dst, src, h->regs[IDMA_NUM_BYTES_REG_OFFSET / 8]);
            if (corrupt_backing) dst[17] ^= 0x40;
            if (inject_verify_error) {
                size_t reg = corrupt_source ? IDMA_SRC_ADDR_REG_OFFSET : IDMA_DST_ADDR_REG_OFFSET;
                u8 *buffer = (u8 *)(uintptr_t)h->regs[reg / 8];

                buffer[17] ^= 0x40;
                buffer[18] ^= 0x80;
                diagnostic_mutation_target = &buffer[17];
            }
            if (h->batch) {
                h->regs[IDMA_BATCH_COMPLETED_OFFSET / 8]++;
                if (!h->stop && h->regs[IDMA_BATCH_COMPLETED_OFFSET / 8] < h->regs[IDMA_BATCH_TARGET_OFFSET / 8]) {
                    h->regs[IDMA_BATCH_ISSUED_OFFSET / 8]++;
                    h->due = now + h->latency;
                    continue;
                }
                h->regs[IDMA_BATCH_CONTROL_OFFSET / 8] = h->stop ? 5 : 9;
            }
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

static void writeq(u64 value, void *address)
{
    size_t offset;
    struct fake_dma *h = mmio(address, &offset);
    if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
        if (offset == IDMA_BATCH_CONTROL_OFFSET && value == 2) {
            h->stop = true;
            return;
        }
        assert(!h->pending);
        if (offset == IDMA_BATCH_GAP_OFFSET)
            p4m_gap_written = true;
        h->regs[offset / 8] = value;
        return;
    }
    if (offset >= IDMA_OBS_MAGIC_OFFSET) {
        obs_mmio++;
        if (offset == IDMA_OBS_CONTROL_OFFSET) {
            if (value == 4) value = 0;
            else if (value == 2) value = 4 | (1ULL << 36);
            else { assert(hpm_acquires == hpm_releases + 1 && !hw[(value >> 8) & 3].pending); obs_arms++; pair_delay = !!(value & (1ULL << 28)); value = obs_arm_error ? (1ULL << 32) | 1 : 1; }
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
    if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
		if (offset == IDMA_BATCH_GAP_MAGIC_OFFSET && p4m_cap_missing)
			return 0;
		if (offset == IDMA_BATCH_GAP_OFFSET && p4m_bad_readback)
			return h->regs[offset / 8] + 1;
		if (offset == IDMA_BATCH_CONTROL_OFFSET && p4m_sticky_error &&
		    p4m_gap_written)
			return h->regs[offset / 8] | (1ULL << 32);
		return h->regs[offset / 8];
	}
    if (offset >= IDMA_OBS_MAGIC_OFFSET) {
        obs_mmio++;
        if (offset == IDMA_OBS_CONTROL_OFFSET && hpm_releases && hpm_acquires == hpm_releases &&
            (h->regs[offset / 8] & 3) && !obs_stuck && !obs_arm_error)
            h->regs[offset / 8] = 4;
        if (offset == IDMA_OBS_DATA_OFFSET) {
            unsigned index = h->regs[IDMA_OBS_INDEX_OFFSET / 8];
            assert(hpm_releases && hpm_acquires == hpm_releases); /* After drain. */
            if (p4m_mode && index >= IDMA_P4M_BANK_BASE) {
                unsigned k = index - IDMA_P4M_BANK_BASE;
                if (k < IDMA_P4M_WORDS)
                    return p4m_window[k];
                return 0;
            }
            if (pair_mode && index >= 192) {
                unsigned k = index - 192;
                if (k == 0) return queue_mode && !queue_bank_bad ? IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
                if (queue_mode && pair_trigger) {
                    if (k == 25) return 4;
                    if (k == 26) return queue_gate_bad ? 4 : 3;
                    if (k == 27) return 2;
                    if (k == 28 || k == 29) return 1;
                    if (k == 30) return 4;
                    if (k == 12) return queue_truncated ? 3 : 2;
                    if (k == 62) return queue_truncated;
                    if (k == 63) return 2;
                    if (k >= 46 && k < 62) {
                        unsigned slot=(k-46)/8, field=(k-46)%8;
                        const u64 entries[2][8]={{2,4,11,4096,1,145,153,1},{3,4,11,8192,1,146,155,0}};
                        if (queue_insert_bad && field==2) return 10;
                        return entries[slot][field];
                    }
                }
                if (k == 1) return pair_bad || (pair_second_bad && obs_arms == 2);
                if (k == 2) return pair_trigger;
                if (k == 3) return pair_delay ? 128 : 0;
                if (k == 17 || k == 18) return 128;
                if (k >= 96 && k < 160) {
                    unsigned field = k % 8;
                    if (field < 2) return 128;
                    if (field == 3 || field == 7) return 256;
                    if (field == 4) return 2;
                }
                if (k >= 224 && k <= 226) return 128;
                if (k == 233) return 256;
                if (pair_trigger) {
                    if (k == 4) return 20;
                    if (k == 5 || k == 6) return 21 + (pair_delay ? 128 : 0);
                    if (k == 7) return pair_delay ? 128 : 0;
                    if (k == 13) return 3;
                    if (k == 14) return 2;
                    if (k == 15) return 1;
                    if (k >= 32 && k < 96) {
                        unsigned shot = k / 32 - 1, f = k % 32;
                        if (f == 0) return 1;
                        if (f == 1) return shot ? 0 : 4;
                        if (f == 2) return shot ? 10 : 11;
                        if (f == 4) return 127;
                        if (f == 12) return shot;
                        if (f >= 5 && f <= 11) {
                            const u64 times[2][7]={{1,2,3,4,150,151,152},{5,160,161,162,163,164,165}};
                            return queue_mode ? times[shot][f-5] : f;
                        }
                    }
                }
                return 0;
            }
            if (idma_path_observe && index >= 64) {
                unsigned row = (index - 64) / 8, field = (index - 64) % 8;
                if (row < 8) {
                    if (field < 2) return 128 + (path_count && row == 0);
                    if (field == 3 || field == 7) return 256 + (path_bad && field == 7);
                    if (field == 4) return 2;
                    return 0;
                }
                /* A carried into stage 0, crosses all stages, another A enters
                 * stage 0 and remains there at window end. */
                if (path_carry && row == 8) {
                    if (field == 0 || field == 2 || field == 5 || field == 6) return 1;
                    if (field == 7) return 9;
                } else if (path_carry && row > 8 && row < 12) {
                    if (field < 2) return 1;
                    if (field == 3 || field == 4 || field == 7) return 2;
                }
                if (path_stock && row == 8 && field == 6) return 1;
                return 0;
            }
            if (idma_path_observe && index >= 14 && index <= 17) return 128;
            if (index == 14 && obs_bad_axi) return 1;
            if (index == 0 || index == 10) return 30;
            if (index == 1) return obs_bad_sum ? 11 : 10;
            if (hw[0].regs[IDMA_OBS_CONTROL_OFFSET / 8] & (1ULL << 25)) assert(!"mode must be tracked separately");
            bool batch = hw[0].batch || hw[1].batch;
            if (index == 2) return batch ? 0 : 10;
            if (index == 3) return batch ? 20 : 10;
            if (index == 9 && batch) return 1;
            if (index == 34 && batch) return 5;
            if (index >= 7 && index <= 9)
                return h->regs[IDMA_OBS_TARGET_OFFSET / 8] + obs_wrong_count;
            return 0;
        }
        if (offset == IDMA_OBS_CONTROL_OFFSET && p4m_mode && p4m_bad_status &&
            hpm_releases && hpm_acquires == hpm_releases)
            return h->regs[offset / 8] | BIT_ULL(40);
        return h->regs[offset / 8];
    }
    if (offset == IDMA_NEXT_ID_REG_OFFSET) {
        assert(!h->pending);
        assert(h->submits < 4096);
        h->submitted_src[h->submits++] = h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
        h->pending = true;
        h->batch = h->regs[IDMA_BATCH_CONTROL_OFFSET / 8] == 1;
        if (h->batch) {
            h->stop = false;
            h->regs[IDMA_BATCH_COMPLETED_OFFSET / 8] = 0;
            h->regs[IDMA_BATCH_ISSUED_OFFSET / 8] = 1;
            h->regs[IDMA_BATCH_CONTROL_OFFSET / 8] = 3;
        }
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

/* Deterministic timeout model; kernel iopoll/MMIO timing is not under test. */
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
    while (n--) { if (*p != (unsigned char)c) return p; p++; }
    return NULL;
}
static bool idma_any_quarantined(void) { return idmas[0].quarantined || idmas[1].quarantined; }
static struct idma_device *idma_get_instance(u32 id) { return id < 2 ? &idmas[id] : NULL; }
static void idma_put_instance(struct idma_device *d) { (void)d; }
static void idma_iommu_unmap_range(struct iommu_domain *d, unsigned long iova, size_t size, size_t page)
{
    (void)d; (void)iova; (void)size; (void)page;
    unmaps++;
}
static void dma_free_attrs(struct device *d, size_t n, void *ptr, dma_addr_t dma, unsigned attrs)
{
    (void)n; (void)dma; (void)attrs;
    assert(!hw[d->index].pending); /* Catch a real production cleanup safety regression. */
    frees++;
    if (cache_shadow && ptr == hw[d->index].cpu_dst) {
        free(hw[d->index].memory_src);
        free(hw[d->index].memory_dst);
    }
    free(ptr);
}

static u64 idma_pattern(struct idma_stream *s, size_t word);
static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
                               u32 pages, u32 chunk, struct idma_experiment_stats *stats)
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
    if (cache_shadow) {
        struct fake_dma *h = &hw[idma->id];
        h->cpu_src = s->src; h->cpu_dst = s->dst; h->buffer_size = s->size;
        h->memory_src = calloc(1, s->size); h->memory_dst = calloc(1, s->size);
        assert(h->memory_src && h->memory_dst);
    }
    return 0;
}
static int riscv_iommu_hpm_acquire(struct device *v, struct device *a,
                                  struct riscv_iommu_hpm_session **s)
{
    (void)v; (void)a; hpm_acquires++; *s = &session; return 0;
}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *s,
                               struct riscv_iommu_hpm_sample *sample)
{
    (void)s; memset(sample, 0, sizeof(*sample)); advance(1000);
    if (diag_mode) for (int i=0;i<2;i++) {
        sample->iotlb_miss[i]=hw[i].submits*3;
        sample->s1_ptw[i]=hw[i].submits*5;
        sample->s2_ptw[i]=hw[i].submits*7;
    }
}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *s)
{
    assert(s == &session); hpm_releases++;
}

/* PRODUCTION_FUNCTIONS */

static struct idma_experiment input(void)
{
    struct idma_experiment e = {
        .abi_version = IDMA_EXPERIMENT_ABI, .struct_size = sizeof(e),
        .aggressor_id = 1, .victim_pages = 4, .aggressor_pages = 2,
        .chunk_bytes = 4096, .run_timeout_ms = 1, .victim_transfers = 3,
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

static void assert_all_freed(void)
{
    assert(allocations == 4 && frees == 4 && unmaps == 4);
    assert(!hw[0].pending && !hw[1].pending);
    assert(!idma_any_quarantined());
    assert(hpm_releases == 1);
}

int main(int argc, char **argv)
{
    struct idma_experiment e = input();
    const char *name;
    long ret;
    assert(argc == 2 || argc == 5);
    name = argv[1];
    if (!strncmp(name, "queue_", 6)) {
        struct idma_e3_diag d = {.pair={.abi_version=1,.struct_size=sizeof(d.pair)}};
        bool old=!strcmp(name,"queue_old_hw"),solo=!strcmp(name,"queue_solo");
        bool legacy=!strcmp(name,"queue_v5_legacy"),diag=!strcmp(name,"queue_v5_diag");
        queue_mode=true;diag_mode=!legacy;pair_mode=idma_path_observe=true;
        queue_bank_bad=!strcmp(name,"queue_bank_bad");queue_gate_bad=!strcmp(name,"queue_gate_bad");
        queue_insert_bad=!strcmp(name,"queue_insert_bad");queue_truncated=!strcmp(name,"queue_truncated");
        pair_second_bad=!strcmp(name,"queue_second_bad");
        pair_trigger=!solo && strcmp(name,"queue_no_trigger");
        d.pair.first_delay=!strcmp(name,"queue_reverse")?128:0;
        e.victim_pages=1;e.aggressor_pages=8;e.victim_transfers=64;e.warmup_transfers=2;e.run_timeout_ms=100;
        if(solo)e.aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR;
        d.pair.request=e;
        hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]=old?IDMA_OBS_MAGIC_V4:IDMA_OBS_MAGIC_V5;
        for(int n=0;n<2;n++)hw[n].regs[IDMA_BATCH_MAGIC_OFFSET/8]=IDMA_BATCH_MAGIC;
        ret=idma_run_e3_pair(&idmas[0],&d,legacy?0:diag?1:2);
        assert(ret==(old?-EOPNOTSUPP:(queue_bank_bad||queue_gate_bad||queue_insert_bad||pair_second_bad)?-EIO:0));
        assert(d.pair.status==ret);
        if(old)assert(!allocations&&!d.pair.rounds_attempted&&!obs_arms);
        else {
            assert(allocations==(solo?2U:4U)&&allocations==frees);
            unsigned rounds=ret&&!pair_second_bad?1:2;
            assert(d.pair.rounds_attempted==rounds&&d.pair.rounds_completed==rounds-!!ret);
            for(unsigned n=0;n<rounds;n++) {
                unsigned delay=diag?0:n?128-d.pair.first_delay:d.pair.first_delay;
                assert(d.pair.observation[n][3]==delay);
                assert(d.pair.result[n].victim.completed==64);
                if(!legacy)assert(d.diagnostic[n].warm_complete&&d.diagnostic[n].lane[0].warm_completed==2);
            }
            if(!ret&&pair_trigger)assert(d.pair.observation[1][62]==queue_truncated&&d.pair.observation[1][63]==2);
        }
        printf("PASS %s (v5 version gate, paired diagnostics and extended records)\n",name);
        return 0;
    } else if (!strncmp(name, "diag_", 5)) {
        struct idma_e3_diag d;
        memset(&d, 0xa5, sizeof(d)); /* Kernel must clear untrusted output. */
        d.pair = (struct idma_e3_pair){.abi_version=1, .struct_size=sizeof(d.pair)};
        bool solo = !strcmp(name, "diag_solo"), zero = !strcmp(name, "diag_no_warmup");
        bool invalid = !strcmp(name, "diag_invalid"), reserved = !strcmp(name, "diag_reserved");
        bool timeout = !strcmp(name, "diag_warm_timeout"), bad = !strcmp(name, "diag_second_bad");
        bool noid = !strcmp(name, "diag_zero_id"), interrupt = !strcmp(name, "diag_interrupt");
        pair_mode = idma_path_observe = true;
        diag_mode = true;
        pair_trigger = !strcmp(name, "diag_trigger"); pair_second_bad = bad;
        e.victim_pages=1; e.aggressor_pages=8; e.victim_transfers=64;
        e.warmup_transfers=zero?0:7; e.run_timeout_ms=timeout?1:100;
        if (solo) e.aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR;
        if (timeout) hw[0].latency=UINT64_MAX;
        if (noid) hw[1].zero_id=true;
        if (interrupt) interrupt_at=0;
        if (!strcmp(name, "diag_fast_a")) hw[1].latency=1000;
        d.pair.request=e; d.pair.first_delay=invalid?128:0; d.pair.reserved=reserved;
        hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC_V4;
        for(int i=0;i<2;i++) hw[i].regs[IDMA_BATCH_MAGIC_OFFSET/8]=IDMA_BATCH_MAGIC;
        ret=idma_run_e3_pair(&idmas[0], &d, true);
        assert(ret == ((invalid||reserved)?-EINVAL:timeout?-ETIMEDOUT:(bad||noid)?-EIO:interrupt?-EINTR:0));
        assert(d.pair.status==ret);
        if (invalid||reserved) {
            assert(!allocations && !d.pair.rounds_attempted);
            for(unsigned i=0;i<sizeof(d.diagnostic);i++) assert(!((unsigned char*)d.diagnostic)[i]);
        } else if (ret && !bad) {
            assert(d.pair.rounds_attempted==1 && !d.pair.rounds_completed);
            assert(!d.diagnostic[0].warm_complete && !d.diagnostic[0].arm_begin_ns);
            assert(!d.diagnostic[1].warm_begin_ns);
            if (timeout||noid) assert(allocations>frees && idma_any_quarantined());
            else assert(allocations==frees);
        } else {
            assert(d.pair.rounds_attempted==2 && d.pair.rounds_completed==(bad?1U:2U));
            assert(allocations==(solo?2U:4U) && allocations==frees);
            for(int r=0;r<2;r++) {
                struct idma_e3_diag_round *g=&d.diagnostic[r];
                assert(d.pair.observation[r][3]==0 && g->warm_complete);
                assert(g->warm_begin_ns <= g->warm_end_ns && g->warm_end_ns <= g->warm_drained_ns);
                assert(g->warm_drained_ns < g->arm_begin_ns && g->arm_begin_ns < g->arm_end_ns);
                assert(g->lane[0].warm_submitted==e.warmup_transfers);
                assert(g->lane[0].warm_completed==e.warmup_transfers && !g->lane[0].warm_drained);
                for(int n=0;n<(solo?1:2);n++) {
                    struct idma_e3_diag_lane *l=&g->lane[n];
                    assert(l->warm_hpm.iotlb_miss==l->warm_submitted*3);
                    assert(l->warm_hpm.s1_ptw==l->warm_submitted*5);
                    assert(l->warm_hpm.s2_ptw==l->warm_submitted*7);
                    assert(l->warm_submitted==l->warm_completed+l->warm_drained);
                    assert(l->warm_drained==l->warm_in_flight);
                    assert(l->launch_begin_ns>g->arm_end_ns && l->launch_end_ns>l->launch_begin_ns);
                    assert(l->warm_last_offset<(n?8:1)*PAGE_SIZE);
                    assert(l->warm_next_offset<(n?8:1)*PAGE_SIZE);
                    if (zero) assert(!l->warm_submitted && !l->warm_next_offset);
                }
                if (!solo) {
                    /*
                     * launch_begin_ns still records the configuration-write
                     * begin instant (the field name is kept for ABI
                     * compatibility), so the A and V launch brackets overlap:
                     * both register sets are configured before either NEXT_ID
                     * trigger read.  The stamped order is A-config-begin <
                     * V-config-begin < A-trigger-end < V-trigger-end, so check
                     * that ordering instead of the old per-lane sequencing.
                     */
                    assert(g->lane[1].launch_begin_ns < g->lane[1].launch_end_ns);
                    assert(g->lane[0].launch_begin_ns < g->lane[1].launch_end_ns);
                    assert(g->lane[1].launch_end_ns < g->lane[0].launch_end_ns);
                } else
                    assert(!g->lane[1].warm_submitted && !g->lane[1].launch_begin_ns);
                if(r) assert(g->warm_begin_ns>d.diagnostic[0].lane[0].launch_end_ns);
                assert(d.pair.result[r].victim.completed==64);
            }
            assert(hw[0].submits==2*(e.warmup_transfers+1));
            assert(hw[1].submits==d.diagnostic[0].lane[1].warm_submitted+
                   d.diagnostic[1].lane[1].warm_submitted+(solo?0:2));
        }
        printf("PASS %s (D0/D0, per-round history and failure cleanup)\n",name);
        return 0;
    } else if (!strncmp(name, "pair_", 5)) {
        struct idma_e3_pair p = { .abi_version = 1, .struct_size = sizeof(p) };
        bool missing = !strcmp(name, "pair_missing"), invalid = !strcmp(name, "pair_invalid");
        bool solo = !strcmp(name, "pair_solo"), timeout = !strcmp(name, "pair_timeout");
        pair_mode = idma_path_observe = true;
        pair_bad = !strcmp(name, "pair_bad"); pair_second_bad = !strcmp(name, "pair_second_bad");
        pair_trigger = !strcmp(name, "pair_trigger") || !strcmp(name, "pair_reverse");
        p.first_delay = !strcmp(name, "pair_reverse") ? 128 : invalid ? 1 : 0;
        e.victim_pages = 1; e.aggressor_pages = 8; e.victim_transfers = 64;
        e.warmup_transfers = timeout ? 0 : 2; e.run_timeout_ms = timeout ? 1 : 100;
        if (solo) e.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
        if (timeout) hw[0].latency = UINT64_MAX;
        p.request = e;
        hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = missing ? IDMA_OBS_MAGIC_V3 : IDMA_OBS_MAGIC_V4;
        for (int i = 0; i < 2; i++) hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8] = IDMA_BATCH_MAGIC;
        ret = idma_run_e3_pair(&idmas[0], &p, false);
        assert(ret == (missing ? -EOPNOTSUPP : invalid ? -EINVAL : timeout ? -ETIMEDOUT : (pair_bad || pair_second_bad) ? -EIO : 0));
        assert(p.status == ret);
        if (missing || invalid) assert(!allocations && !p.rounds_attempted);
        else {
            unsigned rounds = (pair_bad || timeout) ? 1 : 2;
            assert(p.rounds_attempted == rounds && p.rounds_completed == rounds - !!ret);
            assert(allocations == (solo ? 2U : 4U));
            assert(p.mapping[0] == hw[0].submitted_src[0]);
            assert(p.sequence[0] && (rounds == 1 || p.sequence[1] > p.sequence[0]));
            assert(p.observation[0][3] == p.first_delay);
            if (rounds == 2) assert(p.observation[1][3] == 128 - p.first_delay);
            if (timeout) assert(allocations > frees && p.result[0].victim.quarantined);
            else assert(allocations == frees && !hw[0].pending && !hw[1].pending);
            if (!ret) {
                assert(prepare_flushes == 2 && finish_flushes == 2);
                assert(p.result[0].victim.completed == 64 && p.result[1].victim.completed == 64);
            }
        }
        printf("PASS %s (paired mapping lifetime and failure cleanup)\n", name);
        return 0;
    } else if (!strncmp(name, "path_", 5)) {
        idma_path_observe = true;
        path_bad = !strcmp(name, "path_bad_area");
        path_stock = !strcmp(name, "path_bad_stock");
        path_count = !strcmp(name, "path_bad_count");
        path_carry = !strcmp(name, "path_carry");
        bool missing = !strcmp(name, "path_missing_v3");
        e.victim_pages=1; e.aggressor_pages=8; e.victim_transfers=64;
        e.warmup_transfers=2; e.run_timeout_ms=100;
        hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = missing ? IDMA_OBS_MAGIC_V2 : IDMA_OBS_MAGIC_V3;
        for (int i=0; i<2; i++) hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8]=IDMA_BATCH_MAGIC;
        ret=idma_run_experiment_mode(&idmas[0], &e, true);
        assert(ret == (missing ? -EOPNOTSUPP : (path_bad || path_stock || path_count ? -EIO : 0)));
        assert(obs_reports == (missing ? 0 : 19));
        assert(allocations == frees && !hw[0].pending && !hw[1].pending);
        if (missing) assert(!allocations && !obs_arms);
    } else if (!strcmp(name, "batch_v_zero_drain_a") ||
               !strcmp(name, "batch_v_zero_drain_a_timeout")) {
        struct idma_stream sv = {}, sa = {};
        struct idma_batch_start st = {};
        bool drain_timeout = !strcmp(name, "batch_v_zero_drain_a_timeout");
        u64 end = 0;
        int r;

        /*
         * Production batch failure contract: the V trigger returns no usable
         * ID after the A trigger already produced one.  A must stay
         * stop/drain-able while V is retained as unknown.  STOP, DONE and the
         * stopped batch status come from the fake DMA model.
         */
        e.victim_pages = 1;
        e.aggressor_pages = 1;
        e.victim_transfers = 1;
        e.chunk_bytes = PAGE_SIZE;
        hw[0].latency = 20000;
        hw[1].latency = drain_timeout ? UINT64_MAX : 20000;
        for (int i = 0; i < 2; i++)
            hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8] = IDMA_BATCH_MAGIC;
        assert(idma_stream_prepare(&sv, &idmas[0], 1, PAGE_SIZE, &e.victim) == 0);
        assert(idma_stream_prepare(&sa, &idmas[1], 1, PAGE_SIZE, &e.aggressor) == 0);
        assert(idma_batch_configure(&sv, 1) == 0);
        assert(idma_batch_configure(&sa, IDMA_EXPERIMENT_MAX_TRANSFERS) == 0);
        hw[0].zero_id = true;
        st.v_did = 10;
        st.a_did = 11;
        st.has_a = true;
        r = idma_batch_start(&sv, &sa, now + 1000000000, &end, &st);
        assert(r == -EIO);
        assert(st.attempted && st.a_triggered && st.v_triggered);
        assert(st.a_id && !st.v_id);
        /* Valid A is published before V is retained, so the caller drain of A
         * remains safe. */
        assert(sa.in_flight && !sa.retain && sa.transfer_id == st.a_id);
        assert(!idmas[1].quarantined);
        assert(sv.batch_started && !sv.in_flight && !sv.transfer_id);
        assert(sv.retain && idmas[0].quarantined);

        /* Same production STOP + drain order the caller uses on failure. */
        idma_batch_stop(&sv);
        idma_batch_stop(&sa);
        assert(hw[0].stop && hw[1].stop);
        r = idma_drain(&sa);
        if (drain_timeout) {
            assert(r == -ETIMEDOUT);
            assert(sa.retain && idmas[1].quarantined && hw[1].pending);
        } else {
            assert(r == 0);
            assert(!sa.in_flight && !sa.retain && !hw[1].pending);
            assert(idma_batch_collect(&sa) == 0);
        }
        /* V has no ID: collecting and draining it must keep refusing. */
        assert(idma_batch_collect(&sv) == -EIO);
        assert(idma_drain(&sv) == -EIO);
        assert(sv.retain && !sv.in_flight);

        idma_stream_cleanup(&sa);
        idma_stream_cleanup(&sv);
        assert(allocations == 4);
        assert(frees == (drain_timeout ? 0U : 2U));
        assert(unmaps == (drain_timeout ? 0U : 2U));
        ret = -EIO;
    } else if (!strncmp(name, "batch_", 6)) {
        bool solo = !strcmp(name,"batch_solo");
        bool busy = !strcmp(name,"batch_busy");
        bool no_warmup = !strcmp(name,"batch_no_warmup");
        bool swapped = !strcmp(name,"batch_swapped");
        bool missing = !strcmp(name,"batch_missing");
        bool missing_obs = !strcmp(name,"batch_missing_observer");
        bool timeout = !strcmp(name,"batch_timeout");
        bool interrupt = !strcmp(name,"batch_interrupt");
        bool zero = !strcmp(name,"batch_zero_id");
        bool data = !strcmp(name,"batch_data_error");
        unsigned v = swapped ? 1 : 0;
        e.victim_pages=1; e.aggressor_pages=8; e.victim_transfers=64;
        e.warmup_transfers=2; e.run_timeout_ms=100;
        if(solo) e.aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR;
        if(swapped) e.aggressor_id=0;
        hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8]=missing_obs ? IDMA_OBS_MAGIC : IDMA_OBS_MAGIC_V2;
        for(int i=0;i<2;i++) hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8]=IDMA_BATCH_MAGIC;
        if(missing) hw[1].regs[IDMA_BATCH_MAGIC_OFFSET / 8]=0;
        if(timeout||zero||interrupt||no_warmup) e.warmup_transfers=0;
        if(busy) hw[0].regs[IDMA_BATCH_CONTROL_OFFSET / 8]=3;
        if(timeout) {hw[v].latency=UINT64_MAX;e.run_timeout_ms=1;}
        if(zero) hw[v].zero_id=true;
        if(interrupt) interrupt_at=now+100000;
        inject_verify_error=data;
        ret=idma_run_experiment_mode(&idmas[v],&e,true);
        if(missing||missing_obs||busy) {assert(ret==(busy?-EBUSY:-EOPNOTSUPP));assert(!allocations&&!hw[0].submits&&!hw[1].submits);}
        else if(timeout||zero) {
            assert(ret==(timeout?-ETIMEDOUT:-EIO));
            assert(e.victim.quarantined&&allocations>frees);
            if (zero) {
                /*
                 * V never produced an ID, but the valid A trigger was still
                 * committed, stopped, drained and freed by production code.
                 */
                assert(!e.aggressor.quarantined);
                assert(allocations - frees == 2 && !hw[1].pending);
            }
        }
        else if(interrupt) {assert(ret==-EINTR);assert(allocations==frees);}
        else if(data) {assert(ret==-EILSEQ);assert(allocations==frees);}
        else {assert(ret==0);assert(e.victim.completed==64&&e.victim.submitted==64);assert(e.victim.refill_gap_samples==0);assert(hw[v].submits==(no_warmup?1:3));assert(allocations==frees);}
        assert(e.status==ret);
    } else if (!strncmp(name, "p4m_", 4)) {
        struct idma_p4m p;
        bool solo = !strcmp(name, "p4m_solo_ok");
        bool timeout = !strcmp(name, "p4m_timeout");
        bool interrupt = !strcmp(name, "p4m_interrupt");
        bool old_hw = !strcmp(name, "p4m_old_hw");
        bool cap_missing = !strcmp(name, "p4m_cap_missing");
        bool readback_bad = !strcmp(name, "p4m_readback_mismatch");
        bool solo_gap = !strcmp(name, "p4m_solo_gap_nonzero");
        bool wrong_roles = !strcmp(name, "p4m_wrong_roles");
        bool bad_reserved = !strcmp(name, "p4m_bad_reserved");
        bool bad_counts = !strcmp(name, "p4m_bad_counts");
        bool bad_status = !strcmp(name, "p4m_bad_status");
        bool bad_bank = !strcmp(name, "p4m_bad_bank");
        bool bad_flags = !strcmp(name, "p4m_bad_flags");
        bool bad_conservation = !strcmp(name, "p4m_bad_conservation");
        bool bad_carry = !strcmp(name, "p4m_bad_carry");
        bool bad_carry_done = !strcmp(name, "p4m_bad_carry_done");
        bool bad_ids = !strcmp(name, "p4m_bad_ids");
        bool bad_unknown_flag = !strcmp(name, "p4m_bad_unknown_flag");
        bool zero_cycles = !strcmp(name, "p4m_bad_zero_cycles");
        bool id_sequence = !strcmp(name, "p4m_bad_id_sequence");
        bool id_overflow = !strcmp(name, "p4m_bad_id_overflow");
        bool zero_drained = !strcmp(name, "p4m_zero_a_progress_drained");
        bool zero_pending = !strcmp(name, "p4m_zero_a_progress_pending");
        bool strict_carry = !strcmp(name, "p4m_strict_carry_ok");
        bool open_edge = !strcmp(name, "p4m_open_edge_ok");
        bool sticky = !strcmp(name, "p4m_sticky_error");
        bool direct = !strcmp(name, "p4m_direct_finish_not_armed");
        bool has_a = !solo && !solo_gap && !wrong_roles;
        bool expect_invalid = bad_reserved || bad_counts || bad_status ||
                              bad_bank || bad_flags || bad_conservation ||
                              bad_carry || bad_carry_done || bad_ids ||
                              bad_unknown_flag ||
                              zero_cycles || id_sequence || id_overflow;

        if (direct) {
            struct idma_observation o = {};
            struct idma_p4m q = {};
            struct idma_experiment req = e;

            assert(idma_p4m_finish(&o, &req, &q, 0) == -EIO);
            assert(idma_p4m_finish(&o, &req, &q, -ETIMEDOUT) == -ETIMEDOUT);
            printf("PASS %s (unarmed finish cannot report success)\n", name);
            return 0;
        }

        memset(&p, 0, sizeof(p));
        p.experiment = e;
        p.experiment.victim_pages = 1;
        p.experiment.aggressor_pages = 1;
        p.experiment.victim_transfers = 8;
        p.experiment.warmup_transfers = 2;
        p.experiment.chunk_bytes = 4096;
        p.experiment.run_timeout_ms = 100;
        if (solo || solo_gap) {
            p.experiment.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
            p.aggressor_gap_cycles = solo_gap ? 1 : 0;
        } else if (wrong_roles) {
            p.experiment.aggressor_id = 2;
        } else {
            p.experiment.aggressor_id = 1;
            p.aggressor_gap_cycles = 1000;
        }
        if (bad_reserved)
            p.reserved[0] = 1;

        p4m_mode = true;
        idma_path_observe = true;
        p4m_bad_bank = bad_bank;
        p4m_bad_flags = bad_flags;
        p4m_bad_conservation = bad_conservation || bad_carry || bad_ids;
        p4m_bad_counts = bad_counts;
        p4m_bad_status = bad_status;
        p4m_bad_readback = readback_bad;
        p4m_cap_missing = cap_missing;
        p4m_sticky_error = sticky;

        hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = old_hw ? IDMA_OBS_MAGIC_V6 : IDMA_OBS_MAGIC_V7;
        hw[0].regs[IDMA_BATCH_GAP_MAGIC_OFFSET / 8] = IDMA_BATCH_GAP_MAGIC;
        hw[1].regs[IDMA_BATCH_GAP_MAGIC_OFFSET / 8] = IDMA_BATCH_GAP_MAGIC;
        for (int i = 0; i < 2; i++)
            hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8] = IDMA_BATCH_MAGIC;

        memset(p4m_window, 0, sizeof(p4m_window));
        p4m_window[0] = IDMA_P4M_BANK_MAGIC;
        p4m_window[1] = 30;
        p4m_window[13] = p.experiment.victim_transfers;
        p4m_window[14] = p.experiment.victim_transfers;
        p4m_window[15] = BIT_ULL(1) | (has_a ? BIT_ULL(0) : 0);
        if (has_a) {
            p4m_window[2] = 2;
            p4m_window[3] = 1;
            p4m_window[5] = 1;
            /* Coherent batch IDs: endID = last retired ID + 1. */
            p4m_window[9] = 0x101;
            p4m_window[10] = 0x100;
            p4m_window[11] = 0x100;
        }
        if (zero_drained) {
            /* Genuine zero A progress and fully drained at the closing edge:
             * no A issue/done events, no A stock and no A IDs anywhere. */
            p4m_window[2] = 0;
            p4m_window[3] = 0;
            p4m_window[4] = 0;
            p4m_window[5] = 0;
            p4m_window[6] = 0;
            p4m_window[7] = 0;
            p4m_window[8] = 0;
            p4m_window[9] = 0;
            p4m_window[10] = 0;
            p4m_window[11] = 0;
            p4m_window[12] = 0;
            p4m_window[15] = BIT_ULL(0) | BIT_ULL(1);
        }
        if (zero_pending) {
            /* Strict pre-opening A job still in flight at both edges, with no
             * A event and no A completion inside the window: done stays zero. */
            p4m_window[2] = 0;
            p4m_window[3] = 0;
            p4m_window[4] = 1;
            p4m_window[5] = 1;
            p4m_window[6] = 0;
            p4m_window[7] = 0;
            p4m_window[8] = 0x100;
            p4m_window[9] = 0x100;
            p4m_window[10] = 0;
            p4m_window[11] = 0;
            p4m_window[15] = BIT_ULL(0) | BIT_ULL(1) | BIT_ULL(2);
        }
        if (strict_carry) {
            p4m_window[2] = 2;
            p4m_window[3] = 2;
            p4m_window[4] = 1;
            p4m_window[5] = 1;
            p4m_window[6] = 1;
            p4m_window[8] = 0x100;
            p4m_window[9] = 0x102;
            p4m_window[10] = 0x100;
            p4m_window[11] = 0x101;
            p4m_window[15] |= BIT_ULL(2);
        }
        if (open_edge) {
            p4m_window[2] = 2;
            p4m_window[3] = 2;
            p4m_window[4] = 1;
            p4m_window[5] = 1;
            p4m_window[6] = 0;
            p4m_window[7] = 1;
            p4m_window[8] = 0x100;
            p4m_window[9] = 0x102;
            p4m_window[10] = 0x100;
            p4m_window[11] = 0x101;
        }
        if (bad_bank)
            p4m_window[0] = 0;
        if (bad_flags)
            p4m_window[15] &= ~BIT_ULL(1);
        if (bad_conservation)
            p4m_window[2] += 1;
        if (bad_carry)
            p4m_window[6] = 1;
        if (bad_carry_done) {
            /* Valid strict pre-opening cohort with in-window A completions,
             * correct conservation and a correct strict flag, but the carried
             * job's completion is not recorded: carry_done stays zero. */
            p4m_window[2] = 2;
            p4m_window[3] = 2;
            p4m_window[4] = 1;
            p4m_window[5] = 1;
            p4m_window[6] = 0;
            p4m_window[8] = 0x100;
            p4m_window[9] = 0x102;
            p4m_window[10] = 0x100;
            p4m_window[11] = 0x101;
            p4m_window[15] |= BIT_ULL(2);
        }
        if (bad_ids)
            p4m_window[10] = 0;
        if (bad_counts)
            p4m_window[13] -= 1;
        if (bad_unknown_flag)
            p4m_window[15] |= BIT_ULL(3);
        if (zero_cycles)
            p4m_window[1] = 0;
        if (id_sequence) {
            /* done=2 but last still equals first: not contiguous. */
            p4m_window[2] = 3;
            p4m_window[3] = 2;
        }
        if (id_overflow) {
            /* Conservation and contiguity hold; only endID wraps the range. */
            p4m_window[2] = 2;
            p4m_window[3] = 1;
            p4m_window[10] = IDMA_EXPERIMENT_MAX_TRANSFERS;
            p4m_window[11] = IDMA_EXPERIMENT_MAX_TRANSFERS;
            p4m_window[9] = IDMA_EXPERIMENT_MAX_TRANSFERS + 1;
        }

        if (timeout || interrupt) {
            hw[0].latency = UINT64_MAX;
            hw[1].latency = UINT64_MAX;
        }
        if (interrupt)
            interrupt_at = now + 100000;

        ret = idma_run_p4m(&idmas[0], &p);

        if (bad_reserved || solo_gap || wrong_roles) {
            assert(ret == -EINVAL);
            assert(p.experiment.status == -EINVAL);
            assert(!allocations && !hw[0].submits && !hw[1].submits);
        } else if (old_hw || cap_missing) {
            assert(ret == -EOPNOTSUPP);
            assert(p.experiment.status == -EOPNOTSUPP);
            assert(!allocations && !hw[0].submits && !hw[1].submits);
        } else if (readback_bad) {
            assert(ret == -EIO);
            assert(p.experiment.status == -EIO);
            assert(allocations == frees && !hw[0].pending && !hw[1].pending);
        } else if (timeout || interrupt) {
            assert(ret == (timeout ? -ETIMEDOUT : -EINTR));
            assert(p.experiment.status == ret);
            assert(idmas[0].quarantined);
            assert(allocations > frees);
        } else if (sticky) {
            assert(ret == -EIO);
            assert(p.experiment.status == -EIO);
            assert(allocations == frees && !hw[0].pending && !hw[1].pending);
        } else if (expect_invalid) {
            assert(ret == -EIO);
            assert(p.experiment.status == -EIO);
            assert(allocations == frees);
            if (bad_bank)
                assert(p.window[0] == 0);
            else
                assert(p.window[0] == IDMA_P4M_BANK_MAGIC);
            if (bad_flags)
                assert(!(p.window[15] & BIT_ULL(1)));
            if (bad_conservation)
                assert(p.window[2] == 3);
            if (bad_carry)
                assert(p.window[6] == 1);
            if (bad_carry_done) {
                assert(!p.window[6]);
                assert(p.window[15] & BIT_ULL(2));
                assert(p.window[4] == 1 && p.window[3] > 0);
                assert(p.window[4] + p.window[2] == p.window[3] + p.window[5]);
            }
            if (bad_ids)
                assert(p.window[10] == 0);
            if (bad_counts)
                assert(p.window[13] != p.experiment.victim_transfers);
            if (bad_unknown_flag)
                assert(p.window[15] & BIT_ULL(3));
            if (zero_cycles)
                assert(!p.window[1]);
            if (id_overflow)
                assert(p.window[9] > IDMA_EXPERIMENT_MAX_TRANSFERS);
        } else {
            assert(ret == 0);
            assert(p.experiment.status == 0);
            assert(p.sequence && p.observer_status == 4);
            assert(p.window[0] == IDMA_P4M_BANK_MAGIC);
            assert(p.window[1]);
            assert(p.window[15] & BIT_ULL(1));
            if (zero_pending) {
                assert(p.window[2] == 0 && p.window[3] == 0);
                assert(p.window[4] == 1 && p.window[5] == 1);
                assert(p.window[6] == 0 && p.window[7] == 0);
                assert(p.window[8] == 0x100 && p.window[9] == 0x100);
                assert(p.window[10] == 0 && p.window[11] == 0);
                assert(p.window[15] == (BIT_ULL(0) | BIT_ULL(1) | BIT_ULL(2)));
            }
            if (strict_carry) {
                assert(p.window[4] == 1 && p.window[6] == 1);
                assert(p.window[15] & BIT_ULL(2));
            }
            if (open_edge) {
                assert(p.window[4] == 1 && p.window[7] == 1);
                assert(!(p.window[15] & BIT_ULL(2)));
            }
            if (zero_drained) {
                assert(p.window[2] == 0 && p.window[3] == 0);
                assert(p.window[4] == 0 && p.window[5] == 0);
                assert(p.window[6] == 0 && p.window[7] == 0);
                assert(p.window[8] == 0 && p.window[9] == 0);
                assert(p.window[10] == 0 && p.window[11] == 0);
                assert(p.window[12] == 0);
                assert(p.window[15] == (BIT_ULL(0) | BIT_ULL(1)));
            }
            if (has_a) {
                if (!strcmp(name, "p4m_ok")) {
                    assert(p.window[2] == 2 && p.window[3] == 1);
                    assert(p.experiment.aggressor.submitted >=
                           p.experiment.aggressor.completed);
                }
            } else {
                for (unsigned int i = 2; i <= 12; i++)
                    assert(!p.window[i]);
            }
            assert(allocations == frees);
        }
        printf("PASS %s (P4M software contract; no FPGA/RTL timing claims)\n", name);
        return 0;
    } else if (!strncmp(name, "observer_", 9)) {
        bool swapped = !strcmp(name, "observer_roles_swapped");
        bool data_error = !strcmp(name, "observer_data_error");
        idma_observe = true;
        if (swapped) e.aggressor_id = 0;
        inject_verify_error = data_error;
        hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC;
        e.warmup_transfers = 2;
        obs_bad_sum = !strcmp(name, "observer_bad_sum");
        obs_bad_axi = !strcmp(name, "observer_bad_axi");
        obs_wrong_count = !strcmp(name, "observer_wrong_count");
        obs_stuck = !strcmp(name, "observer_stuck");
        obs_arm_error = !strcmp(name, "observer_arm_error");
        if (!strcmp(name, "observer_missing")) hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = 0;
        if (!strcmp(name, "observer_busy")) hw[0].regs[IDMA_OBS_CONTROL_OFFSET / 8] = 2;
        if (!strcmp(name, "observer_timeout")) { hw[0].latency = UINT64_MAX; e.warmup_transfers = 0; }
        ret = idma_run_experiment(&idmas[swapped ? 1 : 0], &e);
        if (!strcmp(name, "observer_missing") || !strcmp(name, "observer_busy")) {
            assert(ret == (!strcmp(name, "observer_missing") ? -EOPNOTSUPP : -EBUSY));
            assert(!allocations && !hw[0].submits && !hw[1].submits && !obs_arms && !obs_reports);
        } else {
            assert(obs_arms == 1 && obs_reports == 3);
            assert(ret == ((!strcmp(name, "observer_ok") || swapped) ? 0 : data_error ? -EILSEQ : !strcmp(name, "observer_timeout") ? -ETIMEDOUT : -EIO));
            if (!strcmp(name, "observer_ok") || swapped) assert(hw[swapped ? 1 : 0].submits == 5 && e.victim.completed == 3);
            if (strcmp(name, "observer_timeout")) assert(allocations == frees);
            else assert(e.victim.quarantined && allocations > frees);
        }
        assert(e.status == ret);
    } else if (!strncmp(name, "cache_", 6)) {
        cache_shadow = true;
        corrupt_backing = !strcmp(name, "cache_real_corruption");
        e.victim_pages = e.aggressor_pages = 1;
        e.victim_transfers = 8;
        e.warmup_transfers = 32;
        if (strcmp(name, "cache_corun")) e.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == (corrupt_backing ? -EILSEQ : 0));
        assert(e.status == ret && e.measurement_complete);
        assert(e.victim.completed == 8 && hw[0].submits == 40);
        assert(e.victim.data_errors == corrupt_backing && !e.aggressor.data_errors);
        assert(prepare_flushes == 1 && finish_flushes == (corrupt_backing ? 2U : 1U));
        assert(diagnostic_flushes == corrupt_backing);
        assert(allocations == frees && !hw[0].pending && !hw[1].pending);
    } else if (!strncmp(name, "verify_", 7)) {
        struct idma_stream stream = {.idma = &idmas[0]};
        u64 pattern = idma_pattern(&stream, 17 / sizeof(u64));
        u8 expected = ((u8 *)&pattern)[17 % sizeof(u64)];

        inject_verify_error = true;
        corrupt_source = strstr(name, "source") != NULL;
        restore_on_flush = strstr(name, "stale") != NULL;
        e.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
        e.victim_pages = 1;
        e.victim_transfers = 1;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == -EILSEQ && e.status == -EILSEQ && e.measurement_complete);
        assert(e.victim.data_errors == 1 && !e.victim.quarantined);
        assert(diagnostic.count == 1 && diagnostic_flushes == 1 && diagnostic_mutated);
        assert(diagnostic.id == 0 && diagnostic.did == 10);
        assert(diagnostic.offset == 17 && diagnostic.touched_end == 4096);
        assert(diagnostic.size == 4096 && diagnostic.chunk == 4096);
        assert(diagnostic.expected_src == expected && diagnostic.expected_dst == expected);
        assert(diagnostic.src == (unsigned)(corrupt_source ? expected ^ 0x40 : expected));
        assert(diagnostic.dst == (unsigned)(corrupt_source ? expected : expected ^ 0x40));
        assert(diagnostic.src_after == (restore_on_flush ? expected : diagnostic.src));
        assert(diagnostic.dst_after == (restore_on_flush ? expected : diagnostic.dst));
        assert(diagnostic.src_iova == hw[0].submitted_src[0] + 17);
        assert(diagnostic.dst_iova == hw[0].regs[IDMA_DST_ADDR_REG_OFFSET / 8] + 17);
        assert(diagnostic.src_pa == diagnostic.src_iova && diagnostic.dst_pa == diagnostic.dst_iova);
        assert(diagnostic.src_pte_pa == diagnostic.src_pa + PAGE_SIZE);
        assert(diagnostic.dst_pte_pa == diagnostic.dst_pa + PAGE_SIZE);
        assert(allocations == 2 && frees == 2 && unmaps == 2 && hpm_releases == 1);
    } else if (!strcmp(name, "small_chunk_copy") || !strcmp(name, "small_chunk_wrong_source")) {
        assert(argc == 5);
        e.chunk_bytes = strtoul(argv[2], NULL, 10);
        idmas[0].did = strtoul(argv[3], NULL, 10);
        idmas[1].did = strtoul(argv[4], NULL, 10);
        e.victim_pages = e.aggressor_pages = 1;
        e.victim_transfers = 1;
        /* Keep A's first copy in flight until V ends; each device copies once. */
        hw[1].latency = 100000;
        wrong_source = !strcmp(name, "small_chunk_wrong_source");
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == (wrong_source ? -EILSEQ : 0));
        assert(e.status == ret && e.measurement_complete);
        assert(e.victim.data_errors == wrong_source && e.aggressor.data_errors == wrong_source);
        assert(hw[0].submits == 1 && hw[1].submits == 1);
        assert_all_freed();
    } else if (!strcmp(name, "victim_finishes_first")) {
        hw[0].latency = 1000; hw[1].latency = 1500000;
        e.victim_transfers = 2;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == 0 && e.measurement_complete && e.status == 0);
        assert(e.victim_elapsed_ns < hw[1].latency);
        assert(e.victim.completed == 2 && e.victim.submitted == 2);
        assert(e.aggressor.in_flight_at_end == 1 && e.aggressor.completed == 0);
        assert(hw[1].submits == 1 && e.aggressor.submitted == 0);
        assert(e.drain_ns > e.victim_elapsed_ns);
        assert_all_freed();
    } else if (!strcmp(name, "both_ready_no_aggressor_refill")) {
        hw[0].latency = hw[1].latency = 1000;
        e.victim_transfers = 1;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == 0 && e.victim.completed == 1);
        assert(hw[1].submits == 1 && e.aggressor.completed == 0);
        assert(e.aggressor.in_flight_at_end == 1);
        assert_all_freed();
    } else if (!strcmp(name, "aggressor_refills")) {
        size_t i;
        hw[0].latency = 100000; hw[1].latency = 1000;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == 0 && e.victim.completed == 3);
        assert(hw[1].submits > 10);
        assert(e.aggressor.submitted == hw[1].submits - 1);
        assert(e.aggressor.completed == e.aggressor.submitted);
        assert(e.aggressor.refill_gap_samples == e.aggressor.submitted);
        assert(e.aggressor.refill_gap_ns_max > 0);
        for (i = 0; i < hw[1].submits; i++)
            assert(hw[1].submitted_src[i] == hw[1].submitted_src[0] + (i % 2) * 4096);
        assert_all_freed();
    } else if (!strcmp(name, "warmup_not_counted")) {
        e.warmup_transfers = 7; e.victim_transfers = 5;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == 0 && hw[0].submits == 12);
        assert(e.victim.submitted == 5 && e.victim.completed == 5);
        assert(e.victim.completed_bytes == 5 * 4096);
        assert(e.victim.refill_gap_samples == 4);
        assert(hw[0].submitted_src[7] == hw[0].submitted_src[0]);
        assert(e.aggressor.submitted < hw[1].submits - 1);
        assert_all_freed();
    } else if (!strcmp(name, "expired_before_submit") ||
               !strcmp(name, "interrupted_before_submit")) {
        struct idma_stream v = {};
        u64 end = 0, deadline = UINT64_MAX;
        idma_stream_prepare(&v, &idmas[0], 1, 4096, &e.victim);
        if (name[0] == 'e') deadline = now - 1;
        else interrupt_at = 0;
        ret = idma_run_window(&v, NULL, 1, deadline, true, &end);
        assert(ret == (name[0] == 'e' ? -ETIMEDOUT : -EINTR));
        assert(hw[0].submits == 0 && e.victim.submitted == 0 && !v.in_flight);
        assert(end >= now);
        idma_stream_cleanup(&v);
        assert(frees == 2);
    } else if (!strcmp(name, "timeout_then_safe_drain") ||
               !strcmp(name, "interrupt_then_safe_drain")) {
        bool interrupted = name[0] == 'i';
        hw[0].latency = 1500000; hw[1].latency = 1800000;
        if (interrupted) interrupt_at = 30000;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == (interrupted ? -EINTR : -ETIMEDOUT));
        assert(e.status == ret && !e.measurement_complete && e.measurement_started);
        assert(hw[0].submits == 1 && hw[1].submits == 1);
        assert(e.victim.completed == 0 && e.aggressor.completed == 0);
        assert_all_freed();
    } else if (!strcmp(name, "timeout_quarantine_blocks_reuse")) {
        unsigned initial_allocations;
        hw[0].latency = UINT64_MAX;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == -ETIMEDOUT && e.status == -ETIMEDOUT);
        assert(e.victim.quarantined == 1 && e.aggressor.quarantined == 0);
        assert(hw[0].pending && !hw[1].pending);
        assert(frees == 2 && unmaps == 2 && hpm_releases == 1);
        initial_allocations = allocations;
        e.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
        ret = idma_run_experiment(&idmas[1], &e);
        assert(ret == -EIO && allocations == initial_allocations);
    } else if (!strcmp(name, "interrupt_quarantines_both")) {
        hw[0].latency = hw[1].latency = UINT64_MAX;
        interrupt_at = 30000;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == -EINTR && e.status == -EINTR);
        assert(e.victim.quarantined && e.aggressor.quarantined);
        assert(hw[0].pending && hw[1].pending);
        assert(frees == 0 && unmaps == 0 && hpm_releases == 1);
    } else if (!strcmp(name, "zero_victim_id_retains") ||
               !strcmp(name, "zero_aggressor_id_retains")) {
        bool victim = !strcmp(name, "zero_victim_id_retains");
        int bad = victim ? 0 : 1;
        hw[bad].zero_id = true;
        ret = idma_run_experiment(&idmas[0], &e);
        assert(ret == -EIO && e.status == -EIO);
        assert(idmas[bad].quarantined && hw[bad].pending);
        assert(!idmas[1 - bad].quarantined && !hw[1 - bad].pending);
        assert((victim ? e.victim.quarantined : e.aggressor.quarantined) == 1);
        assert(frees == 2 && unmaps == 2 && hpm_releases == 1);
    } else {
        assert(!"unknown test");
    }
    if (!inject_verify_error && !wrong_source && !corrupt_backing)
        assert(diagnostic_flushes == 0 && diagnostic.count == 0);
    if (!ret)
        assert(prepare_flushes == 1 && finish_flushes == 1);
    else if (ret != -EILSEQ && !obs_reports)
        assert(finish_flushes == 0); /* Never flush/verify on uncertain drain. */
    if (!idma_observe && !idma_path_observe && strncmp(name, "batch_", 6)) assert(!obs_mmio);
    printf("PASS %s (host model; no FPGA claims)\n", name);
    return 0;
}
