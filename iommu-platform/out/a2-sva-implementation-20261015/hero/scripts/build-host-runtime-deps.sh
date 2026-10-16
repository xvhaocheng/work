#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$artifact_root"
prefix="$artifact_root/build/rv64-runtime-deps"
mkdir -p "$prefix" build/libffi-rv64 build/elfutils-rv64
cd build/libffi-rv64
"$artifact_root/source/libffi-3.4.2/configure" --host=riscv64-linux-gnu \
 --prefix="$prefix" --disable-exec-static-tramp --disable-docs \
 CC=riscv64-linux-gnu-gcc CXX=riscv64-linux-gnu-g++
make -j16
make install
cd "$artifact_root/source/zlib-1.2.11"
CHOST=riscv64-linux-gnu CC=riscv64-linux-gnu-gcc ./configure --shared --prefix="$prefix"
make -j16
make install
cd "$artifact_root/build/elfutils-rv64"
"$artifact_root/source/elfutils-0.184/configure" --host=riscv64-linux-gnu \
 --prefix="$prefix" --disable-debuginfod --disable-libdebuginfod --disable-progs \
 --without-bzlib --without-lzma --without-zstd --disable-nls --disable-werror \
 CC=riscv64-linux-gnu-gcc CXX=riscv64-linux-gnu-g++ \
 CPPFLAGS="-I$prefix/include" LDFLAGS="-L$prefix/lib"
make -C lib -j16
make -C libelf -j16
make -C libelf install
