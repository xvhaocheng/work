#include <stddef.h>
#include <stdio.h>
#include <linux/idma_p0.h>
_Static_assert(sizeof(struct idma_experiment) == 328, "legacy ABI size changed");
_Static_assert(sizeof(struct idma_p0_experiment) == 328, "P0 ABI size");
_Static_assert(sizeof(struct idma_experiment_caps) == 64, "legacy caps changed");
_Static_assert(sizeof(struct idma_p0_caps) == 64, "P0 caps size");
_Static_assert(offsetof(struct idma_p0_experiment, status) == 80, "P0 input boundary");
_Static_assert(offsetof(struct idma_p0_experiment, victim_transfers) == 40, "P0 count offset");
_Static_assert(offsetof(struct idma_p0_experiment, reserved) == 56, "P0 reserved offset");
_Static_assert(offsetof(struct idma_p0_experiment, victim) == 136, "P0 result offset");
_Static_assert(IDMA_IOC_EXPERIMENT_CAPS == 0x80406906U, "legacy caps ioctl changed");
_Static_assert(IDMA_IOC_RUN_EXPERIMENT == 0xc1486907U, "legacy run ioctl changed");
_Static_assert(IDMA_IOC_RUN_BATCH == 0xc1486908U, "legacy batch ioctl changed");
_Static_assert(IDMA_IOC_CAPS_P0 == 0x80406909U, "P0 caps ioctl");
_Static_assert(IDMA_IOC_RUN_BATCH_P0 == 0xc148690aU, "P0 batch ioctl");
int main(void)
{
	printf("PASS ABI1 unchanged; P0 caps=64 run=328 input=80 reserved@56 victim@136\n");
	printf("CAPS_P0=%#lx RUN_BATCH_P0=%#lx\n",
	       (unsigned long)IDMA_IOC_CAPS_P0, (unsigned long)IDMA_IOC_RUN_BATCH_P0);
	return 0;
}
