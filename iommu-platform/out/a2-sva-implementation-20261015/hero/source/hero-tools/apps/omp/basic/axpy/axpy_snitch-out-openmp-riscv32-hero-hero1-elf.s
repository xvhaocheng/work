	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_zfh1p0_xdma0p1_xfalthalf0p1_xfaltquarter0p1_xfauxalthalf0p1_xfauxaltquarter0p1_xfauxhalf0p1_xfauxquarter0p1_xfauxvecalthalf0p1_xfauxvecaltquarter0p1_xfauxvechalf0p1_xfauxvecquarter0p1_xfauxvecsingle0p1_xfexpauxvecalthalf0p1_xfexpauxvecaltquarter0p1_xfexpauxvechalf0p1_xfexpauxvecquarter0p1_xfquarter0p1_xfrep0p1_xfvecalthalf0p1_xfvecaltquarter0p1_xfvechalf0p1_xfvecquarter0p1_xfvecsingle0p1_xssr0p1"
	.file	"axpy_snitch.c"
	.file	0 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" "axpy_snitch.c" md5 0xd4e1bcc458f5a3c3132146de425bd950
	.file	1 "axpy_snitch.c"
	.file	2 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools" "install/rv32imafd-ilp32d/riscv32-unknown-elf/include/machine/_default_types.h"
	.file	3 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools" "install/rv32imafd-ilp32d/riscv32-unknown-elf/include/sys/_stdint.h"
	.protected	dev_axpy                # -- Begin function dev_axpy
	.globl	dev_axpy
	.p2align	2
	.type	dev_axpy,@function
dev_axpy:                               # @dev_axpy
.Lfunc_begin0:
	.loc	1 54 0                          # axpy_snitch.c:54:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:n <- $x14
	#DEBUG_VALUE: dev_axpy:n <- $x14
	addi	sp, sp, -64
	.cfi_def_cfa_offset 64
	sw	ra, 60(sp)                      # 4-byte Folded Spill
	sw	s0, 56(sp)                      # 4-byte Folded Spill
	sw	s1, 52(sp)                      # 4-byte Folded Spill
	sw	s2, 48(sp)                      # 4-byte Folded Spill
	sw	s3, 44(sp)                      # 4-byte Folded Spill
	sw	s4, 40(sp)                      # 4-byte Folded Spill
	sw	s5, 36(sp)                      # 4-byte Folded Spill
	sw	s6, 32(sp)                      # 4-byte Folded Spill
	sw	s7, 28(sp)                      # 4-byte Folded Spill
	sw	s8, 24(sp)                      # 4-byte Folded Spill
	sw	s9, 20(sp)                      # 4-byte Folded Spill
	sw	s10, 16(sp)                     # 4-byte Folded Spill
	sw	s11, 12(sp)                     # 4-byte Folded Spill
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	.cfi_offset s2, -16
	.cfi_offset s3, -20
	.cfi_offset s4, -24
	.cfi_offset s5, -28
	.cfi_offset s6, -32
	.cfi_offset s7, -36
	.cfi_offset s8, -40
	.cfi_offset s9, -44
	.cfi_offset s10, -48
	.cfi_offset s11, -52
	mv	a6, a2
	mv	a7, a1
	mv	s10, a0
.Ltmp0:
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x13
	.file	4 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools" "platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h"
	.loc	4 28 2 prologue_end             # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:28:2
	#APP
	csrr	a0, mhartid
	#NO_APP
.Ltmp1:
	#DEBUG_VALUE: pulp_get_core_id:r <- $x10
	.loc	1 60 10                         # axpy_snitch.c:60:10
	or	a1, s10, a1
	snez	a1, a1
	or	a2, a2, a3
	snez	a2, a2
	.loc	1 60 17 is_stmt 0               # axpy_snitch.c:60:17
	and	a1, a1, a2
	snez	a2, a4
	and	a1, a1, a2
	andi	a2, a4, 511
	seqz	a2, a2
	and	a1, a1, a2
.Ltmp2:
	.loc	4 30 10 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:30:10
	addi	t2, a0, -16
.Ltmp3:
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	.loc	1 60 17                         # axpy_snitch.c:60:17
	beqz	a1, .LBB0_4
.Ltmp4:
# %bb.1:
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:n <- $x14
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 0 17 is_stmt 0                # axpy_snitch.c:0:17
	mv	t0, a4
.Ltmp5:
	#DEBUG_VALUE: dev_axpy:n <- $x5
	mv	a5, a3
.Ltmp6:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	1 66 9 is_stmt 1                # axpy_snitch.c:66:9
	beqz	t2, .LBB0_6
.Ltmp7:
# %bb.2:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 0 9 is_stmt 0                 # axpy_snitch.c:0:9
	li	a0, 8
	.loc	1 67 9 is_stmt 1                # axpy_snitch.c:67:9
	bne	t2, a0, .LBB0_8
.Ltmp8:
# %bb.3:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 68 18                         # axpy_snitch.c:68:18
	lui	a0, %hi(dma_time)
	sw	zero, %lo(dma_time)(a0)
	.loc	1 69 20                         # axpy_snitch.c:69:20
	lui	a0, %hi(issue_time)
	sw	zero, %lo(issue_time)(a0)
	lui	a0, %hi(all_time)
	addi	a0, a0, %lo(all_time)
	j	.LBB0_7
.Ltmp9:
.LBB0_4:
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:n <- $x14
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 61 12                         # axpy_snitch.c:61:12
	bnez	t2, .LBB0_45
.Ltmp10:
# %bb.5:
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:n <- $x14
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 62 13                         # axpy_snitch.c:62:13
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	li	a1, 0
	lw	ra, 60(sp)                      # 4-byte Folded Reload
	lw	s0, 56(sp)                      # 4-byte Folded Reload
	lw	s1, 52(sp)                      # 4-byte Folded Reload
	lw	s2, 48(sp)                      # 4-byte Folded Reload
	lw	s3, 44(sp)                      # 4-byte Folded Reload
	lw	s4, 40(sp)                      # 4-byte Folded Reload
	lw	s5, 36(sp)                      # 4-byte Folded Reload
	lw	s6, 32(sp)                      # 4-byte Folded Reload
	lw	s7, 28(sp)                      # 4-byte Folded Reload
	lw	s8, 24(sp)                      # 4-byte Folded Reload
	lw	s9, 20(sp)                      # 4-byte Folded Reload
	lw	s10, 16(sp)                     # 4-byte Folded Reload
.Ltmp11:
	lw	s11, 12(sp)                     # 4-byte Folded Reload
	addi	sp, sp, 64
.Ltmp12:
	tail	printf_
.Ltmp13:
.LBB0_6:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 0 13 is_stmt 0                # axpy_snitch.c:0:13
	lui	a0, %hi(compute_time)
	addi	a0, a0, %lo(compute_time)
.Ltmp14:
.LBB0_7:                                # %.sink.split
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	snez	t3, t2
	sw	zero, 0(a0)
	j	.LBB0_9
.Ltmp15:
.LBB0_8:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	li	t3, 0
.Ltmp16:
.LBB0_9:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp17:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp18:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB0_11
.Ltmp19:
# %bb.10:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp20:
	#DEBUG_VALUE: tmp <- $x10
.LBB0_11:                               # %pulp_barrier.exit
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
.Ltmp21:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp22:
	.loc	1 75 8                          # axpy_snitch.c:75:8
	beqz	t3, .LBB0_13
.Ltmp23:
# %bb.12:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp24:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 75 33                         # axpy_snitch.c:75:33
	lui	a1, %hi(all_timer)
	sw	a0, %lo(all_timer)(a1)
.Ltmp25:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp26:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 78 21                         # axpy_snitch.c:78:21
	lui	t4, %hi(issue_timer)
	sw	a0, %lo(issue_timer)(t4)
.Ltmp27:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x17
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x26
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 254 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	mv	a3, a7
	mv	a2, s10
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	lui	a0, %hi(l1_buf_x)
	addi	a0, a0, %lo(l1_buf_x)
.Ltmp28:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	.loc	4 0 5 is_stmt 0                 # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:0:5
	lui	a1, 1
	addi	a4, a1, -2048
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp29:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x15
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x16
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- undef
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	lui	a0, %hi(l1_buf_y)
	addi	a0, a0, %lo(l1_buf_y)
.Ltmp30:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	.loc	4 254 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	mv	a3, a5
	mv	a2, a6
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp31:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp32:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 81 42                         # axpy_snitch.c:81:42
	lw	a1, %lo(issue_timer)(t4)
	.loc	1 81 20 is_stmt 0               # axpy_snitch.c:81:20
	lui	a2, %hi(issue_time)
	lw	a3, %lo(issue_time)(a2)
	.loc	1 81 40                         # axpy_snitch.c:81:40
	sub	a0, a0, a1
	.loc	1 81 20                         # axpy_snitch.c:81:20
	add	a0, a0, a3
	sw	a0, %lo(issue_time)(a2)
.Ltmp33:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- undef
.LBB0_13:                               # %.lr.ph
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:itr <- 0
	#DEBUG_VALUE: i <- 0
	.loc	1 0 20                          # axpy_snitch.c:0:20
	mv	t1, a7
	li	a1, 512
	mv	a0, t0
.Ltmp34:
	.loc	1 85 5 is_stmt 1                # axpy_snitch.c:85:5
	bltu	a1, t0, .LBB0_15
.Ltmp35:
# %bb.14:                               # %.lr.ph
	#DEBUG_VALUE: i <- 0
	#DEBUG_VALUE: dev_axpy:itr <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 0 5 is_stmt 0                 # axpy_snitch.c:0:5
	li	a0, 512
.Ltmp36:
.LBB0_15:                               # %.lr.ph
	#DEBUG_VALUE: i <- 0
	#DEBUG_VALUE: dev_axpy:itr <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	li	ra, 0
	li	t4, 0
	addi	t5, t0, -512
	.loc	1 85 5                          # axpy_snitch.c:85:5
	addi	a0, a0, -1
	srli	t6, a0, 9
	lui	a0, 1
	addi	a4, a0, -2048
	lui	s0, %hi(dma_timer)
	lui	s1, %hi(dma_time)
	lui	a0, %hi(barrier_reg)
	addi	s2, a0, %lo(barrier_reg)
	lui	s3, %hi(issue_timer)
	lui	a0, %hi(l1_buf_x)
	addi	s4, a0, %lo(l1_buf_x)
	lui	a0, %hi(l1_buf_y)
	addi	s5, a0, %lo(l1_buf_y)
	lui	s6, %hi(compute_timer)
	lui	s7, %hi(compute_time)
	lui	s8, %hi(issue_time)
	slli	s9, t2, 8
	mv	s11, a4
	j	.LBB0_18
.Ltmp37:
.LBB0_16:                               # %.thread91
                                        #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_plus_uconst 512, DW_OP_stack_value] $x27
	beq	t6, t4, .LBB0_38
.Ltmp38:
.LBB0_17:                               # %.backedge
                                        #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_plus_uconst 512, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 138 12 is_stmt 1              # axpy_snitch.c:138:12
	addi	t4, t4, 1
.Ltmp39:
	.loc	1 0 0 is_stmt 0                 # axpy_snitch.c:0:0
	addi	a0, s11, 1024
	addi	s11, a0, 1024
.Ltmp40:
	addi	t5, t5, -512
.Ltmp41:
.LBB0_18:                               # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: rest <- [DW_OP_LLVM_arg 0, DW_OP_constu 512, DW_OP_minus, DW_OP_LLVM_arg 0, DW_OP_minus, DW_OP_stack_value] undef
	.loc	1 88 12 is_stmt 1               # axpy_snitch.c:88:12
	beqz	t3, .LBB0_24
.Ltmp42:
# %bb.19:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: rest <- [DW_OP_LLVM_arg 0, DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 18446744073709551104, DW_OP_mul, DW_OP_consts 18446744073709551104, DW_OP_LLVM_arg 0, DW_OP_plus, DW_OP_plus, DW_OP_stack_value] undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp43:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 89 23                         # axpy_snitch.c:89:23
	sw	a0, %lo(dma_timer)(s0)
.Ltmp44:
	.loc	4 448 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:448:5
	#APP
.Ltmp45:
	.word	136315563
	bnez	t0, .Ltmp45

	#NO_APP
.Ltmp46:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp47:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 91 44                         # axpy_snitch.c:91:44
	lw	a1, %lo(dma_timer)(s0)
	.loc	1 91 22 is_stmt 0               # axpy_snitch.c:91:22
	lw	a2, %lo(dma_time)(s1)
	.loc	1 91 42                         # axpy_snitch.c:91:42
	sub	a0, a0, a1
	.loc	1 91 22                         # axpy_snitch.c:91:22
	add	a0, a0, a2
	sw	a0, %lo(dma_time)(s1)
.Ltmp48:
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp49:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp50:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB0_21
.Ltmp51:
# %bb.20:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp52:
	#DEBUG_VALUE: tmp <- $x10
.LBB0_21:                               # %pulp_barrier.exit69
                                        #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
.Ltmp53:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp54:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp55:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 95 25                         # axpy_snitch.c:95:25
	sw	a0, %lo(issue_timer)(s3)
	.loc	1 96 16                         # axpy_snitch.c:96:16
	blez	t5, .LBB0_23
.Ltmp56:
# %bb.22:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 97 83                         # axpy_snitch.c:97:83
	not	a0, t4
	andi	a0, a0, 1
	.loc	1 97 67 is_stmt 0               # axpy_snitch.c:97:67
	slli	t0, a0, 11
	.loc	1 97 95                         # axpy_snitch.c:97:95
	add	a2, s11, s10
	sltu	a0, a2, s11
	add	a3, t1, a0
.Ltmp57:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_convert 32 7, DW_OP_LLVM_convert 64 7, DW_OP_stack_value] undef
	.loc	1 97 67                         # axpy_snitch.c:97:67
	add	a0, t0, s4
.Ltmp58:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x12
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x12
	.loc	4 254 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp59:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	1 98 95                         # axpy_snitch.c:98:95
	add	a2, s11, a6
.Ltmp60:
	sltu	a0, a2, s11
	add	a3, a5, a0
.Ltmp61:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_convert 32 7, DW_OP_LLVM_convert 64 7, DW_OP_stack_value] undef
	.loc	1 98 67 is_stmt 0               # axpy_snitch.c:98:67
	add	a0, t0, s5
.Ltmp62:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x12
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x12
	.loc	4 254 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp63:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
.LBB0_23:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp64:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 100 45                        # axpy_snitch.c:100:45
	lw	a1, %lo(issue_timer)(s3)
	.loc	1 100 43 is_stmt 0              # axpy_snitch.c:100:43
	sub	ra, a0, a1
.Ltmp65:
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	.loc	1 0 43                          # axpy_snitch.c:0:43
	j	.LBB0_29
.Ltmp66:
.LBB0_24:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp67:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp68:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB0_26
.Ltmp69:
# %bb.25:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp70:
	#DEBUG_VALUE: tmp <- $x10
.LBB0_26:                               # %pulp_barrier.exit71
                                        #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
.Ltmp71:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp72:
	.loc	1 105 16                        # axpy_snitch.c:105:16
	bnez	t2, .LBB0_28
.Ltmp73:
# %bb.27:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp74:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 105 45                        # axpy_snitch.c:105:45
	sw	a0, %lo(compute_timer)(s6)
.Ltmp75:
.LBB0_28:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 110 71                        # axpy_snitch.c:110:71
	andi	a0, t4, 1
	.loc	1 110 58 is_stmt 0              # axpy_snitch.c:110:58
	slli	a0, a0, 11
	add	a0, a0, s9
	add	a1, a0, s4
.Ltmp76:
	#DEBUG_VALUE: px <- $x11
	.loc	1 111 58 is_stmt 1              # axpy_snitch.c:111:58
	add	a0, a0, s5
.Ltmp77:
	#DEBUG_VALUE: py <- $x10
	.loc	1 112 13                        # axpy_snitch.c:112:13
	li	a2, 64
	#APP
	li	t0, 0
.Ltmp78:
	flw	ft1, 0(a1)
	flw	ft2, 0(a0)
	fmadd.s	ft2, fa0, ft1, ft2
	fsw	ft2, 0(a0)
	addi	a1, a1, 4
	addi	a0, a0, 4
	addi	t0, t0, 1
	blt	t0, a2, .Ltmp78

	#NO_APP
.Ltmp79:
	#DEBUG_VALUE: py <- undef
	#DEBUG_VALUE: px <- undef
.LBB0_29:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	.loc	4 40 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp80:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp81:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB0_31
.Ltmp82:
# %bb.30:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp83:
	#DEBUG_VALUE: tmp <- $x10
.LBB0_31:                               # %pulp_barrier.exit73
                                        #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
.Ltmp84:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp85:
	.loc	1 129 12                        # axpy_snitch.c:129:12
	beqz	t2, .LBB0_33
.Ltmp86:
# %bb.32:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 131 12                        # axpy_snitch.c:131:12
	beqz	t3, .LBB0_16
	j	.LBB0_34
.Ltmp87:
.LBB0_33:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp88:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 129 62                        # axpy_snitch.c:129:62
	lw	a1, %lo(compute_timer)(s6)
	.loc	1 129 40 is_stmt 0              # axpy_snitch.c:129:40
	lw	a2, %lo(compute_time)(s7)
	.loc	1 129 60                        # axpy_snitch.c:129:60
	sub	a0, a0, a1
	.loc	1 129 40                        # axpy_snitch.c:129:40
	add	a0, a0, a2
	sw	a0, %lo(compute_time)(s7)
.Ltmp89:
	.loc	1 131 12 is_stmt 1              # axpy_snitch.c:131:12
	beqz	t3, .LBB0_16
.Ltmp90:
.LBB0_34:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	t0, mcycle
	#NO_APP
.Ltmp91:
	#DEBUG_VALUE: __tmp <- $x5
	.loc	1 132 57                        # axpy_snitch.c:132:57
	lw	a7, %lo(compute_timer)(s6)
.Ltmp92:
	#DEBUG_VALUE: compute_diff <- undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp93:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 134 25                        # axpy_snitch.c:134:25
	sw	a0, %lo(issue_timer)(s3)
	.loc	1 135 50                        # axpy_snitch.c:135:50
	addi	a1, s11, -2048
	.loc	1 135 48 is_stmt 0              # axpy_snitch.c:135:48
	add	a0, a1, a6
	sltu	a1, a0, a1
	.loc	1 135 102                       # axpy_snitch.c:135:102
	andi	a2, t4, 1
	.loc	1 135 90                        # axpy_snitch.c:135:90
	slli	a2, a2, 11
	add	a2, a2, s5
	.loc	1 135 48                        # axpy_snitch.c:135:48
	add	a1, a5, a1
.Ltmp94:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- $x11
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_convert 32 7, DW_OP_LLVM_convert 64 7, DW_OP_stack_value] undef
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x12
	.loc	4 254 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	li	a3, 0
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp95:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp96:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 136 46                        # axpy_snitch.c:136:46
	lw	a1, %lo(issue_timer)(s3)
.Ltmp97:
	.loc	1 136 24 is_stmt 0              # axpy_snitch.c:136:24
	lw	a2, %lo(issue_time)(s8)
.Ltmp98:
	.loc	1 136 88                        # axpy_snitch.c:136:88
	sub	a3, a7, t0
	add	a3, ra, a3
	.loc	1 136 44                        # axpy_snitch.c:136:44
	bltu	ra, a3, .LBB0_36
.Ltmp99:
# %bb.35:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: __tmp <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __tmp <- $x5
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 0 44                          # axpy_snitch.c:0:44
	add	a0, a0, a3
.Ltmp100:
.LBB0_36:                               #   in Loop: Header=BB0_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	1 136 58                        # axpy_snitch.c:136:58
	sub	a0, a0, a1
	.loc	1 136 24                        # axpy_snitch.c:136:24
	add	a0, a0, a2
	sw	a0, %lo(issue_time)(s8)
.Ltmp101:
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_plus_uconst 512, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	.loc	1 85 5 is_stmt 1                # axpy_snitch.c:85:5
	bne	t6, t4, .LBB0_17
.Ltmp102:
# %bb.37:                               # %._crit_edge
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp103:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 144 19                        # axpy_snitch.c:144:19
	lui	a1, %hi(dma_timer)
	sw	a0, %lo(dma_timer)(a1)
.Ltmp104:
	.loc	4 448 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:448:5
	#APP
.Ltmp105:
	.word	136315563
	bnez	t0, .Ltmp105

	#NO_APP
.Ltmp106:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp107:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 146 40                        # axpy_snitch.c:146:40
	lw	a1, %lo(dma_timer)(a1)
	.loc	1 146 18 is_stmt 0              # axpy_snitch.c:146:18
	lui	a2, %hi(dma_time)
	lw	a3, %lo(dma_time)(a2)
	.loc	1 146 38                        # axpy_snitch.c:146:38
	sub	a0, a0, a1
	.loc	1 146 18                        # axpy_snitch.c:146:18
	add	a0, a0, a3
	sw	a0, %lo(dma_time)(a2)
	.loc	1 147 9 is_stmt 1               # axpy_snitch.c:147:9
	#APP
	fence	rw, rw
	#NO_APP
.Ltmp108:
.LBB0_38:                               # %._crit_edge.thread
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 40 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp109:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp110:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB0_40
.Ltmp111:
# %bb.39:
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp112:
	#DEBUG_VALUE: tmp <- $x10
.LBB0_40:                               # %pulp_barrier.exit75
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
.Ltmp113:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp114:
	.loc	1 151 8                         # axpy_snitch.c:151:8
	beqz	t3, .LBB0_42
.Ltmp115:
# %bb.41:
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x15
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- $f10_f
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp116:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 152 39                        # axpy_snitch.c:152:39
	lui	a1, %hi(all_timer)
	lw	a1, %lo(all_timer)(a1)
	.loc	1 152 37 is_stmt 0              # axpy_snitch.c:152:37
	sub	a0, a0, a1
	.loc	1 152 18                        # axpy_snitch.c:152:18
	lui	a1, %hi(all_time)
	sw	a0, %lo(all_time)(a1)
	.loc	1 153 64 is_stmt 1              # axpy_snitch.c:153:64
	lw	a2, %lo(all_time)(a1)
	.loc	1 153 74 is_stmt 0              # axpy_snitch.c:153:74
	lui	a0, %hi(dma_time)
	lw	a3, %lo(dma_time)(a0)
	.loc	1 153 84                        # axpy_snitch.c:153:84
	lui	a0, %hi(issue_time)
	lw	a4, %lo(issue_time)(a0)
	.loc	1 153 96                        # axpy_snitch.c:153:96
	lui	a0, %hi(compute_time)
	lw	a5, %lo(compute_time)(a0)
.Ltmp117:
	.loc	1 153 9                         # axpy_snitch.c:153:9
	lui	a0, %hi(.L.str.1)
	addi	a0, a0, %lo(.L.str.1)
	li	a1, 0
	call	printf_
.Ltmp118:
.LBB0_42:
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp119:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp120:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB0_44
.Ltmp121:
# %bb.43:
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp122:
	#DEBUG_VALUE: tmp <- $x10
.LBB0_44:                               # %pulp_barrier.exit77
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
.Ltmp123:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp124:
.LBB0_45:
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x26
	.loc	1 157 1                         # axpy_snitch.c:157:1
	lw	ra, 60(sp)                      # 4-byte Folded Reload
	lw	s0, 56(sp)                      # 4-byte Folded Reload
	lw	s1, 52(sp)                      # 4-byte Folded Reload
	lw	s2, 48(sp)                      # 4-byte Folded Reload
	lw	s3, 44(sp)                      # 4-byte Folded Reload
	lw	s4, 40(sp)                      # 4-byte Folded Reload
	lw	s5, 36(sp)                      # 4-byte Folded Reload
	lw	s6, 32(sp)                      # 4-byte Folded Reload
	lw	s7, 28(sp)                      # 4-byte Folded Reload
	lw	s8, 24(sp)                      # 4-byte Folded Reload
	lw	s9, 20(sp)                      # 4-byte Folded Reload
	lw	s10, 16(sp)                     # 4-byte Folded Reload
.Ltmp125:
	lw	s11, 12(sp)                     # 4-byte Folded Reload
	addi	sp, sp, 64
	ret
.Ltmp126:
.Lfunc_end0:
	.size	dev_axpy, .Lfunc_end0-dev_axpy
	.cfi_endproc
	.file	5 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/llvm-minimal/lib/clang/15.0.0/include/stddef.h"
	.file	6 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools" "install/rv32imafd-ilp32d/riscv32-unknown-elf/include/stdio.h"
                                        # -- End function
	.weak	__wrapped_omp__omp_offloading_10303_7120698_axpy_l19 # -- Begin function __wrapped_omp__omp_offloading_10303_7120698_axpy_l19
	.p2align	2
	.type	__wrapped_omp__omp_offloading_10303_7120698_axpy_l19,@function
__wrapped_omp__omp_offloading_10303_7120698_axpy_l19: # @__wrapped_omp__omp_offloading_10303_7120698_axpy_l19
.Lfunc_begin1:
	.loc	1 19 0                          # axpy_snitch.c:19:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:x_phys <- $x10
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:y_phys <- $x11
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:n <- $x13
	addi	sp, sp, -64
	.cfi_def_cfa_offset 64
.Ltmp127:
	.loc	1 19 1 prologue_end             # axpy_snitch.c:19:1
	sw	ra, 60(sp)                      # 4-byte Folded Spill
	sw	s0, 56(sp)                      # 4-byte Folded Spill
	sw	s1, 52(sp)                      # 4-byte Folded Spill
	sw	s2, 48(sp)                      # 4-byte Folded Spill
	sw	s3, 44(sp)                      # 4-byte Folded Spill
	sw	s4, 40(sp)                      # 4-byte Folded Spill
	sw	s5, 36(sp)                      # 4-byte Folded Spill
	sw	s6, 32(sp)                      # 4-byte Folded Spill
	sw	s7, 28(sp)                      # 4-byte Folded Spill
	sw	s8, 24(sp)                      # 4-byte Folded Spill
	sw	s9, 20(sp)                      # 4-byte Folded Spill
	sw	s10, 16(sp)                     # 4-byte Folded Spill
	sw	s11, 12(sp)                     # 4-byte Folded Spill
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	.cfi_offset s2, -16
	.cfi_offset s3, -20
	.cfi_offset s4, -24
	.cfi_offset s5, -28
	.cfi_offset s6, -32
	.cfi_offset s7, -36
	.cfi_offset s8, -40
	.cfi_offset s9, -44
	.cfi_offset s10, -48
	.cfi_offset s11, -52
	lw	s10, 4(a0)
	lw	a6, 0(a0)
	lw	a7, 4(a1)
	lw	t1, 0(a1)
	lw	t0, 0(a3)
.Ltmp128:
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:alpha <- undef
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19_debug__:n <- undef
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19_debug__:alpha <- undef
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19_debug__:y_phys <- undef
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19_debug__:x_phys <- undef
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	.loc	4 28 2                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:28:2
	#APP
	csrr	a0, mhartid
	#NO_APP
.Ltmp129:
	#DEBUG_VALUE: pulp_get_core_id:r <- $x10
	.loc	1 60 10                         # axpy_snitch.c:60:10
	or	a1, a6, s10
.Ltmp130:
	snez	a1, a1
	or	a3, t1, a7
.Ltmp131:
	.loc	1 0 10 is_stmt 0                # axpy_snitch.c:0:10
	snez	a3, a3
	.loc	1 60 17                         # axpy_snitch.c:60:17
	and	a1, a1, a3
	snez	a3, t0
	and	a1, a1, a3
	andi	a3, t0, 511
	seqz	a3, a3
	and	a1, a1, a3
.Ltmp132:
	.loc	4 30 10 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:30:10
	addi	t2, a0, -16
.Ltmp133:
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	.loc	1 60 17                         # axpy_snitch.c:60:17
	beqz	a1, .LBB1_4
.Ltmp134:
# %bb.1:
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	1 0 17 is_stmt 0                # axpy_snitch.c:0:17
	flw	ft0, 0(a2)
.Ltmp135:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	.loc	1 66 9 is_stmt 1                # axpy_snitch.c:66:9
	beqz	t2, .LBB1_6
.Ltmp136:
# %bb.2:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	1 0 9 is_stmt 0                 # axpy_snitch.c:0:9
	li	a0, 8
	.loc	1 67 9 is_stmt 1                # axpy_snitch.c:67:9
	bne	t2, a0, .LBB1_8
.Ltmp137:
# %bb.3:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	1 68 18                         # axpy_snitch.c:68:18
	lui	a0, %hi(dma_time)
	sw	zero, %lo(dma_time)(a0)
	.loc	1 69 20                         # axpy_snitch.c:69:20
	lui	a0, %hi(issue_time)
	sw	zero, %lo(issue_time)(a0)
	lui	a0, %hi(all_time)
	addi	a0, a0, %lo(all_time)
	j	.LBB1_7
.Ltmp138:
.LBB1_4:
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	1 61 12                         # axpy_snitch.c:61:12
	bnez	t2, .LBB1_45
.Ltmp139:
# %bb.5:
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	1 62 13                         # axpy_snitch.c:62:13
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	li	a1, 0
	lw	ra, 60(sp)                      # 4-byte Folded Reload
	lw	s0, 56(sp)                      # 4-byte Folded Reload
	lw	s1, 52(sp)                      # 4-byte Folded Reload
	lw	s2, 48(sp)                      # 4-byte Folded Reload
	lw	s3, 44(sp)                      # 4-byte Folded Reload
	lw	s4, 40(sp)                      # 4-byte Folded Reload
	lw	s5, 36(sp)                      # 4-byte Folded Reload
	lw	s6, 32(sp)                      # 4-byte Folded Reload
	lw	s7, 28(sp)                      # 4-byte Folded Reload
	lw	s8, 24(sp)                      # 4-byte Folded Reload
	lw	s9, 20(sp)                      # 4-byte Folded Reload
	lw	s10, 16(sp)                     # 4-byte Folded Reload
.Ltmp140:
	lw	s11, 12(sp)                     # 4-byte Folded Reload
	addi	sp, sp, 64
.Ltmp141:
	tail	printf_
.Ltmp142:
.LBB1_6:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	1 0 13 is_stmt 0                # axpy_snitch.c:0:13
	lui	a0, %hi(compute_time)
	addi	a0, a0, %lo(compute_time)
.Ltmp143:
.LBB1_7:                                # %.sink.split.i.i
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	snez	t3, t2
	sw	zero, 0(a0)
	j	.LBB1_9
.Ltmp144:
.LBB1_8:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	li	t3, 0
.Ltmp145:
.LBB1_9:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp146:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp147:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB1_11
.Ltmp148:
# %bb.10:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp149:
	#DEBUG_VALUE: tmp <- $x10
.LBB1_11:                               # %pulp_barrier.exit.i.i
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
.Ltmp150:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp151:
	.loc	1 75 8                          # axpy_snitch.c:75:8
	beqz	t3, .LBB1_13
.Ltmp152:
# %bb.12:
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __omp_offloading_10303_7120698_axpy_l19:alpha <- $x12
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp153:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 75 33                         # axpy_snitch.c:75:33
	lui	a1, %hi(all_timer)
	sw	a0, %lo(all_timer)(a1)
.Ltmp154:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp155:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 78 21                         # axpy_snitch.c:78:21
	lui	t4, %hi(issue_timer)
	sw	a0, %lo(issue_timer)(t4)
.Ltmp156:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x26
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x16
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x16
	.loc	4 254 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	mv	a3, s10
	mv	a2, a6
.Ltmp157:
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	lui	a0, %hi(l1_buf_x)
	addi	a0, a0, %lo(l1_buf_x)
.Ltmp158:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	.loc	4 0 5 is_stmt 0                 # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:0:5
	lui	a1, 1
	addi	a4, a1, -2048
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp159:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x17
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x6
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- undef
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	lui	a0, %hi(l1_buf_y)
	addi	a0, a0, %lo(l1_buf_y)
.Ltmp160:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	.loc	4 254 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	mv	a3, a7
	mv	a2, t1
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp161:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp162:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 81 42                         # axpy_snitch.c:81:42
	lw	a1, %lo(issue_timer)(t4)
	.loc	1 81 20 is_stmt 0               # axpy_snitch.c:81:20
	lui	a2, %hi(issue_time)
	lw	a3, %lo(issue_time)(a2)
	.loc	1 81 40                         # axpy_snitch.c:81:40
	sub	a0, a0, a1
	.loc	1 81 20                         # axpy_snitch.c:81:20
	add	a0, a0, a3
	sw	a0, %lo(issue_time)(a2)
.Ltmp163:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- undef
.LBB1_13:                               # %.lr.ph.i.i
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:itr <- 0
	.loc	1 0 20                          # axpy_snitch.c:0:20
	li	a1, 512
	mv	a0, t0
.Ltmp164:
	#DEBUG_VALUE: i <- 0
	.loc	1 85 5 is_stmt 1                # axpy_snitch.c:85:5
	bltu	a1, t0, .LBB1_15
.Ltmp165:
# %bb.14:                               # %.lr.ph.i.i
	#DEBUG_VALUE: i <- 0
	#DEBUG_VALUE: dev_axpy:itr <- 0
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 0 5 is_stmt 0                 # axpy_snitch.c:0:5
	li	a0, 512
.Ltmp166:
.LBB1_15:                               # %.lr.ph.i.i
	#DEBUG_VALUE: i <- 0
	#DEBUG_VALUE: dev_axpy:itr <- 0
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:issue_diff <- 0
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:n <- $x5
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	li	ra, 0
	li	t4, 0
	addi	t5, t0, -512
	.loc	1 85 5                          # axpy_snitch.c:85:5
	addi	a0, a0, -1
	srli	t6, a0, 9
	lui	a0, 1
	addi	a4, a0, -2048
	lui	s0, %hi(dma_timer)
	lui	s1, %hi(dma_time)
	lui	a0, %hi(barrier_reg)
	addi	s2, a0, %lo(barrier_reg)
	lui	s3, %hi(issue_timer)
	lui	a0, %hi(l1_buf_x)
	addi	s4, a0, %lo(l1_buf_x)
	lui	a0, %hi(l1_buf_y)
	addi	s5, a0, %lo(l1_buf_y)
	lui	s6, %hi(compute_timer)
	lui	s7, %hi(compute_time)
	lui	s8, %hi(issue_time)
	slli	s9, t2, 8
	mv	s11, a4
	j	.LBB1_18
.Ltmp167:
.LBB1_16:                               # %.thread29.i.i
                                        #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_plus_uconst 512, DW_OP_stack_value] $x27
	beq	t6, t4, .LBB1_38
.Ltmp168:
.LBB1_17:                               # %.backedge.i.i
                                        #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_plus_uconst 512, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 138 12 is_stmt 1              # axpy_snitch.c:138:12
	addi	t4, t4, 1
.Ltmp169:
	.loc	1 0 0 is_stmt 0                 # axpy_snitch.c:0:0
	addi	a0, s11, 1024
	addi	s11, a0, 1024
.Ltmp170:
	addi	t5, t5, -512
.Ltmp171:
.LBB1_18:                               # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: rest <- [DW_OP_LLVM_arg 0, DW_OP_constu 512, DW_OP_minus, DW_OP_LLVM_arg 0, DW_OP_minus, DW_OP_stack_value] undef
	.loc	1 88 12 is_stmt 1               # axpy_snitch.c:88:12
	beqz	t3, .LBB1_24
.Ltmp172:
# %bb.19:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: rest <- [DW_OP_LLVM_arg 0, DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 18446744073709551104, DW_OP_mul, DW_OP_consts 18446744073709551104, DW_OP_LLVM_arg 0, DW_OP_plus, DW_OP_plus, DW_OP_stack_value] undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp173:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 89 23                         # axpy_snitch.c:89:23
	sw	a0, %lo(dma_timer)(s0)
.Ltmp174:
	.loc	4 448 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:448:5
	#APP
.Ltmp175:
	.word	136315563
	bnez	t0, .Ltmp175

	#NO_APP
.Ltmp176:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp177:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 91 44                         # axpy_snitch.c:91:44
	lw	a1, %lo(dma_timer)(s0)
	.loc	1 91 22 is_stmt 0               # axpy_snitch.c:91:22
	lw	a2, %lo(dma_time)(s1)
	.loc	1 91 42                         # axpy_snitch.c:91:42
	sub	a0, a0, a1
	.loc	1 91 22                         # axpy_snitch.c:91:22
	add	a0, a0, a2
	sw	a0, %lo(dma_time)(s1)
.Ltmp178:
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp179:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp180:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB1_21
.Ltmp181:
# %bb.20:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp182:
	#DEBUG_VALUE: tmp <- $x10
.LBB1_21:                               # %pulp_barrier.exit7.i.i
                                        #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
.Ltmp183:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp184:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp185:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 95 25                         # axpy_snitch.c:95:25
	sw	a0, %lo(issue_timer)(s3)
	.loc	1 96 16                         # axpy_snitch.c:96:16
	blez	t5, .LBB1_23
.Ltmp186:
# %bb.22:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 97 83                         # axpy_snitch.c:97:83
	not	a0, t4
	andi	a0, a0, 1
	.loc	1 97 67 is_stmt 0               # axpy_snitch.c:97:67
	slli	t0, a0, 11
	.loc	1 97 95                         # axpy_snitch.c:97:95
	add	a2, a6, s11
	sltu	a0, a2, a6
	add	a3, s10, a0
.Ltmp187:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_convert 32 7, DW_OP_LLVM_convert 64 7, DW_OP_stack_value] undef
	.loc	1 97 67                         # axpy_snitch.c:97:67
	add	a0, t0, s4
.Ltmp188:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x12
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x12
	.loc	4 254 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp189:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	1 98 95                         # axpy_snitch.c:98:95
	add	a2, t1, s11
.Ltmp190:
	sltu	a0, a2, t1
	add	a3, a7, a0
.Ltmp191:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_convert 32 7, DW_OP_LLVM_convert 64 7, DW_OP_stack_value] undef
	.loc	1 98 67 is_stmt 0               # axpy_snitch.c:98:67
	add	a0, t0, s5
.Ltmp192:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x12
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_fragment 0 32] $x12
	.loc	4 254 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp193:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
.LBB1_23:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp194:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 100 45                        # axpy_snitch.c:100:45
	lw	a1, %lo(issue_timer)(s3)
	.loc	1 100 43 is_stmt 0              # axpy_snitch.c:100:43
	sub	ra, a0, a1
.Ltmp195:
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	.loc	1 0 43                          # axpy_snitch.c:0:43
	j	.LBB1_29
.Ltmp196:
.LBB1_24:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp197:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp198:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB1_26
.Ltmp199:
# %bb.25:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp200:
	#DEBUG_VALUE: tmp <- $x10
.LBB1_26:                               # %pulp_barrier.exit9.i.i
                                        #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
.Ltmp201:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp202:
	.loc	1 105 16                        # axpy_snitch.c:105:16
	bnez	t2, .LBB1_28
.Ltmp203:
# %bb.27:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp204:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 105 45                        # axpy_snitch.c:105:45
	sw	a0, %lo(compute_timer)(s6)
.Ltmp205:
.LBB1_28:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 110 71                        # axpy_snitch.c:110:71
	andi	a0, t4, 1
	.loc	1 110 58 is_stmt 0              # axpy_snitch.c:110:58
	slli	a0, a0, 11
	add	a0, a0, s9
	add	a1, a0, s4
.Ltmp206:
	#DEBUG_VALUE: px <- $x11
	.loc	1 111 58 is_stmt 1              # axpy_snitch.c:111:58
	add	a0, a0, s5
.Ltmp207:
	#DEBUG_VALUE: py <- $x10
	.loc	1 112 13                        # axpy_snitch.c:112:13
	li	a2, 64
	#APP
	li	t0, 0
.Ltmp208:
	flw	ft1, 0(a1)
	flw	ft2, 0(a0)
	fmadd.s	ft2, ft0, ft1, ft2
	fsw	ft2, 0(a0)
	addi	a1, a1, 4
	addi	a0, a0, 4
	addi	t0, t0, 1
	blt	t0, a2, .Ltmp208

	#NO_APP
.Ltmp209:
	#DEBUG_VALUE: py <- undef
	#DEBUG_VALUE: px <- undef
.LBB1_29:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	.loc	4 40 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp210:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp211:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB1_31
.Ltmp212:
# %bb.30:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp213:
	#DEBUG_VALUE: tmp <- $x10
.LBB1_31:                               # %pulp_barrier.exit11.i.i
                                        #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
.Ltmp214:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp215:
	.loc	1 129 12                        # axpy_snitch.c:129:12
	beqz	t2, .LBB1_33
.Ltmp216:
# %bb.32:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 131 12                        # axpy_snitch.c:131:12
	beqz	t3, .LBB1_16
	j	.LBB1_34
.Ltmp217:
.LBB1_33:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp218:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 129 62                        # axpy_snitch.c:129:62
	lw	a1, %lo(compute_timer)(s6)
	.loc	1 129 40 is_stmt 0              # axpy_snitch.c:129:40
	lw	a2, %lo(compute_time)(s7)
	.loc	1 129 60                        # axpy_snitch.c:129:60
	sub	a0, a0, a1
	.loc	1 129 40                        # axpy_snitch.c:129:40
	add	a0, a0, a2
	sw	a0, %lo(compute_time)(s7)
.Ltmp219:
	.loc	1 131 12 is_stmt 1              # axpy_snitch.c:131:12
	beqz	t3, .LBB1_16
.Ltmp220:
.LBB1_34:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	t0, mcycle
	#NO_APP
.Ltmp221:
	#DEBUG_VALUE: __tmp <- $x5
	.loc	1 132 57                        # axpy_snitch.c:132:57
	lw	a5, %lo(compute_timer)(s6)
.Ltmp222:
	#DEBUG_VALUE: compute_diff <- undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp223:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 134 25                        # axpy_snitch.c:134:25
	sw	a0, %lo(issue_timer)(s3)
	.loc	1 135 50                        # axpy_snitch.c:135:50
	add	a0, s11, t1
	.loc	1 135 48 is_stmt 0              # axpy_snitch.c:135:48
	addi	a0, a0, -2048
	sltu	a1, a0, t1
	.loc	1 135 102                       # axpy_snitch.c:135:102
	andi	a2, t4, 1
	.loc	1 135 90                        # axpy_snitch.c:135:90
	slli	a2, a2, 11
	add	a2, a2, s5
	.loc	1 135 48                        # axpy_snitch.c:135:48
	add	a1, a7, a1
.Ltmp224:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_high <- $x11
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_dst_low <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:src <- [DW_OP_LLVM_convert 32 7, DW_OP_LLVM_convert 64 7, DW_OP_stack_value] undef
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:dst <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_low <- $x12
	.loc	4 254 5 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:254:5
	li	a3, 0
	#APP
	.word	14024747

	#NO_APP
	.loc	4 264 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:264:5
	#APP
	.word	45416491

	#NO_APP
	.loc	4 275 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:275:5
	#APP
	.word	67568939

	#NO_APP
.Ltmp225:
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_txid <- undef
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp226:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 136 46                        # axpy_snitch.c:136:46
	lw	a1, %lo(issue_timer)(s3)
.Ltmp227:
	.loc	1 136 24 is_stmt 0              # axpy_snitch.c:136:24
	lw	a2, %lo(issue_time)(s8)
.Ltmp228:
	.loc	1 136 88                        # axpy_snitch.c:136:88
	sub	a3, a5, t0
	add	a3, ra, a3
	.loc	1 136 44                        # axpy_snitch.c:136:44
	bltu	ra, a3, .LBB1_36
.Ltmp229:
# %bb.35:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: __tmp <- $x10
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_src_high <- 0
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:reg_size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:size <- 2048
	#DEBUG_VALUE: __dma_start_1d_wideptr_base:__deserialize <- 0
	#DEBUG_VALUE: __tmp <- $x5
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 0 44                          # axpy_snitch.c:0:44
	add	a0, a0, a3
.Ltmp230:
.LBB1_36:                               #   in Loop: Header=BB1_18 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	1 136 58                        # axpy_snitch.c:136:58
	sub	a0, a0, a1
	.loc	1 136 24                        # axpy_snitch.c:136:24
	add	a0, a0, a2
	sw	a0, %lo(issue_time)(s8)
.Ltmp231:
	#DEBUG_VALUE: i <- [DW_OP_consts 2048, DW_OP_minus, DW_OP_consts 2048, DW_OP_div, DW_OP_consts 512, DW_OP_mul, DW_OP_plus_uconst 512, DW_OP_stack_value] $x27
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	.loc	1 85 5 is_stmt 1                # axpy_snitch.c:85:5
	bne	t6, t4, .LBB1_17
.Ltmp232:
# %bb.37:                               # %._crit_edge.i.i
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp233:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 144 19                        # axpy_snitch.c:144:19
	lui	a1, %hi(dma_timer)
	sw	a0, %lo(dma_timer)(a1)
.Ltmp234:
	.loc	4 448 5                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:448:5
	#APP
.Ltmp235:
	.word	136315563
	bnez	t0, .Ltmp235

	#NO_APP
.Ltmp236:
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp237:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 146 40                        # axpy_snitch.c:146:40
	lw	a1, %lo(dma_timer)(a1)
	.loc	1 146 18 is_stmt 0              # axpy_snitch.c:146:18
	lui	a2, %hi(dma_time)
	lw	a3, %lo(dma_time)(a2)
	.loc	1 146 38                        # axpy_snitch.c:146:38
	sub	a0, a0, a1
	.loc	1 146 18                        # axpy_snitch.c:146:18
	add	a0, a0, a3
	sw	a0, %lo(dma_time)(a2)
	.loc	1 147 9 is_stmt 1               # axpy_snitch.c:147:9
	#APP
	fence	rw, rw
	#NO_APP
.Ltmp238:
.LBB1_38:                               # %._crit_edge.thread.i.i
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 40 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp239:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp240:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB1_40
.Ltmp241:
# %bb.39:
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp242:
	#DEBUG_VALUE: tmp <- $x10
.LBB1_40:                               # %pulp_barrier.exit13.i.i
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
.Ltmp243:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp244:
	.loc	1 151 8                         # axpy_snitch.c:151:8
	beqz	t3, .LBB1_42
.Ltmp245:
# %bb.41:
	#DEBUG_VALUE: dev_axpy:itr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x29
	#DEBUG_VALUE: dev_axpy:issue_diff <- $x1
	#DEBUG_VALUE: dev_axpy:alpha <- $f0_f
	#DEBUG_VALUE: dev_axpy:core_idx <- $x7
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 0 32] $x16
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 0 32] $x6
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	#DEBUG_VALUE: dev_axpy:y_phys <- [DW_OP_LLVM_fragment 32 32] $x17
	.loc	4 35 33                         # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:35:33
	#APP
	csrr	a0, mcycle
	#NO_APP
.Ltmp246:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	1 152 39                        # axpy_snitch.c:152:39
	lui	a1, %hi(all_timer)
	lw	a1, %lo(all_timer)(a1)
	.loc	1 152 37 is_stmt 0              # axpy_snitch.c:152:37
	sub	a0, a0, a1
	.loc	1 152 18                        # axpy_snitch.c:152:18
	lui	a1, %hi(all_time)
	sw	a0, %lo(all_time)(a1)
	.loc	1 153 64 is_stmt 1              # axpy_snitch.c:153:64
	lw	a2, %lo(all_time)(a1)
	.loc	1 153 74 is_stmt 0              # axpy_snitch.c:153:74
	lui	a0, %hi(dma_time)
	lw	a3, %lo(dma_time)(a0)
	.loc	1 153 84                        # axpy_snitch.c:153:84
	lui	a0, %hi(issue_time)
	lw	a4, %lo(issue_time)(a0)
	.loc	1 153 96                        # axpy_snitch.c:153:96
	lui	a0, %hi(compute_time)
	lw	a5, %lo(compute_time)(a0)
	.loc	1 153 9                         # axpy_snitch.c:153:9
	lui	a0, %hi(.L.str.1)
	addi	a0, a0, %lo(.L.str.1)
	li	a1, 0
	call	printf_
.Ltmp247:
.LBB1_42:
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	.loc	4 40 33 is_stmt 1               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:33
	#APP
	csrr	a0, misa
	#NO_APP
.Ltmp248:
	#DEBUG_VALUE: __tmp <- $x10
	.loc	4 40 92 is_stmt 0               # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:92
	andi	a0, a0, 32
.Ltmp249:
	.loc	4 40 9                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:40:9
	beqz	a0, .LBB1_44
.Ltmp250:
# %bb.43:
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	.loc	4 42 9 is_stmt 1                # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:42:9
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.Ltmp251:
	#DEBUG_VALUE: tmp <- $x10
.LBB1_44:                               # %pulp_barrier.exit15.i.i
	#DEBUG_VALUE: dev_axpy:x_phys <- [DW_OP_LLVM_fragment 32 32] $x26
	.loc	4 73 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:73:5
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
.Ltmp252:
	#DEBUG_VALUE: pulp_barrier:tmp <- $x10
	.loc	4 79 5                          # platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h:79:5
	#APP
	csrr	zero, 1986
	#NO_APP
.Ltmp253:
.LBB1_45:                               # %__omp_offloading_10303_7120698_axpy_l19_debug__.exit
	.loc	1 19 1                          # axpy_snitch.c:19:1
	lw	ra, 60(sp)                      # 4-byte Folded Reload
	lw	s0, 56(sp)                      # 4-byte Folded Reload
	lw	s1, 52(sp)                      # 4-byte Folded Reload
	lw	s2, 48(sp)                      # 4-byte Folded Reload
	lw	s3, 44(sp)                      # 4-byte Folded Reload
	lw	s4, 40(sp)                      # 4-byte Folded Reload
	lw	s5, 36(sp)                      # 4-byte Folded Reload
	lw	s6, 32(sp)                      # 4-byte Folded Reload
	lw	s7, 28(sp)                      # 4-byte Folded Reload
	lw	s8, 24(sp)                      # 4-byte Folded Reload
	lw	s9, 20(sp)                      # 4-byte Folded Reload
	lw	s10, 16(sp)                     # 4-byte Folded Reload
	lw	s11, 12(sp)                     # 4-byte Folded Reload
	addi	sp, sp, 64
	ret
.Ltmp254:
.Lfunc_end1:
	.size	__wrapped_omp__omp_offloading_10303_7120698_axpy_l19, .Lfunc_end1-__wrapped_omp__omp_offloading_10303_7120698_axpy_l19
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint32_noblock
	.type	hero_load_uint32_noblock,@function
hero_load_uint32_noblock:               # @hero_load_uint32_noblock
.Lfunc_begin2:
	.file	7 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp" "common/hero_64.h"
	.loc	7 164 0                         # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint32_noblock:val <- $x12
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:lower <- $x10
	.loc	7 164 327 prologue_end          # common/hero_64.h:164:327
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	lw	a3, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp255:
	#DEBUG_VALUE: hero_load_uint32_noblock:mstatus <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:reg <- $x13
	.loc	7 164 597 is_stmt 0             # common/hero_64.h:164:597
	sw	a3, 0(a2)
	.loc	7 164 604                       # common/hero_64.h:164:604
	li	a0, 0
.Ltmp256:
	ret
.Ltmp257:
.Lfunc_end2:
	.size	hero_load_uint32_noblock, .Lfunc_end2-hero_load_uint32_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint32
	.type	hero_load_uint32,@function
hero_load_uint32:                       # @hero_load_uint32
.Lfunc_begin3:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint32:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:val <- undef
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:lower <- $x10
	.loc	7 164 327 prologue_end          # common/hero_64.h:164:327
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	lw	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp258:
	#DEBUG_VALUE: hero_load_uint32:val <- undef
	#DEBUG_VALUE: hero_load_uint32:res <- 0
	#DEBUG_VALUE: hero_load_uint32_noblock:mstatus <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:reg <- $x12
	.loc	7 164 791 is_stmt 0             # common/hero_64.h:164:791
	mv	a0, a2
.Ltmp259:
	ret
.Ltmp260:
.Lfunc_end3:
	.size	hero_load_uint32, .Lfunc_end3-hero_load_uint32
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint32_noblock
	.type	hero_store_uint32_noblock,@function
hero_store_uint32_noblock:              # @hero_store_uint32_noblock
.Lfunc_begin4:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint32_noblock:val <- $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:lower <- $x10
	.loc	7 164 1085 prologue_end         # common/hero_64.h:164:1085
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	sw	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp261:
	#DEBUG_VALUE: hero_store_uint32_noblock:mstatus <- $x13
	.loc	7 164 1348 is_stmt 0            # common/hero_64.h:164:1348
	li	a0, 0
.Ltmp262:
	ret
.Ltmp263:
.Lfunc_end4:
	.size	hero_store_uint32_noblock, .Lfunc_end4-hero_store_uint32_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint32
	.type	hero_store_uint32,@function
hero_store_uint32:                      # @hero_store_uint32
.Lfunc_begin5:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint32:val <- $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:val <- $x12
	#DEBUG_VALUE: hero_store_uint32:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:lower <- $x10
	.loc	7 164 1085 prologue_end         # common/hero_64.h:164:1085
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	sw	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp264:
	#DEBUG_VALUE: hero_store_uint32:res <- 0
	#DEBUG_VALUE: hero_store_uint32_noblock:mstatus <- $x13
	.loc	7 164 1494 is_stmt 0            # common/hero_64.h:164:1494
	ret
.Ltmp265:
.Lfunc_end5:
	.size	hero_store_uint32, .Lfunc_end5-hero_store_uint32
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint16_noblock
	.type	hero_load_uint16_noblock,@function
hero_load_uint16_noblock:               # @hero_load_uint16_noblock
.Lfunc_begin6:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint16_noblock:val <- $x12
	#DEBUG_VALUE: hero_load_uint16_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint16_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint16_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint16_noblock:lower <- $x10
	.loc	7 164 1822 prologue_end         # common/hero_64.h:164:1822
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	lhu	a3, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp266:
	#DEBUG_VALUE: hero_load_uint16_noblock:reg <- undef
	#DEBUG_VALUE: hero_load_uint16_noblock:mstatus <- undef
	.loc	7 164 2096 is_stmt 0            # common/hero_64.h:164:2096
	sh	a3, 0(a2)
	.loc	7 164 2103                      # common/hero_64.h:164:2103
	li	a0, 0
.Ltmp267:
	ret
.Ltmp268:
.Lfunc_end6:
	.size	hero_load_uint16_noblock, .Lfunc_end6-hero_load_uint16_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint16
	.type	hero_load_uint16,@function
hero_load_uint16:                       # @hero_load_uint16
.Lfunc_begin7:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint16:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint16_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint16:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint16_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint16_noblock:val <- undef
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint16_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint16_noblock:lower <- $x10
	.loc	7 164 1822 prologue_end         # common/hero_64.h:164:1822
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	lhu	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp269:
	#DEBUG_VALUE: hero_load_uint16:val <- undef
	#DEBUG_VALUE: hero_load_uint16:res <- 0
	#DEBUG_VALUE: hero_load_uint16_noblock:mstatus <- undef
	.loc	7 164 2290 is_stmt 0            # common/hero_64.h:164:2290
	slli	a0, a2, 16
.Ltmp270:
	srli	a0, a0, 16
	#DEBUG_VALUE: hero_load_uint16_noblock:reg <- $x10
	ret
.Ltmp271:
.Lfunc_end7:
	.size	hero_load_uint16, .Lfunc_end7-hero_load_uint16
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint16_noblock
	.type	hero_store_uint16_noblock,@function
hero_store_uint16_noblock:              # @hero_store_uint16_noblock
.Lfunc_begin8:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint16_noblock:val <- $x12
	#DEBUG_VALUE: hero_store_uint16_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint16_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint16_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint16_noblock:lower <- $x10
	.loc	7 164 2584 prologue_end         # common/hero_64.h:164:2584
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	sh	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp272:
	#DEBUG_VALUE: hero_store_uint16_noblock:mstatus <- $x13
	.loc	7 164 2847 is_stmt 0            # common/hero_64.h:164:2847
	li	a0, 0
.Ltmp273:
	ret
.Ltmp274:
.Lfunc_end8:
	.size	hero_store_uint16_noblock, .Lfunc_end8-hero_store_uint16_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint16
	.type	hero_store_uint16,@function
hero_store_uint16:                      # @hero_store_uint16
.Lfunc_begin9:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint16:val <- $x12
	#DEBUG_VALUE: hero_store_uint16_noblock:val <- $x12
	#DEBUG_VALUE: hero_store_uint16:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint16_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint16:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint16_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint16_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint16_noblock:lower <- $x10
	.loc	7 164 2584 prologue_end         # common/hero_64.h:164:2584
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	sh	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp275:
	#DEBUG_VALUE: hero_store_uint16:res <- 0
	#DEBUG_VALUE: hero_store_uint16_noblock:mstatus <- $x13
	.loc	7 164 2993 is_stmt 0            # common/hero_64.h:164:2993
	ret
.Ltmp276:
.Lfunc_end9:
	.size	hero_store_uint16, .Lfunc_end9-hero_store_uint16
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint8_noblock
	.type	hero_load_uint8_noblock,@function
hero_load_uint8_noblock:                # @hero_load_uint8_noblock
.Lfunc_begin10:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint8_noblock:val <- $x12
	#DEBUG_VALUE: hero_load_uint8_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint8_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint8_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint8_noblock:lower <- $x10
	.loc	7 164 3316 prologue_end         # common/hero_64.h:164:3316
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	lbu	a3, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp277:
	#DEBUG_VALUE: hero_load_uint8_noblock:reg <- undef
	#DEBUG_VALUE: hero_load_uint8_noblock:mstatus <- undef
	.loc	7 164 3590 is_stmt 0            # common/hero_64.h:164:3590
	sb	a3, 0(a2)
	.loc	7 164 3597                      # common/hero_64.h:164:3597
	li	a0, 0
.Ltmp278:
	ret
.Ltmp279:
.Lfunc_end10:
	.size	hero_load_uint8_noblock, .Lfunc_end10-hero_load_uint8_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint8
	.type	hero_load_uint8,@function
hero_load_uint8:                        # @hero_load_uint8
.Lfunc_begin11:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint8:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint8_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint8:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint8_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint8_noblock:val <- undef
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint8_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint8_noblock:lower <- $x10
	.loc	7 164 3316 prologue_end         # common/hero_64.h:164:3316
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	lbu	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp280:
	#DEBUG_VALUE: hero_load_uint8:val <- undef
	#DEBUG_VALUE: hero_load_uint8:res <- 0
	#DEBUG_VALUE: hero_load_uint8_noblock:mstatus <- undef
	.loc	7 164 3779 is_stmt 0            # common/hero_64.h:164:3779
	andi	a0, a2, 255
.Ltmp281:
	#DEBUG_VALUE: hero_load_uint8_noblock:reg <- $x10
	ret
.Ltmp282:
.Lfunc_end11:
	.size	hero_load_uint8, .Lfunc_end11-hero_load_uint8
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint8_noblock
	.type	hero_store_uint8_noblock,@function
hero_store_uint8_noblock:               # @hero_store_uint8_noblock
.Lfunc_begin12:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint8_noblock:val <- $x12
	#DEBUG_VALUE: hero_store_uint8_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint8_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint8_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint8_noblock:lower <- $x10
	.loc	7 164 4069 prologue_end         # common/hero_64.h:164:4069
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	sb	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp283:
	#DEBUG_VALUE: hero_store_uint8_noblock:mstatus <- $x13
	.loc	7 164 4332 is_stmt 0            # common/hero_64.h:164:4332
	li	a0, 0
.Ltmp284:
	ret
.Ltmp285:
.Lfunc_end12:
	.size	hero_store_uint8_noblock, .Lfunc_end12-hero_store_uint8_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint8
	.type	hero_store_uint8,@function
hero_store_uint8:                       # @hero_store_uint8
.Lfunc_begin13:
	.loc	7 164 0 is_stmt 1               # common/hero_64.h:164:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint8:val <- $x12
	#DEBUG_VALUE: hero_store_uint8_noblock:val <- $x12
	#DEBUG_VALUE: hero_store_uint8:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint8_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint8:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint8_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint8_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint8_noblock:lower <- $x10
	.loc	7 164 4069 prologue_end         # common/hero_64.h:164:4069
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	sb	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp286:
	#DEBUG_VALUE: hero_store_uint8:res <- 0
	#DEBUG_VALUE: hero_store_uint8_noblock:mstatus <- $x13
	.loc	7 164 4475 is_stmt 0            # common/hero_64.h:164:4475
	ret
.Ltmp287:
.Lfunc_end13:
	.size	hero_store_uint8, .Lfunc_end13-hero_store_uint8
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint64
	.type	hero_load_uint64,@function
hero_load_uint64:                       # @hero_load_uint64
.Lfunc_begin14:
	.loc	7 167 0 is_stmt 1               # common/hero_64.h:167:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint64:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint64:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:val <- undef
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:lower <- $x10
	.loc	7 164 327 prologue_end          # common/hero_64.h:164:327
	#APP
	csrrci	a3, mstatus, 3
	csrw	3008, zero
	lw	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a3
	#NO_APP
.Ltmp288:
	#DEBUG_VALUE: hero_load_uint32:val <- undef
	#DEBUG_VALUE: hero_load_uint32:res <- 0
	#DEBUG_VALUE: hero_load_uint32_noblock:mstatus <- undef
	#DEBUG_VALUE: hero_load_uint64:ret_val <- [DW_OP_LLVM_fragment 0 32] $x12
	#DEBUG_VALUE: hero_load_uint32:val <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:mstatus <- undef
	#DEBUG_VALUE: hero_load_uint64:lower <- $x12
	#DEBUG_VALUE: hero_load_uint32_noblock:reg <- $x12
	.loc	7 169 50                        # common/hero_64.h:169:50
	addi	a3, a0, 4
.Ltmp289:
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x13
	sltu	a0, a3, a0
.Ltmp290:
	add	a0, a1, a0
.Ltmp291:
	#DEBUG_VALUE: hero_load_uint32_noblock:lower <- $x13
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x13
	#DEBUG_VALUE: hero_load_uint32_noblock:upper <- $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:val <- undef
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32:addr <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x13
	#DEBUG_VALUE: hero_load_uint32:addr <- [DW_OP_LLVM_fragment 0 32] $x13
	#DEBUG_VALUE: hero_load_uint32:res <- 0
	.loc	7 164 327                       # common/hero_64.h:164:327
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	lw	a1, 0(a3)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp292:
	#DEBUG_VALUE: hero_load_uint64:ret_val <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint64:upper <- $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:reg <- $x11
	.loc	7 172 5                         # common/hero_64.h:172:5
	mv	a0, a2
.Ltmp293:
	ret
.Ltmp294:
.Lfunc_end14:
	.size	hero_load_uint64, .Lfunc_end14-hero_load_uint64
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint64
	.type	hero_store_uint64,@function
hero_store_uint64:                      # @hero_store_uint64
.Lfunc_begin15:
	.loc	7 176 0                         # common/hero_64.h:176:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint64:val <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: hero_store_uint64:val <- [DW_OP_LLVM_fragment 0 32] $x12
	#DEBUG_VALUE: hero_store_uint64:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint64:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint32:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint32:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32:val <- $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:val <- $x12
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint64:lower <- $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:lower <- $x10
	.loc	7 164 1085 prologue_end         # common/hero_64.h:164:1085
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	sw	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp295:
	#DEBUG_VALUE: hero_store_uint64:upper <- $x13
	#DEBUG_VALUE: hero_store_uint32:res <- 0
	#DEBUG_VALUE: hero_store_uint32_noblock:mstatus <- $x14
	.loc	7 180 28                        # common/hero_64.h:180:28
	addi	a2, a0, 4
.Ltmp296:
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x12
	sltu	a0, a2, a0
.Ltmp297:
	add	a0, a1, a0
.Ltmp298:
	#DEBUG_VALUE: hero_store_uint32_noblock:lower <- $x12
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:upper <- $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:val <- $x13
	#DEBUG_VALUE: hero_store_uint32:val <- $x13
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32:addr <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x12
	#DEBUG_VALUE: hero_store_uint32:addr <- [DW_OP_LLVM_fragment 0 32] $x12
	.loc	7 164 1085                      # common/hero_64.h:164:1085
	#APP
	csrrci	a1, mstatus, 3
	csrw	3008, zero
	sw	a3, 0(a2)
	csrw	3008, zero
	csrw	mstatus, a1
	#NO_APP
.Ltmp299:
	#DEBUG_VALUE: hero_store_uint32:res <- 0
	#DEBUG_VALUE: hero_store_uint32_noblock:mstatus <- $x11
	.loc	7 181 1                         # common/hero_64.h:181:1
	ret
.Ltmp300:
.Lfunc_end15:
	.size	hero_store_uint64, .Lfunc_end15-hero_store_uint64
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_load_uint64_noblock
	.type	hero_load_uint64_noblock,@function
hero_load_uint64_noblock:               # @hero_load_uint64_noblock
.Lfunc_begin16:
	.loc	7 184 0                         # common/hero_64.h:184:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_load_uint64_noblock:val <- $x12
	#DEBUG_VALUE: hero_load_uint64_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint64_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:val <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_load_uint64_noblock:lower <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_load_uint32_noblock:lower <- $x10
	.loc	7 164 327 prologue_end          # common/hero_64.h:164:327
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	lw	a3, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp301:
	#DEBUG_VALUE: hero_load_uint32_noblock:mstatus <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:reg <- $x13
	.loc	7 164 597 is_stmt 0             # common/hero_64.h:164:597
	sw	a3, 0(a2)
.Ltmp302:
	#DEBUG_VALUE: hero_load_uint64_noblock:upper <- [DW_OP_plus_uconst 4, DW_OP_stack_value] $x12
	#DEBUG_VALUE: hero_load_uint64_noblock:res_lower <- 0
	.loc	7 188 57 is_stmt 1              # common/hero_64.h:188:57
	addi	a3, a0, 4
.Ltmp303:
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x13
	sltu	a0, a3, a0
.Ltmp304:
	add	a0, a1, a0
.Ltmp305:
	#DEBUG_VALUE: hero_load_uint32_noblock:lower <- $x13
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x13
	#DEBUG_VALUE: hero_load_uint32_noblock:upper <- $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:val <- [DW_OP_plus_uconst 4, DW_OP_stack_value] $x12
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_load_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x13
	.loc	7 164 327                       # common/hero_64.h:164:327
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	lw	a1, 0(a3)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp306:
	#DEBUG_VALUE: hero_load_uint32_noblock:mstatus <- undef
	#DEBUG_VALUE: hero_load_uint32_noblock:reg <- $x11
	.loc	7 164 597 is_stmt 0             # common/hero_64.h:164:597
	sw	a1, 4(a2)
.Ltmp307:
	#DEBUG_VALUE: hero_load_uint64_noblock:res_upper <- 0
	.loc	7 189 5 is_stmt 1               # common/hero_64.h:189:5
	li	a0, 0
.Ltmp308:
	ret
.Ltmp309:
.Lfunc_end16:
	.size	hero_load_uint64_noblock, .Lfunc_end16-hero_load_uint64_noblock
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function hero_store_uint64_noblock
	.type	hero_store_uint64_noblock,@function
hero_store_uint64_noblock:              # @hero_store_uint64_noblock
.Lfunc_begin17:
	.loc	7 193 0                         # common/hero_64.h:193:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: hero_store_uint64_noblock:val <- [DW_OP_LLVM_fragment 32 32] $x13
	#DEBUG_VALUE: hero_store_uint64_noblock:val <- [DW_OP_LLVM_fragment 0 32] $x12
	#DEBUG_VALUE: hero_store_uint64_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint64_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:val <- $x12
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x11
	#DEBUG_VALUE: hero_store_uint64_noblock:lower <- $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:upper <- $x11
	#DEBUG_VALUE: hero_store_uint32_noblock:lower <- $x10
	.loc	7 164 1085 prologue_end         # common/hero_64.h:164:1085
	#APP
	csrrci	a4, mstatus, 3
	csrw	3008, zero
	sw	a2, 0(a0)
	csrw	3008, zero
	csrw	mstatus, a4
	#NO_APP
.Ltmp310:
	#DEBUG_VALUE: hero_store_uint64_noblock:upper <- $x13
	#DEBUG_VALUE: hero_store_uint64_noblock:res_lower <- 0
	#DEBUG_VALUE: hero_store_uint32_noblock:mstatus <- $x14
	.loc	7 197 58                        # common/hero_64.h:197:58
	addi	a2, a0, 4
.Ltmp311:
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 0 32] $x12
	sltu	a0, a2, a0
.Ltmp312:
	add	a0, a1, a0
.Ltmp313:
	#DEBUG_VALUE: hero_store_uint32_noblock:lower <- $x12
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 0 32] $x12
	#DEBUG_VALUE: hero_store_uint32_noblock:upper <- $x10
	#DEBUG_VALUE: __lower32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:val <- $x13
	#DEBUG_VALUE: __upper32:dw <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 32 32] $x10
	#DEBUG_VALUE: hero_store_uint32_noblock:addr <- [DW_OP_LLVM_fragment 0 32] $x12
	.loc	7 164 1085                      # common/hero_64.h:164:1085
	#APP
	csrrci	a1, mstatus, 3
	csrw	3008, zero
	sw	a3, 0(a2)
	csrw	3008, zero
	csrw	mstatus, a1
	#NO_APP
.Ltmp314:
	#DEBUG_VALUE: hero_store_uint64_noblock:res_upper <- 0
	#DEBUG_VALUE: hero_store_uint32_noblock:mstatus <- $x11
	.loc	7 198 5                         # common/hero_64.h:198:5
	li	a0, 0
.Ltmp315:
	ret
.Ltmp316:
.Lfunc_end17:
	.size	hero_store_uint64_noblock, .Lfunc_end17-hero_store_uint64_noblock
	.cfi_endproc
                                        # -- End function
	.weak	__omp_offloading_10303_7120698_axpy_l19 # -- Begin function __omp_offloading_10303_7120698_axpy_l19
	.p2align	2
	.type	__omp_offloading_10303_7120698_axpy_l19,@function
__omp_offloading_10303_7120698_axpy_l19: # @__omp_offloading_10303_7120698_axpy_l19
.Lfunc_begin18:
	.cfi_startproc
# %bb.0:                                # %entry
	addi	sp, sp, -64
	.cfi_def_cfa_offset 64
	sw	ra, 60(sp)                      # 4-byte Folded Spill
	sw	s0, 56(sp)                      # 4-byte Folded Spill
	sw	s1, 52(sp)                      # 4-byte Folded Spill
	sw	s2, 48(sp)                      # 4-byte Folded Spill
	sw	s3, 44(sp)                      # 4-byte Folded Spill
	sw	s4, 40(sp)                      # 4-byte Folded Spill
	sw	s5, 36(sp)                      # 4-byte Folded Spill
	sw	s6, 32(sp)                      # 4-byte Folded Spill
	sw	s7, 28(sp)                      # 4-byte Folded Spill
	sw	s8, 24(sp)                      # 4-byte Folded Spill
	sw	s9, 20(sp)                      # 4-byte Folded Spill
	sw	s10, 16(sp)                     # 4-byte Folded Spill
	sw	s11, 12(sp)                     # 4-byte Folded Spill
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	.cfi_offset s2, -16
	.cfi_offset s3, -20
	.cfi_offset s4, -24
	.cfi_offset s5, -28
	.cfi_offset s6, -32
	.cfi_offset s7, -36
	.cfi_offset s8, -40
	.cfi_offset s9, -44
	.cfi_offset s10, -48
	.cfi_offset s11, -52
	mv	s1, a1
	mv	s2, a0
	call	hero_load_uint32
	mv	s3, a0
	addi	a0, s2, 8
	sltu	a1, a0, s2
	add	a1, s1, a1
	call	hero_load_uint32
	mv	s4, a0
	addi	a0, s2, 16
	sltu	a1, a0, s2
	add	a1, s1, a1
	call	hero_load_uint32
	mv	s0, a0
	addi	a0, s2, 24
	sltu	a1, a0, s2
	add	a1, s1, a1
	call	hero_load_uint32
	lw	s10, 4(s3)
	lw	a6, 0(s3)
	lw	a7, 4(s4)
	lw	t1, 0(s4)
	lw	t0, 0(a0)
	#APP
	csrr	a0, mhartid
	#NO_APP
	or	a1, a6, s10
	snez	a1, a1
	or	a2, t1, a7
	snez	a2, a2
	and	a1, a1, a2
	snez	a2, t0
	and	a1, a1, a2
	andi	a2, t0, 511
	seqz	a2, a2
	and	a1, a1, a2
	addi	t2, a0, -16
	beqz	a1, .LBB18_4
# %bb.1:
	flw	ft0, 0(s0)
	beqz	t2, .LBB18_6
# %bb.2:
	li	a0, 8
	bne	t2, a0, .LBB18_8
# %bb.3:
	lui	a0, %hi(dma_time)
	sw	zero, %lo(dma_time)(a0)
	lui	a0, %hi(issue_time)
	sw	zero, %lo(issue_time)(a0)
	lui	a0, %hi(all_time)
	addi	a0, a0, %lo(all_time)
	j	.LBB18_7
.LBB18_4:
	bnez	t2, .LBB18_45
# %bb.5:
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	li	a1, 0
	lw	ra, 60(sp)                      # 4-byte Folded Reload
	lw	s0, 56(sp)                      # 4-byte Folded Reload
	lw	s1, 52(sp)                      # 4-byte Folded Reload
	lw	s2, 48(sp)                      # 4-byte Folded Reload
	lw	s3, 44(sp)                      # 4-byte Folded Reload
	lw	s4, 40(sp)                      # 4-byte Folded Reload
	lw	s5, 36(sp)                      # 4-byte Folded Reload
	lw	s6, 32(sp)                      # 4-byte Folded Reload
	lw	s7, 28(sp)                      # 4-byte Folded Reload
	lw	s8, 24(sp)                      # 4-byte Folded Reload
	lw	s9, 20(sp)                      # 4-byte Folded Reload
	lw	s10, 16(sp)                     # 4-byte Folded Reload
	lw	s11, 12(sp)                     # 4-byte Folded Reload
	addi	sp, sp, 64
	tail	printf_
.LBB18_6:
	lui	a0, %hi(compute_time)
	addi	a0, a0, %lo(compute_time)
.LBB18_7:                               # %.sink.split.i.i.i
	snez	t3, t2
	sw	zero, 0(a0)
	j	.LBB18_9
.LBB18_8:
	li	t3, 0
.LBB18_9:
	#APP
	csrr	a0, misa
	#NO_APP
	andi	a0, a0, 32
	beqz	a0, .LBB18_11
# %bb.10:
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.LBB18_11:                              # %pulp_barrier.exit.i.i.i
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
	#APP
	csrr	zero, 1986
	#NO_APP
	beqz	t3, .LBB18_13
# %bb.12:
	#APP
	csrr	a0, mcycle
	#NO_APP
	lui	a1, %hi(all_timer)
	sw	a0, %lo(all_timer)(a1)
	#APP
	csrr	a0, mcycle
	#NO_APP
	lui	t4, %hi(issue_timer)
	sw	a0, %lo(issue_timer)(t4)
	mv	a3, s10
	mv	a2, a6
	#APP
	.word	14024747

	#NO_APP
	lui	a0, %hi(l1_buf_x)
	addi	a0, a0, %lo(l1_buf_x)
	lui	a1, 1
	addi	a4, a1, -2048
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	#APP
	.word	67568939

	#NO_APP
	lui	a0, %hi(l1_buf_y)
	addi	a0, a0, %lo(l1_buf_y)
	mv	a3, a7
	mv	a2, t1
	#APP
	.word	14024747

	#NO_APP
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	#APP
	.word	67568939

	#NO_APP
	#APP
	csrr	a0, mcycle
	#NO_APP
	lw	a1, %lo(issue_timer)(t4)
	lui	a2, %hi(issue_time)
	lw	a3, %lo(issue_time)(a2)
	sub	a0, a0, a1
	add	a0, a0, a3
	sw	a0, %lo(issue_time)(a2)
.LBB18_13:                              # %.lr.ph.i.i.i
	li	a1, 512
	mv	a0, t0
	bltu	a1, t0, .LBB18_15
# %bb.14:                               # %.lr.ph.i.i.i
	li	a0, 512
.LBB18_15:                              # %.lr.ph.i.i.i
	li	ra, 0
	li	t4, 0
	addi	t5, t0, -512
	addi	a0, a0, -1
	srli	t6, a0, 9
	lui	a0, 1
	addi	a4, a0, -2048
	lui	s0, %hi(dma_timer)
	lui	s1, %hi(dma_time)
	lui	a0, %hi(barrier_reg)
	addi	s2, a0, %lo(barrier_reg)
	lui	s3, %hi(issue_timer)
	lui	a0, %hi(l1_buf_x)
	addi	s4, a0, %lo(l1_buf_x)
	lui	a0, %hi(l1_buf_y)
	addi	s5, a0, %lo(l1_buf_y)
	lui	s6, %hi(compute_timer)
	lui	s7, %hi(compute_time)
	lui	s8, %hi(issue_time)
	slli	s9, t2, 8
	mv	s11, a4
	j	.LBB18_18
.LBB18_16:                              # %.thread29.i.i.i
                                        #   in Loop: Header=BB18_18 Depth=1
	beq	t6, t4, .LBB18_38
.LBB18_17:                              # %.backedge.i.i.i
                                        #   in Loop: Header=BB18_18 Depth=1
	addi	t4, t4, 1
	addi	a0, s11, 1024
	addi	s11, a0, 1024
	addi	t5, t5, -512
.LBB18_18:                              # =>This Inner Loop Header: Depth=1
	beqz	t3, .LBB18_24
# %bb.19:                               #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	a0, mcycle
	#NO_APP
	sw	a0, %lo(dma_timer)(s0)
	#APP
.Ltmp317:
	.word	136315563
	bnez	t0, .Ltmp317

	#NO_APP
	#APP
	csrr	a0, mcycle
	#NO_APP
	lw	a1, %lo(dma_timer)(s0)
	lw	a2, %lo(dma_time)(s1)
	sub	a0, a0, a1
	add	a0, a0, a2
	sw	a0, %lo(dma_time)(s1)
	#APP
	csrr	a0, misa
	#NO_APP
	andi	a0, a0, 32
	beqz	a0, .LBB18_21
# %bb.20:                               #   in Loop: Header=BB18_18 Depth=1
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.LBB18_21:                              # %pulp_barrier.exit7.i.i.i
                                        #   in Loop: Header=BB18_18 Depth=1
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
	#APP
	csrr	zero, 1986
	#NO_APP
	#APP
	csrr	a0, mcycle
	#NO_APP
	sw	a0, %lo(issue_timer)(s3)
	blez	t5, .LBB18_23
# %bb.22:                               #   in Loop: Header=BB18_18 Depth=1
	not	a0, t4
	andi	a0, a0, 1
	slli	t0, a0, 11
	add	a2, a6, s11
	sltu	a0, a2, a6
	add	a3, s10, a0
	add	a0, t0, s4
	#APP
	.word	14024747

	#NO_APP
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	#APP
	.word	67568939

	#NO_APP
	add	a2, t1, s11
	sltu	a0, a2, t1
	add	a3, a7, a0
	add	a0, t0, s5
	#APP
	.word	14024747

	#NO_APP
	li	a1, 0
	#APP
	.word	45416491

	#NO_APP
	#APP
	.word	67568939

	#NO_APP
.LBB18_23:                              #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	a0, mcycle
	#NO_APP
	lw	a1, %lo(issue_timer)(s3)
	sub	ra, a0, a1
	j	.LBB18_29
.LBB18_24:                              #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	a0, misa
	#NO_APP
	andi	a0, a0, 32
	beqz	a0, .LBB18_26
# %bb.25:                               #   in Loop: Header=BB18_18 Depth=1
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.LBB18_26:                              # %pulp_barrier.exit9.i.i.i
                                        #   in Loop: Header=BB18_18 Depth=1
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
	#APP
	csrr	zero, 1986
	#NO_APP
	bnez	t2, .LBB18_28
# %bb.27:                               #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	a0, mcycle
	#NO_APP
	sw	a0, %lo(compute_timer)(s6)
.LBB18_28:                              #   in Loop: Header=BB18_18 Depth=1
	andi	a0, t4, 1
	slli	a0, a0, 11
	add	a0, a0, s9
	add	a1, a0, s4
	add	a0, a0, s5
	li	a2, 64
	#APP
	li	t0, 0
.Ltmp318:
	flw	ft1, 0(a1)
	flw	ft2, 0(a0)
	fmadd.s	ft2, ft0, ft1, ft2
	fsw	ft2, 0(a0)
	addi	a1, a1, 4
	addi	a0, a0, 4
	addi	t0, t0, 1
	blt	t0, a2, .Ltmp318

	#NO_APP
.LBB18_29:                              #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	a0, misa
	#NO_APP
	andi	a0, a0, 32
	beqz	a0, .LBB18_31
# %bb.30:                               #   in Loop: Header=BB18_18 Depth=1
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.LBB18_31:                              # %pulp_barrier.exit11.i.i.i
                                        #   in Loop: Header=BB18_18 Depth=1
	#APP
	lw	a0, 0(s2)
	mv	zero, a0

	#NO_APP
	#APP
	csrr	zero, 1986
	#NO_APP
	beqz	t2, .LBB18_33
# %bb.32:                               #   in Loop: Header=BB18_18 Depth=1
	beqz	t3, .LBB18_16
	j	.LBB18_34
.LBB18_33:                              #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	a0, mcycle
	#NO_APP
	lw	a1, %lo(compute_timer)(s6)
	lw	a2, %lo(compute_time)(s7)
	sub	a0, a0, a1
	add	a0, a0, a2
	sw	a0, %lo(compute_time)(s7)
	beqz	t3, .LBB18_16
.LBB18_34:                              #   in Loop: Header=BB18_18 Depth=1
	#APP
	csrr	t0, mcycle
	#NO_APP
	lw	a5, %lo(compute_timer)(s6)
	#APP
	csrr	a0, mcycle
	#NO_APP
	sw	a0, %lo(issue_timer)(s3)
	add	a0, s11, t1
	addi	a0, a0, -2048
	sltu	a1, a0, t1
	andi	a2, t4, 1
	slli	a2, a2, 11
	add	a2, a2, s5
	add	a1, a7, a1
	li	a3, 0
	#APP
	.word	14024747

	#NO_APP
	#APP
	.word	45416491

	#NO_APP
	#APP
	.word	67568939

	#NO_APP
	#APP
	csrr	a0, mcycle
	#NO_APP
	lw	a1, %lo(issue_timer)(s3)
	lw	a2, %lo(issue_time)(s8)
	sub	a3, a5, t0
	add	a3, ra, a3
	bltu	ra, a3, .LBB18_36
# %bb.35:                               #   in Loop: Header=BB18_18 Depth=1
	add	a0, a0, a3
.LBB18_36:                              #   in Loop: Header=BB18_18 Depth=1
	sub	a0, a0, a1
	add	a0, a0, a2
	sw	a0, %lo(issue_time)(s8)
	bne	t6, t4, .LBB18_17
# %bb.37:                               # %._crit_edge.i.i.i
	#APP
	csrr	a0, mcycle
	#NO_APP
	lui	a1, %hi(dma_timer)
	sw	a0, %lo(dma_timer)(a1)
	#APP
.Ltmp319:
	.word	136315563
	bnez	t0, .Ltmp319

	#NO_APP
	#APP
	csrr	a0, mcycle
	#NO_APP
	lw	a1, %lo(dma_timer)(a1)
	lui	a2, %hi(dma_time)
	lw	a3, %lo(dma_time)(a2)
	sub	a0, a0, a1
	add	a0, a0, a3
	sw	a0, %lo(dma_time)(a2)
	#APP
	fence	rw, rw
	#NO_APP
.LBB18_38:                              # %._crit_edge.thread.i.i.i
	#APP
	csrr	a0, misa
	#NO_APP
	andi	a0, a0, 32
	beqz	a0, .LBB18_40
# %bb.39:
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.LBB18_40:                              # %pulp_barrier.exit13.i.i.i
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
	#APP
	csrr	zero, 1986
	#NO_APP
	beqz	t3, .LBB18_42
# %bb.41:
	#APP
	csrr	a0, mcycle
	#NO_APP
	lui	a1, %hi(all_timer)
	lw	a1, %lo(all_timer)(a1)
	sub	a0, a0, a1
	lui	a1, %hi(all_time)
	sw	a0, %lo(all_time)(a1)
	lw	a2, %lo(all_time)(a1)
	lui	a0, %hi(dma_time)
	lw	a3, %lo(dma_time)(a0)
	lui	a0, %hi(issue_time)
	lw	a4, %lo(issue_time)(a0)
	lui	a0, %hi(compute_time)
	lw	a5, %lo(compute_time)(a0)
	lui	a0, %hi(.L.str.1)
	addi	a0, a0, %lo(.L.str.1)
	li	a1, 0
	call	printf_
.LBB18_42:
	#APP
	csrr	a0, misa
	#NO_APP
	andi	a0, a0, 32
	beqz	a0, .LBB18_44
# %bb.43:
	#APP
	fmv.x.w	a0, fa0
	mv	zero, a0

	#NO_APP
.LBB18_44:                              # %pulp_barrier.exit15.i.i.i
	lui	a0, %hi(barrier_reg)
	addi	a0, a0, %lo(barrier_reg)
	#APP
	lw	a0, 0(a0)
	mv	zero, a0

	#NO_APP
	#APP
	csrr	zero, 1986
	#NO_APP
.LBB18_45:                              # %__wrapped_omp__omp_offloading_10303_7120698_axpy_l19.exit
	lw	ra, 60(sp)                      # 4-byte Folded Reload
	lw	s0, 56(sp)                      # 4-byte Folded Reload
	lw	s1, 52(sp)                      # 4-byte Folded Reload
	lw	s2, 48(sp)                      # 4-byte Folded Reload
	lw	s3, 44(sp)                      # 4-byte Folded Reload
	lw	s4, 40(sp)                      # 4-byte Folded Reload
	lw	s5, 36(sp)                      # 4-byte Folded Reload
	lw	s6, 32(sp)                      # 4-byte Folded Reload
	lw	s7, 28(sp)                      # 4-byte Folded Reload
	lw	s8, 24(sp)                      # 4-byte Folded Reload
	lw	s9, 20(sp)                      # 4-byte Folded Reload
	lw	s10, 16(sp)                     # 4-byte Folded Reload
	lw	s11, 12(sp)                     # 4-byte Folded Reload
	addi	sp, sp, 64
	ret
.Lfunc_end18:
	.size	__omp_offloading_10303_7120698_axpy_l19, .Lfunc_end18-__omp_offloading_10303_7120698_axpy_l19
	.cfi_endproc
                                        # -- End function
	.protected	l1_buf_x                # @l1_buf_x
	.type	l1_buf_x,@object
	.section	.noinit_l1,"aw",@progbits
	.globl	l1_buf_x
	.p2align	2
l1_buf_x:
	.zero	4096
	.size	l1_buf_x, 4096

	.protected	l1_buf_y                # @l1_buf_y
	.type	l1_buf_y,@object
	.globl	l1_buf_y
	.p2align	2
l1_buf_y:
	.zero	4096
	.size	l1_buf_y, 4096

	.protected	dma_timer               # @dma_timer
	.type	dma_timer,@object
	.globl	dma_timer
	.p2align	2
dma_timer:
	.word	0                               # 0x0
	.size	dma_timer, 4

	.protected	dma_time                # @dma_time
	.type	dma_time,@object
	.globl	dma_time
	.p2align	2
dma_time:
	.word	0                               # 0x0
	.size	dma_time, 4

	.protected	all_timer               # @all_timer
	.type	all_timer,@object
	.globl	all_timer
	.p2align	2
all_timer:
	.word	0                               # 0x0
	.size	all_timer, 4

	.protected	all_time                # @all_time
	.type	all_time,@object
	.globl	all_time
	.p2align	2
all_time:
	.word	0                               # 0x0
	.size	all_time, 4

	.protected	issue_timer             # @issue_timer
	.type	issue_timer,@object
	.globl	issue_timer
	.p2align	2
issue_timer:
	.word	0                               # 0x0
	.size	issue_timer, 4

	.protected	issue_time              # @issue_time
	.type	issue_time,@object
	.globl	issue_time
	.p2align	2
issue_time:
	.word	0                               # 0x0
	.size	issue_time, 4

	.protected	compute_timer           # @compute_timer
	.type	compute_timer,@object
	.globl	compute_timer
	.p2align	2
compute_timer:
	.word	0                               # 0x0
	.size	compute_timer, 4

	.protected	compute_time            # @compute_time
	.type	compute_time,@object
	.globl	compute_time
	.p2align	2
compute_time:
	.word	0                               # 0x0
	.size	compute_time, 4

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Error!\n\r"
	.size	.L.str, 9

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"Tot : %u Dma : %u Issue : %u Compute : %u\n"
	.size	.L.str.1, 43

	.type	.omp_offloading.entry_name,@object # @.omp_offloading.entry_name
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4
.omp_offloading.entry_name:
	.asciz	"__omp_offloading_10303_7120698_axpy_l19"
	.size	.omp_offloading.entry_name, 40

	.type	.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19,@object # @.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19
	.section	omp_offloading_entries,"a",@progbits
	.weak	.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19
.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19:
	.word	__omp_offloading_10303_7120698_axpy_l19
	.word	.omp_offloading.entry_name
	.word	0                               # 0x0
	.word	0                               # 0x0
	.word	0                               # 0x0
	.size	.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19, 20

	.section	.debug_loclists,"",@progbits
	.word	.Ldebug_list_header_end0-.Ldebug_list_header_start0 # Length
.Ldebug_list_header_start0:
	.half	5                               # Version
	.byte	4                               # Address size
	.byte	0                               # Segment selector size
	.word	118                             # Offset entry count
.Lloclists_table_base0:
	.word	.Ldebug_loc0-.Lloclists_table_base0
	.word	.Ldebug_loc1-.Lloclists_table_base0
	.word	.Ldebug_loc2-.Lloclists_table_base0
	.word	.Ldebug_loc3-.Lloclists_table_base0
	.word	.Ldebug_loc4-.Lloclists_table_base0
	.word	.Ldebug_loc5-.Lloclists_table_base0
	.word	.Ldebug_loc6-.Lloclists_table_base0
	.word	.Ldebug_loc7-.Lloclists_table_base0
	.word	.Ldebug_loc8-.Lloclists_table_base0
	.word	.Ldebug_loc9-.Lloclists_table_base0
	.word	.Ldebug_loc10-.Lloclists_table_base0
	.word	.Ldebug_loc11-.Lloclists_table_base0
	.word	.Ldebug_loc12-.Lloclists_table_base0
	.word	.Ldebug_loc13-.Lloclists_table_base0
	.word	.Ldebug_loc14-.Lloclists_table_base0
	.word	.Ldebug_loc15-.Lloclists_table_base0
	.word	.Ldebug_loc16-.Lloclists_table_base0
	.word	.Ldebug_loc17-.Lloclists_table_base0
	.word	.Ldebug_loc18-.Lloclists_table_base0
	.word	.Ldebug_loc19-.Lloclists_table_base0
	.word	.Ldebug_loc20-.Lloclists_table_base0
	.word	.Ldebug_loc21-.Lloclists_table_base0
	.word	.Ldebug_loc22-.Lloclists_table_base0
	.word	.Ldebug_loc23-.Lloclists_table_base0
	.word	.Ldebug_loc24-.Lloclists_table_base0
	.word	.Ldebug_loc25-.Lloclists_table_base0
	.word	.Ldebug_loc26-.Lloclists_table_base0
	.word	.Ldebug_loc27-.Lloclists_table_base0
	.word	.Ldebug_loc28-.Lloclists_table_base0
	.word	.Ldebug_loc29-.Lloclists_table_base0
	.word	.Ldebug_loc30-.Lloclists_table_base0
	.word	.Ldebug_loc31-.Lloclists_table_base0
	.word	.Ldebug_loc32-.Lloclists_table_base0
	.word	.Ldebug_loc33-.Lloclists_table_base0
	.word	.Ldebug_loc34-.Lloclists_table_base0
	.word	.Ldebug_loc35-.Lloclists_table_base0
	.word	.Ldebug_loc36-.Lloclists_table_base0
	.word	.Ldebug_loc37-.Lloclists_table_base0
	.word	.Ldebug_loc38-.Lloclists_table_base0
	.word	.Ldebug_loc39-.Lloclists_table_base0
	.word	.Ldebug_loc40-.Lloclists_table_base0
	.word	.Ldebug_loc41-.Lloclists_table_base0
	.word	.Ldebug_loc42-.Lloclists_table_base0
	.word	.Ldebug_loc43-.Lloclists_table_base0
	.word	.Ldebug_loc44-.Lloclists_table_base0
	.word	.Ldebug_loc45-.Lloclists_table_base0
	.word	.Ldebug_loc46-.Lloclists_table_base0
	.word	.Ldebug_loc47-.Lloclists_table_base0
	.word	.Ldebug_loc48-.Lloclists_table_base0
	.word	.Ldebug_loc49-.Lloclists_table_base0
	.word	.Ldebug_loc50-.Lloclists_table_base0
	.word	.Ldebug_loc51-.Lloclists_table_base0
	.word	.Ldebug_loc52-.Lloclists_table_base0
	.word	.Ldebug_loc53-.Lloclists_table_base0
	.word	.Ldebug_loc54-.Lloclists_table_base0
	.word	.Ldebug_loc55-.Lloclists_table_base0
	.word	.Ldebug_loc56-.Lloclists_table_base0
	.word	.Ldebug_loc57-.Lloclists_table_base0
	.word	.Ldebug_loc58-.Lloclists_table_base0
	.word	.Ldebug_loc59-.Lloclists_table_base0
	.word	.Ldebug_loc60-.Lloclists_table_base0
	.word	.Ldebug_loc61-.Lloclists_table_base0
	.word	.Ldebug_loc62-.Lloclists_table_base0
	.word	.Ldebug_loc63-.Lloclists_table_base0
	.word	.Ldebug_loc64-.Lloclists_table_base0
	.word	.Ldebug_loc65-.Lloclists_table_base0
	.word	.Ldebug_loc66-.Lloclists_table_base0
	.word	.Ldebug_loc67-.Lloclists_table_base0
	.word	.Ldebug_loc68-.Lloclists_table_base0
	.word	.Ldebug_loc69-.Lloclists_table_base0
	.word	.Ldebug_loc70-.Lloclists_table_base0
	.word	.Ldebug_loc71-.Lloclists_table_base0
	.word	.Ldebug_loc72-.Lloclists_table_base0
	.word	.Ldebug_loc73-.Lloclists_table_base0
	.word	.Ldebug_loc74-.Lloclists_table_base0
	.word	.Ldebug_loc75-.Lloclists_table_base0
	.word	.Ldebug_loc76-.Lloclists_table_base0
	.word	.Ldebug_loc77-.Lloclists_table_base0
	.word	.Ldebug_loc78-.Lloclists_table_base0
	.word	.Ldebug_loc79-.Lloclists_table_base0
	.word	.Ldebug_loc80-.Lloclists_table_base0
	.word	.Ldebug_loc81-.Lloclists_table_base0
	.word	.Ldebug_loc82-.Lloclists_table_base0
	.word	.Ldebug_loc83-.Lloclists_table_base0
	.word	.Ldebug_loc84-.Lloclists_table_base0
	.word	.Ldebug_loc85-.Lloclists_table_base0
	.word	.Ldebug_loc86-.Lloclists_table_base0
	.word	.Ldebug_loc87-.Lloclists_table_base0
	.word	.Ldebug_loc88-.Lloclists_table_base0
	.word	.Ldebug_loc89-.Lloclists_table_base0
	.word	.Ldebug_loc90-.Lloclists_table_base0
	.word	.Ldebug_loc91-.Lloclists_table_base0
	.word	.Ldebug_loc92-.Lloclists_table_base0
	.word	.Ldebug_loc93-.Lloclists_table_base0
	.word	.Ldebug_loc94-.Lloclists_table_base0
	.word	.Ldebug_loc95-.Lloclists_table_base0
	.word	.Ldebug_loc96-.Lloclists_table_base0
	.word	.Ldebug_loc97-.Lloclists_table_base0
	.word	.Ldebug_loc98-.Lloclists_table_base0
	.word	.Ldebug_loc99-.Lloclists_table_base0
	.word	.Ldebug_loc100-.Lloclists_table_base0
	.word	.Ldebug_loc101-.Lloclists_table_base0
	.word	.Ldebug_loc102-.Lloclists_table_base0
	.word	.Ldebug_loc103-.Lloclists_table_base0
	.word	.Ldebug_loc104-.Lloclists_table_base0
	.word	.Ldebug_loc105-.Lloclists_table_base0
	.word	.Ldebug_loc106-.Lloclists_table_base0
	.word	.Ldebug_loc107-.Lloclists_table_base0
	.word	.Ldebug_loc108-.Lloclists_table_base0
	.word	.Ldebug_loc109-.Lloclists_table_base0
	.word	.Ldebug_loc110-.Lloclists_table_base0
	.word	.Ldebug_loc111-.Lloclists_table_base0
	.word	.Ldebug_loc112-.Lloclists_table_base0
	.word	.Ldebug_loc113-.Lloclists_table_base0
	.word	.Ldebug_loc114-.Lloclists_table_base0
	.word	.Ldebug_loc115-.Lloclists_table_base0
	.word	.Ldebug_loc116-.Lloclists_table_base0
	.word	.Ldebug_loc117-.Lloclists_table_base0
.Ldebug_loc0:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp118-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	42                              # 42
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc1:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp5-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	94                              # DW_OP_reg14
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp5-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp9-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	85                              # DW_OP_reg5
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp9-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp13-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	94                              # DW_OP_reg14
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp13-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	85                              # DW_OP_reg5
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc2:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp11-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	97                              # DW_OP_reg17
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp11-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp13-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	97                              # DW_OP_reg17
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp13-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	97                              # DW_OP_reg17
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp37-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp125-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc3:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp6-.Lfunc_begin0           #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp6-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp9-.Lfunc_begin0           #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp9-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp13-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp13-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp117-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp117-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp118-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc4:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp1-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp4-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc5:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp3-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp37-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp118-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	81                              # DW_OP_reg1
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc6:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp3-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp118-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	87                              # DW_OP_reg7
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc7:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp21-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp23-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc8:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp27-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp33-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	97                              # DW_OP_reg17
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc9:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp28-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp29-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc10:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp33-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc11:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp30-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp31-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc12:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp33-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp37-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp39-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	141                             # DW_OP_breg29
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp41-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp101-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	109                             # DW_OP_reg29
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp101-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp118-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	141                             # DW_OP_breg29
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc13:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp33-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp37-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp40-.Lfunc_begin0          #   ending offset
	.byte	18                              # Loc expr size
	.byte	139                             # DW_OP_breg27
	.byte	0                               # 0
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	28                              # DW_OP_minus
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	27                              # DW_OP_div
	.byte	17                              # DW_OP_consts
	.byte	128                             # 512
	.byte	4                               # 
	.byte	30                              # DW_OP_mul
	.byte	35                              # DW_OP_plus_uconst
	.byte	128                             # 512
	.byte	4                               # 
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp41-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp101-.Lfunc_begin0         #   ending offset
	.byte	15                              # Loc expr size
	.byte	139                             # DW_OP_breg27
	.byte	0                               # 0
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	28                              # DW_OP_minus
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	27                              # DW_OP_div
	.byte	17                              # DW_OP_consts
	.byte	128                             # 512
	.byte	4                               # 
	.byte	30                              # DW_OP_mul
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp101-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp102-.Lfunc_begin0         #   ending offset
	.byte	18                              # Loc expr size
	.byte	139                             # DW_OP_breg27
	.byte	0                               # 0
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	28                              # DW_OP_minus
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	27                              # DW_OP_div
	.byte	17                              # DW_OP_consts
	.byte	128                             # 512
	.byte	4                               # 
	.byte	30                              # DW_OP_mul
	.byte	35                              # DW_OP_plus_uconst
	.byte	128                             # 512
	.byte	4                               # 
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc14:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp53-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp55-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc15:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp58-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp60-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp60-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp61-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc16:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp62-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp63-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc17:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp71-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp73-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc18:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp76-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp79-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc19:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp77-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp79-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc20:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp84-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp86-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc21:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp94-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp95-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp95-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp97-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc22:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp113-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp115-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc23:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp123-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp124-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc24:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin1-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp129-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc25:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin1-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc26:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin1-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp157-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc27:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin1-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp131-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc28:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp128-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp247-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	86                              # DW_OP_reg6
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	97                              # DW_OP_reg17
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc29:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp128-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp140-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp140-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp142-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp142-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp247-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp247-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp253-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc30:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp135-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp138-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	32                              # 32
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp142-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp247-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	32                              # 32
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc31:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp128-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp167-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	85                              # DW_OP_reg5
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc32:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp129-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp134-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc33:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp133-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp167-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp167-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp247-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	81                              # DW_OP_reg1
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc34:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp133-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp247-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	87                              # DW_OP_reg7
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc35:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp150-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp152-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc36:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp156-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp163-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	96                              # DW_OP_reg16
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	106                             # DW_OP_reg26
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc37:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp158-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp159-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc38:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp159-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp163-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	86                              # DW_OP_reg6
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	97                              # DW_OP_reg17
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc39:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp160-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp161-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc40:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp163-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp167-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp167-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp169-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	141                             # DW_OP_breg29
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp171-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp231-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	109                             # DW_OP_reg29
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp231-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp247-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	141                             # DW_OP_breg29
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc41:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp164-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp167-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp167-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp170-.Lfunc_begin0         #   ending offset
	.byte	18                              # Loc expr size
	.byte	139                             # DW_OP_breg27
	.byte	0                               # 0
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	28                              # DW_OP_minus
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	27                              # DW_OP_div
	.byte	17                              # DW_OP_consts
	.byte	128                             # 512
	.byte	4                               # 
	.byte	30                              # DW_OP_mul
	.byte	35                              # DW_OP_plus_uconst
	.byte	128                             # 512
	.byte	4                               # 
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp171-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp231-.Lfunc_begin0         #   ending offset
	.byte	15                              # Loc expr size
	.byte	139                             # DW_OP_breg27
	.byte	0                               # 0
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	28                              # DW_OP_minus
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	27                              # DW_OP_div
	.byte	17                              # DW_OP_consts
	.byte	128                             # 512
	.byte	4                               # 
	.byte	30                              # DW_OP_mul
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp231-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp232-.Lfunc_begin0         #   ending offset
	.byte	18                              # Loc expr size
	.byte	139                             # DW_OP_breg27
	.byte	0                               # 0
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	28                              # DW_OP_minus
	.byte	17                              # DW_OP_consts
	.byte	128                             # 2048
	.byte	16                              # 
	.byte	27                              # DW_OP_div
	.byte	17                              # DW_OP_consts
	.byte	128                             # 512
	.byte	4                               # 
	.byte	30                              # DW_OP_mul
	.byte	35                              # DW_OP_plus_uconst
	.byte	128                             # 512
	.byte	4                               # 
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc42:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp183-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp185-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc43:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp188-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp190-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp190-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp191-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc44:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp192-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp193-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc45:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp201-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp203-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc46:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp206-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp209-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc47:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp207-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp209-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc48:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp214-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp216-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc49:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp224-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp225-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp225-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp227-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc50:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp243-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp245-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc51:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp252-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp253-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc52:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin2-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp256-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp256-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end2-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc53:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin2-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp256-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc54:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp255-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end2-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc55:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin3-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp259-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp259-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end3-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc56:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin3-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp259-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp259-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end3-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc57:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp258-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end3-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc58:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin4-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp262-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp262-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end4-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc59:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin4-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp262-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc60:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp261-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end4-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc61:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin5-.Lfunc_begin0    #   starting offset
	.uleb128 .Lfunc_end5-.Lfunc_begin0      #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc62:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin5-.Lfunc_begin0    #   starting offset
	.uleb128 .Lfunc_end5-.Lfunc_begin0      #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc63:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp264-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end5-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc64:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin6-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp267-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp267-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end6-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc65:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin6-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp267-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc66:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin7-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp270-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp270-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end7-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc67:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin7-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp270-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp270-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end7-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc68:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp269-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end7-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc69:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin8-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp273-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp273-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end8-.Lfunc_begin0      #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc70:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin8-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp273-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc71:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp272-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end8-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc72:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin9-.Lfunc_begin0    #   starting offset
	.uleb128 .Lfunc_end9-.Lfunc_begin0      #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc73:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin9-.Lfunc_begin0    #   starting offset
	.uleb128 .Lfunc_end9-.Lfunc_begin0      #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc74:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp275-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end9-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc75:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin10-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp278-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp278-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end10-.Lfunc_begin0     #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc76:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin10-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp278-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc77:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin11-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp281-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp281-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end11-.Lfunc_begin0     #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc78:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin11-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp281-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp281-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end11-.Lfunc_begin0     #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc79:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp280-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end11-.Lfunc_begin0     #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc80:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin12-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp284-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp284-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end12-.Lfunc_begin0     #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc81:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin12-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp284-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc82:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp283-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end12-.Lfunc_begin0     #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc83:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin13-.Lfunc_begin0   #   starting offset
	.uleb128 .Lfunc_end13-.Lfunc_begin0     #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc84:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin13-.Lfunc_begin0   #   starting offset
	.uleb128 .Lfunc_end13-.Lfunc_begin0     #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc85:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp286-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end13-.Lfunc_begin0     #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc86:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin14-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp290-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp290-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp292-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc87:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin14-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp290-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp290-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp292-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc88:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin14-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp290-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp290-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp292-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc89:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp288-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp292-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp292-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end14-.Lfunc_begin0     #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc90:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp288-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end14-.Lfunc_begin0     #   ending offset
	.byte	1                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc91:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp291-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp293-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp293-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end14-.Lfunc_begin0     #   ending offset
	.byte	3                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc92:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp291-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp293-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp293-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end14-.Lfunc_begin0     #   ending offset
	.byte	3                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc93:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp292-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end14-.Lfunc_begin0     #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc94:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin15-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp296-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp296-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end15-.Lfunc_begin0     #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc95:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin15-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp297-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp297-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp299-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc96:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin15-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp297-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp297-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp299-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc97:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin15-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp297-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp297-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp299-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc98:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin15-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp296-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc99:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp295-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end15-.Lfunc_begin0     #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc100:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp298-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end15-.Lfunc_begin0     #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc101:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp298-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end15-.Lfunc_begin0     #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc102:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin16-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp304-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp304-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp306-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc103:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin16-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp304-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp304-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp306-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc104:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp301-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp303-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc105:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp302-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end16-.Lfunc_begin0     #   ending offset
	.byte	3                               # Loc expr size
	.byte	124                             # DW_OP_breg12
	.byte	4                               # 4
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc106:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp302-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end16-.Lfunc_begin0     #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc107:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp305-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp308-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp308-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end16-.Lfunc_begin0     #   ending offset
	.byte	3                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc108:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp306-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end16-.Lfunc_begin0     #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc109:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp307-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end16-.Lfunc_begin0     #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc110:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin17-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp311-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp311-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end17-.Lfunc_begin0     #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	93                              # DW_OP_reg13
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc111:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin17-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp312-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp312-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp314-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc112:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin17-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp312-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp312-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp314-.Lfunc_begin0         #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	91                              # DW_OP_reg11
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc113:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin17-.Lfunc_begin0   #   starting offset
	.uleb128 .Ltmp311-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc114:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp310-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end17-.Lfunc_begin0     #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc115:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp310-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end17-.Lfunc_begin0     #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc116:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp313-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp315-.Lfunc_begin0         #   ending offset
	.byte	6                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	90                              # DW_OP_reg10
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp315-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end17-.Lfunc_begin0     #   ending offset
	.byte	3                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	4                               # 4
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc117:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp314-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end17-.Lfunc_begin0     #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_list_header_end0:
	.section	.debug_abbrev,"",@progbits
	.byte	1                               # Abbreviation Code
	.byte	17                              # DW_TAG_compile_unit
	.byte	1                               # DW_CHILDREN_yes
	.byte	37                              # DW_AT_producer
	.byte	37                              # DW_FORM_strx1
	.byte	19                              # DW_AT_language
	.byte	5                               # DW_FORM_data2
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	114                             # DW_AT_str_offsets_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	16                              # DW_AT_stmt_list
	.byte	23                              # DW_FORM_sec_offset
	.byte	27                              # DW_AT_comp_dir
	.byte	37                              # DW_FORM_strx1
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	115                             # DW_AT_addr_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	116                             # DW_AT_rnglists_base
	.byte	23                              # DW_FORM_sec_offset
	.ascii	"\214\001"                      # DW_AT_loclists_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	2                               # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	3                               # Abbreviation Code
	.byte	1                               # DW_TAG_array_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	4                               # Abbreviation Code
	.byte	33                              # DW_TAG_subrange_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	55                              # DW_AT_count
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	5                               # Abbreviation Code
	.byte	36                              # DW_TAG_base_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	62                              # DW_AT_encoding
	.byte	11                              # DW_FORM_data1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	6                               # Abbreviation Code
	.byte	36                              # DW_TAG_base_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	62                              # DW_AT_encoding
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	7                               # Abbreviation Code
	.byte	53                              # DW_TAG_volatile_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	8                               # Abbreviation Code
	.byte	22                              # DW_TAG_typedef
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	9                               # Abbreviation Code
	.byte	15                              # DW_TAG_pointer_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	10                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	11                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	12                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	13                              # Abbreviation Code
	.byte	11                              # DW_TAG_lexical_block
	.byte	1                               # DW_CHILDREN_yes
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	14                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	15                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	16                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	5                               # DW_FORM_data2
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	17                              # Abbreviation Code
	.byte	38                              # DW_TAG_const_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	18                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	5                               # DW_FORM_data2
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	19                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.byte	122                             # DW_AT_call_all_calls
	.byte	25                              # DW_FORM_flag_present
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	20                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	21                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	22                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	85                              # DW_AT_ranges
	.byte	35                              # DW_FORM_rnglistx
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	23                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	24                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	25                              # Abbreviation Code
	.byte	11                              # DW_TAG_lexical_block
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	26                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	27                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	0                               # DW_CHILDREN_no
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	28                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	29                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	28                              # DW_AT_const_value
	.byte	15                              # DW_FORM_udata
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	30                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	31                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	28                              # DW_AT_const_value
	.byte	15                              # DW_FORM_udata
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	32                              # Abbreviation Code
	.byte	11                              # DW_TAG_lexical_block
	.byte	1                               # DW_CHILDREN_yes
	.byte	85                              # DW_AT_ranges
	.byte	35                              # DW_FORM_rnglistx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	33                              # Abbreviation Code
	.byte	72                              # DW_TAG_call_site
	.byte	0                               # DW_CHILDREN_no
	.byte	127                             # DW_AT_call_origin
	.byte	19                              # DW_FORM_ref4
	.ascii	"\202\001"                      # DW_AT_call_tail_call
	.byte	25                              # DW_FORM_flag_present
	.ascii	"\201\001"                      # DW_AT_call_pc
	.byte	27                              # DW_FORM_addrx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	34                              # Abbreviation Code
	.byte	72                              # DW_TAG_call_site
	.byte	0                               # DW_CHILDREN_no
	.byte	127                             # DW_AT_call_origin
	.byte	19                              # DW_FORM_ref4
	.byte	125                             # DW_AT_call_return_pc
	.byte	27                              # DW_FORM_addrx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	35                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	60                              # DW_AT_declaration
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	36                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	37                              # Abbreviation Code
	.byte	24                              # DW_TAG_unspecified_parameters
	.byte	0                               # DW_CHILDREN_no
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	38                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	39                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	40                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.byte	122                             # DW_AT_call_all_calls
	.byte	25                              # DW_FORM_flag_present
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	41                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	52                              # DW_AT_artificial
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	42                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.byte	122                             # DW_AT_call_all_calls
	.byte	25                              # DW_FORM_flag_present
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	43                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	44                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	5                               # DW_FORM_data2
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	45                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.byte	122                             # DW_AT_call_all_calls
	.byte	25                              # DW_FORM_flag_present
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	46                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	47                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	28                              # DW_AT_const_value
	.byte	13                              # DW_FORM_sdata
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	0                               # EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.word	.Ldebug_info_end0-.Ldebug_info_start0 # Length of Unit
.Ldebug_info_start0:
	.half	5                               # DWARF version number
	.byte	1                               # DWARF Unit Type
	.byte	4                               # Address Size (in bytes)
	.word	.debug_abbrev                   # Offset Into Abbrev. Section
	.byte	1                               # Abbrev [1] 0xc:0x11ca DW_TAG_compile_unit
	.byte	0                               # DW_AT_producer
	.half	12                              # DW_AT_language
	.byte	1                               # DW_AT_name
	.word	.Lstr_offsets_base0             # DW_AT_str_offsets_base
	.word	.Lline_table_start0             # DW_AT_stmt_list
	.byte	2                               # DW_AT_comp_dir
	.byte	10                              # DW_AT_low_pc
	.word	.Lfunc_end17-.Lfunc_begin0      # DW_AT_high_pc
	.word	.Laddr_table_base0              # DW_AT_addr_base
	.word	.Lrnglists_table_base0          # DW_AT_rnglists_base
	.word	.Lloclists_table_base0          # DW_AT_loclists_base
	.byte	2                               # Abbrev [2] 0x2b:0xb DW_TAG_variable
	.byte	3                               # DW_AT_name
	.word	54                              # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	0
	.byte	3                               # Abbrev [3] 0x36:0x18 DW_TAG_array_type
	.word	78                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0x3b:0x6 DW_TAG_subrange_type
	.word	82                              # DW_AT_type
	.byte	2                               # DW_AT_count
	.byte	4                               # Abbrev [4] 0x41:0x6 DW_TAG_subrange_type
	.word	82                              # DW_AT_type
	.byte	8                               # DW_AT_count
	.byte	4                               # Abbrev [4] 0x47:0x6 DW_TAG_subrange_type
	.word	82                              # DW_AT_type
	.byte	64                              # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x4e:0x4 DW_TAG_base_type
	.byte	4                               # DW_AT_name
	.byte	4                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	6                               # Abbrev [6] 0x52:0x4 DW_TAG_base_type
	.byte	5                               # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	7                               # DW_AT_encoding
	.byte	2                               # Abbrev [2] 0x56:0xb DW_TAG_variable
	.byte	6                               # DW_AT_name
	.word	54                              # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	50                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	1
	.byte	2                               # Abbrev [2] 0x61:0xb DW_TAG_variable
	.byte	7                               # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	2
	.byte	7                               # Abbrev [7] 0x6c:0x5 DW_TAG_volatile_type
	.word	113                             # DW_AT_type
	.byte	8                               # Abbrev [8] 0x71:0x8 DW_TAG_typedef
	.word	121                             # DW_AT_type
	.byte	10                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0x79:0x8 DW_TAG_typedef
	.word	129                             # DW_AT_type
	.byte	9                               # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	79                              # DW_AT_decl_line
	.byte	5                               # Abbrev [5] 0x81:0x4 DW_TAG_base_type
	.byte	8                               # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # Abbrev [2] 0x85:0xb DW_TAG_variable
	.byte	11                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	3
	.byte	2                               # Abbrev [2] 0x90:0xb DW_TAG_variable
	.byte	12                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	4
	.byte	2                               # Abbrev [2] 0x9b:0xb DW_TAG_variable
	.byte	13                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	5
	.byte	2                               # Abbrev [2] 0xa6:0xb DW_TAG_variable
	.byte	14                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	6
	.byte	2                               # Abbrev [2] 0xb1:0xb DW_TAG_variable
	.byte	15                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	7
	.byte	2                               # Abbrev [2] 0xbc:0xb DW_TAG_variable
	.byte	16                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	8
	.byte	2                               # Abbrev [2] 0xc7:0xb DW_TAG_variable
	.byte	17                              # DW_AT_name
	.word	108                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	9
	.byte	8                               # Abbrev [8] 0xd2:0x8 DW_TAG_typedef
	.word	218                             # DW_AT_type
	.byte	20                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	60                              # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0xda:0x8 DW_TAG_typedef
	.word	226                             # DW_AT_type
	.byte	19                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	105                             # DW_AT_decl_line
	.byte	5                               # Abbrev [5] 0xe2:0x4 DW_TAG_base_type
	.byte	18                              # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0xe6:0x5 DW_TAG_pointer_type
	.word	108                             # DW_AT_type
	.byte	9                               # Abbrev [9] 0xeb:0x5 DW_TAG_pointer_type
	.word	113                             # DW_AT_type
	.byte	9                               # Abbrev [9] 0xf0:0x5 DW_TAG_pointer_type
	.word	245                             # DW_AT_type
	.byte	7                               # Abbrev [7] 0xf5:0x5 DW_TAG_volatile_type
	.word	250                             # DW_AT_type
	.byte	8                               # Abbrev [8] 0xfa:0x8 DW_TAG_typedef
	.word	258                             # DW_AT_type
	.byte	23                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	36                              # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0x102:0x8 DW_TAG_typedef
	.word	266                             # DW_AT_type
	.byte	22                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	57                              # DW_AT_decl_line
	.byte	5                               # Abbrev [5] 0x10a:0x4 DW_TAG_base_type
	.byte	21                              # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	2                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x10e:0x5 DW_TAG_pointer_type
	.word	250                             # DW_AT_type
	.byte	9                               # Abbrev [9] 0x113:0x5 DW_TAG_pointer_type
	.word	280                             # DW_AT_type
	.byte	7                               # Abbrev [7] 0x118:0x5 DW_TAG_volatile_type
	.word	285                             # DW_AT_type
	.byte	8                               # Abbrev [8] 0x11d:0x8 DW_TAG_typedef
	.word	293                             # DW_AT_type
	.byte	26                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	24                              # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0x125:0x8 DW_TAG_typedef
	.word	301                             # DW_AT_type
	.byte	25                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	43                              # DW_AT_decl_line
	.byte	5                               # Abbrev [5] 0x12d:0x4 DW_TAG_base_type
	.byte	24                              # DW_AT_name
	.byte	8                               # DW_AT_encoding
	.byte	1                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x131:0x5 DW_TAG_pointer_type
	.word	285                             # DW_AT_type
	.byte	8                               # Abbrev [8] 0x136:0x8 DW_TAG_typedef
	.word	318                             # DW_AT_type
	.byte	28                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	82                              # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0x13e:0x8 DW_TAG_typedef
	.word	129                             # DW_AT_type
	.byte	27                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	232                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x146:0x11 DW_TAG_subprogram
	.byte	29                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.word	343                             # DW_AT_type
                                        # DW_AT_inline
	.byte	11                              # Abbrev [11] 0x14e:0x8 DW_TAG_variable
	.byte	31                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.word	343                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	8                               # Abbrev [8] 0x157:0x8 DW_TAG_typedef
	.word	113                             # DW_AT_type
	.byte	30                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	10                              # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x15f:0x19 DW_TAG_subprogram
	.byte	32                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	39                              # DW_AT_decl_line
                                        # DW_AT_inline
	.byte	13                              # Abbrev [13] 0x163:0xa DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x164:0x8 DW_TAG_variable
	.byte	33                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	40                              # DW_AT_decl_line
	.word	376                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	13                              # Abbrev [13] 0x16d:0xa DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x16e:0x8 DW_TAG_variable
	.byte	35                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	41                              # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x178:0x4 DW_TAG_base_type
	.byte	34                              # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	12                              # Abbrev [12] 0x17c:0xd DW_TAG_subprogram
	.byte	36                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
                                        # DW_AT_inline
	.byte	11                              # Abbrev [11] 0x180:0x8 DW_TAG_variable
	.byte	35                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	71                              # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	10                              # Abbrev [10] 0x189:0x13 DW_TAG_subprogram
	.byte	37                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	34                              # DW_AT_decl_line
	.word	412                             # DW_AT_type
                                        # DW_AT_inline
	.byte	13                              # Abbrev [13] 0x191:0xa DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x192:0x8 DW_TAG_variable
	.byte	33                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	35                              # DW_AT_decl_line
	.word	376                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	8                               # Abbrev [8] 0x19c:0x8 DW_TAG_typedef
	.word	113                             # DW_AT_type
	.byte	38                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	11                              # DW_AT_decl_line
	.byte	14                              # Abbrev [14] 0x1a4:0x5a DW_TAG_subprogram
	.byte	39                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	241                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	510                             # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0x1ac:0x8 DW_TAG_formal_parameter
	.byte	41                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	242                             # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x1b4:0x8 DW_TAG_formal_parameter
	.byte	42                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	243                             # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x1bc:0x8 DW_TAG_formal_parameter
	.byte	43                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	244                             # DW_AT_decl_line
	.word	518                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x1c4:0x8 DW_TAG_formal_parameter
	.byte	45                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	245                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x1cc:0x8 DW_TAG_variable
	.byte	46                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	250                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x1d4:0x8 DW_TAG_variable
	.byte	47                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	251                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x1dc:0x8 DW_TAG_variable
	.byte	48                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	249                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x1e4:0x8 DW_TAG_variable
	.byte	49                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	248                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x1ec:0x8 DW_TAG_variable
	.byte	50                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	247                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	16                              # Abbrev [16] 0x1f4:0x9 DW_TAG_variable
	.byte	51                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.half	274                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	8                               # Abbrev [8] 0x1fe:0x8 DW_TAG_typedef
	.word	113                             # DW_AT_type
	.byte	40                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	238                             # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0x206:0x8 DW_TAG_typedef
	.word	129                             # DW_AT_type
	.byte	44                              # DW_AT_name
	.byte	5                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.byte	17                              # Abbrev [17] 0x20e:0x5 DW_TAG_const_type
	.word	113                             # DW_AT_type
	.byte	18                              # Abbrev [18] 0x213:0x5 DW_TAG_subprogram
	.byte	52                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.half	446                             # DW_AT_decl_line
                                        # DW_AT_inline
	.byte	19                              # Abbrev [19] 0x218:0x3e4 DW_TAG_subprogram
	.byte	10                              # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	56                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_external
	.byte	20                              # Abbrev [20] 0x223:0x9 DW_TAG_formal_parameter
	.byte	2                               # DW_AT_location
	.byte	57                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0x22c:0x9 DW_TAG_formal_parameter
	.byte	3                               # DW_AT_location
	.byte	58                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0x235:0x9 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_location
	.byte	59                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	78                              # DW_AT_type
	.byte	20                              # Abbrev [20] 0x23e:0x9 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	60                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x247:0x9 DW_TAG_variable
	.byte	5                               # DW_AT_location
	.byte	61                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	58                              # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x250:0x9 DW_TAG_variable
	.byte	6                               # DW_AT_location
	.byte	62                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	56                              # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x259:0x9 DW_TAG_variable
	.byte	12                              # DW_AT_location
	.byte	63                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	84                              # DW_AT_decl_line
	.word	1547                            # DW_AT_type
	.byte	22                              # Abbrev [22] 0x262:0x10 DW_TAG_inlined_subroutine
	.word	326                             # DW_AT_abstract_origin
	.byte	0                               # DW_AT_ranges
	.byte	1                               # DW_AT_call_file
	.byte	56                              # DW_AT_call_line
	.byte	31                              # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x26b:0x6 DW_TAG_variable
	.byte	4                               # DW_AT_location
	.word	334                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x272:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	11                              # DW_AT_low_pc
	.word	.Ltmp22-.Ltmp16                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	73                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x27f:0x6 DW_TAG_variable
	.byte	7                               # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x285:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	11                              # DW_AT_low_pc
	.word	.Ltmp20-.Ltmp16                 # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x292:0xc DW_TAG_lexical_block
	.byte	12                              # DW_AT_low_pc
	.word	.Ltmp20-.Ltmp19                 # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x298:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x2a0:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	13                              # DW_AT_low_pc
	.word	.Ltmp24-.Ltmp23                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	75                              # DW_AT_call_line
	.byte	35                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x2ad:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	14                              # DW_AT_low_pc
	.word	.Ltmp26-.Ltmp25                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	78                              # DW_AT_call_line
	.byte	23                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x2ba:0x42 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	15                              # DW_AT_low_pc
	.word	.Ltmp29-.Ltmp27                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	79                              # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x2c7:0x6 DW_TAG_formal_parameter
	.byte	8                               # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x2cd:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x2d4:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x2da:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	97
	.word	460                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x2e1:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x2e8:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	106
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x2ef:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x2f5:0x6 DW_TAG_variable
	.byte	9                               # DW_AT_location
	.word	492                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x2fc:0x42 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	16                              # DW_AT_low_pc
	.word	.Ltmp31-.Ltmp29                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	80                              # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x309:0x6 DW_TAG_formal_parameter
	.byte	10                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x30f:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x316:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x31c:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	95
	.word	460                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x323:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x32a:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	96
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x331:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x337:0x6 DW_TAG_variable
	.byte	11                              # DW_AT_location
	.word	492                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x33e:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	17                              # DW_AT_low_pc
	.word	.Ltmp32-.Ltmp31                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	81                              # DW_AT_call_line
	.byte	23                              # DW_AT_call_column
	.byte	32                              # Abbrev [32] 0x34b:0x214 DW_TAG_lexical_block
	.byte	1                               # DW_AT_ranges
	.byte	21                              # Abbrev [21] 0x34d:0x9 DW_TAG_variable
	.byte	13                              # DW_AT_location
	.byte	64                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	85                              # DW_AT_decl_line
	.word	1547                            # DW_AT_type
	.byte	32                              # Abbrev [32] 0x356:0x208 DW_TAG_lexical_block
	.byte	2                               # DW_AT_ranges
	.byte	11                              # Abbrev [11] 0x358:0x8 DW_TAG_variable
	.byte	65                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	86                              # DW_AT_decl_line
	.word	1547                            # DW_AT_type
	.byte	27                              # Abbrev [27] 0x360:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	18                              # DW_AT_low_pc
	.word	.Ltmp43-.Ltmp42                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	89                              # DW_AT_call_line
	.byte	25                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x36d:0xd DW_TAG_inlined_subroutine
	.word	531                             # DW_AT_abstract_origin
	.byte	19                              # DW_AT_low_pc
	.word	.Ltmp46-.Ltmp44                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	90                              # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x37a:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	20                              # DW_AT_low_pc
	.word	.Ltmp47-.Ltmp46                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	91                              # DW_AT_call_line
	.byte	25                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x387:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	21                              # DW_AT_low_pc
	.word	.Ltmp54-.Ltmp48                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	93                              # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x394:0x6 DW_TAG_variable
	.byte	14                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x39a:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	21                              # DW_AT_low_pc
	.word	.Ltmp52-.Ltmp48                 # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x3a7:0xc DW_TAG_lexical_block
	.byte	22                              # DW_AT_low_pc
	.word	.Ltmp52-.Ltmp51                 # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x3ad:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x3b5:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	23                              # DW_AT_low_pc
	.word	.Ltmp55-.Ltmp54                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	95                              # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x3c2:0x43 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	24                              # DW_AT_low_pc
	.word	.Ltmp59-.Ltmp58                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	97                              # DW_AT_call_line
	.byte	17                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x3cf:0x6 DW_TAG_formal_parameter
	.byte	15                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x3d5:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x3dc:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x3e2:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	492                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x3e9:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x3f0:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	93
	.word	460                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x3f7:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x3fe:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x405:0x43 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	25                              # DW_AT_low_pc
	.word	.Ltmp63-.Ltmp62                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	98                              # DW_AT_call_line
	.byte	17                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x412:0x6 DW_TAG_formal_parameter
	.byte	16                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x418:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x41f:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x425:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	492                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x42c:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x433:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	93
	.word	460                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x43a:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x441:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x448:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	26                              # DW_AT_low_pc
	.word	.Ltmp64-.Ltmp63                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	100                             # DW_AT_call_line
	.byte	26                              # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x455:0x54 DW_TAG_lexical_block
	.byte	27                              # DW_AT_low_pc
	.word	.Ltmp79-.Ltmp66                 # DW_AT_high_pc
	.byte	21                              # Abbrev [21] 0x45b:0x9 DW_TAG_variable
	.byte	18                              # DW_AT_location
	.byte	66                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	110                             # DW_AT_decl_line
	.word	1674                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x464:0x9 DW_TAG_variable
	.byte	19                              # DW_AT_location
	.byte	67                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	111                             # DW_AT_decl_line
	.word	1674                            # DW_AT_type
	.byte	24                              # Abbrev [24] 0x46d:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	27                              # DW_AT_low_pc
	.word	.Ltmp72-.Ltmp66                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	103                             # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x47a:0x6 DW_TAG_variable
	.byte	17                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x480:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	27                              # DW_AT_low_pc
	.word	.Ltmp70-.Ltmp66                 # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x48d:0xc DW_TAG_lexical_block
	.byte	28                              # DW_AT_low_pc
	.word	.Ltmp70-.Ltmp69                 # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x493:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x49b:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	29                              # DW_AT_low_pc
	.word	.Ltmp74-.Ltmp73                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	105                             # DW_AT_call_line
	.byte	47                              # DW_AT_call_column
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x4a9:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	30                              # DW_AT_low_pc
	.word	.Ltmp85-.Ltmp79                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	127                             # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x4b6:0x6 DW_TAG_variable
	.byte	20                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x4bc:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	30                              # DW_AT_low_pc
	.word	.Ltmp83-.Ltmp79                 # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x4c9:0xc DW_TAG_lexical_block
	.byte	31                              # DW_AT_low_pc
	.word	.Ltmp83-.Ltmp82                 # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x4cf:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x4d7:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	32                              # DW_AT_low_pc
	.word	.Ltmp88-.Ltmp87                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	129                             # DW_AT_call_line
	.byte	43                              # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x4e4:0x79 DW_TAG_lexical_block
	.byte	33                              # DW_AT_low_pc
	.word	.Ltmp101-.Ltmp90                # DW_AT_high_pc
	.byte	11                              # Abbrev [11] 0x4ea:0x8 DW_TAG_variable
	.byte	68                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	132                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	27                              # Abbrev [27] 0x4f2:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	33                              # DW_AT_low_pc
	.word	.Ltmp91-.Ltmp90                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	132                             # DW_AT_call_line
	.byte	38                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x4ff:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	34                              # DW_AT_low_pc
	.word	.Ltmp93-.Ltmp92                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	134                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x50c:0x43 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	35                              # DW_AT_low_pc
	.word	.Ltmp95-.Ltmp94                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	135                             # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x519:0x6 DW_TAG_formal_parameter
	.byte	21                              # DW_AT_location
	.word	428                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x51f:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x526:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x52c:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	484                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x533:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	492                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x53a:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x541:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	460                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x547:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	476                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x54f:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	36                              # DW_AT_low_pc
	.word	.Ltmp96-.Ltmp95                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	136                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x55f:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	37                              # DW_AT_low_pc
	.word	.Ltmp103-.Ltmp102               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	144                             # DW_AT_call_line
	.byte	21                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x56c:0xd DW_TAG_inlined_subroutine
	.word	531                             # DW_AT_abstract_origin
	.byte	38                              # DW_AT_low_pc
	.word	.Ltmp106-.Ltmp104               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	145                             # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x579:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	39                              # DW_AT_low_pc
	.word	.Ltmp107-.Ltmp106               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	146                             # DW_AT_call_line
	.byte	21                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x586:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	40                              # DW_AT_low_pc
	.word	.Ltmp114-.Ltmp108               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	149                             # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x593:0x6 DW_TAG_variable
	.byte	22                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x599:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	40                              # DW_AT_low_pc
	.word	.Ltmp112-.Ltmp108               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x5a6:0xc DW_TAG_lexical_block
	.byte	41                              # DW_AT_low_pc
	.word	.Ltmp112-.Ltmp111               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x5ac:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x5b4:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	42                              # DW_AT_low_pc
	.word	.Ltmp116-.Ltmp115               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	152                             # DW_AT_call_line
	.byte	20                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x5c1:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	43                              # DW_AT_low_pc
	.word	.Ltmp124-.Ltmp118               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	156                             # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x5ce:0x6 DW_TAG_variable
	.byte	23                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x5d4:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	43                              # DW_AT_low_pc
	.word	.Ltmp122-.Ltmp118               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x5e1:0xc DW_TAG_lexical_block
	.byte	44                              # DW_AT_low_pc
	.word	.Ltmp122-.Ltmp121               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x5e7:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	33                              # Abbrev [33] 0x5ef:0x6 DW_TAG_call_site
	.word	1532                            # DW_AT_call_origin
                                        # DW_AT_call_tail_call
	.byte	45                              # DW_AT_call_pc
	.byte	34                              # Abbrev [34] 0x5f5:0x6 DW_TAG_call_site
	.word	1532                            # DW_AT_call_origin
	.byte	43                              # DW_AT_call_return_pc
	.byte	0                               # End Of Children Mark
	.byte	35                              # Abbrev [35] 0x5fc:0xf DW_TAG_subprogram
	.byte	53                              # DW_AT_name
	.byte	6                               # DW_AT_decl_file
	.byte	200                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	36                              # Abbrev [36] 0x604:0x5 DW_TAG_formal_parameter
	.word	1551                            # DW_AT_type
	.byte	37                              # Abbrev [37] 0x609:0x1 DW_TAG_unspecified_parameters
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x60b:0x4 DW_TAG_base_type
	.byte	54                              # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x60f:0x5 DW_TAG_pointer_type
	.word	1556                            # DW_AT_type
	.byte	17                              # Abbrev [17] 0x614:0x5 DW_TAG_const_type
	.word	1561                            # DW_AT_type
	.byte	5                               # Abbrev [5] 0x619:0x4 DW_TAG_base_type
	.byte	55                              # DW_AT_name
	.byte	8                               # DW_AT_encoding
	.byte	1                               # DW_AT_byte_size
	.byte	38                              # Abbrev [38] 0x61d:0x6d DW_TAG_subprogram
	.byte	56                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_external
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0x621:0x8 DW_TAG_formal_parameter
	.byte	57                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x629:0x8 DW_TAG_formal_parameter
	.byte	58                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x631:0x8 DW_TAG_formal_parameter
	.byte	59                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	78                              # DW_AT_type
	.byte	15                              # Abbrev [15] 0x639:0x8 DW_TAG_formal_parameter
	.byte	60                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	54                              # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x641:0x8 DW_TAG_variable
	.byte	61                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	58                              # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x649:0x8 DW_TAG_variable
	.byte	62                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	56                              # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x651:0x8 DW_TAG_variable
	.byte	63                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	84                              # DW_AT_decl_line
	.word	1547                            # DW_AT_type
	.byte	13                              # Abbrev [13] 0x659:0x30 DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x65a:0x8 DW_TAG_variable
	.byte	64                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	85                              # DW_AT_decl_line
	.word	1547                            # DW_AT_type
	.byte	13                              # Abbrev [13] 0x662:0x26 DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x663:0x8 DW_TAG_variable
	.byte	65                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	86                              # DW_AT_decl_line
	.word	1547                            # DW_AT_type
	.byte	13                              # Abbrev [13] 0x66b:0x12 DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x66c:0x8 DW_TAG_variable
	.byte	66                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	110                             # DW_AT_decl_line
	.word	1674                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0x674:0x8 DW_TAG_variable
	.byte	67                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	111                             # DW_AT_decl_line
	.word	1674                            # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	13                              # Abbrev [13] 0x67d:0xa DW_TAG_lexical_block
	.byte	11                              # Abbrev [11] 0x67e:0x8 DW_TAG_variable
	.byte	68                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	132                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x68a:0x5 DW_TAG_pointer_type
	.word	78                              # DW_AT_type
	.byte	39                              # Abbrev [39] 0x68f:0x25 DW_TAG_subprogram
	.byte	69                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	20                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0x693:0x8 DW_TAG_formal_parameter
	.byte	57                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	1716                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0x69b:0x8 DW_TAG_formal_parameter
	.byte	58                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	1716                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0x6a3:0x8 DW_TAG_formal_parameter
	.byte	59                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	1674                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0x6ab:0x8 DW_TAG_formal_parameter
	.byte	60                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	235                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x6b4:0x5 DW_TAG_pointer_type
	.word	210                             # DW_AT_type
	.byte	40                              # Abbrev [40] 0x6b9:0x3ec DW_TAG_subprogram
	.byte	46                              # DW_AT_low_pc
	.word	.Lfunc_end1-.Lfunc_begin1       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	85                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	19                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	41                              # Abbrev [41] 0x6c4:0x7 DW_TAG_formal_parameter
	.byte	24                              # DW_AT_location
	.byte	57                              # DW_AT_name
	.word	1716                            # DW_AT_type
                                        # DW_AT_artificial
	.byte	41                              # Abbrev [41] 0x6cb:0x7 DW_TAG_formal_parameter
	.byte	25                              # DW_AT_location
	.byte	58                              # DW_AT_name
	.word	1716                            # DW_AT_type
                                        # DW_AT_artificial
	.byte	41                              # Abbrev [41] 0x6d2:0x7 DW_TAG_formal_parameter
	.byte	26                              # DW_AT_location
	.byte	59                              # DW_AT_name
	.word	1674                            # DW_AT_type
                                        # DW_AT_artificial
	.byte	41                              # Abbrev [41] 0x6d9:0x7 DW_TAG_formal_parameter
	.byte	27                              # DW_AT_location
	.byte	60                              # DW_AT_name
	.word	235                             # DW_AT_type
                                        # DW_AT_artificial
	.byte	24                              # Abbrev [24] 0x6e0:0x3b8 DW_TAG_inlined_subroutine
	.word	1679                            # DW_AT_abstract_origin
	.byte	47                              # DW_AT_low_pc
	.word	.Ltmp253-.Ltmp128               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	19                              # DW_AT_call_line
	.byte	1                               # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x6ed:0x3aa DW_TAG_inlined_subroutine
	.word	1565                            # DW_AT_abstract_origin
	.byte	47                              # DW_AT_low_pc
	.word	.Ltmp253-.Ltmp128               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	26                              # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x6fa:0x6 DW_TAG_formal_parameter
	.byte	29                              # DW_AT_location
	.word	1569                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0x700:0x6 DW_TAG_formal_parameter
	.byte	28                              # DW_AT_location
	.word	1577                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0x706:0x6 DW_TAG_formal_parameter
	.byte	30                              # DW_AT_location
	.word	1585                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0x70c:0x6 DW_TAG_formal_parameter
	.byte	31                              # DW_AT_location
	.word	1593                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x712:0x6 DW_TAG_variable
	.byte	33                              # DW_AT_location
	.word	1601                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x718:0x6 DW_TAG_variable
	.byte	34                              # DW_AT_location
	.word	1609                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x71e:0x6 DW_TAG_variable
	.byte	40                              # DW_AT_location
	.word	1617                            # DW_AT_abstract_origin
	.byte	22                              # Abbrev [22] 0x724:0x10 DW_TAG_inlined_subroutine
	.word	326                             # DW_AT_abstract_origin
	.byte	3                               # DW_AT_ranges
	.byte	1                               # DW_AT_call_file
	.byte	56                              # DW_AT_call_line
	.byte	31                              # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x72d:0x6 DW_TAG_variable
	.byte	32                              # DW_AT_location
	.word	334                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x734:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	48                              # DW_AT_low_pc
	.word	.Ltmp151-.Ltmp145               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	73                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x741:0x6 DW_TAG_variable
	.byte	35                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x747:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	48                              # DW_AT_low_pc
	.word	.Ltmp149-.Ltmp145               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x754:0xc DW_TAG_lexical_block
	.byte	49                              # DW_AT_low_pc
	.word	.Ltmp149-.Ltmp148               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x75a:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x762:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	50                              # DW_AT_low_pc
	.word	.Ltmp153-.Ltmp152               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	75                              # DW_AT_call_line
	.byte	35                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x76f:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	51                              # DW_AT_low_pc
	.word	.Ltmp155-.Ltmp154               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	78                              # DW_AT_call_line
	.byte	23                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x77c:0x42 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	52                              # DW_AT_low_pc
	.word	.Ltmp159-.Ltmp156               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	79                              # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x789:0x6 DW_TAG_formal_parameter
	.byte	36                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x78f:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x796:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x79c:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	106
	.word	460                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x7a3:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x7aa:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	96
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x7b1:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x7b7:0x6 DW_TAG_variable
	.byte	37                              # DW_AT_location
	.word	492                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x7be:0x42 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	53                              # DW_AT_low_pc
	.word	.Ltmp161-.Ltmp159               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	80                              # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x7cb:0x6 DW_TAG_formal_parameter
	.byte	38                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x7d1:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x7d8:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x7de:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	97
	.word	460                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x7e5:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x7ec:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	86
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x7f3:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x7f9:0x6 DW_TAG_variable
	.byte	39                              # DW_AT_location
	.word	492                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x800:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	54                              # DW_AT_low_pc
	.word	.Ltmp162-.Ltmp161               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	81                              # DW_AT_call_line
	.byte	23                              # DW_AT_call_column
	.byte	32                              # Abbrev [32] 0x80d:0x1f9 DW_TAG_lexical_block
	.byte	4                               # DW_AT_ranges
	.byte	23                              # Abbrev [23] 0x80f:0x6 DW_TAG_variable
	.byte	41                              # DW_AT_location
	.word	1626                            # DW_AT_abstract_origin
	.byte	32                              # Abbrev [32] 0x815:0x1f0 DW_TAG_lexical_block
	.byte	5                               # DW_AT_ranges
	.byte	26                              # Abbrev [26] 0x817:0x5 DW_TAG_variable
	.word	1635                            # DW_AT_abstract_origin
	.byte	27                              # Abbrev [27] 0x81c:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	55                              # DW_AT_low_pc
	.word	.Ltmp173-.Ltmp172               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	89                              # DW_AT_call_line
	.byte	25                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x829:0xd DW_TAG_inlined_subroutine
	.word	531                             # DW_AT_abstract_origin
	.byte	56                              # DW_AT_low_pc
	.word	.Ltmp176-.Ltmp174               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	90                              # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x836:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	57                              # DW_AT_low_pc
	.word	.Ltmp177-.Ltmp176               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	91                              # DW_AT_call_line
	.byte	25                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x843:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	58                              # DW_AT_low_pc
	.word	.Ltmp184-.Ltmp178               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	93                              # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x850:0x6 DW_TAG_variable
	.byte	42                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x856:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	58                              # DW_AT_low_pc
	.word	.Ltmp182-.Ltmp178               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x863:0xc DW_TAG_lexical_block
	.byte	59                              # DW_AT_low_pc
	.word	.Ltmp182-.Ltmp181               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x869:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x871:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	60                              # DW_AT_low_pc
	.word	.Ltmp185-.Ltmp184               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	95                              # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x87e:0x43 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	61                              # DW_AT_low_pc
	.word	.Ltmp189-.Ltmp188               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	97                              # DW_AT_call_line
	.byte	17                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x88b:0x6 DW_TAG_formal_parameter
	.byte	43                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x891:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x898:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x89e:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	492                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x8a5:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x8ac:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	93
	.word	460                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x8b3:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x8ba:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x8c1:0x43 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	62                              # DW_AT_low_pc
	.word	.Ltmp193-.Ltmp192               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	98                              # DW_AT_call_line
	.byte	17                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x8ce:0x6 DW_TAG_formal_parameter
	.byte	44                              # DW_AT_location
	.word	436                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x8d4:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x8db:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x8e1:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	492                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x8e8:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x8ef:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	93
	.word	460                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x8f6:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	476                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x8fd:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	484                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x904:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	63                              # DW_AT_low_pc
	.word	.Ltmp194-.Ltmp193               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	100                             # DW_AT_call_line
	.byte	26                              # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x911:0x4e DW_TAG_lexical_block
	.byte	64                              # DW_AT_low_pc
	.word	.Ltmp209-.Ltmp196               # DW_AT_high_pc
	.byte	23                              # Abbrev [23] 0x917:0x6 DW_TAG_variable
	.byte	46                              # DW_AT_location
	.word	1644                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x91d:0x6 DW_TAG_variable
	.byte	47                              # DW_AT_location
	.word	1652                            # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x923:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	64                              # DW_AT_low_pc
	.word	.Ltmp202-.Ltmp196               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	103                             # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x930:0x6 DW_TAG_variable
	.byte	45                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x936:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	64                              # DW_AT_low_pc
	.word	.Ltmp200-.Ltmp196               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x943:0xc DW_TAG_lexical_block
	.byte	65                              # DW_AT_low_pc
	.word	.Ltmp200-.Ltmp199               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x949:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x951:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	66                              # DW_AT_low_pc
	.word	.Ltmp204-.Ltmp203               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	105                             # DW_AT_call_line
	.byte	47                              # DW_AT_call_column
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x95f:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	67                              # DW_AT_low_pc
	.word	.Ltmp215-.Ltmp209               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	127                             # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0x96c:0x6 DW_TAG_variable
	.byte	48                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0x972:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	67                              # DW_AT_low_pc
	.word	.Ltmp213-.Ltmp209               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0x97f:0xc DW_TAG_lexical_block
	.byte	68                              # DW_AT_low_pc
	.word	.Ltmp213-.Ltmp212               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0x985:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x98d:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	69                              # DW_AT_low_pc
	.word	.Ltmp218-.Ltmp217               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	129                             # DW_AT_call_line
	.byte	43                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x99a:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	70                              # DW_AT_low_pc
	.word	.Ltmp221-.Ltmp220               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	132                             # DW_AT_call_line
	.byte	38                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0x9a7:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	71                              # DW_AT_low_pc
	.word	.Ltmp223-.Ltmp222               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	134                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0x9b4:0x43 DW_TAG_inlined_subroutine
	.word	420                             # DW_AT_abstract_origin
	.byte	72                              # DW_AT_low_pc
	.word	.Ltmp225-.Ltmp224               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	135                             # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x9c1:0x6 DW_TAG_formal_parameter
	.byte	49                              # DW_AT_location
	.word	428                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x9c7:0x7 DW_TAG_formal_parameter
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	444                             # DW_AT_abstract_origin
	.byte	29                              # Abbrev [29] 0x9ce:0x6 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_const_value
	.word	452                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x9d4:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	484                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x9db:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	492                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x9e2:0x7 DW_TAG_variable
	.ascii	"\200\020"                      # DW_AT_const_value
	.word	468                             # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x9e9:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	460                             # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x9ef:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	476                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x9f7:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	73                              # DW_AT_low_pc
	.word	.Ltmp226-.Ltmp225               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	136                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0xa06:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	74                              # DW_AT_low_pc
	.word	.Ltmp233-.Ltmp232               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	144                             # DW_AT_call_line
	.byte	21                              # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0xa13:0xd DW_TAG_inlined_subroutine
	.word	531                             # DW_AT_abstract_origin
	.byte	75                              # DW_AT_low_pc
	.word	.Ltmp236-.Ltmp234               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	145                             # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	27                              # Abbrev [27] 0xa20:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	76                              # DW_AT_low_pc
	.word	.Ltmp237-.Ltmp236               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	146                             # DW_AT_call_line
	.byte	21                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0xa2d:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	77                              # DW_AT_low_pc
	.word	.Ltmp244-.Ltmp238               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	149                             # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0xa3a:0x6 DW_TAG_variable
	.byte	50                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0xa40:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	77                              # DW_AT_low_pc
	.word	.Ltmp242-.Ltmp238               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0xa4d:0xc DW_TAG_lexical_block
	.byte	78                              # DW_AT_low_pc
	.word	.Ltmp242-.Ltmp241               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0xa53:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0xa5b:0xd DW_TAG_inlined_subroutine
	.word	393                             # DW_AT_abstract_origin
	.byte	79                              # DW_AT_low_pc
	.word	.Ltmp246-.Ltmp245               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	152                             # DW_AT_call_line
	.byte	20                              # DW_AT_call_column
	.byte	24                              # Abbrev [24] 0xa68:0x2e DW_TAG_inlined_subroutine
	.word	380                             # DW_AT_abstract_origin
	.byte	80                              # DW_AT_low_pc
	.word	.Ltmp253-.Ltmp247               # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	156                             # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	23                              # Abbrev [23] 0xa75:0x6 DW_TAG_variable
	.byte	51                              # DW_AT_location
	.word	384                             # DW_AT_abstract_origin
	.byte	24                              # Abbrev [24] 0xa7b:0x1a DW_TAG_inlined_subroutine
	.word	351                             # DW_AT_abstract_origin
	.byte	80                              # DW_AT_low_pc
	.word	.Ltmp251-.Ltmp247               # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	72                              # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	25                              # Abbrev [25] 0xa88:0xc DW_TAG_lexical_block
	.byte	81                              # DW_AT_low_pc
	.word	.Ltmp251-.Ltmp250               # DW_AT_high_pc
	.byte	26                              # Abbrev [26] 0xa8e:0x5 DW_TAG_variable
	.word	366                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	33                              # Abbrev [33] 0xa98:0x6 DW_TAG_call_site
	.word	1532                            # DW_AT_call_origin
                                        # DW_AT_call_tail_call
	.byte	82                              # DW_AT_call_pc
	.byte	34                              # Abbrev [34] 0xa9e:0x6 DW_TAG_call_site
	.word	1532                            # DW_AT_call_origin
	.byte	80                              # DW_AT_call_return_pc
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xaa5:0x32 DW_TAG_subprogram
	.byte	83                              # DW_AT_low_pc
	.word	.Lfunc_end2-.Lfunc_begin2       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	2775                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xab1:0x6 DW_TAG_formal_parameter
	.byte	52                              # DW_AT_location
	.word	2783                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xab7:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2791                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xabe:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2799                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xac5:0x6 DW_TAG_variable
	.byte	53                              # DW_AT_location
	.word	2807                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xacb:0x6 DW_TAG_variable
	.byte	54                              # DW_AT_location
	.word	2823                            # DW_AT_abstract_origin
	.byte	26                              # Abbrev [26] 0xad1:0x5 DW_TAG_variable
	.word	2815                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xad7:0x39 DW_TAG_subprogram
	.byte	70                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xadf:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xae7:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2837                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xaef:0x8 DW_TAG_variable
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xaf7:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2842                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xaff:0x8 DW_TAG_variable
	.byte	75                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xb07:0x8 DW_TAG_variable
	.byte	76                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0xb10:0x5 DW_TAG_const_type
	.word	210                             # DW_AT_type
	.byte	17                              # Abbrev [17] 0xb15:0x5 DW_TAG_const_type
	.word	235                             # DW_AT_type
	.byte	17                              # Abbrev [17] 0xb1a:0x5 DW_TAG_const_type
	.word	230                             # DW_AT_type
	.byte	42                              # Abbrev [42] 0xb1f:0x41 DW_TAG_subprogram
	.byte	84                              # DW_AT_low_pc
	.word	.Lfunc_end3-.Lfunc_begin3       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	3828                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xb2b:0x6 DW_TAG_formal_parameter
	.byte	55                              # DW_AT_location
	.word	3836                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xb31:0x6 DW_TAG_variable
	.byte	57                              # DW_AT_location
	.word	3844                            # DW_AT_abstract_origin
	.byte	26                              # Abbrev [26] 0xb37:0x5 DW_TAG_variable
	.word	3852                            # DW_AT_abstract_origin
	.byte	44                              # Abbrev [44] 0xb3c:0x23 DW_TAG_inlined_subroutine
	.word	2775                            # DW_AT_abstract_origin
	.byte	84                              # DW_AT_low_pc
	.word	.Ltmp258-.Lfunc_begin3          # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	709                             # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xb4a:0x6 DW_TAG_formal_parameter
	.byte	56                              # DW_AT_location
	.word	2783                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xb50:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2799                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xb57:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2807                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xb60:0x2d DW_TAG_subprogram
	.byte	85                              # DW_AT_low_pc
	.word	.Lfunc_end4-.Lfunc_begin4       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	2957                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xb6c:0x6 DW_TAG_formal_parameter
	.byte	58                              # DW_AT_location
	.word	2965                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xb72:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2973                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xb79:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2981                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xb80:0x6 DW_TAG_variable
	.byte	59                              # DW_AT_location
	.word	2989                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xb86:0x6 DW_TAG_variable
	.byte	60                              # DW_AT_location
	.word	2997                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xb8d:0x31 DW_TAG_subprogram
	.byte	77                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xb95:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xb9d:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xba5:0x8 DW_TAG_variable
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xbad:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2842                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xbb5:0x8 DW_TAG_variable
	.byte	75                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xbbe:0x4a DW_TAG_subprogram
	.byte	86                              # DW_AT_low_pc
	.word	.Lfunc_end5-.Lfunc_begin5       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	4034                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xbca:0x6 DW_TAG_formal_parameter
	.byte	61                              # DW_AT_location
	.word	4038                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xbd0:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	4046                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xbd7:0x6 DW_TAG_variable
	.byte	63                              # DW_AT_location
	.word	4054                            # DW_AT_abstract_origin
	.byte	44                              # Abbrev [44] 0xbdd:0x2a DW_TAG_inlined_subroutine
	.word	2957                            # DW_AT_abstract_origin
	.byte	86                              # DW_AT_low_pc
	.word	.Ltmp264-.Lfunc_begin5          # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	1456                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xbeb:0x6 DW_TAG_formal_parameter
	.byte	62                              # DW_AT_location
	.word	2965                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xbf1:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2973                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xbf8:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2981                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xbff:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2989                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xc08:0x31 DW_TAG_subprogram
	.byte	87                              # DW_AT_low_pc
	.word	.Lfunc_end6-.Lfunc_begin6       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	3129                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xc14:0x6 DW_TAG_formal_parameter
	.byte	64                              # DW_AT_location
	.word	3137                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xc1a:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	3145                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xc21:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3153                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xc28:0x6 DW_TAG_variable
	.byte	65                              # DW_AT_location
	.word	3161                            # DW_AT_abstract_origin
	.byte	26                              # Abbrev [26] 0xc2e:0x5 DW_TAG_variable
	.word	3169                            # DW_AT_abstract_origin
	.byte	26                              # Abbrev [26] 0xc33:0x5 DW_TAG_variable
	.word	3177                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xc39:0x39 DW_TAG_subprogram
	.byte	78                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xc41:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xc49:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3186                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc51:0x8 DW_TAG_variable
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc59:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3191                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc61:0x8 DW_TAG_variable
	.byte	75                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc69:0x8 DW_TAG_variable
	.byte	76                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	250                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0xc72:0x5 DW_TAG_const_type
	.word	270                             # DW_AT_type
	.byte	17                              # Abbrev [17] 0xc77:0x5 DW_TAG_const_type
	.word	240                             # DW_AT_type
	.byte	45                              # Abbrev [45] 0xc7c:0x4d DW_TAG_subprogram
	.byte	88                              # DW_AT_low_pc
	.word	.Lfunc_end7-.Lfunc_begin7       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	86                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	250                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0xc8b:0x9 DW_TAG_formal_parameter
	.byte	66                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0xc94:0x9 DW_TAG_variable
	.byte	68                              # DW_AT_location
	.byte	83                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc9d:0x8 DW_TAG_variable
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	250                             # DW_AT_type
	.byte	44                              # Abbrev [44] 0xca5:0x23 DW_TAG_inlined_subroutine
	.word	3129                            # DW_AT_abstract_origin
	.byte	88                              # DW_AT_low_pc
	.word	.Ltmp269-.Lfunc_begin7          # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	2208                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xcb3:0x6 DW_TAG_formal_parameter
	.byte	67                              # DW_AT_location
	.word	3137                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xcb9:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3153                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xcc0:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	3161                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xcc9:0x2d DW_TAG_subprogram
	.byte	89                              # DW_AT_low_pc
	.word	.Lfunc_end8-.Lfunc_begin8       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	3318                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xcd5:0x6 DW_TAG_formal_parameter
	.byte	69                              # DW_AT_location
	.word	3326                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xcdb:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	3334                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xce2:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3342                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xce9:0x6 DW_TAG_variable
	.byte	70                              # DW_AT_location
	.word	3350                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xcef:0x6 DW_TAG_variable
	.byte	71                              # DW_AT_location
	.word	3358                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xcf6:0x31 DW_TAG_subprogram
	.byte	79                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xcfe:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xd06:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3367                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xd0e:0x8 DW_TAG_variable
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xd16:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3191                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xd1e:0x8 DW_TAG_variable
	.byte	75                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0xd27:0x5 DW_TAG_const_type
	.word	250                             # DW_AT_type
	.byte	40                              # Abbrev [40] 0xd2c:0x52 DW_TAG_subprogram
	.byte	90                              # DW_AT_low_pc
	.word	.Lfunc_end9-.Lfunc_begin9       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	87                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	20                              # Abbrev [20] 0xd37:0x9 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	46                              # Abbrev [46] 0xd40:0xa DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3367                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0xd4a:0x9 DW_TAG_variable
	.byte	74                              # DW_AT_location
	.byte	83                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	44                              # Abbrev [44] 0xd53:0x2a DW_TAG_inlined_subroutine
	.word	3318                            # DW_AT_abstract_origin
	.byte	90                              # DW_AT_low_pc
	.word	.Ltmp275-.Lfunc_begin9          # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	2955                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xd61:0x6 DW_TAG_formal_parameter
	.byte	73                              # DW_AT_location
	.word	3326                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xd67:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	3334                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xd6e:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3342                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xd75:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	3350                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xd7e:0x31 DW_TAG_subprogram
	.byte	91                              # DW_AT_low_pc
	.word	.Lfunc_end10-.Lfunc_begin10     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	3503                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xd8a:0x6 DW_TAG_formal_parameter
	.byte	75                              # DW_AT_location
	.word	3511                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xd90:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	3519                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xd97:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3527                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xd9e:0x6 DW_TAG_variable
	.byte	76                              # DW_AT_location
	.word	3535                            # DW_AT_abstract_origin
	.byte	26                              # Abbrev [26] 0xda4:0x5 DW_TAG_variable
	.word	3543                            # DW_AT_abstract_origin
	.byte	26                              # Abbrev [26] 0xda9:0x5 DW_TAG_variable
	.word	3551                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xdaf:0x39 DW_TAG_subprogram
	.byte	80                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xdb7:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xdbf:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3560                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xdc7:0x8 DW_TAG_variable
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xdcf:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3565                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xdd7:0x8 DW_TAG_variable
	.byte	75                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xddf:0x8 DW_TAG_variable
	.byte	76                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	285                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0xde8:0x5 DW_TAG_const_type
	.word	305                             # DW_AT_type
	.byte	17                              # Abbrev [17] 0xded:0x5 DW_TAG_const_type
	.word	275                             # DW_AT_type
	.byte	45                              # Abbrev [45] 0xdf2:0x4d DW_TAG_subprogram
	.byte	92                              # DW_AT_low_pc
	.word	.Lfunc_end11-.Lfunc_begin11     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	88                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	285                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0xe01:0x9 DW_TAG_formal_parameter
	.byte	77                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0xe0a:0x9 DW_TAG_variable
	.byte	79                              # DW_AT_location
	.byte	83                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xe13:0x8 DW_TAG_variable
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	285                             # DW_AT_type
	.byte	44                              # Abbrev [44] 0xe1b:0x23 DW_TAG_inlined_subroutine
	.word	3503                            # DW_AT_abstract_origin
	.byte	92                              # DW_AT_low_pc
	.word	.Ltmp280-.Lfunc_begin11         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	3699                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xe29:0x6 DW_TAG_formal_parameter
	.byte	78                              # DW_AT_location
	.word	3511                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xe2f:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3527                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xe36:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	3535                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	42                              # Abbrev [42] 0xe3f:0x2d DW_TAG_subprogram
	.byte	93                              # DW_AT_low_pc
	.word	.Lfunc_end12-.Lfunc_begin12     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.word	3692                            # DW_AT_abstract_origin
	.byte	28                              # Abbrev [28] 0xe4b:0x6 DW_TAG_formal_parameter
	.byte	80                              # DW_AT_location
	.word	3700                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xe51:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	3708                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xe58:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3716                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xe5f:0x6 DW_TAG_variable
	.byte	81                              # DW_AT_location
	.word	3724                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0xe65:0x6 DW_TAG_variable
	.byte	82                              # DW_AT_location
	.word	3732                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xe6c:0x31 DW_TAG_subprogram
	.byte	81                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xe74:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xe7c:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3741                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xe84:0x8 DW_TAG_variable
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xe8c:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3565                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xe94:0x8 DW_TAG_variable
	.byte	75                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0xe9d:0x5 DW_TAG_const_type
	.word	285                             # DW_AT_type
	.byte	40                              # Abbrev [40] 0xea2:0x52 DW_TAG_subprogram
	.byte	94                              # DW_AT_low_pc
	.word	.Lfunc_end13-.Lfunc_begin13     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	89                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	20                              # Abbrev [20] 0xead:0x9 DW_TAG_formal_parameter
	.byte	83                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	46                              # Abbrev [46] 0xeb6:0xa DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3741                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0xec0:0x9 DW_TAG_variable
	.byte	85                              # DW_AT_location
	.byte	83                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	44                              # Abbrev [44] 0xec9:0x2a DW_TAG_inlined_subroutine
	.word	3692                            # DW_AT_abstract_origin
	.byte	94                              # DW_AT_low_pc
	.word	.Ltmp286-.Lfunc_begin13         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	4438                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xed7:0x6 DW_TAG_formal_parameter
	.byte	84                              # DW_AT_location
	.word	3700                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0xedd:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	3708                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xee4:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	3716                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xeeb:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	3724                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0xef4:0x21 DW_TAG_subprogram
	.byte	82                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	113                             # DW_AT_type
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xefc:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xf04:0x8 DW_TAG_variable
	.byte	83                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xf0c:0x8 DW_TAG_variable
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	113                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0xf15:0x5 DW_TAG_const_type
	.word	1547                            # DW_AT_type
	.byte	45                              # Abbrev [45] 0xf1a:0xa8 DW_TAG_subprogram
	.byte	95                              # DW_AT_low_pc
	.word	.Lfunc_end14-.Lfunc_begin14     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	90                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	166                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	210                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0xf29:0x9 DW_TAG_formal_parameter
	.byte	86                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	166                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0xf32:0x9 DW_TAG_variable
	.byte	89                              # DW_AT_location
	.byte	94                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	170                             # DW_AT_decl_line
	.word	210                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0xf3b:0x9 DW_TAG_variable
	.byte	90                              # DW_AT_location
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	168                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0xf44:0x9 DW_TAG_variable
	.byte	93                              # DW_AT_location
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	169                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	24                              # Abbrev [24] 0xf4d:0x37 DW_TAG_inlined_subroutine
	.word	3828                            # DW_AT_abstract_origin
	.byte	95                              # DW_AT_low_pc
	.word	.Ltmp288-.Lfunc_begin14         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	168                             # DW_AT_call_line
	.byte	28                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xf5a:0x6 DW_TAG_formal_parameter
	.byte	87                              # DW_AT_location
	.word	3836                            # DW_AT_abstract_origin
	.byte	44                              # Abbrev [44] 0xf60:0x23 DW_TAG_inlined_subroutine
	.word	2775                            # DW_AT_abstract_origin
	.byte	95                              # DW_AT_low_pc
	.word	.Ltmp288-.Lfunc_begin14         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	709                             # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xf6e:0x6 DW_TAG_formal_parameter
	.byte	88                              # DW_AT_location
	.word	2783                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xf74:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2799                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xf7b:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2807                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0xf84:0x3d DW_TAG_inlined_subroutine
	.word	3828                            # DW_AT_abstract_origin
	.byte	96                              # DW_AT_low_pc
	.word	.Ltmp292-.Ltmp291               # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	169                             # DW_AT_call_line
	.byte	28                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xf91:0x6 DW_TAG_formal_parameter
	.byte	92                              # DW_AT_location
	.word	3836                            # DW_AT_abstract_origin
	.byte	47                              # Abbrev [47] 0xf97:0x6 DW_TAG_variable
	.byte	0                               # DW_AT_const_value
	.word	3844                            # DW_AT_abstract_origin
	.byte	44                              # Abbrev [44] 0xf9d:0x23 DW_TAG_inlined_subroutine
	.word	2775                            # DW_AT_abstract_origin
	.byte	96                              # DW_AT_low_pc
	.word	.Ltmp292-.Ltmp291               # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	709                             # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0xfab:0x6 DW_TAG_formal_parameter
	.byte	91                              # DW_AT_location
	.word	2783                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xfb1:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	93
	.word	2807                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0xfb8:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2799                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	39                              # Abbrev [39] 0xfc2:0x1d DW_TAG_subprogram
	.byte	84                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_inline
	.byte	15                              # Abbrev [15] 0xfc6:0x8 DW_TAG_formal_parameter
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xfce:0x8 DW_TAG_formal_parameter
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xfd6:0x8 DW_TAG_variable
	.byte	83                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	164                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	40                              # Abbrev [40] 0xfdf:0xba DW_TAG_subprogram
	.byte	97                              # DW_AT_low_pc
	.word	.Lfunc_end15-.Lfunc_begin15     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	91                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	175                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	20                              # Abbrev [20] 0xfea:0x9 DW_TAG_formal_parameter
	.byte	95                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	175                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	20                              # Abbrev [20] 0xff3:0x9 DW_TAG_formal_parameter
	.byte	94                              # DW_AT_location
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	175                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0xffc:0x9 DW_TAG_variable
	.byte	98                              # DW_AT_location
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	177                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x1005:0x9 DW_TAG_variable
	.byte	99                              # DW_AT_location
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	179                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	24                              # Abbrev [24] 0x100e:0x45 DW_TAG_inlined_subroutine
	.word	4034                            # DW_AT_abstract_origin
	.byte	97                              # DW_AT_low_pc
	.word	.Ltmp295-.Lfunc_begin15         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	178                             # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x101b:0x6 DW_TAG_formal_parameter
	.byte	96                              # DW_AT_location
	.word	4038                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x1021:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	4046                            # DW_AT_abstract_origin
	.byte	44                              # Abbrev [44] 0x1028:0x2a DW_TAG_inlined_subroutine
	.word	2957                            # DW_AT_abstract_origin
	.byte	97                              # DW_AT_low_pc
	.word	.Ltmp295-.Lfunc_begin15         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	1456                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x1036:0x6 DW_TAG_formal_parameter
	.byte	97                              # DW_AT_location
	.word	2965                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x103c:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2973                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x1043:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2981                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x104a:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2989                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x1053:0x45 DW_TAG_inlined_subroutine
	.word	4034                            # DW_AT_abstract_origin
	.byte	98                              # DW_AT_low_pc
	.word	.Ltmp299-.Ltmp298               # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	180                             # DW_AT_call_line
	.byte	5                               # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x1060:0x6 DW_TAG_formal_parameter
	.byte	101                             # DW_AT_location
	.word	4038                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x1066:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	93
	.word	4046                            # DW_AT_abstract_origin
	.byte	44                              # Abbrev [44] 0x106d:0x2a DW_TAG_inlined_subroutine
	.word	2957                            # DW_AT_abstract_origin
	.byte	98                              # DW_AT_low_pc
	.word	.Ltmp299-.Ltmp298               # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	164                             # DW_AT_call_line
	.half	1456                            # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x107b:0x6 DW_TAG_formal_parameter
	.byte	100                             # DW_AT_location
	.word	2965                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x1081:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	93
	.word	2973                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x1088:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2989                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x108f:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2981                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	45                              # Abbrev [45] 0x1099:0x9f DW_TAG_subprogram
	.byte	99                              # DW_AT_low_pc
	.word	.Lfunc_end16-.Lfunc_begin16     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	92                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	183                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
	.byte	20                              # Abbrev [20] 0x10a8:0x9 DW_TAG_formal_parameter
	.byte	102                             # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	183                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	46                              # Abbrev [46] 0x10b1:0xa DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	183                             # DW_AT_decl_line
	.word	4560                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x10bb:0x9 DW_TAG_variable
	.byte	105                             # DW_AT_location
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.word	2837                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x10c4:0x9 DW_TAG_variable
	.byte	106                             # DW_AT_location
	.byte	95                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	186                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x10cd:0x9 DW_TAG_variable
	.byte	109                             # DW_AT_location
	.byte	96                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0x10d6:0x8 DW_TAG_variable
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	185                             # DW_AT_decl_line
	.word	2837                            # DW_AT_type
	.byte	24                              # Abbrev [24] 0x10de:0x28 DW_TAG_inlined_subroutine
	.word	2775                            # DW_AT_abstract_origin
	.byte	99                              # DW_AT_low_pc
	.word	.Ltmp302-.Lfunc_begin16         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	186                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x10eb:0x6 DW_TAG_formal_parameter
	.byte	103                             # DW_AT_location
	.word	2783                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x10f1:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2799                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x10f8:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2807                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x10ff:0x6 DW_TAG_variable
	.byte	104                             # DW_AT_location
	.word	2823                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x1106:0x31 DW_TAG_inlined_subroutine
	.word	2775                            # DW_AT_abstract_origin
	.byte	100                             # DW_AT_low_pc
	.word	.Ltmp307-.Ltmp305               # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	188                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x1113:0x6 DW_TAG_formal_parameter
	.byte	107                             # DW_AT_location
	.word	2783                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x1119:0x9 DW_TAG_formal_parameter
	.byte	3                               # DW_AT_location
	.byte	124
	.byte	4
	.byte	159
	.word	2791                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x1122:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	93
	.word	2807                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x1129:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2799                            # DW_AT_abstract_origin
	.byte	23                              # Abbrev [23] 0x1130:0x6 DW_TAG_variable
	.byte	108                             # DW_AT_location
	.word	2823                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	45                              # Abbrev [45] 0x1138:0x98 DW_TAG_subprogram
	.byte	101                             # DW_AT_low_pc
	.word	.Lfunc_end17-.Lfunc_begin17     # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	93                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	1547                            # DW_AT_type
	.byte	20                              # Abbrev [20] 0x1147:0x9 DW_TAG_formal_parameter
	.byte	111                             # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	20                              # Abbrev [20] 0x1150:0x9 DW_TAG_formal_parameter
	.byte	110                             # DW_AT_location
	.byte	72                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.word	2832                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x1159:0x9 DW_TAG_variable
	.byte	113                             # DW_AT_location
	.byte	74                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	194                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x1162:0x9 DW_TAG_variable
	.byte	114                             # DW_AT_location
	.byte	73                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	196                             # DW_AT_decl_line
	.word	526                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x116b:0x9 DW_TAG_variable
	.byte	115                             # DW_AT_location
	.byte	95                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	195                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x1174:0x9 DW_TAG_variable
	.byte	117                             # DW_AT_location
	.byte	96                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	197                             # DW_AT_decl_line
	.word	3861                            # DW_AT_type
	.byte	24                              # Abbrev [24] 0x117d:0x29 DW_TAG_inlined_subroutine
	.word	2957                            # DW_AT_abstract_origin
	.byte	101                             # DW_AT_low_pc
	.word	.Ltmp310-.Lfunc_begin17         # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	195                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x118a:0x6 DW_TAG_formal_parameter
	.byte	112                             # DW_AT_location
	.word	2965                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x1190:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2973                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x1197:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	91
	.word	2981                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x119e:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2989                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	24                              # Abbrev [24] 0x11a6:0x29 DW_TAG_inlined_subroutine
	.word	2957                            # DW_AT_abstract_origin
	.byte	102                             # DW_AT_low_pc
	.word	.Ltmp314-.Ltmp313               # DW_AT_high_pc
	.byte	7                               # DW_AT_call_file
	.byte	197                             # DW_AT_call_line
	.byte	27                              # DW_AT_call_column
	.byte	28                              # Abbrev [28] 0x11b3:0x6 DW_TAG_formal_parameter
	.byte	116                             # DW_AT_location
	.word	2965                            # DW_AT_abstract_origin
	.byte	43                              # Abbrev [43] 0x11b9:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	93
	.word	2973                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x11c0:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	92
	.word	2989                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x11c7:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	90
	.word	2981                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x11d0:0x5 DW_TAG_const_type
	.word	1716                            # DW_AT_type
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_rnglists,"",@progbits
	.word	.Ldebug_list_header_end1-.Ldebug_list_header_start1 # Length
.Ldebug_list_header_start1:
	.half	5                               # Version
	.byte	4                               # Address size
	.byte	0                               # Segment selector size
	.word	6                               # Offset entry count
.Lrnglists_table_base0:
	.word	.Ldebug_ranges0-.Lrnglists_table_base0
	.word	.Ldebug_ranges1-.Lrnglists_table_base0
	.word	.Ldebug_ranges2-.Lrnglists_table_base0
	.word	.Ldebug_ranges3-.Lrnglists_table_base0
	.word	.Ldebug_ranges4-.Lrnglists_table_base0
	.word	.Ldebug_ranges5-.Lrnglists_table_base0
.Ldebug_ranges0:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp1-.Lfunc_begin0           #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp2-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp3-.Lfunc_begin0           #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges1:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp34-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp39-.Lfunc_begin0          #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp41-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp102-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges2:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp38-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp39-.Lfunc_begin0          #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp41-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp101-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges3:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp128-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp129-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp132-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp133-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges4:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp164-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp169-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp171-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp232-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges5:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp168-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp169-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp171-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp231-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_list_header_end1:
	.section	.debug_str_offsets,"",@progbits
	.word	392                             # Length of String Offsets Set
	.half	5
	.half	0
.Lstr_offsets_base0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)" # string offset=0
.Linfo_string1:
	.asciz	"axpy_snitch.c"                 # string offset=114
.Linfo_string2:
	.asciz	"/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" # string offset=128
.Linfo_string3:
	.asciz	"l1_buf_x"                      # string offset=243
.Linfo_string4:
	.asciz	"float"                         # string offset=252
.Linfo_string5:
	.asciz	"__ARRAY_SIZE_TYPE__"           # string offset=258
.Linfo_string6:
	.asciz	"l1_buf_y"                      # string offset=278
.Linfo_string7:
	.asciz	"dma_timer"                     # string offset=287
.Linfo_string8:
	.asciz	"unsigned int"                  # string offset=297
.Linfo_string9:
	.asciz	"__uint32_t"                    # string offset=310
.Linfo_string10:
	.asciz	"uint32_t"                      # string offset=321
.Linfo_string11:
	.asciz	"dma_time"                      # string offset=330
.Linfo_string12:
	.asciz	"all_timer"                     # string offset=339
.Linfo_string13:
	.asciz	"all_time"                      # string offset=349
.Linfo_string14:
	.asciz	"issue_timer"                   # string offset=358
.Linfo_string15:
	.asciz	"issue_time"                    # string offset=370
.Linfo_string16:
	.asciz	"compute_timer"                 # string offset=381
.Linfo_string17:
	.asciz	"compute_time"                  # string offset=395
.Linfo_string18:
	.asciz	"unsigned long long"            # string offset=408
.Linfo_string19:
	.asciz	"__uint64_t"                    # string offset=427
.Linfo_string20:
	.asciz	"uint64_t"                      # string offset=438
.Linfo_string21:
	.asciz	"unsigned short"                # string offset=447
.Linfo_string22:
	.asciz	"__uint16_t"                    # string offset=462
.Linfo_string23:
	.asciz	"uint16_t"                      # string offset=473
.Linfo_string24:
	.asciz	"unsigned char"                 # string offset=482
.Linfo_string25:
	.asciz	"__uint8_t"                     # string offset=496
.Linfo_string26:
	.asciz	"uint8_t"                       # string offset=506
.Linfo_string27:
	.asciz	"__uintptr_t"                   # string offset=514
.Linfo_string28:
	.asciz	"uintptr_t"                     # string offset=526
.Linfo_string29:
	.asciz	"pulp_get_core_id"              # string offset=536
.Linfo_string30:
	.asciz	"pulp_id_t"                     # string offset=553
.Linfo_string31:
	.asciz	"r"                             # string offset=563
.Linfo_string32:
	.asciz	"fpu_fence"                     # string offset=565
.Linfo_string33:
	.asciz	"__tmp"                         # string offset=575
.Linfo_string34:
	.asciz	"unsigned long"                 # string offset=581
.Linfo_string35:
	.asciz	"tmp"                           # string offset=595
.Linfo_string36:
	.asciz	"pulp_barrier"                  # string offset=599
.Linfo_string37:
	.asciz	"pulp_get_timer"                # string offset=612
.Linfo_string38:
	.asciz	"pulp_timer_t"                  # string offset=627
.Linfo_string39:
	.asciz	"__dma_start_1d_wideptr_base"   # string offset=640
.Linfo_string40:
	.asciz	"dma_txid_t"                    # string offset=668
.Linfo_string41:
	.asciz	"dst"                           # string offset=679
.Linfo_string42:
	.asciz	"src"                           # string offset=683
.Linfo_string43:
	.asciz	"size"                          # string offset=687
.Linfo_string44:
	.asciz	"size_t"                        # string offset=692
.Linfo_string45:
	.asciz	"__deserialize"                 # string offset=699
.Linfo_string46:
	.asciz	"reg_src_high"                  # string offset=713
.Linfo_string47:
	.asciz	"reg_size"                      # string offset=726
.Linfo_string48:
	.asciz	"reg_src_low"                   # string offset=735
.Linfo_string49:
	.asciz	"reg_dst_high"                  # string offset=747
.Linfo_string50:
	.asciz	"reg_dst_low"                   # string offset=760
.Linfo_string51:
	.asciz	"reg_txid"                      # string offset=772
.Linfo_string52:
	.asciz	"dma_wait_all"                  # string offset=781
.Linfo_string53:
	.asciz	"printf_"                       # string offset=794
.Linfo_string54:
	.asciz	"int"                           # string offset=802
.Linfo_string55:
	.asciz	"char"                          # string offset=806
.Linfo_string56:
	.asciz	"dev_axpy"                      # string offset=811
.Linfo_string57:
	.asciz	"x_phys"                        # string offset=820
.Linfo_string58:
	.asciz	"y_phys"                        # string offset=827
.Linfo_string59:
	.asciz	"alpha"                         # string offset=834
.Linfo_string60:
	.asciz	"n"                             # string offset=840
.Linfo_string61:
	.asciz	"issue_diff"                    # string offset=842
.Linfo_string62:
	.asciz	"core_idx"                      # string offset=853
.Linfo_string63:
	.asciz	"itr"                           # string offset=862
.Linfo_string64:
	.asciz	"i"                             # string offset=866
.Linfo_string65:
	.asciz	"rest"                          # string offset=868
.Linfo_string66:
	.asciz	"px"                            # string offset=873
.Linfo_string67:
	.asciz	"py"                            # string offset=876
.Linfo_string68:
	.asciz	"compute_diff"                  # string offset=879
.Linfo_string69:
	.asciz	"__omp_offloading_10303_7120698_axpy_l19_debug__" # string offset=892
.Linfo_string70:
	.asciz	"hero_load_uint32_noblock"      # string offset=940
.Linfo_string71:
	.asciz	"addr"                          # string offset=965
.Linfo_string72:
	.asciz	"val"                           # string offset=970
.Linfo_string73:
	.asciz	"upper"                         # string offset=974
.Linfo_string74:
	.asciz	"lower"                         # string offset=980
.Linfo_string75:
	.asciz	"mstatus"                       # string offset=986
.Linfo_string76:
	.asciz	"reg"                           # string offset=994
.Linfo_string77:
	.asciz	"hero_store_uint32_noblock"     # string offset=998
.Linfo_string78:
	.asciz	"hero_load_uint16_noblock"      # string offset=1024
.Linfo_string79:
	.asciz	"hero_store_uint16_noblock"     # string offset=1049
.Linfo_string80:
	.asciz	"hero_load_uint8_noblock"       # string offset=1075
.Linfo_string81:
	.asciz	"hero_store_uint8_noblock"      # string offset=1099
.Linfo_string82:
	.asciz	"hero_load_uint32"              # string offset=1124
.Linfo_string83:
	.asciz	"res"                           # string offset=1141
.Linfo_string84:
	.asciz	"hero_store_uint32"             # string offset=1145
.Linfo_string85:
	.asciz	"__omp_offloading_10303_7120698_axpy_l19" # string offset=1163
.Linfo_string86:
	.asciz	"hero_load_uint16"              # string offset=1203
.Linfo_string87:
	.asciz	"hero_store_uint16"             # string offset=1220
.Linfo_string88:
	.asciz	"hero_load_uint8"               # string offset=1238
.Linfo_string89:
	.asciz	"hero_store_uint8"              # string offset=1254
.Linfo_string90:
	.asciz	"hero_load_uint64"              # string offset=1271
.Linfo_string91:
	.asciz	"hero_store_uint64"             # string offset=1288
.Linfo_string92:
	.asciz	"hero_load_uint64_noblock"      # string offset=1306
.Linfo_string93:
	.asciz	"hero_store_uint64_noblock"     # string offset=1331
.Linfo_string94:
	.asciz	"ret_val"                       # string offset=1357
.Linfo_string95:
	.asciz	"res_lower"                     # string offset=1365
.Linfo_string96:
	.asciz	"res_upper"                     # string offset=1375
	.section	.debug_str_offsets,"",@progbits
	.word	.Linfo_string0
	.word	.Linfo_string1
	.word	.Linfo_string2
	.word	.Linfo_string3
	.word	.Linfo_string4
	.word	.Linfo_string5
	.word	.Linfo_string6
	.word	.Linfo_string7
	.word	.Linfo_string8
	.word	.Linfo_string9
	.word	.Linfo_string10
	.word	.Linfo_string11
	.word	.Linfo_string12
	.word	.Linfo_string13
	.word	.Linfo_string14
	.word	.Linfo_string15
	.word	.Linfo_string16
	.word	.Linfo_string17
	.word	.Linfo_string18
	.word	.Linfo_string19
	.word	.Linfo_string20
	.word	.Linfo_string21
	.word	.Linfo_string22
	.word	.Linfo_string23
	.word	.Linfo_string24
	.word	.Linfo_string25
	.word	.Linfo_string26
	.word	.Linfo_string27
	.word	.Linfo_string28
	.word	.Linfo_string29
	.word	.Linfo_string30
	.word	.Linfo_string31
	.word	.Linfo_string32
	.word	.Linfo_string33
	.word	.Linfo_string34
	.word	.Linfo_string35
	.word	.Linfo_string36
	.word	.Linfo_string37
	.word	.Linfo_string38
	.word	.Linfo_string39
	.word	.Linfo_string40
	.word	.Linfo_string41
	.word	.Linfo_string42
	.word	.Linfo_string43
	.word	.Linfo_string44
	.word	.Linfo_string45
	.word	.Linfo_string46
	.word	.Linfo_string47
	.word	.Linfo_string48
	.word	.Linfo_string49
	.word	.Linfo_string50
	.word	.Linfo_string51
	.word	.Linfo_string52
	.word	.Linfo_string53
	.word	.Linfo_string54
	.word	.Linfo_string55
	.word	.Linfo_string56
	.word	.Linfo_string57
	.word	.Linfo_string58
	.word	.Linfo_string59
	.word	.Linfo_string60
	.word	.Linfo_string61
	.word	.Linfo_string62
	.word	.Linfo_string63
	.word	.Linfo_string64
	.word	.Linfo_string65
	.word	.Linfo_string66
	.word	.Linfo_string67
	.word	.Linfo_string68
	.word	.Linfo_string69
	.word	.Linfo_string70
	.word	.Linfo_string71
	.word	.Linfo_string72
	.word	.Linfo_string73
	.word	.Linfo_string74
	.word	.Linfo_string75
	.word	.Linfo_string76
	.word	.Linfo_string77
	.word	.Linfo_string78
	.word	.Linfo_string79
	.word	.Linfo_string80
	.word	.Linfo_string81
	.word	.Linfo_string82
	.word	.Linfo_string83
	.word	.Linfo_string84
	.word	.Linfo_string85
	.word	.Linfo_string86
	.word	.Linfo_string87
	.word	.Linfo_string88
	.word	.Linfo_string89
	.word	.Linfo_string90
	.word	.Linfo_string91
	.word	.Linfo_string92
	.word	.Linfo_string93
	.word	.Linfo_string94
	.word	.Linfo_string95
	.word	.Linfo_string96
	.section	.debug_addr,"",@progbits
	.word	.Ldebug_addr_end0-.Ldebug_addr_start0 # Length of contribution
.Ldebug_addr_start0:
	.half	5                               # DWARF version number
	.byte	4                               # Address size
	.byte	0                               # Segment selector size
.Laddr_table_base0:
	.word	l1_buf_x
	.word	l1_buf_y
	.word	dma_timer
	.word	dma_time
	.word	all_timer
	.word	all_time
	.word	issue_timer
	.word	issue_time
	.word	compute_timer
	.word	compute_time
	.word	.Lfunc_begin0
	.word	.Ltmp16
	.word	.Ltmp19
	.word	.Ltmp23
	.word	.Ltmp25
	.word	.Ltmp27
	.word	.Ltmp29
	.word	.Ltmp31
	.word	.Ltmp42
	.word	.Ltmp44
	.word	.Ltmp46
	.word	.Ltmp48
	.word	.Ltmp51
	.word	.Ltmp54
	.word	.Ltmp58
	.word	.Ltmp62
	.word	.Ltmp63
	.word	.Ltmp66
	.word	.Ltmp69
	.word	.Ltmp73
	.word	.Ltmp79
	.word	.Ltmp82
	.word	.Ltmp87
	.word	.Ltmp90
	.word	.Ltmp92
	.word	.Ltmp94
	.word	.Ltmp95
	.word	.Ltmp102
	.word	.Ltmp104
	.word	.Ltmp106
	.word	.Ltmp108
	.word	.Ltmp111
	.word	.Ltmp115
	.word	.Ltmp118
	.word	.Ltmp121
	.word	.Ltmp12
	.word	.Lfunc_begin1
	.word	.Ltmp128
	.word	.Ltmp145
	.word	.Ltmp148
	.word	.Ltmp152
	.word	.Ltmp154
	.word	.Ltmp156
	.word	.Ltmp159
	.word	.Ltmp161
	.word	.Ltmp172
	.word	.Ltmp174
	.word	.Ltmp176
	.word	.Ltmp178
	.word	.Ltmp181
	.word	.Ltmp184
	.word	.Ltmp188
	.word	.Ltmp192
	.word	.Ltmp193
	.word	.Ltmp196
	.word	.Ltmp199
	.word	.Ltmp203
	.word	.Ltmp209
	.word	.Ltmp212
	.word	.Ltmp217
	.word	.Ltmp220
	.word	.Ltmp222
	.word	.Ltmp224
	.word	.Ltmp225
	.word	.Ltmp232
	.word	.Ltmp234
	.word	.Ltmp236
	.word	.Ltmp238
	.word	.Ltmp241
	.word	.Ltmp245
	.word	.Ltmp247
	.word	.Ltmp250
	.word	.Ltmp141
	.word	.Lfunc_begin2
	.word	.Lfunc_begin3
	.word	.Lfunc_begin4
	.word	.Lfunc_begin5
	.word	.Lfunc_begin6
	.word	.Lfunc_begin7
	.word	.Lfunc_begin8
	.word	.Lfunc_begin9
	.word	.Lfunc_begin10
	.word	.Lfunc_begin11
	.word	.Lfunc_begin12
	.word	.Lfunc_begin13
	.word	.Lfunc_begin14
	.word	.Ltmp291
	.word	.Lfunc_begin15
	.word	.Ltmp298
	.word	.Lfunc_begin16
	.word	.Ltmp305
	.word	.Lfunc_begin17
	.word	.Ltmp313
.Ldebug_addr_end0:
	.ident	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym hero_load_uint32_noblock
	.addrsig_sym hero_load_uint32
	.addrsig_sym hero_store_uint32_noblock
	.addrsig_sym hero_store_uint32
	.addrsig_sym hero_load_uint16_noblock
	.addrsig_sym hero_load_uint16
	.addrsig_sym hero_store_uint16_noblock
	.addrsig_sym hero_store_uint16
	.addrsig_sym hero_load_uint8_noblock
	.addrsig_sym hero_load_uint8
	.addrsig_sym hero_store_uint8_noblock
	.addrsig_sym hero_store_uint8
	.addrsig_sym hero_load_uint64
	.addrsig_sym hero_store_uint64
	.addrsig_sym hero_load_uint64_noblock
	.addrsig_sym hero_store_uint64_noblock
	.addrsig_sym __omp_offloading_10303_7120698_axpy_l19
	.addrsig_sym l1_buf_x
	.addrsig_sym l1_buf_y
	.addrsig_sym dma_timer
	.addrsig_sym dma_time
	.addrsig_sym all_timer
	.addrsig_sym all_time
	.addrsig_sym issue_timer
	.addrsig_sym issue_time
	.addrsig_sym compute_timer
	.addrsig_sym compute_time
	.addrsig_sym barrier_reg
	.section	.debug_line,"",@progbits
.Lline_table_start0:
