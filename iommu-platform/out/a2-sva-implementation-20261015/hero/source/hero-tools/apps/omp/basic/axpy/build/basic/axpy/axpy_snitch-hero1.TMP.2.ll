; ModuleID = 'build/basic/axpy/axpy_snitch-hero1.TMP.1.ll'
source_filename = "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c"
target datalayout = "e-m:e-p:32:32-p1:64:32-i64:64-n32-S128-P0-A0"
target triple = "riscv32-hero-hero1-elf"

%struct.__tgt_offload_entry = type { i8*, i8*, i32, i32, i32 }

@l1_buf_x = protected global [2 x [8 x [64 x float]]] zeroinitializer, section ".noinit_l1", align 4, !dbg !0
@l1_buf_y = protected global [2 x [8 x [64 x float]]] zeroinitializer, section ".noinit_l1", align 4, !dbg !32
@dma_timer = protected global i32 0, section ".noinit_l1", align 4, !dbg !40
@dma_time = protected global i32 0, section ".noinit_l1", align 4, !dbg !42
@all_timer = protected global i32 0, section ".noinit_l1", align 4, !dbg !44
@all_time = protected global i32 0, section ".noinit_l1", align 4, !dbg !46
@issue_timer = protected global i32 0, section ".noinit_l1", align 4, !dbg !48
@issue_time = protected global i32 0, section ".noinit_l1", align 4, !dbg !50
@compute_timer = protected global i32 0, section ".noinit_l1", align 4, !dbg !52
@compute_time = protected global i32 0, section ".noinit_l1", align 4, !dbg !54
@.str = private unnamed_addr constant [9 x i8] c"Error!\0A\0D\00", align 1
@.str.1 = private unnamed_addr constant [43 x i8] c"Tot : %u Dma : %u Issue : %u Compute : %u\0A\00", align 1
@barrier_reg = external dso_local global i32, align 4
@.omp_offloading.entry_name = internal unnamed_addr constant [40 x i8] c"__omp_offloading_10303_7120698_axpy_l19\00"
@.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19 = weak local_unnamed_addr constant %struct.__tgt_offload_entry { i8* bitcast (void (i64)* @__omp_offloading_10303_7120698_axpy_l19 to i8*), i8* getelementptr inbounds ([40 x i8], [40 x i8]* @.omp_offloading.entry_name, i32 0, i32 0), i32 0, i32 0, i32 0 }, section "omp_offloading_entries", align 1
@llvm.compiler.used = appending global [16 x i8*] [i8* bitcast (i16 (i64)* @hero_load_uint16 to i8*), i8* bitcast (i32 (i64, i16*)* @hero_load_uint16_noblock to i8*), i8* bitcast (i32 (i64)* @hero_load_uint32 to i8*), i8* bitcast (i32 (i64, i32*)* @hero_load_uint32_noblock to i8*), i8* bitcast (i64 (i64)* @hero_load_uint64 to i8*), i8* bitcast (i32 (i64, i64*)* @hero_load_uint64_noblock to i8*), i8* bitcast (i8 (i64)* @hero_load_uint8 to i8*), i8* bitcast (i32 (i64, i8*)* @hero_load_uint8_noblock to i8*), i8* bitcast (void (i64, i16)* @hero_store_uint16 to i8*), i8* bitcast (i32 (i64, i16)* @hero_store_uint16_noblock to i8*), i8* bitcast (void (i64, i32)* @hero_store_uint32 to i8*), i8* bitcast (i32 (i64, i32)* @hero_store_uint32_noblock to i8*), i8* bitcast (void (i64, i64)* @hero_store_uint64 to i8*), i8* bitcast (i32 (i64, i64)* @hero_store_uint64_noblock to i8*), i8* bitcast (void (i64, i8)* @hero_store_uint8 to i8*), i8* bitcast (i32 (i64, i8)* @hero_store_uint8_noblock to i8*)], section "llvm.metadata"

; Function Attrs: convergent nounwind
define protected void @dev_axpy(i64 noundef %0, i64 noundef %1, float noundef %2, i32 noundef %3) local_unnamed_addr #0 !dbg !65 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !69, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i64 %1, metadata !70, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata float %2, metadata !71, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 %3, metadata !72, metadata !DIExpression()), !dbg !91
  %5 = tail call i32 asm sideeffect "csrr $0, mhartid", "=r"() #5, !dbg !92, !srcloc !101
  call void @llvm.dbg.value(metadata i32 %5, metadata !99, metadata !DIExpression()) #6, !dbg !102
  %6 = add i32 %5, -16, !dbg !103
  call void @llvm.dbg.value(metadata i32 %6, metadata !73, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 0, metadata !75, metadata !DIExpression()), !dbg !91
  %7 = icmp ne i64 %0, 0, !dbg !104
  %8 = icmp ne i64 %1, 0
  %or.cond = and i1 %7, %8, !dbg !106
  %9 = icmp ne i32 %3, 0
  %or.cond3 = and i1 %or.cond, %9, !dbg !106
  %10 = and i32 %3, 511
  %.not = icmp eq i32 %10, 0
  %or.cond67 = and i1 %or.cond3, %.not, !dbg !106
  %11 = icmp eq i32 %6, 0, !dbg !91
  br i1 %or.cond67, label %15, label %12, !dbg !106

12:                                               ; preds = %4
  br i1 %11, label %13, label %160, !dbg !107

13:                                               ; preds = %12
  %14 = tail call i32 (i8 addrspace(1)*, ...) @printf_(i8 addrspace(1)* noundef nonnull inttoptr (i64 ptrtoint ([9 x i8]* @.str to i64) to i8 addrspace(1)*)) #5, !dbg !109
  br label %160, !dbg !109

15:                                               ; preds = %4
  br i1 %11, label %.sink.split, label %16, !dbg !111

16:                                               ; preds = %15
  %17 = icmp eq i32 %6, 8, !dbg !112
  br i1 %17, label %18, label %20, !dbg !114

18:                                               ; preds = %16
  store volatile i32 0, i32* @dma_time, align 4, !dbg !115, !tbaa !117
  store volatile i32 0, i32* @issue_time, align 4, !dbg !121, !tbaa !117
  br label %.sink.split, !dbg !122

.sink.split:                                      ; preds = %18, %15
  %compute_time.sink = phi i32* [ @all_time, %18 ], [ @compute_time, %15 ]
  %19 = xor i1 %11, true, !dbg !91
  store volatile i32 0, i32* %compute_time.sink, align 4, !dbg !91, !tbaa !117
  br label %20, !dbg !123

20:                                               ; preds = %.sink.split, %16
  %21 = phi i1 [ false, %16 ], [ %19, %.sink.split ]
  %22 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !123, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %22, metadata !130, metadata !DIExpression()) #6, !dbg !140
  %23 = and i32 %22, 32, !dbg !141
  %.not.i.i = icmp eq i32 %23, 0, !dbg !141
  br i1 %.not.i.i, label %pulp_barrier.exit, label %24, !dbg !142

24:                                               ; preds = %20
  %25 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !143, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %25, metadata !132, metadata !DIExpression()) #6, !dbg !145
  br label %pulp_barrier.exit, !dbg !146

pulp_barrier.exit:                                ; preds = %24, %20
  %26 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !147, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %26, metadata !137, metadata !DIExpression()) #6, !dbg !149
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !150, !srcloc !151
  br i1 %21, label %27, label %.lr.ph, !dbg !152

27:                                               ; preds = %pulp_barrier.exit
  %28 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !153, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %28, metadata !160, metadata !DIExpression()) #6, !dbg !164
  store volatile i32 %28, i32* @all_timer, align 4, !dbg !165, !tbaa !117
  %29 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !166, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %29, metadata !160, metadata !DIExpression()) #6, !dbg !170
  store volatile i32 %29, i32* @issue_timer, align 4, !dbg !171, !tbaa !117
  call void @llvm.dbg.value(metadata i64 zext (i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32) to i64), metadata !172, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i64 %0, metadata !180, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32), metadata !183, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !189
  %30 = trunc i64 %0 to i32, !dbg !191
  call void @llvm.dbg.value(metadata i32 %30, metadata !185, metadata !DIExpression()) #6, !dbg !189
  %31 = lshr i64 %0, 32, !dbg !192
  %32 = trunc i64 %31 to i32, !dbg !193
  call void @llvm.dbg.value(metadata i32 %32, metadata !186, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !189
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %32, i32 %30) #5, !dbg !194, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32)) #5, !dbg !196, !srcloc !197
  %33 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !198, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %33, metadata !188, metadata !DIExpression()) #6, !dbg !189
  call void @llvm.dbg.value(metadata i64 zext (i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32) to i64), metadata !172, metadata !DIExpression()) #6, !dbg !200
  call void @llvm.dbg.value(metadata i64 %1, metadata !180, metadata !DIExpression()) #6, !dbg !200
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !200
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !200
  call void @llvm.dbg.value(metadata i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32), metadata !183, metadata !DIExpression()) #6, !dbg !200
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !200
  %34 = trunc i64 %1 to i32, !dbg !202
  call void @llvm.dbg.value(metadata i32 %34, metadata !185, metadata !DIExpression()) #6, !dbg !200
  %35 = lshr i64 %1, 32, !dbg !203
  %36 = trunc i64 %35 to i32, !dbg !204
  call void @llvm.dbg.value(metadata i32 %36, metadata !186, metadata !DIExpression()) #6, !dbg !200
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !200
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %36, i32 %34) #5, !dbg !205, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32)) #5, !dbg !206, !srcloc !197
  %37 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !207, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %37, metadata !188, metadata !DIExpression()) #6, !dbg !200
  %38 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !208, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %38, metadata !160, metadata !DIExpression()) #6, !dbg !210
  %39 = load volatile i32, i32* @issue_timer, align 4, !dbg !211, !tbaa !117
  %40 = sub i32 %38, %39, !dbg !212
  %41 = load volatile i32, i32* @issue_time, align 4, !dbg !213, !tbaa !117
  %42 = add i32 %40, %41, !dbg !213
  store volatile i32 %42, i32* @issue_time, align 4, !dbg !213, !tbaa !117
  br label %.lr.ph, !dbg !214

.lr.ph:                                           ; preds = %27, %pulp_barrier.exit
  call void @llvm.dbg.value(metadata i32 0, metadata !76, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 0, metadata !78, metadata !DIExpression()), !dbg !215
  call void @llvm.dbg.value(metadata i32 0, metadata !75, metadata !DIExpression()), !dbg !91
  %43 = add i32 %3, -512
  %umax = call i32 @llvm.umax.i32(i32 %3, i32 512), !dbg !216
  %44 = add i32 %umax, -1, !dbg !216
  %45 = lshr i32 %44, 9, !dbg !216
  br label %46, !dbg !216

46:                                               ; preds = %.backedge, %.lr.ph
  %.090 = phi i32 [ 0, %.lr.ph ], [ %.1, %.backedge ]
  %.06186 = phi i32 [ 0, %.lr.ph ], [ %.06186.be, %.backedge ]
  %.06282 = phi i32 [ 0, %.lr.ph ], [ %.06282.be, %.backedge ]
  call void @llvm.dbg.value(metadata i32 %.090, metadata !75, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 %.06186, metadata !76, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 %.06282, metadata !78, metadata !DIExpression()), !dbg !215
  call void @llvm.dbg.value(metadata !DIArgList(i32 %3, i32 %.06282), metadata !80, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_constu, 512, DW_OP_minus, DW_OP_LLVM_arg, 1, DW_OP_minus, DW_OP_stack_value)), !dbg !217
  br i1 %21, label %47, label %86, !dbg !218

47:                                               ; preds = %46
  call void @llvm.dbg.value(metadata !DIArgList(i32 %43, i32 %.06282), metadata !80, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_LLVM_arg, 1, DW_OP_minus, DW_OP_stack_value)), !dbg !217
  %48 = sub i32 %43, %.06282, !dbg !219
  call void @llvm.dbg.value(metadata i32 %48, metadata !80, metadata !DIExpression()), !dbg !217
  %49 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !220, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %49, metadata !160, metadata !DIExpression()) #6, !dbg !223
  store volatile i32 %49, i32* @dma_timer, align 4, !dbg !224, !tbaa !117
  tail call void asm sideeffect "1: \0A.word (0b0000100 << 25) $|                (  0b00010 << 20) $|                (    0b000 << 12) $|                (      (5) <<  7) $|                (0b0101011 <<  0)   \0Abne t0, zero, 1b \0A", "~{x5}"() #5, !dbg !225, !srcloc !229
  %50 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !230, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %50, metadata !160, metadata !DIExpression()) #6, !dbg !232
  %51 = load volatile i32, i32* @dma_timer, align 4, !dbg !233, !tbaa !117
  %52 = sub i32 %50, %51, !dbg !234
  %53 = load volatile i32, i32* @dma_time, align 4, !dbg !235, !tbaa !117
  %54 = add i32 %52, %53, !dbg !235
  store volatile i32 %54, i32* @dma_time, align 4, !dbg !235, !tbaa !117
  %55 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !236, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %55, metadata !130, metadata !DIExpression()) #6, !dbg !239
  %56 = and i32 %55, 32, !dbg !240
  %.not.i.i68 = icmp eq i32 %56, 0, !dbg !240
  br i1 %.not.i.i68, label %pulp_barrier.exit69, label %57, !dbg !241

57:                                               ; preds = %47
  %58 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !242, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %58, metadata !132, metadata !DIExpression()) #6, !dbg !243
  br label %pulp_barrier.exit69, !dbg !244

pulp_barrier.exit69:                              ; preds = %57, %47
  %59 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !245, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %59, metadata !137, metadata !DIExpression()) #6, !dbg !246
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !247, !srcloc !151
  %60 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !248, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %60, metadata !160, metadata !DIExpression()) #6, !dbg !250
  store volatile i32 %60, i32* @issue_timer, align 4, !dbg !251, !tbaa !117
  %61 = icmp sgt i32 %48, 0, !dbg !252
  br i1 %61, label %62, label %82, !dbg !254

62:                                               ; preds = %pulp_barrier.exit69
  %63 = and i32 %.06186, 1, !dbg !255
  %64 = xor i32 %63, 1, !dbg !255
  %65 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_x, i32 0, i32 %64, i32 0, !dbg !257
  %66 = ptrtoint [64 x float]* %65 to i32, !dbg !258
  %67 = shl i32 %.06282, 2, !dbg !259
  %68 = add i32 %67, 2048, !dbg !259
  %69 = zext i32 %68 to i64, !dbg !260
  %70 = add i64 %69, %0, !dbg !261
  call void @llvm.dbg.value(metadata i32 undef, metadata !172, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !262
  call void @llvm.dbg.value(metadata i64 %70, metadata !180, metadata !DIExpression()) #6, !dbg !262
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !262
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !262
  call void @llvm.dbg.value(metadata [64 x float]* %65, metadata !183, metadata !DIExpression()) #6, !dbg !262
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !262
  %71 = trunc i64 %70 to i32, !dbg !264
  call void @llvm.dbg.value(metadata i32 %71, metadata !185, metadata !DIExpression()) #6, !dbg !262
  %72 = lshr i64 %70, 32, !dbg !265
  %73 = trunc i64 %72 to i32, !dbg !266
  call void @llvm.dbg.value(metadata i32 %73, metadata !186, metadata !DIExpression()) #6, !dbg !262
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !262
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %73, i32 %71) #5, !dbg !267, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 %66) #5, !dbg !268, !srcloc !197
  %74 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !269, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %74, metadata !188, metadata !DIExpression()) #6, !dbg !262
  %75 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %64, i32 0, !dbg !270
  %76 = ptrtoint [64 x float]* %75 to i32, !dbg !271
  %77 = add i64 %69, %1, !dbg !272
  call void @llvm.dbg.value(metadata i32 undef, metadata !172, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !273
  call void @llvm.dbg.value(metadata i64 %77, metadata !180, metadata !DIExpression()) #6, !dbg !273
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !273
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !273
  call void @llvm.dbg.value(metadata [64 x float]* %75, metadata !183, metadata !DIExpression()) #6, !dbg !273
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !273
  %78 = trunc i64 %77 to i32, !dbg !275
  call void @llvm.dbg.value(metadata i32 %78, metadata !185, metadata !DIExpression()) #6, !dbg !273
  %79 = lshr i64 %77, 32, !dbg !276
  %80 = trunc i64 %79 to i32, !dbg !277
  call void @llvm.dbg.value(metadata i32 %80, metadata !186, metadata !DIExpression()) #6, !dbg !273
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !273
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %80, i32 %78) #5, !dbg !278, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 %76) #5, !dbg !279, !srcloc !197
  %81 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !280, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %81, metadata !188, metadata !DIExpression()) #6, !dbg !273
  br label %82, !dbg !281

82:                                               ; preds = %62, %pulp_barrier.exit69
  %83 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !282, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %83, metadata !160, metadata !DIExpression()) #6, !dbg !284
  %84 = load volatile i32, i32* @issue_timer, align 4, !dbg !285, !tbaa !117
  %85 = sub i32 %83, %84, !dbg !286
  call void @llvm.dbg.value(metadata i32 %85, metadata !75, metadata !DIExpression()), !dbg !91
  br label %99, !dbg !287

86:                                               ; preds = %46
  %87 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !288, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %87, metadata !130, metadata !DIExpression()) #6, !dbg !291
  %88 = and i32 %87, 32, !dbg !292
  %.not.i.i70 = icmp eq i32 %88, 0, !dbg !292
  br i1 %.not.i.i70, label %pulp_barrier.exit71, label %89, !dbg !293

89:                                               ; preds = %86
  %90 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !294, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %90, metadata !132, metadata !DIExpression()) #6, !dbg !295
  br label %pulp_barrier.exit71, !dbg !296

pulp_barrier.exit71:                              ; preds = %89, %86
  %91 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !297, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %91, metadata !137, metadata !DIExpression()) #6, !dbg !298
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !299, !srcloc !151
  br i1 %11, label %92, label %94, !dbg !300

92:                                               ; preds = %pulp_barrier.exit71
  %93 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !301, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %93, metadata !160, metadata !DIExpression()) #6, !dbg !304
  store volatile i32 %93, i32* @compute_timer, align 4, !dbg !305, !tbaa !117
  br label %94, !dbg !306

94:                                               ; preds = %92, %pulp_barrier.exit71
  %95 = and i32 %.06186, 1, !dbg !307
  %96 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_x, i32 0, i32 %95, i32 %6, i32 0, !dbg !308
  call void @llvm.dbg.value(metadata float* %96, metadata !83, metadata !DIExpression()), !dbg !309
  %97 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %95, i32 %6, i32 0, !dbg !310
  call void @llvm.dbg.value(metadata float* %97, metadata !87, metadata !DIExpression()), !dbg !309
  %98 = tail call { float*, float* } asm sideeffect "li t0, 0\0A1: flw ft1, 0($0)\0Aflw ft2, 0($1)\0Afmadd.s ft2, $3, ft1, ft2\0Afsw ft2, 0($1)\0Aaddi $0, $0, 4\0Aaddi $1, $1, 4\0Aaddi t0, t0, 1\0Ablt t0, $2, 1b\0A", "=&r,=&r,r,f,0,1,~{f1},~{f2},~{x5},~{memory}"(i32 64, float %2, float* nonnull %96, float* nonnull %97) #5, !dbg !311, !srcloc !312
  call void @llvm.dbg.value(metadata float* undef, metadata !83, metadata !DIExpression()), !dbg !309
  call void @llvm.dbg.value(metadata float* undef, metadata !87, metadata !DIExpression()), !dbg !309
  br label %99

99:                                               ; preds = %94, %82
  %.1 = phi i32 [ %85, %82 ], [ %.090, %94 ], !dbg !91
  call void @llvm.dbg.value(metadata i32 %.1, metadata !75, metadata !DIExpression()), !dbg !91
  %100 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !313, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %100, metadata !130, metadata !DIExpression()) #6, !dbg !316
  %101 = and i32 %100, 32, !dbg !317
  %.not.i.i72 = icmp eq i32 %101, 0, !dbg !317
  br i1 %.not.i.i72, label %pulp_barrier.exit73, label %102, !dbg !318

102:                                              ; preds = %99
  %103 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !319, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %103, metadata !132, metadata !DIExpression()) #6, !dbg !320
  br label %pulp_barrier.exit73, !dbg !321

pulp_barrier.exit73:                              ; preds = %102, %99
  %104 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !322, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %104, metadata !137, metadata !DIExpression()) #6, !dbg !323
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !324, !srcloc !151
  br i1 %11, label %105, label %111, !dbg !325

105:                                              ; preds = %pulp_barrier.exit73
  %106 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !326, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %106, metadata !160, metadata !DIExpression()) #6, !dbg !329
  %107 = load volatile i32, i32* @compute_timer, align 4, !dbg !330, !tbaa !117
  %108 = sub i32 %106, %107, !dbg !331
  %109 = load volatile i32, i32* @compute_time, align 4, !dbg !332, !tbaa !117
  %110 = add i32 %108, %109, !dbg !332
  store volatile i32 %110, i32* @compute_time, align 4, !dbg !332, !tbaa !117
  br label %111, !dbg !333

111:                                              ; preds = %105, %pulp_barrier.exit73
  br i1 %21, label %112, label %.thread91, !dbg !334

112:                                              ; preds = %111
  %113 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !335, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %113, metadata !160, metadata !DIExpression()) #6, !dbg !337
  %114 = load volatile i32, i32* @compute_timer, align 4, !dbg !338, !tbaa !117
  %115 = sub i32 %113, %114, !dbg !339
  call void @llvm.dbg.value(metadata i32 %115, metadata !88, metadata !DIExpression()), !dbg !340
  %116 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !341, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %116, metadata !160, metadata !DIExpression()) #6, !dbg !343
  store volatile i32 %116, i32* @issue_timer, align 4, !dbg !344, !tbaa !117
  %117 = shl i32 %.06282, 2, !dbg !345
  %118 = zext i32 %117 to i64, !dbg !346
  %119 = add i64 %118, %1, !dbg !347
  %120 = and i32 %.06186, 1, !dbg !348
  %121 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %120, !dbg !349
  %122 = ptrtoint [8 x [64 x float]]* %121 to i32, !dbg !350
  call void @llvm.dbg.value(metadata i64 %119, metadata !172, metadata !DIExpression()) #6, !dbg !351
  call void @llvm.dbg.value(metadata i32 undef, metadata !180, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !351
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !351
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !351
  %123 = trunc i64 %119 to i32, !dbg !353
  call void @llvm.dbg.value(metadata i32 %123, metadata !183, metadata !DIExpression()) #6, !dbg !351
  %124 = lshr i64 %119, 32, !dbg !354
  %125 = trunc i64 %124 to i32, !dbg !355
  call void @llvm.dbg.value(metadata i32 %125, metadata !184, metadata !DIExpression()) #6, !dbg !351
  call void @llvm.dbg.value(metadata [8 x [64 x float]]* %121, metadata !185, metadata !DIExpression()) #6, !dbg !351
  call void @llvm.dbg.value(metadata i32 0, metadata !186, metadata !DIExpression()) #6, !dbg !351
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !351
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 0, i32 %122) #5, !dbg !356, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 %125, i32 %123) #5, !dbg !357, !srcloc !197
  %126 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !358, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %126, metadata !188, metadata !DIExpression()) #6, !dbg !351
  %127 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !359, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %127, metadata !160, metadata !DIExpression()) #6, !dbg !361
  %128 = load volatile i32, i32* @issue_timer, align 4, !dbg !362, !tbaa !117
  %129 = tail call i32 @llvm.usub.sat.i32(i32 %.1, i32 %115), !dbg !363
  %130 = load volatile i32, i32* @issue_time, align 4, !dbg !364, !tbaa !117
  %131 = add i32 %127, %129, !dbg !365
  %132 = sub i32 %131, %128, !dbg !366
  %133 = add i32 %132, %130, !dbg !364
  store volatile i32 %133, i32* @issue_time, align 4, !dbg !364, !tbaa !117
  call void @llvm.dbg.value(metadata i32 %.06186, metadata !76, metadata !DIExpression(DW_OP_plus_uconst, 1, DW_OP_stack_value)), !dbg !91
  call void @llvm.dbg.value(metadata i32 %.1, metadata !75, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 %.06282, metadata !78, metadata !DIExpression(DW_OP_plus_uconst, 512, DW_OP_stack_value)), !dbg !215
  %exitcond.not = icmp eq i32 %.06186, %45, !dbg !367
  br i1 %exitcond.not, label %._crit_edge, label %.backedge, !dbg !216

.backedge:                                        ; preds = %.thread91, %112
  %.06282.be = add nuw nsw i32 %.06282, 512, !dbg !368
  %.06186.be = add nuw nsw i32 %.06186, 1, !dbg !369
  br label %46, !dbg !91, !llvm.loop !370

.thread91:                                        ; preds = %111
  call void @llvm.dbg.value(metadata i32 %.06186, metadata !76, metadata !DIExpression(DW_OP_plus_uconst, 1, DW_OP_stack_value)), !dbg !91
  call void @llvm.dbg.value(metadata i32 %.1, metadata !75, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.value(metadata i32 %.06282, metadata !78, metadata !DIExpression(DW_OP_plus_uconst, 512, DW_OP_stack_value)), !dbg !215
  %exitcond.not92 = icmp eq i32 %.06186, %45, !dbg !367
  br i1 %exitcond.not92, label %._crit_edge.thread.loopexit, label %.backedge, !dbg !216

._crit_edge:                                      ; preds = %112
  %134 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !373, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %134, metadata !160, metadata !DIExpression()) #6, !dbg !377
  store volatile i32 %134, i32* @dma_timer, align 4, !dbg !378, !tbaa !117
  tail call void asm sideeffect "1: \0A.word (0b0000100 << 25) $|                (  0b00010 << 20) $|                (    0b000 << 12) $|                (      (5) <<  7) $|                (0b0101011 <<  0)   \0Abne t0, zero, 1b \0A", "~{x5}"() #5, !dbg !379, !srcloc !229
  %135 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !381, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %135, metadata !160, metadata !DIExpression()) #6, !dbg !383
  %136 = load volatile i32, i32* @dma_timer, align 4, !dbg !384, !tbaa !117
  %137 = sub i32 %135, %136, !dbg !385
  %138 = load volatile i32, i32* @dma_time, align 4, !dbg !386, !tbaa !117
  %139 = add i32 %137, %138, !dbg !386
  store volatile i32 %139, i32* @dma_time, align 4, !dbg !386, !tbaa !117
  tail call void asm sideeffect "fence rw, rw", "~{memory}"() #5, !dbg !387, !srcloc !388
  br label %._crit_edge.thread, !dbg !389

._crit_edge.thread.loopexit:                      ; preds = %.thread91
  br label %._crit_edge.thread, !dbg !390

._crit_edge.thread:                               ; preds = %._crit_edge.thread.loopexit, %._crit_edge
  %140 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !390, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %140, metadata !130, metadata !DIExpression()) #6, !dbg !393
  %141 = and i32 %140, 32, !dbg !394
  %.not.i.i74 = icmp eq i32 %141, 0, !dbg !394
  br i1 %.not.i.i74, label %pulp_barrier.exit75, label %142, !dbg !395

142:                                              ; preds = %._crit_edge.thread
  %143 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !396, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %143, metadata !132, metadata !DIExpression()) #6, !dbg !397
  br label %pulp_barrier.exit75, !dbg !398

pulp_barrier.exit75:                              ; preds = %142, %._crit_edge.thread
  %144 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !399, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %144, metadata !137, metadata !DIExpression()) #6, !dbg !400
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !401, !srcloc !151
  br i1 %21, label %145, label %154, !dbg !402

145:                                              ; preds = %pulp_barrier.exit75
  %146 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !403, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %146, metadata !160, metadata !DIExpression()) #6, !dbg !407
  %147 = load volatile i32, i32* @all_timer, align 4, !dbg !408, !tbaa !117
  %148 = sub i32 %146, %147, !dbg !409
  store volatile i32 %148, i32* @all_time, align 4, !dbg !410, !tbaa !117
  %149 = load volatile i32, i32* @all_time, align 4, !dbg !411, !tbaa !117
  %150 = load volatile i32, i32* @dma_time, align 4, !dbg !412, !tbaa !117
  %151 = load volatile i32, i32* @issue_time, align 4, !dbg !413, !tbaa !117
  %152 = load volatile i32, i32* @compute_time, align 4, !dbg !414, !tbaa !117
  %153 = tail call i32 (i8 addrspace(1)*, ...) @printf_(i8 addrspace(1)* noundef nonnull inttoptr (i64 ptrtoint ([43 x i8]* @.str.1 to i64) to i8 addrspace(1)*), i32 noundef %149, i32 noundef %150, i32 noundef %151, i32 noundef %152) #5, !dbg !415
  br label %154, !dbg !416

154:                                              ; preds = %145, %pulp_barrier.exit75
  %155 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !417, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %155, metadata !130, metadata !DIExpression()) #6, !dbg !420
  %156 = and i32 %155, 32, !dbg !421
  %.not.i.i76 = icmp eq i32 %156, 0, !dbg !421
  br i1 %.not.i.i76, label %pulp_barrier.exit77, label %157, !dbg !422

157:                                              ; preds = %154
  %158 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !423, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %158, metadata !132, metadata !DIExpression()) #6, !dbg !424
  br label %pulp_barrier.exit77, !dbg !425

pulp_barrier.exit77:                              ; preds = %157, %154
  %159 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !426, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %159, metadata !137, metadata !DIExpression()) #6, !dbg !427
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !428, !srcloc !151
  br label %160, !dbg !429

160:                                              ; preds = %pulp_barrier.exit77, %13, %12
  ret void, !dbg !429
}

; Function Attrs: alwaysinline convergent norecurse nounwind
define weak void @__wrapped_omp__omp_offloading_10303_7120698_axpy_l19(i64* noundef %0, i64* noundef %1, float* noundef %2, i32* noundef %3) #1 !dbg !430 {
  call void @llvm.dbg.value(metadata i64* %0, metadata !435, metadata !DIExpression()), !dbg !439
  call void @llvm.dbg.value(metadata i64* %1, metadata !436, metadata !DIExpression()), !dbg !439
  call void @llvm.dbg.value(metadata float* %2, metadata !437, metadata !DIExpression()), !dbg !439
  call void @llvm.dbg.value(metadata i32* %3, metadata !438, metadata !DIExpression()), !dbg !439
  %.val = load i64, i64* %0, align 8, !dbg !440, !tbaa !441
  %.val8 = load i64, i64* %1, align 8, !dbg !440, !tbaa !441
  %.val9 = load float, float* %2, align 4, !dbg !440, !tbaa !443
  %.val10 = load i32, i32* %3, align 4, !dbg !440, !tbaa !117
  call void @llvm.dbg.value(metadata i64* undef, metadata !445, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata i64* undef, metadata !448, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata float* undef, metadata !449, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata i32* undef, metadata !450, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata i64 %.val, metadata !453, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i64 %.val8, metadata !456, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata float %.val9, metadata !457, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.val10, metadata !458, metadata !DIExpression()) #6, !dbg !474
  %5 = tail call i32 asm sideeffect "csrr $0, mhartid", "=r"() #5, !dbg !477, !srcloc !101
  call void @llvm.dbg.value(metadata i32 %5, metadata !99, metadata !DIExpression()) #6, !dbg !479
  %6 = add i32 %5, -16, !dbg !480
  call void @llvm.dbg.value(metadata i32 %6, metadata !459, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 0, metadata !460, metadata !DIExpression()) #6, !dbg !474
  %7 = icmp ne i64 %.val, 0, !dbg !481
  %8 = icmp ne i64 %.val8, 0
  %or.cond.i.i = and i1 %7, %8, !dbg !483
  %9 = icmp ne i32 %.val10, 0
  %or.cond3.i.i = and i1 %or.cond.i.i, %9, !dbg !483
  %10 = and i32 %.val10, 511
  %.not.i.i = icmp eq i32 %10, 0
  %or.cond5.i.i = and i1 %or.cond3.i.i, %.not.i.i, !dbg !483
  %11 = icmp eq i32 %6, 0, !dbg !474
  br i1 %or.cond5.i.i, label %15, label %12, !dbg !483

12:                                               ; preds = %4
  br i1 %11, label %13, label %__omp_offloading_10303_7120698_axpy_l19_debug__.exit, !dbg !484

13:                                               ; preds = %12
  %14 = tail call i32 (i8 addrspace(1)*, ...) @printf_(i8 addrspace(1)* noundef nonnull inttoptr (i64 ptrtoint ([9 x i8]* @.str to i64) to i8 addrspace(1)*)) #5, !dbg !486
  br label %__omp_offloading_10303_7120698_axpy_l19_debug__.exit, !dbg !486

15:                                               ; preds = %4
  br i1 %11, label %.sink.split.i.i, label %16, !dbg !488

16:                                               ; preds = %15
  %17 = icmp eq i32 %6, 8, !dbg !489
  br i1 %17, label %18, label %20, !dbg !491

18:                                               ; preds = %16
  store volatile i32 0, i32* @dma_time, align 4, !dbg !492, !tbaa !117
  store volatile i32 0, i32* @issue_time, align 4, !dbg !494, !tbaa !117
  br label %.sink.split.i.i, !dbg !495

.sink.split.i.i:                                  ; preds = %18, %15
  %compute_time.sink.i.i = phi i32* [ @all_time, %18 ], [ @compute_time, %15 ]
  %19 = xor i1 %11, true, !dbg !474
  store volatile i32 0, i32* %compute_time.sink.i.i, align 4, !dbg !474, !tbaa !117
  br label %20, !dbg !496

20:                                               ; preds = %.sink.split.i.i, %16
  %21 = phi i1 [ false, %16 ], [ %19, %.sink.split.i.i ]
  %22 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !496, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %22, metadata !130, metadata !DIExpression()) #6, !dbg !499
  %23 = and i32 %22, 32, !dbg !500
  %.not.i.i.i.i = icmp eq i32 %23, 0, !dbg !500
  br i1 %.not.i.i.i.i, label %pulp_barrier.exit.i.i, label %24, !dbg !501

24:                                               ; preds = %20
  %25 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !502, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %25, metadata !132, metadata !DIExpression()) #6, !dbg !503
  br label %pulp_barrier.exit.i.i, !dbg !504

pulp_barrier.exit.i.i:                            ; preds = %24, %20
  %26 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !505, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %26, metadata !137, metadata !DIExpression()) #6, !dbg !506
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !507, !srcloc !151
  br i1 %21, label %27, label %.lr.ph.i.i, !dbg !508

27:                                               ; preds = %pulp_barrier.exit.i.i
  %28 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !509, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %28, metadata !160, metadata !DIExpression()) #6, !dbg !512
  store volatile i32 %28, i32* @all_timer, align 4, !dbg !513, !tbaa !117
  %29 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !514, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %29, metadata !160, metadata !DIExpression()) #6, !dbg !518
  store volatile i32 %29, i32* @issue_timer, align 4, !dbg !519, !tbaa !117
  call void @llvm.dbg.value(metadata i64 zext (i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32) to i64), metadata !172, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i64 %.val, metadata !180, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32), metadata !183, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !520
  %30 = trunc i64 %.val to i32, !dbg !522
  call void @llvm.dbg.value(metadata i32 %30, metadata !185, metadata !DIExpression()) #6, !dbg !520
  %31 = lshr i64 %.val, 32, !dbg !523
  %32 = trunc i64 %31 to i32, !dbg !524
  call void @llvm.dbg.value(metadata i32 %32, metadata !186, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !520
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %32, i32 %30) #5, !dbg !525, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32)) #5, !dbg !526, !srcloc !197
  %33 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !527, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %33, metadata !188, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i64 zext (i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32) to i64), metadata !172, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i64 %.val8, metadata !180, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32), metadata !183, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !528
  %34 = trunc i64 %.val8 to i32, !dbg !530
  call void @llvm.dbg.value(metadata i32 %34, metadata !185, metadata !DIExpression()) #6, !dbg !528
  %35 = lshr i64 %.val8, 32, !dbg !531
  %36 = trunc i64 %35 to i32, !dbg !532
  call void @llvm.dbg.value(metadata i32 %36, metadata !186, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !528
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %36, i32 %34) #5, !dbg !533, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32)) #5, !dbg !534, !srcloc !197
  %37 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !535, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %37, metadata !188, metadata !DIExpression()) #6, !dbg !528
  %38 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !536, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %38, metadata !160, metadata !DIExpression()) #6, !dbg !538
  %39 = load volatile i32, i32* @issue_timer, align 4, !dbg !539, !tbaa !117
  %40 = sub i32 %38, %39, !dbg !540
  %41 = load volatile i32, i32* @issue_time, align 4, !dbg !541, !tbaa !117
  %42 = add i32 %40, %41, !dbg !541
  store volatile i32 %42, i32* @issue_time, align 4, !dbg !541, !tbaa !117
  br label %.lr.ph.i.i, !dbg !542

.lr.ph.i.i:                                       ; preds = %27, %pulp_barrier.exit.i.i
  call void @llvm.dbg.value(metadata i32 0, metadata !461, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 0, metadata !462, metadata !DIExpression()) #6, !dbg !543
  call void @llvm.dbg.value(metadata i32 0, metadata !460, metadata !DIExpression()) #6, !dbg !474
  %43 = add i32 %.val10, -512
  %umax.i.i = tail call i32 @llvm.umax.i32(i32 %.val10, i32 512) #6, !dbg !544, !range !545
  %44 = add i32 %umax.i.i, -1, !dbg !544
  %45 = lshr i32 %44, 9, !dbg !544
  br label %46, !dbg !544

46:                                               ; preds = %.backedge.i.i, %.lr.ph.i.i
  %.028.i.i = phi i32 [ 0, %.lr.ph.i.i ], [ %.1.i.i, %.backedge.i.i ]
  %.06124.i.i = phi i32 [ 0, %.lr.ph.i.i ], [ %.06124.be.i.i, %.backedge.i.i ]
  %.06220.i.i = phi i32 [ 0, %.lr.ph.i.i ], [ %.06220.be.i.i, %.backedge.i.i ]
  call void @llvm.dbg.value(metadata i32 %.028.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06124.i.i, metadata !461, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06220.i.i, metadata !462, metadata !DIExpression()) #6, !dbg !543
  call void @llvm.dbg.value(metadata !DIArgList(i32 %.val10, i32 %.06220.i.i), metadata !464, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_constu, 512, DW_OP_minus, DW_OP_LLVM_arg, 1, DW_OP_minus, DW_OP_stack_value)) #6, !dbg !546
  br i1 %21, label %47, label %86, !dbg !547

47:                                               ; preds = %46
  call void @llvm.dbg.value(metadata !DIArgList(i32 %43, i32 %.06220.i.i), metadata !464, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_LLVM_arg, 1, DW_OP_minus, DW_OP_stack_value)) #6, !dbg !546
  %48 = sub i32 %43, %.06220.i.i, !dbg !548
  call void @llvm.dbg.value(metadata i32 %48, metadata !464, metadata !DIExpression()) #6, !dbg !546
  %49 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !549, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %49, metadata !160, metadata !DIExpression()) #6, !dbg !552
  store volatile i32 %49, i32* @dma_timer, align 4, !dbg !553, !tbaa !117
  tail call void asm sideeffect "1: \0A.word (0b0000100 << 25) $|                (  0b00010 << 20) $|                (    0b000 << 12) $|                (      (5) <<  7) $|                (0b0101011 <<  0)   \0Abne t0, zero, 1b \0A", "~{x5}"() #5, !dbg !554, !srcloc !229
  %50 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !556, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %50, metadata !160, metadata !DIExpression()) #6, !dbg !558
  %51 = load volatile i32, i32* @dma_timer, align 4, !dbg !559, !tbaa !117
  %52 = sub i32 %50, %51, !dbg !560
  %53 = load volatile i32, i32* @dma_time, align 4, !dbg !561, !tbaa !117
  %54 = add i32 %52, %53, !dbg !561
  store volatile i32 %54, i32* @dma_time, align 4, !dbg !561, !tbaa !117
  %55 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !562, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %55, metadata !130, metadata !DIExpression()) #6, !dbg !565
  %56 = and i32 %55, 32, !dbg !566
  %.not.i.i6.i.i = icmp eq i32 %56, 0, !dbg !566
  br i1 %.not.i.i6.i.i, label %pulp_barrier.exit7.i.i, label %57, !dbg !567

57:                                               ; preds = %47
  %58 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !568, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %58, metadata !132, metadata !DIExpression()) #6, !dbg !569
  br label %pulp_barrier.exit7.i.i, !dbg !570

pulp_barrier.exit7.i.i:                           ; preds = %57, %47
  %59 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !571, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %59, metadata !137, metadata !DIExpression()) #6, !dbg !572
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !573, !srcloc !151
  %60 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !574, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %60, metadata !160, metadata !DIExpression()) #6, !dbg !576
  store volatile i32 %60, i32* @issue_timer, align 4, !dbg !577, !tbaa !117
  %61 = icmp sgt i32 %48, 0, !dbg !578
  br i1 %61, label %62, label %82, !dbg !580

62:                                               ; preds = %pulp_barrier.exit7.i.i
  %63 = and i32 %.06124.i.i, 1, !dbg !581
  %64 = xor i32 %63, 1, !dbg !581
  %65 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_x, i32 0, i32 %64, i32 0, !dbg !583
  %66 = ptrtoint [64 x float]* %65 to i32, !dbg !584
  %67 = shl i32 %.06220.i.i, 2, !dbg !585
  %68 = add i32 %67, 2048, !dbg !585
  %69 = zext i32 %68 to i64, !dbg !586
  %70 = add i64 %.val, %69, !dbg !587
  call void @llvm.dbg.value(metadata i32 undef, metadata !172, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !588
  call void @llvm.dbg.value(metadata i64 %70, metadata !180, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata [64 x float]* %65, metadata !183, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !588
  %71 = trunc i64 %70 to i32, !dbg !590
  call void @llvm.dbg.value(metadata i32 %71, metadata !185, metadata !DIExpression()) #6, !dbg !588
  %72 = lshr i64 %70, 32, !dbg !591
  %73 = trunc i64 %72 to i32, !dbg !592
  call void @llvm.dbg.value(metadata i32 %73, metadata !186, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !588
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %73, i32 %71) #5, !dbg !593, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 %66) #5, !dbg !594, !srcloc !197
  %74 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !595, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %74, metadata !188, metadata !DIExpression()) #6, !dbg !588
  %75 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %64, i32 0, !dbg !596
  %76 = ptrtoint [64 x float]* %75 to i32, !dbg !597
  %77 = add i64 %.val8, %69, !dbg !598
  call void @llvm.dbg.value(metadata i32 undef, metadata !172, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !599
  call void @llvm.dbg.value(metadata i64 %77, metadata !180, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata [64 x float]* %75, metadata !183, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !599
  %78 = trunc i64 %77 to i32, !dbg !601
  call void @llvm.dbg.value(metadata i32 %78, metadata !185, metadata !DIExpression()) #6, !dbg !599
  %79 = lshr i64 %77, 32, !dbg !602
  %80 = trunc i64 %79 to i32, !dbg !603
  call void @llvm.dbg.value(metadata i32 %80, metadata !186, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !599
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %80, i32 %78) #5, !dbg !604, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 %76) #5, !dbg !605, !srcloc !197
  %81 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !606, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %81, metadata !188, metadata !DIExpression()) #6, !dbg !599
  br label %82, !dbg !607

82:                                               ; preds = %62, %pulp_barrier.exit7.i.i
  %83 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !608, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %83, metadata !160, metadata !DIExpression()) #6, !dbg !610
  %84 = load volatile i32, i32* @issue_timer, align 4, !dbg !611, !tbaa !117
  %85 = sub i32 %83, %84, !dbg !612
  call void @llvm.dbg.value(metadata i32 %85, metadata !460, metadata !DIExpression()) #6, !dbg !474
  br label %99, !dbg !613

86:                                               ; preds = %46
  %87 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !614, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %87, metadata !130, metadata !DIExpression()) #6, !dbg !617
  %88 = and i32 %87, 32, !dbg !618
  %.not.i.i8.i.i = icmp eq i32 %88, 0, !dbg !618
  br i1 %.not.i.i8.i.i, label %pulp_barrier.exit9.i.i, label %89, !dbg !619

89:                                               ; preds = %86
  %90 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !620, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %90, metadata !132, metadata !DIExpression()) #6, !dbg !621
  br label %pulp_barrier.exit9.i.i, !dbg !622

pulp_barrier.exit9.i.i:                           ; preds = %89, %86
  %91 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !623, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %91, metadata !137, metadata !DIExpression()) #6, !dbg !624
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !625, !srcloc !151
  br i1 %11, label %92, label %94, !dbg !626

92:                                               ; preds = %pulp_barrier.exit9.i.i
  %93 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !627, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %93, metadata !160, metadata !DIExpression()) #6, !dbg !630
  store volatile i32 %93, i32* @compute_timer, align 4, !dbg !631, !tbaa !117
  br label %94, !dbg !632

94:                                               ; preds = %92, %pulp_barrier.exit9.i.i
  %95 = and i32 %.06124.i.i, 1, !dbg !633
  %96 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_x, i32 0, i32 %95, i32 %6, i32 0, !dbg !634
  call void @llvm.dbg.value(metadata float* %96, metadata !467, metadata !DIExpression()) #6, !dbg !635
  %97 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %95, i32 %6, i32 0, !dbg !636
  call void @llvm.dbg.value(metadata float* %97, metadata !470, metadata !DIExpression()) #6, !dbg !635
  %98 = tail call { float*, float* } asm sideeffect "li t0, 0\0A1: flw ft1, 0($0)\0Aflw ft2, 0($1)\0Afmadd.s ft2, $3, ft1, ft2\0Afsw ft2, 0($1)\0Aaddi $0, $0, 4\0Aaddi $1, $1, 4\0Aaddi t0, t0, 1\0Ablt t0, $2, 1b\0A", "=&r,=&r,r,f,0,1,~{f1},~{f2},~{x5},~{memory}"(i32 64, float %.val9, float* nonnull %96, float* nonnull %97) #5, !dbg !637, !srcloc !312
  call void @llvm.dbg.value(metadata float* undef, metadata !467, metadata !DIExpression()) #6, !dbg !635
  call void @llvm.dbg.value(metadata float* undef, metadata !470, metadata !DIExpression()) #6, !dbg !635
  br label %99

99:                                               ; preds = %94, %82
  %.1.i.i = phi i32 [ %85, %82 ], [ %.028.i.i, %94 ], !dbg !474
  call void @llvm.dbg.value(metadata i32 %.1.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  %100 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !638, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %100, metadata !130, metadata !DIExpression()) #6, !dbg !641
  %101 = and i32 %100, 32, !dbg !642
  %.not.i.i10.i.i = icmp eq i32 %101, 0, !dbg !642
  br i1 %.not.i.i10.i.i, label %pulp_barrier.exit11.i.i, label %102, !dbg !643

102:                                              ; preds = %99
  %103 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !644, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %103, metadata !132, metadata !DIExpression()) #6, !dbg !645
  br label %pulp_barrier.exit11.i.i, !dbg !646

pulp_barrier.exit11.i.i:                          ; preds = %102, %99
  %104 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !647, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %104, metadata !137, metadata !DIExpression()) #6, !dbg !648
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !649, !srcloc !151
  br i1 %11, label %105, label %111, !dbg !650

105:                                              ; preds = %pulp_barrier.exit11.i.i
  %106 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !651, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %106, metadata !160, metadata !DIExpression()) #6, !dbg !654
  %107 = load volatile i32, i32* @compute_timer, align 4, !dbg !655, !tbaa !117
  %108 = sub i32 %106, %107, !dbg !656
  %109 = load volatile i32, i32* @compute_time, align 4, !dbg !657, !tbaa !117
  %110 = add i32 %108, %109, !dbg !657
  store volatile i32 %110, i32* @compute_time, align 4, !dbg !657, !tbaa !117
  br label %111, !dbg !658

111:                                              ; preds = %105, %pulp_barrier.exit11.i.i
  br i1 %21, label %112, label %.thread29.i.i, !dbg !659

112:                                              ; preds = %111
  %113 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !660, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %113, metadata !160, metadata !DIExpression()) #6, !dbg !662
  %114 = load volatile i32, i32* @compute_timer, align 4, !dbg !663, !tbaa !117
  %115 = sub i32 %113, %114, !dbg !664
  call void @llvm.dbg.value(metadata i32 %115, metadata !471, metadata !DIExpression()) #6, !dbg !665
  %116 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !666, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %116, metadata !160, metadata !DIExpression()) #6, !dbg !668
  store volatile i32 %116, i32* @issue_timer, align 4, !dbg !669, !tbaa !117
  %117 = shl i32 %.06220.i.i, 2, !dbg !670
  %118 = zext i32 %117 to i64, !dbg !671
  %119 = add i64 %.val8, %118, !dbg !672
  %120 = and i32 %.06124.i.i, 1, !dbg !673
  %121 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %120, !dbg !674
  %122 = ptrtoint [8 x [64 x float]]* %121 to i32, !dbg !675
  call void @llvm.dbg.value(metadata i64 %119, metadata !172, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 undef, metadata !180, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !676
  %123 = trunc i64 %119 to i32, !dbg !678
  call void @llvm.dbg.value(metadata i32 %123, metadata !183, metadata !DIExpression()) #6, !dbg !676
  %124 = lshr i64 %119, 32, !dbg !679
  %125 = trunc i64 %124 to i32, !dbg !680
  call void @llvm.dbg.value(metadata i32 %125, metadata !184, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata [8 x [64 x float]]* %121, metadata !185, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 0, metadata !186, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !676
  tail call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 0, i32 %122) #5, !dbg !681, !srcloc !195
  tail call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 %125, i32 %123) #5, !dbg !682, !srcloc !197
  %126 = tail call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !683, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %126, metadata !188, metadata !DIExpression()) #6, !dbg !676
  %127 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !684, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %127, metadata !160, metadata !DIExpression()) #6, !dbg !686
  %128 = load volatile i32, i32* @issue_timer, align 4, !dbg !687, !tbaa !117
  %129 = tail call i32 @llvm.usub.sat.i32(i32 %.1.i.i, i32 %115) #6, !dbg !688
  %130 = load volatile i32, i32* @issue_time, align 4, !dbg !689, !tbaa !117
  %131 = add i32 %127, %129, !dbg !690
  %132 = sub i32 %131, %128, !dbg !691
  %133 = add i32 %132, %130, !dbg !689
  store volatile i32 %133, i32* @issue_time, align 4, !dbg !689, !tbaa !117
  call void @llvm.dbg.value(metadata i32 %.06124.i.i, metadata !461, metadata !DIExpression(DW_OP_plus_uconst, 1, DW_OP_stack_value)) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.1.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06220.i.i, metadata !462, metadata !DIExpression(DW_OP_plus_uconst, 512, DW_OP_stack_value)) #6, !dbg !543
  %exitcond.not.i.i = icmp eq i32 %.06124.i.i, %45, !dbg !692
  br i1 %exitcond.not.i.i, label %._crit_edge.i.i, label %.backedge.i.i, !dbg !544

.backedge.i.i:                                    ; preds = %.thread29.i.i, %112
  %.06220.be.i.i = add nuw nsw i32 %.06220.i.i, 512, !dbg !693
  %.06124.be.i.i = add nuw nsw i32 %.06124.i.i, 1, !dbg !694
  br label %46, !dbg !474, !llvm.loop !695

.thread29.i.i:                                    ; preds = %111
  call void @llvm.dbg.value(metadata i32 %.06124.i.i, metadata !461, metadata !DIExpression(DW_OP_plus_uconst, 1, DW_OP_stack_value)) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.1.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06220.i.i, metadata !462, metadata !DIExpression(DW_OP_plus_uconst, 512, DW_OP_stack_value)) #6, !dbg !543
  %exitcond.not30.i.i = icmp eq i32 %.06124.i.i, %45, !dbg !692
  br i1 %exitcond.not30.i.i, label %._crit_edge.thread.i.i.loopexit, label %.backedge.i.i, !dbg !544

._crit_edge.i.i:                                  ; preds = %112
  %134 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !697, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %134, metadata !160, metadata !DIExpression()) #6, !dbg !701
  store volatile i32 %134, i32* @dma_timer, align 4, !dbg !702, !tbaa !117
  tail call void asm sideeffect "1: \0A.word (0b0000100 << 25) $|                (  0b00010 << 20) $|                (    0b000 << 12) $|                (      (5) <<  7) $|                (0b0101011 <<  0)   \0Abne t0, zero, 1b \0A", "~{x5}"() #5, !dbg !703, !srcloc !229
  %135 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !705, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %135, metadata !160, metadata !DIExpression()) #6, !dbg !707
  %136 = load volatile i32, i32* @dma_timer, align 4, !dbg !708, !tbaa !117
  %137 = sub i32 %135, %136, !dbg !709
  %138 = load volatile i32, i32* @dma_time, align 4, !dbg !710, !tbaa !117
  %139 = add i32 %137, %138, !dbg !710
  store volatile i32 %139, i32* @dma_time, align 4, !dbg !710, !tbaa !117
  tail call void asm sideeffect "fence rw, rw", "~{memory}"() #5, !dbg !711, !srcloc !388
  br label %._crit_edge.thread.i.i, !dbg !712

._crit_edge.thread.i.i.loopexit:                  ; preds = %.thread29.i.i
  br label %._crit_edge.thread.i.i, !dbg !713

._crit_edge.thread.i.i:                           ; preds = %._crit_edge.thread.i.i.loopexit, %._crit_edge.i.i
  %140 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !713, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %140, metadata !130, metadata !DIExpression()) #6, !dbg !716
  %141 = and i32 %140, 32, !dbg !717
  %.not.i.i12.i.i = icmp eq i32 %141, 0, !dbg !717
  br i1 %.not.i.i12.i.i, label %pulp_barrier.exit13.i.i, label %142, !dbg !718

142:                                              ; preds = %._crit_edge.thread.i.i
  %143 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !719, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %143, metadata !132, metadata !DIExpression()) #6, !dbg !720
  br label %pulp_barrier.exit13.i.i, !dbg !721

pulp_barrier.exit13.i.i:                          ; preds = %142, %._crit_edge.thread.i.i
  %144 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !722, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %144, metadata !137, metadata !DIExpression()) #6, !dbg !723
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !724, !srcloc !151
  br i1 %21, label %145, label %154, !dbg !725

145:                                              ; preds = %pulp_barrier.exit13.i.i
  %146 = tail call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !726, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %146, metadata !160, metadata !DIExpression()) #6, !dbg !730
  %147 = load volatile i32, i32* @all_timer, align 4, !dbg !731, !tbaa !117
  %148 = sub i32 %146, %147, !dbg !732
  store volatile i32 %148, i32* @all_time, align 4, !dbg !733, !tbaa !117
  %149 = load volatile i32, i32* @all_time, align 4, !dbg !734, !tbaa !117
  %150 = load volatile i32, i32* @dma_time, align 4, !dbg !735, !tbaa !117
  %151 = load volatile i32, i32* @issue_time, align 4, !dbg !736, !tbaa !117
  %152 = load volatile i32, i32* @compute_time, align 4, !dbg !737, !tbaa !117
  %153 = tail call i32 (i8 addrspace(1)*, ...) @printf_(i8 addrspace(1)* noundef nonnull inttoptr (i64 ptrtoint ([43 x i8]* @.str.1 to i64) to i8 addrspace(1)*), i32 noundef %149, i32 noundef %150, i32 noundef %151, i32 noundef %152) #5, !dbg !738
  br label %154, !dbg !739

154:                                              ; preds = %145, %pulp_barrier.exit13.i.i
  %155 = tail call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !740, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %155, metadata !130, metadata !DIExpression()) #6, !dbg !743
  %156 = and i32 %155, 32, !dbg !744
  %.not.i.i14.i.i = icmp eq i32 %156, 0, !dbg !744
  br i1 %.not.i.i14.i.i, label %pulp_barrier.exit15.i.i, label %157, !dbg !745

157:                                              ; preds = %154
  %158 = tail call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !746, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %158, metadata !132, metadata !DIExpression()) #6, !dbg !747
  br label %pulp_barrier.exit15.i.i, !dbg !748

pulp_barrier.exit15.i.i:                          ; preds = %157, %154
  %159 = tail call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !749, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %159, metadata !137, metadata !DIExpression()) #6, !dbg !750
  tail call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !751, !srcloc !151
  br label %__omp_offloading_10303_7120698_axpy_l19_debug__.exit, !dbg !752

__omp_offloading_10303_7120698_axpy_l19_debug__.exit: ; preds = %pulp_barrier.exit15.i.i, %13, %12
  ret void, !dbg !440
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_load_uint32_noblock(i64 noundef %0, i32* nocapture noundef writeonly %1) #2 !dbg !753 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !760, metadata !DIExpression()), !dbg !767
  call void @llvm.dbg.value(metadata i32* %1, metadata !761, metadata !DIExpression()), !dbg !767
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()), !dbg !773
  %3 = lshr i64 %0, 32, !dbg !775
  %4 = trunc i64 %3 to i32, !dbg !776
  call void @llvm.dbg.value(metadata i32 %4, metadata !762, metadata !DIExpression()), !dbg !767
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()), !dbg !780
  %5 = trunc i64 %0 to i32, !dbg !782
  %6 = inttoptr i32 %5 to i32*, !dbg !783
  call void @llvm.dbg.value(metadata i32* %6, metadata !763, metadata !DIExpression()), !dbg !767
  %7 = tail call { i32, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lw $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %4, i32* %6, i32 undef) #5, !dbg !784, !srcloc !785
  %8 = extractvalue { i32, i32 } %7, 0, !dbg !784
  call void @llvm.dbg.value(metadata i32 %8, metadata !766, metadata !DIExpression()), !dbg !767
  call void @llvm.dbg.value(metadata i32 undef, metadata !765, metadata !DIExpression()), !dbg !767
  store i32 %8, i32* %1, align 4, !dbg !786, !tbaa !117
  ret i32 0, !dbg !787
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_load_uint32(i64 noundef %0) #2 !dbg !788 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !790, metadata !DIExpression()), !dbg !794
  call void @llvm.dbg.value(metadata i64 %0, metadata !760, metadata !DIExpression()) #6, !dbg !795
  call void @llvm.dbg.value(metadata i32* undef, metadata !761, metadata !DIExpression()) #6, !dbg !795
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !797
  %2 = lshr i64 %0, 32, !dbg !799
  %3 = trunc i64 %2 to i32, !dbg !800
  call void @llvm.dbg.value(metadata i32 %3, metadata !762, metadata !DIExpression()) #6, !dbg !795
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !801
  %4 = trunc i64 %0 to i32, !dbg !803
  %5 = inttoptr i32 %4 to i32*, !dbg !804
  call void @llvm.dbg.value(metadata i32* %5, metadata !763, metadata !DIExpression()) #6, !dbg !795
  %6 = tail call { i32, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lw $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %3, i32* %5, i32 undef) #5, !dbg !805, !srcloc !785
  %7 = extractvalue { i32, i32 } %6, 0, !dbg !805
  call void @llvm.dbg.value(metadata i32 %7, metadata !766, metadata !DIExpression()) #6, !dbg !795
  call void @llvm.dbg.value(metadata i32 undef, metadata !765, metadata !DIExpression()) #6, !dbg !795
  call void @llvm.dbg.value(metadata i32 0, metadata !792, metadata !DIExpression()), !dbg !794
  call void @llvm.dbg.value(metadata i32 undef, metadata !791, metadata !DIExpression()), !dbg !794
  ret i32 %7, !dbg !806
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_store_uint32_noblock(i64 noundef %0, i32 noundef %1) #2 !dbg !807 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !811, metadata !DIExpression()), !dbg !816
  call void @llvm.dbg.value(metadata i32 %1, metadata !812, metadata !DIExpression()), !dbg !816
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()), !dbg !817
  %3 = lshr i64 %0, 32, !dbg !819
  %4 = trunc i64 %3 to i32, !dbg !820
  call void @llvm.dbg.value(metadata i32 %4, metadata !813, metadata !DIExpression()), !dbg !816
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()), !dbg !821
  %5 = trunc i64 %0 to i32, !dbg !823
  %6 = inttoptr i32 %5 to i32*, !dbg !824
  call void @llvm.dbg.value(metadata i32* %6, metadata !814, metadata !DIExpression()), !dbg !816
  %7 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sw $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %4, i32 %1, i32* %6, i32 undef) #5, !dbg !825, !srcloc !826
  call void @llvm.dbg.value(metadata i32 %7, metadata !815, metadata !DIExpression()), !dbg !816
  ret i32 0, !dbg !827
}

; Function Attrs: convergent inlinehint nounwind
define internal void @hero_store_uint32(i64 noundef %0, i32 noundef %1) #2 !dbg !828 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !832, metadata !DIExpression()), !dbg !835
  call void @llvm.dbg.value(metadata i32 %1, metadata !833, metadata !DIExpression()), !dbg !835
  call void @llvm.dbg.value(metadata i64 %0, metadata !811, metadata !DIExpression()) #6, !dbg !836
  call void @llvm.dbg.value(metadata i32 %1, metadata !812, metadata !DIExpression()) #6, !dbg !836
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !838
  %3 = lshr i64 %0, 32, !dbg !840
  %4 = trunc i64 %3 to i32, !dbg !841
  call void @llvm.dbg.value(metadata i32 %4, metadata !813, metadata !DIExpression()) #6, !dbg !836
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !842
  %5 = trunc i64 %0 to i32, !dbg !844
  %6 = inttoptr i32 %5 to i32*, !dbg !845
  call void @llvm.dbg.value(metadata i32* %6, metadata !814, metadata !DIExpression()) #6, !dbg !836
  %7 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sw $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %4, i32 %1, i32* %6, i32 undef) #5, !dbg !846, !srcloc !826
  call void @llvm.dbg.value(metadata i32 %7, metadata !815, metadata !DIExpression()) #6, !dbg !836
  call void @llvm.dbg.value(metadata i32 0, metadata !834, metadata !DIExpression()), !dbg !835
  ret void, !dbg !847
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_load_uint16_noblock(i64 noundef %0, i16* nocapture noundef writeonly %1) #2 !dbg !848 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !853, metadata !DIExpression()), !dbg !860
  call void @llvm.dbg.value(metadata i16* %1, metadata !854, metadata !DIExpression()), !dbg !860
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()), !dbg !861
  %3 = lshr i64 %0, 32, !dbg !863
  %4 = trunc i64 %3 to i32, !dbg !864
  call void @llvm.dbg.value(metadata i32 %4, metadata !855, metadata !DIExpression()), !dbg !860
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()), !dbg !865
  %5 = trunc i64 %0 to i32, !dbg !867
  %6 = inttoptr i32 %5 to i16*, !dbg !868
  call void @llvm.dbg.value(metadata i16* %6, metadata !856, metadata !DIExpression()), !dbg !860
  %7 = tail call { i16, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lhu $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %4, i16* %6, i32 undef) #5, !dbg !869, !srcloc !870
  %8 = extractvalue { i16, i32 } %7, 0, !dbg !869
  call void @llvm.dbg.value(metadata i16 %8, metadata !859, metadata !DIExpression()), !dbg !860
  call void @llvm.dbg.value(metadata i32 undef, metadata !858, metadata !DIExpression()), !dbg !860
  store i16 %8, i16* %1, align 2, !dbg !871, !tbaa !872
  ret i32 0, !dbg !874
}

; Function Attrs: convergent inlinehint nounwind
define internal zeroext i16 @hero_load_uint16(i64 noundef %0) #2 !dbg !875 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !879, metadata !DIExpression()), !dbg !882
  call void @llvm.dbg.value(metadata i64 %0, metadata !853, metadata !DIExpression()) #6, !dbg !883
  call void @llvm.dbg.value(metadata i16* undef, metadata !854, metadata !DIExpression()) #6, !dbg !883
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !885
  %2 = lshr i64 %0, 32, !dbg !887
  %3 = trunc i64 %2 to i32, !dbg !888
  call void @llvm.dbg.value(metadata i32 %3, metadata !855, metadata !DIExpression()) #6, !dbg !883
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !889
  %4 = trunc i64 %0 to i32, !dbg !891
  %5 = inttoptr i32 %4 to i16*, !dbg !892
  call void @llvm.dbg.value(metadata i16* %5, metadata !856, metadata !DIExpression()) #6, !dbg !883
  %6 = tail call { i16, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lhu $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %3, i16* %5, i32 undef) #5, !dbg !893, !srcloc !870
  %7 = extractvalue { i16, i32 } %6, 0, !dbg !893
  call void @llvm.dbg.value(metadata i16 %7, metadata !859, metadata !DIExpression()) #6, !dbg !883
  call void @llvm.dbg.value(metadata i32 undef, metadata !858, metadata !DIExpression()) #6, !dbg !883
  call void @llvm.dbg.value(metadata i32 0, metadata !881, metadata !DIExpression()), !dbg !882
  call void @llvm.dbg.value(metadata i16 undef, metadata !880, metadata !DIExpression()), !dbg !882
  ret i16 %7, !dbg !894
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_store_uint16_noblock(i64 noundef %0, i16 noundef zeroext %1) #2 !dbg !895 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !900, metadata !DIExpression()), !dbg !905
  call void @llvm.dbg.value(metadata i16 %1, metadata !901, metadata !DIExpression()), !dbg !905
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()), !dbg !906
  %3 = lshr i64 %0, 32, !dbg !908
  %4 = trunc i64 %3 to i32, !dbg !909
  call void @llvm.dbg.value(metadata i32 %4, metadata !902, metadata !DIExpression()), !dbg !905
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()), !dbg !910
  %5 = trunc i64 %0 to i32, !dbg !912
  %6 = inttoptr i32 %5 to i16*, !dbg !913
  call void @llvm.dbg.value(metadata i16* %6, metadata !903, metadata !DIExpression()), !dbg !905
  %7 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sh $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %4, i16 %1, i16* %6, i32 undef) #5, !dbg !914, !srcloc !915
  call void @llvm.dbg.value(metadata i32 %7, metadata !904, metadata !DIExpression()), !dbg !905
  ret i32 0, !dbg !916
}

; Function Attrs: convergent inlinehint nounwind
define internal void @hero_store_uint16(i64 noundef %0, i16 noundef zeroext %1) #2 !dbg !917 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !921, metadata !DIExpression()), !dbg !924
  call void @llvm.dbg.value(metadata i16 %1, metadata !922, metadata !DIExpression()), !dbg !924
  call void @llvm.dbg.value(metadata i64 %0, metadata !900, metadata !DIExpression()) #6, !dbg !925
  call void @llvm.dbg.value(metadata i16 %1, metadata !901, metadata !DIExpression()) #6, !dbg !925
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !927
  %3 = lshr i64 %0, 32, !dbg !929
  %4 = trunc i64 %3 to i32, !dbg !930
  call void @llvm.dbg.value(metadata i32 %4, metadata !902, metadata !DIExpression()) #6, !dbg !925
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !931
  %5 = trunc i64 %0 to i32, !dbg !933
  %6 = inttoptr i32 %5 to i16*, !dbg !934
  call void @llvm.dbg.value(metadata i16* %6, metadata !903, metadata !DIExpression()) #6, !dbg !925
  %7 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sh $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %4, i16 %1, i16* %6, i32 undef) #5, !dbg !935, !srcloc !915
  call void @llvm.dbg.value(metadata i32 %7, metadata !904, metadata !DIExpression()) #6, !dbg !925
  call void @llvm.dbg.value(metadata i32 0, metadata !923, metadata !DIExpression()), !dbg !924
  ret void, !dbg !936
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_load_uint8_noblock(i64 noundef %0, i8* nocapture noundef writeonly %1) #2 !dbg !937 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !942, metadata !DIExpression()), !dbg !949
  call void @llvm.dbg.value(metadata i8* %1, metadata !943, metadata !DIExpression()), !dbg !949
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()), !dbg !950
  %3 = lshr i64 %0, 32, !dbg !952
  %4 = trunc i64 %3 to i32, !dbg !953
  call void @llvm.dbg.value(metadata i32 %4, metadata !944, metadata !DIExpression()), !dbg !949
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()), !dbg !954
  %5 = trunc i64 %0 to i32, !dbg !956
  %6 = inttoptr i32 %5 to i8*, !dbg !957
  call void @llvm.dbg.value(metadata i8* %6, metadata !945, metadata !DIExpression()), !dbg !949
  %7 = tail call { i8, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lbu $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %4, i8* %6, i32 undef) #5, !dbg !958, !srcloc !959
  %8 = extractvalue { i8, i32 } %7, 0, !dbg !958
  call void @llvm.dbg.value(metadata i8 %8, metadata !948, metadata !DIExpression()), !dbg !949
  call void @llvm.dbg.value(metadata i32 undef, metadata !947, metadata !DIExpression()), !dbg !949
  store i8 %8, i8* %1, align 1, !dbg !960, !tbaa !961
  ret i32 0, !dbg !962
}

; Function Attrs: convergent inlinehint nounwind
define internal zeroext i8 @hero_load_uint8(i64 noundef %0) #2 !dbg !963 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !967, metadata !DIExpression()), !dbg !970
  call void @llvm.dbg.value(metadata i64 %0, metadata !942, metadata !DIExpression()) #6, !dbg !971
  call void @llvm.dbg.value(metadata i8* undef, metadata !943, metadata !DIExpression()) #6, !dbg !971
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !973
  %2 = lshr i64 %0, 32, !dbg !975
  %3 = trunc i64 %2 to i32, !dbg !976
  call void @llvm.dbg.value(metadata i32 %3, metadata !944, metadata !DIExpression()) #6, !dbg !971
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !977
  %4 = trunc i64 %0 to i32, !dbg !979
  %5 = inttoptr i32 %4 to i8*, !dbg !980
  call void @llvm.dbg.value(metadata i8* %5, metadata !945, metadata !DIExpression()) #6, !dbg !971
  %6 = tail call { i8, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lbu $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %3, i8* %5, i32 undef) #5, !dbg !981, !srcloc !959
  %7 = extractvalue { i8, i32 } %6, 0, !dbg !981
  call void @llvm.dbg.value(metadata i8 %7, metadata !948, metadata !DIExpression()) #6, !dbg !971
  call void @llvm.dbg.value(metadata i32 undef, metadata !947, metadata !DIExpression()) #6, !dbg !971
  call void @llvm.dbg.value(metadata i32 0, metadata !969, metadata !DIExpression()), !dbg !970
  call void @llvm.dbg.value(metadata i8 undef, metadata !968, metadata !DIExpression()), !dbg !970
  ret i8 %7, !dbg !982
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_store_uint8_noblock(i64 noundef %0, i8 noundef zeroext %1) #2 !dbg !983 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !988, metadata !DIExpression()), !dbg !993
  call void @llvm.dbg.value(metadata i8 %1, metadata !989, metadata !DIExpression()), !dbg !993
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()), !dbg !994
  %3 = lshr i64 %0, 32, !dbg !996
  %4 = trunc i64 %3 to i32, !dbg !997
  call void @llvm.dbg.value(metadata i32 %4, metadata !990, metadata !DIExpression()), !dbg !993
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()), !dbg !998
  %5 = trunc i64 %0 to i32, !dbg !1000
  %6 = inttoptr i32 %5 to i8*, !dbg !1001
  call void @llvm.dbg.value(metadata i8* %6, metadata !991, metadata !DIExpression()), !dbg !993
  %7 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sb $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %4, i8 %1, i8* %6, i32 undef) #5, !dbg !1002, !srcloc !1003
  call void @llvm.dbg.value(metadata i32 %7, metadata !992, metadata !DIExpression()), !dbg !993
  ret i32 0, !dbg !1004
}

; Function Attrs: convergent inlinehint nounwind
define internal void @hero_store_uint8(i64 noundef %0, i8 noundef zeroext %1) #2 !dbg !1005 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !1009, metadata !DIExpression()), !dbg !1012
  call void @llvm.dbg.value(metadata i8 %1, metadata !1010, metadata !DIExpression()), !dbg !1012
  call void @llvm.dbg.value(metadata i64 %0, metadata !988, metadata !DIExpression()) #6, !dbg !1013
  call void @llvm.dbg.value(metadata i8 %1, metadata !989, metadata !DIExpression()) #6, !dbg !1013
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !1015
  %3 = lshr i64 %0, 32, !dbg !1017
  %4 = trunc i64 %3 to i32, !dbg !1018
  call void @llvm.dbg.value(metadata i32 %4, metadata !990, metadata !DIExpression()) #6, !dbg !1013
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !1019
  %5 = trunc i64 %0 to i32, !dbg !1021
  %6 = inttoptr i32 %5 to i8*, !dbg !1022
  call void @llvm.dbg.value(metadata i8* %6, metadata !991, metadata !DIExpression()) #6, !dbg !1013
  %7 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sb $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %4, i8 %1, i8* %6, i32 undef) #5, !dbg !1023, !srcloc !1003
  call void @llvm.dbg.value(metadata i32 %7, metadata !992, metadata !DIExpression()) #6, !dbg !1013
  call void @llvm.dbg.value(metadata i32 0, metadata !1011, metadata !DIExpression()), !dbg !1012
  ret void, !dbg !1024
}

; Function Attrs: convergent inlinehint nounwind
define internal i64 @hero_load_uint64(i64 noundef %0) #2 !dbg !1025 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !1029, metadata !DIExpression()), !dbg !1033
  call void @llvm.dbg.value(metadata i64 %0, metadata !790, metadata !DIExpression()) #6, !dbg !1034
  call void @llvm.dbg.value(metadata i64 %0, metadata !760, metadata !DIExpression()) #6, !dbg !1036
  call void @llvm.dbg.value(metadata i32* undef, metadata !761, metadata !DIExpression()) #6, !dbg !1036
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !1038
  %2 = lshr i64 %0, 32, !dbg !1040
  %3 = trunc i64 %2 to i32, !dbg !1041
  call void @llvm.dbg.value(metadata i32 %3, metadata !762, metadata !DIExpression()) #6, !dbg !1036
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !1042
  %4 = trunc i64 %0 to i32, !dbg !1044
  %5 = inttoptr i32 %4 to i32*, !dbg !1045
  call void @llvm.dbg.value(metadata i32* %5, metadata !763, metadata !DIExpression()) #6, !dbg !1036
  %6 = tail call { i32, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lw $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %3, i32* %5, i32 undef) #5, !dbg !1046, !srcloc !785
  %7 = extractvalue { i32, i32 } %6, 0, !dbg !1046
  call void @llvm.dbg.value(metadata i32 %7, metadata !766, metadata !DIExpression()) #6, !dbg !1036
  call void @llvm.dbg.value(metadata i32 undef, metadata !765, metadata !DIExpression()) #6, !dbg !1036
  call void @llvm.dbg.value(metadata i32 0, metadata !792, metadata !DIExpression()) #6, !dbg !1034
  call void @llvm.dbg.value(metadata i32 undef, metadata !791, metadata !DIExpression()) #6, !dbg !1034
  call void @llvm.dbg.value(metadata i32 %7, metadata !1030, metadata !DIExpression()), !dbg !1033
  %8 = add i64 %0, 4, !dbg !1047
  call void @llvm.dbg.value(metadata i64 %8, metadata !790, metadata !DIExpression()) #6, !dbg !1048
  call void @llvm.dbg.value(metadata i64 %8, metadata !760, metadata !DIExpression()) #6, !dbg !1050
  call void @llvm.dbg.value(metadata i32* undef, metadata !761, metadata !DIExpression()) #6, !dbg !1050
  call void @llvm.dbg.value(metadata i64 %8, metadata !768, metadata !DIExpression()) #6, !dbg !1052
  %9 = lshr i64 %8, 32, !dbg !1054
  %10 = trunc i64 %9 to i32, !dbg !1055
  call void @llvm.dbg.value(metadata i32 %10, metadata !762, metadata !DIExpression()) #6, !dbg !1050
  call void @llvm.dbg.value(metadata i64 %8, metadata !777, metadata !DIExpression()) #6, !dbg !1056
  %11 = trunc i64 %8 to i32, !dbg !1058
  %12 = inttoptr i32 %11 to i32*, !dbg !1059
  call void @llvm.dbg.value(metadata i32* %12, metadata !763, metadata !DIExpression()) #6, !dbg !1050
  %13 = tail call { i32, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lw $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %10, i32* %12, i32 undef) #5, !dbg !1060, !srcloc !785
  %14 = extractvalue { i32, i32 } %13, 0, !dbg !1060
  call void @llvm.dbg.value(metadata i32 %14, metadata !766, metadata !DIExpression()) #6, !dbg !1050
  call void @llvm.dbg.value(metadata i32 undef, metadata !765, metadata !DIExpression()) #6, !dbg !1050
  call void @llvm.dbg.value(metadata i32 0, metadata !792, metadata !DIExpression()) #6, !dbg !1048
  call void @llvm.dbg.value(metadata i32 undef, metadata !791, metadata !DIExpression()) #6, !dbg !1048
  call void @llvm.dbg.value(metadata i32 %14, metadata !1031, metadata !DIExpression()), !dbg !1033
  %15 = zext i32 %14 to i64, !dbg !1061
  %16 = shl nuw i64 %15, 32, !dbg !1062
  %17 = zext i32 %7 to i64, !dbg !1063
  %18 = or i64 %16, %17, !dbg !1064
  call void @llvm.dbg.value(metadata i64 %18, metadata !1032, metadata !DIExpression()), !dbg !1033
  ret i64 %18, !dbg !1065
}

; Function Attrs: convergent inlinehint nounwind
define internal void @hero_store_uint64(i64 noundef %0, i64 noundef %1) #2 !dbg !1066 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !1070, metadata !DIExpression()), !dbg !1074
  call void @llvm.dbg.value(metadata i64 %1, metadata !1071, metadata !DIExpression()), !dbg !1074
  %3 = trunc i64 %1 to i32, !dbg !1075
  call void @llvm.dbg.value(metadata i32 %3, metadata !1072, metadata !DIExpression()), !dbg !1074
  call void @llvm.dbg.value(metadata i64 %0, metadata !832, metadata !DIExpression()) #6, !dbg !1076
  call void @llvm.dbg.value(metadata i32 %3, metadata !833, metadata !DIExpression()) #6, !dbg !1076
  call void @llvm.dbg.value(metadata i64 %0, metadata !811, metadata !DIExpression()) #6, !dbg !1078
  call void @llvm.dbg.value(metadata i32 %3, metadata !812, metadata !DIExpression()) #6, !dbg !1078
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !1080
  %4 = lshr i64 %0, 32, !dbg !1082
  %5 = trunc i64 %4 to i32, !dbg !1083
  call void @llvm.dbg.value(metadata i32 %5, metadata !813, metadata !DIExpression()) #6, !dbg !1078
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !1084
  %6 = trunc i64 %0 to i32, !dbg !1086
  %7 = inttoptr i32 %6 to i32*, !dbg !1087
  call void @llvm.dbg.value(metadata i32* %7, metadata !814, metadata !DIExpression()) #6, !dbg !1078
  %8 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sw $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %5, i32 %3, i32* %7, i32 undef) #5, !dbg !1088, !srcloc !826
  call void @llvm.dbg.value(metadata i32 %8, metadata !815, metadata !DIExpression()) #6, !dbg !1078
  call void @llvm.dbg.value(metadata i32 0, metadata !834, metadata !DIExpression()) #6, !dbg !1076
  %9 = lshr i64 %1, 32, !dbg !1089
  %10 = trunc i64 %9 to i32, !dbg !1090
  call void @llvm.dbg.value(metadata i32 %10, metadata !1073, metadata !DIExpression()), !dbg !1074
  %11 = add i64 %0, 4, !dbg !1091
  call void @llvm.dbg.value(metadata i64 %11, metadata !832, metadata !DIExpression()) #6, !dbg !1092
  call void @llvm.dbg.value(metadata i32 %10, metadata !833, metadata !DIExpression()) #6, !dbg !1092
  call void @llvm.dbg.value(metadata i64 %11, metadata !811, metadata !DIExpression()) #6, !dbg !1094
  call void @llvm.dbg.value(metadata i32 %10, metadata !812, metadata !DIExpression()) #6, !dbg !1094
  call void @llvm.dbg.value(metadata i64 %11, metadata !768, metadata !DIExpression()) #6, !dbg !1096
  %12 = lshr i64 %11, 32, !dbg !1098
  %13 = trunc i64 %12 to i32, !dbg !1099
  call void @llvm.dbg.value(metadata i32 %13, metadata !813, metadata !DIExpression()) #6, !dbg !1094
  call void @llvm.dbg.value(metadata i64 %11, metadata !777, metadata !DIExpression()) #6, !dbg !1100
  %14 = trunc i64 %11 to i32, !dbg !1102
  %15 = inttoptr i32 %14 to i32*, !dbg !1103
  call void @llvm.dbg.value(metadata i32* %15, metadata !814, metadata !DIExpression()) #6, !dbg !1094
  %16 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sw $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %13, i32 %10, i32* %15, i32 undef) #5, !dbg !1104, !srcloc !826
  call void @llvm.dbg.value(metadata i32 %16, metadata !815, metadata !DIExpression()) #6, !dbg !1094
  call void @llvm.dbg.value(metadata i32 0, metadata !834, metadata !DIExpression()) #6, !dbg !1092
  ret void, !dbg !1105
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_load_uint64_noblock(i64 noundef %0, i64* nocapture noundef writeonly %1) #2 !dbg !1106 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !1111, metadata !DIExpression()), !dbg !1117
  call void @llvm.dbg.value(metadata i64* %1, metadata !1112, metadata !DIExpression()), !dbg !1117
  %3 = bitcast i64* %1 to i32*, !dbg !1118
  call void @llvm.dbg.value(metadata i32* %3, metadata !1113, metadata !DIExpression()), !dbg !1117
  call void @llvm.dbg.value(metadata i64 %0, metadata !760, metadata !DIExpression()) #6, !dbg !1119
  call void @llvm.dbg.value(metadata i32* %3, metadata !761, metadata !DIExpression()) #6, !dbg !1119
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !1121
  %4 = lshr i64 %0, 32, !dbg !1123
  %5 = trunc i64 %4 to i32, !dbg !1124
  call void @llvm.dbg.value(metadata i32 %5, metadata !762, metadata !DIExpression()) #6, !dbg !1119
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !1125
  %6 = trunc i64 %0 to i32, !dbg !1127
  %7 = inttoptr i32 %6 to i32*, !dbg !1128
  call void @llvm.dbg.value(metadata i32* %7, metadata !763, metadata !DIExpression()) #6, !dbg !1119
  %8 = tail call { i32, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lw $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %5, i32* %7, i32 undef) #5, !dbg !1129, !srcloc !785
  %9 = extractvalue { i32, i32 } %8, 0, !dbg !1129
  call void @llvm.dbg.value(metadata i32 %9, metadata !766, metadata !DIExpression()) #6, !dbg !1119
  call void @llvm.dbg.value(metadata i32 undef, metadata !765, metadata !DIExpression()) #6, !dbg !1119
  store i32 %9, i32* %3, align 4, !dbg !1130, !tbaa !117
  call void @llvm.dbg.value(metadata i32 0, metadata !1114, metadata !DIExpression()), !dbg !1117
  %10 = getelementptr inbounds i32, i32* %3, i32 1, !dbg !1131
  call void @llvm.dbg.value(metadata i32* %10, metadata !1115, metadata !DIExpression()), !dbg !1117
  %11 = add i64 %0, 4, !dbg !1132
  call void @llvm.dbg.value(metadata i64 %11, metadata !760, metadata !DIExpression()) #6, !dbg !1133
  call void @llvm.dbg.value(metadata i32* %10, metadata !761, metadata !DIExpression()) #6, !dbg !1133
  call void @llvm.dbg.value(metadata i64 %11, metadata !768, metadata !DIExpression()) #6, !dbg !1135
  %12 = lshr i64 %11, 32, !dbg !1137
  %13 = trunc i64 %12 to i32, !dbg !1138
  call void @llvm.dbg.value(metadata i32 %13, metadata !762, metadata !DIExpression()) #6, !dbg !1133
  call void @llvm.dbg.value(metadata i64 %11, metadata !777, metadata !DIExpression()) #6, !dbg !1139
  %14 = trunc i64 %11 to i32, !dbg !1141
  %15 = inttoptr i32 %14 to i32*, !dbg !1142
  call void @llvm.dbg.value(metadata i32* %15, metadata !763, metadata !DIExpression()) #6, !dbg !1133
  %16 = tail call { i32, i32 } asm sideeffect "csrrci $1, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09lw $0, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $1", "=&r,=&r,r,r,1,~{memory}"(i32 %13, i32* %15, i32 undef) #5, !dbg !1143, !srcloc !785
  %17 = extractvalue { i32, i32 } %16, 0, !dbg !1143
  call void @llvm.dbg.value(metadata i32 %17, metadata !766, metadata !DIExpression()) #6, !dbg !1133
  call void @llvm.dbg.value(metadata i32 undef, metadata !765, metadata !DIExpression()) #6, !dbg !1133
  store i32 %17, i32* %10, align 4, !dbg !1144, !tbaa !117
  call void @llvm.dbg.value(metadata i32 0, metadata !1116, metadata !DIExpression()), !dbg !1117
  ret i32 0, !dbg !1145
}

; Function Attrs: convergent inlinehint nounwind
define internal i32 @hero_store_uint64_noblock(i64 noundef %0, i64 noundef %1) #2 !dbg !1146 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !1150, metadata !DIExpression()), !dbg !1156
  call void @llvm.dbg.value(metadata i64 %1, metadata !1151, metadata !DIExpression()), !dbg !1156
  %3 = trunc i64 %1 to i32, !dbg !1157
  call void @llvm.dbg.value(metadata i32 %3, metadata !1152, metadata !DIExpression()), !dbg !1156
  call void @llvm.dbg.value(metadata i64 %0, metadata !811, metadata !DIExpression()) #6, !dbg !1158
  call void @llvm.dbg.value(metadata i32 %3, metadata !812, metadata !DIExpression()) #6, !dbg !1158
  call void @llvm.dbg.value(metadata i64 %0, metadata !768, metadata !DIExpression()) #6, !dbg !1160
  %4 = lshr i64 %0, 32, !dbg !1162
  %5 = trunc i64 %4 to i32, !dbg !1163
  call void @llvm.dbg.value(metadata i32 %5, metadata !813, metadata !DIExpression()) #6, !dbg !1158
  call void @llvm.dbg.value(metadata i64 %0, metadata !777, metadata !DIExpression()) #6, !dbg !1164
  %6 = trunc i64 %0 to i32, !dbg !1166
  %7 = inttoptr i32 %6 to i32*, !dbg !1167
  call void @llvm.dbg.value(metadata i32* %7, metadata !814, metadata !DIExpression()) #6, !dbg !1158
  %8 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sw $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %5, i32 %3, i32* %7, i32 undef) #5, !dbg !1168, !srcloc !826
  call void @llvm.dbg.value(metadata i32 %8, metadata !815, metadata !DIExpression()) #6, !dbg !1158
  call void @llvm.dbg.value(metadata i32 0, metadata !1153, metadata !DIExpression()), !dbg !1156
  %9 = lshr i64 %1, 32, !dbg !1169
  %10 = trunc i64 %9 to i32, !dbg !1170
  call void @llvm.dbg.value(metadata i32 %10, metadata !1154, metadata !DIExpression()), !dbg !1156
  %11 = add i64 %0, 4, !dbg !1171
  call void @llvm.dbg.value(metadata i64 %11, metadata !811, metadata !DIExpression()) #6, !dbg !1172
  call void @llvm.dbg.value(metadata i32 %10, metadata !812, metadata !DIExpression()) #6, !dbg !1172
  call void @llvm.dbg.value(metadata i64 %11, metadata !768, metadata !DIExpression()) #6, !dbg !1174
  %12 = lshr i64 %11, 32, !dbg !1176
  %13 = trunc i64 %12 to i32, !dbg !1177
  call void @llvm.dbg.value(metadata i32 %13, metadata !813, metadata !DIExpression()) #6, !dbg !1172
  call void @llvm.dbg.value(metadata i64 %11, metadata !777, metadata !DIExpression()) #6, !dbg !1178
  %14 = trunc i64 %11 to i32, !dbg !1180
  %15 = inttoptr i32 %14 to i32*, !dbg !1181
  call void @llvm.dbg.value(metadata i32* %15, metadata !814, metadata !DIExpression()) #6, !dbg !1172
  %16 = tail call i32 asm sideeffect "csrrci $0, 0x300, 3\0A\09csrw 0xbc0, zero\0A\09sw $2, 0($3)\0A\09csrw 0xbc0, zero\0A\09csrrw x0, 0x300, $0", "=&r,r,r,r,0,~{memory}"(i32 %13, i32 %10, i32* %15, i32 undef) #5, !dbg !1182, !srcloc !826
  call void @llvm.dbg.value(metadata i32 %16, metadata !815, metadata !DIExpression()) #6, !dbg !1172
  call void @llvm.dbg.value(metadata i32 0, metadata !1155, metadata !DIExpression()), !dbg !1156
  ret i32 0, !dbg !1183
}

; Function Attrs: convergent
declare !dbg !1184 dso_local i32 @printf_(i8 addrspace(1)* noundef, ...) local_unnamed_addr #3

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #4

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare i32 @llvm.usub.sat.i32(i32, i32) #4

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare i32 @llvm.umax.i32(i32, i32) #4

define weak void @__omp_offloading_10303_7120698_axpy_l19(i64 %0) {
entry:
  %1 = inttoptr i64 %0 to i64 addrspace(1)*
  %2 = call i32 @hero_load_uint32(i64 %0)
  %3 = inttoptr i32 %2 to i64*
  %4 = getelementptr i64, i64 addrspace(1)* %1, i64 1
  %5 = ptrtoint i64 addrspace(1)* %4 to i64
  %6 = call i32 @hero_load_uint32(i64 %5)
  %7 = inttoptr i32 %6 to i64*
  %8 = getelementptr i64, i64 addrspace(1)* %1, i64 2
  %9 = ptrtoint i64 addrspace(1)* %8 to i64
  %10 = call i32 @hero_load_uint32(i64 %9)
  %11 = inttoptr i32 %10 to float*
  %12 = getelementptr i64, i64 addrspace(1)* %1, i64 3
  %13 = ptrtoint i64 addrspace(1)* %12 to i64
  %14 = call i32 @hero_load_uint32(i64 %13)
  %15 = inttoptr i32 %14 to i32*
  call void @llvm.dbg.value(metadata i64* %3, metadata !435, metadata !DIExpression()) #6, !dbg !439
  call void @llvm.dbg.value(metadata i64* %7, metadata !436, metadata !DIExpression()) #6, !dbg !439
  call void @llvm.dbg.value(metadata float* %11, metadata !437, metadata !DIExpression()) #6, !dbg !439
  call void @llvm.dbg.value(metadata i32* %15, metadata !438, metadata !DIExpression()) #6, !dbg !439
  %.val.i = load i64, i64* %3, align 8, !dbg !440, !tbaa !441
  %.val8.i = load i64, i64* %7, align 8, !dbg !440, !tbaa !441
  %.val9.i = load float, float* %11, align 4, !dbg !440, !tbaa !443
  %.val10.i = load i32, i32* %15, align 4, !dbg !440, !tbaa !117
  call void @llvm.dbg.value(metadata i64* undef, metadata !445, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata i64* undef, metadata !448, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata float* undef, metadata !449, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata i32* undef, metadata !450, metadata !DIExpression()) #6, !dbg !451
  call void @llvm.dbg.value(metadata i64 %.val.i, metadata !453, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i64 %.val8.i, metadata !456, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata float %.val9.i, metadata !457, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.val10.i, metadata !458, metadata !DIExpression()) #6, !dbg !474
  %16 = call i32 asm sideeffect "csrr $0, mhartid", "=r"() #5, !dbg !477, !srcloc !101
  call void @llvm.dbg.value(metadata i32 %16, metadata !99, metadata !DIExpression()) #6, !dbg !479
  %17 = add i32 %16, -16, !dbg !480
  call void @llvm.dbg.value(metadata i32 %17, metadata !459, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 0, metadata !460, metadata !DIExpression()) #6, !dbg !474
  %18 = icmp ne i64 %.val.i, 0, !dbg !481
  %19 = icmp ne i64 %.val8.i, 0
  %or.cond.i.i.i = and i1 %18, %19, !dbg !483
  %20 = icmp ne i32 %.val10.i, 0
  %or.cond3.i.i.i = and i1 %or.cond.i.i.i, %20, !dbg !483
  %21 = and i32 %.val10.i, 511
  %.not.i.i.i = icmp eq i32 %21, 0
  %or.cond5.i.i.i = and i1 %or.cond3.i.i.i, %.not.i.i.i, !dbg !483
  %22 = icmp eq i32 %17, 0, !dbg !474
  br i1 %or.cond5.i.i.i, label %26, label %23, !dbg !483

23:                                               ; preds = %entry
  br i1 %22, label %24, label %__wrapped_omp__omp_offloading_10303_7120698_axpy_l19.exit, !dbg !484

24:                                               ; preds = %23
  %25 = call i32 (i8 addrspace(1)*, ...) @printf_(i8 addrspace(1)* noundef nonnull inttoptr (i64 ptrtoint ([9 x i8]* @.str to i64) to i8 addrspace(1)*)) #5, !dbg !486
  br label %__wrapped_omp__omp_offloading_10303_7120698_axpy_l19.exit, !dbg !486

26:                                               ; preds = %entry
  br i1 %22, label %.sink.split.i.i.i, label %27, !dbg !488

27:                                               ; preds = %26
  %28 = icmp eq i32 %17, 8, !dbg !489
  br i1 %28, label %29, label %31, !dbg !491

29:                                               ; preds = %27
  store volatile i32 0, i32* @dma_time, align 4, !dbg !492, !tbaa !117
  store volatile i32 0, i32* @issue_time, align 4, !dbg !494, !tbaa !117
  br label %.sink.split.i.i.i, !dbg !495

.sink.split.i.i.i:                                ; preds = %29, %26
  %compute_time.sink.i.i.i = phi i32* [ @all_time, %29 ], [ @compute_time, %26 ]
  %30 = xor i1 %22, true, !dbg !474
  store volatile i32 0, i32* %compute_time.sink.i.i.i, align 4, !dbg !474, !tbaa !117
  br label %31, !dbg !496

31:                                               ; preds = %.sink.split.i.i.i, %27
  %32 = phi i1 [ false, %27 ], [ %30, %.sink.split.i.i.i ]
  %33 = call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !496, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %33, metadata !130, metadata !DIExpression()) #6, !dbg !499
  %34 = and i32 %33, 32, !dbg !500
  %.not.i.i.i.i.i = icmp eq i32 %34, 0, !dbg !500
  br i1 %.not.i.i.i.i.i, label %pulp_barrier.exit.i.i.i, label %35, !dbg !501

35:                                               ; preds = %31
  %36 = call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !502, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %36, metadata !132, metadata !DIExpression()) #6, !dbg !503
  br label %pulp_barrier.exit.i.i.i, !dbg !504

pulp_barrier.exit.i.i.i:                          ; preds = %35, %31
  %37 = call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !505, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %37, metadata !137, metadata !DIExpression()) #6, !dbg !506
  call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !507, !srcloc !151
  br i1 %32, label %38, label %.lr.ph.i.i.i, !dbg !508

38:                                               ; preds = %pulp_barrier.exit.i.i.i
  %39 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !509, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %39, metadata !160, metadata !DIExpression()) #6, !dbg !512
  store volatile i32 %39, i32* @all_timer, align 4, !dbg !513, !tbaa !117
  %40 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !514, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %40, metadata !160, metadata !DIExpression()) #6, !dbg !518
  store volatile i32 %40, i32* @issue_timer, align 4, !dbg !519, !tbaa !117
  call void @llvm.dbg.value(metadata i64 zext (i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32) to i64), metadata !172, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i64 %.val.i, metadata !180, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32), metadata !183, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !520
  %41 = trunc i64 %.val.i to i32, !dbg !522
  call void @llvm.dbg.value(metadata i32 %41, metadata !185, metadata !DIExpression()) #6, !dbg !520
  %42 = lshr i64 %.val.i, 32, !dbg !523
  %43 = trunc i64 %42 to i32, !dbg !524
  call void @llvm.dbg.value(metadata i32 %43, metadata !186, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !520
  call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %43, i32 %41) #5, !dbg !525, !srcloc !195
  call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_x to i32)) #5, !dbg !526, !srcloc !197
  %44 = call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !527, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %44, metadata !188, metadata !DIExpression()) #6, !dbg !520
  call void @llvm.dbg.value(metadata i64 zext (i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32) to i64), metadata !172, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i64 %.val8.i, metadata !180, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32), metadata !183, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !528
  %45 = trunc i64 %.val8.i to i32, !dbg !530
  call void @llvm.dbg.value(metadata i32 %45, metadata !185, metadata !DIExpression()) #6, !dbg !528
  %46 = lshr i64 %.val8.i, 32, !dbg !531
  %47 = trunc i64 %46 to i32, !dbg !532
  call void @llvm.dbg.value(metadata i32 %47, metadata !186, metadata !DIExpression()) #6, !dbg !528
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !528
  call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %47, i32 %45) #5, !dbg !533, !srcloc !195
  call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 ptrtoint ([2 x [8 x [64 x float]]]* @l1_buf_y to i32)) #5, !dbg !534, !srcloc !197
  %48 = call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !535, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %48, metadata !188, metadata !DIExpression()) #6, !dbg !528
  %49 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !536, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %49, metadata !160, metadata !DIExpression()) #6, !dbg !538
  %50 = load volatile i32, i32* @issue_timer, align 4, !dbg !539, !tbaa !117
  %51 = sub i32 %49, %50, !dbg !540
  %52 = load volatile i32, i32* @issue_time, align 4, !dbg !541, !tbaa !117
  %53 = add i32 %51, %52, !dbg !541
  store volatile i32 %53, i32* @issue_time, align 4, !dbg !541, !tbaa !117
  br label %.lr.ph.i.i.i, !dbg !542

.lr.ph.i.i.i:                                     ; preds = %38, %pulp_barrier.exit.i.i.i
  call void @llvm.dbg.value(metadata i32 0, metadata !461, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 0, metadata !462, metadata !DIExpression()) #6, !dbg !543
  call void @llvm.dbg.value(metadata i32 0, metadata !460, metadata !DIExpression()) #6, !dbg !474
  %54 = add i32 %.val10.i, -512
  %umax.i.i.i = call i32 @llvm.umax.i32(i32 %.val10.i, i32 512) #6, !dbg !544, !range !545
  %55 = add i32 %umax.i.i.i, -1, !dbg !544
  %56 = lshr i32 %55, 9, !dbg !544
  br label %57, !dbg !544

57:                                               ; preds = %.backedge.i.i.i, %.lr.ph.i.i.i
  %.028.i.i.i = phi i32 [ 0, %.lr.ph.i.i.i ], [ %.1.i.i.i, %.backedge.i.i.i ]
  %.06124.i.i.i = phi i32 [ 0, %.lr.ph.i.i.i ], [ %.06124.be.i.i.i, %.backedge.i.i.i ]
  %.06220.i.i.i = phi i32 [ 0, %.lr.ph.i.i.i ], [ %.06220.be.i.i.i, %.backedge.i.i.i ]
  call void @llvm.dbg.value(metadata i32 %.028.i.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06124.i.i.i, metadata !461, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06220.i.i.i, metadata !462, metadata !DIExpression()) #6, !dbg !543
  call void @llvm.dbg.value(metadata !DIArgList(i32 %.val10.i, i32 %.06220.i.i.i), metadata !464, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_constu, 512, DW_OP_minus, DW_OP_LLVM_arg, 1, DW_OP_minus, DW_OP_stack_value)) #6, !dbg !546
  br i1 %32, label %58, label %97, !dbg !547

58:                                               ; preds = %57
  call void @llvm.dbg.value(metadata !DIArgList(i32 %54, i32 %.06220.i.i.i), metadata !464, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_LLVM_arg, 1, DW_OP_minus, DW_OP_stack_value)) #6, !dbg !546
  %59 = sub i32 %54, %.06220.i.i.i, !dbg !548
  call void @llvm.dbg.value(metadata i32 %59, metadata !464, metadata !DIExpression()) #6, !dbg !546
  %60 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !549, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %60, metadata !160, metadata !DIExpression()) #6, !dbg !552
  store volatile i32 %60, i32* @dma_timer, align 4, !dbg !553, !tbaa !117
  call void asm sideeffect "1: \0A.word (0b0000100 << 25) $|                (  0b00010 << 20) $|                (    0b000 << 12) $|                (      (5) <<  7) $|                (0b0101011 <<  0)   \0Abne t0, zero, 1b \0A", "~{x5}"() #5, !dbg !554, !srcloc !229
  %61 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !556, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %61, metadata !160, metadata !DIExpression()) #6, !dbg !558
  %62 = load volatile i32, i32* @dma_timer, align 4, !dbg !559, !tbaa !117
  %63 = sub i32 %61, %62, !dbg !560
  %64 = load volatile i32, i32* @dma_time, align 4, !dbg !561, !tbaa !117
  %65 = add i32 %63, %64, !dbg !561
  store volatile i32 %65, i32* @dma_time, align 4, !dbg !561, !tbaa !117
  %66 = call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !562, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %66, metadata !130, metadata !DIExpression()) #6, !dbg !565
  %67 = and i32 %66, 32, !dbg !566
  %.not.i.i6.i.i.i = icmp eq i32 %67, 0, !dbg !566
  br i1 %.not.i.i6.i.i.i, label %pulp_barrier.exit7.i.i.i, label %68, !dbg !567

68:                                               ; preds = %58
  %69 = call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !568, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %69, metadata !132, metadata !DIExpression()) #6, !dbg !569
  br label %pulp_barrier.exit7.i.i.i, !dbg !570

pulp_barrier.exit7.i.i.i:                         ; preds = %68, %58
  %70 = call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !571, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %70, metadata !137, metadata !DIExpression()) #6, !dbg !572
  call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !573, !srcloc !151
  %71 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !574, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %71, metadata !160, metadata !DIExpression()) #6, !dbg !576
  store volatile i32 %71, i32* @issue_timer, align 4, !dbg !577, !tbaa !117
  %72 = icmp sgt i32 %59, 0, !dbg !578
  br i1 %72, label %73, label %93, !dbg !580

73:                                               ; preds = %pulp_barrier.exit7.i.i.i
  %74 = and i32 %.06124.i.i.i, 1, !dbg !581
  %75 = xor i32 %74, 1, !dbg !581
  %76 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_x, i32 0, i32 %75, i32 0, !dbg !583
  %77 = ptrtoint [64 x float]* %76 to i32, !dbg !584
  %78 = shl i32 %.06220.i.i.i, 2, !dbg !585
  %79 = add i32 %78, 2048, !dbg !585
  %80 = zext i32 %79 to i64, !dbg !586
  %81 = add i64 %.val.i, %80, !dbg !587
  call void @llvm.dbg.value(metadata i32 undef, metadata !172, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !588
  call void @llvm.dbg.value(metadata i64 %81, metadata !180, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata [64 x float]* %76, metadata !183, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !588
  %82 = trunc i64 %81 to i32, !dbg !590
  call void @llvm.dbg.value(metadata i32 %82, metadata !185, metadata !DIExpression()) #6, !dbg !588
  %83 = lshr i64 %81, 32, !dbg !591
  %84 = trunc i64 %83 to i32, !dbg !592
  call void @llvm.dbg.value(metadata i32 %84, metadata !186, metadata !DIExpression()) #6, !dbg !588
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !588
  call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %84, i32 %82) #5, !dbg !593, !srcloc !195
  call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 %77) #5, !dbg !594, !srcloc !197
  %85 = call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !595, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %85, metadata !188, metadata !DIExpression()) #6, !dbg !588
  %86 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %75, i32 0, !dbg !596
  %87 = ptrtoint [64 x float]* %86 to i32, !dbg !597
  %88 = add i64 %.val8.i, %80, !dbg !598
  call void @llvm.dbg.value(metadata i32 undef, metadata !172, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !599
  call void @llvm.dbg.value(metadata i64 %88, metadata !180, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata [64 x float]* %86, metadata !183, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 0, metadata !184, metadata !DIExpression()) #6, !dbg !599
  %89 = trunc i64 %88 to i32, !dbg !601
  call void @llvm.dbg.value(metadata i32 %89, metadata !185, metadata !DIExpression()) #6, !dbg !599
  %90 = lshr i64 %88, 32, !dbg !602
  %91 = trunc i64 %90 to i32, !dbg !603
  call void @llvm.dbg.value(metadata i32 %91, metadata !186, metadata !DIExpression()) #6, !dbg !599
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !599
  call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 %91, i32 %89) #5, !dbg !604, !srcloc !195
  call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 0, i32 %87) #5, !dbg !605, !srcloc !197
  %92 = call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !606, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %92, metadata !188, metadata !DIExpression()) #6, !dbg !599
  br label %93, !dbg !607

93:                                               ; preds = %73, %pulp_barrier.exit7.i.i.i
  %94 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !608, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %94, metadata !160, metadata !DIExpression()) #6, !dbg !610
  %95 = load volatile i32, i32* @issue_timer, align 4, !dbg !611, !tbaa !117
  %96 = sub i32 %94, %95, !dbg !612
  call void @llvm.dbg.value(metadata i32 %96, metadata !460, metadata !DIExpression()) #6, !dbg !474
  br label %110, !dbg !613

97:                                               ; preds = %57
  %98 = call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !614, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %98, metadata !130, metadata !DIExpression()) #6, !dbg !617
  %99 = and i32 %98, 32, !dbg !618
  %.not.i.i8.i.i.i = icmp eq i32 %99, 0, !dbg !618
  br i1 %.not.i.i8.i.i.i, label %pulp_barrier.exit9.i.i.i, label %100, !dbg !619

100:                                              ; preds = %97
  %101 = call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !620, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %101, metadata !132, metadata !DIExpression()) #6, !dbg !621
  br label %pulp_barrier.exit9.i.i.i, !dbg !622

pulp_barrier.exit9.i.i.i:                         ; preds = %100, %97
  %102 = call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !623, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %102, metadata !137, metadata !DIExpression()) #6, !dbg !624
  call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !625, !srcloc !151
  br i1 %22, label %103, label %105, !dbg !626

103:                                              ; preds = %pulp_barrier.exit9.i.i.i
  %104 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !627, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %104, metadata !160, metadata !DIExpression()) #6, !dbg !630
  store volatile i32 %104, i32* @compute_timer, align 4, !dbg !631, !tbaa !117
  br label %105, !dbg !632

105:                                              ; preds = %103, %pulp_barrier.exit9.i.i.i
  %106 = and i32 %.06124.i.i.i, 1, !dbg !633
  %107 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_x, i32 0, i32 %106, i32 %17, i32 0, !dbg !634
  call void @llvm.dbg.value(metadata float* %107, metadata !467, metadata !DIExpression()) #6, !dbg !635
  %108 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %106, i32 %17, i32 0, !dbg !636
  call void @llvm.dbg.value(metadata float* %108, metadata !470, metadata !DIExpression()) #6, !dbg !635
  %109 = call { float*, float* } asm sideeffect "li t0, 0\0A1: flw ft1, 0($0)\0Aflw ft2, 0($1)\0Afmadd.s ft2, $3, ft1, ft2\0Afsw ft2, 0($1)\0Aaddi $0, $0, 4\0Aaddi $1, $1, 4\0Aaddi t0, t0, 1\0Ablt t0, $2, 1b\0A", "=&r,=&r,r,f,0,1,~{f1},~{f2},~{x5},~{memory}"(i32 64, float %.val9.i, float* nonnull %107, float* nonnull %108) #5, !dbg !637, !srcloc !312
  call void @llvm.dbg.value(metadata float* undef, metadata !467, metadata !DIExpression()) #6, !dbg !635
  call void @llvm.dbg.value(metadata float* undef, metadata !470, metadata !DIExpression()) #6, !dbg !635
  br label %110

110:                                              ; preds = %105, %93
  %.1.i.i.i = phi i32 [ %96, %93 ], [ %.028.i.i.i, %105 ], !dbg !474
  call void @llvm.dbg.value(metadata i32 %.1.i.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  %111 = call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !638, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %111, metadata !130, metadata !DIExpression()) #6, !dbg !641
  %112 = and i32 %111, 32, !dbg !642
  %.not.i.i10.i.i.i = icmp eq i32 %112, 0, !dbg !642
  br i1 %.not.i.i10.i.i.i, label %pulp_barrier.exit11.i.i.i, label %113, !dbg !643

113:                                              ; preds = %110
  %114 = call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !644, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %114, metadata !132, metadata !DIExpression()) #6, !dbg !645
  br label %pulp_barrier.exit11.i.i.i, !dbg !646

pulp_barrier.exit11.i.i.i:                        ; preds = %113, %110
  %115 = call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !647, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %115, metadata !137, metadata !DIExpression()) #6, !dbg !648
  call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !649, !srcloc !151
  br i1 %22, label %116, label %122, !dbg !650

116:                                              ; preds = %pulp_barrier.exit11.i.i.i
  %117 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !651, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %117, metadata !160, metadata !DIExpression()) #6, !dbg !654
  %118 = load volatile i32, i32* @compute_timer, align 4, !dbg !655, !tbaa !117
  %119 = sub i32 %117, %118, !dbg !656
  %120 = load volatile i32, i32* @compute_time, align 4, !dbg !657, !tbaa !117
  %121 = add i32 %119, %120, !dbg !657
  store volatile i32 %121, i32* @compute_time, align 4, !dbg !657, !tbaa !117
  br label %122, !dbg !658

122:                                              ; preds = %116, %pulp_barrier.exit11.i.i.i
  br i1 %32, label %123, label %.thread29.i.i.i, !dbg !659

123:                                              ; preds = %122
  %124 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !660, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %124, metadata !160, metadata !DIExpression()) #6, !dbg !662
  %125 = load volatile i32, i32* @compute_timer, align 4, !dbg !663, !tbaa !117
  %126 = sub i32 %124, %125, !dbg !664
  call void @llvm.dbg.value(metadata i32 %126, metadata !471, metadata !DIExpression()) #6, !dbg !665
  %127 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !666, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %127, metadata !160, metadata !DIExpression()) #6, !dbg !668
  store volatile i32 %127, i32* @issue_timer, align 4, !dbg !669, !tbaa !117
  %128 = shl i32 %.06220.i.i.i, 2, !dbg !670
  %129 = zext i32 %128 to i64, !dbg !671
  %130 = add i64 %.val8.i, %129, !dbg !672
  %131 = and i32 %.06124.i.i.i, 1, !dbg !673
  %132 = getelementptr inbounds [2 x [8 x [64 x float]]], [2 x [8 x [64 x float]]]* @l1_buf_y, i32 0, i32 %131, !dbg !674
  %133 = ptrtoint [8 x [64 x float]]* %132 to i32, !dbg !675
  call void @llvm.dbg.value(metadata i64 %130, metadata !172, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 undef, metadata !180, metadata !DIExpression(DW_OP_LLVM_convert, 32, DW_ATE_unsigned, DW_OP_LLVM_convert, 64, DW_ATE_unsigned, DW_OP_stack_value)) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 2048, metadata !181, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 0, metadata !182, metadata !DIExpression()) #6, !dbg !676
  %134 = trunc i64 %130 to i32, !dbg !678
  call void @llvm.dbg.value(metadata i32 %134, metadata !183, metadata !DIExpression()) #6, !dbg !676
  %135 = lshr i64 %130, 32, !dbg !679
  %136 = trunc i64 %135 to i32, !dbg !680
  call void @llvm.dbg.value(metadata i32 %136, metadata !184, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata [8 x [64 x float]]* %132, metadata !185, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 0, metadata !186, metadata !DIExpression()) #6, !dbg !676
  call void @llvm.dbg.value(metadata i32 2048, metadata !187, metadata !DIExpression()) #6, !dbg !676
  call void asm sideeffect ".word (0b0000000 << 25) $|                (     (13) << 20) $|                (     (12) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x13},{x12}"(i32 0, i32 %133) #5, !dbg !681, !srcloc !195
  call void asm sideeffect ".word (0b0000001 << 25) $|                (     (11) << 20) $|                (     (10) << 15) $|                (    0b000 << 12) $|                (0b0101011 <<  0)   \0A", "{x11},{x10}"(i32 %136, i32 %134) #5, !dbg !682, !srcloc !197
  %137 = call i32 asm sideeffect ".word (0b0000010 << 25) $|                (   $2 << 20) $|                (     (14) << 15) $|                (    0b000 << 12) $|                (     (10) <<  7) $|                (0b0101011 <<  0)   \0A", "={x10},{x14},i"(i32 2048, i32 0) #5, !dbg !683, !srcloc !199
  call void @llvm.dbg.value(metadata i32 %137, metadata !188, metadata !DIExpression()) #6, !dbg !676
  %138 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !684, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %138, metadata !160, metadata !DIExpression()) #6, !dbg !686
  %139 = load volatile i32, i32* @issue_timer, align 4, !dbg !687, !tbaa !117
  %140 = call i32 @llvm.usub.sat.i32(i32 %.1.i.i.i, i32 %126) #6, !dbg !688
  %141 = load volatile i32, i32* @issue_time, align 4, !dbg !689, !tbaa !117
  %142 = add i32 %138, %140, !dbg !690
  %143 = sub i32 %142, %139, !dbg !691
  %144 = add i32 %143, %141, !dbg !689
  store volatile i32 %144, i32* @issue_time, align 4, !dbg !689, !tbaa !117
  call void @llvm.dbg.value(metadata i32 %.06124.i.i.i, metadata !461, metadata !DIExpression(DW_OP_plus_uconst, 1, DW_OP_stack_value)) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.1.i.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06220.i.i.i, metadata !462, metadata !DIExpression(DW_OP_plus_uconst, 512, DW_OP_stack_value)) #6, !dbg !543
  %exitcond.not.i.i.i = icmp eq i32 %.06124.i.i.i, %56, !dbg !692
  br i1 %exitcond.not.i.i.i, label %._crit_edge.i.i.i, label %.backedge.i.i.i, !dbg !544

.backedge.i.i.i:                                  ; preds = %.thread29.i.i.i, %123
  %.06220.be.i.i.i = add nuw nsw i32 %.06220.i.i.i, 512, !dbg !693
  %.06124.be.i.i.i = add nuw nsw i32 %.06124.i.i.i, 1, !dbg !694
  br label %57, !dbg !474, !llvm.loop !695

.thread29.i.i.i:                                  ; preds = %122
  call void @llvm.dbg.value(metadata i32 %.06124.i.i.i, metadata !461, metadata !DIExpression(DW_OP_plus_uconst, 1, DW_OP_stack_value)) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.1.i.i.i, metadata !460, metadata !DIExpression()) #6, !dbg !474
  call void @llvm.dbg.value(metadata i32 %.06220.i.i.i, metadata !462, metadata !DIExpression(DW_OP_plus_uconst, 512, DW_OP_stack_value)) #6, !dbg !543
  %exitcond.not30.i.i.i = icmp eq i32 %.06124.i.i.i, %56, !dbg !692
  br i1 %exitcond.not30.i.i.i, label %._crit_edge.thread.i.i.i.loopexit, label %.backedge.i.i.i, !dbg !544

._crit_edge.i.i.i:                                ; preds = %123
  %145 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !697, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %145, metadata !160, metadata !DIExpression()) #6, !dbg !701
  store volatile i32 %145, i32* @dma_timer, align 4, !dbg !702, !tbaa !117
  call void asm sideeffect "1: \0A.word (0b0000100 << 25) $|                (  0b00010 << 20) $|                (    0b000 << 12) $|                (      (5) <<  7) $|                (0b0101011 <<  0)   \0Abne t0, zero, 1b \0A", "~{x5}"() #5, !dbg !703, !srcloc !229
  %146 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !705, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %146, metadata !160, metadata !DIExpression()) #6, !dbg !707
  %147 = load volatile i32, i32* @dma_timer, align 4, !dbg !708, !tbaa !117
  %148 = sub i32 %146, %147, !dbg !709
  %149 = load volatile i32, i32* @dma_time, align 4, !dbg !710, !tbaa !117
  %150 = add i32 %148, %149, !dbg !710
  store volatile i32 %150, i32* @dma_time, align 4, !dbg !710, !tbaa !117
  call void asm sideeffect "fence rw, rw", "~{memory}"() #5, !dbg !711, !srcloc !388
  br label %._crit_edge.thread.i.i.i, !dbg !712

._crit_edge.thread.i.i.i.loopexit:                ; preds = %.thread29.i.i.i
  br label %._crit_edge.thread.i.i.i, !dbg !713

._crit_edge.thread.i.i.i:                         ; preds = %._crit_edge.thread.i.i.i.loopexit, %._crit_edge.i.i.i
  %151 = call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !713, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %151, metadata !130, metadata !DIExpression()) #6, !dbg !716
  %152 = and i32 %151, 32, !dbg !717
  %.not.i.i12.i.i.i = icmp eq i32 %152, 0, !dbg !717
  br i1 %.not.i.i12.i.i.i, label %pulp_barrier.exit13.i.i.i, label %153, !dbg !718

153:                                              ; preds = %._crit_edge.thread.i.i.i
  %154 = call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !719, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %154, metadata !132, metadata !DIExpression()) #6, !dbg !720
  br label %pulp_barrier.exit13.i.i.i, !dbg !721

pulp_barrier.exit13.i.i.i:                        ; preds = %153, %._crit_edge.thread.i.i.i
  %155 = call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !722, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %155, metadata !137, metadata !DIExpression()) #6, !dbg !723
  call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !724, !srcloc !151
  br i1 %32, label %156, label %165, !dbg !725

156:                                              ; preds = %pulp_barrier.exit13.i.i.i
  %157 = call i32 asm sideeffect "csrr $0, mcycle", "=r"() #5, !dbg !726, !srcloc !163
  call void @llvm.dbg.value(metadata i32 %157, metadata !160, metadata !DIExpression()) #6, !dbg !730
  %158 = load volatile i32, i32* @all_timer, align 4, !dbg !731, !tbaa !117
  %159 = sub i32 %157, %158, !dbg !732
  store volatile i32 %159, i32* @all_time, align 4, !dbg !733, !tbaa !117
  %160 = load volatile i32, i32* @all_time, align 4, !dbg !734, !tbaa !117
  %161 = load volatile i32, i32* @dma_time, align 4, !dbg !735, !tbaa !117
  %162 = load volatile i32, i32* @issue_time, align 4, !dbg !736, !tbaa !117
  %163 = load volatile i32, i32* @compute_time, align 4, !dbg !737, !tbaa !117
  %164 = call i32 (i8 addrspace(1)*, ...) @printf_(i8 addrspace(1)* noundef nonnull inttoptr (i64 ptrtoint ([43 x i8]* @.str.1 to i64) to i8 addrspace(1)*), i32 noundef %160, i32 noundef %161, i32 noundef %162, i32 noundef %163) #5, !dbg !738
  br label %165, !dbg !739

165:                                              ; preds = %156, %pulp_barrier.exit13.i.i.i
  %166 = call i32 asm sideeffect "csrr $0, misa", "=r"() #5, !dbg !740, !srcloc !139
  call void @llvm.dbg.value(metadata i32 %166, metadata !130, metadata !DIExpression()) #6, !dbg !743
  %167 = and i32 %166, 32, !dbg !744
  %.not.i.i14.i.i.i = icmp eq i32 %167, 0, !dbg !744
  br i1 %.not.i.i14.i.i.i, label %pulp_barrier.exit15.i.i.i, label %168, !dbg !745

168:                                              ; preds = %165
  %169 = call i32 asm sideeffect "fmv.x.w $0, fa0 \0Amv zero, $0 \0A", "=r,~{memory}"() #5, !dbg !746, !srcloc !144
  call void @llvm.dbg.value(metadata i32 %169, metadata !132, metadata !DIExpression()) #6, !dbg !747
  br label %pulp_barrier.exit15.i.i.i, !dbg !748

pulp_barrier.exit15.i.i.i:                        ; preds = %168, %165
  %170 = call i32 asm sideeffect "lw $0, 0($1) \0Amv zero, $0 \0A", "=r,r,~{memory}"(i32* nonnull @barrier_reg) #5, !dbg !749, !srcloc !148
  call void @llvm.dbg.value(metadata i32 %170, metadata !137, metadata !DIExpression()) #6, !dbg !750
  call void asm sideeffect "csrr x0, 0x7C2", "~{memory}"() #5, !dbg !751, !srcloc !151
  br label %__wrapped_omp__omp_offloading_10303_7120698_axpy_l19.exit, !dbg !752

__wrapped_omp__omp_offloading_10303_7120698_axpy_l19.exit: ; preds = %pulp_barrier.exit15.i.i.i, %24, %23
  ret void
}

attributes #0 = { convergent nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+d,+f,+m,+relax,+xdma,+xfalthalf,+xfaltquarter,+xfauxalthalf,+xfauxaltquarter,+xfauxhalf,+xfauxquarter,+xfauxvecalthalf,+xfauxvecaltquarter,+xfauxvechalf,+xfauxvecquarter,+xfauxvecsingle,+xfexpauxvecalthalf,+xfexpauxvecaltquarter,+xfexpauxvechalf,+xfexpauxvecquarter,+xfquarter,+xfrep,+xfvecalthalf,+xfvecaltquarter,+xfvechalf,+xfvecquarter,+xfvecsingle,+xssr,+zfh,-save-restore" }
attributes #1 = { alwaysinline convergent norecurse nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+d,+f,+m,+relax,+xdma,+xfalthalf,+xfaltquarter,+xfauxalthalf,+xfauxaltquarter,+xfauxhalf,+xfauxquarter,+xfauxvecalthalf,+xfauxvecaltquarter,+xfauxvechalf,+xfauxvecquarter,+xfauxvecsingle,+xfexpauxvecalthalf,+xfexpauxvecaltquarter,+xfexpauxvechalf,+xfexpauxvecquarter,+xfquarter,+xfrep,+xfvecalthalf,+xfvecaltquarter,+xfvechalf,+xfvecquarter,+xfvecsingle,+xssr,+zfh,-save-restore" }
attributes #2 = { convergent inlinehint nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+d,+f,+m,+relax,+xdma,+xfalthalf,+xfaltquarter,+xfauxalthalf,+xfauxaltquarter,+xfauxhalf,+xfauxquarter,+xfauxvecalthalf,+xfauxvecaltquarter,+xfauxvechalf,+xfauxvecquarter,+xfauxvecsingle,+xfexpauxvecalthalf,+xfexpauxvecaltquarter,+xfexpauxvechalf,+xfexpauxvecquarter,+xfquarter,+xfrep,+xfvecalthalf,+xfvecaltquarter,+xfvechalf,+xfvecquarter,+xfvecsingle,+xssr,+zfh,-save-restore" }
attributes #3 = { convergent "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+d,+f,+m,+relax,+xdma,+xfalthalf,+xfaltquarter,+xfauxalthalf,+xfauxaltquarter,+xfauxhalf,+xfauxquarter,+xfauxvecalthalf,+xfauxvecaltquarter,+xfauxvechalf,+xfauxvecquarter,+xfauxvecsingle,+xfexpauxvecalthalf,+xfexpauxvecaltquarter,+xfexpauxvechalf,+xfexpauxvecquarter,+xfquarter,+xfrep,+xfvecalthalf,+xfvecaltquarter,+xfvechalf,+xfvecquarter,+xfvecsingle,+xssr,+zfh,-save-restore" }
attributes #4 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #5 = { convergent nounwind }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!2}
!omp_offload.info = !{!56}
!llvm.module.flags = !{!57, !58, !59, !60, !61, !62, !63}
!llvm.ident = !{!64}
!nvvm.annotations = !{}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "l1_buf_x", scope: !2, file: !34, line: 49, type: !35, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !4, globals: !31, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "axpy_snitch.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy", checksumkind: CSK_MD5, checksum: "d4e1bcc458f5a3c3132146de425bd950")
!4 = !{!5, !10, !11, !14, !16, !17, !22, !23, !28, !29}
!5 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !6, line: 60, baseType: !7)
!6 = !DIFile(filename: "install/rv32imafd-ilp32d/riscv32-unknown-elf/include/sys/_stdint.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools")
!7 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint64_t", file: !8, line: 105, baseType: !9)
!8 = !DIFile(filename: "install/rv32imafd-ilp32d/riscv32-unknown-elf/include/machine/_default_types.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools")
!9 = !DIBasicType(name: "unsigned long long", size: 64, encoding: DW_ATE_unsigned)
!10 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!11 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !6, line: 48, baseType: !12)
!12 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint32_t", file: !8, line: 79, baseType: !13)
!13 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !15, size: 32)
!15 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !11)
!16 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !11, size: 32)
!17 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 32)
!18 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !19)
!19 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint16_t", file: !6, line: 36, baseType: !20)
!20 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint16_t", file: !8, line: 57, baseType: !21)
!21 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!22 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 32)
!23 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !24, size: 32)
!24 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !25)
!25 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !6, line: 24, baseType: !26)
!26 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint8_t", file: !8, line: 43, baseType: !27)
!27 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !25, size: 32)
!29 = !DIDerivedType(tag: DW_TAG_typedef, name: "uintptr_t", file: !6, line: 82, baseType: !30)
!30 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uintptr_t", file: !8, line: 232, baseType: !13)
!31 = !{!0, !32, !40, !42, !44, !46, !48, !50, !52, !54}
!32 = !DIGlobalVariableExpression(var: !33, expr: !DIExpression())
!33 = distinct !DIGlobalVariable(name: "l1_buf_y", scope: !2, file: !34, line: 50, type: !35, isLocal: false, isDefinition: true)
!34 = !DIFile(filename: "axpy_snitch.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy")
!35 = !DICompositeType(tag: DW_TAG_array_type, baseType: !10, size: 32768, elements: !36)
!36 = !{!37, !38, !39}
!37 = !DISubrange(count: 2)
!38 = !DISubrange(count: 8)
!39 = !DISubrange(count: 64)
!40 = !DIGlobalVariableExpression(var: !41, expr: !DIExpression())
!41 = distinct !DIGlobalVariable(name: "dma_timer", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!42 = !DIGlobalVariableExpression(var: !43, expr: !DIExpression())
!43 = distinct !DIGlobalVariable(name: "dma_time", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!44 = !DIGlobalVariableExpression(var: !45, expr: !DIExpression())
!45 = distinct !DIGlobalVariable(name: "all_timer", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!46 = !DIGlobalVariableExpression(var: !47, expr: !DIExpression())
!47 = distinct !DIGlobalVariable(name: "all_time", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!48 = !DIGlobalVariableExpression(var: !49, expr: !DIExpression())
!49 = distinct !DIGlobalVariable(name: "issue_timer", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!50 = !DIGlobalVariableExpression(var: !51, expr: !DIExpression())
!51 = distinct !DIGlobalVariable(name: "issue_time", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!52 = !DIGlobalVariableExpression(var: !53, expr: !DIExpression())
!53 = distinct !DIGlobalVariable(name: "compute_timer", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!54 = !DIGlobalVariableExpression(var: !55, expr: !DIExpression())
!55 = distinct !DIGlobalVariable(name: "compute_time", scope: !2, file: !34, line: 52, type: !15, isLocal: false, isDefinition: true)
!56 = !{i32 0, i32 66307, i32 118621848, !"axpy", i32 19, i32 0}
!57 = !{i32 7, !"Dwarf Version", i32 5}
!58 = !{i32 2, !"Debug Info Version", i32 3}
!59 = !{i32 1, !"wchar_size", i32 4}
!60 = !{i32 1, !"target-abi", !"ilp32d"}
!61 = !{i32 7, !"openmp", i32 50}
!62 = !{i32 7, !"openmp-device", i32 50}
!63 = !{i32 1, !"SmallDataLimit", i32 8}
!64 = !{!"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"}
!65 = distinct !DISubprogram(name: "dev_axpy", scope: !34, file: !34, line: 54, type: !66, scopeLine: 54, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !68)
!66 = !DISubroutineType(types: !67)
!67 = !{null, !5, !5, !10, !11}
!68 = !{!69, !70, !71, !72, !73, !75, !76, !78, !80, !83, !87, !88}
!69 = !DILocalVariable(name: "x_phys", arg: 1, scope: !65, file: !34, line: 54, type: !5)
!70 = !DILocalVariable(name: "y_phys", arg: 2, scope: !65, file: !34, line: 54, type: !5)
!71 = !DILocalVariable(name: "alpha", arg: 3, scope: !65, file: !34, line: 54, type: !10)
!72 = !DILocalVariable(name: "n", arg: 4, scope: !65, file: !34, line: 54, type: !11)
!73 = !DILocalVariable(name: "core_idx", scope: !65, file: !34, line: 56, type: !74)
!74 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !11)
!75 = !DILocalVariable(name: "issue_diff", scope: !65, file: !34, line: 58, type: !11)
!76 = !DILocalVariable(name: "itr", scope: !65, file: !34, line: 84, type: !77)
!77 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!78 = !DILocalVariable(name: "i", scope: !79, file: !34, line: 85, type: !77)
!79 = distinct !DILexicalBlock(scope: !65, file: !34, line: 85, column: 5)
!80 = !DILocalVariable(name: "rest", scope: !81, file: !34, line: 86, type: !77)
!81 = distinct !DILexicalBlock(scope: !82, file: !34, line: 85, column: 38)
!82 = distinct !DILexicalBlock(scope: !79, file: !34, line: 85, column: 5)
!83 = !DILocalVariable(name: "px", scope: !84, file: !34, line: 110, type: !86)
!84 = distinct !DILexicalBlock(scope: !85, file: !34, line: 101, column: 16)
!85 = distinct !DILexicalBlock(scope: !81, file: !34, line: 88, column: 12)
!86 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 32)
!87 = !DILocalVariable(name: "py", scope: !84, file: !34, line: 111, type: !86)
!88 = !DILocalVariable(name: "compute_diff", scope: !89, file: !34, line: 132, type: !11)
!89 = distinct !DILexicalBlock(scope: !90, file: !34, line: 131, column: 27)
!90 = distinct !DILexicalBlock(scope: !81, file: !34, line: 131, column: 12)
!91 = !DILocation(line: 0, scope: !65)
!92 = !DILocation(line: 28, column: 2, scope: !93, inlinedAt: !100)
!93 = distinct !DISubprogram(name: "pulp_get_core_id", scope: !94, file: !94, line: 26, type: !95, scopeLine: 26, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !98)
!94 = !DIFile(filename: "platforms/carfield/sw/tests/bare-metal/snitchd/runtime/runtime.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools")
!95 = !DISubroutineType(types: !96)
!96 = !{!97}
!97 = !DIDerivedType(tag: DW_TAG_typedef, name: "pulp_id_t", file: !94, line: 10, baseType: !11)
!98 = !{!99}
!99 = !DILocalVariable(name: "r", scope: !93, file: !94, line: 27, type: !97)
!100 = distinct !DILocation(line: 56, column: 31, scope: !65)
!101 = !{i64 10116}
!102 = !DILocation(line: 0, scope: !93, inlinedAt: !100)
!103 = !DILocation(line: 30, column: 10, scope: !93, inlinedAt: !100)
!104 = !DILocation(line: 60, column: 10, scope: !105)
!105 = distinct !DILexicalBlock(scope: !65, file: !34, line: 60, column: 9)
!106 = !DILocation(line: 60, column: 17, scope: !105)
!107 = !DILocation(line: 61, column: 12, scope: !108)
!108 = distinct !DILexicalBlock(scope: !105, file: !34, line: 60, column: 54)
!109 = !DILocation(line: 62, column: 13, scope: !110)
!110 = distinct !DILexicalBlock(scope: !108, file: !34, line: 61, column: 12)
!111 = !DILocation(line: 66, column: 9, scope: !65)
!112 = !DILocation(line: 67, column: 18, scope: !113)
!113 = distinct !DILexicalBlock(scope: !65, file: !34, line: 67, column: 9)
!114 = !DILocation(line: 67, column: 9, scope: !65)
!115 = !DILocation(line: 68, column: 18, scope: !116)
!116 = distinct !DILexicalBlock(scope: !113, file: !34, line: 67, column: 24)
!117 = !{!118, !118, i64 0}
!118 = !{!"int", !119, i64 0}
!119 = !{!"omnipotent char", !120, i64 0}
!120 = !{!"Simple C/C++ TBAA"}
!121 = !DILocation(line: 69, column: 20, scope: !116)
!122 = !DILocation(line: 71, column: 5, scope: !116)
!123 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !134)
!124 = distinct !DILexicalBlock(scope: !125, file: !94, line: 40, column: 10)
!125 = distinct !DILexicalBlock(scope: !126, file: !94, line: 40, column: 9)
!126 = distinct !DISubprogram(name: "fpu_fence", scope: !94, file: !94, line: 39, type: !127, scopeLine: 39, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !129)
!127 = !DISubroutineType(types: !128)
!128 = !{null}
!129 = !{!130, !132}
!130 = !DILocalVariable(name: "__tmp", scope: !124, file: !94, line: 40, type: !131)
!131 = !DIBasicType(name: "unsigned long", size: 32, encoding: DW_ATE_unsigned)
!132 = !DILocalVariable(name: "tmp", scope: !133, file: !94, line: 41, type: !11)
!133 = distinct !DILexicalBlock(scope: !125, file: !94, line: 40, column: 104)
!134 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !138)
!135 = distinct !DISubprogram(name: "pulp_barrier", scope: !94, file: !94, line: 52, type: !127, scopeLine: 52, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !136)
!136 = !{!137}
!137 = !DILocalVariable(name: "tmp", scope: !135, file: !94, line: 71, type: !11)
!138 = distinct !DILocation(line: 73, column: 5, scope: !65)
!139 = !{i64 10391}
!140 = !DILocation(line: 0, scope: !124, inlinedAt: !134)
!141 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !134)
!142 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !134)
!143 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !134)
!144 = !{i64 10507, i64 10545}
!145 = !DILocation(line: 0, scope: !133, inlinedAt: !134)
!146 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !134)
!147 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !138)
!148 = !{i64 10905, i64 10941}
!149 = !DILocation(line: 0, scope: !135, inlinedAt: !138)
!150 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !138)
!151 = !{i64 11060}
!152 = !DILocation(line: 75, column: 8, scope: !65)
!153 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !161)
!154 = distinct !DILexicalBlock(scope: !155, file: !94, line: 35, column: 10)
!155 = distinct !DISubprogram(name: "pulp_get_timer", scope: !94, file: !94, line: 34, type: !156, scopeLine: 34, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !159)
!156 = !DISubroutineType(types: !157)
!157 = !{!158}
!158 = !DIDerivedType(tag: DW_TAG_typedef, name: "pulp_timer_t", file: !94, line: 11, baseType: !11)
!159 = !{!160}
!160 = !DILocalVariable(name: "__tmp", scope: !154, file: !94, line: 35, type: !131)
!161 = distinct !DILocation(line: 75, column: 35, scope: !162)
!162 = distinct !DILexicalBlock(scope: !65, file: !34, line: 75, column: 8)
!163 = !{i64 10260}
!164 = !DILocation(line: 0, scope: !154, inlinedAt: !161)
!165 = !DILocation(line: 75, column: 33, scope: !162)
!166 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !167)
!167 = distinct !DILocation(line: 78, column: 23, scope: !168)
!168 = distinct !DILexicalBlock(scope: !169, file: !34, line: 77, column: 23)
!169 = distinct !DILexicalBlock(scope: !65, file: !34, line: 77, column: 8)
!170 = !DILocation(line: 0, scope: !154, inlinedAt: !167)
!171 = !DILocation(line: 78, column: 21, scope: !168)
!172 = !DILocalVariable(name: "dst", arg: 1, scope: !173, file: !94, line: 242, type: !5)
!173 = distinct !DISubprogram(name: "__dma_start_1d_wideptr_base", scope: !94, file: !94, line: 241, type: !174, scopeLine: 246, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !179)
!174 = !DISubroutineType(types: !175)
!175 = !{!176, !5, !5, !177, !74}
!176 = !DIDerivedType(tag: DW_TAG_typedef, name: "dma_txid_t", file: !94, line: 238, baseType: !11)
!177 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !178, line: 46, baseType: !13)
!178 = !DIFile(filename: "build/llvm-minimal/lib/clang/15.0.0/include/stddef.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero")
!179 = !{!172, !180, !181, !182, !183, !184, !185, !186, !187, !188}
!180 = !DILocalVariable(name: "src", arg: 2, scope: !173, file: !94, line: 243, type: !5)
!181 = !DILocalVariable(name: "size", arg: 3, scope: !173, file: !94, line: 244, type: !177)
!182 = !DILocalVariable(name: "__deserialize", arg: 4, scope: !173, file: !94, line: 245, type: !74)
!183 = !DILocalVariable(name: "reg_dst_low", scope: !173, file: !94, line: 247, type: !11)
!184 = !DILocalVariable(name: "reg_dst_high", scope: !173, file: !94, line: 248, type: !11)
!185 = !DILocalVariable(name: "reg_src_low", scope: !173, file: !94, line: 249, type: !11)
!186 = !DILocalVariable(name: "reg_src_high", scope: !173, file: !94, line: 250, type: !11)
!187 = !DILocalVariable(name: "reg_size", scope: !173, file: !94, line: 251, type: !11)
!188 = !DILocalVariable(name: "reg_txid", scope: !173, file: !94, line: 274, type: !11)
!189 = !DILocation(line: 0, scope: !173, inlinedAt: !190)
!190 = distinct !DILocation(line: 79, column: 9, scope: !168)
!191 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !190)
!192 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !190)
!193 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !190)
!194 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !190)
!195 = !{i64 14937}
!196 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !190)
!197 = !{i64 15192}
!198 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !190)
!199 = !{i64 15490}
!200 = !DILocation(line: 0, scope: !173, inlinedAt: !201)
!201 = distinct !DILocation(line: 80, column: 9, scope: !168)
!202 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !201)
!203 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !201)
!204 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !201)
!205 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !201)
!206 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !201)
!207 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !201)
!208 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !209)
!209 = distinct !DILocation(line: 81, column: 23, scope: !168)
!210 = !DILocation(line: 0, scope: !154, inlinedAt: !209)
!211 = !DILocation(line: 81, column: 42, scope: !168)
!212 = !DILocation(line: 81, column: 40, scope: !168)
!213 = !DILocation(line: 81, column: 20, scope: !168)
!214 = !DILocation(line: 82, column: 5, scope: !168)
!215 = !DILocation(line: 0, scope: !79)
!216 = !DILocation(line: 85, column: 5, scope: !79)
!217 = !DILocation(line: 0, scope: !81)
!218 = !DILocation(line: 88, column: 12, scope: !81)
!219 = !DILocation(line: 86, column: 29, scope: !81)
!220 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !221)
!221 = distinct !DILocation(line: 89, column: 25, scope: !222)
!222 = distinct !DILexicalBlock(scope: !85, file: !34, line: 88, column: 27)
!223 = !DILocation(line: 0, scope: !154, inlinedAt: !221)
!224 = !DILocation(line: 89, column: 23, scope: !222)
!225 = !DILocation(line: 448, column: 5, scope: !226, inlinedAt: !228)
!226 = distinct !DISubprogram(name: "dma_wait_all", scope: !94, file: !94, line: 446, type: !127, scopeLine: 446, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !227)
!227 = !{}
!228 = distinct !DILocation(line: 90, column: 13, scope: !222)
!229 = !{i64 19589, i64 19606, i64 19789}
!230 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !231)
!231 = distinct !DILocation(line: 91, column: 25, scope: !222)
!232 = !DILocation(line: 0, scope: !154, inlinedAt: !231)
!233 = !DILocation(line: 91, column: 44, scope: !222)
!234 = !DILocation(line: 91, column: 42, scope: !222)
!235 = !DILocation(line: 91, column: 22, scope: !222)
!236 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !237)
!237 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !238)
!238 = distinct !DILocation(line: 93, column: 13, scope: !222)
!239 = !DILocation(line: 0, scope: !124, inlinedAt: !237)
!240 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !237)
!241 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !237)
!242 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !237)
!243 = !DILocation(line: 0, scope: !133, inlinedAt: !237)
!244 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !237)
!245 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !238)
!246 = !DILocation(line: 0, scope: !135, inlinedAt: !238)
!247 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !238)
!248 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !249)
!249 = distinct !DILocation(line: 95, column: 27, scope: !222)
!250 = !DILocation(line: 0, scope: !154, inlinedAt: !249)
!251 = !DILocation(line: 95, column: 25, scope: !222)
!252 = !DILocation(line: 96, column: 21, scope: !253)
!253 = distinct !DILexicalBlock(scope: !222, file: !34, line: 96, column: 16)
!254 = !DILocation(line: 96, column: 16, scope: !222)
!255 = !DILocation(line: 97, column: 83, scope: !256)
!256 = distinct !DILexicalBlock(scope: !253, file: !34, line: 96, column: 26)
!257 = !DILocation(line: 97, column: 67, scope: !256)
!258 = !DILocation(line: 97, column: 55, scope: !256)
!259 = !DILocation(line: 97, column: 105, scope: !256)
!260 = !DILocation(line: 97, column: 97, scope: !256)
!261 = !DILocation(line: 97, column: 95, scope: !256)
!262 = !DILocation(line: 0, scope: !173, inlinedAt: !263)
!263 = distinct !DILocation(line: 97, column: 17, scope: !256)
!264 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !263)
!265 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !263)
!266 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !263)
!267 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !263)
!268 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !263)
!269 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !263)
!270 = !DILocation(line: 98, column: 67, scope: !256)
!271 = !DILocation(line: 98, column: 55, scope: !256)
!272 = !DILocation(line: 98, column: 95, scope: !256)
!273 = !DILocation(line: 0, scope: !173, inlinedAt: !274)
!274 = distinct !DILocation(line: 98, column: 17, scope: !256)
!275 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !274)
!276 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !274)
!277 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !274)
!278 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !274)
!279 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !274)
!280 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !274)
!281 = !DILocation(line: 99, column: 13, scope: !256)
!282 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !283)
!283 = distinct !DILocation(line: 100, column: 26, scope: !222)
!284 = !DILocation(line: 0, scope: !154, inlinedAt: !283)
!285 = !DILocation(line: 100, column: 45, scope: !222)
!286 = !DILocation(line: 100, column: 43, scope: !222)
!287 = !DILocation(line: 101, column: 9, scope: !222)
!288 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !289)
!289 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !290)
!290 = distinct !DILocation(line: 103, column: 13, scope: !84)
!291 = !DILocation(line: 0, scope: !124, inlinedAt: !289)
!292 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !289)
!293 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !289)
!294 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !289)
!295 = !DILocation(line: 0, scope: !133, inlinedAt: !289)
!296 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !289)
!297 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !290)
!298 = !DILocation(line: 0, scope: !135, inlinedAt: !290)
!299 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !290)
!300 = !DILocation(line: 105, column: 16, scope: !84)
!301 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !302)
!302 = distinct !DILocation(line: 105, column: 47, scope: !303)
!303 = distinct !DILexicalBlock(scope: !84, file: !34, line: 105, column: 16)
!304 = !DILocation(line: 0, scope: !154, inlinedAt: !302)
!305 = !DILocation(line: 105, column: 45, scope: !303)
!306 = !DILocation(line: 105, column: 31, scope: !303)
!307 = !DILocation(line: 110, column: 71, scope: !84)
!308 = !DILocation(line: 110, column: 58, scope: !84)
!309 = !DILocation(line: 0, scope: !84)
!310 = !DILocation(line: 111, column: 58, scope: !84)
!311 = !DILocation(line: 112, column: 13, scope: !84)
!312 = !{i64 85063, i64 85093, i64 85133, i64 85170, i64 85222, i64 85259, i64 85298, i64 85337, i64 85372}
!313 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !314)
!314 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !315)
!315 = distinct !DILocation(line: 127, column: 9, scope: !81)
!316 = !DILocation(line: 0, scope: !124, inlinedAt: !314)
!317 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !314)
!318 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !314)
!319 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !314)
!320 = !DILocation(line: 0, scope: !133, inlinedAt: !314)
!321 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !314)
!322 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !315)
!323 = !DILocation(line: 0, scope: !135, inlinedAt: !315)
!324 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !315)
!325 = !DILocation(line: 129, column: 12, scope: !81)
!326 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !327)
!327 = distinct !DILocation(line: 129, column: 43, scope: !328)
!328 = distinct !DILexicalBlock(scope: !81, file: !34, line: 129, column: 12)
!329 = !DILocation(line: 0, scope: !154, inlinedAt: !327)
!330 = !DILocation(line: 129, column: 62, scope: !328)
!331 = !DILocation(line: 129, column: 60, scope: !328)
!332 = !DILocation(line: 129, column: 40, scope: !328)
!333 = !DILocation(line: 129, column: 27, scope: !328)
!334 = !DILocation(line: 131, column: 12, scope: !81)
!335 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !336)
!336 = distinct !DILocation(line: 132, column: 38, scope: !89)
!337 = !DILocation(line: 0, scope: !154, inlinedAt: !336)
!338 = !DILocation(line: 132, column: 57, scope: !89)
!339 = !DILocation(line: 132, column: 55, scope: !89)
!340 = !DILocation(line: 0, scope: !89)
!341 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !342)
!342 = distinct !DILocation(line: 134, column: 27, scope: !89)
!343 = !DILocation(line: 0, scope: !154, inlinedAt: !342)
!344 = !DILocation(line: 134, column: 25, scope: !89)
!345 = !DILocation(line: 135, column: 51, scope: !89)
!346 = !DILocation(line: 135, column: 50, scope: !89)
!347 = !DILocation(line: 135, column: 48, scope: !89)
!348 = !DILocation(line: 135, column: 102, scope: !89)
!349 = !DILocation(line: 135, column: 90, scope: !89)
!350 = !DILocation(line: 135, column: 77, scope: !89)
!351 = !DILocation(line: 0, scope: !173, inlinedAt: !352)
!352 = distinct !DILocation(line: 135, column: 13, scope: !89)
!353 = !DILocation(line: 247, column: 48, scope: !173, inlinedAt: !352)
!354 = !DILocation(line: 248, column: 53, scope: !173, inlinedAt: !352)
!355 = !DILocation(line: 248, column: 49, scope: !173, inlinedAt: !352)
!356 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !352)
!357 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !352)
!358 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !352)
!359 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !360)
!360 = distinct !DILocation(line: 136, column: 27, scope: !89)
!361 = !DILocation(line: 0, scope: !154, inlinedAt: !360)
!362 = !DILocation(line: 136, column: 46, scope: !89)
!363 = !DILocation(line: 136, column: 88, scope: !89)
!364 = !DILocation(line: 136, column: 24, scope: !89)
!365 = !DILocation(line: 136, column: 44, scope: !89)
!366 = !DILocation(line: 136, column: 58, scope: !89)
!367 = !DILocation(line: 85, column: 22, scope: !82)
!368 = !DILocation(line: 85, column: 29, scope: !82)
!369 = !DILocation(line: 138, column: 12, scope: !81)
!370 = distinct !{!370, !216, !371, !372}
!371 = !DILocation(line: 139, column: 5, scope: !79)
!372 = !{!"llvm.loop.mustprogress"}
!373 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !374)
!374 = distinct !DILocation(line: 144, column: 21, scope: !375)
!375 = distinct !DILexicalBlock(scope: !376, file: !34, line: 143, column: 24)
!376 = distinct !DILexicalBlock(scope: !65, file: !34, line: 143, column: 9)
!377 = !DILocation(line: 0, scope: !154, inlinedAt: !374)
!378 = !DILocation(line: 144, column: 19, scope: !375)
!379 = !DILocation(line: 448, column: 5, scope: !226, inlinedAt: !380)
!380 = distinct !DILocation(line: 145, column: 9, scope: !375)
!381 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !382)
!382 = distinct !DILocation(line: 146, column: 21, scope: !375)
!383 = !DILocation(line: 0, scope: !154, inlinedAt: !382)
!384 = !DILocation(line: 146, column: 40, scope: !375)
!385 = !DILocation(line: 146, column: 38, scope: !375)
!386 = !DILocation(line: 146, column: 18, scope: !375)
!387 = !DILocation(line: 147, column: 9, scope: !375)
!388 = !{i64 86236}
!389 = !DILocation(line: 148, column: 5, scope: !375)
!390 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !391)
!391 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !392)
!392 = distinct !DILocation(line: 149, column: 5, scope: !65)
!393 = !DILocation(line: 0, scope: !124, inlinedAt: !391)
!394 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !391)
!395 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !391)
!396 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !391)
!397 = !DILocation(line: 0, scope: !133, inlinedAt: !391)
!398 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !391)
!399 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !392)
!400 = !DILocation(line: 0, scope: !135, inlinedAt: !392)
!401 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !392)
!402 = !DILocation(line: 151, column: 8, scope: !65)
!403 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !404)
!404 = distinct !DILocation(line: 152, column: 20, scope: !405)
!405 = distinct !DILexicalBlock(scope: !406, file: !34, line: 151, column: 23)
!406 = distinct !DILexicalBlock(scope: !65, file: !34, line: 151, column: 8)
!407 = !DILocation(line: 0, scope: !154, inlinedAt: !404)
!408 = !DILocation(line: 152, column: 39, scope: !405)
!409 = !DILocation(line: 152, column: 37, scope: !405)
!410 = !DILocation(line: 152, column: 18, scope: !405)
!411 = !DILocation(line: 153, column: 64, scope: !405)
!412 = !DILocation(line: 153, column: 74, scope: !405)
!413 = !DILocation(line: 153, column: 84, scope: !405)
!414 = !DILocation(line: 153, column: 96, scope: !405)
!415 = !DILocation(line: 153, column: 9, scope: !405)
!416 = !DILocation(line: 154, column: 5, scope: !405)
!417 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !418)
!418 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !419)
!419 = distinct !DILocation(line: 156, column: 5, scope: !65)
!420 = !DILocation(line: 0, scope: !124, inlinedAt: !418)
!421 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !418)
!422 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !418)
!423 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !418)
!424 = !DILocation(line: 0, scope: !133, inlinedAt: !418)
!425 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !418)
!426 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !419)
!427 = !DILocation(line: 0, scope: !135, inlinedAt: !419)
!428 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !419)
!429 = !DILocation(line: 157, column: 1, scope: !65)
!430 = distinct !DISubprogram(name: "__omp_offloading_10303_7120698_axpy_l19", scope: !34, file: !34, line: 19, type: !431, scopeLine: 19, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !434)
!431 = !DISubroutineType(types: !432)
!432 = !{null, !433, !433, !86, !16}
!433 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 32)
!434 = !{!435, !436, !437, !438}
!435 = !DILocalVariable(name: "x_phys", arg: 1, scope: !430, type: !433, flags: DIFlagArtificial)
!436 = !DILocalVariable(name: "y_phys", arg: 2, scope: !430, type: !433, flags: DIFlagArtificial)
!437 = !DILocalVariable(name: "alpha", arg: 3, scope: !430, type: !86, flags: DIFlagArtificial)
!438 = !DILocalVariable(name: "n", arg: 4, scope: !430, type: !16, flags: DIFlagArtificial)
!439 = !DILocation(line: 0, scope: !430)
!440 = !DILocation(line: 19, column: 1, scope: !430)
!441 = !{!442, !442, i64 0}
!442 = !{!"long long", !119, i64 0}
!443 = !{!444, !444, i64 0}
!444 = !{!"float", !119, i64 0}
!445 = !DILocalVariable(name: "x_phys", arg: 1, scope: !446, file: !34, line: 17, type: !433)
!446 = distinct !DISubprogram(name: "__omp_offloading_10303_7120698_axpy_l19_debug__", scope: !34, file: !34, line: 20, type: !431, scopeLine: 20, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !447)
!447 = !{!445, !448, !449, !450}
!448 = !DILocalVariable(name: "y_phys", arg: 2, scope: !446, file: !34, line: 17, type: !433)
!449 = !DILocalVariable(name: "alpha", arg: 3, scope: !446, file: !34, line: 17, type: !86)
!450 = !DILocalVariable(name: "n", arg: 4, scope: !446, file: !34, line: 17, type: !16)
!451 = !DILocation(line: 0, scope: !446, inlinedAt: !452)
!452 = distinct !DILocation(line: 19, column: 1, scope: !430)
!453 = !DILocalVariable(name: "x_phys", arg: 1, scope: !454, file: !34, line: 54, type: !5)
!454 = distinct !DISubprogram(name: "dev_axpy", scope: !34, file: !34, line: 54, type: !66, scopeLine: 54, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !455)
!455 = !{!453, !456, !457, !458, !459, !460, !461, !462, !464, !467, !470, !471}
!456 = !DILocalVariable(name: "y_phys", arg: 2, scope: !454, file: !34, line: 54, type: !5)
!457 = !DILocalVariable(name: "alpha", arg: 3, scope: !454, file: !34, line: 54, type: !10)
!458 = !DILocalVariable(name: "n", arg: 4, scope: !454, file: !34, line: 54, type: !11)
!459 = !DILocalVariable(name: "core_idx", scope: !454, file: !34, line: 56, type: !74)
!460 = !DILocalVariable(name: "issue_diff", scope: !454, file: !34, line: 58, type: !11)
!461 = !DILocalVariable(name: "itr", scope: !454, file: !34, line: 84, type: !77)
!462 = !DILocalVariable(name: "i", scope: !463, file: !34, line: 85, type: !77)
!463 = distinct !DILexicalBlock(scope: !454, file: !34, line: 85, column: 5)
!464 = !DILocalVariable(name: "rest", scope: !465, file: !34, line: 86, type: !77)
!465 = distinct !DILexicalBlock(scope: !466, file: !34, line: 85, column: 38)
!466 = distinct !DILexicalBlock(scope: !463, file: !34, line: 85, column: 5)
!467 = !DILocalVariable(name: "px", scope: !468, file: !34, line: 110, type: !86)
!468 = distinct !DILexicalBlock(scope: !469, file: !34, line: 101, column: 16)
!469 = distinct !DILexicalBlock(scope: !465, file: !34, line: 88, column: 12)
!470 = !DILocalVariable(name: "py", scope: !468, file: !34, line: 111, type: !86)
!471 = !DILocalVariable(name: "compute_diff", scope: !472, file: !34, line: 132, type: !11)
!472 = distinct !DILexicalBlock(scope: !473, file: !34, line: 131, column: 27)
!473 = distinct !DILexicalBlock(scope: !465, file: !34, line: 131, column: 12)
!474 = !DILocation(line: 0, scope: !454, inlinedAt: !475)
!475 = distinct !DILocation(line: 26, column: 9, scope: !476, inlinedAt: !452)
!476 = distinct !DILexicalBlock(scope: !446, file: !34, line: 20, column: 5)
!477 = !DILocation(line: 28, column: 2, scope: !93, inlinedAt: !478)
!478 = distinct !DILocation(line: 56, column: 31, scope: !454, inlinedAt: !475)
!479 = !DILocation(line: 0, scope: !93, inlinedAt: !478)
!480 = !DILocation(line: 30, column: 10, scope: !93, inlinedAt: !478)
!481 = !DILocation(line: 60, column: 10, scope: !482, inlinedAt: !475)
!482 = distinct !DILexicalBlock(scope: !454, file: !34, line: 60, column: 9)
!483 = !DILocation(line: 60, column: 17, scope: !482, inlinedAt: !475)
!484 = !DILocation(line: 61, column: 12, scope: !485, inlinedAt: !475)
!485 = distinct !DILexicalBlock(scope: !482, file: !34, line: 60, column: 54)
!486 = !DILocation(line: 62, column: 13, scope: !487, inlinedAt: !475)
!487 = distinct !DILexicalBlock(scope: !485, file: !34, line: 61, column: 12)
!488 = !DILocation(line: 66, column: 9, scope: !454, inlinedAt: !475)
!489 = !DILocation(line: 67, column: 18, scope: !490, inlinedAt: !475)
!490 = distinct !DILexicalBlock(scope: !454, file: !34, line: 67, column: 9)
!491 = !DILocation(line: 67, column: 9, scope: !454, inlinedAt: !475)
!492 = !DILocation(line: 68, column: 18, scope: !493, inlinedAt: !475)
!493 = distinct !DILexicalBlock(scope: !490, file: !34, line: 67, column: 24)
!494 = !DILocation(line: 69, column: 20, scope: !493, inlinedAt: !475)
!495 = !DILocation(line: 71, column: 5, scope: !493, inlinedAt: !475)
!496 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !497)
!497 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !498)
!498 = distinct !DILocation(line: 73, column: 5, scope: !454, inlinedAt: !475)
!499 = !DILocation(line: 0, scope: !124, inlinedAt: !497)
!500 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !497)
!501 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !497)
!502 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !497)
!503 = !DILocation(line: 0, scope: !133, inlinedAt: !497)
!504 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !497)
!505 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !498)
!506 = !DILocation(line: 0, scope: !135, inlinedAt: !498)
!507 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !498)
!508 = !DILocation(line: 75, column: 8, scope: !454, inlinedAt: !475)
!509 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !510)
!510 = distinct !DILocation(line: 75, column: 35, scope: !511, inlinedAt: !475)
!511 = distinct !DILexicalBlock(scope: !454, file: !34, line: 75, column: 8)
!512 = !DILocation(line: 0, scope: !154, inlinedAt: !510)
!513 = !DILocation(line: 75, column: 33, scope: !511, inlinedAt: !475)
!514 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !515)
!515 = distinct !DILocation(line: 78, column: 23, scope: !516, inlinedAt: !475)
!516 = distinct !DILexicalBlock(scope: !517, file: !34, line: 77, column: 23)
!517 = distinct !DILexicalBlock(scope: !454, file: !34, line: 77, column: 8)
!518 = !DILocation(line: 0, scope: !154, inlinedAt: !515)
!519 = !DILocation(line: 78, column: 21, scope: !516, inlinedAt: !475)
!520 = !DILocation(line: 0, scope: !173, inlinedAt: !521)
!521 = distinct !DILocation(line: 79, column: 9, scope: !516, inlinedAt: !475)
!522 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !521)
!523 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !521)
!524 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !521)
!525 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !521)
!526 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !521)
!527 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !521)
!528 = !DILocation(line: 0, scope: !173, inlinedAt: !529)
!529 = distinct !DILocation(line: 80, column: 9, scope: !516, inlinedAt: !475)
!530 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !529)
!531 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !529)
!532 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !529)
!533 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !529)
!534 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !529)
!535 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !529)
!536 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !537)
!537 = distinct !DILocation(line: 81, column: 23, scope: !516, inlinedAt: !475)
!538 = !DILocation(line: 0, scope: !154, inlinedAt: !537)
!539 = !DILocation(line: 81, column: 42, scope: !516, inlinedAt: !475)
!540 = !DILocation(line: 81, column: 40, scope: !516, inlinedAt: !475)
!541 = !DILocation(line: 81, column: 20, scope: !516, inlinedAt: !475)
!542 = !DILocation(line: 82, column: 5, scope: !516, inlinedAt: !475)
!543 = !DILocation(line: 0, scope: !463, inlinedAt: !475)
!544 = !DILocation(line: 85, column: 5, scope: !463, inlinedAt: !475)
!545 = !{i32 512, i32 -511}
!546 = !DILocation(line: 0, scope: !465, inlinedAt: !475)
!547 = !DILocation(line: 88, column: 12, scope: !465, inlinedAt: !475)
!548 = !DILocation(line: 86, column: 29, scope: !465, inlinedAt: !475)
!549 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !550)
!550 = distinct !DILocation(line: 89, column: 25, scope: !551, inlinedAt: !475)
!551 = distinct !DILexicalBlock(scope: !469, file: !34, line: 88, column: 27)
!552 = !DILocation(line: 0, scope: !154, inlinedAt: !550)
!553 = !DILocation(line: 89, column: 23, scope: !551, inlinedAt: !475)
!554 = !DILocation(line: 448, column: 5, scope: !226, inlinedAt: !555)
!555 = distinct !DILocation(line: 90, column: 13, scope: !551, inlinedAt: !475)
!556 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !557)
!557 = distinct !DILocation(line: 91, column: 25, scope: !551, inlinedAt: !475)
!558 = !DILocation(line: 0, scope: !154, inlinedAt: !557)
!559 = !DILocation(line: 91, column: 44, scope: !551, inlinedAt: !475)
!560 = !DILocation(line: 91, column: 42, scope: !551, inlinedAt: !475)
!561 = !DILocation(line: 91, column: 22, scope: !551, inlinedAt: !475)
!562 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !563)
!563 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !564)
!564 = distinct !DILocation(line: 93, column: 13, scope: !551, inlinedAt: !475)
!565 = !DILocation(line: 0, scope: !124, inlinedAt: !563)
!566 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !563)
!567 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !563)
!568 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !563)
!569 = !DILocation(line: 0, scope: !133, inlinedAt: !563)
!570 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !563)
!571 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !564)
!572 = !DILocation(line: 0, scope: !135, inlinedAt: !564)
!573 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !564)
!574 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !575)
!575 = distinct !DILocation(line: 95, column: 27, scope: !551, inlinedAt: !475)
!576 = !DILocation(line: 0, scope: !154, inlinedAt: !575)
!577 = !DILocation(line: 95, column: 25, scope: !551, inlinedAt: !475)
!578 = !DILocation(line: 96, column: 21, scope: !579, inlinedAt: !475)
!579 = distinct !DILexicalBlock(scope: !551, file: !34, line: 96, column: 16)
!580 = !DILocation(line: 96, column: 16, scope: !551, inlinedAt: !475)
!581 = !DILocation(line: 97, column: 83, scope: !582, inlinedAt: !475)
!582 = distinct !DILexicalBlock(scope: !579, file: !34, line: 96, column: 26)
!583 = !DILocation(line: 97, column: 67, scope: !582, inlinedAt: !475)
!584 = !DILocation(line: 97, column: 55, scope: !582, inlinedAt: !475)
!585 = !DILocation(line: 97, column: 105, scope: !582, inlinedAt: !475)
!586 = !DILocation(line: 97, column: 97, scope: !582, inlinedAt: !475)
!587 = !DILocation(line: 97, column: 95, scope: !582, inlinedAt: !475)
!588 = !DILocation(line: 0, scope: !173, inlinedAt: !589)
!589 = distinct !DILocation(line: 97, column: 17, scope: !582, inlinedAt: !475)
!590 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !589)
!591 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !589)
!592 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !589)
!593 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !589)
!594 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !589)
!595 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !589)
!596 = !DILocation(line: 98, column: 67, scope: !582, inlinedAt: !475)
!597 = !DILocation(line: 98, column: 55, scope: !582, inlinedAt: !475)
!598 = !DILocation(line: 98, column: 95, scope: !582, inlinedAt: !475)
!599 = !DILocation(line: 0, scope: !173, inlinedAt: !600)
!600 = distinct !DILocation(line: 98, column: 17, scope: !582, inlinedAt: !475)
!601 = !DILocation(line: 249, column: 48, scope: !173, inlinedAt: !600)
!602 = !DILocation(line: 250, column: 53, scope: !173, inlinedAt: !600)
!603 = !DILocation(line: 250, column: 49, scope: !173, inlinedAt: !600)
!604 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !600)
!605 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !600)
!606 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !600)
!607 = !DILocation(line: 99, column: 13, scope: !582, inlinedAt: !475)
!608 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !609)
!609 = distinct !DILocation(line: 100, column: 26, scope: !551, inlinedAt: !475)
!610 = !DILocation(line: 0, scope: !154, inlinedAt: !609)
!611 = !DILocation(line: 100, column: 45, scope: !551, inlinedAt: !475)
!612 = !DILocation(line: 100, column: 43, scope: !551, inlinedAt: !475)
!613 = !DILocation(line: 101, column: 9, scope: !551, inlinedAt: !475)
!614 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !615)
!615 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !616)
!616 = distinct !DILocation(line: 103, column: 13, scope: !468, inlinedAt: !475)
!617 = !DILocation(line: 0, scope: !124, inlinedAt: !615)
!618 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !615)
!619 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !615)
!620 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !615)
!621 = !DILocation(line: 0, scope: !133, inlinedAt: !615)
!622 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !615)
!623 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !616)
!624 = !DILocation(line: 0, scope: !135, inlinedAt: !616)
!625 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !616)
!626 = !DILocation(line: 105, column: 16, scope: !468, inlinedAt: !475)
!627 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !628)
!628 = distinct !DILocation(line: 105, column: 47, scope: !629, inlinedAt: !475)
!629 = distinct !DILexicalBlock(scope: !468, file: !34, line: 105, column: 16)
!630 = !DILocation(line: 0, scope: !154, inlinedAt: !628)
!631 = !DILocation(line: 105, column: 45, scope: !629, inlinedAt: !475)
!632 = !DILocation(line: 105, column: 31, scope: !629, inlinedAt: !475)
!633 = !DILocation(line: 110, column: 71, scope: !468, inlinedAt: !475)
!634 = !DILocation(line: 110, column: 58, scope: !468, inlinedAt: !475)
!635 = !DILocation(line: 0, scope: !468, inlinedAt: !475)
!636 = !DILocation(line: 111, column: 58, scope: !468, inlinedAt: !475)
!637 = !DILocation(line: 112, column: 13, scope: !468, inlinedAt: !475)
!638 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !639)
!639 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !640)
!640 = distinct !DILocation(line: 127, column: 9, scope: !465, inlinedAt: !475)
!641 = !DILocation(line: 0, scope: !124, inlinedAt: !639)
!642 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !639)
!643 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !639)
!644 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !639)
!645 = !DILocation(line: 0, scope: !133, inlinedAt: !639)
!646 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !639)
!647 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !640)
!648 = !DILocation(line: 0, scope: !135, inlinedAt: !640)
!649 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !640)
!650 = !DILocation(line: 129, column: 12, scope: !465, inlinedAt: !475)
!651 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !652)
!652 = distinct !DILocation(line: 129, column: 43, scope: !653, inlinedAt: !475)
!653 = distinct !DILexicalBlock(scope: !465, file: !34, line: 129, column: 12)
!654 = !DILocation(line: 0, scope: !154, inlinedAt: !652)
!655 = !DILocation(line: 129, column: 62, scope: !653, inlinedAt: !475)
!656 = !DILocation(line: 129, column: 60, scope: !653, inlinedAt: !475)
!657 = !DILocation(line: 129, column: 40, scope: !653, inlinedAt: !475)
!658 = !DILocation(line: 129, column: 27, scope: !653, inlinedAt: !475)
!659 = !DILocation(line: 131, column: 12, scope: !465, inlinedAt: !475)
!660 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !661)
!661 = distinct !DILocation(line: 132, column: 38, scope: !472, inlinedAt: !475)
!662 = !DILocation(line: 0, scope: !154, inlinedAt: !661)
!663 = !DILocation(line: 132, column: 57, scope: !472, inlinedAt: !475)
!664 = !DILocation(line: 132, column: 55, scope: !472, inlinedAt: !475)
!665 = !DILocation(line: 0, scope: !472, inlinedAt: !475)
!666 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !667)
!667 = distinct !DILocation(line: 134, column: 27, scope: !472, inlinedAt: !475)
!668 = !DILocation(line: 0, scope: !154, inlinedAt: !667)
!669 = !DILocation(line: 134, column: 25, scope: !472, inlinedAt: !475)
!670 = !DILocation(line: 135, column: 51, scope: !472, inlinedAt: !475)
!671 = !DILocation(line: 135, column: 50, scope: !472, inlinedAt: !475)
!672 = !DILocation(line: 135, column: 48, scope: !472, inlinedAt: !475)
!673 = !DILocation(line: 135, column: 102, scope: !472, inlinedAt: !475)
!674 = !DILocation(line: 135, column: 90, scope: !472, inlinedAt: !475)
!675 = !DILocation(line: 135, column: 77, scope: !472, inlinedAt: !475)
!676 = !DILocation(line: 0, scope: !173, inlinedAt: !677)
!677 = distinct !DILocation(line: 135, column: 13, scope: !472, inlinedAt: !475)
!678 = !DILocation(line: 247, column: 48, scope: !173, inlinedAt: !677)
!679 = !DILocation(line: 248, column: 53, scope: !173, inlinedAt: !677)
!680 = !DILocation(line: 248, column: 49, scope: !173, inlinedAt: !677)
!681 = !DILocation(line: 254, column: 5, scope: !173, inlinedAt: !677)
!682 = !DILocation(line: 264, column: 5, scope: !173, inlinedAt: !677)
!683 = !DILocation(line: 275, column: 5, scope: !173, inlinedAt: !677)
!684 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !685)
!685 = distinct !DILocation(line: 136, column: 27, scope: !472, inlinedAt: !475)
!686 = !DILocation(line: 0, scope: !154, inlinedAt: !685)
!687 = !DILocation(line: 136, column: 46, scope: !472, inlinedAt: !475)
!688 = !DILocation(line: 136, column: 88, scope: !472, inlinedAt: !475)
!689 = !DILocation(line: 136, column: 24, scope: !472, inlinedAt: !475)
!690 = !DILocation(line: 136, column: 44, scope: !472, inlinedAt: !475)
!691 = !DILocation(line: 136, column: 58, scope: !472, inlinedAt: !475)
!692 = !DILocation(line: 85, column: 22, scope: !466, inlinedAt: !475)
!693 = !DILocation(line: 85, column: 29, scope: !466, inlinedAt: !475)
!694 = !DILocation(line: 138, column: 12, scope: !465, inlinedAt: !475)
!695 = distinct !{!695, !544, !696, !372}
!696 = !DILocation(line: 139, column: 5, scope: !463, inlinedAt: !475)
!697 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !698)
!698 = distinct !DILocation(line: 144, column: 21, scope: !699, inlinedAt: !475)
!699 = distinct !DILexicalBlock(scope: !700, file: !34, line: 143, column: 24)
!700 = distinct !DILexicalBlock(scope: !454, file: !34, line: 143, column: 9)
!701 = !DILocation(line: 0, scope: !154, inlinedAt: !698)
!702 = !DILocation(line: 144, column: 19, scope: !699, inlinedAt: !475)
!703 = !DILocation(line: 448, column: 5, scope: !226, inlinedAt: !704)
!704 = distinct !DILocation(line: 145, column: 9, scope: !699, inlinedAt: !475)
!705 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !706)
!706 = distinct !DILocation(line: 146, column: 21, scope: !699, inlinedAt: !475)
!707 = !DILocation(line: 0, scope: !154, inlinedAt: !706)
!708 = !DILocation(line: 146, column: 40, scope: !699, inlinedAt: !475)
!709 = !DILocation(line: 146, column: 38, scope: !699, inlinedAt: !475)
!710 = !DILocation(line: 146, column: 18, scope: !699, inlinedAt: !475)
!711 = !DILocation(line: 147, column: 9, scope: !699, inlinedAt: !475)
!712 = !DILocation(line: 148, column: 5, scope: !699, inlinedAt: !475)
!713 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !714)
!714 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !715)
!715 = distinct !DILocation(line: 149, column: 5, scope: !454, inlinedAt: !475)
!716 = !DILocation(line: 0, scope: !124, inlinedAt: !714)
!717 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !714)
!718 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !714)
!719 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !714)
!720 = !DILocation(line: 0, scope: !133, inlinedAt: !714)
!721 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !714)
!722 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !715)
!723 = !DILocation(line: 0, scope: !135, inlinedAt: !715)
!724 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !715)
!725 = !DILocation(line: 151, column: 8, scope: !454, inlinedAt: !475)
!726 = !DILocation(line: 35, column: 33, scope: !154, inlinedAt: !727)
!727 = distinct !DILocation(line: 152, column: 20, scope: !728, inlinedAt: !475)
!728 = distinct !DILexicalBlock(scope: !729, file: !34, line: 151, column: 23)
!729 = distinct !DILexicalBlock(scope: !454, file: !34, line: 151, column: 8)
!730 = !DILocation(line: 0, scope: !154, inlinedAt: !727)
!731 = !DILocation(line: 152, column: 39, scope: !728, inlinedAt: !475)
!732 = !DILocation(line: 152, column: 37, scope: !728, inlinedAt: !475)
!733 = !DILocation(line: 152, column: 18, scope: !728, inlinedAt: !475)
!734 = !DILocation(line: 153, column: 64, scope: !728, inlinedAt: !475)
!735 = !DILocation(line: 153, column: 74, scope: !728, inlinedAt: !475)
!736 = !DILocation(line: 153, column: 84, scope: !728, inlinedAt: !475)
!737 = !DILocation(line: 153, column: 96, scope: !728, inlinedAt: !475)
!738 = !DILocation(line: 153, column: 9, scope: !728, inlinedAt: !475)
!739 = !DILocation(line: 154, column: 5, scope: !728, inlinedAt: !475)
!740 = !DILocation(line: 40, column: 33, scope: !124, inlinedAt: !741)
!741 = distinct !DILocation(line: 72, column: 5, scope: !135, inlinedAt: !742)
!742 = distinct !DILocation(line: 156, column: 5, scope: !454, inlinedAt: !475)
!743 = !DILocation(line: 0, scope: !124, inlinedAt: !741)
!744 = !DILocation(line: 40, column: 92, scope: !125, inlinedAt: !741)
!745 = !DILocation(line: 40, column: 9, scope: !126, inlinedAt: !741)
!746 = !DILocation(line: 42, column: 9, scope: !133, inlinedAt: !741)
!747 = !DILocation(line: 0, scope: !133, inlinedAt: !741)
!748 = !DILocation(line: 48, column: 5, scope: !133, inlinedAt: !741)
!749 = !DILocation(line: 73, column: 5, scope: !135, inlinedAt: !742)
!750 = !DILocation(line: 0, scope: !135, inlinedAt: !742)
!751 = !DILocation(line: 79, column: 5, scope: !135, inlinedAt: !742)
!752 = !DILocation(line: 157, column: 1, scope: !454, inlinedAt: !475)
!753 = distinct !DISubprogram(name: "hero_load_uint32_noblock", scope: !754, file: !754, line: 164, type: !755, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !759)
!754 = !DIFile(filename: "common/hero_64.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp")
!755 = !DISubroutineType(types: !756)
!756 = !{!77, !757, !758}
!757 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !5)
!758 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !16)
!759 = !{!760, !761, !762, !763, !765, !766}
!760 = !DILocalVariable(name: "addr", arg: 1, scope: !753, file: !754, line: 164, type: !757)
!761 = !DILocalVariable(name: "val", arg: 2, scope: !753, file: !754, line: 164, type: !758)
!762 = !DILocalVariable(name: "upper", scope: !753, file: !754, line: 164, type: !74)
!763 = !DILocalVariable(name: "lower", scope: !753, file: !754, line: 164, type: !764)
!764 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !14)
!765 = !DILocalVariable(name: "mstatus", scope: !753, file: !754, line: 164, type: !11)
!766 = !DILocalVariable(name: "reg", scope: !753, file: !754, line: 164, type: !11)
!767 = !DILocation(line: 0, scope: !753)
!768 = !DILocalVariable(name: "dw", arg: 1, scope: !769, file: !754, line: 70, type: !757)
!769 = distinct !DISubprogram(name: "__upper32", scope: !754, file: !754, line: 70, type: !770, scopeLine: 71, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !772)
!770 = !DISubroutineType(types: !771)
!771 = !{!11, !757}
!772 = !{!768}
!773 = !DILocation(line: 0, scope: !769, inlinedAt: !774)
!774 = distinct !DILocation(line: 164, column: 143, scope: !753)
!775 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !774)
!776 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !774)
!777 = !DILocalVariable(name: "dw", arg: 1, scope: !778, file: !754, line: 75, type: !757)
!778 = distinct !DISubprogram(name: "__lower32", scope: !754, file: !754, line: 75, type: !770, scopeLine: 76, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !779)
!779 = !{!777}
!780 = !DILocation(line: 0, scope: !778, inlinedAt: !781)
!781 = distinct !DILocation(line: 164, column: 278, scope: !753)
!782 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !781)
!783 = !DILocation(line: 164, column: 225, scope: !753)
!784 = !DILocation(line: 164, column: 327, scope: !753)
!785 = !{i64 77172, i64 77205, i64 77228, i64 77263, i64 77286}
!786 = !DILocation(line: 164, column: 597, scope: !753)
!787 = !DILocation(line: 164, column: 604, scope: !753)
!788 = distinct !DISubprogram(name: "hero_load_uint32", scope: !754, file: !754, line: 164, type: !770, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !789)
!789 = !{!790, !791, !792}
!790 = !DILocalVariable(name: "addr", arg: 1, scope: !788, file: !754, line: 164, type: !757)
!791 = !DILocalVariable(name: "val", scope: !788, file: !754, line: 164, type: !11)
!792 = !DILocalVariable(name: "res", scope: !788, file: !754, line: 164, type: !793)
!793 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !77)
!794 = !DILocation(line: 0, scope: !788)
!795 = !DILocation(line: 0, scope: !753, inlinedAt: !796)
!796 = distinct !DILocation(line: 164, column: 709, scope: !788)
!797 = !DILocation(line: 0, scope: !769, inlinedAt: !798)
!798 = distinct !DILocation(line: 164, column: 143, scope: !753, inlinedAt: !796)
!799 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !798)
!800 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !798)
!801 = !DILocation(line: 0, scope: !778, inlinedAt: !802)
!802 = distinct !DILocation(line: 164, column: 278, scope: !753, inlinedAt: !796)
!803 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !802)
!804 = !DILocation(line: 164, column: 225, scope: !753, inlinedAt: !796)
!805 = !DILocation(line: 164, column: 327, scope: !753, inlinedAt: !796)
!806 = !DILocation(line: 164, column: 791, scope: !788)
!807 = distinct !DISubprogram(name: "hero_store_uint32_noblock", scope: !754, file: !754, line: 164, type: !808, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !810)
!808 = !DISubroutineType(types: !809)
!809 = !{!77, !757, !74}
!810 = !{!811, !812, !813, !814, !815}
!811 = !DILocalVariable(name: "addr", arg: 1, scope: !807, file: !754, line: 164, type: !757)
!812 = !DILocalVariable(name: "val", arg: 2, scope: !807, file: !754, line: 164, type: !74)
!813 = !DILocalVariable(name: "upper", scope: !807, file: !754, line: 164, type: !74)
!814 = !DILocalVariable(name: "lower", scope: !807, file: !754, line: 164, type: !764)
!815 = !DILocalVariable(name: "mstatus", scope: !807, file: !754, line: 164, type: !11)
!816 = !DILocation(line: 0, scope: !807)
!817 = !DILocation(line: 0, scope: !769, inlinedAt: !818)
!818 = distinct !DILocation(line: 164, column: 915, scope: !807)
!819 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !818)
!820 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !818)
!821 = !DILocation(line: 0, scope: !778, inlinedAt: !822)
!822 = distinct !DILocation(line: 164, column: 1050, scope: !807)
!823 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !822)
!824 = !DILocation(line: 164, column: 997, scope: !807)
!825 = !DILocation(line: 164, column: 1085, scope: !807)
!826 = !{i64 77930, i64 77963, i64 77986, i64 78021, i64 78044}
!827 = !DILocation(line: 164, column: 1348, scope: !807)
!828 = distinct !DISubprogram(name: "hero_store_uint32", scope: !754, file: !754, line: 164, type: !829, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !831)
!829 = !DISubroutineType(types: !830)
!830 = !{null, !757, !74}
!831 = !{!832, !833, !834}
!832 = !DILocalVariable(name: "addr", arg: 1, scope: !828, file: !754, line: 164, type: !757)
!833 = !DILocalVariable(name: "val", arg: 2, scope: !828, file: !754, line: 164, type: !74)
!834 = !DILocalVariable(name: "res", scope: !828, file: !754, line: 164, type: !793)
!835 = !DILocation(line: 0, scope: !828)
!836 = !DILocation(line: 0, scope: !807, inlinedAt: !837)
!837 = distinct !DILocation(line: 164, column: 1456, scope: !828)
!838 = !DILocation(line: 0, scope: !769, inlinedAt: !839)
!839 = distinct !DILocation(line: 164, column: 915, scope: !807, inlinedAt: !837)
!840 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !839)
!841 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !839)
!842 = !DILocation(line: 0, scope: !778, inlinedAt: !843)
!843 = distinct !DILocation(line: 164, column: 1050, scope: !807, inlinedAt: !837)
!844 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !843)
!845 = !DILocation(line: 164, column: 997, scope: !807, inlinedAt: !837)
!846 = !DILocation(line: 164, column: 1085, scope: !807, inlinedAt: !837)
!847 = !DILocation(line: 164, column: 1494, scope: !828)
!848 = distinct !DISubprogram(name: "hero_load_uint16_noblock", scope: !754, file: !754, line: 164, type: !849, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !852)
!849 = !DISubroutineType(types: !850)
!850 = !{!77, !757, !851}
!851 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !22)
!852 = !{!853, !854, !855, !856, !858, !859}
!853 = !DILocalVariable(name: "addr", arg: 1, scope: !848, file: !754, line: 164, type: !757)
!854 = !DILocalVariable(name: "val", arg: 2, scope: !848, file: !754, line: 164, type: !851)
!855 = !DILocalVariable(name: "upper", scope: !848, file: !754, line: 164, type: !74)
!856 = !DILocalVariable(name: "lower", scope: !848, file: !754, line: 164, type: !857)
!857 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !17)
!858 = !DILocalVariable(name: "mstatus", scope: !848, file: !754, line: 164, type: !11)
!859 = !DILocalVariable(name: "reg", scope: !848, file: !754, line: 164, type: !19)
!860 = !DILocation(line: 0, scope: !848)
!861 = !DILocation(line: 0, scope: !769, inlinedAt: !862)
!862 = distinct !DILocation(line: 164, column: 1638, scope: !848)
!863 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !862)
!864 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !862)
!865 = !DILocation(line: 0, scope: !778, inlinedAt: !866)
!866 = distinct !DILocation(line: 164, column: 1773, scope: !848)
!867 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !866)
!868 = !DILocation(line: 164, column: 1720, scope: !848)
!869 = !DILocation(line: 164, column: 1822, scope: !848)
!870 = !{i64 78667, i64 78700, i64 78723, i64 78762, i64 78785}
!871 = !DILocation(line: 164, column: 2096, scope: !848)
!872 = !{!873, !873, i64 0}
!873 = !{!"short", !119, i64 0}
!874 = !DILocation(line: 164, column: 2103, scope: !848)
!875 = distinct !DISubprogram(name: "hero_load_uint16", scope: !754, file: !754, line: 164, type: !876, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !878)
!876 = !DISubroutineType(types: !877)
!877 = !{!19, !757}
!878 = !{!879, !880, !881}
!879 = !DILocalVariable(name: "addr", arg: 1, scope: !875, file: !754, line: 164, type: !757)
!880 = !DILocalVariable(name: "val", scope: !875, file: !754, line: 164, type: !19)
!881 = !DILocalVariable(name: "res", scope: !875, file: !754, line: 164, type: !793)
!882 = !DILocation(line: 0, scope: !875)
!883 = !DILocation(line: 0, scope: !848, inlinedAt: !884)
!884 = distinct !DILocation(line: 164, column: 2208, scope: !875)
!885 = !DILocation(line: 0, scope: !769, inlinedAt: !886)
!886 = distinct !DILocation(line: 164, column: 1638, scope: !848, inlinedAt: !884)
!887 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !886)
!888 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !886)
!889 = !DILocation(line: 0, scope: !778, inlinedAt: !890)
!890 = distinct !DILocation(line: 164, column: 1773, scope: !848, inlinedAt: !884)
!891 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !890)
!892 = !DILocation(line: 164, column: 1720, scope: !848, inlinedAt: !884)
!893 = !DILocation(line: 164, column: 1822, scope: !848, inlinedAt: !884)
!894 = !DILocation(line: 164, column: 2290, scope: !875)
!895 = distinct !DISubprogram(name: "hero_store_uint16_noblock", scope: !754, file: !754, line: 164, type: !896, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !899)
!896 = !DISubroutineType(types: !897)
!897 = !{!77, !757, !898}
!898 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !19)
!899 = !{!900, !901, !902, !903, !904}
!900 = !DILocalVariable(name: "addr", arg: 1, scope: !895, file: !754, line: 164, type: !757)
!901 = !DILocalVariable(name: "val", arg: 2, scope: !895, file: !754, line: 164, type: !898)
!902 = !DILocalVariable(name: "upper", scope: !895, file: !754, line: 164, type: !74)
!903 = !DILocalVariable(name: "lower", scope: !895, file: !754, line: 164, type: !857)
!904 = !DILocalVariable(name: "mstatus", scope: !895, file: !754, line: 164, type: !11)
!905 = !DILocation(line: 0, scope: !895)
!906 = !DILocation(line: 0, scope: !769, inlinedAt: !907)
!907 = distinct !DILocation(line: 164, column: 2414, scope: !895)
!908 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !907)
!909 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !907)
!910 = !DILocation(line: 0, scope: !778, inlinedAt: !911)
!911 = distinct !DILocation(line: 164, column: 2549, scope: !895)
!912 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !911)
!913 = !DILocation(line: 164, column: 2496, scope: !895)
!914 = !DILocation(line: 164, column: 2584, scope: !895)
!915 = !{i64 79429, i64 79462, i64 79485, i64 79520, i64 79543}
!916 = !DILocation(line: 164, column: 2847, scope: !895)
!917 = distinct !DISubprogram(name: "hero_store_uint16", scope: !754, file: !754, line: 164, type: !918, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !920)
!918 = !DISubroutineType(types: !919)
!919 = !{null, !757, !898}
!920 = !{!921, !922, !923}
!921 = !DILocalVariable(name: "addr", arg: 1, scope: !917, file: !754, line: 164, type: !757)
!922 = !DILocalVariable(name: "val", arg: 2, scope: !917, file: !754, line: 164, type: !898)
!923 = !DILocalVariable(name: "res", scope: !917, file: !754, line: 164, type: !793)
!924 = !DILocation(line: 0, scope: !917)
!925 = !DILocation(line: 0, scope: !895, inlinedAt: !926)
!926 = distinct !DILocation(line: 164, column: 2955, scope: !917)
!927 = !DILocation(line: 0, scope: !769, inlinedAt: !928)
!928 = distinct !DILocation(line: 164, column: 2414, scope: !895, inlinedAt: !926)
!929 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !928)
!930 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !928)
!931 = !DILocation(line: 0, scope: !778, inlinedAt: !932)
!932 = distinct !DILocation(line: 164, column: 2549, scope: !895, inlinedAt: !926)
!933 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !932)
!934 = !DILocation(line: 164, column: 2496, scope: !895, inlinedAt: !926)
!935 = !DILocation(line: 164, column: 2584, scope: !895, inlinedAt: !926)
!936 = !DILocation(line: 164, column: 2993, scope: !917)
!937 = distinct !DISubprogram(name: "hero_load_uint8_noblock", scope: !754, file: !754, line: 164, type: !938, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !941)
!938 = !DISubroutineType(types: !939)
!939 = !{!77, !757, !940}
!940 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !28)
!941 = !{!942, !943, !944, !945, !947, !948}
!942 = !DILocalVariable(name: "addr", arg: 1, scope: !937, file: !754, line: 164, type: !757)
!943 = !DILocalVariable(name: "val", arg: 2, scope: !937, file: !754, line: 164, type: !940)
!944 = !DILocalVariable(name: "upper", scope: !937, file: !754, line: 164, type: !74)
!945 = !DILocalVariable(name: "lower", scope: !937, file: !754, line: 164, type: !946)
!946 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !23)
!947 = !DILocalVariable(name: "mstatus", scope: !937, file: !754, line: 164, type: !11)
!948 = !DILocalVariable(name: "reg", scope: !937, file: !754, line: 164, type: !25)
!949 = !DILocation(line: 0, scope: !937)
!950 = !DILocation(line: 0, scope: !769, inlinedAt: !951)
!951 = distinct !DILocation(line: 164, column: 3135, scope: !937)
!952 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !951)
!953 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !951)
!954 = !DILocation(line: 0, scope: !778, inlinedAt: !955)
!955 = distinct !DILocation(line: 164, column: 3268, scope: !937)
!956 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !955)
!957 = !DILocation(line: 164, column: 3216, scope: !937)
!958 = !DILocation(line: 164, column: 3316, scope: !937)
!959 = !{i64 80161, i64 80194, i64 80217, i64 80256, i64 80279}
!960 = !DILocation(line: 164, column: 3590, scope: !937)
!961 = !{!119, !119, i64 0}
!962 = !DILocation(line: 164, column: 3597, scope: !937)
!963 = distinct !DISubprogram(name: "hero_load_uint8", scope: !754, file: !754, line: 164, type: !964, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !966)
!964 = !DISubroutineType(types: !965)
!965 = !{!25, !757}
!966 = !{!967, !968, !969}
!967 = !DILocalVariable(name: "addr", arg: 1, scope: !963, file: !754, line: 164, type: !757)
!968 = !DILocalVariable(name: "val", scope: !963, file: !754, line: 164, type: !25)
!969 = !DILocalVariable(name: "res", scope: !963, file: !754, line: 164, type: !793)
!970 = !DILocation(line: 0, scope: !963)
!971 = !DILocation(line: 0, scope: !937, inlinedAt: !972)
!972 = distinct !DILocation(line: 164, column: 3699, scope: !963)
!973 = !DILocation(line: 0, scope: !769, inlinedAt: !974)
!974 = distinct !DILocation(line: 164, column: 3135, scope: !937, inlinedAt: !972)
!975 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !974)
!976 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !974)
!977 = !DILocation(line: 0, scope: !778, inlinedAt: !978)
!978 = distinct !DILocation(line: 164, column: 3268, scope: !937, inlinedAt: !972)
!979 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !978)
!980 = !DILocation(line: 164, column: 3216, scope: !937, inlinedAt: !972)
!981 = !DILocation(line: 164, column: 3316, scope: !937, inlinedAt: !972)
!982 = !DILocation(line: 164, column: 3779, scope: !963)
!983 = distinct !DISubprogram(name: "hero_store_uint8_noblock", scope: !754, file: !754, line: 164, type: !984, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !987)
!984 = !DISubroutineType(types: !985)
!985 = !{!77, !757, !986}
!986 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !25)
!987 = !{!988, !989, !990, !991, !992}
!988 = !DILocalVariable(name: "addr", arg: 1, scope: !983, file: !754, line: 164, type: !757)
!989 = !DILocalVariable(name: "val", arg: 2, scope: !983, file: !754, line: 164, type: !986)
!990 = !DILocalVariable(name: "upper", scope: !983, file: !754, line: 164, type: !74)
!991 = !DILocalVariable(name: "lower", scope: !983, file: !754, line: 164, type: !946)
!992 = !DILocalVariable(name: "mstatus", scope: !983, file: !754, line: 164, type: !11)
!993 = !DILocation(line: 0, scope: !983)
!994 = !DILocation(line: 0, scope: !769, inlinedAt: !995)
!995 = distinct !DILocation(line: 164, column: 3901, scope: !983)
!996 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !995)
!997 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !995)
!998 = !DILocation(line: 0, scope: !778, inlinedAt: !999)
!999 = distinct !DILocation(line: 164, column: 4034, scope: !983)
!1000 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !999)
!1001 = !DILocation(line: 164, column: 3982, scope: !983)
!1002 = !DILocation(line: 164, column: 4069, scope: !983)
!1003 = !{i64 80914, i64 80947, i64 80970, i64 81005, i64 81028}
!1004 = !DILocation(line: 164, column: 4332, scope: !983)
!1005 = distinct !DISubprogram(name: "hero_store_uint8", scope: !754, file: !754, line: 164, type: !1006, scopeLine: 164, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !1008)
!1006 = !DISubroutineType(types: !1007)
!1007 = !{null, !757, !986}
!1008 = !{!1009, !1010, !1011}
!1009 = !DILocalVariable(name: "addr", arg: 1, scope: !1005, file: !754, line: 164, type: !757)
!1010 = !DILocalVariable(name: "val", arg: 2, scope: !1005, file: !754, line: 164, type: !986)
!1011 = !DILocalVariable(name: "res", scope: !1005, file: !754, line: 164, type: !793)
!1012 = !DILocation(line: 0, scope: !1005)
!1013 = !DILocation(line: 0, scope: !983, inlinedAt: !1014)
!1014 = distinct !DILocation(line: 164, column: 4438, scope: !1005)
!1015 = !DILocation(line: 0, scope: !769, inlinedAt: !1016)
!1016 = distinct !DILocation(line: 164, column: 3901, scope: !983, inlinedAt: !1014)
!1017 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1016)
!1018 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1016)
!1019 = !DILocation(line: 0, scope: !778, inlinedAt: !1020)
!1020 = distinct !DILocation(line: 164, column: 4034, scope: !983, inlinedAt: !1014)
!1021 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1020)
!1022 = !DILocation(line: 164, column: 3982, scope: !983, inlinedAt: !1014)
!1023 = !DILocation(line: 164, column: 4069, scope: !983, inlinedAt: !1014)
!1024 = !DILocation(line: 164, column: 4475, scope: !1005)
!1025 = distinct !DISubprogram(name: "hero_load_uint64", scope: !754, file: !754, line: 166, type: !1026, scopeLine: 167, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !1028)
!1026 = !DISubroutineType(types: !1027)
!1027 = !{!5, !757}
!1028 = !{!1029, !1030, !1031, !1032}
!1029 = !DILocalVariable(name: "addr", arg: 1, scope: !1025, file: !754, line: 166, type: !757)
!1030 = !DILocalVariable(name: "lower", scope: !1025, file: !754, line: 168, type: !74)
!1031 = !DILocalVariable(name: "upper", scope: !1025, file: !754, line: 169, type: !74)
!1032 = !DILocalVariable(name: "ret_val", scope: !1025, file: !754, line: 170, type: !5)
!1033 = !DILocation(line: 0, scope: !1025)
!1034 = !DILocation(line: 0, scope: !788, inlinedAt: !1035)
!1035 = distinct !DILocation(line: 168, column: 28, scope: !1025)
!1036 = !DILocation(line: 0, scope: !753, inlinedAt: !1037)
!1037 = distinct !DILocation(line: 164, column: 709, scope: !788, inlinedAt: !1035)
!1038 = !DILocation(line: 0, scope: !769, inlinedAt: !1039)
!1039 = distinct !DILocation(line: 164, column: 143, scope: !753, inlinedAt: !1037)
!1040 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1039)
!1041 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1039)
!1042 = !DILocation(line: 0, scope: !778, inlinedAt: !1043)
!1043 = distinct !DILocation(line: 164, column: 278, scope: !753, inlinedAt: !1037)
!1044 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1043)
!1045 = !DILocation(line: 164, column: 225, scope: !753, inlinedAt: !1037)
!1046 = !DILocation(line: 164, column: 327, scope: !753, inlinedAt: !1037)
!1047 = !DILocation(line: 169, column: 50, scope: !1025)
!1048 = !DILocation(line: 0, scope: !788, inlinedAt: !1049)
!1049 = distinct !DILocation(line: 169, column: 28, scope: !1025)
!1050 = !DILocation(line: 0, scope: !753, inlinedAt: !1051)
!1051 = distinct !DILocation(line: 164, column: 709, scope: !788, inlinedAt: !1049)
!1052 = !DILocation(line: 0, scope: !769, inlinedAt: !1053)
!1053 = distinct !DILocation(line: 164, column: 143, scope: !753, inlinedAt: !1051)
!1054 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1053)
!1055 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1053)
!1056 = !DILocation(line: 0, scope: !778, inlinedAt: !1057)
!1057 = distinct !DILocation(line: 164, column: 278, scope: !753, inlinedAt: !1051)
!1058 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1057)
!1059 = !DILocation(line: 164, column: 225, scope: !753, inlinedAt: !1051)
!1060 = !DILocation(line: 164, column: 327, scope: !753, inlinedAt: !1051)
!1061 = !DILocation(line: 170, column: 25, scope: !1025)
!1062 = !DILocation(line: 170, column: 41, scope: !1025)
!1063 = !DILocation(line: 170, column: 50, scope: !1025)
!1064 = !DILocation(line: 170, column: 48, scope: !1025)
!1065 = !DILocation(line: 172, column: 5, scope: !1025)
!1066 = distinct !DISubprogram(name: "hero_store_uint64", scope: !754, file: !754, line: 175, type: !1067, scopeLine: 176, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !1069)
!1067 = !DISubroutineType(types: !1068)
!1068 = !{null, !757, !757}
!1069 = !{!1070, !1071, !1072, !1073}
!1070 = !DILocalVariable(name: "addr", arg: 1, scope: !1066, file: !754, line: 175, type: !757)
!1071 = !DILocalVariable(name: "val", arg: 2, scope: !1066, file: !754, line: 175, type: !757)
!1072 = !DILocalVariable(name: "lower", scope: !1066, file: !754, line: 177, type: !74)
!1073 = !DILocalVariable(name: "upper", scope: !1066, file: !754, line: 179, type: !74)
!1074 = !DILocation(line: 0, scope: !1066)
!1075 = !DILocation(line: 177, column: 28, scope: !1066)
!1076 = !DILocation(line: 0, scope: !828, inlinedAt: !1077)
!1077 = distinct !DILocation(line: 178, column: 5, scope: !1066)
!1078 = !DILocation(line: 0, scope: !807, inlinedAt: !1079)
!1079 = distinct !DILocation(line: 164, column: 1456, scope: !828, inlinedAt: !1077)
!1080 = !DILocation(line: 0, scope: !769, inlinedAt: !1081)
!1081 = distinct !DILocation(line: 164, column: 915, scope: !807, inlinedAt: !1079)
!1082 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1081)
!1083 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1081)
!1084 = !DILocation(line: 0, scope: !778, inlinedAt: !1085)
!1085 = distinct !DILocation(line: 164, column: 1050, scope: !807, inlinedAt: !1079)
!1086 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1085)
!1087 = !DILocation(line: 164, column: 997, scope: !807, inlinedAt: !1079)
!1088 = !DILocation(line: 164, column: 1085, scope: !807, inlinedAt: !1079)
!1089 = !DILocation(line: 179, column: 43, scope: !1066)
!1090 = !DILocation(line: 179, column: 28, scope: !1066)
!1091 = !DILocation(line: 180, column: 28, scope: !1066)
!1092 = !DILocation(line: 0, scope: !828, inlinedAt: !1093)
!1093 = distinct !DILocation(line: 180, column: 5, scope: !1066)
!1094 = !DILocation(line: 0, scope: !807, inlinedAt: !1095)
!1095 = distinct !DILocation(line: 164, column: 1456, scope: !828, inlinedAt: !1093)
!1096 = !DILocation(line: 0, scope: !769, inlinedAt: !1097)
!1097 = distinct !DILocation(line: 164, column: 915, scope: !807, inlinedAt: !1095)
!1098 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1097)
!1099 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1097)
!1100 = !DILocation(line: 0, scope: !778, inlinedAt: !1101)
!1101 = distinct !DILocation(line: 164, column: 1050, scope: !807, inlinedAt: !1095)
!1102 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1101)
!1103 = !DILocation(line: 164, column: 997, scope: !807, inlinedAt: !1095)
!1104 = !DILocation(line: 164, column: 1085, scope: !807, inlinedAt: !1095)
!1105 = !DILocation(line: 181, column: 1, scope: !1066)
!1106 = distinct !DISubprogram(name: "hero_load_uint64_noblock", scope: !754, file: !754, line: 183, type: !1107, scopeLine: 184, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !1110)
!1107 = !DISubroutineType(types: !1108)
!1108 = !{!77, !757, !1109}
!1109 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !433)
!1110 = !{!1111, !1112, !1113, !1114, !1115, !1116}
!1111 = !DILocalVariable(name: "addr", arg: 1, scope: !1106, file: !754, line: 183, type: !757)
!1112 = !DILocalVariable(name: "val", arg: 2, scope: !1106, file: !754, line: 183, type: !1109)
!1113 = !DILocalVariable(name: "lower", scope: !1106, file: !754, line: 185, type: !758)
!1114 = !DILocalVariable(name: "res_lower", scope: !1106, file: !754, line: 186, type: !793)
!1115 = !DILocalVariable(name: "upper", scope: !1106, file: !754, line: 187, type: !758)
!1116 = !DILocalVariable(name: "res_upper", scope: !1106, file: !754, line: 188, type: !793)
!1117 = !DILocation(line: 0, scope: !1106)
!1118 = !DILocation(line: 185, column: 61, scope: !1106)
!1119 = !DILocation(line: 0, scope: !753, inlinedAt: !1120)
!1120 = distinct !DILocation(line: 186, column: 27, scope: !1106)
!1121 = !DILocation(line: 0, scope: !769, inlinedAt: !1122)
!1122 = distinct !DILocation(line: 164, column: 143, scope: !753, inlinedAt: !1120)
!1123 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1122)
!1124 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1122)
!1125 = !DILocation(line: 0, scope: !778, inlinedAt: !1126)
!1126 = distinct !DILocation(line: 164, column: 278, scope: !753, inlinedAt: !1120)
!1127 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1126)
!1128 = !DILocation(line: 164, column: 225, scope: !753, inlinedAt: !1120)
!1129 = !DILocation(line: 164, column: 327, scope: !753, inlinedAt: !1120)
!1130 = !DILocation(line: 164, column: 597, scope: !753, inlinedAt: !1120)
!1131 = !DILocation(line: 187, column: 67, scope: !1106)
!1132 = !DILocation(line: 188, column: 57, scope: !1106)
!1133 = !DILocation(line: 0, scope: !753, inlinedAt: !1134)
!1134 = distinct !DILocation(line: 188, column: 27, scope: !1106)
!1135 = !DILocation(line: 0, scope: !769, inlinedAt: !1136)
!1136 = distinct !DILocation(line: 164, column: 143, scope: !753, inlinedAt: !1134)
!1137 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1136)
!1138 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1136)
!1139 = !DILocation(line: 0, scope: !778, inlinedAt: !1140)
!1140 = distinct !DILocation(line: 164, column: 278, scope: !753, inlinedAt: !1134)
!1141 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1140)
!1142 = !DILocation(line: 164, column: 225, scope: !753, inlinedAt: !1134)
!1143 = !DILocation(line: 164, column: 327, scope: !753, inlinedAt: !1134)
!1144 = !DILocation(line: 164, column: 597, scope: !753, inlinedAt: !1134)
!1145 = !DILocation(line: 189, column: 5, scope: !1106)
!1146 = distinct !DISubprogram(name: "hero_store_uint64_noblock", scope: !754, file: !754, line: 192, type: !1147, scopeLine: 193, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !1149)
!1147 = !DISubroutineType(types: !1148)
!1148 = !{!77, !757, !757}
!1149 = !{!1150, !1151, !1152, !1153, !1154, !1155}
!1150 = !DILocalVariable(name: "addr", arg: 1, scope: !1146, file: !754, line: 192, type: !757)
!1151 = !DILocalVariable(name: "val", arg: 2, scope: !1146, file: !754, line: 192, type: !757)
!1152 = !DILocalVariable(name: "lower", scope: !1146, file: !754, line: 194, type: !74)
!1153 = !DILocalVariable(name: "res_lower", scope: !1146, file: !754, line: 195, type: !793)
!1154 = !DILocalVariable(name: "upper", scope: !1146, file: !754, line: 196, type: !74)
!1155 = !DILocalVariable(name: "res_upper", scope: !1146, file: !754, line: 197, type: !793)
!1156 = !DILocation(line: 0, scope: !1146)
!1157 = !DILocation(line: 194, column: 28, scope: !1146)
!1158 = !DILocation(line: 0, scope: !807, inlinedAt: !1159)
!1159 = distinct !DILocation(line: 195, column: 27, scope: !1146)
!1160 = !DILocation(line: 0, scope: !769, inlinedAt: !1161)
!1161 = distinct !DILocation(line: 164, column: 915, scope: !807, inlinedAt: !1159)
!1162 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1161)
!1163 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1161)
!1164 = !DILocation(line: 0, scope: !778, inlinedAt: !1165)
!1165 = distinct !DILocation(line: 164, column: 1050, scope: !807, inlinedAt: !1159)
!1166 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1165)
!1167 = !DILocation(line: 164, column: 997, scope: !807, inlinedAt: !1159)
!1168 = !DILocation(line: 164, column: 1085, scope: !807, inlinedAt: !1159)
!1169 = !DILocation(line: 196, column: 43, scope: !1146)
!1170 = !DILocation(line: 196, column: 28, scope: !1146)
!1171 = !DILocation(line: 197, column: 58, scope: !1146)
!1172 = !DILocation(line: 0, scope: !807, inlinedAt: !1173)
!1173 = distinct !DILocation(line: 197, column: 27, scope: !1146)
!1174 = !DILocation(line: 0, scope: !769, inlinedAt: !1175)
!1175 = distinct !DILocation(line: 164, column: 915, scope: !807, inlinedAt: !1173)
!1176 = !DILocation(line: 72, column: 26, scope: !769, inlinedAt: !1175)
!1177 = !DILocation(line: 72, column: 12, scope: !769, inlinedAt: !1175)
!1178 = !DILocation(line: 0, scope: !778, inlinedAt: !1179)
!1179 = distinct !DILocation(line: 164, column: 1050, scope: !807, inlinedAt: !1173)
!1180 = !DILocation(line: 77, column: 12, scope: !778, inlinedAt: !1179)
!1181 = !DILocation(line: 164, column: 997, scope: !807, inlinedAt: !1173)
!1182 = !DILocation(line: 164, column: 1085, scope: !807, inlinedAt: !1173)
!1183 = !DILocation(line: 198, column: 5, scope: !1146)
!1184 = !DISubprogram(name: "printf_", scope: !1185, file: !1185, line: 200, type: !1186, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !227)
!1185 = !DIFile(filename: "install/rv32imafd-ilp32d/riscv32-unknown-elf/include/stdio.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools")
!1186 = !DISubroutineType(types: !1187)
!1187 = !{!77, !1188, null}
!1188 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1189, size: 64)
!1189 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1190)
!1190 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
