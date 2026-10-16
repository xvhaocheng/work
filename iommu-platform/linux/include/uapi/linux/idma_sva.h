/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_IDMA_SVA_H
#define _UAPI_LINUX_IDMA_SVA_H

#include <linux/ioctl.h>
#include <linux/types.h>

/* Private, synchronous, fixed-task research ABI; not generic Linux SVA. */
#define IDMA_SVA_ABI 1U
#define IDMA_SVA_PAGE_BYTES 4096U
#define IDMA_SVA_ARENA_PAGES 8U
#define IDMA_SVA_DATA_PAGES 7U
#define IDMA_SVA_ARENA_BYTES (IDMA_SVA_PAGE_BYTES * IDMA_SVA_ARENA_PAGES)
#define IDMA_SVA_DESCRIPTORS 4U
#define IDMA_SVA_PACKETS 3U
#define IDMA_SVA_PAYLOAD_BYTES 1639U
#define IDMA_SVA_RECORD_BYTES 1675U
#define IDMA_SVA_HEADER_BYTES 12U
#define IDMA_SVA_SOURCE_OFFSET 32U

#define IDMA_SVA_MODE_IOVA 1U
#define IDMA_SVA_MODE_SHADOW 2U
#define IDMA_SVA_MODE_BORROWED_MM 3U
#define IDMA_SVA_MODE_BIT(mode) (1U << ((mode) - 1U))

#define IDMA_SVA_CAP_SYNCHRONOUS (1U << 0)
#define IDMA_SVA_CAP_FIXED_TASK (1U << 1)
#define IDMA_SVA_CAP_SINGLE_HART_FENCE_I (1U << 2)
#define IDMA_SVA_CAP_QUIESCENT_CONTEXT (1U << 3)

#define IDMA_SVA_RESULT_CPU_OWNED (1U << 0)
#define IDMA_SVA_RESULT_CONTEXT_RESTORED (1U << 1)
#define IDMA_SVA_RESULT_PINS_RELEASED (1U << 2)
#define IDMA_SVA_RESULT_SAFE (IDMA_SVA_RESULT_CPU_OWNED | \
	IDMA_SVA_RESULT_CONTEXT_RESTORED | IDMA_SVA_RESULT_PINS_RELEASED)

/* CAPS input: abi_version, struct_size, peer_index; all other words zero.
 * A mode bit is usable only when the complete binding/cache profile passed
 * its runtime checks. Capability is not FPGA execution/performance evidence.
 */
struct idma_sva_caps {
	__u32 abi_version, struct_size, peer_index, flags;
	__u32 capabilities, address_modes, primary_index, primary_did;
	__u32 peer_did, page_bytes, arena_pages, data_pages;
	__u32 descriptors, packets, max_timeout_ms, reserved0;
	__u32 reserved[8];
};

/* Fixed descriptor offsets: 32->16396/60, 4128->20492/65,
 * 8224->20557/503, 12320->24588/1011; lanes 0,1,1,0.
 * Destination headers are page+0..11, payload starts page+12.
 * RUN consumes initialized private anonymous RW memory of exactly eight pages.
 * RUN never returns while hardware can still access arena/mm/page tables.
 * A timeout is not cancellation: unsafe hardware drain must retain resources
 * and block the syscall rather than report a releasable failed task.
 * Four public-DONE jobs run globally in order d0,d1,d2,d3. Success transfers
 * all bytes to CPU ownership, restores contexts, and releases all task pins.
 * Userspace independently checks data/guards before the actual file consumer.
 */
struct idma_sva_run {
	__u32 abi_version, struct_size, flags, address_mode;
	__u32 peer_index, timeout_ms, reserved0[2];
	__aligned_u64 arena_user;
	__aligned_u64 arena_bytes;
	__u32 reserved[4];
	/* All output fields cleared by the kernel before validating input. */
	__s32 status, restore_status;
	__u32 result_flags, submitted, completed, primary_did, peer_did;
	__u32 pinned_pages, mapped_pages, domain_pt_pages;
	__aligned_u64 transfer_id[IDMA_SVA_DESCRIPTORS];
	__aligned_u64 device_src[IDMA_SVA_DESCRIPTORS];
	__aligned_u64 device_dst[IDMA_SVA_DESCRIPTORS];
	__u32 reserved_output[8];
};

#define IDMA_IOC_SVA_CAPS _IOWR('i', 0x12, struct idma_sva_caps)
#define IDMA_IOC_SVA_RUN _IOWR('i', 0x13, struct idma_sva_run)

#endif
