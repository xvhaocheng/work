# Copyright 2020 ETH Zurich and University of Bologna.
# Solderpad Hardware License, Version 0.51, see LICENSE for details.
# SPDX-License-Identifier: SHL-0.51

HERO_ROOT = $(realpath ../../..)

# A2 fixed enabled-IOVA profile. scripts/setenv.sh points at this exact author
# snapshot; there is no native or 6.1 kernel fallback in this profile.
PLATFORM := BUILDROOT
KERNEL_DIR := $(HERO_ROOT)/cva6-sdk/buildroot/output/build/linux-e4b0725a5089f6cc0fc5afec06f3091d98dffa68
BR_OUTPUT_DIR := $(HERO_ROOT)/cva6-sdk/buildroot/output/
CROSS_COMPILE := $(BR_OUTPUT_DIR)/host/bin/riscv64-buildroot-linux-gnu-

# Linux compilation arguments
obj-m := carfield.o
carfield-objs := carfield_driver.o carfield_fops.o ../common/hero_iommu.o ../common/hero_platform_gate.o
ccflags-y += -I$(src)/../common -Idrivers/iommu/riscv -DNATIVE=1 -DBUILDROOT=2 -DHERO_PLATFORM=$(PLATFORM)

.PHONY: all dis clean build

all: modules
build: modules

modules:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(shell pwd) ARCH=riscv modules

dis: modules
	$(CROSS_COMPILE)objdump -DS carfield.ko > carfield.disasm

deploy:
	cp carfield.ko $(HERO_ROOT)/cva6-sdk/rootfs/root

clean:
	rm -f modules.order
	rm -f *.mod
	rm -f *.o
	rm -f *.ko
	rm -rf *.mod.c
	rm -f Module.symvers
	rm -f .*.o.cmd
	rm -f .*.ko.cmd
	rm -f *.disasm
	rm -f *.o_shipped
	rm -f *.cmd
	rm -f .*.cmd
	rm -f ../common/*.o.*
