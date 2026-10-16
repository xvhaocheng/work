#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$artifact_root"
cmake_bin="$artifact_root/build/llvm-tools-venv/bin/cmake"
llvm_src="$artifact_root/source/llvm-project-d8cffb8d7294483b45732283719e250116fbe6c0"
llvm_build="$artifact_root/build/llvm-minimal"
hero_root="$artifact_root/source/hero-tools"
export HERO_INSTALL="$hero_root/install"
export PATH="$HERO_INSTALL/bin:$PATH"
"$cmake_bin" --build "$llvm_build" --parallel 24 --target llvm-as llvm-ranlib llvm-nm llvm-objcopy
mkdir -p "$HERO_INSTALL/bin" "$HERO_INSTALL/lib"
for name in clang clang++ clang-offload-bundler opt llvm-link llvm-dis llvm-ar llvm-as llvm-ranlib llvm-nm llvm-objcopy llvm-objdump llvm-readelf llvm-config llvm-tblgen clang-tblgen ld.lld lld; do
  ln -sfn "$llvm_build/bin/$name" "$HERO_INSTALL/bin/$name"
done
ln -sfn "$llvm_build/lib/clang" "$HERO_INSTALL/lib/clang"
"$cmake_bin" -G Ninja -S "$hero_root/toolchain/llvm-support" \
  -B "$artifact_root/build/llvm-passes" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$HERO_INSTALL" -DLLVM_DIR="$llvm_build/lib/cmake/llvm"
"$cmake_bin" --build "$artifact_root/build/llvm-passes" --parallel 4
"$cmake_bin" --install "$artifact_root/build/llvm-passes"
cp "$hero_root/toolchain/llvm-support/hc-omp-pass" "$HERO_INSTALL/bin/"
newlib_src="$artifact_root/source/newlib-6d79e0a58866548f435527798fbd4a6849d05bc7"
[[ $(git -C "$newlib_src" rev-parse HEAD) == 6d79e0a58866548f435527798fbd4a6849d05bc7 ]]
mkdir -p "$artifact_root/build/newlib-rv32"
cd "$artifact_root/build/newlib-rv32"
"$newlib_src/configure" --target=riscv32-unknown-elf -nfp \
  --prefix="$HERO_INSTALL/rv32imafd-ilp32d" \
  AR_FOR_TARGET="$HERO_INSTALL/bin/llvm-ar" \
  AS_FOR_TARGET="$HERO_INSTALL/bin/clang -c -march=rv32imafd -mabi=ilp32d" \
  LD_FOR_TARGET="$HERO_INSTALL/bin/ld.lld" \
  RANLIB_FOR_TARGET="$HERO_INSTALL/bin/llvm-ranlib" \
  CC_FOR_TARGET="$HERO_INSTALL/bin/clang -march=rv32imafd -mabi=ilp32d" \
  CFLAGS_FOR_TARGET="-mno-relax -DMALLOC_PROVIDED=1"
make -j24 all-target-newlib all-target-libgloss
make install-target-newlib install-target-libgloss
cd "$artifact_root"
"$cmake_bin" -G Ninja -S "$llvm_src/compiler-rt" -B build/compiler-rt-rv32 \
  -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
  -DCMAKE_INSTALL_PREFIX="$HERO_INSTALL/lib/clang/15.0.0/rv32imafd-ilp32d" \
  -DCMAKE_C_COMPILER="$HERO_INSTALL/bin/clang" \
  -DCMAKE_C_COMPILER_TARGET=riscv32-unknown-elf \
  -DCMAKE_C_FLAGS="-march=rv32imafd -mabi=ilp32d -mno-relax" \
  -DCMAKE_ASM_FLAGS="-march=rv32imafd -mabi=ilp32d -mno-relax" \
  -DCMAKE_SYSROOT="$HERO_INSTALL/rv32imafd-ilp32d/riscv32-unknown-elf" \
  -DCOMPILER_RT_BAREMETAL_BUILD=ON -DCOMPILER_RT_BUILD_BUILTINS=ON \
  -DCOMPILER_RT_BUILD_MEMPROF=OFF -DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
  -DCOMPILER_RT_BUILD_PROFILE=OFF -DCOMPILER_RT_BUILD_SANITIZERS=OFF \
  -DCOMPILER_RT_BUILD_XRAY=OFF -DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON \
  -DCOMPILER_RT_OS_DIR="" -DLLVM_CONFIG_PATH="$llvm_build/bin/llvm-config"
"$cmake_bin" --build build/compiler-rt-rv32 --parallel 16
"$cmake_bin" --install build/compiler-rt-rv32
mkdir -p "$hero_root/platforms"
ln -sfn "$artifact_root/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0" "$hero_root/platforms/carfield"
make -C "$hero_root/platforms/carfield/sw/tests/bare-metal/snitchd" all
