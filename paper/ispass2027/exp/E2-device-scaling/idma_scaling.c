// SPDX-License-Identifier: GPL-2.0
/* One bounded ABI3 E2 trial. No fallback, retries, or direct MMIO. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/idma_multi.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static void string(const char *s)
{
	const unsigned char *p = (const unsigned char *)s;
	putchar('"');
	for (; *p; p++) {
		if (*p == '"' || *p == '\\') printf("\\%c", *p);
		else if (*p < 32) printf("\\u%04x", *p);
		else putchar(*p);
	}
	putchar('"');
}

static int decimal(const char *text, unsigned int maximum, unsigned int *result)
{
	uint64_t n = 0;
	if (!*text) return -1;
	for (; *text; text++) {
		if (*text < '0' || *text > '9') return -1;
		n = n * 10 + (unsigned int)(*text - '0');
		if (n > maximum) return -1;
	}
	*result = (unsigned int)n;
	return 0;
}

static int validate(const struct idma_multi_experiment *m,
		    const struct idma_multi_experiment *request)
{
	unsigned int mask = (1U << request->nr_devices) - 1;
	if (memcmp(m, request, offsetof(struct idma_multi_experiment, status)) || m->status ||
	    m->hpm_configured != 1 || m->measurement_started != 1 || m->measurement_complete != 1 ||
	    !m->observer_sequence || m->prepared_mask != mask || m->start_attempted_mask != mask ||
	    m->stopped_mask != mask || m->drained_mask != mask || m->verified_mask != mask ||
	    m->stop_status_valid_mask != (mask & ~1U)) return -1;
	for (unsigned int i = 0; i < m->nr_devices; i++) {
		const struct idma_experiment_stats *s = &m->stats[i];
		if (m->did[i] != 10 + i || s->data_errors || s->quarantined ||
		    s->submitted != s->completed || s->completed > IDMA_EXPERIMENT_MAX_TRANSFERS ||
		    s->completed_bytes != s->completed * 4096 || s->in_flight_at_start ||
		    s->in_flight_at_end != (i != 0)) return -1;
		if (i && (!(m->pre_stop_status[i] & 2) || m->pre_stop_status[i] >> 32 ||
			  m->pre_stop_issued[i] >= IDMA_EXPERIMENT_MAX_TRANSFERS ||
			  m->start_ns[i] >= m->start_ns[0])) return -1;
	}
	return m->stats[0].completed == 4096 ? 0 : -1;
}

static void stat(const struct idma_experiment_stats *s)
{
	printf("{\"submitted\":%" PRIu64 ",\"completed\":%" PRIu64 ",\"completed_bytes\":%" PRIu64
	       ",\"active_cycles\":%" PRIu64 ",\"iotlb_miss\":%" PRIu64 ",\"s1_ptw\":%" PRIu64
	       ",\"s2_ptw\":%" PRIu64 ",\"refill_gap_ns_sum\":%" PRIu64
	       ",\"refill_gap_ns_max\":%" PRIu64 ",\"refill_gap_samples\":%" PRIu64
	       ",\"in_flight_at_start\":%u,\"in_flight_at_end\":%u,\"data_errors\":%u,\"quarantined\":%u}",
	       (uint64_t)s->submitted, (uint64_t)s->completed, (uint64_t)s->completed_bytes,
	       (uint64_t)s->active_cycles, (uint64_t)s->iotlb_miss, (uint64_t)s->s1_ptw,
	       (uint64_t)s->s2_ptw, (uint64_t)s->refill_gap_ns_sum, (uint64_t)s->refill_gap_ns_max,
	       (uint64_t)s->refill_gap_samples, s->in_flight_at_start, s->in_flight_at_end,
	       s->data_errors, s->quarantined);
}

static void u32s(const __u32 *values, unsigned int nr)
{
	putchar('[');
	for (unsigned int i = 0; i < nr; i++) {
		if (i) putchar(',');
		printf("%u", values[i]);
	}
	putchar(']');
}

static void input(const struct idma_multi_experiment *m)
{
	printf("{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,\"nr_devices\":%u,\"device_id\":",
	       m->abi_version, m->struct_size, m->flags, m->nr_devices);
	u32s(m->device_id, 4); printf(",\"pages\":"); u32s(m->pages, 4);
	printf(",\"path_aggressor_index\":%u,\"chunk_bytes\":%u,\"run_timeout_ms\":%u,"
	       "\"reserved0\":%u,\"victim_transfers\":%" PRIu64 ",\"warmup_transfers\":%" PRIu64 ",\"reserved\":",
	       m->path_aggressor_index, m->chunk_bytes, m->run_timeout_ms, m->reserved0,
	       (uint64_t)m->victim_transfers, (uint64_t)m->warmup_transfers);
	u32s(m->reserved, 8); putchar('}');
}

static void u64s(const __u64 values[4])
{
	printf("[%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%" PRIu64 "]",
	       (uint64_t)values[0], (uint64_t)values[1], (uint64_t)values[2], (uint64_t)values[3]);
}

int main(int argc, char **argv)
{
	unsigned int nr = 2, pages = 8, timeout = 60000, seen = 0;
	int caps_only = 0, fd = -1, error = 0, success = 0;
	const char *message = "";
	struct idma_multi_caps caps = {};
	struct idma_multi_experiment m = {}, request = {};
	for (int i = 1; i < argc; i++) {
		unsigned int bit, value;
		if (!strcmp(argv[i], "--help")) {
			puts("idma_scaling --devices 2|3|4 --aggressor-pages N [--timeout-ms N] [--caps]\n"
			     "Fixed V0/A1.., selected A1, COPY4096, transfers4096, warmup256; ABI3 only.");
			return 0;
		}
		if (!strcmp(argv[i], "--caps")) {
			if (seen & 8) { error = EINVAL; break; }
			seen |= 8; caps_only = 1; continue;
		}
		if (!strcmp(argv[i], "--devices")) bit = 1;
		else if (!strcmp(argv[i], "--aggressor-pages")) bit = 2;
		else if (!strcmp(argv[i], "--timeout-ms")) bit = 4;
		else { error = EINVAL; break; }
		if ((seen & bit) || ++i == argc || decimal(argv[i], 300000, &value)) {
			error = EINVAL; break;
		}
		seen |= bit;
		if (bit == 1) nr = value;
		else if (bit == 2) pages = value;
		else timeout = value;
	}
	if (error || nr < 2 || nr > 4 || !timeout ||
	    (pages != 1 && pages != 8 && pages != 6 / (nr - 1))) {
		error = EINVAL; message = "invalid or duplicate E2 option"; goto output;
	}
	m.abi_version = IDMA_MULTI_ABI; m.struct_size = sizeof(m); m.nr_devices = nr;
	m.path_aggressor_index = 1; m.chunk_bytes = 4096; m.run_timeout_ms = timeout;
	m.victim_transfers = 4096; m.warmup_transfers = 256;
	for (unsigned int i = 0; i < nr; i++) { m.device_id[i] = i; m.pages[i] = i ? pages : 1; }
	request = m;
	fd = open("/dev/idma0", O_RDWR | O_CLOEXEC);
	if (fd < 0) { error = errno; message = "open"; goto output; }
	if (ioctl(fd, IDMA_IOC_CAPS_MULTI, &caps) < 0) {
		error = errno; message = "CAPS_MULTI"; goto output;
	}
	if (caps.abi_version != 3 || caps.struct_size != sizeof(caps) || caps.device_id || caps.did != 10 ||
	    caps.max_devices < nr || caps.max_pages < pages || caps.page_bytes != 4096 ||
	    caps.max_run_timeout_ms < timeout) { error = EPROTO; message = "capability layout"; goto output; }
	for (unsigned int i = 0; i < 7; i++)
		if (caps.reserved[i]) { error = EPROTO; message = "capability reserved"; goto output; }
	if (caps_only) { success = 1; goto output; }
	if ((caps.capabilities & 7) != 7) { error = EOPNOTSUPP; message = "required capabilities"; goto output; }
	if (ioctl(fd, IDMA_IOC_RUN_MULTI_BATCH, &m) < 0) {
		error = errno; message = "RUN_MULTI_BATCH"; goto output;
	}
	if (validate(&m, &request)) { error = EPROTO; message = "result contract"; goto output; }
	success = 1;
output:
	if (fd >= 0) close(fd);
	printf("{\"schema\":\"idma.multi_batch.v1\",\"success\":%s,\"errno\":%d,\"message\":",
	       success ? "true" : "false", error);
	string(message);
	printf(",\"device\":\"/dev/idma0\",\"caps_only\":%s,\"input\":", caps_only ? "true" : "false");
	input(&request);
	putchar(',');
	printf("\"capabilities\":{\"abi_version\":%u,\"struct_size\":%u,\"capabilities\":%u,"
	       "\"device_id\":%u,\"did\":%u,\"max_devices\":%u,\"max_pages\":%u,"
	       "\"page_bytes\":%u,\"max_run_timeout_ms\":%u,\"reserved\":", caps.abi_version, caps.struct_size,
	       caps.capabilities, caps.device_id, caps.did, caps.max_devices, caps.max_pages,
	       caps.page_bytes, caps.max_run_timeout_ms);
	u32s(caps.reserved, 7); printf("},");
	printf("\"returned_input\":"); input(&m); putchar(',');
	printf("\"result\":{\"status\":%d,\"hpm_configured\":%u,\"measurement_started\":%u,"
	       "\"measurement_complete\":%u,\"did\":", m.status, m.hpm_configured,
	       m.measurement_started, m.measurement_complete); u32s(m.did, 4);
	printf(",\"prepared_mask\":%u,\"start_attempted_mask\":%u,\"stopped_mask\":%u,"
	       "\"drained_mask\":%u,\"verified_mask\":%u,\"stop_status_valid_mask\":%u,"
	       "\"observer_sequence\":%" PRIu64 ",\"victim_elapsed_ns\":%" PRIu64
	       ",\"snapshot_before_ns\":%" PRIu64 ",\"snapshot_after_ns\":%" PRIu64 ",\"drain_ns\":%" PRIu64
	       ",\"stats\":[", m.prepared_mask, m.start_attempted_mask, m.stopped_mask, m.drained_mask,
	       m.verified_mask, m.stop_status_valid_mask, (uint64_t)m.observer_sequence,
	       (uint64_t)m.victim_elapsed_ns, (uint64_t)m.snapshot_before_ns,
	       (uint64_t)m.snapshot_after_ns, (uint64_t)m.drain_ns);
	for (unsigned int i = 0; i < 4; i++) { if (i) putchar(','); stat(&m.stats[i]); }
	printf("],\"start_ns\":"); u64s(m.start_ns);
	printf(",\"stop_ns\":"); u64s(m.stop_ns);
	printf(",\"drain_end_ns\":"); u64s(m.drain_end_ns);
	printf(",\"pre_stop_status\":"); u64s(m.pre_stop_status);
	printf(",\"pre_stop_issued\":"); u64s(m.pre_stop_issued);
	printf("}}\n");
	return success ? 0 : 1;
}
