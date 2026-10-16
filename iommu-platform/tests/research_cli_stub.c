#define _GNU_SOURCE
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/idma.h>

#define OBS_MAGIC 0x49444d414f425308ULL
#define RES_MAGIC 0x49444d4152455331ULL
#define COST_MAGIC 0x49444d4152454331ULL
#define P4M_WINDOW_MAGIC 0x49444d4150344d01ULL
#define IDMA_FULLSTATS_MAGIC 0x49444d4141474731ULL

static const char *mode(void)
{
	const char *m = getenv("IDMA_TEST_MODE");

	return m ? m : "ok";
}

static void log_call(const char *name)
{
	const char *path = getenv("IDMA_TEST_LOG");
	FILE *f;

	if (!path)
		return;
	f = fopen(path, "a");
	if (!f)
		return;
	fprintf(f, "%s\n", name);
	fclose(f);
}

static int handle_caps(struct idma_experiment_caps *c)
{
	const char *m = mode();

	log_call("caps");
	memset(c, 0, sizeof(*c));
	c->abi_version = IDMA_EXPERIMENT_ABI;
	c->struct_size = sizeof(*c);
	c->capabilities = IDMA_EXPERIMENT_CAP_RESEARCH;
	c->device_id = 0;
	c->did = 10;
	c->max_pages = 16;
	c->page_bytes = 4096;
	c->max_run_timeout_ms = 10000;
	if (!strcmp(m, "caps_no_research"))
		c->capabilities = 0;
	else if (!strcmp(m, "caps_bad_abi"))
		c->abi_version = 2;
	else if (!strcmp(m, "caps_bad_size"))
		c->struct_size = 0;
	else if (!strcmp(m, "caps_bad_device"))
		c->device_id = 1;
	else if (!strcmp(m, "caps_bad_did"))
		c->did = 11;
	else if (!strcmp(m, "caps_bad_page"))
		c->page_bytes = 8192;
	else if (!strcmp(m, "caps_enotty")) {
		errno = ENOTTY;
		return -1;
	}
	return 0;
}

static void put_fullstats_group(uint64_t *raw, unsigned int group,
				uint64_t count, uint64_t sum_main,
				uint64_t sum_complete, uint64_t sum_duration,
				uint64_t sum_req, uint64_t sum_miss,
				uint64_t min_main, uint64_t max_main,
				uint64_t min_complete, uint64_t max_complete,
				uint64_t min_duration, uint64_t max_duration)
{
	unsigned int base = 0xD10 + group * 16;

	raw[base + 0] = count;
	raw[base + 1] = sum_main;
	raw[base + 2] = sum_complete;
	raw[base + 3] = sum_duration;
	raw[base + 4] = sum_req;
	raw[base + 5] = sum_miss;
	raw[base + 6] = (max_main << 32) | min_main;
	raw[base + 7] = (max_complete << 32) | min_complete;
	raw[base + 8] = (max_duration << 32) | min_duration;
}

static void fill_fullstats(struct idma_research *r, const char *m)
{
	uint64_t *raw = (uint64_t *)(uintptr_t)r->raw_user;
	unsigned int base = 0xD10;

	if (!raw)
		return;
	if (strncmp(m, "fullstats_", 10) != 0)
		return;
	raw[0xd00] = IDMA_FULLSTATS_MAGIC;
	raw[0xd01] = 1;
	raw[0xd02] = 6;
	raw[0xd03] = 16;
	raw[0xd04] = 0;
	if (r->a_pages == 0) {
		raw[0xd05] = 0;
		raw[0xc14] = 0;
		raw[0xc15] = 0;
	} else if (!strcmp(m, "fullstats_large")) {
		/* Wide-product case: min_main * count = 2^31 * 2^33 is 2^64 and
		 * wraps to 0 in 64-bit arithmetic, so a 64-bit mean check would
		 * wrongly accept; the 128-bit check rejects with the mean-range
		 * error.  The totals are irrelevant (the group loop fires first). */
		raw[0xd05] = 16;
		raw[0xc14] = 16;
		raw[0xc15] = 1;
		put_fullstats_group(raw, 0, 0x200000000ULL, 1, 0, 0,
				    0x800000000ULL, 0,
				    0x80000000ULL, 0xFFFFFFFFULL,
				    0, 0, 0, 0);
	} else {
		raw[0xd05] = 16;
		raw[0xc14] = 16;
		raw[0xc15] = 1;
		put_fullstats_group(raw, 0, 4, 4, 4, 8, 16, 0,
				    1, 1, 1, 1, 2, 2);
		put_fullstats_group(raw, 1, 4, 4, 4, 8, 16, 4,
				    1, 1, 1, 1, 2, 2);
		put_fullstats_group(raw, 2, 4, 4, 4, 8, 16, 8,
				    1, 1, 1, 1, 2, 2);
		put_fullstats_group(raw, 3, 4, 4, 4, 8, 16, 12,
				    1, 1, 1, 1, 2, 2);
	}
	if (!strcmp(m, "fullstats_corrupt"))
		raw[0xd04] = 1;
	else if (!strcmp(m, "fullstats_magic"))
		raw[0xd00] = 0xDEADBEEFDEADBEEFULL;
	else if (!strcmp(m, "fullstats_version"))
		raw[0xd01] = 2;
	else if (!strcmp(m, "fullstats_groups"))
		raw[0xd02] = 5;
	else if (!strcmp(m, "fullstats_stride"))
		raw[0xd03] = 8;
	else if (!strcmp(m, "fullstats_header_reserved"))
		raw[0xd07] = 1;
	else if (!strcmp(m, "fullstats_group_reserved"))
		raw[base + 9] = 1;
	else if (!strcmp(m, "fullstats_empty"))
		raw[base + 0] = 0;
	else if (!strcmp(m, "fullstats_minmax"))
		raw[base + 6] = (3ULL << 32) | 5ULL;
	else if (!strcmp(m, "fullstats_total_main"))
		raw[0xc1b] = 1;
	else if (!strcmp(m, "fullstats_total_complete"))
		raw[0xc1d] = 1;
	else if (!strcmp(m, "fullstats_total_req"))
		raw[0xc1f] = 1;
	else if (!strcmp(m, "fullstats_total_miss"))
		raw[0xc23] = 1;
}

static void fill_raw(struct idma_research *r)
{
	const char *m = mode();
	uint64_t *raw = (uint64_t *)(uintptr_t)r->raw_user;
	uint64_t T = r->window_cycles ? r->window_cycles : 5000000;
	uint64_t vpn0, vpn1, lo0, lo1;

	if (!raw)
		return;
	memset(raw, 0, IDMA_RESEARCH_RAW_WORDS * sizeof(uint64_t));
	r->mapping[0] = 0x10000000;
	r->mapping[1] = 0x20000000;
	r->mapping[2] = 0x30000000;
	r->mapping[3] = 0x40000000;
	raw[0] = RES_MAGIC;
	raw[1] = OBS_MAGIC;
	raw[2] = (1ULL << 21) | (1ULL << 22);
	raw[4] = r->mode;
	raw[6] = 4;
	raw[0x21] = 7;
	raw[0x1c] = 3;
	raw[0x1d] = 3;
	raw[0x25] = r->a_transfers;
	raw[0x26] = r->a_transfers;
	raw[8] = T;
	raw[7] = 1000;
	raw[0x1e] = raw[7] + T;
	raw[0x1f] = raw[0x1e] + 10;
	raw[0x14] = raw[0x1f] + 5;
	raw[0x18] = raw[0x1f] + 6;
	raw[0x20] = raw[0x18] + 7;
	vpn0 = (r->mapping[0] >> 12) & ((1ULL << 29) - 1);
	vpn1 = (r->mapping[1] >> 12) & ((1ULL << 29) - 1);
	lo0 = vpn0 | (0x123ULL << 29) | (1ULL << 49) | (1ULL << 51);
	lo1 = vpn1 | (0x123ULL << 29) | (1ULL << 49) | (1ULL << 51);
	raw[0x10] = lo0;
	raw[0x11] = 0x456;
	raw[0x12] = lo1;
	raw[0x13] = 0x456;
	raw[0x16] = lo0;
	raw[0x17] = 0x456;
	raw[0x1a] = lo1;
	raw[0x1b] = 0x456;
	raw[0x15] = 1ULL | (1ULL << 32);
	raw[0x19] = 1ULL | (1ULL << 32);
	raw[0xa] = 1;
	raw[0xe] = 1;
	raw[0x200 + 1] = 1;
	raw[0xc00] = COST_MAGIC;
	raw[0xc01] = 6;
	raw[0xc02] = 0;
	raw[0xc16] = 0;
	/* Hardware common window; V and A rates both divide by this. */
	raw[0xc19] = 1000000;
	raw[0xc11] = r->transfers;
	raw[0xc12] = r->transfers;
	/* Whole-session controller counters may exceed the window. */
	raw[0xc04] = 2000000;
	raw[0xc05] = 1500000;
	raw[0xc06] = 2000000;
	raw[0xc07] = 1500000;
	/* Window-scoped counters are bounded by raw[0xc19].  0xc1a/0xc1c are
	 * V-side window counters; 0xc1b/0xc1d are the A main/complete window
	 * counters and 0xc1f/0xc23 the A select/miss window counters used to
	 * bound the fullstats group totals. */
	raw[0xc1a] = 300000;
	raw[0xc1b] = 100000;
	raw[0xc1c] = 400000;
	raw[0xc1d] = 150000;
	raw[0xc1f] = 200000;
	raw[0xc23] = 120000;
	raw[0xc14] = 32;
	raw[0xc15] = 2;
	fill_fullstats(r, m);
	if (!strcmp(m, "magic"))
		raw[0] = 0;
	else if (!strcmp(m, "context"))
		raw[0x12] = (raw[0x12] & ~((0x123ULL) << 29)) | (0x124ULL << 29);
	else if (!strcmp(m, "timeout"))
		raw[8] = T + 1;
	else if (!strcmp(m, "count"))
		raw[0x25] = r->a_transfers + 1;
	else if (!strcmp(m, "overflow"))
		raw[0xc02] = 1;
	else if (!strcmp(m, "fp_miss")) {
		raw[0x15] = 0x0001;
		raw[0x19] = 0x0001;
	} else if (!strcmp(m, "fp_valid_miss")) {
		raw[0x15] = 0;
		raw[0x19] = 0;
		raw[0x16] = vpn0 | (0x123ULL << 29) | (1ULL << 51);
		raw[0x1a] = vpn1 | (0x123ULL << 29) | (1ULL << 51);
	}
}

static void fill_stats(struct idma_research *r)
{
	const char *m = mode();

	memset(r->phase_stats, 0, sizeof(r->phase_stats));
	r->phase_stats[0].submitted = 2;
	r->phase_stats[0].completed = 2;
	r->phase_stats[0].completed_bytes = 2ULL * 4096;
	r->phase_stats[1].submitted = r->a_transfers;
	r->phase_stats[1].completed = r->a_transfers;
	r->phase_stats[1].completed_bytes = (uint64_t)r->a_transfers * 4096;
	r->phase_stats[2].submitted = 1;
	r->phase_stats[2].completed = 1;
	r->phase_stats[2].completed_bytes = 4096;
	if (!strcmp(m, "count"))
		r->phase_stats[1].submitted = r->a_transfers + 1;
}

static void fill_p4m(struct idma_research *r)
{
	memset(&r->p4m, 0, sizeof(r->p4m));
	r->p4m.sequence = 1;
	r->p4m.observer_status = 4;
	r->p4m.window[0] = P4M_WINDOW_MAGIC;
	r->p4m.window[1] = 1000000;
	r->p4m.observation[0] = 1000000;
	r->p4m.window[13] = r->transfers;
	r->p4m.window[14] = r->transfers;
	if (r->a_pages) {
		r->p4m.window[15] = 0x3;
		r->p4m.window[2] = 10;
		/* A same-window completions used for the A rate. */
		r->p4m.window[3] = 1024;
		r->p4m.window[4] = 1014;
		r->p4m.window[5] = 0;
		r->p4m.experiment.aggressor_did = 11;
		r->p4m.experiment.aggressor.submitted = 10;
		r->p4m.experiment.aggressor.completed = 10;
		r->p4m.experiment.aggressor.completed_bytes = 10ULL * 4096;
	} else {
		r->p4m.window[15] = 0x2;
		r->p4m.experiment.aggressor_did = 0;
	}
	r->p4m.experiment.status = 0;
	r->p4m.experiment.victim_did = 10;
	r->p4m.experiment.victim_transfers = r->transfers;
	r->p4m.experiment.warmup_transfers = r->warmup;
	r->p4m.experiment.victim.submitted = r->transfers;
	r->p4m.experiment.victim.completed = r->transfers;
	r->p4m.experiment.victim.completed_bytes = (uint64_t)r->transfers * 4096;
}

static int handle_run(struct idma_research *r)
{
	const char *m = mode();
	uint64_t *raw;

	log_call("run");
	fill_raw(r);
	fill_stats(r);
	fill_p4m(r);
	raw = (uint64_t *)(uintptr_t)r->raw_user;
	r->observer_magic = OBS_MAGIC;
	r->raw_captured = IDMA_RESEARCH_RAW_WORDS;
	r->stage = 5;
	r->status = 0;
	r->flush_status = (r->mode == IDMA_RESEARCH_E2) ? 0 : -EINPROGRESS;
	if (!strcmp(m, "errno_mismatch")) {
		r->status = -EIO;
		errno = EPERM;
		return -1;
	}
	if (!strcmp(m, "rawmissing")) {
		r->raw_captured = 0;
		return 0;
	}
	if (!strcmp(m, "status")) {
		r->status = -EIO;
		errno = EIO;
		return -1;
	}
	if (!strcmp(m, "flush")) {
		r->flush_status = -EIO;
		return 0;
	}
	if (!strcmp(m, "stage")) {
		r->stage = 4;
		return 0;
	}
	if (!strcmp(m, "p4m_window_mismatch") && raw)
		raw[0xc19] = 1001;
	if (!strcmp(m, "p4m_magic"))
		r->p4m.window[0] = 0;
	return 0;
}

int ioctl(int fd, unsigned long request, ...)
{
	va_list ap;
	void *arg;

	(void)fd;
	va_start(ap, request);
	arg = va_arg(ap, void *);
	va_end(ap);
	if (request == IDMA_IOC_EXPERIMENT_CAPS)
		return handle_caps((struct idma_experiment_caps *)arg);
	if (request == IDMA_IOC_RUN_RESEARCH)
		return handle_run((struct idma_research *)arg);
	errno = ENOTTY;
	return -1;
}
