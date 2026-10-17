/* Design draft only: no frozen kernel implements these ioctls. */
#ifndef E3_PROPOSED_IDMA_WORKLOAD_H
#define E3_PROPOSED_IDMA_WORKLOAD_H
#include <linux/idma.h>
#define IDMA_WORKLOAD_ABI 5U
#define IDMA_WORKLOAD_BATCH_MAGIC 0x49444d4142544303ULL
#define IDMA_WORKLOAD_OBSERVER_MAGIC 0x49444d414f425305ULL
#define IDMA_WORKLOAD_LINEAR_RING 0U
#define IDMA_WORKLOAD_PAGE_PACKED_RING 1U
#define IDMA_WORKLOAD_TILE_ROWS 2U
/* These maxima are not permission for arbitrary parameter combinations. */
struct idma_workload_caps {
    __u32 abi_version, struct_size, capabilities, device_id;
    __u32 did, max_pages, max_rows_per_op, max_operation_bytes;
    __u64 batch_magic, observer_magic;
    __u32 geometry_mask, row_length_mask, path_queue_depth, max_run_timeout_ms;
};
struct idma_workload_geometry {
    __u32 geometry, row_bytes, rows_per_operation, row_pitch_bytes;
    __u32 ring_pages, matrix_rows, columns, reserved0;
    __u64 reserved[4];
};
struct idma_workload_coverage {
    __u64 measured_distinct_bytes;
    __u32 measured_distinct_pages, warmup_distinct_pages;
    __u64 verified_union_bytes, reserved;
};
struct idma_workload_experiment {
    /* V0, optional A1. Supply=1 (HW batch). All flags/reserved must be zero. */
    __u32 abi_version, struct_size, flags, aggressor_id;
    __u32 run_timeout_ms, supply, reserved0, reserved1;
    __u64 victim_operations, victim_warmup_operations, reserved[2];
    struct idma_workload_geometry victim_geometry, aggressor_geometry;
    /* Output starts at byte192. Counters are row COPY descriptors, not tiles. */
    __s32 status;
    __u32 hpm_configured, measurement_started, measurement_complete;
    __u32 victim_did, aggressor_did;
    __u64 observer_sequence, victim_elapsed_ns, snapshot_before_ns, snapshot_after_ns, drain_ns;
    struct idma_experiment_stats victim, aggressor;
    __u64 victim_warmup_descriptors, aggressor_warmup_descriptors;
    __u64 victim_warmup_bytes, aggressor_warmup_bytes;
    struct idma_workload_coverage victim_coverage, aggressor_coverage;
};
#define IDMA_IOC_CAPS_WORKLOAD _IOR('i',0x0f,struct idma_workload_caps)
#define IDMA_IOC_RUN_WORKLOAD _IOWR('i',0x10,struct idma_workload_experiment)
#endif
