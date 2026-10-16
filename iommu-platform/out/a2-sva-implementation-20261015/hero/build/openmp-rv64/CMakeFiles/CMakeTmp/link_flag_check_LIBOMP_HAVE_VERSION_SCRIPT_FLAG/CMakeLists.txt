cmake_minimum_required(VERSION 3.13.4)
     project(foo C)
     set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--version-script=/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/llvm-project-d8cffb8d7294483b45732283719e250116fbe6c0/openmp/runtime/src/exports_so.txt")
     add_library(foo SHARED src_to_link.c)