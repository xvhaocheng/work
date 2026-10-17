/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_P0_H
#define _UAPI_LINUX_IDMA_P0_H

#include <linux/idma.h>

/* Separate ABI/commands; linux/idma.h ABI 1 layout and operations are unchanged.
 * P0 is V0, optional A1, V one page, A one/eight pages, packed ring, 4096 tasks
 * after 256 software-warmup tasks. V length is 64/1024/4096; A length is 4096.
 * Every input flags/reserved word must be zero. No fallback to software refill.
 */
#define IDMA_P0_ABI 2U
#define IDMA_P0_BATCH_MAGIC 0x49444d4142544302ULL
#define IDMA_P0_GEOMETRY_PACKED_RING 1U
#define IDMA_P0_GEOMETRY_MASK_PACKED_RING (1U << 0)
#define IDMA_P0_LENGTH_64 (1U << 0)
#define IDMA_P0_LENGTH_1024 (1U << 1)
#define IDMA_P0_LENGTH_4096 (1U << 2)

struct idma_p0_caps {
	__u32 abi_version;
	__u32 struct_size;
	__u32 capabilities; /* Existing DID_HPM/HW_BATCH bit definitions. */
	__u32 device_id;
	__u32 did;
	__u32 max_pages; /* Allocation limit; P0's stricter geometry applies. */
	__u32 page_bytes;
	__u32 max_run_timeout_ms;
	__u64 batch_magic;
	__u32 victim_length_mask;
	__u32 aggressor_length_mask;
	__u32 geometry_mask;
	__u32 reserved[3];
};

struct idma_p0_experiment {
	__u32 abi_version;
	__u32 struct_size;
	__u32 flags;
	__u32 aggressor_id;
	__u32 victim_pages;
	__u32 aggressor_pages;
	__u32 victim_chunk_bytes;
	__u32 aggressor_chunk_bytes;
	__u32 run_timeout_ms;
	__u32 geometry;
	__u64 victim_transfers;
	__u64 warmup_transfers;
	__u32 reserved[6];
	/* Same output meanings as ABI 1, with per-lane completed_bytes = count*len.
	 * HPM is still a non-atomic software bracket. A counts include startup/drain.
	 */
	__s32 status;
	__u32 victim_did;
	__u32 aggressor_did;
	__u32 hpm_configured;
	__u32 measurement_started;
	__u32 measurement_complete;
	__u64 victim_elapsed_ns;
	__u64 snapshot_before_ns;
	__u64 snapshot_after_ns;
	__u64 drain_ns;
	struct idma_experiment_stats victim;
	struct idma_experiment_stats aggressor;
};

#define IDMA_IOC_CAPS_P0 _IOR('i', 0x09, struct idma_p0_caps)
#define IDMA_IOC_RUN_BATCH_P0 _IOWR('i', 0x0a, struct idma_p0_experiment)

#endif
