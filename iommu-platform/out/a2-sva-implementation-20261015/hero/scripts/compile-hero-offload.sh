#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
hero_root="$artifact_root/source/hero-tools"
# Must be built from toolchain/llvm-project@d8cffb8d7294483b45732283719e250116fbe6c0.
# These exact tools/runtimes were actually built by the companion scripts.
# Host headers/libs use the explicitly labelled Ubuntu GCC11/glibc2.35 adapter;
# this is a compile variant, not proof of the paper's Buildroot execution.
# The host wrapper preserves EH while translating the proved 61->57 ABI pair.
export HERO_ROOT="$hero_root"
export HERO_INSTALL="$hero_root/install"
export CARFIELD_ROOT="$hero_root/platforms/carfield"
export CROSS_COMPILE=riscv64-linux-gnu-
export PATH="$HERO_INSTALL/bin:$PATH"
make -C "$hero_root/apps/omp/basic/axpy" HERO_DEVICE=carfield_snitch_cluster \
  RISCV=/usr RV64_SYSROOT="$artifact_root/build/rv64-ubuntu-sysroot" HOST_LD="$artifact_root/scripts/hero-host-ld-compat.py" \
  HOST_OBJDUMP=/usr/bin/riscv64-linux-gnu-objdump all
