PYTHON ?= python3
PROFILE ?= migration-4dma
JOBS ?= 12

.PHONY: help doctor check bench rootfs kernel firmware software hpm-test fpga bundle
help:
	@echo 'doctor/check  : tools and pinned-source checks'
	@echo 'software      : benchmark + rootfs + Linux + OpenSBI payload'
	@echo 'hpm-test      : directed test of the actual HPM RTL'
	@echo 'fpga          : full FPGA build in out/<profile>/fpga-src'
	@echo 'PROFILE=migration-4dma (DS 50/50) or baseline-4dma (DS 0/0)'
	@echo 'bundle        : self-contained source bundles for offline restoration'
doctor check bench rootfs kernel firmware software hpm-test fpga bundle:
	$(PYTHON) scripts/platform.py $@ --profile $(PROFILE) --jobs $(JOBS)
