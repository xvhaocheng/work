/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_MULTI_H
#define _UAPI_LINUX_IDMA_MULTI_H

#include <linux/idma.h>

#define IDMA_MULTI_ABI 3U
#define IDMA_MULTI_MAX_DEVICES 4U
#define IDMA_MULTI_MAX_PAGES 8U
#define IDMA_MULTI_CAP_HW_BATCH (1U << 0)
#define IDMA_MULTI_CAP_ALL_DID_HPM (1U << 1)
#define IDMA_MULTI_CAP_SELECTED_PATH (1U << 2)

struct idma_multi_caps {
	__u32 abi_version;
	__u32 struct_size;
	__u32 capabilities;
	__u32 device_id;
	__u32 did;
	__u32 max_devices;
	__u32 max_pages;
	__u32 page_bytes;
	__u32 max_run_timeout_ms;
	__u32 reserved[7];
};

struct idma_multi_experiment {
	/*
	 * ABI3's bounded E2 contract: V0, A1..(nr-1), selected A index1;
	 * V one page, all A equal pages: nr2 {1,6,8}, nr3 {1,3,8}, nr4 {1,2,8}.
	 * COPY4096, measured4096, warmup256. All flags/reserved/unused slots=0.
	 * Slot0 is V; remaining slots are A in launch order.
	 */
	__u32 abi_version;
	__u32 struct_size;
	__u32 flags;
	__u32 nr_devices;
	__u32 device_id[IDMA_MULTI_MAX_DEVICES];
	__u32 pages[IDMA_MULTI_MAX_DEVICES];
	__u32 path_aggressor_index;
	__u32 chunk_bytes;
	__u32 run_timeout_ms;
	__u32 reserved0;
	__u64 victim_transfers;
	__u64 warmup_transfers;
	__u32 reserved[8];
	/* Output: masks identify valid lifecycle evidence even after failure. */
	__s32 status;
	__u32 hpm_configured;
	__u32 measurement_started;
	__u32 measurement_complete;
	__u32 did[IDMA_MULTI_MAX_DEVICES];
	__u32 prepared_mask;
	__u32 start_attempted_mask;
	__u32 stopped_mask;
	__u32 drained_mask;
	__u32 verified_mask;
	__u32 stop_status_valid_mask;
	__u64 observer_sequence;
	__u64 victim_elapsed_ns;
	/* Sequential software snapshots bracket V; not atomic HW window counts. */
	__u64 snapshot_before_ns;
	__u64 snapshot_after_ns;
	__u64 drain_ns;
	/* A completed/bytes include startup + final drain, not exact V-window work. */
	struct idma_experiment_stats stats[IDMA_MULTI_MAX_DEVICES];
	/* Monotonic software times; not simultaneous hardware launch timestamps. */
	__u64 start_ns[IDMA_MULTI_MAX_DEVICES];
	__u64 stop_ns[IDMA_MULTI_MAX_DEVICES];
	__u64 drain_end_ns[IDMA_MULTI_MAX_DEVICES];
	__u64 pre_stop_status[IDMA_MULTI_MAX_DEVICES];
	__u64 pre_stop_issued[IDMA_MULTI_MAX_DEVICES];
};

#define IDMA_IOC_CAPS_MULTI _IOR('i', 0x0b, struct idma_multi_caps)
#define IDMA_IOC_RUN_MULTI_BATCH _IOWR('i', 0x0c, struct idma_multi_experiment)

#endif
