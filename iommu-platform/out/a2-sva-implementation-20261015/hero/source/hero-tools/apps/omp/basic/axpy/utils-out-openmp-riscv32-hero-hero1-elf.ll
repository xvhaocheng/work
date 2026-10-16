; ModuleID = 'build/common/utils-hero1.TMP.1.ll'
source_filename = "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c"
target datalayout = "e-m:e-p:32:32-p1:64:32-i64:64-n32-S128-P0-A0"
target triple = "riscv32-hero-hero1-elf"

%struct.__tgt_offload_entry = type { i8*, i8*, i32, i32, i32 }

@.omp_offloading.entry_name = internal unnamed_addr constant [50 x i8] c"__omp_offloading_10303_71206ba_init_omp_target_l9\00"
@.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9 = weak local_unnamed_addr constant %struct.__tgt_offload_entry { i8* bitcast (void (i64)* @__omp_offloading_10303_71206ba_init_omp_target_l9 to i8*), i8* getelementptr inbounds ([50 x i8], [50 x i8]* @.omp_offloading.entry_name, i32 0, i32 0), i32 0, i32 0, i32 0 }, section "omp_offloading_entries", align 1

; Function Attrs: alwaysinline convergent norecurse nounwind
define weak void @__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9() #0 !dbg !11 {
  tail call void asm sideeffect "nop", ""() #1, !dbg !16, !srcloc !19
  ret void, !dbg !20
}

define weak void @__omp_offloading_10303_71206ba_init_omp_target_l9(i64 %0) {
entry:
  call void asm sideeffect "nop", ""() #1, !dbg !16, !srcloc !19
  ret void
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
