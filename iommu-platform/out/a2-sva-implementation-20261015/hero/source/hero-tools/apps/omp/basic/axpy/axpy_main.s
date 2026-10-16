	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.file	"axpy_main.c"
	.file	0 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" "axpy_main.c" md5 0x10b52410629400e32e91ccd738cc851d
	.section	.sdata,"aw",@progbits
	.p2align	2                               # -- Begin function main
.LCPI0_0:
	.word	0x3f800000                      # float 1
.LCPI0_1:
	.word	0x7f800000                      # float +Inf
.LCPI0_2:
	.word	0x3e800000                      # float 0.25
	.text
	.globl	main
	.p2align	1
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.file	1 "axpy_main.c"
	.loc	1 43 0                          # axpy_main.c:43:0
	.cfi_sections .debug_frame
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: main:argc <- $x10
	#DEBUG_VALUE: main:argv <- $x11
	addi	sp, sp, -160
	.cfi_def_cfa_offset 160
	sd	ra, 152(sp)                     # 8-byte Folded Spill
	sd	s0, 144(sp)                     # 8-byte Folded Spill
	sd	s1, 136(sp)                     # 8-byte Folded Spill
	sd	s2, 128(sp)                     # 8-byte Folded Spill
	sd	s3, 120(sp)                     # 8-byte Folded Spill
	sd	s4, 112(sp)                     # 8-byte Folded Spill
	sd	s5, 104(sp)                     # 8-byte Folded Spill
	sd	s6, 96(sp)                      # 8-byte Folded Spill
	sd	s7, 88(sp)                      # 8-byte Folded Spill
	sd	s8, 80(sp)                      # 8-byte Folded Spill
	sd	s9, 72(sp)                      # 8-byte Folded Spill
	sd	s10, 64(sp)                     # 8-byte Folded Spill
	sd	s11, 56(sp)                     # 8-byte Folded Spill
	fsd	fs0, 48(sp)                     # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
	.cfi_offset s3, -40
	.cfi_offset s4, -48
	.cfi_offset s5, -56
	.cfi_offset s6, -64
	.cfi_offset s7, -72
	.cfi_offset s8, -80
	.cfi_offset s9, -88
	.cfi_offset s10, -96
	.cfi_offset s11, -104
	.cfi_offset fs0, -112
	li	a2, 2
	#DEBUG_VALUE: main:argv <- $x11
.Ltmp0:
	#DEBUG_VALUE: main:bytes <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:n <- 32768
	#DEBUG_VALUE: main:argc <- $x10
	.loc	1 55 9 prologue_end             # axpy_main.c:55:9
	blt	a0, a2, .LBB0_10
.Ltmp1:
# %bb.1:
	#DEBUG_VALUE: main:n <- 32768
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	#DEBUG_VALUE: main:argv <- $x11
	#DEBUG_VALUE: main:argc <- $x10
	.loc	1 0 9 is_stmt 0                 # axpy_main.c:0:9
	mv	s2, a1
.Ltmp2:
	#DEBUG_VALUE: main:argv <- $x18
	mv	s1, a0
.Ltmp3:
	#DEBUG_VALUE: main:argc <- $x9
	.loc	1 56 11 is_stmt 1               # axpy_main.c:56:11
	call	__errno_location
.Ltmp4:
	mv	s0, a0
	.loc	1 56 32 is_stmt 0               # axpy_main.c:56:32
	sw	zero, 0(a0)
	.loc	1 56 53                         # axpy_main.c:56:53
	ld	a0, 8(s2)
.Ltmp5:
	#DEBUG_VALUE: main:end <- [DW_OP_plus_uconst 40, DW_OP_deref] $x2
	.loc	1 56 46                         # axpy_main.c:56:46
	addi	a1, sp, 40
	li	a2, 10
	call	strtol
.Ltmp6:
	.loc	1 57 14 is_stmt 1               # axpy_main.c:57:14
	lw	a1, 0(s0)
.Ltmp7:
	#DEBUG_VALUE: main:parsed <- $x10
	.loc	1 57 36 is_stmt 0               # axpy_main.c:57:36
	bnez	a1, .LBB0_31
.Ltmp8:
# %bb.2:
	#DEBUG_VALUE: main:parsed <- $x10
	#DEBUG_VALUE: main:end <- [DW_OP_plus_uconst 40, DW_OP_deref] $x2
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:n <- 32768
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 0 0                           # axpy_main.c:0:0
	mv	s5, a0
.Ltmp9:
	#DEBUG_VALUE: main:parsed <- $x21
	.loc	1 57 40                         # axpy_main.c:57:40
	ld	a0, 40(sp)
.Ltmp10:
	#DEBUG_VALUE: main:end <- $x10
	.loc	1 57 39                         # axpy_main.c:57:39
	lbu	a0, 0(a0)
.Ltmp11:
	snez	a0, a0
	slti	a1, s5, 1
	.loc	1 57 44                         # axpy_main.c:57:44
	or	a0, a0, a1
	srli	a1, s5, 30
	snez	a1, a1
	or	a0, a0, a1
	bnez	a0, .LBB0_31
.Ltmp12:
# %bb.3:
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:n <- 32768
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	.loc	1 0 44                          # axpy_main.c:0:44
	li	a0, 3
	.loc	1 61 9 is_stmt 1                # axpy_main.c:61:9
	bltu	s1, a0, .LBB0_11
.Ltmp13:
# %bb.4:
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 62 32                         # axpy_main.c:62:32
	sw	zero, 0(s0)
	.loc	1 62 52 is_stmt 0               # axpy_main.c:62:52
	ld	a0, 16(s2)
.Ltmp14:
	#DEBUG_VALUE: main:end <- [DW_OP_plus_uconst 40, DW_OP_deref] $x2
	.loc	1 62 45                         # axpy_main.c:62:45
	addi	a1, sp, 40
	call	strtof
.Ltmp15:
	.loc	1 63 14 is_stmt 1               # axpy_main.c:63:14
	lw	a0, 0(s0)
.Ltmp16:
	#DEBUG_VALUE: main:alpha <- $f10_f
	.loc	1 63 36 is_stmt 0               # axpy_main.c:63:36
	bnez	a0, .LBB0_31
.Ltmp17:
# %bb.5:
	#DEBUG_VALUE: main:alpha <- $f10_f
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:end <- [DW_OP_plus_uconst 40, DW_OP_deref] $x2
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 0 0                           # axpy_main.c:0:0
	fmv.s	fs0, fa0
.Ltmp18:
	#DEBUG_VALUE: main:alpha <- $f8_f
	.loc	1 63 40                         # axpy_main.c:63:40
	ld	a0, 40(sp)
.Ltmp19:
	#DEBUG_VALUE: main:end <- $x10
	.loc	1 63 39                         # axpy_main.c:63:39
	lbu	a0, 0(a0)
.Ltmp20:
	.loc	1 0 39                          # axpy_main.c:0:39
	lui	a1, %hi(.LCPI0_1)
	flw	ft0, %lo(.LCPI0_1)(a1)
	.loc	1 63 39                         # axpy_main.c:63:39
	seqz	a0, a0
	fabs.s	ft1, fa0
	flt.s	a1, ft1, ft0
	.loc	1 63 44                         # axpy_main.c:63:44
	and	a0, a0, a1
	andi	a1, s5, 511
	seqz	a1, a1
	and	a0, a0, a1
	#DEBUG_VALUE: main:alpha <- $f8_f
	beqz	a0, .LBB0_31
.Ltmp21:
# %bb.6:
	#DEBUG_VALUE: main:alpha <- $f8_f
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	#DEBUG_VALUE: main:alpha <- $f8_f
	.loc	1 0 44                          # axpy_main.c:0:44
	li	a0, 4
	bltu	s1, a0, .LBB0_13
.Ltmp22:
# %bb.7:
	#DEBUG_VALUE: main:alpha <- $f8_f
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 67 41 is_stmt 1               # axpy_main.c:67:41
	ld	a0, 24(s2)
	.loc	1 67 34 is_stmt 0               # axpy_main.c:67:34
	lui	a1, %hi(.L.str)
	addi	a1, a1, %lo(.L.str)
	call	strcmp
.Ltmp23:
	.loc	1 0 34                          # axpy_main.c:0:34
	bnez	a0, .LBB0_31
.Ltmp24:
# %bb.8:
	#DEBUG_VALUE: main:alpha <- $f8_f
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	li	a0, 5
	bltu	s1, a0, .LBB0_13
.Ltmp25:
# %bb.9:
	#DEBUG_VALUE: main:alpha <- $f8_f
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 68 29 is_stmt 1               # axpy_main.c:68:29
	ld	a0, 32(s2)
	.loc	1 68 22 is_stmt 0               # axpy_main.c:68:22
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	call	strcmp
.Ltmp26:
	.loc	1 0 22                          # axpy_main.c:0:22
	xori	a1, s1, 5
	.loc	1 68 44                         # axpy_main.c:68:44
	or	a0, a0, a1
	beqz	a0, .LBB0_13
	j	.LBB0_31
.Ltmp27:
.LBB0_10:
	#DEBUG_VALUE: main:n <- 32768
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	#DEBUG_VALUE: main:argv <- $x11
	#DEBUG_VALUE: main:argc <- $x10
	.loc	1 0 44                          # axpy_main.c:0:44
	lui	a0, %hi(.LCPI0_0)
.Ltmp28:
	flw	fs0, %lo(.LCPI0_0)(a0)
	lui	s5, 8
	lui	s10, 32
	j	.LBB0_15
.Ltmp29:
.LBB0_11:                               # %.thread238
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 67 11 is_stmt 1               # axpy_main.c:67:11
	andi	a0, s5, 511
	.loc	1 67 18 is_stmt 0               # axpy_main.c:67:18
	bnez	a0, .LBB0_31
.Ltmp30:
# %bb.12:
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:alpha <- 1.000000e+00
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	.loc	1 0 18                          # axpy_main.c:0:18
	lui	a0, %hi(.LCPI0_0)
	flw	fs0, %lo(.LCPI0_0)(a0)
.Ltmp31:
.LBB0_13:                               # %.thread244
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:bytes <- 0
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	.loc	1 70 13 is_stmt 1               # axpy_main.c:70:13
	slliw	a0, s5, 2
	lui	a1, 1048575
.Ltmp32:
	#DEBUG_VALUE: main:bytes <- undef
	.loc	1 71 9                          # axpy_main.c:71:9
	bltu	a1, a0, .LBB0_31
.Ltmp33:
# %bb.14:
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	#DEBUG_VALUE: main:parsed <- $x21
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:n <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $x21
	.loc	1 0 9 is_stmt 0                 # axpy_main.c:0:9
	slli	s10, s5, 2
.Ltmp34:
	#DEBUG_VALUE: main:bytes <- $x26
.LBB0_15:                               # %.thread244.thread
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	lui	a0, 1
	addiw	a0, a0, -1
	.loc	1 72 28 is_stmt 1               # axpy_main.c:72:28
	addw	s0, s10, a0
.Ltmp35:
	#DEBUG_VALUE: main:bytes <- undef
	.loc	1 0 28 is_stmt 0                # axpy_main.c:0:28
	lui	a0, 256
	addiw	a0, a0, -1
	slli	a0, a0, 12
	.loc	1 73 34 is_stmt 1               # axpy_main.c:73:34
	and	s1, s0, a0
	.loc	1 73 13 is_stmt 0               # axpy_main.c:73:13
	lui	a0, 1
	mv	a1, s1
	call	aligned_alloc
.Ltmp36:
	mv	s9, a0
.Ltmp37:
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 74 13 is_stmt 1               # axpy_main.c:74:13
	lui	a0, 1
	mv	a1, s1
	call	aligned_alloc
.Ltmp38:
	mv	s2, a0
.Ltmp39:
	#DEBUG_VALUE: main:y_ref <- $x18
	.loc	1 75 16                         # axpy_main.c:75:16
	lui	a0, 1
	mv	a1, s1
	call	aligned_alloc
.Ltmp40:
	mv	s11, a0
.Ltmp41:
	#DEBUG_VALUE: main:expected <- $x27
	.loc	1 76 13                         # axpy_main.c:76:13
	lui	a0, 1
	mv	a1, s1
	call	aligned_alloc
.Ltmp42:
	mv	s4, a0
.Ltmp43:
	#DEBUG_VALUE: main:x_dma <- $x20
	.loc	1 77 13                         # axpy_main.c:77:13
	lui	a0, 1
	mv	a1, s1
	call	aligned_alloc
.Ltmp44:
	mv	s3, a0
.Ltmp45:
	#DEBUG_VALUE: main:y_dma <- $x19
	.loc	1 78 10                         # axpy_main.c:78:10
	seqz	a0, s9
	seqz	a1, s2
	.loc	1 78 16 is_stmt 0               # axpy_main.c:78:16
	or	a0, a0, a1
	seqz	a1, s11
	or	a0, a0, a1
	seqz	a1, s4
	or	a0, a0, a1
	seqz	a1, s3
	or	a0, a0, a1
	bnez	a0, .LBB0_25
.Ltmp46:
# %bb.16:                               # %.lr.ph.preheader
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	.loc	1 0 16                          # axpy_main.c:0:16
	lui	a0, 1048575
	and	s6, s0, a0
.Ltmp47:
	#DEBUG_VALUE: main:bytes <- $x22
	.loc	1 80 5 is_stmt 1                # axpy_main.c:80:5
	mv	a0, s4
	li	a1, 0
	mv	a2, s1
	call	memset@plt
.Ltmp48:
	.loc	1 80 30 is_stmt 0               # axpy_main.c:80:30
	mv	a0, s3
	li	a1, 0
	mv	a2, s1
	call	memset@plt
.Ltmp49:
	#DEBUG_VALUE: i <- 0
	.loc	1 0 30                          # axpy_main.c:0:30
	li	a0, 0
	li	a1, 0
.Ltmp50:
	.loc	1 81 28 is_stmt 1               # axpy_main.c:81:28
	slli	a2, s5, 32
	srli	a2, a2, 32
	lui	a3, 139192
	addiw	a3, a3, -2029
	slli	a3, a3, 32
	li	a6, 113
	lui	a5, %hi(.LCPI0_2)
	flw	ft0, %lo(.LCPI0_2)(a5)
	li	a7, 11
	lui	s1, %hi(.LCPI0_1)
	flw	ft1, %lo(.LCPI0_1)(s1)
	lui	s1, 8257
	addiw	s1, s1, -2031
	slli	s1, s1, 32
.Ltmp51:
.LBB0_17:                               # %.lr.ph
                                        # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: i <- $x11
	.loc	1 82 31                         # axpy_main.c:82:31
	slliw	s0, a1, 4
	addw	s0, s0, a1
	.loc	1 82 37 is_stmt 0               # axpy_main.c:82:37
	addiw	s0, s0, 3
	.loc	1 82 43                         # axpy_main.c:82:43
	slli	a4, s0, 32
	mulhu	a4, a4, a3
	srli	a4, a4, 32
	subw	a5, s0, a4
	srliw	a5, a5, 1
	add	a4, a4, a5
	srli	a4, a4, 6
	mulw	a4, a4, a6
	subw	a4, s0, a4
	.loc	1 82 20                         # axpy_main.c:82:20
	fcvt.s.wu	ft2, a4
	.loc	1 82 51                         # axpy_main.c:82:51
	fmul.s	ft2, ft2, ft0
	.loc	1 82 18                         # axpy_main.c:82:18
	add	a4, s9, a0
	fsw	ft2, 0(a4)
	.loc	1 83 31 is_stmt 1               # axpy_main.c:83:31
	mulw	a4, a1, a7
	.loc	1 83 37 is_stmt 0               # axpy_main.c:83:37
	addiw	a4, a4, 7
	.loc	1 83 43                         # axpy_main.c:83:43
	slli	a5, a4, 32
	mulhu	a5, a5, s1
	srli	a5, a5, 32
	subw	s0, a4, a5
	srliw	s0, s0, 1
	add	a5, a5, s0
	srli	a5, a5, 6
	slliw	s0, a5, 7
	subw	a5, a5, s0
	addw	a4, a4, a5
	.loc	1 83 20                         # axpy_main.c:83:20
	fcvt.s.wu	ft3, a4
	.loc	1 83 51                         # axpy_main.c:83:51
	fmul.s	ft3, ft3, ft0
	.loc	1 83 18                         # axpy_main.c:83:18
	add	a4, s2, a0
	fsw	ft3, 0(a4)
	.loc	1 84 23 is_stmt 1               # axpy_main.c:84:23
	fmadd.s	ft2, fs0, ft2, ft3
	.loc	1 84 21 is_stmt 0               # axpy_main.c:84:21
	add	a4, s11, a0
.Ltmp52:
	.loc	1 85 14 is_stmt 1               # axpy_main.c:85:14
	fabs.s	ft3, ft2
	flt.s	a5, ft3, ft1
	flt.s	s0, ft1, ft3
	or	a5, a5, s0
.Ltmp53:
	#DEBUG_VALUE: i <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x11
	.loc	1 84 21                         # axpy_main.c:84:21
	fsw	ft2, 0(a4)
	.loc	1 85 13                         # axpy_main.c:85:13
	beqz	a5, .LBB0_25
.Ltmp54:
# %bb.18:                               #   in Loop: Header=BB0_17 Depth=1
	#DEBUG_VALUE: i <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x11
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	.loc	1 81 34                         # axpy_main.c:81:34
	addi	a1, a1, 1
.Ltmp55:
	#DEBUG_VALUE: i <- [DW_OP_plus_uconst 1, DW_OP_stack_value] $x11
	.loc	1 81 28 is_stmt 0               # axpy_main.c:81:28
	addi	a0, a0, 4
.Ltmp56:
	.loc	1 81 5                          # axpy_main.c:81:5
	bne	a2, a1, .LBB0_17
.Ltmp57:
# %bb.19:                               # %._crit_edge
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	.loc	1 87 5 is_stmt 1                # axpy_main.c:87:5
	call	init_omp_target
.Ltmp58:
	#DEBUG_VALUE: main:x_attempted <- 1
	.loc	1 91 14                         # axpy_main.c:91:14
	li	a0, 0
	mv	a1, s6
	mv	a2, s4
	call	hero_iommu_map_virt
.Ltmp59:
	#DEBUG_VALUE: main:x_iova <- $x10
	.loc	1 0 14 is_stmt 0                # axpy_main.c:0:14
	sd	a0, 32(sp)                      # 8-byte Folded Spill
.Ltmp60:
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	.loc	1 92 9 is_stmt 1                # axpy_main.c:92:9
	beqz	a0, .LBB0_49
.Ltmp61:
# %bb.20:
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	.loc	1 95 14                         # axpy_main.c:95:14
	li	a0, 0
	mv	a1, s6
	mv	a2, s3
	call	hero_iommu_map_virt
.Ltmp62:
	mv	s7, a0
.Ltmp63:
	#DEBUG_VALUE: main:y_iova <- $x23
	.loc	1 96 9                          # axpy_main.c:96:9
	beqz	a0, .LBB0_50
.Ltmp64:
# %bb.21:                               # %.preheader
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: run <- 0
	.loc	1 0 9 is_stmt 0                 # axpy_main.c:0:9
	li	s8, 1
.Ltmp65:
	.loc	1 102 9 is_stmt 1               # axpy_main.c:102:9
	mv	a0, s4
	mv	a1, s9
	mv	a2, s10
	call	memcpy@plt
.Ltmp66:
	.loc	1 103 9                         # axpy_main.c:103:9
	mv	a0, s3
	mv	a1, s2
	mv	a2, s10
	call	memcpy@plt
.Ltmp67:
	.loc	1 104 13                        # axpy_main.c:104:13
	li	a0, 0
	mv	a1, s6
	mv	a2, s4
	ld	a3, 32(sp)                      # 8-byte Folded Reload
	call	hero_iommu_sync_for_device
.Ltmp68:
	#DEBUG_VALUE: label <- undef
	.loc	1 104 74 is_stmt 0              # axpy_main.c:104:74
	bnez	a0, .LBB0_48
.Ltmp69:
# %bb.22:
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 105 13 is_stmt 1              # axpy_main.c:105:13
	mv	a1, s6
	mv	a2, s3
	mv	a3, s7
	call	hero_iommu_sync_for_device
.Ltmp70:
	.loc	1 104 13                        # axpy_main.c:104:13
	bnez	a0, .LBB0_48
.Ltmp71:
# %bb.23:
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	sd	s10, 24(sp)                     # 8-byte Folded Spill
	li	a0, 1
	mv	s8, s6
.Ltmp72:
	#DEBUG_VALUE: main:bytes <- $x24
	slli	a0, a0, 33
	ld	s0, 32(sp)                      # 8-byte Folded Reload
	add	s6, s0, a0
	mv	s10, s7
	add	s7, s7, a0
.Ltmp73:
	.loc	1 110 13 is_stmt 1              # axpy_main.c:110:13
	sext.w	s1, s5
	mv	a0, s6
	mv	a1, s7
	fmv.s	fa0, fs0
	mv	a2, s1
	call	axpy
.Ltmp74:
	.loc	1 110 13 is_stmt 0              # axpy_main.c:110:13
	beqz	a0, .LBB0_32
.Ltmp75:
# %bb.24:
	#DEBUG_VALUE: main:bytes <- $x24
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 0 13                          # axpy_main.c:0:13
	mv	s6, s8
.Ltmp76:
	#DEBUG_VALUE: main:bytes <- $x22
	li	s8, 1
	mv	s7, s10
	mv	s10, s0
	j	.LBB0_54
.Ltmp77:
.LBB0_25:
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	li	s7, 0
	li	s5, 0
	li	s8, 1
	li	s0, 1
.Ltmp78:
.LBB0_26:                               # %.thread297.thread
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 136 48 is_stmt 1              # axpy_main.c:136:48
	mv	a0, s4
	call	free
.Ltmp79:
	.loc	1 0 48 is_stmt 0                # axpy_main.c:0:48
	mv	s1, s0
.Ltmp80:
.LBB0_27:                               # %.thread297.thread335
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 137 22 is_stmt 1              # axpy_main.c:137:22
	not	a0, s1
	seqz	a1, s7
	or	a1, a1, s5
	and	a0, a0, a1
	bnez	a0, .LBB0_29
.Ltmp81:
.LBB0_28:
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 137 48 is_stmt 0              # axpy_main.c:137:48
	mv	a0, s3
	call	free
.Ltmp82:
.LBB0_29:
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 138 5 is_stmt 1               # axpy_main.c:138:5
	mv	a0, s9
	call	free
.Ltmp83:
	.loc	1 138 18 is_stmt 0              # axpy_main.c:138:18
	mv	a0, s2
	call	free
.Ltmp84:
	.loc	1 138 31                        # axpy_main.c:138:31
	mv	a0, s11
	call	free
.Ltmp85:
.LBB0_30:
	.loc	1 143 1 is_stmt 1               # axpy_main.c:143:1
	mv	a0, s8
	ld	ra, 152(sp)                     # 8-byte Folded Reload
	ld	s0, 144(sp)                     # 8-byte Folded Reload
	ld	s1, 136(sp)                     # 8-byte Folded Reload
	ld	s2, 128(sp)                     # 8-byte Folded Reload
	ld	s3, 120(sp)                     # 8-byte Folded Reload
	ld	s4, 112(sp)                     # 8-byte Folded Reload
	ld	s5, 104(sp)                     # 8-byte Folded Reload
	ld	s6, 96(sp)                      # 8-byte Folded Reload
	ld	s7, 88(sp)                      # 8-byte Folded Reload
	ld	s8, 80(sp)                      # 8-byte Folded Reload
	ld	s9, 72(sp)                      # 8-byte Folded Reload
	ld	s10, 64(sp)                     # 8-byte Folded Reload
	ld	s11, 56(sp)                     # 8-byte Folded Reload
	fld	fs0, 48(sp)                     # 8-byte Folded Reload
	addi	sp, sp, 160
	ret
.LBB0_31:
.Ltmp86:
	#DEBUG_VALUE: main:argc <- $x9
	#DEBUG_VALUE: main:argv <- $x18
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:x_attempted <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:x_ref <- 0
	#DEBUG_VALUE: main:y_ref <- 0
	#DEBUG_VALUE: main:expected <- 0
	#DEBUG_VALUE: main:x_dma <- 0
	#DEBUG_VALUE: main:y_dma <- 0
	#DEBUG_VALUE: main:x_iova <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	#DEBUG_LABEL: main:usage
	.loc	1 141 13                        # axpy_main.c:141:13
	lui	a0, %hi(stderr)
	ld	a0, %lo(stderr)(a0)
	.loc	1 141 80 is_stmt 0              # axpy_main.c:141:80
	ld	a2, 0(s2)
	.loc	1 141 5                         # axpy_main.c:141:5
	lui	a1, %hi(.L.str.10)
	addi	a1, a1, %lo(.L.str.10)
	call	fprintf
.Ltmp87:
	.loc	1 0 5                           # axpy_main.c:0:5
	li	s8, 1
	j	.LBB0_30
.Ltmp88:
.LBB0_32:
	#DEBUG_VALUE: main:bytes <- $x24
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	sd	s7, 0(sp)                       # 8-byte Folded Spill
	sd	s6, 8(sp)                       # 8-byte Folded Spill
	sd	s1, 16(sp)                      # 8-byte Folded Spill
	mv	s6, s8
.Ltmp89:
	.loc	1 112 13 is_stmt 1              # axpy_main.c:112:13
	mv	a1, s8
	mv	a2, s4
	mv	a3, s0
	call	hero_iommu_sync_for_cpu
.Ltmp90:
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	li	s8, 1
.Ltmp91:
	mv	s7, s10
	.loc	1 112 71                        # axpy_main.c:112:71
	bnez	a0, .LBB0_51
.Ltmp92:
# %bb.33:
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 113 13 is_stmt 1              # axpy_main.c:113:13
	mv	a1, s6
	mv	a2, s3
	mv	a3, s7
	call	hero_iommu_sync_for_cpu
.Ltmp93:
	.loc	1 112 13                        # axpy_main.c:112:13
	bnez	a0, .LBB0_51
.Ltmp94:
# %bb.34:                               # %.lr.ph.i.preheader
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	li	a3, 0
	lui	a1, %hi(.L.str.5)
	addi	a2, a1, %lo(.L.str.5)
	ld	s10, 24(sp)                     # 8-byte Folded Reload
.Ltmp95:
.LBB0_35:                               # %.lr.ph.i
                                        # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: i <- $x13
	.loc	1 31 13 is_stmt 1               # axpy_main.c:31:13
	add	a1, s4, a0
	flw	ft0, 0(a1)
	.loc	1 31 28 is_stmt 0               # axpy_main.c:31:28
	add	a1, s9, a0
	flw	ft1, 0(a1)
	.loc	1 31 25                         # axpy_main.c:31:25
	feq.s	a1, ft0, ft1
	.loc	1 31 43                         # axpy_main.c:31:43
	beqz	a1, .LBB0_60
.Ltmp96:
# %bb.36:                               #   in Loop: Header=BB0_35 Depth=1
	#DEBUG_VALUE: i <- $x13
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 32 35 is_stmt 1               # axpy_main.c:32:35
	add	a1, s3, a0
	.loc	1 32 13 is_stmt 0               # axpy_main.c:32:13
	add	a4, s11, a0
	lbu	a5, 1(a4)
	lbu	s0, 0(a4)
	lb	s1, 3(a4)
	lbu	a4, 2(a4)
	slli	a5, a5, 8
	or	a5, a5, s0
	slli	s1, s1, 8
	or	a4, a4, s1
	slli	a4, a4, 16
	or	a4, a4, a5
	lbu	a5, 1(a1)
	lbu	s1, 0(a1)
	lb	s0, 3(a1)
	lbu	a1, 2(a1)
	slli	a5, a5, 8
	or	a5, a5, s1
	slli	s1, s0, 8
	or	a1, a1, s1
	slli	a1, a1, 16
	or	a1, a1, a5
.Ltmp97:
	.loc	1 31 13 is_stmt 1               # axpy_main.c:31:13
	bne	a1, a4, .LBB0_60
.Ltmp98:
# %bb.37:                               #   in Loop: Header=BB0_35 Depth=1
	#DEBUG_VALUE: i <- $x13
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 30 34                         # axpy_main.c:30:34
	addi	a3, a3, 1
.Ltmp99:
	#DEBUG_VALUE: i <- $x13
	.loc	1 30 28 is_stmt 0               # axpy_main.c:30:28
	addi	a0, a0, 4
.Ltmp100:
	.loc	1 30 5                          # axpy_main.c:30:5
	bne	s5, a3, .LBB0_35
.Ltmp101:
# %bb.38:                               # %.loopexit
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 38 5 is_stmt 1                # axpy_main.c:38:5
	lui	a0, %hi(.L.str.12)
	addi	a0, a0, %lo(.L.str.12)
	lui	a1, %hi(.L.str.5)
	addi	a1, a1, %lo(.L.str.5)
	ld	a2, 16(sp)                      # 8-byte Folded Reload
	call	printf
.Ltmp102:
	#DEBUG_VALUE: run <- 1
	.loc	1 102 9                         # axpy_main.c:102:9
	mv	a0, s4
	mv	a1, s9
	mv	a2, s10
	call	memcpy@plt
.Ltmp103:
	.loc	1 103 9                         # axpy_main.c:103:9
	mv	a0, s3
	mv	a1, s2
	mv	a2, s10
	call	memcpy@plt
.Ltmp104:
	.loc	1 104 13                        # axpy_main.c:104:13
	li	a0, 0
	mv	a1, s6
	mv	a2, s4
	ld	a3, 32(sp)                      # 8-byte Folded Reload
	call	hero_iommu_sync_for_device
.Ltmp105:
	#DEBUG_VALUE: label <- undef
	.loc	1 104 74 is_stmt 0              # axpy_main.c:104:74
	bnez	a0, .LBB0_48
.Ltmp106:
# %bb.39:
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 105 13 is_stmt 1              # axpy_main.c:105:13
	mv	a1, s6
	mv	a2, s3
	mv	a3, s7
	call	hero_iommu_sync_for_device
.Ltmp107:
	.loc	1 104 13                        # axpy_main.c:104:13
	bnez	a0, .LBB0_48
.Ltmp108:
# %bb.40:
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 110 13                        # axpy_main.c:110:13
	ld	a0, 8(sp)                       # 8-byte Folded Reload
	ld	a1, 0(sp)                       # 8-byte Folded Reload
	fmv.s	fa0, fs0
	ld	a2, 16(sp)                      # 8-byte Folded Reload
	call	axpy
.Ltmp109:
	.loc	1 110 13 is_stmt 0              # axpy_main.c:110:13
	bnez	a0, .LBB0_53
.Ltmp110:
# %bb.41:
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 112 13 is_stmt 1              # axpy_main.c:112:13
	mv	a1, s6
	mv	a2, s4
	ld	a3, 32(sp)                      # 8-byte Folded Reload
	call	hero_iommu_sync_for_cpu
.Ltmp111:
	.loc	1 112 71 is_stmt 0              # axpy_main.c:112:71
	bnez	a0, .LBB0_51
.Ltmp112:
# %bb.42:
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 113 13 is_stmt 1              # axpy_main.c:113:13
	mv	a1, s6
	mv	a2, s3
	mv	a3, s7
	call	hero_iommu_sync_for_cpu
.Ltmp113:
	.loc	1 112 13                        # axpy_main.c:112:13
	bnez	a0, .LBB0_51
.Ltmp114:
# %bb.43:                               # %.lr.ph.i.1.preheader
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	li	a3, 0
	lui	a0, %hi(.L.str.4)
	addi	a2, a0, %lo(.L.str.4)
	mv	a0, s4
	mv	a1, s9
	mv	a4, s3
	mv	a5, s11
	ld	s10, 32(sp)                     # 8-byte Folded Reload
.Ltmp115:
.LBB0_44:                               # %.lr.ph.i.1
                                        # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: i <- $x13
	.loc	1 31 13 is_stmt 1               # axpy_main.c:31:13
	flw	ft0, 0(a0)
	.loc	1 31 28 is_stmt 0               # axpy_main.c:31:28
	flw	ft1, 0(a1)
	.loc	1 31 25                         # axpy_main.c:31:25
	feq.s	s1, ft0, ft1
	.loc	1 31 43                         # axpy_main.c:31:43
	beqz	s1, .LBB0_61
.Ltmp116:
# %bb.45:                               #   in Loop: Header=BB0_44 Depth=1
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: i <- $x13
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 32 13 is_stmt 1               # axpy_main.c:32:13
	lbu	a7, 1(a5)
	lbu	a6, 0(a5)
	lb	t0, 3(a5)
	lbu	s1, 2(a5)
	slli	s0, a7, 8
	or	a6, s0, a6
	slli	s0, t0, 8
	or	s1, s1, s0
	slli	s1, s1, 16
	or	a6, s1, a6
	lbu	a7, 1(a4)
	lbu	t0, 0(a4)
	lb	t1, 3(a4)
	lbu	s1, 2(a4)
	slli	s0, a7, 8
	or	a7, s0, t0
	slli	s0, t1, 8
	or	s1, s1, s0
	slli	s1, s1, 16
	or	s1, s1, a7
.Ltmp117:
	.loc	1 31 13                         # axpy_main.c:31:13
	bne	s1, a6, .LBB0_61
.Ltmp118:
# %bb.46:                               #   in Loop: Header=BB0_44 Depth=1
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: i <- $x13
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 30 34                         # axpy_main.c:30:34
	addi	a3, a3, 1
.Ltmp119:
	#DEBUG_VALUE: i <- $x13
	.loc	1 30 28 is_stmt 0               # axpy_main.c:30:28
	addi	a5, a5, 4
	addi	a4, a4, 4
	addi	a1, a1, 4
	addi	a0, a0, 4
.Ltmp120:
	.loc	1 30 5                          # axpy_main.c:30:5
	bne	s5, a3, .LBB0_44
.Ltmp121:
# %bb.47:                               # %.loopexit.1
	#DEBUG_VALUE: run <- 1
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 38 5 is_stmt 1                # axpy_main.c:38:5
	lui	a0, %hi(.L.str.12)
	addi	a0, a0, %lo(.L.str.12)
	lui	a1, %hi(.L.str.4)
	addi	a1, a1, %lo(.L.str.4)
	ld	a2, 16(sp)                      # 8-byte Folded Reload
	call	printf
.Ltmp122:
	#DEBUG_VALUE: run <- 2
	.loc	1 0 5 is_stmt 0                 # axpy_main.c:0:5
	li	s8, 0
	j	.LBB0_54
.Ltmp123:
.LBB0_48:
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 106 13 is_stmt 1              # axpy_main.c:106:13
	lui	a0, %hi(.L.str.6)
	addi	a0, a0, %lo(.L.str.6)
	j	.LBB0_52
.Ltmp124:
.LBB0_49:                               # %.thread297.thread335.thread
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:x_mapped <- 0
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:y_attempted <- 0
	#DEBUG_VALUE: main:y_iova <- 0
	.loc	1 92 20                         # axpy_main.c:92:20
	lui	a0, %hi(.L.str.2)
	addi	a0, a0, %lo(.L.str.2)
	call	perror
.Ltmp125:
	.loc	1 0 20 is_stmt 0                # axpy_main.c:0:20
	li	s8, 1
	j	.LBB0_28
.Ltmp126:
.LBB0_50:
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:bytes <- $x22
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	lui	a0, %hi(.L.str.3)
	addi	a0, a0, %lo(.L.str.3)
	ld	s10, 32(sp)                     # 8-byte Folded Reload
	j	.LBB0_59
.Ltmp127:
.LBB0_51:
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 114 13 is_stmt 1              # axpy_main.c:114:13
	lui	a0, %hi(.L.str.7)
	addi	a0, a0, %lo(.L.str.7)
.Ltmp128:
.LBB0_52:                               # %.thread264
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 0 0 is_stmt 0                 # axpy_main.c:0:0
	call	perror
.Ltmp129:
.LBB0_53:                               # %.thread264
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	ld	s10, 32(sp)                     # 8-byte Folded Reload
.Ltmp130:
.LBB0_54:                               # %.thread264
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	#DEBUG_VALUE: main:status <- $x24
	#DEBUG_VALUE: main:x_mapped <- undef
	#DEBUG_VALUE: main:y_mapped <- undef
	#DEBUG_VALUE: main:x_attempted <- undef
	#DEBUG_VALUE: main:y_attempted <- undef
	#DEBUG_VALUE: main:x_iova <- $x26
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_LABEL: main:cleanup
	.loc	1 121 13 is_stmt 1              # axpy_main.c:121:13
	li	a0, 0
	mv	a1, s6
	mv	a2, s3
	mv	a3, s7
	call	hero_iommu_unmap_virt
.Ltmp131:
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	li	s5, 0
.Ltmp132:
	.loc	1 121 13                        # axpy_main.c:121:13
	bnez	a0, .LBB0_58
.Ltmp133:
.LBB0_55:                               # %.thread282
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 0 0                           # axpy_main.c:0:0
	seqz	s1, s10
.Ltmp134:
	.loc	1 127 13 is_stmt 1              # axpy_main.c:127:13
	li	a0, 0
	mv	a1, s6
	mv	a2, s4
	mv	a3, s10
	call	hero_iommu_unmap_virt
.Ltmp135:
	.loc	1 127 13 is_stmt 0              # axpy_main.c:127:13
	bnez	a0, .LBB0_57
.Ltmp136:
# %bb.56:                               # %.thread297
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- $x24
	#DEBUG_VALUE: main:x_mapped <- undef
	.loc	1 0 13                          # axpy_main.c:0:13
	li	s0, 0
.Ltmp137:
	.loc	1 136 22 is_stmt 1              # axpy_main.c:136:22
	bnez	s10, .LBB0_26
	j	.LBB0_27
.Ltmp138:
.LBB0_57:
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 128 13                        # axpy_main.c:128:13
	lui	a0, %hi(.L.str.9)
	addi	a0, a0, %lo(.L.str.9)
	call	perror
.Ltmp139:
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	li	s8, 1
.Ltmp140:
	#DEBUG_VALUE: main:status <- 1
	j	.LBB0_27
.Ltmp141:
.LBB0_58:
	#DEBUG_VALUE: main:x_iova <- $x26
	#DEBUG_VALUE: main:status <- $x24
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	lui	a0, %hi(.L.str.8)
	addi	a0, a0, %lo(.L.str.8)
.Ltmp142:
.LBB0_59:                               # %.thread282.sink.split
	#DEBUG_VALUE: main:y_iova <- $x23
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	.loc	1 96 10 is_stmt 1               # axpy_main.c:96:10
	snez	s5, s7
.Ltmp143:
	.loc	1 0 0 is_stmt 0                 # axpy_main.c:0:0
	call	perror
.Ltmp144:
	li	s8, 1
	j	.LBB0_55
.Ltmp145:
.LBB0_60:
	#DEBUG_VALUE: i <- $x13
	#DEBUG_VALUE: run <- 0
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	ld	s10, 32(sp)                     # 8-byte Folded Reload
.Ltmp146:
.LBB0_61:                               # %.loopexit330
	#DEBUG_VALUE: i <- $x13
	#DEBUG_VALUE: main:y_attempted <- 1
	#DEBUG_VALUE: main:x_mapped <- 1
	#DEBUG_VALUE: main:x_iova <- [DW_OP_plus_uconst 32] [$x2+0]
	#DEBUG_VALUE: main:x_attempted <- 1
	#DEBUG_VALUE: main:y_dma <- $x19
	#DEBUG_VALUE: main:x_dma <- $x20
	#DEBUG_VALUE: main:expected <- $x27
	#DEBUG_VALUE: main:y_ref <- $x18
	#DEBUG_VALUE: main:x_ref <- $x25
	#DEBUG_VALUE: main:status <- 1
	#DEBUG_VALUE: main:y_mapped <- 0
	.loc	1 34 31 is_stmt 1               # axpy_main.c:34:31
	slli	a0, a3, 2
	add	a1, s3, a0
	flw	ft0, 0(a1)
	.loc	1 34 44 is_stmt 0               # axpy_main.c:34:44
	add	a0, a0, s11
	flw	ft1, 0(a0)
	.loc	1 33 21 is_stmt 1               # axpy_main.c:33:21
	lui	a0, %hi(stderr)
	ld	a0, %lo(stderr)(a0)
	.loc	1 34 31                         # axpy_main.c:34:31
	fcvt.d.s	ft0, ft0
	.loc	1 34 44 is_stmt 0               # axpy_main.c:34:44
	fcvt.d.s	ft1, ft1
	.loc	1 33 13 is_stmt 1               # axpy_main.c:33:13
	fmv.x.d	a4, ft0
	fmv.x.d	a5, ft1
	lui	a1, %hi(.L.str.11)
	addi	a1, a1, %lo(.L.str.11)
	sext.w	a3, a3
.Ltmp147:
	call	fprintf
.Ltmp148:
	.loc	1 0 13 is_stmt 0                # axpy_main.c:0:13
	j	.LBB0_54
.Ltmp149:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	2 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools" "sw/libhero/include/libhero/hero_api.h"
	.file	3 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/rv64-ubuntu-sysroot/usr/include/stdint.h"
	.file	4 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/llvm-minimal/lib/clang/15.0.0/include/stddef.h"
	.file	5 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/rv64-ubuntu-sysroot/usr/include/bits/types.h"
	.file	6 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/rv64-ubuntu-sysroot/usr/include/bits/stdint-uintn.h"
	.file	7 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools" "sw/libhero/include/libhero/ringbuf.h"
	.file	8 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/rv64-ubuntu-sysroot/usr/include/stdio.h"
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"1"
	.size	.L.str, 2

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"0"
	.size	.L.str.1, 2

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"map x (requires accepted platform barriers)"
	.size	.L.str.2, 44

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"map y"
	.size	.L.str.3, 6

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"hot"
	.size	.L.str.4, 4

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"cold"
	.size	.L.str.5, 5

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"cache handoff to device"
	.size	.L.str.6, 24

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"cache handoff to CPU"
	.size	.L.str.7, 21

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.asciz	"unmap y: kernel retains pins until safe revocation"
	.size	.L.str.8, 51

	.type	.L.str.9,@object                # @.str.9
.L.str.9:
	.asciz	"unmap x: kernel retains pins until safe revocation"
	.size	.L.str.9, 51

	.type	.L.str.10,@object               # @.str.10
.L.str.10:
	.asciz	"usage: %s [n divisible by 512] [finite alpha] [1] [0]\n"
	.size	.L.str.10, 55

	.type	.L.str.11,@object               # @.str.11
.L.str.11:
	.asciz	"%s mismatch at %u: got %a expected %a\n"
	.size	.L.str.11, 39

	.type	.L.str.12,@object               # @.str.12
.L.str.12:
	.asciz	"%s consumer verified %u FP32 elements\n"
	.size	.L.str.12, 39

	.section	.debug_loclists,"",@progbits
	.word	.Ldebug_list_header_end0-.Ldebug_list_header_start0 # Length
.Ldebug_list_header_start0:
	.half	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.word	22                              # Offset entry count
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
.Ldebug_loc0:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp3-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp3-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp27-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	89                              # DW_OP_reg9
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp27-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp28-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp34-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	89                              # DW_OP_reg9
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	89                              # DW_OP_reg9
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc1:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp2-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp2-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp27-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	98                              # DW_OP_reg18
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp27-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp29-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp34-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	98                              # DW_OP_reg18
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	98                              # DW_OP_reg18
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc2:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp32-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp47-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp72-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	102                             # DW_OP_reg22
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp72-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp76-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	104                             # DW_OP_reg24
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp76-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp77-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	102                             # DW_OP_reg22
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp91-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	104                             # DW_OP_reg24
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp124-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp127-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	102                             # DW_OP_reg22
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc3:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp63-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp63-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp73-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	103                             # DW_OP_reg23
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp77-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp124-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp126-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp126-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp127-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	103                             # DW_OP_reg23
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp130-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp145-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	103                             # DW_OP_reg23
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc4:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp59-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp59-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp60-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp60-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp77-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	114                             # DW_OP_breg2
	.byte	32                              # 32
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp77-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	114                             # DW_OP_breg2
	.byte	32                              # 32
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp130-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp133-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	106                             # DW_OP_reg26
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp141-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp142-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	106                             # DW_OP_reg26
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp145-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	114                             # DW_OP_breg2
	.byte	32                              # 32
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc5:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp45-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp45-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp85-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	99                              # DW_OP_reg19
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	99                              # DW_OP_reg19
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc6:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp43-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp43-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp85-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	100                             # DW_OP_reg20
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	100                             # DW_OP_reg20
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc7:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp41-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp41-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp85-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	107                             # DW_OP_reg27
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	107                             # DW_OP_reg27
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc8:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp39-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp39-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp85-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	98                              # DW_OP_reg18
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	98                              # DW_OP_reg18
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc9:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp37-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp85-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	105                             # DW_OP_reg25
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	105                             # DW_OP_reg25
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc10:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp61-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp61-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp77-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp77-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp124-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp124-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp126-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp126-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp145-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc11:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp58-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp58-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp77-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp77-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp145-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc12:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp145-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc13:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp61-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp61-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp77-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp77-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp88-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp124-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp124-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp126-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp126-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp145-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc14:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp78-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp86-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp130-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp130-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp133-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	104                             # DW_OP_reg24
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp136-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp138-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	104                             # DW_OP_reg24
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp140-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp141-.Lfunc_begin0         #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp141-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp142-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	104                             # DW_OP_reg24
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp145-.Lfunc_begin0         #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc15:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp16-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	158                             # DW_OP_implicit_value
	.byte	4                               # 4
	.byte	0                               #  
	.byte	0                               #  
	.byte	128                             # €
	.byte	63                              # ?
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp16-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp18-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	42                              # 42
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp18-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp27-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	40                              # 40
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp27-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp31-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	158                             # DW_OP_implicit_value
	.byte	4                               # 4
	.byte	0                               #  
	.byte	0                               #  
	.byte	128                             # €
	.byte	63                              # ?
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc16:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp12-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	16                              # DW_OP_constu
	.byte	128                             # 32768
	.byte	128                             # 
	.byte	2                               # 
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp12-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp27-.Lfunc_begin0          #   ending offset
	.byte	13                              # Loc expr size
	.byte	133                             # DW_OP_breg21
	.byte	0                               # 0
	.byte	168                             # DW_OP_convert
	.asciz	"\253\200\200"                  # 
	.byte	168                             # DW_OP_convert
	.asciz	"\257\200\200"                  # 
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp27-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp29-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	16                              # DW_OP_constu
	.byte	128                             # 32768
	.byte	128                             # 
	.byte	2                               # 
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp34-.Lfunc_begin0          #   ending offset
	.byte	13                              # Loc expr size
	.byte	133                             # DW_OP_breg21
	.byte	0                               # 0
	.byte	168                             # DW_OP_convert
	.asciz	"\253\200\200"                  # 
	.byte	168                             # DW_OP_convert
	.asciz	"\257\200\200"                  # 
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc17:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp5-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp10-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	114                             # DW_OP_breg2
	.byte	40                              # 40
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp10-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp11-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp14-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp19-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	114                             # DW_OP_breg2
	.byte	40                              # 40
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp19-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp20-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc18:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp7-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp9-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp9-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp27-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	101                             # DW_OP_reg21
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp34-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	101                             # DW_OP_reg21
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc19:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp49-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp51-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp51-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp53-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp53-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp57-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	123                             # DW_OP_breg11
	.byte	1                               # 1
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc20:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp64-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp75-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp88-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp102-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp102-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp122-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	49                              # DW_OP_lit1
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp122-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp123-.Lfunc_begin0         #   ending offset
	.byte	2                               # Loc expr size
	.byte	50                              # DW_OP_lit2
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc21:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp95-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp101-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp115-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp121-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp146-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp147-.Lfunc_begin0         #   ending offset
	.byte	1                               # Loc expr size
	.byte	93                              # DW_OP_reg13
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
	.byte	3                               # Abbreviation Code
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
	.byte	4                               # Abbreviation Code
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
	.byte	5                               # Abbreviation Code
	.byte	11                              # DW_TAG_lexical_block
	.byte	1                               # DW_CHILDREN_yes
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	6                               # Abbreviation Code
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
	.byte	7                               # Abbreviation Code
	.byte	15                              # DW_TAG_pointer_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	8                               # Abbreviation Code
	.byte	38                              # DW_TAG_const_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	9                               # Abbreviation Code
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
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	10                              # Abbreviation Code
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
	.byte	11                              # Abbreviation Code
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
	.byte	12                              # Abbreviation Code
	.byte	10                              # DW_TAG_label
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	13                              # Abbreviation Code
	.byte	11                              # DW_TAG_lexical_block
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	14                              # Abbreviation Code
	.byte	11                              # DW_TAG_lexical_block
	.byte	1                               # DW_CHILDREN_yes
	.byte	85                              # DW_AT_ranges
	.byte	35                              # DW_FORM_rnglistx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	15                              # Abbreviation Code
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
	.byte	16                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	17                              # Abbreviation Code
	.byte	72                              # DW_TAG_call_site
	.byte	0                               # DW_CHILDREN_no
	.byte	127                             # DW_AT_call_origin
	.byte	19                              # DW_FORM_ref4
	.byte	125                             # DW_AT_call_return_pc
	.byte	27                              # DW_FORM_addrx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	18                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	5                               # DW_FORM_data2
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
	.byte	19                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	20                              # Abbreviation Code
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
	.byte	21                              # Abbreviation Code
	.byte	19                              # DW_TAG_structure_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	22                              # Abbreviation Code
	.byte	13                              # DW_TAG_member
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	56                              # DW_AT_data_member_location
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	23                              # Abbreviation Code
	.byte	15                              # DW_TAG_pointer_type
	.byte	0                               # DW_CHILDREN_no
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	24                              # Abbreviation Code
	.byte	19                              # DW_TAG_structure_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	25                              # Abbreviation Code
	.byte	1                               # DW_TAG_array_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	26                              # Abbreviation Code
	.byte	33                              # DW_TAG_subrange_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	55                              # DW_AT_count
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	27                              # Abbreviation Code
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
	.byte	28                              # Abbreviation Code
	.byte	53                              # DW_TAG_volatile_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	29                              # Abbreviation Code
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
	.byte	30                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	5                               # DW_FORM_data2
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	60                              # DW_AT_declaration
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	0                               # EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.word	.Ldebug_info_end0-.Ldebug_info_start0 # Length of Unit
.Ldebug_info_start0:
	.half	5                               # DWARF version number
	.byte	1                               # DWARF Unit Type
	.byte	8                               # Address Size (in bytes)
	.word	.debug_abbrev                   # Offset Into Abbrev. Section
	.byte	1                               # Abbrev [1] 0xc:0x3ff DW_TAG_compile_unit
	.byte	0                               # DW_AT_producer
	.half	12                              # DW_AT_language
	.byte	1                               # DW_AT_name
	.word	.Lstr_offsets_base0             # DW_AT_str_offsets_base
	.word	.Lline_table_start0             # DW_AT_stmt_list
	.byte	2                               # DW_AT_comp_dir
	.byte	0                               # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.word	.Laddr_table_base0              # DW_AT_addr_base
	.word	.Lrnglists_table_base0          # DW_AT_rnglists_base
	.word	.Lloclists_table_base0          # DW_AT_loclists_base
	.byte	2                               # Abbrev [2] 0x2b:0x4 DW_TAG_base_type
	.byte	55                              # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	2                               # Abbrev [2] 0x2f:0x4 DW_TAG_base_type
	.byte	54                              # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # Abbrev [2] 0x33:0x4 DW_TAG_base_type
	.byte	3                               # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	2                               # Abbrev [2] 0x37:0x4 DW_TAG_base_type
	.byte	4                               # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # Abbrev [2] 0x3b:0x4 DW_TAG_base_type
	.byte	5                               # DW_AT_name
	.byte	4                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	3                               # Abbrev [3] 0x3f:0x43 DW_TAG_subprogram
	.byte	6                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	130                             # DW_AT_type
                                        # DW_AT_inline
	.byte	4                               # Abbrev [4] 0x47:0x8 DW_TAG_formal_parameter
	.byte	8                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.word	134                             # DW_AT_type
	.byte	4                               # Abbrev [4] 0x4f:0x8 DW_TAG_formal_parameter
	.byte	9                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.word	134                             # DW_AT_type
	.byte	4                               # Abbrev [4] 0x57:0x8 DW_TAG_formal_parameter
	.byte	10                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.word	134                             # DW_AT_type
	.byte	4                               # Abbrev [4] 0x5f:0x8 DW_TAG_formal_parameter
	.byte	11                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.word	134                             # DW_AT_type
	.byte	4                               # Abbrev [4] 0x67:0x8 DW_TAG_formal_parameter
	.byte	12                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.word	55                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0x6f:0x8 DW_TAG_formal_parameter
	.byte	13                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	28                              # DW_AT_decl_line
	.word	144                             # DW_AT_type
	.byte	5                               # Abbrev [5] 0x77:0xa DW_TAG_lexical_block
	.byte	6                               # Abbrev [6] 0x78:0x8 DW_TAG_variable
	.byte	15                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	30                              # DW_AT_decl_line
	.word	55                              # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	2                               # Abbrev [2] 0x82:0x4 DW_TAG_base_type
	.byte	7                               # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	7                               # Abbrev [7] 0x86:0x5 DW_TAG_pointer_type
	.word	139                             # DW_AT_type
	.byte	8                               # Abbrev [8] 0x8b:0x5 DW_TAG_const_type
	.word	59                              # DW_AT_type
	.byte	7                               # Abbrev [7] 0x90:0x5 DW_TAG_pointer_type
	.word	149                             # DW_AT_type
	.byte	8                               # Abbrev [8] 0x95:0x5 DW_TAG_const_type
	.word	154                             # DW_AT_type
	.byte	2                               # Abbrev [2] 0x9a:0x4 DW_TAG_base_type
	.byte	14                              # DW_AT_name
	.byte	8                               # DW_AT_encoding
	.byte	1                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x9e:0x16b DW_TAG_subprogram
	.byte	0                               # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	56                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	130                             # DW_AT_type
                                        # DW_AT_external
	.byte	10                              # Abbrev [10] 0xad:0x9 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_location
	.byte	57                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
	.word	130                             # DW_AT_type
	.byte	10                              # Abbrev [10] 0xb6:0x9 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	58                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
	.word	1020                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xbf:0x9 DW_TAG_variable
	.byte	2                               # DW_AT_location
	.byte	59                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	53                              # DW_AT_decl_line
	.word	55                              # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc8:0x9 DW_TAG_variable
	.byte	3                               # DW_AT_location
	.byte	60                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.word	546                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xd1:0x9 DW_TAG_variable
	.byte	4                               # DW_AT_location
	.byte	61                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.word	546                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xda:0x9 DW_TAG_variable
	.byte	5                               # DW_AT_location
	.byte	62                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	51                              # DW_AT_decl_line
	.word	1025                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xe3:0x9 DW_TAG_variable
	.byte	6                               # DW_AT_location
	.byte	63                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	51                              # DW_AT_decl_line
	.word	1025                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xec:0x9 DW_TAG_variable
	.byte	7                               # DW_AT_location
	.byte	11                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	50                              # DW_AT_decl_line
	.word	1025                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xf5:0x9 DW_TAG_variable
	.byte	8                               # DW_AT_location
	.byte	64                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	50                              # DW_AT_decl_line
	.word	1025                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0xfe:0x9 DW_TAG_variable
	.byte	9                               # DW_AT_location
	.byte	65                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	50                              # DW_AT_decl_line
	.word	1025                            # DW_AT_type
	.byte	11                              # Abbrev [11] 0x107:0x9 DW_TAG_variable
	.byte	10                              # DW_AT_location
	.byte	66                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.word	130                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x110:0x9 DW_TAG_variable
	.byte	11                              # DW_AT_location
	.byte	67                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.word	130                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x119:0x9 DW_TAG_variable
	.byte	12                              # DW_AT_location
	.byte	68                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.word	130                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x122:0x9 DW_TAG_variable
	.byte	13                              # DW_AT_location
	.byte	69                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.word	130                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x12b:0x9 DW_TAG_variable
	.byte	14                              # DW_AT_location
	.byte	70                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.word	130                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x134:0x9 DW_TAG_variable
	.byte	15                              # DW_AT_location
	.byte	71                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	45                              # DW_AT_decl_line
	.word	59                              # DW_AT_type
	.byte	11                              # Abbrev [11] 0x13d:0x9 DW_TAG_variable
	.byte	16                              # DW_AT_location
	.byte	12                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	44                              # DW_AT_decl_line
	.word	55                              # DW_AT_type
	.byte	11                              # Abbrev [11] 0x146:0x9 DW_TAG_variable
	.byte	17                              # DW_AT_location
	.byte	72                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.word	695                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0x14f:0x9 DW_TAG_variable
	.byte	18                              # DW_AT_location
	.byte	73                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	47                              # DW_AT_decl_line
	.word	1030                            # DW_AT_type
	.byte	12                              # Abbrev [12] 0x158:0x5 DW_TAG_label
	.byte	76                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	140                             # DW_AT_decl_line
	.byte	20                              # DW_AT_low_pc
	.byte	12                              # Abbrev [12] 0x15d:0x5 DW_TAG_label
	.byte	77                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	119                             # DW_AT_decl_line
	.byte	21                              # DW_AT_low_pc
	.byte	13                              # Abbrev [13] 0x162:0x10 DW_TAG_lexical_block
	.byte	1                               # DW_AT_low_pc
	.word	.Ltmp57-.Ltmp50                 # DW_AT_high_pc
	.byte	11                              # Abbrev [11] 0x168:0x9 DW_TAG_variable
	.byte	19                              # DW_AT_location
	.byte	15                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	81                              # DW_AT_decl_line
	.word	55                              # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	14                              # Abbrev [14] 0x172:0x2a DW_TAG_lexical_block
	.byte	0                               # DW_AT_ranges
	.byte	11                              # Abbrev [11] 0x174:0x9 DW_TAG_variable
	.byte	20                              # DW_AT_location
	.byte	75                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	99                              # DW_AT_decl_line
	.word	55                              # DW_AT_type
	.byte	14                              # Abbrev [14] 0x17d:0x1e DW_TAG_lexical_block
	.byte	1                               # DW_AT_ranges
	.byte	6                               # Abbrev [6] 0x17f:0x8 DW_TAG_variable
	.byte	13                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	100                             # DW_AT_decl_line
	.word	144                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x187:0x13 DW_TAG_inlined_subroutine
	.word	63                              # DW_AT_abstract_origin
	.byte	2                               # DW_AT_ranges
	.byte	1                               # DW_AT_call_file
	.byte	116                             # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	14                              # Abbrev [14] 0x190:0x9 DW_TAG_lexical_block
	.byte	3                               # DW_AT_ranges
	.byte	16                              # Abbrev [16] 0x192:0x6 DW_TAG_variable
	.byte	21                              # DW_AT_location
	.word	120                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x19c:0x6 DW_TAG_call_site
	.word	521                             # DW_AT_call_origin
	.byte	2                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1a2:0x6 DW_TAG_call_site
	.word	521                             # DW_AT_call_origin
	.byte	3                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1a8:0x6 DW_TAG_call_site
	.word	890                             # DW_AT_call_origin
	.byte	4                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1ae:0x6 DW_TAG_call_site
	.word	890                             # DW_AT_call_origin
	.byte	5                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1b4:0x6 DW_TAG_call_site
	.word	920                             # DW_AT_call_origin
	.byte	6                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1ba:0x6 DW_TAG_call_site
	.word	949                             # DW_AT_call_origin
	.byte	7                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1c0:0x6 DW_TAG_call_site
	.word	949                             # DW_AT_call_origin
	.byte	8                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1c6:0x6 DW_TAG_call_site
	.word	890                             # DW_AT_call_origin
	.byte	9                               # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1cc:0x6 DW_TAG_call_site
	.word	890                             # DW_AT_call_origin
	.byte	10                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1d2:0x6 DW_TAG_call_site
	.word	920                             # DW_AT_call_origin
	.byte	11                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1d8:0x6 DW_TAG_call_site
	.word	949                             # DW_AT_call_origin
	.byte	12                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1de:0x6 DW_TAG_call_site
	.word	949                             # DW_AT_call_origin
	.byte	13                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1e4:0x6 DW_TAG_call_site
	.word	979                             # DW_AT_call_origin
	.byte	14                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1ea:0x6 DW_TAG_call_site
	.word	979                             # DW_AT_call_origin
	.byte	15                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1f0:0x6 DW_TAG_call_site
	.word	990                             # DW_AT_call_origin
	.byte	16                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1f6:0x6 DW_TAG_call_site
	.word	990                             # DW_AT_call_origin
	.byte	17                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x1fc:0x6 DW_TAG_call_site
	.word	979                             # DW_AT_call_origin
	.byte	18                              # DW_AT_call_return_pc
	.byte	17                              # Abbrev [17] 0x202:0x6 DW_TAG_call_site
	.word	979                             # DW_AT_call_origin
	.byte	19                              # DW_AT_call_return_pc
	.byte	0                               # End Of Children Mark
	.byte	18                              # Abbrev [18] 0x209:0x19 DW_TAG_subprogram
	.byte	16                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.half	306                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	546                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	19                              # Abbrev [19] 0x212:0x5 DW_TAG_formal_parameter
	.word	554                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x217:0x5 DW_TAG_formal_parameter
	.word	55                              # DW_AT_type
	.byte	19                              # Abbrev [19] 0x21c:0x5 DW_TAG_formal_parameter
	.word	617                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	20                              # Abbrev [20] 0x222:0x8 DW_TAG_typedef
	.word	51                              # DW_AT_type
	.byte	17                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	90                              # DW_AT_decl_line
	.byte	7                               # Abbrev [7] 0x22a:0x5 DW_TAG_pointer_type
	.word	559                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0x22f:0x8 DW_TAG_typedef
	.word	567                             # DW_AT_type
	.byte	48                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	127                             # DW_AT_decl_line
	.byte	21                              # Abbrev [21] 0x237:0x32 DW_TAG_structure_type
	.byte	200                             # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	121                             # DW_AT_decl_line
	.byte	22                              # Abbrev [22] 0x23b:0x9 DW_TAG_member
	.byte	18                              # DW_AT_name
	.word	617                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	122                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x244:0x9 DW_TAG_member
	.byte	19                              # DW_AT_name
	.word	618                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	123                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x24d:0x9 DW_TAG_member
	.byte	28                              # DW_AT_name
	.word	618                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	124                             # DW_AT_decl_line
	.byte	16                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x256:0x9 DW_TAG_member
	.byte	24                              # DW_AT_name
	.word	705                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	125                             # DW_AT_decl_line
	.byte	24                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x25f:0x9 DW_TAG_member
	.byte	30                              # DW_AT_name
	.word	721                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	126                             # DW_AT_decl_line
	.byte	56                              # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	23                              # Abbrev [23] 0x269:0x1 DW_TAG_pointer_type
	.byte	7                               # Abbrev [7] 0x26a:0x5 DW_TAG_pointer_type
	.word	623                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0x26f:0x8 DW_TAG_typedef
	.word	631                             # DW_AT_type
	.byte	27                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	110                             # DW_AT_decl_line
	.byte	24                              # Abbrev [24] 0x277:0x33 DW_TAG_structure_type
	.byte	26                              # DW_AT_name
	.byte	40                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	104                             # DW_AT_decl_line
	.byte	22                              # Abbrev [22] 0x27c:0x9 DW_TAG_member
	.byte	20                              # DW_AT_name
	.word	682                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	105                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x285:0x9 DW_TAG_member
	.byte	21                              # DW_AT_name
	.word	687                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	106                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x28e:0x9 DW_TAG_member
	.byte	23                              # DW_AT_name
	.word	55                              # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	107                             # DW_AT_decl_line
	.byte	16                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x297:0x9 DW_TAG_member
	.byte	24                              # DW_AT_name
	.word	695                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	108                             # DW_AT_decl_line
	.byte	24                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x2a0:0x9 DW_TAG_member
	.byte	25                              # DW_AT_name
	.word	700                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	109                             # DW_AT_decl_line
	.byte	32                              # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	7                               # Abbrev [7] 0x2aa:0x5 DW_TAG_pointer_type
	.word	55                              # DW_AT_type
	.byte	20                              # Abbrev [20] 0x2af:0x8 DW_TAG_typedef
	.word	51                              # DW_AT_type
	.byte	22                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.byte	7                               # Abbrev [7] 0x2b7:0x5 DW_TAG_pointer_type
	.word	154                             # DW_AT_type
	.byte	7                               # Abbrev [7] 0x2bc:0x5 DW_TAG_pointer_type
	.word	631                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x2c1:0xc DW_TAG_array_type
	.word	154                             # DW_AT_type
	.byte	26                              # Abbrev [26] 0x2c6:0x6 DW_TAG_subrange_type
	.word	717                             # DW_AT_type
	.byte	32                              # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x2cd:0x4 DW_TAG_base_type
	.byte	29                              # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	7                               # DW_AT_encoding
	.byte	20                              # Abbrev [20] 0x2d1:0x8 DW_TAG_typedef
	.word	729                             # DW_AT_type
	.byte	47                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	119                             # DW_AT_decl_line
	.byte	21                              # Abbrev [21] 0x2d9:0x3b DW_TAG_structure_type
	.byte	144                             # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	112                             # DW_AT_decl_line
	.byte	22                              # Abbrev [22] 0x2dd:0x9 DW_TAG_member
	.byte	31                              # DW_AT_name
	.word	788                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	113                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x2e6:0x9 DW_TAG_member
	.byte	42                              # DW_AT_name
	.word	623                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	114                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x2ef:0x9 DW_TAG_member
	.byte	43                              # DW_AT_name
	.word	788                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	115                             # DW_AT_decl_line
	.byte	48                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x2f8:0x9 DW_TAG_member
	.byte	44                              # DW_AT_name
	.word	623                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	116                             # DW_AT_decl_line
	.byte	56                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x301:0x9 DW_TAG_member
	.byte	45                              # DW_AT_name
	.word	788                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	117                             # DW_AT_decl_line
	.byte	96                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x30a:0x9 DW_TAG_member
	.byte	46                              # DW_AT_name
	.word	623                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	118                             # DW_AT_decl_line
	.byte	104                             # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	7                               # Abbrev [7] 0x314:0x5 DW_TAG_pointer_type
	.word	793                             # DW_AT_type
	.byte	28                              # Abbrev [28] 0x319:0x5 DW_TAG_volatile_type
	.word	798                             # DW_AT_type
	.byte	24                              # Abbrev [24] 0x31e:0x3c DW_TAG_structure_type
	.byte	41                              # DW_AT_name
	.byte	32                              # DW_AT_byte_size
	.byte	7                               # DW_AT_decl_file
	.byte	25                              # DW_AT_decl_line
	.byte	22                              # Abbrev [22] 0x323:0x9 DW_TAG_member
	.byte	32                              # DW_AT_name
	.word	858                             # DW_AT_type
	.byte	7                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x32c:0x9 DW_TAG_member
	.byte	23                              # DW_AT_name
	.word	858                             # DW_AT_type
	.byte	7                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.byte	4                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x335:0x9 DW_TAG_member
	.byte	35                              # DW_AT_name
	.word	858                             # DW_AT_type
	.byte	7                               # DW_AT_decl_file
	.byte	28                              # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x33e:0x9 DW_TAG_member
	.byte	36                              # DW_AT_name
	.word	858                             # DW_AT_type
	.byte	7                               # DW_AT_decl_file
	.byte	29                              # DW_AT_decl_line
	.byte	12                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x347:0x9 DW_TAG_member
	.byte	37                              # DW_AT_name
	.word	874                             # DW_AT_type
	.byte	7                               # DW_AT_decl_file
	.byte	30                              # DW_AT_decl_line
	.byte	16                              # DW_AT_data_member_location
	.byte	22                              # Abbrev [22] 0x350:0x9 DW_TAG_member
	.byte	40                              # DW_AT_name
	.word	874                             # DW_AT_type
	.byte	7                               # DW_AT_decl_file
	.byte	31                              # DW_AT_decl_line
	.byte	24                              # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	20                              # Abbrev [20] 0x35a:0x8 DW_TAG_typedef
	.word	866                             # DW_AT_type
	.byte	34                              # DW_AT_name
	.byte	6                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.byte	20                              # Abbrev [20] 0x362:0x8 DW_TAG_typedef
	.word	55                              # DW_AT_type
	.byte	33                              # DW_AT_name
	.byte	5                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
	.byte	20                              # Abbrev [20] 0x36a:0x8 DW_TAG_typedef
	.word	882                             # DW_AT_type
	.byte	39                              # DW_AT_name
	.byte	6                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.byte	20                              # Abbrev [20] 0x372:0x8 DW_TAG_typedef
	.word	51                              # DW_AT_type
	.byte	38                              # DW_AT_name
	.byte	5                               # DW_AT_decl_file
	.byte	45                              # DW_AT_decl_line
	.byte	18                              # Abbrev [18] 0x37a:0x1e DW_TAG_subprogram
	.byte	49                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.half	312                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	130                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	19                              # Abbrev [19] 0x383:0x5 DW_TAG_formal_parameter
	.word	554                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x388:0x5 DW_TAG_formal_parameter
	.word	55                              # DW_AT_type
	.byte	19                              # Abbrev [19] 0x38d:0x5 DW_TAG_formal_parameter
	.word	617                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x392:0x5 DW_TAG_formal_parameter
	.word	546                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	29                              # Abbrev [29] 0x398:0x1d DW_TAG_subprogram
	.byte	50                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	22                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	130                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	19                              # Abbrev [19] 0x3a0:0x5 DW_TAG_formal_parameter
	.word	874                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3a5:0x5 DW_TAG_formal_parameter
	.word	874                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3aa:0x5 DW_TAG_formal_parameter
	.word	59                              # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3af:0x5 DW_TAG_formal_parameter
	.word	858                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	18                              # Abbrev [18] 0x3b5:0x1e DW_TAG_subprogram
	.byte	51                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.half	311                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	130                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	19                              # Abbrev [19] 0x3be:0x5 DW_TAG_formal_parameter
	.word	554                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3c3:0x5 DW_TAG_formal_parameter
	.word	55                              # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3c8:0x5 DW_TAG_formal_parameter
	.word	617                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3cd:0x5 DW_TAG_formal_parameter
	.word	546                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	30                              # Abbrev [30] 0x3d3:0xb DW_TAG_subprogram
	.byte	52                              # DW_AT_name
	.byte	8                               # DW_AT_decl_file
	.half	804                             # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	19                              # Abbrev [19] 0x3d8:0x5 DW_TAG_formal_parameter
	.word	144                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	18                              # Abbrev [18] 0x3de:0x1e DW_TAG_subprogram
	.byte	53                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.half	310                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	130                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	19                              # Abbrev [19] 0x3e7:0x5 DW_TAG_formal_parameter
	.word	554                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3ec:0x5 DW_TAG_formal_parameter
	.word	55                              # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3f1:0x5 DW_TAG_formal_parameter
	.word	617                             # DW_AT_type
	.byte	19                              # Abbrev [19] 0x3f6:0x5 DW_TAG_formal_parameter
	.word	546                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	7                               # Abbrev [7] 0x3fc:0x5 DW_TAG_pointer_type
	.word	695                             # DW_AT_type
	.byte	7                               # Abbrev [7] 0x401:0x5 DW_TAG_pointer_type
	.word	59                              # DW_AT_type
	.byte	2                               # Abbrev [2] 0x406:0x4 DW_TAG_base_type
	.byte	74                              # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_rnglists,"",@progbits
	.word	.Ldebug_list_header_end1-.Ldebug_list_header_start1 # Length
.Ldebug_list_header_start1:
	.half	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.word	4                               # Offset entry count
.Lrnglists_table_base0:
	.word	.Ldebug_ranges0-.Lrnglists_table_base0
	.word	.Ldebug_ranges1-.Lrnglists_table_base0
	.word	.Ldebug_ranges2-.Lrnglists_table_base0
	.word	.Ldebug_ranges3-.Lrnglists_table_base0
.Ldebug_ranges0:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp65-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp75-.Lfunc_begin0          #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp89-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp124-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp127-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp129-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp146-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp149-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges1:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp65-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp75-.Lfunc_begin0          #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp89-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp124-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp127-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp129-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp146-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp149-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges2:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp95-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp102-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp115-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp123-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp146-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp149-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_ranges3:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp95-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp101-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp115-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp121-.Lfunc_begin0         #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp146-.Lfunc_begin0         #   starting offset
	.uleb128 .Ltmp149-.Lfunc_begin0         #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_list_header_end1:
	.section	.debug_str_offsets,"",@progbits
	.word	316                             # Length of String Offsets Set
	.half	5
	.half	0
.Lstr_offsets_base0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)" # string offset=0
.Linfo_string1:
	.asciz	"axpy_main.c"                   # string offset=114
.Linfo_string2:
	.asciz	"/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" # string offset=126
.Linfo_string3:
	.asciz	"unsigned long"                 # string offset=241
.Linfo_string4:
	.asciz	"unsigned int"                  # string offset=255
.Linfo_string5:
	.asciz	"float"                         # string offset=268
.Linfo_string6:
	.asciz	"consume"                       # string offset=274
.Linfo_string7:
	.asciz	"int"                           # string offset=282
.Linfo_string8:
	.asciz	"actual_x"                      # string offset=286
.Linfo_string9:
	.asciz	"actual_y"                      # string offset=295
.Linfo_string10:
	.asciz	"reference_x"                   # string offset=304
.Linfo_string11:
	.asciz	"expected"                      # string offset=316
.Linfo_string12:
	.asciz	"n"                             # string offset=325
.Linfo_string13:
	.asciz	"label"                         # string offset=327
.Linfo_string14:
	.asciz	"char"                          # string offset=333
.Linfo_string15:
	.asciz	"i"                             # string offset=338
.Linfo_string16:
	.asciz	"hero_iommu_map_virt"           # string offset=340
.Linfo_string17:
	.asciz	"uintptr_t"                     # string offset=360
.Linfo_string18:
	.asciz	"dev"                           # string offset=370
.Linfo_string19:
	.asciz	"local_mems"                    # string offset=374
.Linfo_string20:
	.asciz	"v_addr"                        # string offset=385
.Linfo_string21:
	.asciz	"p_addr"                        # string offset=392
.Linfo_string22:
	.asciz	"size_t"                        # string offset=399
.Linfo_string23:
	.asciz	"size"                          # string offset=406
.Linfo_string24:
	.asciz	"alias"                         # string offset=411
.Linfo_string25:
	.asciz	"next"                          # string offset=417
.Linfo_string26:
	.asciz	"HeroSubDev"                    # string offset=422
.Linfo_string27:
	.asciz	"HeroSubDev_t"                  # string offset=433
.Linfo_string28:
	.asciz	"global_mems"                   # string offset=446
.Linfo_string29:
	.asciz	"__ARRAY_SIZE_TYPE__"           # string offset=458
.Linfo_string30:
	.asciz	"mboxes"                        # string offset=478
.Linfo_string31:
	.asciz	"a2h_mbox"                      # string offset=485
.Linfo_string32:
	.asciz	"head"                          # string offset=494
.Linfo_string33:
	.asciz	"__uint32_t"                    # string offset=499
.Linfo_string34:
	.asciz	"uint32_t"                      # string offset=510
.Linfo_string35:
	.asciz	"tail"                          # string offset=519
.Linfo_string36:
	.asciz	"element_size"                  # string offset=524
.Linfo_string37:
	.asciz	"data_v"                        # string offset=537
.Linfo_string38:
	.asciz	"__uint64_t"                    # string offset=544
.Linfo_string39:
	.asciz	"uint64_t"                      # string offset=555
.Linfo_string40:
	.asciz	"data_p"                        # string offset=564
.Linfo_string41:
	.asciz	"ring_buf"                      # string offset=571
.Linfo_string42:
	.asciz	"a2h_mbox_mem"                  # string offset=580
.Linfo_string43:
	.asciz	"h2a_mbox"                      # string offset=593
.Linfo_string44:
	.asciz	"h2a_mbox_mem"                  # string offset=602
.Linfo_string45:
	.asciz	"rb_mbox"                       # string offset=615
.Linfo_string46:
	.asciz	"rb_mbox_mem"                   # string offset=623
.Linfo_string47:
	.asciz	"HeroMboxes_t"                  # string offset=635
.Linfo_string48:
	.asciz	"HeroDev"                       # string offset=648
.Linfo_string49:
	.asciz	"hero_iommu_sync_for_device"    # string offset=656
.Linfo_string50:
	.asciz	"axpy"                          # string offset=683
.Linfo_string51:
	.asciz	"hero_iommu_sync_for_cpu"       # string offset=688
.Linfo_string52:
	.asciz	"perror"                        # string offset=712
.Linfo_string53:
	.asciz	"hero_iommu_unmap_virt"         # string offset=719
.Linfo_string54:
	.asciz	"DW_ATE_unsigned_32"            # string offset=741
.Linfo_string55:
	.asciz	"DW_ATE_unsigned_64"            # string offset=760
.Linfo_string56:
	.asciz	"main"                          # string offset=779
.Linfo_string57:
	.asciz	"argc"                          # string offset=784
.Linfo_string58:
	.asciz	"argv"                          # string offset=789
.Linfo_string59:
	.asciz	"bytes"                         # string offset=794
.Linfo_string60:
	.asciz	"y_iova"                        # string offset=800
.Linfo_string61:
	.asciz	"x_iova"                        # string offset=807
.Linfo_string62:
	.asciz	"y_dma"                         # string offset=814
.Linfo_string63:
	.asciz	"x_dma"                         # string offset=820
.Linfo_string64:
	.asciz	"y_ref"                         # string offset=826
.Linfo_string65:
	.asciz	"x_ref"                         # string offset=832
.Linfo_string66:
	.asciz	"y_attempted"                   # string offset=838
.Linfo_string67:
	.asciz	"x_attempted"                   # string offset=850
.Linfo_string68:
	.asciz	"y_mapped"                      # string offset=862
.Linfo_string69:
	.asciz	"x_mapped"                      # string offset=871
.Linfo_string70:
	.asciz	"status"                        # string offset=880
.Linfo_string71:
	.asciz	"alpha"                         # string offset=887
.Linfo_string72:
	.asciz	"end"                           # string offset=893
.Linfo_string73:
	.asciz	"parsed"                        # string offset=897
.Linfo_string74:
	.asciz	"long"                          # string offset=904
.Linfo_string75:
	.asciz	"run"                           # string offset=909
.Linfo_string76:
	.asciz	"usage"                         # string offset=913
.Linfo_string77:
	.asciz	"cleanup"                       # string offset=919
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
	.section	.debug_addr,"",@progbits
	.word	.Ldebug_addr_end0-.Ldebug_addr_start0 # Length of contribution
.Ldebug_addr_start0:
	.half	5                               # DWARF version number
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
.Laddr_table_base0:
	.quad	.Lfunc_begin0
	.quad	.Ltmp50
	.quad	.Ltmp59
	.quad	.Ltmp62
	.quad	.Ltmp68
	.quad	.Ltmp70
	.quad	.Ltmp74
	.quad	.Ltmp90
	.quad	.Ltmp93
	.quad	.Ltmp105
	.quad	.Ltmp107
	.quad	.Ltmp109
	.quad	.Ltmp111
	.quad	.Ltmp113
	.quad	.Ltmp125
	.quad	.Ltmp129
	.quad	.Ltmp131
	.quad	.Ltmp135
	.quad	.Ltmp139
	.quad	.Ltmp144
	.quad	.Ltmp86
	.quad	.Ltmp130
.Ldebug_addr_end0:
	.ident	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.section	.debug_line,"",@progbits
.Lline_table_start0:
