// SPDX-License-Identifier: GPL-2.0
/* Host-only harness: executes the exact frozen CLI parser and caps validator.
 * It never invokes the original main, opens a device, or sends an ioctl.
 * The capability values below are synthetic and are not a board observation.
 */
#define main pathv3_original_main
#include "source/idma_experiment.pathv3.c"
#undef main

int main(int argc, char **argv)
{
	struct options o = {
		.device = "/dev/idma0",
		.request = {
			.abi_version = IDMA_EXPERIMENT_ABI,
			.struct_size = sizeof(struct idma_experiment),
			.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR,
			.victim_pages = 1,
			.aggressor_pages = 8,
			.chunk_bytes = 4096,
			.run_timeout_ms = 60000,
			.victim_transfers = 4096,
			.warmup_transfers = 256,
		},
	};
	const struct idma_experiment_caps c = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(struct idma_experiment_caps),
		.capabilities = IDMA_EXPERIMENT_CAP_DID_HPM |
			IDMA_EXPERIMENT_CAP_SW_REFILL | IDMA_EXPERIMENT_CAP_HW_BATCH,
		.device_id = 0, .did = 10,
		.max_pages = 4096, .page_bytes = 4096, .max_run_timeout_ms = 300000,
	};
	char err[256] = "";
	int rc = parse(argc, argv, &o, err, sizeof(err));
	const char *stage = "arguments";

	if (!rc) {
		stage = "synthetic_caps";
		rc = validate_caps(&o, &c, err, sizeof(err));
	}
	printf("stage=%s accepted=%d message=%s\n", stage, rc == 0, err);
	return rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
