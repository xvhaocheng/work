# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Cyril Koenig <cykoenig@iis.ee.ethz.ch>

export HERO_ROOT=$(pwd)
export CVA6_SDK=$HERO_ROOT/cva6-sdk
export HERO_INSTALL=$HERO_ROOT/install
export RISCV=$CVA6_SDK/buildroot/output/host/
export RV64_SYSROOT=$RISCV/riscv64-buildroot-linux-gnu/sysroot
export CROSS_COMPILE=$RISCV/bin/riscv64-buildroot-linux-gnu-

# A2 fixed author 6.9 tree; no mutable Buildroot query or 6.1 fallback.
export BR_LINUX_DIR=$CVA6_SDK/buildroot/output/build/linux-e4b0725a5089f6cc0fc5afec06f3091d98dffa68

# For Occamy bootrom
export UBOOT_SPL_BIN=$CVA6_SDK/install64/u-boot-spl.bin
export UBOOT_ITB=$CVA6_SDK/install64/u-boot.itb

# Add linux-gcc and clang to path
export PATH=$RISCV/bin:$PATH
export PATH=$HERO_INSTALL/bin:$PATH

export HERO_ARTIFACTS_ROOT=/usr/scratch2/wuerzburg/cykoenig/memora/hero

export CARFIELD_ROOT=$HERO_ROOT/platforms/carfield
export OCCAMY_ROOT=$HERO_ROOT/platforms/occamy
