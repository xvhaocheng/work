#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$artifact_root"
cmake_bin="$artifact_root/build/llvm-tools-venv/bin/cmake"
llvm_src="$artifact_root/source/llvm-project-d8cffb8d7294483b45732283719e250116fbe6c0"
hero_root="$artifact_root/source/hero-tools"
prefix="$artifact_root/build/rv64-runtime-deps"
export HERO_DEV_INC_DIR="$hero_root/sw/libhero/include"
export HERO_LIBHERO_DIR="$hero_root/sw/libhero"
"$cmake_bin" -G Ninja -S "$llvm_src/openmp" -B build/openmp-rv64 \
 -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=riscv64 \
 -DCMAKE_C_COMPILER=/usr/bin/riscv64-linux-gnu-gcc \
 -DCMAKE_CXX_COMPILER=/usr/bin/riscv64-linux-gnu-g++ \
 -DCMAKE_C_FLAGS=-march=rv64imafdc -DCMAKE_CXX_FLAGS=-march=rv64imafdc \
 -DLLVM_DIR="$artifact_root/build/llvm-minimal/lib/cmake/llvm" \
 -DLIBOMPTARGET_HERO_BUILD=ON -DLIBOMPTARGET_HERO_DEV_TYPE=carfield_snitch_cluster \
 -DLIBOMPTARGET_BUILD_CUDA_PLUGIN=OFF -DLIBOMPTARGET_BUILD_AMDGPU_PLUGIN=OFF \
 -DOPENMP_ENABLE_LIBOMPTARGET_PROFILING=OFF -DOPENMP_ENABLE_OMPT_TOOLS=OFF \
 -DOPENMP_ENABLE_LIBOMPTARGET=ON \
 -DLIBOMPTARGET_DEP_LIBFFI_INCLUDE_DIR="$prefix/include" \
 -DLIBOMPTARGET_DEP_LIBFFI_LIBRARIES="$prefix/lib/libffi.so" \
 -DLIBOMPTARGET_DEP_LIBELF_INCLUDE_DIR="$prefix/include" \
 -DLIBOMPTARGET_DEP_LIBELF_LIBRARIES="$prefix/lib/libelf.so"
"$cmake_bin" --build build/openmp-rv64 --parallel 16 \
 --target omp omptarget omptarget.rtl.herodev_carfield_snitch_cluster
mkdir -p "$hero_root/sw/libomp/lib"
cp build/openmp-rv64/runtime/src/libomp.so "$hero_root/sw/libomp/lib/"
cp build/openmp-rv64/libomptarget/libomptarget.so "$hero_root/sw/libomp/lib/"
cp build/openmp-rv64/libomptarget/libomptarget.rtl.herodev_carfield_snitch_cluster.so \
 "$hero_root/sw/libomp/lib/"
