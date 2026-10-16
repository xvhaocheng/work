// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IDMA_IOC_MAGIC		'i'

#define IDMA_BENCH_F_HUGE	(1U << 0)
#define IDMA_BENCH_F_VERIFY	(1U << 1)
#define IDMA_BENCH_F_DMA_API	(1U << 2)
#define IDMA_BENCH_F_RANDOM	(1U << 3)

#define IDMA_MULTI_BENCH_MAX_DEVS 8
#define MAX_CHUNKS 8

struct idma_bench_args {
	uint64_t size;
	uint32_t iterations;
	uint32_t flags;

	uint64_t bytes;
	uint64_t cycles;
	uint64_t iotlb_miss;
	uint64_t s1_ptw;
	uint64_t s2_ptw;
	uint64_t dma_active_cycles;
	uint32_t errors;
	uint32_t reserved;
};

struct idma_multi_bench_args {
	uint32_t nr_devs;
	uint32_t xfer_chunk;
	uint32_t dev_ids[IDMA_MULTI_BENCH_MAX_DEVS];
	uint32_t reserved[6];

	struct idma_bench_args devs[IDMA_MULTI_BENCH_MAX_DEVS];
};

#define IDMA_IOC_RUN_MULTI_BENCH \
	_IOWR(IDMA_IOC_MAGIC, 0x04, struct idma_multi_bench_args)

enum case_mask {
	CASE_2M_LINEAR = 1U << 0,
	CASE_4K_LINEAR = 1U << 1,
	CASE_4K_RANDOM = 1U << 2,
	CASE_DMA_LINEAR = 1U << 3,
	CASE_ALL = CASE_2M_LINEAR | CASE_4K_LINEAR | CASE_4K_RANDOM,
};

struct bench_case {
	const char *name;
	uint32_t flags;
	uint32_t mask;
};

struct options {
	const char *ioctl_dev;
	uint32_t dev_ids[IDMA_MULTI_BENCH_MAX_DEVS];
	uint32_t nr_dev_ids;
	uint32_t exact_nr_devs;
	uint64_t size;
	uint32_t iterations;
	uint32_t repeats;
	uint32_t chunks[MAX_CHUNKS];
	uint32_t nr_chunks;
	uint32_t case_mask;
	int verify;
	int csv_header;
};

static const struct bench_case bench_cases[] = {
	{
		.name = "2m-linear",
		.flags = IDMA_BENCH_F_HUGE,
		.mask = CASE_2M_LINEAR,
	},
	{
		.name = "4k-linear",
		.flags = 0,
		.mask = CASE_4K_LINEAR,
	},
	{
		.name = "4k-random",
		.flags = IDMA_BENCH_F_RANDOM,
		.mask = CASE_4K_RANDOM,
	},
	{
		.name = "dma-linear",
		.flags = IDMA_BENCH_F_DMA_API,
		.mask = CASE_DMA_LINEAR,
	},
};

static void usage(const char *prog)
{
	fprintf(stderr,
		"Usage: %s [options]\n"
		"\n"
		"Default:\n"
		"  Sweep 1..4 DMA engines using dev ids 0,1,2,3.\n"
		"  Cases: 2m-linear, 4k-linear, 4k-random.\n"
		"  Defaults: size=2m, iters=20, repeats=3, chunks=256,1k,4k, verify=on.\n"
		"\n"
		"Options:\n"
		"  --ioctl-dev PATH       device used to issue ioctl, default /dev/idma0\n"
		"  --devs LIST            comma list of idma instance ids, default 0,1,2,3\n"
		"  --nr-devs N            run exactly first N ids from --devs\n"
		"  --scale                run 1..len(--devs), default\n"
		"  --case CASE            all, 2m-linear, 4k-linear, 4k-random, dma-linear\n"
		"  -s, --size SIZE        default 2m\n"
		"  -i, --iters N          default 20\n"
		"  -r, --repeats N        default 3\n"
		"  --chunks LIST          comma list, default 256,1k,4k\n"
		"  --chunk SIZE           single chunk size\n"
		"  --verify               enable verification\n"
		"  --noverify             disable verification\n"
		"  --no-header            omit CSV header\n",
		prog);
}

static uint64_t parse_size(const char *s)
{
	char *end;
	uint64_t value;
	unsigned int shift = 0;

	errno = 0;
	value = strtoull(s, &end, 0);
	if (errno || end == s) {
		fprintf(stderr, "invalid size: %s\n", s);
		exit(1);
	}

	if (!*end)
		return value;
	if (!strcmp(end, "k") || !strcmp(end, "K"))
		shift = 10;
	else if (!strcmp(end, "m") || !strcmp(end, "M"))
		shift = 20;
	else if (!strcmp(end, "g") || !strcmp(end, "G"))
		shift = 30;
	else {
		fprintf(stderr, "invalid size suffix: %s\n", end);
		exit(1);
	}

	if (value > (UINT64_MAX >> shift)) {
		fprintf(stderr, "size overflow: %s\n", s);
		exit(1);
	}

	return value << shift;
}

static uint32_t parse_u32(const char *s, const char *name)
{
	char *end;
	unsigned long value;

	errno = 0;
	value = strtoul(s, &end, 0);
	if (errno || end == s || *end || value > UINT32_MAX) {
		fprintf(stderr, "invalid %s: %s\n", name, s);
		exit(1);
	}

	return (uint32_t)value;
}

static void add_chunk(struct options *opts, uint64_t chunk)
{
	if (!chunk || chunk > UINT32_MAX || opts->nr_chunks >= MAX_CHUNKS) {
		fprintf(stderr, "invalid chunk: %" PRIu64 "\n", chunk);
		exit(1);
	}

	opts->chunks[opts->nr_chunks++] = (uint32_t)chunk;
}

static char *xstrdup(const char *s)
{
	char *copy = malloc(strlen(s) + 1);

	if (!copy) {
		perror("malloc");
		exit(1);
	}
	strcpy(copy, s);
	return copy;
}

static void parse_chunks(struct options *opts, const char *arg)
{
	char *copy = xstrdup(arg);
	char *tok;

	opts->nr_chunks = 0;
	for (tok = strtok(copy, ","); tok; tok = strtok(NULL, ","))
		add_chunk(opts, parse_size(tok));

	free(copy);

	if (!opts->nr_chunks) {
		fprintf(stderr, "empty chunk list\n");
		exit(1);
	}
}

static void parse_devs(struct options *opts, const char *arg)
{
	char *copy = xstrdup(arg);
	char *tok;
	uint32_t i;

	opts->nr_dev_ids = 0;
	for (tok = strtok(copy, ","); tok; tok = strtok(NULL, ",")) {
		uint32_t id;

		if (opts->nr_dev_ids >= IDMA_MULTI_BENCH_MAX_DEVS) {
			fprintf(stderr, "too many device ids\n");
			exit(1);
		}

		id = parse_u32(tok, "device id");
		for (i = 0; i < opts->nr_dev_ids; i++) {
			if (opts->dev_ids[i] == id) {
				fprintf(stderr, "duplicate device id: %u\n", id);
				exit(1);
			}
		}

		opts->dev_ids[opts->nr_dev_ids++] = id;
	}

	free(copy);

	if (!opts->nr_dev_ids) {
		fprintf(stderr, "empty device list\n");
		exit(1);
	}
}

static uint32_t parse_case_mask(const char *arg)
{
	size_t i;

	if (!strcmp(arg, "all"))
		return CASE_ALL;

	for (i = 0; i < sizeof(bench_cases) / sizeof(bench_cases[0]); i++) {
		if (!strcmp(arg, bench_cases[i].name))
			return bench_cases[i].mask;
	}

	fprintf(stderr, "unknown case: %s\n", arg);
	exit(1);
}

static const char *mode_name(uint32_t flags)
{
	if (flags & IDMA_BENCH_F_DMA_API)
		return "dma";
	if (flags & IDMA_BENCH_F_HUGE)
		return "2m";
	return "4k";
}

static const char *pattern_name(uint32_t flags)
{
	return (flags & IDMA_BENCH_F_RANDOM) ? "random" : "linear";
}

static double ratio_u64(uint64_t n, uint64_t d)
{
	return d ? (double)n / (double)d : 0.0;
}

static void print_header(void)
{
	puts("repeat,nr_devs,case,chunk,size,dev_index,dev_id,mode,pattern,"
	     "iterations,bytes,cycles,cycles_per_byte,active,"
	     "active_per_byte,iotlb_miss,s1_ptw,s2_ptw,errors,"
	     "total_bytes,total_active");
}

static int run_one(int fd, const struct options *opts,
		   const struct bench_case *tc, uint32_t nr_devs,
		   uint32_t chunk, uint32_t repeat)
{
	struct idma_multi_bench_args args;
	uint64_t total_bytes = 0;
	uint64_t total_active = 0;
	int saved_errno = 0;
	int ret;
	int failed = 0;
	uint32_t i;

	memset(&args, 0, sizeof(args));
	args.nr_devs = nr_devs;
	args.xfer_chunk = chunk;

	for (i = 0; i < nr_devs; i++) {
		args.dev_ids[i] = opts->dev_ids[i];
		args.devs[i].size = opts->size;
		args.devs[i].iterations = opts->iterations;
		args.devs[i].flags = tc->flags;
		if (opts->verify)
			args.devs[i].flags |= IDMA_BENCH_F_VERIFY;
	}

	ret = ioctl(fd, IDMA_IOC_RUN_MULTI_BENCH, &args);
	if (ret < 0) {
		saved_errno = errno;
		failed = 1;
	}

	for (i = 0; i < args.nr_devs; i++) {
		total_bytes += args.devs[i].bytes;
		total_active += args.devs[i].dma_active_cycles;
		if (args.devs[i].errors)
			failed = 1;
	}

	for (i = 0; i < args.nr_devs; i++) {
		const struct idma_bench_args *r = &args.devs[i];

		printf("%u,%u,%s,%u,%" PRIu64 ",%u,%u,%s,%s,"
		       "%u,%" PRIu64 ",%" PRIu64 ",%.6f,%" PRIu64
		       ",%.6f,%" PRIu64 ",%" PRIu64 ",%" PRIu64
		       ",%u,%" PRIu64 ",%" PRIu64 "\n",
		       repeat, args.nr_devs, tc->name, args.xfer_chunk,
		       r->size, i, args.dev_ids[i], mode_name(r->flags),
		       pattern_name(r->flags), r->iterations, r->bytes,
		       r->cycles, ratio_u64(r->cycles, r->bytes),
		       r->dma_active_cycles,
		       ratio_u64(r->dma_active_cycles, r->bytes),
		       r->iotlb_miss, r->s1_ptw, r->s2_ptw, r->errors,
		       total_bytes, total_active);
	}

	if (ret < 0) {
		fprintf(stderr,
			"multi ioctl failed: nr_devs=%u case=%s chunk=%u: %s\n",
			nr_devs, tc->name, chunk, strerror(saved_errno));
	}

	return failed;
}

static void init_options(struct options *opts)
{
	memset(opts, 0, sizeof(*opts));
	opts->ioctl_dev = "/dev/idma0";
	opts->dev_ids[0] = 0;
	opts->dev_ids[1] = 1;
	opts->dev_ids[2] = 2;
	opts->dev_ids[3] = 3;
	opts->nr_dev_ids = 4;
	opts->size = 2ULL << 20;
	opts->iterations = 20;
	opts->repeats = 3;
	opts->case_mask = CASE_ALL;
	opts->verify = 1;
	opts->csv_header = 1;
	add_chunk(opts, 256);
	add_chunk(opts, 1ULL << 10);
	add_chunk(opts, 4ULL << 10);
}

int main(int argc, char **argv)
{
	struct options opts;
	int fd;
	int status = 0;
	int i;
	uint32_t repeat;

	init_options(&opts);

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage(argv[0]);
			return 0;
		} else if (!strcmp(argv[i], "--ioctl-dev")) {
			if (++i >= argc)
				goto missing_arg;
			opts.ioctl_dev = argv[i];
		} else if (!strcmp(argv[i], "--devs")) {
			if (++i >= argc)
				goto missing_arg;
			parse_devs(&opts, argv[i]);
		} else if (!strcmp(argv[i], "--nr-devs")) {
			if (++i >= argc)
				goto missing_arg;
			opts.exact_nr_devs = parse_u32(argv[i], "nr-devs");
		} else if (!strcmp(argv[i], "--scale")) {
			opts.exact_nr_devs = 0;
		} else if (!strcmp(argv[i], "--case")) {
			if (++i >= argc)
				goto missing_arg;
			opts.case_mask = parse_case_mask(argv[i]);
		} else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
			if (++i >= argc)
				goto missing_arg;
			opts.size = parse_size(argv[i]);
		} else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--iters")) {
			if (++i >= argc)
				goto missing_arg;
			opts.iterations = parse_u32(argv[i], "iterations");
		} else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--repeats")) {
			if (++i >= argc)
				goto missing_arg;
			opts.repeats = parse_u32(argv[i], "repeats");
		} else if (!strcmp(argv[i], "--chunks")) {
			if (++i >= argc)
				goto missing_arg;
			parse_chunks(&opts, argv[i]);
		} else if (!strcmp(argv[i], "--chunk")) {
			if (++i >= argc)
				goto missing_arg;
			opts.nr_chunks = 0;
			add_chunk(&opts, parse_size(argv[i]));
		} else if (!strcmp(argv[i], "--verify")) {
			opts.verify = 1;
		} else if (!strcmp(argv[i], "--noverify")) {
			opts.verify = 0;
		} else if (!strcmp(argv[i], "--no-header")) {
			opts.csv_header = 0;
		} else {
			fprintf(stderr, "unknown option: %s\n", argv[i]);
			usage(argv[0]);
			return 1;
		}
	}

	if (!opts.nr_dev_ids ||
	    opts.nr_dev_ids > IDMA_MULTI_BENCH_MAX_DEVS) {
		fprintf(stderr, "invalid device list\n");
		return 1;
	}
	if (opts.exact_nr_devs > opts.nr_dev_ids) {
		fprintf(stderr, "nr-devs exceeds --devs length\n");
		return 1;
	}
	if (!opts.repeats) {
		fprintf(stderr, "repeats must be non-zero\n");
		return 1;
	}

	fd = open(opts.ioctl_dev, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s failed: %s\n", opts.ioctl_dev,
			strerror(errno));
		return 1;
	}

	if (opts.csv_header)
		print_header();

	for (repeat = 1; repeat <= opts.repeats; repeat++) {
		size_t c;

		for (c = 0; c < sizeof(bench_cases) / sizeof(bench_cases[0]); c++) {
			uint32_t nr_start;
			uint32_t nr_end;
			uint32_t chunk_idx;

			if (!(opts.case_mask & bench_cases[c].mask))
				continue;

			if (opts.exact_nr_devs) {
				nr_start = opts.exact_nr_devs;
				nr_end = opts.exact_nr_devs;
			} else {
				nr_start = 1;
				nr_end = opts.nr_dev_ids;
			}

			for (chunk_idx = 0; chunk_idx < opts.nr_chunks; chunk_idx++) {
				uint32_t nr;

				for (nr = nr_start; nr <= nr_end; nr++) {
					status |= run_one(fd, &opts, &bench_cases[c],
							  nr, opts.chunks[chunk_idx],
							  repeat);
				}
			}
		}
	}

	close(fd);
	return status ? 2 : 0;

missing_arg:
	fprintf(stderr, "missing argument for %s\n", argv[i - 1]);
	return 1;
}
