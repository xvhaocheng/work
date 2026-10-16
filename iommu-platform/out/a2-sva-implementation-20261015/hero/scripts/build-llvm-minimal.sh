#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cmake_bin="$artifact_root/build/llvm-tools-venv/bin/cmake"
llvm_src="$artifact_root/source/llvm-project-d8cffb8d7294483b45732283719e250116fbe6c0"
llvm_build="$artifact_root/build/llvm-minimal"
hero_install="$artifact_root/source/hero-tools/install"
[[ $(git -C "$llvm_src" rev-parse HEAD) == d8cffb8d7294483b45732283719e250116fbe6c0 ]]
git -C "$llvm_src" diff --quiet
"$cmake_bin" -G Ninja -C "$artifact_root/config/llvm-host-cache.cmake" \
  -DCMAKE_INSTALL_PREFIX="$hero_install" -S "$llvm_src/llvm" -B "$llvm_build"
"$cmake_bin" --build "$llvm_build" --parallel 24 --target \
  clang clang-offload-bundler opt llvm-link llvm-dis llvm-ar llvm-objdump \
  llvm-readelf llvm-config llvm-tblgen clang-tblgen lld
