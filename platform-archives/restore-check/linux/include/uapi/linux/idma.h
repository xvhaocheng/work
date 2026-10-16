/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_H
#define _UAPI_LINUX_IDMA_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define IDMA_IOC_MAGIC		'i'

#define IDMA_BENCH_F_HUGE	(1U << 0)
#define IDMA_BENCH_F_VERIFY	(1U << 1)
#define IDMA_BENCH_F_DMA_API	(1U << 2)
#define IDMA_BENCH_F_RANDOM	(1U << 3)

#define IDMA_PAIR_AGGRESSOR_NONE	((__u32)~0U)
#define IDMA_MULTI_BENCH_MAX_DEVS	8

struct idma_bench_args {
	__u64 size;
	__u32 iterations;
	__u32 flags;

	__u64 bytes;
	__u64 cycles;
	__u64 iotlb_miss;
	__u64 s1_ptw;
	__u64 s2_ptw;
	__u64 dma_active_cycles;
	__u32 errors;
	__u32 reserved;
};

struct idma_once_args {
	__u64 size;

	__u64 bytes;
	__u64 cycles;
	__u64 iotlb_miss;
	__u64 s1_ptw;
	__u64 s2_ptw;
	__u64 dma_active_cycles;
	__u32 errors;
	__u32 reserved;
};

struct idma_pair_bench_args {
	__u32 aggressor_id;
	__u32 xfer_chunk;

	struct idma_bench_args victim;
	struct idma_bench_args aggressor;
};

struct idma_multi_bench_args {
	__u32 nr_devs;
	__u32 xfer_chunk;
	__u32 dev_ids[IDMA_MULTI_BENCH_MAX_DEVS];
	__u32 reserved[6];

	struct idma_bench_args devs[IDMA_MULTI_BENCH_MAX_DEVS];
};

#define IDMA_IOC_RUN_PAIR_BENCH	_IOWR(IDMA_IOC_MAGIC, 0x02, struct idma_pair_bench_args)
#define IDMA_IOC_RUN_ONCE	_IOWR(IDMA_IOC_MAGIC, 0x03, struct idma_once_args)
#define IDMA_IOC_RUN_MULTI_BENCH	_IOWR(IDMA_IOC_MAGIC, 0x04, struct idma_multi_bench_args)
#define IDMA_IOC_RUN_PAIR_BENCH_INDEPENDENT \
	_IOWR(IDMA_IOC_MAGIC, 0x05, struct idma_pair_bench_args)

#endif /* _UAPI_LINUX_IDMA_H */
