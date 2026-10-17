/* Host-only finite fixtures for the actual CLI main; never opens a device. */
#include <errno.h>
#include <fcntl.h>
#include <linux/idma_g.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned opens, caps, runs, closes;
static int mode(const char *s)
{
	const char *m = getenv("G_FAKE_MODE");
	return m && !strcmp(m, s);
}
static void receipt(void)
{
	fprintf(stderr, "{\"opens\":%u,\"caps\":%u,\"runs\":%u,\"closes\":%u}\n",
		opens, caps, runs, closes);
}
__attribute__((constructor)) static void init(void) { atexit(receipt); }
int __wrap_open(const char *path, int flags, ...)
{
	if (opens++ || strcmp(path, "/dev/idma0") || flags != (O_RDWR | O_CLOEXEC)) abort();
	if (mode("open-error")) { errno = ENOENT; return -1; }
	return 17;
}
int __wrap_close(int fd)
{
	if (fd != 17 || closes++) abort();
	if (mode("close-error")) { errno = EIO; return -1; }
	return 0;
}
static void mutate(void *p, size_t size)
{
	const char *offset = getenv("G_FAKE_OFFSET");
	if (!offset) return;
	unsigned long n = strtoul(offset, NULL, 10);
	if (n >= size) abort();
	((unsigned char *)p)[n] ^= 1;
}
int __wrap_ioctl(int fd, unsigned long request, ...)
{
	va_list ap;
	void *arg;
	if (fd != 17) abort();
	va_start(ap, request); arg = va_arg(ap, void *); va_end(ap);
	if (request == IDMA_IOC_CAPS_G) {
		struct idma_g_caps c = {
			.abi_version = IDMA_G_ABI, .struct_size = sizeof(c), .capabilities = 63,
			.did = 10, .page_bytes = 4096, .victim_pages = 1, .aggressor_pages = 8,
			.chunk_bytes = IDMA_G_CHUNK_BYTES, .measured_transfers = IDMA_G_TRANSFERS,
			.warmup_transfers = IDMA_G_WARMUP_TRANSFERS, .max_run_timeout_ms = IDMA_G_MAX_TIMEOUT_MS,
			.batch_magic = IDMA_G_BATCH_MAGIC, .observer_magic = IDMA_G_OBSERVER_MAGIC,
			.gap_magic = IDMA_G_GAP_MAGIC, .bank_magic = IDMA_G_BANK_MAGIC,
			.gap_mask = 7, .bank_first = IDMA_G_BANK_FIRST, .bank_words = IDMA_G_BANK_WORDS,
		};
		if (caps++ || runs) abort();
		if (mode("caps-error")) { errno = ENOTTY; return -1; }
		if (mode("caps-mutated")) mutate(&c, sizeof(c));
		memcpy(arg, &c, sizeof(c));
		if (mode("caps-partial-error")) { errno = EFAULT; return -1; }
		return mode("caps-positive") ? 1 : 0;
	}
	if (request != IDMA_IOC_RUN_G || runs++ || caps != 1) abort();
	struct idma_g_experiment *r = arg;
	int has_a = r->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
	if (mode("run-eintr-untouched")) { errno = EINTR; return -1; }
	r->victim_did = 10; r->aggressor_did = has_a ? 11 : IDMA_EXPERIMENT_NO_AGGRESSOR;
	r->hpm_configured = r->measurement_started = r->measurement_complete = 1;
	r->prepared_lane_mask = r->quiet_lane_mask = r->verified_lane_mask = has_a ? 3 : 1;
	r->observer_sequence = 42;
	r->warmup_completed[0] = 256; r->warmup_bytes[0] = 256 * 4096;
	r->victim_elapsed_ns = 2500000; r->whole_software_elapsed_ns = 200000000;
	r->snapshot_before_ns = 13; r->snapshot_after_ns = 17; r->drain_ns = 19;
	r->old_observer_status = 2;
	r->victim.submitted = r->victim.completed = 4096; r->victim.completed_bytes = 16777216;
	r->victim.active_cycles = 100000; r->victim.iotlb_miss = 23;
	r->victim.s1_ptw = 24; r->victim.s2_ptw = 25;
	r->words[0] = IDMA_G_BANK_MAGIC; r->words[2] = has_a ? 0x3c : 0x0c;
	r->words[4] = r->words[8] = r->words[9] = 4096;
	r->words[10] = r->words[16] = 100000;
	r->words[27] = 1000; r->words[28] = 101000;
	if (has_a) {
		r->warmup_completed[1] = 256; r->warmup_bytes[1] = 256 * 4096;
		r->aggressor.submitted = r->aggressor.completed = 4096; r->aggressor.completed_bytes = 16777216;
		r->aggressor.active_cycles = 200000; r->aggressor.iotlb_miss = 26;
		r->aggressor.s1_ptw = 27; r->aggressor.s2_ptw = 28;
		r->words[5] = r->words[11] = r->words[12] = 4096;
		r->words[7] = r->aggressor_gap_cycles;
		r->words[15] = (uint64_t)4095 * r->aggressor_gap_cycles;
		/* A finishes after V by default. Window counts are independent. */
		r->words[13] = 9000000; r->words[29] = 0; r->words[30] = 9000000;
		r->words[17] = 3; r->words[18] = 3; r->words[19] = r->words[20] = 1;
		r->words[21] = 1; r->words[23] = 1; r->words[24] = 4;
		r->words[25] = 1; r->words[26] = 3;
		if (mode("A-before")) {
			r->words[27] = 10000000; r->words[28] = 10100000;
			memset(&r->words[17], 0, 10 * sizeof(r->words[0]));
		}
		if (mode("A-inside")) {
			/* Gap0 A whole completes strictly inside the V window. */
			r->words[13] = 90000; r->words[29] = 2000; r->words[30] = 92000;
			r->words[17] = r->words[18] = 4096;
			r->words[19] = r->words[20] = r->words[21] = 0;
			r->words[23] = r->words[24] = 0;
			r->words[25] = 1; r->words[26] = 4096;
		}
	}
	if (mode("run-mutated")) mutate(r, sizeof(*r));
	if (mode("run-eintr")) { r->status = -EINTR; errno = EINTR; return -1; }
	if (mode("run-error-raw")) {
		/* Preserve large and otherwise invalid raw outputs on failed ioctl. */
		uint64_t seed = UINT64_C(0xfedcba9876543210);
		unsigned i;
		for (i = 96; i < sizeof(*r); i++) ((unsigned char *)r)[i] = (unsigned char)(i * 37 + 11);
		for (i = 0; i < IDMA_G_BANK_WORDS; i++) r->words[i] = seed + i;
		r->status = -EIO; errno = EIO; return -1;
	}
	return mode("run-positive") ? 1 : 0;
}
