/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_G_H
#define _UAPI_LINUX_IDMA_G_H

#include <linux/idma.h>

/* Independent G protocol: V0/optional A1, fixed whole work, no STOP on success.
 * Warmup is 256 completed software COPYs per participating lane, gap0, drained
 * before ARM; it is excluded from each measured 4096-COPY hardware batch.
 * All G gap conditions share this protocol; do not mix with older pair ABIs.
 */
#define IDMA_G_ABI 6U
#define IDMA_G_TRANSFERS 4096U
#define IDMA_G_WARMUP_TRANSFERS 256U
#define IDMA_G_CHUNK_BYTES 4096U
#define IDMA_G_MAX_TIMEOUT_MS 300000U
#define IDMA_G_BATCH_MAGIC 0x49444d4142544302ULL
#define IDMA_G_OBSERVER_MAGIC 0x49444d414f425304ULL
#define IDMA_G_GAP_MAGIC 0x49444d4147415001ULL
#define IDMA_G_BANK_MAGIC 0x49444d4150344d02ULL
#define IDMA_G_BANK_FIRST 768U
#define IDMA_G_BANK_WORDS 32U
#define IDMA_G_GAP_0 (1U << 0)
#define IDMA_G_GAP_1000 (1U << 1)
#define IDMA_G_GAP_2000 (1U << 2)
#define IDMA_G_CAP_HPM (1U << 0)
#define IDMA_G_CAP_BATCH (1U << 1)
#define IDMA_G_CAP_GAP (1U << 2)
#define IDMA_G_CAP_WHOLE (1U << 3)
#define IDMA_G_CAP_HIST (1U << 4)
/* Set only when the driver implements RUN_G, not merely when RTL is present. */
#define IDMA_G_CAP_RUN_READY (1U << 5)

/* Software snapshot checks, separate from raw hardware error mask words[3]. */
#define IDMA_G_BAD_CAP (1U << 0)
#define IDMA_G_BAD_RESERVED (1U << 1)
#define IDMA_G_BAD_FLAGS (1U << 2)
#define IDMA_G_HARDWARE_ERROR (1U << 3)
#define IDMA_G_BAD_CONFIG (1U << 4)
#define IDMA_G_BAD_BUDGET (1U << 5)
#define IDMA_G_BAD_TIME (1U << 6)
#define IDMA_G_BAD_WINDOW (1U << 7)
#define IDMA_G_BAD_IDS (1U << 8)
#define IDMA_G_BAD_SOLO (1U << 9)

struct idma_g_caps {
	__u32 abi_version, struct_size, capabilities, device_id;
	__u32 did, page_bytes, victim_pages, aggressor_pages;
	__u32 chunk_bytes, measured_transfers, warmup_transfers, max_run_timeout_ms;
	__u64 batch_magic, observer_magic, gap_magic, bank_magic;
	__u32 gap_mask, bank_first, bank_words, reserved;
};

struct idma_g_experiment {
	/* Input ends at byte96; flags and every reserved word must be zero.
	 * Solo requires aggressor_id=NO_AGGRESSOR and A pages/count/gap all zero.
	 * Otherwise A is DMA1, eight pages, 4096 COPYs, gap0/1000/2000 cycles.
	 */
	__u32 abi_version, struct_size, flags, aggressor_id;
	__u32 victim_pages, aggressor_pages, chunk_bytes, run_timeout_ms;
	__u64 victim_transfers, aggressor_transfers, warmup_transfers;
	__u32 victim_gap_cycles, aggressor_gap_cycles, reserved[8];
	/* Output: masks use V bit0/A bit1. No hardware sequence/generation exists;
	 * observer_sequence is the existing locked software sequence only.
	 */
	__s32 status;
	__u32 observer_validation_errors, victim_did, aggressor_did;
	__u32 hpm_configured, measurement_started, measurement_complete, prepared_lane_mask;
	__u32 quiet_lane_mask, retained_lane_mask, verified_lane_mask, reserved_output;
	__u64 observer_sequence, warmup_completed[2], warmup_bytes[2];
	__u64 victim_elapsed_ns, whole_software_elapsed_ns;
	/* HPM uses the non-atomic software V bracket, not hardware A whole time. */
	__u64 snapshot_before_ns, snapshot_after_ns, drain_ns, old_observer_status;
	/* Exact read-only words768..799, in order, including both reserved zeros.
	 * Own V whole cycles=words[10]; own A whole cycles=words[13]. A complete
	 * 16MiB throughput uses words[13], never V window words[16].
	 */
	__u64 words[IDMA_G_BANK_WORDS];
	struct idma_experiment_stats victim, aggressor;
};

#define IDMA_IOC_CAPS_G _IOR('i', 0x11, struct idma_g_caps)
#define IDMA_IOC_RUN_G _IOWR('i', 0x12, struct idma_g_experiment)

#endif
