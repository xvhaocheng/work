/* SPDX-License-Identifier: GPL-2.0 */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/idma_sva.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

/* These are the materialized A2 input offsets, not stage117 physical VAs. */
static const unsigned int lengths[] = { 60, 568, 1011 };
static const unsigned int record_offsets[] = { 0, 72, 652 };
static const unsigned int src_offsets[] = { 32, 4128, 8224, 12320 };
static const unsigned int dst_offsets[] = { 16396, 20492, 20557, 24588 };
static const unsigned int span_lengths[] = { 60, 65, 503, 1011 };
static const unsigned int input_offsets[] = { 12, 84, 149, 664 };

#define MODE_CPU_COPY 4U
#define MODE_SOURCE_WRITEV 5U
#define MAX_REPEATS 1024U

struct options {
	const char *input, *device;
	const char *outputs[MAX_REPEATS];
	unsigned int mode, peer, timeout, repeat, output_count;
	int host_model;
};

static void usage(FILE *stream)
{
	fputs("Usage: idma_sva_task --input FILE --output FILE [--output FILE ...]\n"
	      "  --address-mode iova|sameva-shadow|pinned-mm|cpu-copy|source-writev\n"
	      "  --device PATH       DMA device (default /dev/idma0)\n"
	      "  --peer N            Peer device index (default 1)\n"
	      "  --timeout-ms N      Normal-DONE deadline; not a cancellation (default 1000)\n"
	      "  --repeat N          Requires N distinct --output paths (default 1)\n"
	      "  --backend native|host-model  Explicit logical host model: NO DMA/SVA evidence\n"
	      "Input is exactly three le64-cookie/le32-length frames (1/60,2/568,3/1011).\n"
	      "No timings are measured. DMA errors never fall back to a CPU adapter.\n", stream);
}

static int parse_uint(const char *text, unsigned int *value)
{
	uint64_t n = 0;
	const unsigned char *p = (const unsigned char *)text;

	if (!*p)
		return -1;
	for (; *p; p++) {
		if (*p < '0' || *p > '9' || n > (UINT32_MAX - (*p - '0')) / 10)
			return -1;
		n = n * 10 + *p - '0';
	}
	*value = (unsigned int)n;
	return 0;
}

static const char *mode_name(unsigned int mode)
{
	switch (mode) {
	case IDMA_SVA_MODE_IOVA: return "iova";
	case IDMA_SVA_MODE_SHADOW: return "sameva-shadow";
	case IDMA_SVA_MODE_BORROWED_MM: return "pinned-mm";
	case MODE_CPU_COPY: return "cpu-copy";
	case MODE_SOURCE_WRITEV: return "source-writev";
	default: return "invalid";
	}
}

static int parse_options(int argc, char **argv, struct options *o)
{
	unsigned int seen = 0;
	int i;

	memset(o, 0, sizeof(*o));
	o->device = "/dev/idma0";
	o->peer = 1;
	o->timeout = 1000;
	o->repeat = 1;
	for (i = 1; i < argc; i++) {
		unsigned int bit;
		const char *arg = argv[i], *value;

		if (!strcmp(arg, "--help")) {
			usage(stdout);
			exit(0);
		}
		if (!strcmp(arg, "--input")) bit = 1U;
		else if (!strcmp(arg, "--device")) bit = 2U;
		else if (!strcmp(arg, "--peer")) bit = 4U;
		else if (!strcmp(arg, "--timeout-ms")) bit = 8U;
		else if (!strcmp(arg, "--repeat")) bit = 16U;
		else if (!strcmp(arg, "--address-mode")) bit = 32U;
		else if (!strcmp(arg, "--backend")) bit = 64U;
		else if (!strcmp(arg, "--output")) bit = 0;
		else return -1;
		if ((seen & bit) || i + 1 == argc)
			return -1;
		seen |= bit;
		value = argv[++i];
		if (!*value)
			return -1;
		if (!strcmp(arg, "--input")) o->input = value;
		else if (!strcmp(arg, "--device")) o->device = value;
		else if (!strcmp(arg, "--output")) {
			if (o->output_count == MAX_REPEATS)
				return -1;
			o->outputs[o->output_count++] = value;
		} else if (!strcmp(arg, "--backend")) {
			if (!strcmp(value, "host-model")) o->host_model = 1;
			else if (strcmp(value, "native")) return -1;
		} else if (!strcmp(arg, "--address-mode")) {
			for (o->mode = 1; o->mode <= MODE_SOURCE_WRITEV; o->mode++)
				if (!strcmp(value, mode_name(o->mode)))
					break;
			if (o->mode > MODE_SOURCE_WRITEV)
				return -1;
		} else {
			unsigned int n;
			if (parse_uint(value, &n))
				return -1;
			if (!strcmp(arg, "--peer")) o->peer = n;
			else if (!strcmp(arg, "--repeat")) o->repeat = n;
			else o->timeout = n;
		}
	}
	return !o->input || !o->mode || !o->repeat || o->repeat > MAX_REPEATS ||
		o->repeat != o->output_count || !o->timeout ||
		(o->host_model && o->mode >= MODE_CPU_COPY) ? -1 : 0;
}

static int read_input(const char *path, unsigned char *input)
{
	unsigned char bytes[IDMA_SVA_RECORD_BYTES + 1];
	size_t have = 0;
	int fd = open(path, O_RDONLY | O_CLOEXEC), error = 0;
	unsigned int p;

	if (fd < 0)
		return -1;
	while (have < sizeof(bytes)) {
		ssize_t count = read(fd, bytes + have, sizeof(bytes) - have);
		if (count < 0 && errno == EINTR)
			continue;
		if (count < 0) { error = errno; break; }
		if (!count) break;
		have += (size_t)count;
	}
	if (close(fd) && !error)
		error = errno;
	if (error) { errno = error; return -1; }
	if (have != IDMA_SVA_RECORD_BYTES) { errno = EINVAL; return -1; }
	for (p = 0; p < IDMA_SVA_PACKETS; p++) {
		const unsigned char *header = bytes + record_offsets[p];
		uint64_t cookie = 0;
		uint32_t length = 0;
		unsigned int i;
		for (i = 0; i < 8; i++) cookie |= (uint64_t)header[i] << (8 * i);
		for (i = 0; i < 4; i++) length |= (uint32_t)header[8 + i] << (8 * i);
		if (cookie != p + 1 || length != lengths[p]) { errno = EINVAL; return -1; }
	}
	memcpy(input, bytes, IDMA_SVA_RECORD_BYTES);
	return 0;
}

/* Reject accidental input truncation and repeated outputs, including existing
 * hardlinks. fstat() after open repeats the input check before ftruncate(). */
static int check_paths(const struct options *o)
{
	struct stat source, outputs[MAX_REPEATS];
	unsigned char exists[MAX_REPEATS] = { 0 };
	unsigned int i, j;

	if (stat(o->input, &source))
		return -1;
	for (i = 0; i < o->output_count; i++) {
		if (!stat(o->outputs[i], &outputs[i])) {
			exists[i] = 1;
			if (source.st_dev == outputs[i].st_dev && source.st_ino == outputs[i].st_ino)
				goto invalid;
		} else if (errno != ENOENT) return -1;
		for (j = 0; j < i; j++) {
			if (!strcmp(o->outputs[i], o->outputs[j]) ||
			    (exists[i] && exists[j] && outputs[i].st_dev == outputs[j].st_dev &&
			     outputs[i].st_ino == outputs[j].st_ino))
				goto invalid;
		}
	}
	return 0;
invalid:
	errno = EINVAL;
	return -1;
}

static void initialize_arena(unsigned char *arena, const unsigned char *input)
{
	unsigned int i;

	memset(arena, 0xa5, IDMA_SVA_ARENA_BYTES);
	for (i = 0; i < IDMA_SVA_DESCRIPTORS; i++) {
		memset(arena + src_offsets[i] - 16, 0x3c, 16);
		memcpy(arena + src_offsets[i], input + input_offsets[i], span_lengths[i]);
		memset(arena + src_offsets[i] + span_lengths[i], 0xc3, 16);
	}
	for (i = 0; i < IDMA_SVA_PACKETS; i++) {
		unsigned char *record = arena + (i + 4) * IDMA_SVA_PAGE_BYTES;
		memcpy(record, input + record_offsets[i], IDMA_SVA_HEADER_BYTES);
		memset(record + IDMA_SVA_HEADER_BYTES + lengths[i], 0x5a, 16);
	}
}

/* Compare all 32768 bytes without allocating an extra expected arena. The
 * expected packets come from the input file, not from returned DMA addresses.
 */
static int verify_arena(const unsigned char *arena, const unsigned char *input)
{
	unsigned int page, offset;

	for (page = 0; page < IDMA_SVA_ARENA_PAGES; page++) {
		for (offset = 0; offset < IDMA_SVA_PAGE_BYTES; offset++) {
			unsigned char expected = 0xa5;
			if (page < 4) {
				if (offset >= 16 && offset < 32) expected = 0x3c;
				else if (offset >= 32 && offset < 32 + span_lengths[page])
					expected = input[input_offsets[page] + offset - 32];
				else if (offset >= 32 + span_lengths[page] && offset < 48 + span_lengths[page])
					expected = 0xc3;
			} else if (page < 7) {
				unsigned int p = page - 4, end = IDMA_SVA_HEADER_BYTES + lengths[p];
				if (offset < end) expected = input[record_offsets[p] + offset];
				else if (offset < end + 16) expected = 0x5a;
			}
			if (arena[page * IDMA_SVA_PAGE_BYTES + offset] != expected) {
				fprintf(stderr, "arena mismatch: page=%u offset=%u expected=%02x actual=%02x\n",
					page, offset, expected, arena[page * IDMA_SVA_PAGE_BYTES + offset]);
				errno = EILSEQ;
				return -1;
			}
		}
	}
	return 0;
}

/* This is a logical protocol model, not a bus/cache/IOMMU simulator. The
 * event order describes model obligations only. memcpy implements its data
 * operation; no event below establishes that real hardware executed it.
 */
static const char *const model_job_events[] = {
	"SOURCE_READY", "NEXT_ID", "COUNT_ACCEPT", "R", "W", "B", "DONE",
};

static void host_model_copy(unsigned char *arena)
{
	unsigned int descriptor, state;

	for (descriptor = 0; descriptor < IDMA_SVA_DESCRIPTORS; descriptor++) {
		for (state = 0; state < sizeof(model_job_events) / sizeof(model_job_events[0]); state++) {
			/* The serial model can only reach DONE after its R/W/B stages.
			 * It has no software pending queue or simultaneous live jobs.
			 */
			if (state == 4)
				memcpy(arena + dst_offsets[descriptor], arena + src_offsets[descriptor],
				       span_lengths[descriptor]);
		}
	}
}

static void print_model_trace(void)
{
	unsigned int descriptor, state;
	for (descriptor = 0; descriptor < IDMA_SVA_DESCRIPTORS; descriptor++)
		for (state = 0; state < sizeof(model_job_events) / sizeof(model_job_events[0]); state++)
			printf("%s\"d%u:%s\"", descriptor || state ? "," : "", descriptor, model_job_events[state]);
	fputs(",\"CACHE_ACQUIRE\",\"CONTEXT_RESTORED\",\"UNPIN\",\"INDEPENDENT_VERIFY\",\"FILE_CONSUMER\"", stdout);
}

static int check_caps(int fd, const struct options *o, struct idma_sva_caps *caps)
{
	const unsigned int required = IDMA_SVA_CAP_SYNCHRONOUS | IDMA_SVA_CAP_FIXED_TASK |
		IDMA_SVA_CAP_SINGLE_HART_FENCE_I | IDMA_SVA_CAP_QUIESCENT_CONTEXT;

	memset(caps, 0, sizeof(*caps));
	caps->abi_version = IDMA_SVA_ABI;
	caps->struct_size = sizeof(*caps);
	caps->peer_index = o->peer;
	/* Do not retry RUN after EINTR: submission/ownership may already exist. */
	if (ioctl(fd, IDMA_IOC_SVA_CAPS, caps))
		return -1;
	if (caps->abi_version != IDMA_SVA_ABI || caps->struct_size != sizeof(*caps) ||
	    (caps->capabilities & required) != required ||
	    !(caps->address_modes & IDMA_SVA_MODE_BIT(o->mode)) ||
	    caps->primary_did != 10 || caps->peer_did != 11 || caps->peer_index != o->peer ||
	    caps->page_bytes != IDMA_SVA_PAGE_BYTES || caps->arena_pages != IDMA_SVA_ARENA_PAGES ||
	    caps->data_pages != IDMA_SVA_DATA_PAGES || caps->descriptors != IDMA_SVA_DESCRIPTORS ||
	    caps->packets != IDMA_SVA_PACKETS || o->timeout > caps->max_timeout_ms) {
		errno = EOPNOTSUPP;
		return -1;
	}
	return 0;
}

static int run_dma(int fd, const struct options *o, unsigned char *arena,
		   struct idma_sva_run *run)
{
	unsigned int i;
	uint64_t base;

	memset(run, 0, sizeof(*run));
	run->abi_version = IDMA_SVA_ABI;
	run->struct_size = sizeof(*run);
	run->address_mode = o->mode;
	run->peer_index = o->peer;
	run->timeout_ms = o->timeout;
	run->arena_user = (uintptr_t)arena;
	run->arena_bytes = IDMA_SVA_ARENA_BYTES;
	if (ioctl(fd, IDMA_IOC_SVA_RUN, run)) {
		fprintf(stderr, "RUN failed: status=%d restore_status=%d flags=%u submitted=%u completed=%u\n",
			run->status, run->restore_status, run->result_flags, run->submitted, run->completed);
		return -1;
	}
	if (run->status || run->restore_status ||
	    (run->result_flags & IDMA_SVA_RESULT_SAFE) != IDMA_SVA_RESULT_SAFE ||
	    run->submitted != IDMA_SVA_DESCRIPTORS || run->completed != IDMA_SVA_DESCRIPTORS ||
	    run->primary_did != 10 || run->peer_did != 11 || run->pinned_pages != IDMA_SVA_ARENA_PAGES ||
	    run->mapped_pages != (o->mode == IDMA_SVA_MODE_BORROWED_MM ? 0U : IDMA_SVA_DATA_PAGES)) {
		errno = EPROTO;
		return -1;
	}
	if (run->device_src[0] < src_offsets[0]) { errno = EPROTO; return -1; }
	base = run->device_src[0] - src_offsets[0];
	if (base > UINT64_MAX - IDMA_SVA_ARENA_BYTES || base % IDMA_SVA_PAGE_BYTES ||
	    (o->mode != IDMA_SVA_MODE_IOVA && base != (uintptr_t)arena)) {
		errno = EPROTO;
		return -1;
	}
	for (i = 0; i < IDMA_SVA_DESCRIPTORS; i++) {
		if (!run->transfer_id[i] || run->device_src[i] != base + src_offsets[i] ||
		    run->device_dst[i] != base + dst_offsets[i]) { errno = EPROTO; return -1; }
	}
	return 0;
}

/* Advance exactly the bytes accepted by writev(), including partial vectors. */
static int write_vectors(int fd, struct iovec *iov, int count)
{
	while (count) {
		ssize_t written = writev(fd, iov, count);
		if (written < 0 && errno == EINTR)
			continue;
		if (written < 0)
			return -1;
		if (!written) { errno = EIO; return -1; }
		while (count && (size_t)written >= iov->iov_len) {
			written -= (ssize_t)iov->iov_len;
			iov++;
			count--;
		}
		if (count) {
			iov->iov_base = (unsigned char *)iov->iov_base + written;
			iov->iov_len -= (size_t)written;
		}
	}
	return 0;
}

static int consume_file(const struct options *o, unsigned int iteration,
			const unsigned char *arena, const unsigned char *input)
{
	struct stat source, target;
	const char *path = o->outputs[iteration];
	int fd, error = 0;
	unsigned int p;

	if (stat(o->input, &source))
		return -1;
	fd = open(path, O_WRONLY | O_CREAT | O_CLOEXEC, 0666);
	if (fd < 0)
		return -1;
	if (fstat(fd, &target)) error = errno;
	else if (source.st_dev == target.st_dev && source.st_ino == target.st_ino) error = EINVAL;
	/* Files created in previous iterations now have an inode, including aliases
	 * such as new-file and ./new-file that did not exist during preflight.
	 */
	for (p = 0; p < iteration && !error; p++) {
		struct stat previous;
		if (stat(o->outputs[p], &previous)) error = errno;
		else if (previous.st_dev == target.st_dev && previous.st_ino == target.st_ino) error = EINVAL;
	}
	if (!error && S_ISREG(target.st_mode) && ftruncate(fd, 0)) error = errno;
	for (p = 0; p < IDMA_SVA_PACKETS && !error; p++) {
		const unsigned char *record = o->mode == MODE_SOURCE_WRITEV ?
			input + record_offsets[p] : arena + (p + 4) * IDMA_SVA_PAGE_BYTES;
		struct iovec iov[3] = {
			{ .iov_base = (void *)record, .iov_len = IDMA_SVA_HEADER_BYTES },
			{ .iov_base = (void *)(record + IDMA_SVA_HEADER_BYTES), .iov_len = lengths[p] },
		};
		int count = 2;
		if (o->mode == MODE_SOURCE_WRITEV && p == 1) {
			iov[1].iov_len = 65;
			iov[2].iov_base = (void *)(record + IDMA_SVA_HEADER_BYTES + 65);
			iov[2].iov_len = 503;
			count = 3;
		}
		if (write_vectors(fd, iov, count)) error = errno;
	}
	if (close(fd) && !error) error = errno;
	if (error) { errno = error; return -1; }
	return 0;
}

int main(int argc, char **argv)
{
	struct options options;
	struct idma_sva_caps caps;
	unsigned char input[IDMA_SVA_RECORD_BYTES];
	int fd = -1, result = EXIT_FAILURE;
	unsigned int iteration;

	if (parse_options(argc, argv, &options)) { usage(stderr); return EXIT_FAILURE; }
	if (check_paths(&options)) { perror("input/output paths"); return EXIT_FAILURE; }
	/* A failed pipe consumer must be reported as an error, not silent SIGPIPE. */
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) { perror("SIGPIPE"); return EXIT_FAILURE; }
	if (options.mode < MODE_CPU_COPY && !options.host_model) {
		fd = open(options.device, O_RDWR | O_CLOEXEC);
		if (fd < 0) { perror("open DMA device"); goto done; }
		if (check_caps(fd, &options, &caps)) { perror("CAPS"); goto done; }
	}
	for (iteration = 0; iteration < options.repeat; iteration++) {
		unsigned char *arena = MAP_FAILED;
		struct idma_sva_run run = { 0 };
		unsigned int i;
		if (read_input(options.input, input)) { perror("input frames"); goto done; }
		if (options.mode != MODE_SOURCE_WRITEV) {
			if (sysconf(_SC_PAGESIZE) != IDMA_SVA_PAGE_BYTES) {
				fprintf(stderr, "This fixed task requires 4096-byte OS pages\n");
				goto done;
			}
			arena = mmap(NULL, IDMA_SVA_ARENA_BYTES, PROT_READ | PROT_WRITE,
				     MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
			if (arena == MAP_FAILED) { perror("arena mmap"); goto done; }
			initialize_arena(arena, input);
			if (options.host_model) {
				host_model_copy(arena);
			} else if (options.mode == MODE_CPU_COPY) {
				for (i = 0; i < IDMA_SVA_DESCRIPTORS; i++)
					memcpy(arena + dst_offsets[i], arena + src_offsets[i], span_lengths[i]);
			} else if (run_dma(fd, &options, arena, &run)) {
				perror("DMA RUN");
				munmap(arena, IDMA_SVA_ARENA_BYTES);
				goto done;
			}
			if (verify_arena(arena, input)) {
				perror("independent bytes/guards");
				munmap(arena, IDMA_SVA_ARENA_BYTES);
				goto done;
			}
		}
		if (consume_file(&options, iteration, arena, input)) {
			perror("file consumer");
			if (arena != MAP_FAILED) munmap(arena, IDMA_SVA_ARENA_BYTES);
			goto done;
		}
		if (arena != MAP_FAILED && munmap(arena, IDMA_SVA_ARENA_BYTES)) { perror("arena munmap"); goto done; }
		printf("{\"result\":\"PASS\",\"backend\":\"%s\",\"address_mode\":\"%s\","
		       "\"iteration\":%u,\"arena_pages\":%u,\"semantic_descriptors\":%u,"
		       "\"dma_submitted\":%u,\"dma_completed\":%u,\"pinned_pages\":%u,"
		       "\"mapped_pages\":%u,\"domain_pt_pages\":%u,\"consumer_bytes\":%u,"
		       "\"timing\":\"UNKNOWN\",\"model_trace\":[",
		       options.host_model ? "host-model-NO-DMA" : options.mode >= MODE_CPU_COPY ? "CPU" : "native-DMA",
		       mode_name(options.mode), iteration + 1,
		       options.mode == MODE_SOURCE_WRITEV ? 0U : IDMA_SVA_ARENA_PAGES,
		       options.mode == MODE_SOURCE_WRITEV ? 0U : IDMA_SVA_DESCRIPTORS,
		       run.submitted, run.completed, run.pinned_pages, run.mapped_pages,
		       run.domain_pt_pages, IDMA_SVA_RECORD_BYTES);
		if (options.host_model) print_model_trace();
		puts("]}");
		if (fflush(stdout)) { perror("result output"); goto done; }
	}
	result = EXIT_SUCCESS;
done:
	if (fd >= 0 && close(fd)) { perror("close DMA device"); result = EXIT_FAILURE; }
	return result;
}
