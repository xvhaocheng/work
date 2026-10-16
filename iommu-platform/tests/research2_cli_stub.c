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
	raw[0xc19] = 1000000;
	raw[0xc11] = r->transfers;
	raw[0xc12] = r->transfers;
	raw[0xc04] = 2000000;
	raw[0xc05] = 1500000;
	raw[0xc06] = 2000000;
	raw[0xc07] = 1500000;
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

static void fill_v2_blocks(struct idma_research2 *r2, const char *m)
{
	uint64_t *raw = (uint64_t *)(uintptr_t)r2->base.raw_user;

	if (!raw)
		return;
	raw[0xf00] = IDMA_RESEARCH2_CAP_MAGIC;
	raw[0xf01] = 1;
	raw[0xf02] = r2->config.required_role;
	raw[0xf03] = (r2->config.required_role == IDMA_RESEARCH2_E2_ROLE) ? 0x7 : 0xf;
	raw[0xf10] = r2->config.reverse & 1;
	if (!strcmp(m, "caps_bad_magic"))
		raw[0xf00] = 0;
	else if (!strcmp(m, "caps_bad_version"))
		raw[0xf01] = 2;
	else if (!strcmp(m, "caps_bad_role"))
		raw[0xf02] = (r2->config.required_role == IDMA_RESEARCH2_E2_ROLE) ? 2 : 1;
	else if (!strcmp(m, "caps_bad_features"))
		raw[0xf03] = 0;
	else if (!strcmp(m, "f10_bad_reverse"))
		raw[0xf10] = (r2->config.reverse & 1) ^ 1;

	if (r2->config.required_role == IDMA_RESEARCH2_E2_ROLE) {
		raw[0xd80] = IDMA_RESEARCH2_E2_TIMING_MAGIC;
		raw[0xd81] = 1;
		raw[0xd82] = 0;
		raw[0xd83] = 7;
		raw[0xd84] = 1000;
		raw[0xd85] = 1005;
		raw[0xd86] = 1010;
		raw[0xd87] = raw[0xd86] - raw[0xd84];
		raw[0xd88] = raw[0xd85] - raw[0xd84];
		raw[0xd89] = 3;
		raw[0xd8a] = 4;
		raw[0xd8b] = 1;
		raw[0xd8c] = 1;
		raw[0xd8d] = 0;
		if (!strcmp(m, "timing_bad_magic"))
			raw[0xd80] = 0;
		else if (!strcmp(m, "timing_bad_version"))
			raw[0xd81] = 2;
		else if (!strcmp(m, "timing_error"))
			raw[0xd82] = 1;
		else if (!strcmp(m, "timing_bad_flags"))
			raw[0xd83] = 3;
		else if (!strcmp(m, "timing_bad_eq"))
			raw[0xd87] = 999;
	} else {
		const struct idma_research2_config *c = &r2->config;

		raw[0xe00] = IDMA_RESEARCH2_POLICY_MAGIC;
		raw[0xe01] = 1;
		raw[0xe02] = 0;
		raw[0xe03] = 0x2;
		raw[0xe04] = 0;
		if (!strcmp(m, "control_abort"))
			raw[0xe03] = 0x6;
		raw[0xe05] = c->policy_mode;
		raw[0xe06] = c->window_cycles;
		raw[0xe07] = c->target_bytes;
		raw[0xe08] = c->deadband_bytes;
		raw[0xe09] = c->initial_budget;
		raw[0xe0a] = c->min_budget;
		raw[0xe0b] = c->max_budget;
		raw[0xe0c] = c->budget_step;
		raw[0xe0d] = c->credit_cap;
		raw[0xe0e] = c->hit_weight;
		raw[0xe0f] = c->miss_weight;
		raw[0xe10] = c->ptw_weight;
		if (!strcmp(m, "policy_bad_magic"))
			raw[0xe00] = 0;
		else if (!strcmp(m, "policy_bad_version"))
			raw[0xe01] = 2;
		else if (!strcmp(m, "policy_error"))
			raw[0xe02] = 1;
		else if (!strcmp(m, "policy_not_frozen"))
			raw[0xe03] = 0;
		else if (!strcmp(m, "policy_echo_mismatch"))
			raw[0xe06] = c->window_cycles + 1;
	}
}

static int handle_run2(struct idma_research2 *r2)
{
	const char *m = mode();
	uint64_t *raw;

	log_call("run2");
	fill_raw(&r2->base);
	fill_stats(&r2->base);
	fill_p4m(&r2->base);
	raw = (uint64_t *)(uintptr_t)r2->base.raw_user;
	fill_v2_blocks(r2, m);
	if (!strcmp(m, "config_corrupt"))
		r2->config.window_cycles++;
	r2->base.observer_magic = OBS_MAGIC;
	r2->base.raw_captured = IDMA_RESEARCH_RAW_WORDS;
	r2->base.stage = 5;
	r2->base.status = 0;
	r2->base.flush_status = (r2->base.mode == IDMA_RESEARCH_E2) ? 0 : -EINPROGRESS;
	if (!strcmp(m, "overflow") && raw)
		raw[0xc02] = 1;
	if (!strcmp(m, "count") && raw)
		raw[0x25] = r2->base.a_transfers + 1;
	return 0;
}

static int handle_caps2(struct idma_research2_caps *c)
{
	const char *m = mode();
	const char *role = getenv("IDMA_TEST_CAPS2_ROLE");
	uint64_t r = role ? strtoull(role, NULL, 0) : IDMA_RESEARCH2_E2_ROLE;

	log_call("caps2");
	memset(c, 0, sizeof(*c));
	c->abi_version = IDMA_RESEARCH2_ABI;
	c->struct_size = sizeof(*c);
	c->magic = IDMA_RESEARCH2_CAP_MAGIC;
	c->version = 1;
	c->role = r;
	c->features = (r == IDMA_RESEARCH2_E2_ROLE) ? 0x7 : 0xf;
	c->observer_magic = OBS_MAGIC;
	if (!strcmp(m, "caps2_bad_magic"))
		c->magic = 0;
	else if (!strcmp(m, "caps2_bad_version"))
		c->version = 2;
	else if (!strcmp(m, "caps2_bad_role"))
		c->role = (r == IDMA_RESEARCH2_E2_ROLE) ? IDMA_RESEARCH2_METHOD_ROLE : IDMA_RESEARCH2_E2_ROLE;
	else if (!strcmp(m, "caps2_bad_features"))
		c->features = 0;
	else if (!strcmp(m, "caps2_bad_observer"))
		c->observer_magic = 0;
	else if (!strcmp(m, "caps2_enotty")) {
		errno = ENOTTY;
		return -1;
	}
	return 0;
}

static int handle_run(struct idma_research *r)
{
	const char *m = mode();

	log_call("run");
	fill_raw(r);
	fill_stats(r);
	fill_p4m(r);
	r->observer_magic = OBS_MAGIC;
	r->raw_captured = IDMA_RESEARCH_RAW_WORDS;
	r->stage = 5;
	r->status = 0;
	r->flush_status = (r->mode == IDMA_RESEARCH_E2) ? 0 : -EINPROGRESS;
	if (!strcmp(m, "overflow") && r->mode == IDMA_RESEARCH_COST) {
		uint64_t *raw = (uint64_t *)(uintptr_t)r->raw_user;

		if (raw)
			raw[0xc02] = 1;
	}
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
	if (request == IDMA_IOC_RUN_RESEARCH2)
		return handle_run2((struct idma_research2 *)arg);
	if (request == IDMA_IOC_RESEARCH2_CAPS)
		return handle_caps2((struct idma_research2_caps *)arg);
	errno = ENOTTY;
	return -1;
}
