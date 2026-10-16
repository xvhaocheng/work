/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_H
#define _UAPI_LINUX_IDMA_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define IDMA_EXPERIMENT_ABI 1U
#define IDMA_EXPERIMENT_MAX_TRANSFERS (1ULL << 32)
#define IDMA_EXPERIMENT_NO_AGGRESSOR ((__u32)~0U)
#define IDMA_EXPERIMENT_CAP_DID_HPM (1U << 0)
#define IDMA_EXPERIMENT_CAP_SW_REFILL (1U << 1)
#define IDMA_EXPERIMENT_CAP_HW_BATCH (1U << 2)
#define IDMA_EXPERIMENT_CAP_E3_PAIR (1U << 3)
#define IDMA_EXPERIMENT_CAP_E3_DIAG (1U << 4)
#define IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR (1U << 5)
#define IDMA_EXPERIMENT_CAP_E3_CONTROLLED (1U << 6)
#define IDMA_BATCH_MAX_TRANSFERS (1ULL << 20)
/* HPM capability is preliminary; acquire checks slots/filter readback too. */
/* Neither capability nor configuration establishes board calibration. */
struct idma_experiment_caps {
	__u32 abi_version;
	__u32 struct_size;
	__u32 capabilities;
	__u32 device_id;
	__u32 did;
	__u32 max_pages;
	__u32 page_bytes;
	__u32 max_run_timeout_ms;
	__u32 reserved[8];
};

struct idma_experiment_stats {
	__u64 submitted;
	__u64 completed;
	__u64 completed_bytes;
	__u64 active_cycles;
	__u64 iotlb_miss;
	__u64 s1_ptw;
	__u64 s2_ptw;
	/* Observed completion to observed resubmission, incl. MMIO; not HW idle. */
	__u64 refill_gap_ns_sum;
	__u64 refill_gap_ns_max;
	__u64 refill_gap_samples;
	/* Submitted but not yet observed complete in software; not HW overlap. */
	__u32 in_flight_at_start;
	__u32 in_flight_at_end;
	__u32 data_errors;
	__u32 quarantined;
};

struct idma_experiment {
	/* Input: all reserved words and flags must be zero in ABI 1. */
	__u32 abi_version;
	__u32 struct_size;
	__u32 flags;
	__u32 aggressor_id;
	__u32 victim_pages;
	__u32 aggressor_pages;
	__u32 chunk_bytes;
	__u32 run_timeout_ms;
	__u64 victim_transfers;
	__u64 warmup_transfers;
	__u32 reserved[8];
	/* Output: status is a negative errno or zero, even when ioctl fails. */
	__s32 status;
	__u32 victim_did;
	__u32 aggressor_did;
	__u32 hpm_configured;
	__u32 measurement_started;
	__u32 measurement_complete;
	__u64 victim_elapsed_ns;
	/* Snapshot intervals bracket V's window; HPM is not an atomic snapshot. */
	__u64 snapshot_before_ns;
	__u64 snapshot_after_ns;
	__u64 drain_ns;
	struct idma_experiment_stats victim;
	struct idma_experiment_stats aggressor;
};

/* Old commands 0x01..0x05 are retired and return ENOTTY. */
#define IDMA_IOC_EXPERIMENT_CAPS _IOR('i', 0x06, struct idma_experiment_caps)
#define IDMA_IOC_RUN_EXPERIMENT _IOWR('i', 0x07, struct idma_experiment)

/* Separate operation/JSON semantics, same validated input layout; reserved=0.
 * Batch stats count hardware tasks; refill gaps are not sampled. A counts include
 * its startup and final drain, rather than just the V hardware window.
 */
#define IDMA_IOC_RUN_BATCH _IOWR('i', 0x08, struct idma_experiment)

#define IDMA_E3_WORDS 256U
/* One allocation/mapping lifetime, two separately warmed/drained/verified rounds.
 * first_delay is 0 or 128; the other value is used for the second round.
 * Raw words are versioned observer-v4 bank indices 192..447, read after drain.
 */
struct idma_e3_pair {
	__u32 abi_version, struct_size;
	__u32 first_delay, reserved;
	struct idma_experiment request;
	__s32 status;
	__u32 rounds_attempted, rounds_completed, reserved_output;
	__u64 sequence[2];
	/* V src/dst IOVA, V src/dst PA, A src/dst IOVA, A src/dst PA. */
	__u64 mapping[8];
	struct idma_experiment result[2];
	__u64 observation[2][IDMA_E3_WORDS];
};
#define IDMA_IOC_RUN_E3_PAIR _IOWR('i', 0x09, struct idma_e3_pair)

/* Diagnostic-only D0/D0. Old pair ABI and complementary delays stay unchanged.
 * All timestamps are software CLOCK_MONOTONIC ns, not hardware acceptance.
 * Warm HPM includes the final A drain; counts are software observed tasks.
 * Zero timestamps mean that the corresponding stage was not reached.
 */
struct idma_e3_diag_lane {
	__u64 warm_submitted, warm_completed, warm_drained;
	__u64 warm_last_offset, warm_next_offset;
	__u64 launch_begin_ns, launch_end_ns;
	__u32 warm_in_flight, reserved;
	struct idma_experiment_stats warm_hpm;
};
struct idma_e3_diag_round {
	__u64 warm_begin_ns, warm_end_ns, warm_drained_ns;
	__u64 arm_begin_ns, arm_end_ns;
	/* V then A. HPM snapshot complete and both warm streams drained. */
	__u32 warm_complete, reserved;
	struct idma_e3_diag_lane lane[2];
};
struct idma_e3_diag {
	/* Input uses the unchanged pair size; first_delay must be zero. */
	struct idma_e3_pair pair;
	/* Output only, cleared by the kernel before validation. */
	struct idma_e3_diag_round diagnostic[2];
};
#define IDMA_IOC_RUN_E3_DIAG _IOWR('i', 0x0a, struct idma_e3_diag)

/* Same diagnostic layout; complementary D0/D128 and first_delay 0 or 128.
 * Requires observer v5 / E3 bank v2 with full waiting-queue trigger coverage.
 * Old D0/D0 operation above is unchanged. */
#define IDMA_IOC_RUN_E3_QUEUE_PAIR _IOWR('i', 0x0b, struct idma_e3_diag)

#define IDMA_E3_CONTROLLED_WORDS 32U

/*
 * Controlled finite E3 round.  Inputs are carried in the embedded pair and
 * prep_mode/pair_mode; all other fields are outputs cleared by the kernel.
 *
 * prep_mode: 0 cold (no A prime), 1 hot (one A prime).
 * pair_mode: 0 complementary D0/D128 (first_delay 0 or 128), 1 D0/D0.
 *
 * flush_status is -EINPROGRESS until the mandatory global S1 flush returns.
 * stage follows 1..8 (flush, A prime, V prime, V probe, prepare, running,
 * drain, complete) and is only 8 when the whole round succeeded.
 * prime[0]/prime[1]/probe carry independent per-phase statistics.
 * launcher[] is the raw 32-word launcher record copied on every reachable
 * path.  v_hw_cycles is observer bank index 0, not the HPM/MMIO span.
 */
struct idma_e3_controlled_round {
	__s32 flush_status;
	__u32 prep_complete;
	__u32 stage;
	__u32 reserved;
	struct idma_experiment_stats prime[2];
	struct idma_experiment_stats probe;
	__u64 launcher[IDMA_E3_CONTROLLED_WORDS];
	__u64 observer_magic;
	__u64 v_hw_cycles;
};

struct idma_e3_controlled {
	struct idma_e3_pair pair;
	__u32 prep_mode;
	__u32 pair_mode;
	__u32 reserved[6];
	struct idma_e3_controlled_round controlled[2];
};

#define IDMA_IOC_RUN_E3_CONTROLLED _IOWR('i', 0x0c, struct idma_e3_controlled)

#endif
