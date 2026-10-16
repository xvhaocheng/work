#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$artifact_root"
mkdir -p evidence logs
riscv64-linux-gnu-gcc --version >evidence/gcc-version.txt
riscv64-linux-gnu-gcc -v 2>evidence/gcc-config.txt
riscv64-linux-gnu-gcc -Wall -Wextra -Werror=return-type -O2 -fopenmp \
  -Isource/hero-tools/sw/libhero/include -Isource/hero-tools/apps/omp/common \
  -c source/hero-tools/apps/omp/basic/axpy/axpy_main.c -o evidence/axpy_main.o
riscv64-linux-gnu-gcc -Wall -Wextra -Werror=return-type -O2 -fopenmp \
  -c source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c -o evidence/axpy_snitch_host.o
make -C source/hero-tools/sw/libhero HERO_HOST=cva6 \
  HERO_DEVICE=carfield_snitch_cluster CROSS_COMPILE=riscv64-linux-gnu-
cp source/hero-tools/sw/libhero/lib/libhero_carfield_snitch_cluster.a evidence/
# This checks the real device body and real fixed runtime headers with a
# standard RV32 GCC frontend; it is NOT the custom Hero LLVM offload build.
riscv64-linux-gnu-gcc -O2 -ffreestanding -march=rv32imafd_zicsr -mabi=ilp32d \
  -D__HERO_DEV -D__device= -Wno-unknown-pragmas -Isource/carfield-runtime \
  -Isource/hero-tools/apps/omp/common \
  -isystem source/hero-tools/install/rv32imafd-ilp32d/riscv32-unknown-elf/include \
  -c source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c \
  -o evidence/axpy_snitch-rv32-compat.o
riscv64-linux-gnu-objdump -dr evidence/axpy_snitch-rv32-compat.o \
  >evidence/axpy_snitch-rv32-compat.dis
file evidence/*.o evidence/*.a >evidence/object-identities.txt
