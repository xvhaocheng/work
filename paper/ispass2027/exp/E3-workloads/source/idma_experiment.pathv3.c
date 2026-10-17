// SPDX-License-Identifier: GPL-2.0
/* One explicitly bounded trial. This program does not establish calibration. */
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

struct options {
	const char *device;
	int caps_only, batch;
	struct idma_experiment request;
};

static void usage(void)
{
	puts("Usage: idma_experiment [options]\n"
	     "  --device PATH          Victim device (default /dev/idma0)\n"
	     "  --aggressor none|ID    Background DMA instance (default none)\n"
	     "  --victim-pages N       Source and destination pages each (default 1)\n"
	     "  --aggressor-pages N    Source and destination pages each (default 8)\n"
	     "  --chunk N              Bytes per transfer (default 4096)\n"
	     "  --transfers N          Victim measured transfers (default 4096)\n"
	     "  --warmup N             Warmup transfers (default 256; 0 disables)\n"
	     "  --timeout-ms N         Run timeout (default 60000)\n"
	     "  --batch                Hardware batch copies; requires batch BIT/kernel\n"
	     "  --caps                 Query capabilities without running a trial\n"
	     "  --help                 Show this help\n"
	     "All numbers are unsigned decimal integers. One JSON record per trial.\n"
	     "Timing includes software submission/polling. HPM support is not calibration.");
}

static int decimal(const char *s, uint64_t limit, uint64_t *value)
{
	uint64_t n = 0;
	const unsigned char *p = (const unsigned char *)s;

	if (!*p)
		return -1;
	for (; *p; p++) {
		unsigned int digit;

		if (*p < '0' || *p > '9')
			return -1;
		digit = *p - '0';
		if (n > limit / 10 || (n == limit / 10 && digit > limit % 10))
			return -1;
		n = n * 10 + digit;
	}
	*value = n;
	return 0;
}

static int parse(int argc, char **argv, struct options *o, char *err, size_t size)
{
	static const char *const names[] = {
		"--device", "--aggressor", "--victim-pages", "--aggressor-pages",
		"--chunk", "--transfers", "--warmup", "--timeout-ms", "--caps", "--batch",
	};
	unsigned int seen = 0;
	int i, help = 0;

	for (i = 1; i < argc; i++) {
		unsigned int opt;
		uint64_t number = 0;
		const char *value;

		if (!strcmp(argv[i], "--help")) {
			if (help) {
				snprintf(err, size, "duplicate option: --help");
				return -1;
			}
			help = 1;
			continue;
		}
		for (opt = 0; opt < sizeof(names) / sizeof(names[0]); opt++)
			if (!strcmp(argv[i], names[opt]))
				break;
		if (opt == sizeof(names) / sizeof(names[0])) {
			snprintf(err, size, "unknown argument: %s", argv[i]);
			return -1;
		}
		if (seen & (1U << opt)) {
			snprintf(err, size, "duplicate option: %s", names[opt]);
			return -1;
		}
		seen |= 1U << opt;
		if (opt == 8 || opt == 9) {
			if (opt == 8) o->caps_only = 1;
			else o->batch = 1;
			continue;
		}
		if (++i == argc) {
			snprintf(err, size, "missing value for %s", names[opt]);
			return -1;
		}
		value = argv[i];
		if (opt == 0) {
			if (!*value) {
				snprintf(err, size, "device path is empty");
				return -1;
			}
			o->device = value;
			continue;
		}
		if (opt == 1 && !strcmp(value, "none")) {
			o->request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
			continue;
		}
		if (decimal(value, opt == 5 || opt == 6 ? UINT64_MAX : UINT32_MAX,
			    &number) || (opt == 1 && number == IDMA_EXPERIMENT_NO_AGGRESSOR)) {
			snprintf(err, size, "invalid unsigned decimal value for %s", names[opt]);
			return -1;
		}
		switch (opt) {
		case 1: o->request.aggressor_id = (uint32_t)number; break;
		case 2: o->request.victim_pages = (uint32_t)number; break;
		case 3: o->request.aggressor_pages = (uint32_t)number; break;
		case 4: o->request.chunk_bytes = (uint32_t)number; break;
		case 5: o->request.victim_transfers = number; break;
		case 6: o->request.warmup_transfers = number; break;
		case 7: o->request.run_timeout_ms = (uint32_t)number; break;
		}
	}
	if (!o->request.victim_pages || !o->request.aggressor_pages ||
	    !o->request.chunk_bytes || !o->request.victim_transfers ||
	    !o->request.run_timeout_ms) {
		snprintf(err, size, "pages, chunk, transfers and timeout must be positive");
		return -1;
	}
	if (o->request.victim_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    o->request.warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    o->request.victim_transfers > UINT64_MAX / o->request.chunk_bytes) {
		snprintf(err, size, "transfer count exceeds ABI limit or byte count overflows");
		return -1;
	}
	if (o->batch && (o->request.chunk_bytes != 4096 ||
			o->request.victim_transfers > IDMA_BATCH_MAX_TRANSFERS)) {
		snprintf(err, size, "batch requires 4096-byte copies and at most 1048576 tasks");
		return -1;
	}
	if (help) {
		usage();
		return 1;
	}
	return 0;
}

static int validate_caps(const struct options *o,
			 const struct idma_experiment_caps *c, char *err, size_t size)
{
	const struct idma_experiment *r = &o->request;
	unsigned int i;
	uint32_t required = IDMA_EXPERIMENT_CAP_DID_HPM |
		(o->batch ? IDMA_EXPERIMENT_CAP_HW_BATCH : IDMA_EXPERIMENT_CAP_SW_REFILL);

	if (c->abi_version != IDMA_EXPERIMENT_ABI || c->struct_size != sizeof(*c)) {
		snprintf(err, size, "capability ABI version or structure size mismatch");
		return -1;
	}
	for (i = 0; i < sizeof(c->reserved) / sizeof(c->reserved[0]); i++) {
		if (c->reserved[i]) {
			snprintf(err, size, "capability reserved fields are not zero");
			return -1;
		}
	}
	if (c->page_bytes != 4096 || !c->max_pages || !c->max_run_timeout_ms) {
		snprintf(err, size, "invalid or unsupported capability limits");
		return -1;
	}
	if (o->caps_only)
		return 0;
	if ((c->capabilities & required) != required) {
		snprintf(err, size, "driver lacks required DID HPM or selected supply-mode support");
		return -1;
	}
	if (r->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR &&
	    r->aggressor_id == c->device_id) {
		snprintf(err, size, "Victim and Aggressor must be different DMA instances");
		return -1;
	}
	if (r->victim_pages > c->max_pages || r->aggressor_pages > c->max_pages ||
	    r->chunk_bytes > c->page_bytes || c->page_bytes % r->chunk_bytes ||
	    r->run_timeout_ms > c->max_run_timeout_ms) {
		snprintf(err, size, "pages, chunk or timeout exceed reported capability limits");
		return -1;
	}
	return 0;
}

/* Escape argv and OS messages; they must never corrupt a result record. */
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

static void print_u64(const char *name, uint64_t value)
{
	printf("\"%s\":%" PRIu64, name, value);
}

static void print_stats(const struct idma_experiment_stats *s, int hpm, int started, int batch)
{
	putchar('{');
	if (started) {
		print_u64("submitted", s->submitted);
		putchar(','); print_u64("completed", s->completed);
		putchar(','); print_u64("completed_bytes", s->completed_bytes);
		putchar(','); print_u64("active_cycles", s->active_cycles);
	} else {
		printf("\"submitted\":null,\"completed\":null,\"completed_bytes\":null,"
		       "\"active_cycles\":null");
	}
	if (hpm && started) {
		putchar(','); print_u64("iotlb_miss", s->iotlb_miss);
		putchar(','); print_u64("s1_ptw", s->s1_ptw);
		putchar(','); print_u64("s2_ptw", s->s2_ptw);
	} else {
		printf(",\"iotlb_miss\":null,\"s1_ptw\":null,\"s2_ptw\":null");
	}
	if (started && !batch) {
		putchar(','); print_u64("refill_gap_ns_sum", s->refill_gap_ns_sum);
		putchar(','); print_u64("refill_gap_ns_max", s->refill_gap_ns_max);
		putchar(','); print_u64("refill_gap_samples", s->refill_gap_samples);
		printf(",\"in_flight_at_start\":%u,\"in_flight_at_end\":%u",
		       s->in_flight_at_start, s->in_flight_at_end);
	} else {
		printf(",\"refill_gap_ns_sum\":null,\"refill_gap_ns_max\":null,"
		       "\"refill_gap_samples\":null,\"in_flight_at_start\":null,"
		       "\"in_flight_at_end\":null");
	}
	printf(",\"data_errors\":%u,\"quarantined\":%u}", s->data_errors, s->quarantined);
}

static void report(int argc, char **argv, const struct options *o,
		   const struct idma_experiment_caps *c, int have_caps,
		   const struct idma_experiment *r, int have_result, int attempted,
		   const char *stage, const char *err, int saved_errno, int success)
{
	int i;

	printf("{\"schema\":\"%s\",\"success\":%s,\"stage\":",
	       o->batch ? "idma.batch.v1" : "idma.experiment.v1", success ? "true" : "false");
	json_string(stage);
	printf(",\"error\":");
	if (*err) json_string(err); else printf("null");
	printf(",\"errno\":%d,\"run_attempted\":%s,\"argv\":[", saved_errno,
	       attempted ? "true" : "false");
	for (i = 0; i < argc; i++) {
		if (i) putchar(',');
		json_string(argv[i]);
	}
	printf("],\"input\":{\"device\":");
	json_string(o->device);
	printf(",\"caps_only\":%s,\"abi_version\":%u,\"struct_size\":%u,"
	       "\"flags\":%u,\"aggressor_id\":", o->caps_only ? "true" : "false",
	       o->request.abi_version, o->request.struct_size, o->request.flags);
	if (o->request.aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) printf("null");
	else printf("%u", o->request.aggressor_id);
	printf(",\"victim_pages\":%u,\"aggressor_pages\":%u,\"chunk_bytes\":%u,"
	       "\"run_timeout_ms\":%u,", o->request.victim_pages,
	       o->request.aggressor_pages, o->request.chunk_bytes, o->request.run_timeout_ms);
	print_u64("victim_transfers", o->request.victim_transfers);
	putchar(','); print_u64("warmup_transfers", o->request.warmup_transfers);
	printf(",\"reserved\":[0,0,0,0,0,0,0,0]},\"capabilities\":");
	if (have_caps) {
		printf("{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,"
		       "\"device_id\":%u,\"did\":%u,\"max_pages\":%u,\"page_bytes\":%u,"
		       "\"max_run_timeout_ms\":%u,\"did_hpm_supported\":%s,"
		       "\"software_refill_supported\":%s,\"reserved\":[",
		       c->abi_version, c->struct_size, c->capabilities, c->device_id,
		       c->did, c->max_pages, c->page_bytes, c->max_run_timeout_ms,
		       c->capabilities & IDMA_EXPERIMENT_CAP_DID_HPM ? "true" : "false",
		       c->capabilities & IDMA_EXPERIMENT_CAP_SW_REFILL ? "true" : "false");
		for (i = 0; i < 8; i++) printf("%s%u", i ? "," : "", c->reserved[i]);
		putchar(']'); putchar('}');
	} else printf("null");
	printf(",\"result\":");
	if (have_result) {
		printf("{\"abi_version\":%u,\"struct_size\":%u,\"status\":%d,"
		       "\"victim_did\":%u,\"aggressor_did\":", r->abi_version,
		       r->struct_size, r->status, r->victim_did);
		if (o->request.aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) printf("null");
		else printf("%u", r->aggressor_did);
		printf(",\"hpm_configured\":%u,\"measurement_started\":%u,"
		       "\"measurement_complete\":%u,", r->hpm_configured,
		       r->measurement_started, r->measurement_complete);
		if (r->measurement_started) {
			print_u64("victim_elapsed_ns", r->victim_elapsed_ns);
			putchar(','); print_u64("snapshot_before_ns", r->snapshot_before_ns);
			putchar(','); print_u64("snapshot_after_ns", r->snapshot_after_ns);
		} else {
			printf("\"victim_elapsed_ns\":null,\"snapshot_before_ns\":null,"
			       "\"snapshot_after_ns\":null");
		}
		putchar(','); print_u64("drain_ns", r->drain_ns);
		printf(",\"victim\":");
		print_stats(&r->victim, r->hpm_configured, r->measurement_started, o->batch);
		printf(",\"aggressor\":");
		if (o->request.aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) printf("null");
		else print_stats(&r->aggressor, r->hpm_configured, r->measurement_started, o->batch);
		putchar('}');
	} else printf("null");
	if (o->batch) {
		printf(",\"semantics\":{\"supply\":\"hardware batch, software polls batch completion\","
		       "\"victim_elapsed_ns\":\"software window includes A/V launch and final polling; hardware window is in idma_batch_obs\","
		       "\"task_counts\":\"hardware issued/completed after drain\","
		       "\"aggressor_completed\":\"whole A batch including startup and final drain; not V-window throughput\","
		       "\"refill_gap_ns\":\"not sampled in batch mode\","
		       "\"request_overlap\":\"see matched idma_batch_obs AXI records\","
		       "\"counter_window\":\"HPM brackets software window and is not atomic\","
		       "\"warmup\":\"legacy software warmup then drain before batch; measured offsets restart at zero\","
		       "\"calibration\":\"unverified\"}}\n");
		return;
	}
	printf(",\"semantics\":{"
	       "\"victim_elapsed_ns\":\"software-observed task duration including submission and polling\","
	       "\"hpm_configured\":\"driver configuration status; not board calibration\","
	       "\"snapshot_before_ns\":\"duration of counter reads immediately before Victim window\","
	       "\"snapshot_after_ns\":\"duration of counter reads immediately after Victim window\","
	       "\"counter_window\":\"snapshots bracket Victim window and are not atomic\","
	       "\"aggressor_completed\":\"software-observed completions; transfers may cross window boundaries\","
	       "\"refill_gap_ns\":\"observed completion to observed resubmission including MMIO; not hardware idle time\","
	       "\"request_overlap\":\"unavailable: software activity does not prove concurrent hardware requests\","
	       "\"calibration\":\"unverified\"}}\n");
}

int main(int argc, char **argv)
{
	struct options o = {
		.device = "/dev/idma0",
		.request = {
			.abi_version = IDMA_EXPERIMENT_ABI,
			.struct_size = sizeof(struct idma_experiment),
			.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR,
			.victim_pages = 1,
			.aggressor_pages = 8,
			.chunk_bytes = 4096,
			.run_timeout_ms = 60000,
			.victim_transfers = 4096,
			.warmup_transfers = 256,
		},
	};
	struct idma_experiment_caps caps = {0};
	struct idma_experiment result = {0};
	char err[256] = "";
	const char *stage = "arguments";
	int fd = -1, saved_errno = 0, have_caps = 0, have_result = 0;
	int attempted = 0, success = 0, rc;

	rc = parse(argc, argv, &o, err, sizeof(err));
	if (rc > 0)
		return EXIT_SUCCESS;
	if (rc < 0)
		goto done;
	stage = "open";
	fd = open(o.device, O_RDWR | O_CLOEXEC);
	if (fd < 0)
		goto syscall_error;
	stage = "capabilities";
	rc = ioctl(fd, IDMA_IOC_EXPERIMENT_CAPS, &caps);
	if (rc < 0)
		goto syscall_error;
	have_caps = 1;
	if (rc || validate_caps(&o, &caps, err, sizeof(err))) {
		if (rc) snprintf(err, sizeof(err), "unexpected capability ioctl return value");
		goto done;
	}
	if (o.caps_only) {
		success = 1;
		goto done;
	}
	stage = "run";
	result = o.request;
	attempted = 1;
	/* Never retry a run ioctl on EINTR: it might already have submitted DMA. */
	rc = ioctl(fd, o.batch ? IDMA_IOC_RUN_BATCH : IDMA_IOC_RUN_EXPERIMENT, &result);
	saved_errno = rc < 0 ? errno : 0;
	have_result = rc >= 0 || result.status < 0;
	if (rc < 0) {
		snprintf(err, sizeof(err), "run ioctl: %s", strerror(saved_errno));
		goto done;
	}
	if (rc || result.abi_version != IDMA_EXPERIMENT_ABI ||
	    result.struct_size != sizeof(result)) {
		snprintf(err, sizeof(err), "unexpected run ioctl return value or ABI mismatch");
		goto done;
	}
	if (result.status || result.victim.data_errors || result.aggressor.data_errors ||
	    result.victim.quarantined || result.aggressor.quarantined) {
		snprintf(err, sizeof(err), "trial failed: status, data errors or quarantined DMA");
		goto done;
	}
	if (result.hpm_configured != 1 || result.measurement_started != 1 ||
	    result.measurement_complete != 1 || !result.victim_elapsed_ns ||
	    result.victim.completed != o.request.victim_transfers ||
	    result.victim.submitted != o.request.victim_transfers ||
	    result.victim.completed_bytes != o.request.victim_transfers * o.request.chunk_bytes ||
	    result.victim_did != caps.did) {
		snprintf(err, sizeof(err), "trial result lacks required measurement or has inconsistent work counts");
		goto done;
	}
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
		fprintf(stderr, "idma_experiment: %s\n", err);
	report(argc, argv, &o, &caps, have_caps, &result, have_result, attempted,
	       stage, err, saved_errno, success);
	if (fflush(stdout) == EOF)
		return EXIT_FAILURE;
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
