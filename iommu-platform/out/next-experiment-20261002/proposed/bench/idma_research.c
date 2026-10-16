#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/idma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define RESEARCH_OBSERVER_MAGIC 0x49444d414f425308ULL
#define RESEARCH_COST_MAGIC 0x49444d4152454331ULL
#define P4M_WINDOW_MAGIC 0x49444d4150344d01ULL

struct options {
	const char *device;
	int mode;
	int help;
	uint32_t a_pages, a_transfers, window_cycles, warmup, transfers, gap_cycles, timeout_ms;
	int a_pages_set, a_transfers_set, window_cycles_set, warmup_set, transfers_set, gap_cycles_set, timeout_ms_set;
};

static void usage(void)
{
	puts("Usage: idma_research --e2|--cost [--device PATH] [options]\n"
	     "  --e2                    Passive E2 entry-lifecycle research run\n"
	     "  --cost                  Natural-cost research run\n"
	     "  --device PATH           Device node (default /dev/idma0)\n"
	     "  --a-pages N             Aggressor pages: E2 0/1/4/7/8/16; cost 0/1/8/16\n"
	     "  --a-transfers N         Aggressor transfers (frozen by mode)\n"
	     "  --window-cycles N       E2 window cycles (frozen 5000000; cost 0)\n"
	     "  --warmup N              Warmup transfers (frozen by mode)\n"
	     "  --transfers N           Victim transfers (frozen by mode)\n"
	     "  --a-gap-cycles N        Cost aggressor gap (0/1000/2000, nonzero only A8)\n"
	     "  --timeout-ms N          Run timeout (frozen by mode)\n"
	     "  --help                  Show this help (text, not JSON)\n"
	     "All other arguments/values are rejected. One JSON record per run.");
}

static int parse_u32(const char *s, uint32_t *value)
{
	const unsigned char *p = (const unsigned char *)s;
	uint64_t n = 0;

	if (!*p)
		return -1;
	for (; *p; p++) {
		unsigned int digit;

		if (*p < '0' || *p > '9')
			return -1;
		digit = *p - '0';
		if (n > (UINT32_MAX - digit) / 10)
			return -1;
		n = n * 10 + digit;
	}
	*value = (uint32_t)n;
	return 0;
}

static int parse_args(int argc, char **argv, struct options *o, char *err, size_t errlen)
{
	static const char *const names[] = {
		"--device", "--a-pages", "--a-transfers", "--window-cycles",
		"--warmup", "--transfers", "--a-gap-cycles", "--timeout-ms",
		"--e2", "--cost", "--help",
	};
	unsigned int seen[sizeof(names) / sizeof(names[0])] = {0};
	int i;

	for (i = 1; i < argc; i++) {
		unsigned int opt;

		for (opt = 0; opt < sizeof(names) / sizeof(names[0]); opt++)
			if (!strcmp(argv[i], names[opt]))
				break;
		if (opt == sizeof(names) / sizeof(names[0])) {
			snprintf(err, errlen, "unknown argument: %s", argv[i]);
			return -1;
		}
		if (seen[opt]) {
			snprintf(err, errlen, "duplicate option: %s", names[opt]);
			return -1;
		}
		seen[opt] = 1;
		if (opt == 8) {
			if (seen[9]) {
				snprintf(err, errlen, "specify exactly one of --e2 or --cost");
				return -1;
			}
			o->mode = IDMA_RESEARCH_E2;
			continue;
		}
		if (opt == 9) {
			if (seen[8]) {
				snprintf(err, errlen, "specify exactly one of --e2 or --cost");
				return -1;
			}
			o->mode = IDMA_RESEARCH_COST;
			continue;
		}
		if (opt == 10) {
			o->help = 1;
			continue;
		}
		if (++i == argc) {
			snprintf(err, errlen, "missing value for %s", names[opt]);
			return -1;
		}
		if (opt == 0) {
			if (!*argv[i]) {
				snprintf(err, errlen, "device path is empty");
				return -1;
			}
			o->device = argv[i];
			continue;
		}
		{
			uint32_t num;

			if (parse_u32(argv[i], &num)) {
				snprintf(err, errlen, "invalid numeric value for %s: %s", names[opt], argv[i]);
				return -1;
			}
			switch (opt) {
			case 1: o->a_pages = num; o->a_pages_set = 1; break;
			case 2: o->a_transfers = num; o->a_transfers_set = 1; break;
			case 3: o->window_cycles = num; o->window_cycles_set = 1; break;
			case 4: o->warmup = num; o->warmup_set = 1; break;
			case 5: o->transfers = num; o->transfers_set = 1; break;
			case 6: o->gap_cycles = num; o->gap_cycles_set = 1; break;
			case 7: o->timeout_ms = num; o->timeout_ms_set = 1; break;
			}
		}
	}
	return 0;
}

static int validate_options(struct options *o, char *err, size_t errlen)
{
	if (!o->mode) {
		snprintf(err, errlen, "specify exactly one of --e2 or --cost");
		return -1;
	}
	if (o->mode == IDMA_RESEARCH_E2) {
		if (!o->a_pages_set) o->a_pages = 8;
		if (!o->a_transfers_set) o->a_transfers = o->a_pages ? 112 : 0;
		if (!o->window_cycles_set) o->window_cycles = 5000000;
		if (!o->warmup_set) o->warmup = 2;
		if (!o->transfers_set) o->transfers = 1;
		if (!o->gap_cycles_set) o->gap_cycles = 0;
		if (!o->timeout_ms_set) o->timeout_ms = 2000;
		if (o->a_pages != 0 && o->a_pages != 1 && o->a_pages != 4 &&
		    o->a_pages != 7 && o->a_pages != 8 && o->a_pages != 16) {
			snprintf(err, errlen, "invalid E2 --a-pages");
			return -1;
		}
		if (o->a_transfers != (o->a_pages ? 112 : 0)) {
			snprintf(err, errlen, "invalid E2 --a-transfers");
			return -1;
		}
		if (o->window_cycles != 5000000) {
			snprintf(err, errlen, "invalid E2 --window-cycles");
			return -1;
		}
		if (o->warmup != 2) {
			snprintf(err, errlen, "invalid E2 --warmup");
			return -1;
		}
		if (o->transfers != 1) {
			snprintf(err, errlen, "invalid E2 --transfers");
			return -1;
		}
		if (o->gap_cycles != 0) {
			snprintf(err, errlen, "invalid E2 --a-gap-cycles");
			return -1;
		}
		if (o->timeout_ms != 2000) {
			snprintf(err, errlen, "invalid E2 --timeout-ms");
			return -1;
		}
	} else {
		if (!o->a_pages_set) o->a_pages = 8;
		if (!o->a_transfers_set) o->a_transfers = 0;
		if (!o->window_cycles_set) o->window_cycles = 0;
		if (!o->gap_cycles_set) o->gap_cycles = 0;
		if (!o->transfers_set) o->transfers = 4096;
		if (!o->warmup_set) o->warmup = (o->transfers == 64) ? 8 : 256;
		if (!o->timeout_ms_set) o->timeout_ms = 10000;
		if (o->a_pages != 0 && o->a_pages != 1 && o->a_pages != 8 && o->a_pages != 16) {
			snprintf(err, errlen, "invalid cost --a-pages");
			return -1;
		}
		if (o->a_transfers != 0) {
			snprintf(err, errlen, "invalid cost --a-transfers");
			return -1;
		}
		if (o->window_cycles != 0) {
			snprintf(err, errlen, "invalid cost --window-cycles");
			return -1;
		}
		if (o->gap_cycles && (o->a_pages != 8 ||
		    (o->gap_cycles != 1000 && o->gap_cycles != 2000))) {
			snprintf(err, errlen, "invalid cost --a-gap-cycles");
			return -1;
		}
		if (!((o->transfers == 4096 && o->warmup == 256) ||
		      (o->transfers == 64 && o->warmup == 8))) {
			snprintf(err, errlen, "invalid cost --transfers/--warmup pair");
			return -1;
		}
		if (o->timeout_ms != 10000) {
			snprintf(err, errlen, "invalid cost --timeout-ms");
			return -1;
		}
	}
	return 0;
}

static int validate_caps(const struct idma_experiment_caps *c, char *err, size_t errlen)
{
	if (c->abi_version != IDMA_EXPERIMENT_ABI || c->struct_size != sizeof(*c)) {
		snprintf(err, errlen, "capabilities ABI/size mismatch");
		return -1;
	}
	if (!(c->capabilities & IDMA_EXPERIMENT_CAP_RESEARCH)) {
		snprintf(err, errlen, "driver lacks CAP_RESEARCH");
		return -1;
	}
	if (c->device_id != 0 || c->did != 10 || c->page_bytes != 4096) {
		snprintf(err, errlen, "capabilities context mismatch");
		return -1;
	}
	return 0;
}

static void json_string(const char *s)
{
	const unsigned char *p = (const unsigned char *)s;

	putchar('"');
	for (; *p; p++) {
		if (*p == '"' || *p == '\\')
			printf("\\%c", *p);
		else if (*p < 0x20)
			printf("\\u%04x", *p);
		else
			putchar(*p);
	}
	putchar('"');
}

static void json_hex64(uint64_t v)
{
	printf("\"0x%016" PRIx64 "\"", v);
}

static void print_stats(const struct idma_experiment_stats *s)
{
	printf("{\"submitted\":%" PRIu64 ",\"completed\":%" PRIu64
	       ",\"completed_bytes\":%" PRIu64 ",\"active_cycles\":%" PRIu64
	       ",\"iotlb_miss\":%" PRIu64 ",\"s1_ptw\":%" PRIu64
	       ",\"s2_ptw\":%" PRIu64 ",\"refill_gap_ns_sum\":%" PRIu64
	       ",\"refill_gap_ns_max\":%" PRIu64 ",\"refill_gap_samples\":%" PRIu64
	       ",\"in_flight_at_start\":%u,\"in_flight_at_end\":%u,\"data_errors\":%u,\"quarantined\":%u}",
	       (uint64_t)s->submitted, (uint64_t)s->completed, (uint64_t)s->completed_bytes,
	       (uint64_t)s->active_cycles, (uint64_t)s->iotlb_miss, (uint64_t)s->s1_ptw,
	       (uint64_t)s->s2_ptw, (uint64_t)s->refill_gap_ns_sum,
	       (uint64_t)s->refill_gap_ns_max, (uint64_t)s->refill_gap_samples,
	       s->in_flight_at_start, s->in_flight_at_end, s->data_errors, s->quarantined);
}

static void print_experiment(const struct idma_experiment *e)
{
	unsigned int i;

	printf("{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,\"aggressor_id\":%u,"
	       "\"victim_pages\":%u,\"aggressor_pages\":%u,\"chunk_bytes\":%u,"
	       "\"run_timeout_ms\":%u,\"victim_transfers\":%" PRIu64
	       ",\"warmup_transfers\":%" PRIu64 ",\"reserved\":[",
	       e->abi_version, e->struct_size, e->flags, e->aggressor_id,
	       e->victim_pages, e->aggressor_pages, e->chunk_bytes, e->run_timeout_ms,
	       (uint64_t)e->victim_transfers, (uint64_t)e->warmup_transfers);
	for (i = 0; i < 8; i++)
		printf("%s%u", i ? "," : "", e->reserved[i]);
	printf("],\"status\":%d,\"victim_did\":%u,\"aggressor_did\":%u,"
	       "\"hpm_configured\":%u,\"measurement_started\":%u,\"measurement_complete\":%u,"
	       "\"victim_elapsed_ns\":%" PRIu64 ",\"snapshot_before_ns\":%" PRIu64
	       ",\"snapshot_after_ns\":%" PRIu64 ",\"drain_ns\":%" PRIu64
	       ",\"victim\":",
	       e->status, e->victim_did, e->aggressor_did, e->hpm_configured,
	       e->measurement_started, e->measurement_complete,
	       (uint64_t)e->victim_elapsed_ns, (uint64_t)e->snapshot_before_ns,
	       (uint64_t)e->snapshot_after_ns, (uint64_t)e->drain_ns);
	print_stats(&e->victim);
	printf(",\"aggressor\":");
	print_stats(&e->aggressor);
	printf("}");
}

static void print_rates(const struct idma_research *r, const uint64_t *raw, int valid)
{
	double v_rate = 0.0, a_rate = 0.0, a_jobs = 0.0;
	int v_ok = 0, a_ok = 0;

	if (valid && r->mode == IDMA_RESEARCH_COST) {
		uint64_t v_done = raw[0xc12];
		uint64_t window = raw[0xc19];
		uint64_t a_done = r->p4m.window[3];

		/* V and A rates share the hardware common window raw[0xc19].  The
		 * whole-session controller counters raw[0xc04..0xc07] are not the
		 * window length. */
		if (window && v_done) {
			v_rate = (double)v_done * 4096.0 * 50000000.0 /
				 (double)window / (1024.0 * 1024.0);
			v_ok = 1;
		}
		if (window) {
			a_rate = (double)a_done * 4096.0 * 50000000.0 /
				 (double)window / (1024.0 * 1024.0);
			a_jobs = (double)a_done * 50000000.0 / (double)window;
			a_ok = 1;
		}
	}
	printf("{\"platform_hz\":50000000,\"cycle_ns\":20,\"v_mib_per_s\":");
	if (v_ok)
		printf("%.6f", v_rate);
	else
		printf("null");
	printf(",\"a_common_mib_per_s\":");
	if (a_ok)
		printf("%.6f", a_rate);
	else
		printf("null");
	printf(",\"a_common_jobs_per_s\":");
	if (a_ok)
		printf("%.6f", a_jobs);
	else
		printf("null");
	printf("}");
}

static void output_json(const struct options *o, const struct idma_research *r,
			const uint64_t *raw, int success, int validated,
			int syscall_rc, int saved_errno, const char *stage_name,
			const char *err)
{
	const char *mode = r->mode == IDMA_RESEARCH_E2 ? "e2" :
			   (r->mode == IDMA_RESEARCH_COST ? "cost" : "none");
	unsigned int i;

	printf("{\"schema\":\"idma-research-v1\",\"success\":%s,\"validated\":%s,"
	       "\"mode\":\"%s\",\"device\":",
	       success ? "true" : "false", validated ? "true" : "false", mode);
	json_string(o->device);
	printf(",\"syscall_rc\":%d,\"errno\":%d,\"status\":%d,\"flush_status\":%d,"
	       "\"stage\":%u,\"stage_name\":",
	       syscall_rc, saved_errno, (int)r->status, (int)r->flush_status, r->stage);
	json_string(stage_name);
	printf(",\"raw_captured\":%u,\"observer_magic\":", r->raw_captured);
	json_hex64((uint64_t)r->observer_magic);
	printf(",\"mapping\":[");
	for (i = 0; i < 4; i++) {
		if (i)
			putchar(',');
		json_hex64((uint64_t)r->mapping[i]);
	}
	printf("],\"input\":{");
	printf("\"abi_version\":%u,\"struct_size\":%u,\"mode\":%u,\"a_pages\":%u,"
	       "\"a_transfers\":%u,\"window_cycles\":%u,\"warmup\":%u,"
	       "\"transfers\":%u,\"gap_cycles\":%u,\"timeout_ms\":%u,\"raw_words\":%u",
	       r->abi_version, r->struct_size, r->mode, r->a_pages, r->a_transfers,
	       r->window_cycles, r->warmup, r->transfers, r->gap_cycles,
	       r->timeout_ms, r->raw_words);
	printf("},\"phase_stats\":[");
	for (i = 0; i < 3; i++) {
		if (i)
			putchar(',');
		print_stats(&r->phase_stats[i]);
	}
	printf("],\"p4m\":{\"aggressor_gap_cycles\":%u,\"sequence\":",
	       r->p4m.aggressor_gap_cycles);
	json_hex64((uint64_t)r->p4m.sequence);
	printf(",\"observer_status\":");
	json_hex64((uint64_t)r->p4m.observer_status);
	printf(",\"observation\":[");
	for (i = 0; i < IDMA_P4M_OBS_WORDS; i++) {
		if (i)
			putchar(',');
		json_hex64((uint64_t)r->p4m.observation[i]);
	}
	printf("],\"window\":[");
	for (i = 0; i < IDMA_P4M_WORDS; i++) {
		if (i)
			putchar(',');
		json_hex64((uint64_t)r->p4m.window[i]);
	}
	printf("],\"experiment\":");
	print_experiment(&r->p4m.experiment);
	printf("},\"raw\":[");
	for (i = 0; i < IDMA_RESEARCH_RAW_WORDS; i++) {
		if (i)
			putchar(',');
		json_hex64(raw[i]);
	}
	printf("],\"rates\":");
	print_rates(r, raw, success);
	printf(",\"errors\":[");
	if (*err)
		json_string(err);
	printf("]}");
}

static int entry_is_ordinary(uint64_t lo, uint64_t hi)
{
	(void)hi;
	if (!(lo & (1ULL << 51)))
		return 0;
	if (!(lo & (1ULL << 49)))
		return 0;
	if (lo & (1ULL << 50))
		return 0;
	if (lo & ((1ULL << 52) | (1ULL << 53) | (1ULL << 54) | (1ULL << 55) | (1ULL << 56)))
		return 0;
	return 1;
}

static uint64_t entry_vpn(uint64_t lo)
{
	return lo & ((1ULL << 29) - 1);
}

static uint64_t entry_pscid(uint64_t lo)
{
	return (lo >> 29) & ((1ULL << 20) - 1);
}

static uint64_t entry_gscid(uint64_t hi)
{
	return hi & 0xFFFF;
}

static int validate_e2_raw(const struct options *o, const struct idma_research *r,
			   const uint64_t *raw, char *err, size_t errlen)
{
	uint64_t T = r->window_cycles;
	uint64_t astart, deadline, a_counts = r->a_transfers;
	unsigned int i, events;

	(void)o;
	if (raw[0] != IDMA_RESEARCH_MAGIC) {
		snprintf(err, errlen, "E2 raw[0] magic mismatch");
		return -1;
	}
	if (raw[1] != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, errlen, "E2 raw[1] observer magic mismatch");
		return -1;
	}
	if (raw[4] != IDMA_RESEARCH_E2) {
		snprintf(err, errlen, "E2 raw[4] mode mismatch");
		return -1;
	}
	if (raw[6] != 4) {
		snprintf(err, errlen, "E2 phase is not 4");
		return -1;
	}
	if (raw[3] || raw[0xd] || raw[0xc] || raw[0xb]) {
		snprintf(err, errlen, "E2 error/overflow/inval counters nonzero");
		return -1;
	}
	if (!(raw[0x2] & (1ULL << 21)) || !(raw[0x2] & (1ULL << 22))) {
		snprintf(err, errlen, "E2 observer not quiet/supported");
		return -1;
	}
	if (raw[0x21] != 7) {
		snprintf(err, errlen, "E2 snapshot_valid is not 7");
		return -1;
	}
	if (raw[0x1c] != 3) {
		snprintf(err, errlen, "E2 target_valid is not 3");
		return -1;
	}
	if (raw[0x1d] != 3) {
		snprintf(err, errlen, "E2 firstprobe_done is not 3");
		return -1;
	}
	if (raw[0x25] != a_counts || raw[0x26] != a_counts) {
		snprintf(err, errlen, "E2 A issue/done count mismatch");
		return -1;
	}
	astart = raw[7];
	deadline = raw[0x1e];
	if (raw[8] != T) {
		snprintf(err, errlen, "E2 A_ELAPSED mismatch");
		return -1;
	}
	if (deadline != astart + T) {
		snprintf(err, errlen, "E2 deadline/astart mismatch");
		return -1;
	}
	if (raw[0x1f] < deadline) {
		snprintf(err, errlen, "E2 PROBESTART before deadline");
		return -1;
	}
	if (raw[0x20] < raw[0x14] || raw[0x20] < raw[0x18]) {
		snprintf(err, errlen, "E2 FREEZE before first probe");
		return -1;
	}
	if (!entry_is_ordinary(raw[0x10], raw[0x11]) ||
	    !entry_is_ordinary(raw[0x12], raw[0x13])) {
		snprintf(err, errlen, "E2 target not ordinary S1-only 4K");
		return -1;
	}
	if (entry_pscid(raw[0x10]) != entry_pscid(raw[0x12]) ||
	    entry_gscid(raw[0x11]) != entry_gscid(raw[0x13])) {
		snprintf(err, errlen, "E2 targets differ in PSCID/GSCID");
		return -1;
	}
	if (entry_vpn(raw[0x10]) != ((r->mapping[0] >> 12) & ((1ULL << 29) - 1)) ||
	    entry_vpn(raw[0x12]) != ((r->mapping[1] >> 12) & ((1ULL << 29) - 1))) {
		snprintf(err, errlen, "E2 target VPN does not match mapping");
		return -1;
	}
	/* RTL records the first-probe key as cap_key on a hit, or the
	 * synthesized S1-only lookup context on a miss (ctx_key_128 forces the
	 * valid bit), so the key is not necessarily an ordinary entry and a
	 * first-probe miss is a valid research outcome.  Only identity against
	 * the captured target is required. */
	if (entry_vpn(raw[0x16]) != entry_vpn(raw[0x10]) ||
	    entry_pscid(raw[0x16]) != entry_pscid(raw[0x10]) ||
	    entry_gscid(raw[0x17]) != entry_gscid(raw[0x11]) ||
	    entry_vpn(raw[0x1a]) != entry_vpn(raw[0x12]) ||
	    entry_pscid(raw[0x1a]) != entry_pscid(raw[0x12]) ||
	    entry_gscid(raw[0x1b]) != entry_gscid(raw[0x13])) {
		snprintf(err, errlen, "E2 firstprobe key identity mismatch");
		return -1;
	}
	if ((raw[0x15] & (1ULL << 32)) && !(raw[0x15] & 0xFFFF)) {
		snprintf(err, errlen, "E2 FP0 hit without live target");
		return -1;
	}
	if ((raw[0x19] & (1ULL << 32)) && !(raw[0x19] & 0xFFFF)) {
		snprintf(err, errlen, "E2 FP1 hit without live target");
		return -1;
	}
	if (r->phase_stats[0].submitted != 2 || r->phase_stats[0].completed != 2) {
		snprintf(err, errlen, "E2 warm stats mismatch");
		return -1;
	}
	if (r->phase_stats[1].submitted != a_counts || r->phase_stats[1].completed != a_counts) {
		snprintf(err, errlen, "E2 A stats mismatch");
		return -1;
	}
	if (r->phase_stats[2].submitted != 1 || r->phase_stats[2].completed != 1) {
		snprintf(err, errlen, "E2 probe stats mismatch");
		return -1;
	}
	for (i = 0; i < 3; i++) {
		if (r->phase_stats[i].completed_bytes != r->phase_stats[i].completed * 4096ULL) {
			snprintf(err, errlen, "E2 stats bytes mismatch");
			return -1;
		}
		if (r->phase_stats[i].data_errors || r->phase_stats[i].quarantined) {
			snprintf(err, errlen, "E2 stats errors/quarantine");
			return -1;
		}
	}
	events = (unsigned int)(raw[0xa] & 0xFFFF);
	if (events > 256) {
		snprintf(err, errlen, "E2 event count overflow");
		return -1;
	}
	if ((raw[0xe] & 0xFFFFFFFFULL) > 256) {
		snprintf(err, errlen, "E2 update count overflow");
		return -1;
	}
	for (i = 0; i < events; i++) {
		uint64_t meta = raw[0x200 + i * 6 + 1];
		uint64_t mask = meta & 0xFFFF;

		if (!mask || (mask & (mask - 1))) {
			snprintf(err, errlen, "E2 event slot mask not one-hot");
			return -1;
		}
	}
	return 0;
}

static int validate_cost_raw(const struct options *o, const struct idma_research *r,
			     const uint64_t *raw, char *err, size_t errlen)
{
	const struct idma_p4m *p = &r->p4m;
	uint64_t sum;
	unsigned int i;

	if (raw[0] != IDMA_RESEARCH_MAGIC) {
		snprintf(err, errlen, "cost raw[0] magic mismatch");
		return -1;
	}
	if (raw[1] != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, errlen, "cost raw[1] observer magic mismatch");
		return -1;
	}
	if (raw[3]) {
		snprintf(err, errlen, "cost error counter nonzero");
		return -1;
	}
	if (raw[4] != IDMA_RESEARCH_COST) {
		snprintf(err, errlen, "cost raw[4] mode mismatch");
		return -1;
	}
	if (raw[0xc00] != RESEARCH_COST_MAGIC) {
		snprintf(err, errlen, "cost raw[0xc00] magic mismatch");
		return -1;
	}
	if (raw[0xc01] != 6) {
		snprintf(err, errlen, "cost status is not 6");
		return -1;
	}
	if (raw[0xc02] != 0) {
		snprintf(err, errlen, "cost error nonzero");
		return -1;
	}
	if (raw[0xc16] != 0) {
		snprintf(err, errlen, "cost sample overflow");
		return -1;
	}
	if (!raw[0xc19] || raw[0xc19] != p->window[1] || raw[0xc19] != p->observation[0]) {
		snprintf(err, errlen, "cost window/cycle mismatch");
		return -1;
	}
	if (raw[0xc11] != r->transfers || raw[0xc12] != r->transfers) {
		snprintf(err, errlen, "cost V issue/done mismatch");
		return -1;
	}
	if (raw[0xc15] != raw[0xc14] / 16 || raw[0xc15] > 256) {
		snprintf(err, errlen, "cost sample count mismatch");
		return -1;
	}
	if (p->observer_status != 4) {
		snprintf(err, errlen, "P4M observer status is not 4");
		return -1;
	}
	if (!p->sequence) {
		snprintf(err, errlen, "P4M sequence is zero");
		return -1;
	}
	if (p->window[0] != P4M_WINDOW_MAGIC) {
		snprintf(err, errlen, "P4M window magic mismatch");
		return -1;
	}
	if (!(p->window[15] & 0x2)) {
		snprintf(err, errlen, "P4M window not closed");
		return -1;
	}
	if (p->window[15] & ~0x7ULL) {
		snprintf(err, errlen, "P4M window flags invalid");
		return -1;
	}
	if (p->experiment.status != 0) {
		snprintf(err, errlen, "P4M experiment status nonzero");
		return -1;
	}
	if (p->window[15] & 0x1) {
		if (p->window[4] + p->window[2] != p->window[3] + p->window[5]) {
			snprintf(err, errlen, "P4M conservation mismatch");
			return -1;
		}
	} else {
		for (i = 2; i <= 12; i++) {
			if (p->window[i]) {
				snprintf(err, errlen, "P4M Solo nonzero field");
				return -1;
			}
		}
	}
	/* Whole-session controller counters (raw[0xc04..0xc07]) may exceed the
	 * common window; only the window-scoped occupancy counters
	 * raw[0xc1a..0xc1d] are bounded by raw[0xc19].  At most one of the four
	 * advances per window cycle (one ctl lane in one of {main, comp}). */
	if (raw[0xc1a] > raw[0xc19] || raw[0xc1b] > raw[0xc19] ||
	    raw[0xc1c] > raw[0xc19] || raw[0xc1d] > raw[0xc19]) {
		snprintf(err, errlen, "cost window main/complete exceeds window");
		return -1;
	}
	sum = raw[0xc1a] + raw[0xc1b] + raw[0xc1c] + raw[0xc1d];
	if (sum < raw[0xc1a] || sum > raw[0xc19]) {
		snprintf(err, errlen, "cost window main/complete sum exceeds window");
		return -1;
	}
	if (p->experiment.victim.data_errors || p->experiment.victim.quarantined ||
	    (o->a_pages && (p->experiment.aggressor.data_errors ||
			    p->experiment.aggressor.quarantined))) {
		snprintf(err, errlen, "P4M kernel data errors/quarantine");
		return -1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	struct options o = { .device = "/dev/idma0" };
	struct idma_research req;
	uint64_t raw[IDMA_RESEARCH_RAW_WORDS] = {0};
	struct idma_experiment_caps caps;
	char err[256] = {0};
	const char *stage_name = "arguments";
	int fd = -1, rc = 0, saved_errno = 0;
	int success = 0, validated = 0;
	int parse_rc;

	memset(&req, 0, sizeof(req));
	req.abi_version = IDMA_RESEARCH_ABI;
	req.struct_size = sizeof(req);
	req.raw_words = IDMA_RESEARCH_RAW_WORDS;
	memset(&caps, 0, sizeof(caps));

	parse_rc = parse_args(argc, argv, &o, err, sizeof(err));
	if (parse_rc < 0) {
		rc = -1;
		output_json(&o, &req, raw, 0, 0, rc, 0, stage_name, err);
		fprintf(stderr, "idma_research: %s\n", err);
		return EXIT_FAILURE;
	}
	if (o.help) {
		usage();
		return EXIT_SUCCESS;
	}
	if (validate_options(&o, err, sizeof(err))) {
		rc = -1;
		output_json(&o, &req, raw, 0, 0, rc, 0, stage_name, err);
		fprintf(stderr, "idma_research: %s\n", err);
		return EXIT_FAILURE;
	}

	req.mode = o.mode;
	req.a_pages = o.a_pages;
	req.a_transfers = o.a_transfers;
	req.window_cycles = o.window_cycles;
	req.warmup = o.warmup;
	req.transfers = o.transfers;
	req.gap_cycles = o.gap_cycles;
	req.timeout_ms = o.timeout_ms;
	req.raw_words = IDMA_RESEARCH_RAW_WORDS;
	req.raw_user = (uint64_t)(uintptr_t)raw;

	stage_name = "open";
	fd = open(o.device, O_RDWR | O_CLOEXEC);
	if (fd < 0) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "open: %s", strerror(saved_errno));
		rc = -1;
		goto done;
	}
	stage_name = "capabilities";
	rc = ioctl(fd, IDMA_IOC_EXPERIMENT_CAPS, &caps);
	if (rc < 0) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "capabilities: %s", strerror(saved_errno));
		goto done;
	}
	if (validate_caps(&caps, err, sizeof(err))) {
		rc = 0;
		goto done;
	}
	validated = 1;
	stage_name = "run";
	rc = ioctl(fd, IDMA_IOC_RUN_RESEARCH, &req);
	saved_errno = rc < 0 ? errno : 0;
	if (rc < 0) {
		if (-(int)req.status != saved_errno) {
			snprintf(err, sizeof(err), "ioctl errno %d does not match status %d",
				 saved_errno, (int)req.status);
		} else {
			snprintf(err, sizeof(err), "run: %s", strerror(saved_errno));
		}
		goto done;
	}
	if (req.abi_version != IDMA_RESEARCH_ABI || req.struct_size != sizeof(req) ||
	    req.raw_words != IDMA_RESEARCH_RAW_WORDS) {
		snprintf(err, sizeof(err), "run ABI mismatch");
		goto done;
	}
	if (req.status != 0) {
		snprintf(err, sizeof(err), "run status %d", (int)req.status);
		goto done;
	}
	if (req.flush_status != 0 &&
	    !(req.mode == IDMA_RESEARCH_COST && req.flush_status == -EINPROGRESS)) {
		snprintf(err, sizeof(err), "flush status %d", (int)req.flush_status);
		goto done;
	}
	if (req.stage != 5) {
		snprintf(err, sizeof(err), "stage %u", req.stage);
		goto done;
	}
	if (req.raw_captured != IDMA_RESEARCH_RAW_WORDS) {
		snprintf(err, sizeof(err), "raw captured %u", req.raw_captured);
		goto done;
	}
	if (req.observer_magic != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, sizeof(err), "observer magic mismatch");
		goto done;
	}
	if (req.mode != (uint32_t)o.mode || req.a_pages != o.a_pages ||
	    req.a_transfers != o.a_transfers ||
	    req.window_cycles != o.window_cycles || req.warmup != o.warmup ||
	    req.transfers != o.transfers || req.gap_cycles != o.gap_cycles ||
	    req.timeout_ms != o.timeout_ms ||
	    req.raw_user != (uint64_t)(uintptr_t)raw) {
		snprintf(err, sizeof(err), "run request fields changed");
		goto done;
	}
	if (req.mode == IDMA_RESEARCH_E2) {
		if (validate_e2_raw(&o, &req, raw, err, sizeof(err))) {
			success = 0;
			goto done;
		}
	} else {
		if (validate_cost_raw(&o, &req, raw, err, sizeof(err))) {
			success = 0;
			goto done;
		}
	}
	success = 1;
	stage_name = "complete";
done:
	if (fd >= 0 && close(fd) < 0 && success) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "close: %s", strerror(saved_errno));
		success = 0;
		stage_name = "close";
	}
	if (*err)
		fprintf(stderr, "idma_research: %s\n", err);
	output_json(&o, &req, raw, success, validated, rc, saved_errno, stage_name, err);
	if (fflush(stdout) == EOF)
		return EXIT_FAILURE;
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
