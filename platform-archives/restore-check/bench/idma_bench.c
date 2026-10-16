// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
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

#define IDMA_IOC_RUN_BENCH	_IOWR(IDMA_IOC_MAGIC, 0x01, struct idma_bench_args)

static uint64_t parse_size(const char *s)
{
	char *end;
	uint64_t value = strtoull(s, &end, 0);

	if (!*end)
		return value;
	if (!strcmp(end, "k") || !strcmp(end, "K"))
		return value << 10;
	if (!strcmp(end, "m") || !strcmp(end, "M"))
		return value << 20;

	fprintf(stderr, "invalid size: %s\n", s);
	exit(1);
}

static void run_one(int fd, uint64_t size, uint32_t iterations,
		    uint32_t flags, const char *name)
{
	char label[32];
	struct idma_bench_args args = {
		.size = size,
		.iterations = iterations,
		.flags = flags,
	};

	snprintf(label, sizeof(label), "%s%s", name,
		 (flags & IDMA_BENCH_F_RANDOM) ? "-random" : "");

	if (ioctl(fd, IDMA_IOC_RUN_BENCH, &args) < 0) {
		fprintf(stderr, "%s ioctl failed: %s\n", label, strerror(errno));
		exit(1);
	}

	printf("%s size=%" PRIu64 " iter=%u bytes=%" PRIu64
	       " cycles=%" PRIu64 " cycles/byte=%.4f"
	       " dma_active_cycles=%" PRIu64 " dma_active_cycles/byte=%.4f"
	       " iotlb_miss=%" PRIu64 " s1_ptw=%" PRIu64
	       " s2_ptw=%" PRIu64 " errors=%u\n",
	       label, args.size, args.iterations, args.bytes, args.cycles,
	       args.bytes ? (double)args.cycles / (double)args.bytes : 0.0,
	       args.dma_active_cycles,
	       args.bytes ? (double)args.dma_active_cycles / (double)args.bytes : 0.0,
	       args.iotlb_miss, args.s1_ptw,
	       args.s2_ptw, args.errors);
}

int main(int argc, char **argv)
{
	const char *dev = "/dev/idma0";
	const char *mode = "both";
	uint64_t size = 2ULL << 20;
	uint32_t iterations = 1;
	uint32_t flags = IDMA_BENCH_F_VERIFY;
	int i;
	int fd;

	if (argc > 1)
		size = parse_size(argv[1]);
	if (argc > 2)
		iterations = strtoul(argv[2], NULL, 0);
	for (i = 3; i < argc; i++) {
		if (!strcmp(argv[i], "noverify"))
			flags &= ~IDMA_BENCH_F_VERIFY;
		else if (!strcmp(argv[i], "verify"))
			flags |= IDMA_BENCH_F_VERIFY;
		else if (!strcmp(argv[i], "random"))
			flags |= IDMA_BENCH_F_RANDOM;
		else if (!strcmp(argv[i], "linear"))
			flags &= ~IDMA_BENCH_F_RANDOM;
		else if (!strcmp(argv[i], "4k") ||
			 !strcmp(argv[i], "2m") ||
			 !strcmp(argv[i], "dma") ||
			 !strcmp(argv[i], "both"))
			mode = argv[i];
		else if (!strncmp(argv[i], "dev=", 4))
			dev = argv[i] + 4;
		else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dev")) {
			if (++i >= argc) {
				fprintf(stderr, "%s requires a device path\n", argv[i - 1]);
				return 1;
			}
			dev = argv[i];
		}
		else {
			fprintf(stderr, "unknown option: %s\n", argv[i]);
			return 1;
		}
	}

	fd = open(dev, O_RDWR);
	if (fd < 0) {
		perror(dev);
		return 1;
	}

	if (!strcmp(mode, "4k") || !strcmp(mode, "both"))
		run_one(fd, size, iterations, flags, "4k");
	if (!strcmp(mode, "2m") || !strcmp(mode, "both"))
		run_one(fd, size, iterations, flags | IDMA_BENCH_F_HUGE, "2m");
	if (!strcmp(mode, "dma"))
		run_one(fd, size, iterations, flags | IDMA_BENCH_F_DMA_API, "dma");

	close(fd);
	return 0;
}
