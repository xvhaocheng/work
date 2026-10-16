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

struct idma_once_args {
	uint64_t size;

	uint64_t bytes;
	uint64_t cycles;
	uint64_t iotlb_miss;
	uint64_t s1_ptw;
	uint64_t s2_ptw;
	uint64_t dma_active_cycles;
	uint32_t errors;
	uint32_t reserved;
};

#define IDMA_IOC_RUN_ONCE	_IOWR(IDMA_IOC_MAGIC, 0x03, struct idma_once_args)

static void usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <size>\n", prog);
	fprintf(stderr, "Example: %s 4096\n", prog);
	fprintf(stderr, "         %s 2M\n", prog);
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

int main(int argc, char **argv)
{
	const char *dev = "/dev/idma0";
	struct idma_once_args args = {0};
	int fd;

	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	args.size = parse_size(argv[1]);
	if (!args.size) {
		fprintf(stderr, "size must be non-zero\n");
		return 1;
	}

	fd = open(dev, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s failed: %s\n", dev, strerror(errno));
		return 1;
	}

	if (ioctl(fd, IDMA_IOC_RUN_ONCE, &args) < 0) {
		fprintf(stderr,
			"IDMA_IOC_RUN_ONCE size=%" PRIu64 " failed: %s"
			" bytes=%" PRIu64 " cycles=%" PRIu64
			" dma_active_cycles=%" PRIu64 " errors=%u\n",
			args.size, strerror(errno), args.bytes, args.cycles,
			args.dma_active_cycles, args.errors);
		close(fd);
		return 1;
	}

	close(fd);

	printf("once size=%" PRIu64 " bytes=%" PRIu64
	       " cycles=%" PRIu64 " dma_active_cycles=%" PRIu64
	       " iotlb_miss=%" PRIu64 " s1_ptw=%" PRIu64
	       " s2_ptw=%" PRIu64 " errors=%u\n",
	       args.size, args.bytes, args.cycles, args.dma_active_cycles,
	       args.iotlb_miss, args.s1_ptw, args.s2_ptw, args.errors);

	return args.errors ? 2 : 0;
}
