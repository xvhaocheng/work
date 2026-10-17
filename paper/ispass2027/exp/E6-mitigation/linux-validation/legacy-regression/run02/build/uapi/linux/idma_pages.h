/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_PAGES_H
#define _UAPI_LINUX_IDMA_PAGES_H

#include <linux/idma.h>

#define IDMA_PAGES_ABI 4U
#define IDMA_PAGES_BUFFER_BYTES 2097152U
#define IDMA_PAGES_CAP_HPM (1U << 0)
#define IDMA_PAGES_CAP_BATCH (1U << 1)
#define IDMA_PAGES_CAP_PATH (1U << 2)
#define IDMA_PAGES_CAP_LEAF_API (1U << 3)
#define IDMA_PAGES_MAP_4K (1U << 0)
#define IDMA_PAGES_MAP_2M (1U << 1)

struct idma_pages_caps {
	__u32 abi_version;
	__u32 struct_size;
	__u32 capabilities;
	__u32 device_id;
	__u32 did;
	__u32 kernel_page_bytes;
	__u32 buffer_bytes;
	__u32 chunk_bytes;
	__u32 mapping_page_mask;
	__u32 max_run_timeout_ms;
	__u32 reserved[6];
};

struct idma_pages_buffer {
	__u64 iova;
	__u64 phys;
	__u64 bytes;
	__u64 mapped_bytes;
	__u64 unmapped_bytes;
	__u64 before_first_pte;
	__u64 before_last_pte;
	__u64 after_first_pte;
	__u64 after_last_pte;
	__u64 first_leaf_iova_base;
	__u64 last_leaf_iova_base;
	__u32 requested_page_bytes;
	__u32 leaf_shift_min;
	__u32 leaf_shift_max;
	__u32 verified_before_4k_pages;
	__u32 verified_after_4k_pages;
	__u32 empty_after_cleanup;
	__s32 before_error;
	__s32 after_error;
	__s32 unmap_error;
	__u32 reserved;
};

/*
 * Fixed V0/A1: each source/destination is 2MiB (8MiB total DMA buffers).
 * COPY4096, 512-page ring, measured4096, warmup256. Only mapping_page_bytes
 * differs (4096 or 2097152); allocation method/alignment is identical.
 * All input flags/reserved fields must be zero. No fallback or retry.
 */
struct idma_pages_experiment {
	__u32 abi_version;
	__u32 struct_size;
	__u32 flags;
	__u32 mapping_page_bytes;
	__u32 buffer_bytes;
	__u32 chunk_bytes;
	__u32 run_timeout_ms;
	__u32 reserved0;
	__u64 victim_transfers;
	__u64 warmup_transfers;
	__u32 reserved[8];
	/* Output buffer bits: Vsrc=0,Vdst=1,Asrc=2,Adst=3; lane bits V=0,A=1. */
	__s32 status;
	__u32 prepared_lane_mask;
	__u32 verified_before_mask;
	__u32 verified_after_mask;
	__u32 quiet_lane_mask;
	__u32 unmapped_buffer_mask;
	__u32 freed_buffer_mask;
	__u32 retained_lane_mask;
	__u64 observer_sequence;
	/* Sequential software reads before STOP; not atomic V-window A progress. */
	__u64 pre_stop_A_control;
	__u64 pre_stop_A_issued;
	__u32 A_status_valid;
	__u32 reserved_output;
	/* ABI1-shaped inputs/results; HPM software bracket, final A includes drain. */
	struct idma_experiment experiment;
	struct idma_pages_buffer buffers[4];
};

#define IDMA_IOC_CAPS_PAGES _IOR('i', 0x0d, struct idma_pages_caps)
#define IDMA_IOC_RUN_BATCH_PAGES _IOWR('i', 0x0e, struct idma_pages_experiment)

#endif /* _UAPI_LINUX_IDMA_PAGES_H */
