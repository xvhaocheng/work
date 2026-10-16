// SPDX-License-Identifier: GPL-2.0
/*
 * idma_composition.c - one bounded same-BIT composition trial.
 *
 * Issues exactly one IDMA_IOC_RUN_COMPOSITION ioctl and prints one single-line
 * JSON record (schema idma-composition-v1).  It never re-runs a case, never
 * opens /dev/mem and never touches the research2 role interface.  A failure
 * still prints the JSON record and exits non-zero.
 *
 * Frozen composition ABI: four logical modes (0=S 1=N 2=P 3=PN), victim pages
 * 1, aggressor none or DMA instance 1 with 1/8/16 pages, 4096-byte chunks,
 * victim gap 0/1000/4000 cycles, aggressor gap 0.  protected_did is fixed to 10
 * and no other value can be requested from this tool.  Old HPM fields are
 * carried as raw observations only; they are not validated banked-mode metrics.
 */
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <linux/idma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

enum {
	OPT_DEVICE = 0,
	OPT_MODE,
	OPT_A_PAGES,
	OPT_V_GAP,
	OPT_TRANSFERS,
	OPT_WARMUP,
	OPT_TIMEOUT_MS,
	OPT_HELP,
	OPT_ABI,
	OPT_COUNT
};

static const char *const OPT_NAMES[OPT_COUNT] = {
	"--device", "--mode", "--a-pages", "--v-gap",
	"--transfers", "--warmup", "--timeout-ms", "--help", "--abi",
};

#define PROTECTED_DID       10u
#define AGGRESSOR_INSTANCE  1u
#define DEFAULT_CHUNK_BYTES 4096u
#define DEFAULT_TIMEOUT_MS  15000u
#define MIN_TRANSFERS       1u
#define MAX_TRANSFERS       4096u
#define MAX_WARMUP          4096u
#define TOOL_SCHEMA         "idma-composition-tool-v1"
#define RECORD_SCHEMA       "idma-composition-v1"

struct options {
	const char *device;
	uint32_t mode;
	uint32_t a_pages;
	uint32_t v_gap;
	uint64_t transfers;
	uint64_t warmup;
	uint32_t timeout_ms;
};

static void usage(void)
{
	puts("Usage: idma_composition [options]\n"
	     "  --device PATH       Victim device (default /dev/idma0)\n"
	     "  --mode N            Logical mode 0=S 1=N 2=P 3=PN (required)\n"
	     "  --a-pages N         Aggressor pages 0/1/8/16; 0 means Solo (required)\n"
	     "  --v-gap N           Victim gap cycles 0/1000/4000 (required)\n"
	     "  --transfers N       Victim measured transfers 1..4096 (required)\n"
	     "  --warmup N          Warmup transfers 0..4096 (required)\n"
	     "  --timeout-ms N      Run timeout (default 15000)\n"
	     "  --abi               Print the tool ABI as one JSON line, no hardware\n"
	     "  --help              Show this help\n"
	     "protected_did is fixed to 10; the aggressor is DMA instance 1 with DID 11.\n"
	     "One JSON record per run; a failure still prints the record and exits non-zero.");
}

static int parse_u64(const char *text, uint64_t limit, uint64_t *value)
{
	char *end = NULL;
	unsigned long long parsed;

	if (text == NULL || text[0] == '\0')
		return -1;
	/* Reject a sign or leading whitespace: strtoull would accept both. */
	if (text[0] < '0' || text[0] > '9')
		return -1;
	errno = 0;
	parsed = strtoull(text, &end, 10);
	if (errno != 0 || end == text || *end != '\0')
		return -1;
	if ((uint64_t)parsed > limit)
		return -1;
	*value = (uint64_t)parsed;
	return 0;
}

static int parse_args(int argc, char **argv, struct options *o, char *err, size_t size)
{
	unsigned int seen = 0;
	int help = 0, abi = 0, i;

	for (i = 1; i < argc; i++) {
		unsigned int opt;
		uint64_t number = 0;
		const char *value;

		for (opt = 0; opt < (unsigned int)OPT_COUNT; opt++)
			if (!strcmp(argv[i], OPT_NAMES[opt]))
				break;
		if (opt == (unsigned int)OPT_COUNT) {
			snprintf(err, size, "unknown argument: %s", argv[i]);
			return -1;
		}
		if (seen & (1u << opt)) {
			snprintf(err, size, "duplicate option: %s", OPT_NAMES[opt]);
			return -1;
		}
		seen |= 1u << opt;
		if (opt == (unsigned int)OPT_HELP) {
			help = 1;
			continue;
		}
		if (opt == (unsigned int)OPT_ABI) {
			abi = 1;
			continue;
		}
		if (++i == argc) {
			snprintf(err, size, "missing value for %s", OPT_NAMES[opt]);
			return -1;
		}
		value = argv[i];
		if (opt == (unsigned int)OPT_DEVICE) {
			if (!*value) {
				snprintf(err, size, "device path is empty");
				return -1;
			}
			o->device = value;
			continue;
		}
		if (parse_u64(value, UINT32_MAX, &number)) {
			snprintf(err, size, "invalid unsigned decimal value for %s",
				 OPT_NAMES[opt]);
			return -1;
		}
		switch (opt) {
		case OPT_MODE:
			o->mode = (uint32_t)number;
			break;
		case OPT_A_PAGES:
			o->a_pages = (uint32_t)number;
			break;
		case OPT_V_GAP:
			o->v_gap = (uint32_t)number;
			break;
		case OPT_TRANSFERS:
			o->transfers = number;
			break;
		case OPT_WARMUP:
			o->warmup = number;
			break;
		case OPT_TIMEOUT_MS:
			o->timeout_ms = (uint32_t)number;
			break;
		default:
			break;
		}
	}
	if (help)
		return 1;
	if (abi)
		return 2;
	if (!(seen & (1u << OPT_MODE)) || !(seen & (1u << OPT_A_PAGES)) ||
	    !(seen & (1u << OPT_V_GAP)) || !(seen & (1u << OPT_TRANSFERS)) ||
	    !(seen & (1u << OPT_WARMUP))) {
		snprintf(err, size,
			 "--mode, --a-pages, --v-gap, --transfers and --warmup are required");
		return -1;
	}
	if (o->mode > 3) {
		snprintf(err, size, "--mode must be 0..3");
		return -1;
	}
	if (!(o->a_pages == 0 || o->a_pages == 1 || o->a_pages == 8 || o->a_pages == 16)) {
		snprintf(err, size, "--a-pages must be 0, 1, 8 or 16");
		return -1;
	}
	if (!(o->v_gap == 0 || o->v_gap == 1000 || o->v_gap == 4000)) {
		snprintf(err, size, "--v-gap must be 0, 1000 or 4000");
		return -1;
	}
	if (o->transfers < MIN_TRANSFERS || o->transfers > MAX_TRANSFERS) {
		snprintf(err, size, "--transfers must be 1..%u", (unsigned int)MAX_TRANSFERS);
		return -1;
	}
	if (o->warmup > MAX_WARMUP) {
		snprintf(err, size, "--warmup must be 0..%u", (unsigned int)MAX_WARMUP);
		return -1;
	}
	if (o->timeout_ms == 0) {
		snprintf(err, size, "--timeout-ms must be positive");
		return -1;
	}
	return 0;
}

static void print_abi(void)
{
	printf("{\"schema\":\"%s\","
	       "\"tool\":\"idma_composition\","
	       "\"record_schema\":\"%s\","
	       "\"composition_abi\":%u,"
	       "\"composition_words\":%u,"
	       "\"composition_magic\":%" PRIu64 ","
	       "\"ioctl\":\"IDMA_IOC_RUN_COMPOSITION 0x11\","
	       "\"sizeof_idma_composition\":%zu,"
	       "\"sizeof_idma_p4m\":%zu,"
	       "\"sizeof_idma_experiment\":%zu,"
	       "\"sizeof_idma_experiment_stats\":%zu,"
	       "\"modes\":[0,1,2,3],"
	       "\"a_pages\":[0,1,8,16],"
	       "\"v_gap\":[0,1000,4000],"
	       "\"protected_did\":%u}\n",
	       TOOL_SCHEMA, RECORD_SCHEMA,
	       (unsigned int)IDMA_COMPOSITION_ABI,
	       (unsigned int)IDMA_COMPOSITION_WORDS,
	       (uint64_t)IDMA_COMPOSITION_MAGIC,
	       sizeof(struct idma_composition),
	       sizeof(struct idma_p4m),
	       sizeof(struct idma_experiment),
	       sizeof(struct idma_experiment_stats),
	       (unsigned int)PROTECTED_DID);
}

static void json_string(const char *text)
{
	const unsigned char *p = (const unsigned char *)text;

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

static void print_stats(const struct idma_experiment_stats *s)
{
	putchar('{');
	printf("\"submitted\":%" PRIu64 ",", (uint64_t)s->submitted);
	printf("\"completed\":%" PRIu64 ",", (uint64_t)s->completed);
	printf("\"completed_bytes\":%" PRIu64 ",", (uint64_t)s->completed_bytes);
	printf("\"active_cycles\":%" PRIu64 ",", (uint64_t)s->active_cycles);
	printf("\"iotlb_miss\":%" PRIu64 ",", (uint64_t)s->iotlb_miss);
	printf("\"s1_ptw\":%" PRIu64 ",", (uint64_t)s->s1_ptw);
	printf("\"s2_ptw\":%" PRIu64 ",", (uint64_t)s->s2_ptw);
	printf("\"refill_gap_ns_sum\":%" PRIu64 ",", (uint64_t)s->refill_gap_ns_sum);
	printf("\"refill_gap_ns_max\":%" PRIu64 ",", (uint64_t)s->refill_gap_ns_max);
	printf("\"refill_gap_samples\":%" PRIu64 ",", (uint64_t)s->refill_gap_samples);
	printf("\"in_flight_at_start\":%u,", s->in_flight_at_start);
	printf("\"in_flight_at_end\":%u,", s->in_flight_at_end);
	printf("\"data_errors\":%u,", s->data_errors);
	printf("\"quarantined\":%u,", s->quarantined);
	printf("\"hpm_field_semantics\":\"raw_only, not a validated banked-mode metric\"");
	putchar('}');
}

static void print_experiment(const struct idma_experiment *e, int has_aggressor)
{
	putchar('{');
	printf("\"abi_version\":%u,\"struct_size\":%u,\"status\":%d,",
	       (unsigned int)e->abi_version, (unsigned int)e->struct_size, e->status);
	printf("\"victim_did\":%u,", (unsigned int)e->victim_did);
	printf("\"aggressor_did\":");
	if (has_aggressor)
		printf("%u", (unsigned int)e->aggressor_did);
	else
		printf("null");
	printf(",\"hpm_configured\":%u,\"measurement_started\":%u,\"measurement_complete\":%u,",
	       (unsigned int)e->hpm_configured, (unsigned int)e->measurement_started,
	       (unsigned int)e->measurement_complete);
	printf("\"victim_elapsed_ns\":%" PRIu64 ",", (uint64_t)e->victim_elapsed_ns);
	printf("\"snapshot_before_ns\":%" PRIu64 ",", (uint64_t)e->snapshot_before_ns);
	printf("\"snapshot_after_ns\":%" PRIu64 ",", (uint64_t)e->snapshot_after_ns);
	printf("\"drain_ns\":%" PRIu64 ",", (uint64_t)e->drain_ns);
	printf("\"victim\":");
	print_stats(&e->victim);
	printf(",\"aggressor\":");
	if (has_aggressor)
		print_stats(&e->aggressor);
	else
		printf("null");
	putchar('}');
}

static void report(int argc, char **argv, const struct options *o,
		   const struct idma_composition *r, int have_result,
		   int attempted, const char *stage, const char *err,
		   int saved_errno, int success)
{
	int a;
	unsigned int i;

	printf("{\"schema\":\"%s\",\"success\":%s,\"stage\":", RECORD_SCHEMA,
	       success ? "true" : "false");
	json_string(stage);
	printf(",\"error\":");
	if (*err)
		json_string(err);
	else
		printf("null");
	printf(",\"errno\":%d,\"run_attempted\":%s,\"argv\":[", saved_errno,
	       attempted ? "true" : "false");
	for (a = 0; a < argc; a++) {
		if (a)
			putchar(',');
		json_string(argv[a]);
	}
	printf("],\"request\":{\"device\":");
	json_string(o->device);
	printf(",\"abi_version\":%u,\"struct_size\":%u,\"mode\":%u,\"protected_did\":%u,",
	       (unsigned int)IDMA_COMPOSITION_ABI,
	       (unsigned int)sizeof(struct idma_composition),
	       (unsigned int)o->mode, (unsigned int)PROTECTED_DID);
	printf("\"victim_gap_cycles\":%u,\"a_pages\":%u,\"aggressor_id\":",
	       (unsigned int)o->v_gap, (unsigned int)o->a_pages);
	if (o->a_pages)
		printf("%u", (unsigned int)AGGRESSOR_INSTANCE);
	else
		printf("null");
	printf(",\"victim_pages\":1,\"chunk_bytes\":%u,\"run_timeout_ms\":%u,",
	       (unsigned int)DEFAULT_CHUNK_BYTES, (unsigned int)o->timeout_ms);
	printf("\"victim_transfers\":%" PRIu64 ",\"warmup_transfers\":%" PRIu64 "}",
	       o->transfers, o->warmup);

	printf(",\"result\":");
	if (!have_result) {
		printf("null");
	} else {
		printf("{\"abi_version\":%u,\"struct_size\":%u,\"status\":%d,"
		       "\"restore_status\":%d,\"configured\":%u,\"raw_captured\":%u,",
		       (unsigned int)r->abi_version, (unsigned int)r->struct_size,
		       r->status, r->restore_status,
		       (unsigned int)r->configured, (unsigned int)r->raw_captured);
		printf("\"generation\":%" PRIu64 ",\"restored_generation\":%" PRIu64 ",",
		       (uint64_t)r->generation, (uint64_t)r->restored_generation);
		printf("\"experiment\":");
		print_experiment(&r->p4m.experiment, o->a_pages != 0);
		printf(",\"p4m\":{\"aggressor_gap_cycles\":%u,\"sequence\":%" PRIu64
		       ",\"observer_status\":%" PRIu64 ",\"observation\":[",
		       (unsigned int)r->p4m.aggressor_gap_cycles,
		       (uint64_t)r->p4m.sequence,
		       (uint64_t)r->p4m.observer_status);
		for (i = 0; i < IDMA_P4M_OBS_WORDS; i++) {
			if (i)
				putchar(',');
			printf("%" PRIu64, (uint64_t)r->p4m.observation[i]);
		}
		printf("],\"window\":[");
		for (i = 0; i < IDMA_P4M_WORDS; i++) {
			if (i)
				putchar(',');
			printf("%" PRIu64, (uint64_t)r->p4m.window[i]);
		}
		printf("]},\"raw\":[");
		for (i = 0; i < IDMA_COMPOSITION_WORDS; i++) {
			if (i)
				putchar(',');
			printf("%" PRIu64, (uint64_t)r->raw[i]);
		}
		printf("]}");
	}
	printf(",\"semantics\":{"
	       "\"record\":\"one bounded composition trial; no rerun\","
	       "\"window\":\"(open,close] hardware cycles at the fixed 50 MHz platform assumption\","
	       "\"p4m\":\"shared observer window; observation and window words are raw\","
	       "\"hpm\":\"old HPM fields are raw_only, not validated banked-mode metrics\","
	       "\"protected_did\":\"fixed 10\","
	       "\"aggressor_gap\":\"fixed 0\","
	       "\"calibration\":\"unverified\"}}\n");
}

static void build_request(const struct options *o, struct idma_composition *r)
{
	memset(r, 0, sizeof(*r));
	r->abi_version = IDMA_COMPOSITION_ABI;
	r->struct_size = (__u32)sizeof(*r);
	r->mode = o->mode;
	r->protected_did = PROTECTED_DID;
	r->victim_gap_cycles = o->v_gap;

	r->p4m.experiment.abi_version = IDMA_EXPERIMENT_ABI;
	r->p4m.experiment.struct_size = (__u32)sizeof(struct idma_experiment);
	r->p4m.experiment.flags = 0;
	r->p4m.experiment.aggressor_id = o->a_pages ? AGGRESSOR_INSTANCE
						    : IDMA_EXPERIMENT_NO_AGGRESSOR;
	r->p4m.experiment.victim_pages = 1;
	/* Solo keeps a nonzero page count; the kernel ignores it without an aggressor. */
	r->p4m.experiment.aggressor_pages = o->a_pages ? o->a_pages : 1;
	r->p4m.experiment.chunk_bytes = DEFAULT_CHUNK_BYTES;
	r->p4m.experiment.run_timeout_ms = o->timeout_ms;
	r->p4m.experiment.victim_transfers = o->transfers;
	r->p4m.experiment.warmup_transfers = o->warmup;
	r->p4m.aggressor_gap_cycles = 0;
}

static int validate_result(const struct options *o, const struct idma_composition *r,
			   char *err, size_t size)
{
	if (r->abi_version != IDMA_COMPOSITION_ABI ||
	    r->struct_size != (__u32)sizeof(*r)) {
		snprintf(err, size, "composition ABI version or structure size mismatch");
		return -1;
	}
	if (r->status != 0) {
		snprintf(err, size, "composition status %d", r->status);
		return -1;
	}
	if (r->restore_status != 0) {
		snprintf(err, size, "restore_status %d", r->restore_status);
		return -1;
	}
	if (r->configured != 1) {
		snprintf(err, size, "configured %u", r->configured);
		return -1;
	}
	if (r->raw_captured != 1) {
		snprintf(err, size, "raw_captured %u", r->raw_captured);
		return -1;
	}
	if (r->raw[0] != IDMA_COMPOSITION_MAGIC) {
		snprintf(err, size, "raw bank magic mismatch");
		return -1;
	}
	if (r->p4m.sequence == 0) {
		snprintf(err, size, "p4m sequence is zero");
		return -1;
	}
	if (r->p4m.experiment.victim.data_errors || r->p4m.experiment.victim.quarantined) {
		snprintf(err, size, "victim data errors or quarantined DMA");
		return -1;
	}
	if (o->a_pages && (r->p4m.experiment.aggressor.data_errors ||
			   r->p4m.experiment.aggressor.quarantined)) {
		snprintf(err, size, "aggressor data errors or quarantined DMA");
		return -1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	struct options o;
	struct idma_composition req;
	struct idma_composition result;
	char err[256] = "";
	const char *stage = "arguments";
	int fd = -1, saved_errno = 0;
	int have_result = 0, attempted = 0, success = 0, rc;

	memset(&o, 0, sizeof(o));
	memset(&req, 0, sizeof(req));
	memset(&result, 0, sizeof(result));
	o.device = "/dev/idma0";
	o.timeout_ms = DEFAULT_TIMEOUT_MS;

	rc = parse_args(argc, argv, &o, err, sizeof(err));
	if (rc == 1) {
		usage();
		return EXIT_SUCCESS;
	}
	if (rc == 2) {
		print_abi();
		return EXIT_SUCCESS;
	}
	if (rc < 0)
		goto done;

	build_request(&o, &req);

	stage = "open";
	fd = open(o.device, O_RDWR | O_CLOEXEC);
	if (fd < 0)
		goto syscall_error;

	stage = "composition";
	attempted = 1;
	result = req;
	rc = ioctl(fd, IDMA_IOC_RUN_COMPOSITION, &result);
	saved_errno = rc < 0 ? errno : 0;
	have_result = rc >= 0 || result.status < 0;
	if (rc < 0) {
		snprintf(err, sizeof(err), "composition ioctl: %s", strerror(saved_errno));
		goto done;
	}
	if (validate_result(&o, &result, err, sizeof(err)))
		goto done;
	success = 1;
	goto done;

syscall_error:
	saved_errno = errno;
	snprintf(err, sizeof(err), "%s: %s", stage, strerror(saved_errno));
done:
	if (fd >= 0 && close(fd) < 0 && success) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "close: %s", strerror(saved_errno));
		stage = "close";
		success = 0;
	}
	if (*err)
		fprintf(stderr, "idma_composition: %s\n", err);
	report(argc, argv, &o, &result, have_result, attempted, stage, err,
	       saved_errno, success);
	if (fflush(stdout) == EOF)
		return EXIT_FAILURE;
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
