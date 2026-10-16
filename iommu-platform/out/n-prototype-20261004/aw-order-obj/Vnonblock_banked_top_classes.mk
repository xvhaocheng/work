# Verilated -*- Makefile -*-
# DESCRIPTION: Verilator output: Make include file with class lists
#
# This file lists generated Verilated files, for including in higher level makefiles.
# See Vnonblock_banked_top.mk for the caller.

### Switches...
# C11 constructs required?  0/1 (from --threads, --trace-threads or use of classes)
VM_C11 = 0
# Coverage output mode?  0/1 (from --coverage)
VM_COVERAGE = 0
# Parallel builds?  0/1 (from --output-split)
VM_PARALLEL_BUILDS = 1
# Threaded output mode?  0/1/N threads (from --threads)
VM_THREADS = 0
# Tracing output mode?  0/1 (from --trace/--trace-fst)
VM_TRACE = 0
# Tracing threaded output mode?  0/1/N threads (from --trace-thread)
VM_TRACE_THREADS = 0
# Separate FST writer thread? 0/1 (from --trace-fst with --trace-thread > 0)
VM_TRACE_FST_WRITER_THREAD = 0

### Object file lists...
# Generated module classes, fast-path, compile with highest optimization
VM_CLASSES_FAST += \
	Vnonblock_banked_top \
	Vnonblock_banked_top__1 \
	Vnonblock_banked_top__2 \
	Vnonblock_banked_top___024unit \
	Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4 \
	Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4__1 \
	Vnonblock_banked_top_axi_demux_id_counters__pi40 \
	Vnonblock_banked_top_axi_demux_id_counters__pi33 \

# Generated module classes, non-fast-path, compile with low/medium optimization
VM_CLASSES_SLOW += \
	Vnonblock_banked_top__Slow \
	Vnonblock_banked_top__1__Slow \
	Vnonblock_banked_top___024unit__Slow \
	Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4__Slow \
	Vnonblock_banked_top_axi_demux_id_counters__pi40__Slow \
	Vnonblock_banked_top_axi_demux_id_counters__pi33__Slow \

# Generated support classes, fast-path, compile with highest optimization
VM_SUPPORT_FAST += \

# Generated support classes, non-fast-path, compile with low/medium optimization
VM_SUPPORT_SLOW += \
	Vnonblock_banked_top__Syms \

# Global classes, need linked once per executable, fast-path, compile with highest optimization
VM_GLOBAL_FAST += \
	verilated \

# Global classes, need linked once per executable, non-fast-path, compile with low/medium optimization
VM_GLOBAL_SLOW += \


# Verilated -*- Makefile -*-
