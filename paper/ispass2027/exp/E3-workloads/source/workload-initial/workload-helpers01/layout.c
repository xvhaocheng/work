#include <stddef.h>
#include <linux/idma_workload.h>
_Static_assert(sizeof(struct idma_workload_caps)==64,"caps");
_Static_assert(sizeof(struct idma_workload_geometry)==64,"geometry");
_Static_assert(sizeof(struct idma_workload_coverage)==32,"coverage");
_Static_assert(sizeof(struct idma_experiment_stats)==96,"old stats unchanged");
_Static_assert(sizeof(struct idma_workload_experiment)==544,"run");
_Static_assert(offsetof(struct idma_workload_experiment,victim_geometry)==64,"V geometry");
_Static_assert(offsetof(struct idma_workload_experiment,aggressor_geometry)==128,"A geometry");
_Static_assert(offsetof(struct idma_workload_experiment,status)==192,"output prefix");
_Static_assert(offsetof(struct idma_workload_experiment,observer_sequence)==216,"sequence");
_Static_assert(offsetof(struct idma_workload_experiment,victim)==256,"V stats");
_Static_assert(offsetof(struct idma_workload_experiment,aggressor)==352,"A stats");
_Static_assert(offsetof(struct idma_workload_experiment,victim_coverage)==480,"V coverage");
_Static_assert(offsetof(struct idma_workload_experiment,aggressor_coverage)==512,"A coverage");
_Static_assert(IDMA_IOC_CAPS_WORKLOAD==0x8040690fUL,"caps opcode");
_Static_assert(IDMA_IOC_RUN_WORKLOAD==0xc2206910UL,"run opcode");
int main(void){return 0;}
