# 1 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 369 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c" 2







void init_omp_target() {
#pragma omp target device(1)
    asm volatile("nop");
}

int global_seed = 2048;
