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

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uintptr_t dma_addr_t;
struct device { int index; };
struct miscdevice { struct device *parent; };
struct mutex { int unused; };
struct iommu_domain { int unused; };
#define __iomem
#define __user
#define WRITE_ONCE(dst, src) ((dst) = (src))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define dev_crit(dev, ...) ((void)(dev))
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define PAGE_SIZE 4096U
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL

#define IDMA_SRC_ADDR_REG_OFFSET 0x00
#define IDMA_DST_ADDR_REG_OFFSET 0x08
#define IDMA_NUM_BYTES_REG_OFFSET 0x10
#define IDMA_CONF_REG_OFFSET 0x18
#define IDMA_STATUS_REG_OFFSET 0x20
#define IDMA_NEXT_ID_REG_OFFSET 0x28
#define IDMA_DONE_REG_OFFSET 0x30
#define IDMA_ACTIVE_CYCLES_REG_OFFSET 0x50
#define IDMA_XFER_TIMEOUT_US 2000000
#define IDMA_MAX_DEVS 8
#define IDMA_MAX_PAGES 4096U
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U
struct idma_device {
	struct miscdevice miscdev;
	char name[16];
	int id;
	u32 did;
	void __iomem *reg;
	bool quarantined;
};
struct idma_stream {
	struct idma_device *idma;
	struct device *dev;
	struct iommu_domain *domain;
	void *src, *dst;
	dma_addr_t src_dma, dst_dma;
	unsigned long src_iova, dst_iova;
	size_t size, offset, next_offset, touched_end;
	u32 chunk;
	u64 transfer_id, deadline_ns, last_complete_ns;
	bool mapped_src, mapped_dst, in_flight, retain;
	struct idma_experiment_stats *stats;
};

struct riscv_iommu_hpm_session { int unused; };
struct riscv_iommu_hpm_sample { u64 iotlb_miss[2], s1_ptw[2], s2_ptw[2]; };
struct fake_dma {
    u64 regs[16], latency, due, next_id, done, submits;
    uintptr_t submitted_src[4096];
    bool pending, zero_id;
};
static struct fake_dma hw[2];
static struct device devices[2] = {{0}, {1}};
static struct idma_device idmas[2];
static struct mutex idma_experiment_lock;
static struct riscv_iommu_hpm_session session;
static u64 now = 1000, interrupt_at = UINT64_MAX;
static unsigned allocations, frees, unmaps, hpm_releases;

static void advance(u64 delta)
{
    int i;
    now += delta;
    for (i = 0; i < 2; i++) {
        struct fake_dma *h = &hw[i];
        if (h->pending && h->due != UINT64_MAX && now >= h->due) {
            memcpy((void *)(uintptr_t)h->regs[IDMA_DST_ADDR_REG_OFFSET / 8],
                   (void *)(uintptr_t)hw[1 - i].regs[IDMA_SRC_ADDR_REG_OFFSET / 8],
                   h->regs[IDMA_NUM_BYTES_REG_OFFSET / 8]);
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
    assert(!h->pending);
    h->regs[offset / 8] = value;
}

static u64 readq(void *address)
{
    size_t offset;
    struct fake_dma *h = mmio(address, &offset);
    advance(1000);
    if (offset == IDMA_NEXT_ID_REG_OFFSET) {
        assert(!h->pending);
        assert(h->submits < 4096);
        h->submitted_src[h->submits++] = h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
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
    return 0;
}
static int riscv_iommu_hpm_acquire(struct device *v, struct device *a,
                                  struct riscv_iommu_hpm_session **s)
{
    (void)v; (void)a; *s = &session; return 0;
}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *s,
                               struct riscv_iommu_hpm_sample *sample)
{
    (void)s; memset(sample, 0, sizeof(*sample)); advance(1000);
}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *s)
{
    assert(s == &session); hpm_releases++;
}

static u64 idma_pattern(struct idma_stream *s, size_t word)
{
	return (0xa53c691df027b84eULL ^ ((u64)s->idma->did << 40) ^
		((u64)word * 0x9e3779b97f4a7c15ULL));
}
static void idma_stream_cleanup(struct idma_stream *s)
{
	if (!s->dev || s->retain)
		return;
	if (s->mapped_dst)
		idma_iommu_unmap_range(s->domain, s->dst_iova, s->size, PAGE_SIZE);
	if (s->mapped_src)
		idma_iommu_unmap_range(s->domain, s->src_iova, s->size, PAGE_SIZE);
	if (s->dst)
		dma_free_attrs(s->dev, s->size, s->dst, s->dst_dma,
			       DMA_ATTR_FORCE_CONTIGUOUS);
	if (s->src)
		dma_free_attrs(s->dev, s->size, s->src, s->src_dma,
			       DMA_ATTR_FORCE_CONTIGUOUS);
}
static void idma_quarantine(struct idma_stream *s)
{
	s->retain = true;
	WRITE_ONCE(s->idma->quarantined, true);
	s->stats->quarantined = 1;
	dev_crit(s->dev, "experiment DMA not safely drained; buffers retained until reboot\n");
}
static int idma_submit(struct idma_stream *s, bool measured)
{
	u64 now;

	s->offset = s->next_offset;
	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
	s->transfer_id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
	if (!s->transfer_id) {
		/* Without a valid ID we cannot prove that buffers are no longer used. */
		idma_quarantine(s);
		return -EIO;
	}
	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
	now = ktime_get_ns();
	if (s->last_complete_ns && measured) {
		u64 gap = now - s->last_complete_ns;

		s->stats->refill_gap_ns_sum += gap;
		s->stats->refill_gap_ns_max = max(s->stats->refill_gap_ns_max, gap);
		s->stats->refill_gap_samples++;
	}
	s->last_complete_ns = 0;
	s->deadline_ns = now + (u64)IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;
	s->in_flight = true;
	s->next_offset += s->chunk;
	if (s->next_offset == s->size)
		s->next_offset = 0;
	if (measured)
		s->stats->submitted++;
	return 0;
}
static int idma_poll(struct idma_stream *s, bool measured)
{
	u64 done;

	if (!s->in_flight)
		return 0;
	done = readq(s->idma->reg + IDMA_DONE_REG_OFFSET);
	if (done != s->transfer_id)
		return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;
	s->last_complete_ns = ktime_get_ns();
	s->in_flight = false;
	s->touched_end = max(s->touched_end, s->offset + s->chunk);
	if (measured) {
		s->stats->completed++;
		s->stats->completed_bytes += s->chunk;
	}
	return 1;
}
static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
			   u64 count, u64 deadline, bool measured, u64 *end_ns)
{
	u64 completed = 0;
	u32 spins = 0;
	int ret;

	if (!count)
		return 0;
	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}
	ret = idma_submit(v, measured);
	if (ret)
		goto out;
	for (;;) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			break;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		ret = idma_poll(v, measured);
		if (ret < 0)
			break;
		if (ret == 1) {
			if (++completed == count) {
				/* Capture V's end before polling/refilling A or taking snapshots. */
				*end_ns = v->last_complete_ns;
				return 0;
			}
			ret = idma_submit(v, measured);
			if (ret)
				break;
		}
		if (a) {
			ret = idma_poll(a, measured);
			if (ret < 0)
				break;
			if (ret == 1) {
				ret = idma_submit(a, measured);
				if (ret)
					break;
			}
		}
		if (!(++spins & 1023))
			cond_resched();
		cpu_relax();
	}
 out:
	*end_ns = ktime_get_ns();
	return ret;
}
static int idma_drain(struct idma_stream *s)
{
	u64 done;
	int ret;

	if (!s->in_flight)
		return s->retain ? -EIO : 0;
	ret = readq_poll_timeout(s->idma->reg + IDMA_DONE_REG_OFFSET, done,
				 done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);
	if (ret) {
		idma_quarantine(s);
		return ret;
	}
	s->in_flight = false;
	s->touched_end = max(s->touched_end, s->offset + s->chunk);
	return 0;
}
static int idma_verify(struct idma_stream *s)
{
	size_t i;

	if (s->retain)
		return -EIO;
	/* Verify untouched bytes as well, and source integrity independently. */
	for (i = 0; i < s->size; i++) {
		u64 pattern = idma_pattern(s, i / sizeof(u64));
		u8 value = ((u8 *)&pattern)[i % sizeof(u64)];
		u8 expected = i < s->touched_end ? value : (u8)~value;

		if (((u8 *)s->src)[i] != value || ((u8 *)s->dst)[i] != expected) {
			s->stats->data_errors++;
			return -EILSEQ;
		}
	}
	return 0;
}
static void idma_snapshot(struct riscv_iommu_hpm_session *hpm,
			  struct idma_stream *v, struct idma_stream *a,
			  struct riscv_iommu_hpm_sample *sample, u64 active[2])
{
	riscv_iommu_hpm_read(hpm, sample);
	active[0] = readq(v->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
	active[1] = a ? readq(a->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET) : 0;
}
static void idma_snapshot_delta(struct idma_experiment_stats *stats, int n,
			       const struct riscv_iommu_hpm_sample *before,
			       const struct riscv_iommu_hpm_sample *after,
			       u64 active_before, u64 active_after)
{
	stats->active_cycles = active_after - active_before;
	stats->iotlb_miss = after->iotlb_miss[n] - before->iotlb_miss[n];
	stats->s1_ptw = after->s1_ptw[n] - before->s1_ptw[n];
	stats->s2_ptw = after->s2_ptw[n] - before->s2_ptw[n];
}
static int idma_validate(const struct idma_experiment *e)
{
	if (PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e))
		return -EPROTO;
	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	if (!e->victim_pages || e->victim_pages > IDMA_MAX_PAGES ||
	    !e->aggressor_pages || e->aggressor_pages > IDMA_MAX_PAGES ||
	    !e->chunk_bytes || e->chunk_bytes > PAGE_SIZE ||
	    PAGE_SIZE % e->chunk_bytes || !e->victim_transfers ||
	    e->victim_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    e->warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    !e->run_timeout_ms || e->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	return 0;
}
static long idma_run_experiment(struct idma_device *victim,
			       struct idma_experiment __user *uarg)
{
	struct idma_experiment e;
	struct idma_stream streams[2] = {};
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	u64 active_before[2], active_after[2];
	u64 start, end = 0, deadline, stamp;
	int ret, drain_ret, verify_ret;

	if (copy_from_user(&e, uarg, sizeof(e)))
		return -EFAULT;
	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
	e.victim_did = victim->did;
	e.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_validate(&e);
	if (ret)
		goto out_copy;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_copy;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto out_unlock;
	}
	if (e.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) {
		aggressor = idma_get_instance(e.aggressor_id);
		if (!aggressor) {
			ret = -ENODEV;
			goto out_unlock;
		}
		if (aggressor == victim || aggressor->did == victim->did) {
			ret = -EINVAL;
			goto out_put;
		}
		e.aggressor_did = aggressor->did;
		a = &streams[1];
	}
	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
	if (ret)
		goto out_cleanup;
	if (a) {
		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
					  e.chunk_bytes, &e.aggressor);
		if (ret)
			goto out_cleanup;
	}
	ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
	if (ret)
		goto out_cleanup;
	e.hpm_configured = 1;
	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
	if (a) {
		ret = idma_submit(a, false);
		if (ret)
			goto out_drain;
	}
	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
	if (ret)
		goto out_drain;
	/* Every measured V task starts at the same address after warmup. */
	v->next_offset = 0;
	v->last_complete_ns = 0;
	if (a)
		a->last_complete_ns = 0;
	e.victim.in_flight_at_start = v->in_flight;
	e.aggressor.in_flight_at_start = a && a->in_flight;
	stamp = ktime_get_ns();
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e.snapshot_before_ns = start - stamp;
	e.measurement_started = 1;
	ret = idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
	e.victim_elapsed_ns = end - start;
	e.measurement_complete = !ret;
	e.victim.in_flight_at_end = v->in_flight;
	e.aggressor.in_flight_at_end = a && a->in_flight;
	/* No more A refills after V's final observed completion. */
	idma_snapshot(hpm, v, a, &after, active_after);
	e.snapshot_after_ns = ktime_get_ns() - end;
	idma_snapshot_delta(&e.victim, 0, &before, &after,
			    active_before[0], active_after[0]);
	if (a)
		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
				    active_before[1], active_after[1]);
 out_drain:
	stamp = ktime_get_ns();
	drain_ret = idma_drain(v);
	if (drain_ret && !ret)
		ret = drain_ret;
	if (a) {
		drain_ret = idma_drain(a);
		if (drain_ret && !ret)
			ret = drain_ret;
	}
	e.drain_ns = ktime_get_ns() - stamp;
	riscv_iommu_hpm_release(hpm);
	if (!ret) {
		verify_ret = idma_verify(v);
		if (verify_ret)
			ret = verify_ret;
		if (a) {
			verify_ret = idma_verify(a);
			if (verify_ret && !ret)
				ret = verify_ret;
		}
	}
 out_cleanup:
	if (a)
		idma_stream_cleanup(a);
	idma_stream_cleanup(v);
 out_put:
	idma_put_instance(aggressor);
 out_unlock:
	mutex_unlock(&idma_experiment_lock);
 out_copy:
	e.status = ret;
	if (copy_to_user(uarg, &e, sizeof(e)))
		return -EFAULT;
	return ret;
}

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
    assert(argc == 2);
    name = argv[1];
    if (!strcmp(name, "wrong_device_chunk4")) {
        e.chunk_bytes = 8; e.victim_transfers = 1; hw[1].latency = 100000;
        ret = idma_run_experiment(&idmas[0], &e);
        printf("WRONG-SOURCE-COPY ret=%ld status=%d complete=%u V_errors=%u A_errors=%u\n", ret, e.status, e.measurement_complete, e.victim.data_errors, e.aggressor.data_errors);
        assert(ret == -EILSEQ && e.measurement_complete && e.victim.data_errors == 1 && e.aggressor.data_errors == 1);
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
    printf("PASS %s (host model; no FPGA claims)\n", name);
    return 0;
}
