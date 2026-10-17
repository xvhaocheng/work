// SPDX-License-Identifier: GPL-2.0
/* Offline compile-time layout contract for the uninstalled ABI3 proposal. */
#include <stddef.h>
#include <stdio.h>
#include "idma_multi.h.proposal"

_Static_assert(sizeof(struct idma_experiment) == 328, "legacy ABI unchanged");
_Static_assert(sizeof(struct idma_multi_caps) == 64, "multi caps size");
_Static_assert(offsetof(struct idma_multi_experiment, status) == 112, "input boundary");
_Static_assert(offsetof(struct idma_multi_experiment, stats) == 208, "stats offset");
_Static_assert(sizeof(struct idma_multi_experiment) == 752, "multi request size");
_Static_assert(_IOC_NR(IDMA_IOC_CAPS_MULTI) == 0x0b, "caps command");
_Static_assert(_IOC_NR(IDMA_IOC_RUN_MULTI_BATCH) == 0x0c, "run command");
_Static_assert(_IOC_SIZE(IDMA_IOC_RUN_MULTI_BATCH) == 752, "ioctl size");

int main(void)
{
	printf("legacy=%zu caps=%zu request=%zu status_offset=%zu stats_offset=%zu\n",
	       sizeof(struct idma_experiment), sizeof(struct idma_multi_caps),
	       sizeof(struct idma_multi_experiment),
	       offsetof(struct idma_multi_experiment, status),
	       offsetof(struct idma_multi_experiment, stats));
	return 0;
}
