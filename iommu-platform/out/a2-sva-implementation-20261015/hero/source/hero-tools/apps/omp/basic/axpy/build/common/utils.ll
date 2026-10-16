
; __CLANG_OFFLOAD_BUNDLE____START__ openmp-riscv32-hero-hero1-elf
; ModuleID = 'utils-openmp-riscv32-hero-hero1-elf.tmp.bc'
source_filename = "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c"
target datalayout = "e-m:e-p:32:32-p1:64:32-i64:64-n32-S128-P0-A0"
target triple = "riscv32-hero-hero1-elf"

%struct.__tgt_offload_entry = type { i8*, i8*, i32, i32, i32 }

@.omp_offloading.entry_name = internal unnamed_addr constant [50 x i8] c"__omp_offloading_10303_71206ba_init_omp_target_l9\00"
@.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9 = weak local_unnamed_addr constant %struct.__tgt_offload_entry { i8* bitcast (void ()* @__omp_offloading_10303_71206ba_init_omp_target_l9 to i8*), i8* getelementptr inbounds ([50 x i8], [50 x i8]* @.omp_offloading.entry_name, i32 0, i32 0), i32 0, i32 0, i32 0 }, section "omp_offloading_entries", align 1

; Function Attrs: alwaysinline convergent norecurse nounwind
define weak void @__omp_offloading_10303_71206ba_init_omp_target_l9() #0 !dbg !11 {
  tail call void asm sideeffect "nop", ""() #1, !dbg !16, !srcloc !19
  ret void, !dbg !20
}

attributes #0 = { alwaysinline convergent norecurse nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+d,+f,+m,+relax,+xdma,+xfalthalf,+xfaltquarter,+xfauxalthalf,+xfauxaltquarter,+xfauxhalf,+xfauxquarter,+xfauxvecalthalf,+xfauxvecaltquarter,+xfauxvechalf,+xfauxvecquarter,+xfauxvecsingle,+xfexpauxvecalthalf,+xfexpauxvecaltquarter,+xfexpauxvechalf,+xfexpauxvecquarter,+xfquarter,+xfrep,+xfvecalthalf,+xfvecaltquarter,+xfvechalf,+xfvecquarter,+xfvecsingle,+xssr,+zfh,-save-restore" }
attributes #1 = { convergent nounwind }

!llvm.dbg.cu = !{!0}
!omp_offload.info = !{!2}
!llvm.module.flags = !{!3, !4, !5, !6, !7, !8, !9}
!llvm.ident = !{!10}
!nvvm.annotations = !{}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "utils.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy", checksumkind: CSK_MD5, checksum: "41dab1e1e78654b5843ae00cf03cf08d")
!2 = !{i32 0, i32 66307, i32 118621882, !"init_omp_target", i32 9, i32 0}
!3 = !{i32 7, !"Dwarf Version", i32 5}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 1, !"target-abi", !"ilp32d"}
!7 = !{i32 7, !"openmp", i32 50}
!8 = !{i32 7, !"openmp-device", i32 50}
!9 = !{i32 1, !"SmallDataLimit", i32 8}
!10 = !{!"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"}
!11 = distinct !DISubprogram(name: "__omp_offloading_10303_71206ba_init_omp_target_l9", scope: !12, file: !12, line: 9, type: !13, scopeLine: 9, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !15)
!12 = !DIFile(filename: "common/utils.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp")
!13 = !DISubroutineType(types: !14)
!14 = !{null}
!15 = !{}
!16 = !DILocation(line: 10, column: 5, scope: !17, inlinedAt: !18)
!17 = distinct !DISubprogram(name: "__omp_offloading_10303_71206ba_init_omp_target_l9_debug__", scope: !12, file: !12, line: 10, type: !13, scopeLine: 10, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !15)
!18 = distinct !DILocation(line: 9, column: 1, scope: !11)
!19 = !{i64 433}
!20 = !DILocation(line: 9, column: 1, scope: !11)

; __CLANG_OFFLOAD_BUNDLE____END__ openmp-riscv32-hero-hero1-elf

; __CLANG_OFFLOAD_BUNDLE____START__ host-riscv64-hero-linux-gnu
; ModuleID = 'utils-host-riscv64-hero-linux-gnu.tmp.bc'
source_filename = "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n64-S128"
target triple = "riscv64-hero-linux-gnu"

%struct.ident_t = type { i32, i32, i32, i32, i8* }
%struct.__tgt_offload_entry = type { i8*, i8*, i64, i32, i32 }

@.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id = weak constant i8 0
@0 = private unnamed_addr constant [142 x i8] c";/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c;init_omp_target;9;1;;\00", align 1
@1 = private unnamed_addr constant %struct.ident_t { i32 0, i32 2, i32 0, i32 141, i8* getelementptr inbounds ([142 x i8], [142 x i8]* @0, i32 0, i32 0) }, align 8
@global_seed = dso_local local_unnamed_addr global i32 2048, align 4, !dbg !0
@.omp_offloading.entry_name = internal unnamed_addr constant [50 x i8] c"__omp_offloading_10303_71206ba_init_omp_target_l9\00"
@.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9 = weak local_unnamed_addr constant %struct.__tgt_offload_entry { i8* @.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id, i8* getelementptr inbounds ([50 x i8], [50 x i8]* @.omp_offloading.entry_name, i32 0, i32 0), i64 0, i32 0, i32 0 }, section "omp_offloading_entries", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 0, void ()* @.omp_offloading.requires_reg, i8* null }]

; Function Attrs: nounwind
define dso_local void @init_omp_target() local_unnamed_addr #0 !dbg !15 {
  %1 = tail call i32 @__tgt_target_mapper(%struct.ident_t* nonnull @1, i64 1, i8* nonnull @.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id, i32 0, i8** null, i8** null, i64* null, i64* null, i8** null, i8** null, i32 -1), !dbg !19
  %.not = icmp eq i32 %1, 0, !dbg !19
  br i1 %.not, label %3, label %2, !dbg !19

2:                                                ; preds = %0
  tail call void asm sideeffect "nop", ""() #1, !dbg !20, !srcloc !26
  br label %3, !dbg !19

3:                                                ; preds = %2, %0
  ret void, !dbg !27
}

; Function Attrs: nounwind
declare i32 @__tgt_target_mapper(%struct.ident_t*, i64, i8*, i32, i8**, i8**, i64*, i64*, i8**, i8**, i32) local_unnamed_addr #1

; Function Attrs: nounwind
define internal void @.omp_offloading.requires_reg() #0 section ".text.startup" !dbg !28 {
  tail call void @__tgt_register_requires(i64 1)
  ret void
}

; Function Attrs: nounwind
declare void @__tgt_register_requires(i64) local_unnamed_addr #1

attributes #0 = { nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,-save-restore" }
attributes #1 = { nounwind }

!llvm.dbg.cu = !{!2}
!omp_offload.info = !{!7}
!llvm.module.flags = !{!8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}
!nvvm.annotations = !{}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "global_seed", scope: !2, file: !5, line: 13, type: !6, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "utils.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy", checksumkind: CSK_MD5, checksum: "e9345ddfa45a81aedc3162cb3b57bc3a")
!4 = !{!0}
!5 = !DIFile(filename: "common/utils.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp")
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !{i32 0, i32 66307, i32 118621882, !"init_omp_target", i32 9, i32 0}
!8 = !{i32 7, !"Dwarf Version", i32 5}
!9 = !{i32 2, !"Debug Info Version", i32 3}
!10 = !{i32 1, !"wchar_size", i32 4}
!11 = !{i32 1, !"target-abi", !"lp64d"}
!12 = !{i32 7, !"openmp", i32 50}
!13 = !{i32 1, !"SmallDataLimit", i32 8}
!14 = !{!"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"}
!15 = distinct !DISubprogram(name: "init_omp_target", scope: !5, file: !5, line: 8, type: !16, scopeLine: 8, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !18)
!16 = !DISubroutineType(types: !17)
!17 = !{null}
!18 = !{}
!19 = !DILocation(line: 9, column: 1, scope: !15)
!20 = !DILocation(line: 10, column: 5, scope: !21, inlinedAt: !22)
!21 = distinct !DISubprogram(name: "__omp_offloading_10303_71206ba_init_omp_target_l9_debug__", scope: !5, file: !5, line: 10, type: !16, scopeLine: 10, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !18)
!22 = distinct !DILocation(line: 9, column: 1, scope: !23, inlinedAt: !24)
!23 = distinct !DISubprogram(name: "__omp_offloading_10303_71206ba_init_omp_target_l9", scope: !5, file: !5, line: 9, type: !16, scopeLine: 9, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !18)
!24 = distinct !DILocation(line: 9, column: 1, scope: !25)
!25 = distinct !DILexicalBlock(scope: !15, file: !5, line: 9, column: 1)
!26 = !{i64 433}
!27 = !DILocation(line: 11, column: 1, scope: !15)
!28 = distinct !DISubprogram(linkageName: ".omp_offloading.requires_reg", scope: !29, file: !29, type: !30, flags: DIFlagArtificial | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !18)
!29 = !DIFile(filename: "utils.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy")
!30 = !DISubroutineType(types: !18)

; __CLANG_OFFLOAD_BUNDLE____END__ host-riscv64-hero-linux-gnu
