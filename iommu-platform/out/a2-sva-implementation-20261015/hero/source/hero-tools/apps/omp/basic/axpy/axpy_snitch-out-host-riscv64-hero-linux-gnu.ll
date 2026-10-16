; ModuleID = 'build/basic/axpy/axpy_snitch-host.ll'
source_filename = "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n64-S128"
target triple = "riscv64-hero-linux-gnu"

%struct.ident_t = type { i32, i32, i32, i32, i8* }
%struct.__tgt_offload_entry = type { i8*, i8*, i64, i32, i32 }

@.__omp_offloading_10303_7120698_axpy_l19.region_id = weak constant i8 0
@.offload_sizes = private unnamed_addr constant [4 x i64] [i64 8, i64 8, i64 4, i64 4]
@.offload_maptypes = private unnamed_addr constant [4 x i64] [i64 33, i64 33, i64 33, i64 33]
@0 = private unnamed_addr constant [145 x i8] c";x_phys;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;19;;\00", align 1
@1 = private unnamed_addr constant [145 x i8] c";y_phys;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;36;;\00", align 1
@2 = private unnamed_addr constant [144 x i8] c";alpha;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;50;;\00", align 1
@3 = private unnamed_addr constant [140 x i8] c";n;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;66;;\00", align 1
@.offload_mapnames = private constant [4 x i8*] [i8* getelementptr inbounds ([145 x i8], [145 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([145 x i8], [145 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([144 x i8], [144 x i8]* @2, i32 0, i32 0), i8* getelementptr inbounds ([140 x i8], [140 x i8]* @3, i32 0, i32 0)]
@4 = private unnamed_addr constant [142 x i8] c";/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;axpy;19;1;;\00", align 1
@5 = private unnamed_addr constant %struct.ident_t { i32 0, i32 2, i32 0, i32 141, i8* getelementptr inbounds ([142 x i8], [142 x i8]* @4, i32 0, i32 0) }, align 8
@.omp_offloading.entry_name = internal unnamed_addr constant [40 x i8] c"__omp_offloading_10303_7120698_axpy_l19\00"
@.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19 = weak local_unnamed_addr constant %struct.__tgt_offload_entry { i8* @.__omp_offloading_10303_7120698_axpy_l19.region_id, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @.omp_offloading.entry_name, i32 0, i32 0), i64 0, i32 0, i32 0 }, section "omp_offloading_entries", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 0, void ()* @.omp_offloading.requires_reg, i8* null }]

; Function Attrs: nounwind
define dso_local signext i32 @axpy(i64 noundef %0, i64 noundef %1, float noundef %2, i32 noundef signext %3) local_unnamed_addr #0 !dbg !20 {
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca float, align 4
  %8 = alloca i32, align 4
  %9 = alloca [4 x i8*], align 8
  %10 = alloca [4 x i8*], align 8
  call void @llvm.dbg.value(metadata i64 %0, metadata !26, metadata !DIExpression()), !dbg !30
  store i64 %0, i64* %5, align 8, !tbaa !31
  call void @llvm.dbg.value(metadata i64 %1, metadata !27, metadata !DIExpression()), !dbg !30
  store i64 %1, i64* %6, align 8, !tbaa !31
  call void @llvm.dbg.value(metadata float %2, metadata !28, metadata !DIExpression()), !dbg !30
  store float %2, float* %7, align 4, !tbaa !35
  call void @llvm.dbg.value(metadata i32 %3, metadata !29, metadata !DIExpression()), !dbg !30
  store i32 %3, i32* %8, align 4, !tbaa !37
  call void @llvm.dbg.value(metadata i64 %0, metadata !26, metadata !DIExpression()), !dbg !30
  %11 = icmp ne i64 %0, 0, !dbg !39
  call void @llvm.dbg.value(metadata i64 %1, metadata !27, metadata !DIExpression()), !dbg !30
  %12 = icmp ne i64 %1, 0
  %or.cond = and i1 %11, %12, !dbg !41
  %13 = icmp ne i32 %3, 0
  %or.cond3 = and i1 %or.cond, %13, !dbg !41
  %14 = and i32 %3, 511
  %.not = icmp eq i32 %14, 0
  %or.cond6 = and i1 %or.cond3, %.not, !dbg !41
  br i1 %or.cond6, label %15, label %33, !dbg !41

15:                                               ; preds = %4
  %16 = getelementptr inbounds [4 x i8*], [4 x i8*]* %9, i64 0, i64 0, !dbg !42
  %17 = bitcast [4 x i8*]* %9 to i64**, !dbg !42
  store i64* %5, i64** %17, align 8, !dbg !42
  %18 = getelementptr inbounds [4 x i8*], [4 x i8*]* %10, i64 0, i64 0, !dbg !42
  %19 = bitcast [4 x i8*]* %10 to i64**, !dbg !42
  store i64* %5, i64** %19, align 8, !dbg !42
  %20 = getelementptr inbounds [4 x i8*], [4 x i8*]* %9, i64 0, i64 1, !dbg !42
  %21 = bitcast i8** %20 to i64**, !dbg !42
  store i64* %6, i64** %21, align 8, !dbg !42
  %22 = getelementptr inbounds [4 x i8*], [4 x i8*]* %10, i64 0, i64 1, !dbg !42
  %23 = bitcast i8** %22 to i64**, !dbg !42
  store i64* %6, i64** %23, align 8, !dbg !42
  %24 = getelementptr inbounds [4 x i8*], [4 x i8*]* %9, i64 0, i64 2, !dbg !42
  %25 = bitcast i8** %24 to float**, !dbg !42
  store float* %7, float** %25, align 8, !dbg !42
  %26 = getelementptr inbounds [4 x i8*], [4 x i8*]* %10, i64 0, i64 2, !dbg !42
  %27 = bitcast i8** %26 to float**, !dbg !42
  store float* %7, float** %27, align 8, !dbg !42
  %28 = getelementptr inbounds [4 x i8*], [4 x i8*]* %9, i64 0, i64 3, !dbg !42
  %29 = bitcast i8** %28 to i32**, !dbg !42
  store i32* %8, i32** %29, align 8, !dbg !42
  %30 = getelementptr inbounds [4 x i8*], [4 x i8*]* %10, i64 0, i64 3, !dbg !42
  %31 = bitcast i8** %30 to i32**, !dbg !42
  store i32* %8, i32** %31, align 8, !dbg !42
  %32 = call i32 @__tgt_target_mapper(%struct.ident_t* nonnull @5, i64 1, i8* nonnull @.__omp_offloading_10303_7120698_axpy_l19.region_id, i32 4, i8** nonnull %16, i8** nonnull %18, i64* getelementptr inbounds ([4 x i64], [4 x i64]* @.offload_sizes, i64 0, i64 0), i64* getelementptr inbounds ([4 x i64], [4 x i64]* @.offload_maptypes, i64 0, i64 0), i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @.offload_mapnames, i64 0, i64 0), i8** null, i32 -1), !dbg !42
  br label %33, !dbg !42

33:                                               ; preds = %15, %4
  %.0 = phi i32 [ -1, %4 ], [ 0, %15 ], !dbg !30
  ret i32 %.0, !dbg !43
}

; Function Attrs: nounwind
declare i32 @__tgt_target_mapper(%struct.ident_t*, i64, i8*, i32, i8**, i8**, i64*, i64*, i8**, i8**, i32) local_unnamed_addr #1

; Function Attrs: nounwind
define internal void @.omp_offloading.requires_reg() #0 section ".text.startup" !dbg !44 {
  tail call void @__tgt_register_requires(i64 1)
  ret void
}

; Function Attrs: nounwind
declare void @__tgt_register_requires(i64) local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #2

attributes #0 = { nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,-save-restore" }
attributes #1 = { nounwind }
attributes #2 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!omp_offload.info = !{!12}
!llvm.module.flags = !{!13, !14, !15, !16, !17, !18}
!llvm.ident = !{!19}
!nvvm.annotations = !{}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "axpy_snitch.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy", checksumkind: CSK_MD5, checksum: "e4db55a76ea4d57c31fe6e72474c39e7")
!2 = !{!3, !8, !9}
!3 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !4, line: 27, baseType: !5)
!4 = !DIFile(filename: "build/rv64-ubuntu-sysroot/usr/include/bits/stdint-uintn.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero")
!5 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint64_t", file: !6, line: 45, baseType: !7)
!6 = !DIFile(filename: "build/rv64-ubuntu-sysroot/usr/include/bits/types.h", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero")
!7 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!8 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!9 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !4, line: 26, baseType: !10)
!10 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint32_t", file: !6, line: 42, baseType: !11)
!11 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!12 = !{i32 0, i32 66307, i32 118621848, !"axpy", i32 19, i32 0}
!13 = !{i32 7, !"Dwarf Version", i32 5}
!14 = !{i32 2, !"Debug Info Version", i32 3}
!15 = !{i32 1, !"wchar_size", i32 4}
!16 = !{i32 1, !"target-abi", !"lp64d"}
!17 = !{i32 7, !"openmp", i32 50}
!18 = !{i32 1, !"SmallDataLimit", i32 8}
!19 = !{!"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"}
!20 = distinct !DISubprogram(name: "axpy", scope: !21, file: !21, line: 17, type: !22, scopeLine: 17, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !25)
!21 = !DIFile(filename: "axpy_snitch.c", directory: "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy")
!22 = !DISubroutineType(types: !23)
!23 = !{!24, !3, !3, !8, !9}
!24 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!25 = !{!26, !27, !28, !29}
!26 = !DILocalVariable(name: "x_phys", arg: 1, scope: !20, file: !21, line: 17, type: !3)
!27 = !DILocalVariable(name: "y_phys", arg: 2, scope: !20, file: !21, line: 17, type: !3)
!28 = !DILocalVariable(name: "alpha", arg: 3, scope: !20, file: !21, line: 17, type: !8)
!29 = !DILocalVariable(name: "n", arg: 4, scope: !20, file: !21, line: 17, type: !9)
!30 = !DILocation(line: 0, scope: !20)
!31 = !{!32, !32, i64 0}
!32 = !{!"long", !33, i64 0}
!33 = !{!"omnipotent char", !34, i64 0}
!34 = !{!"Simple C/C++ TBAA"}
!35 = !{!36, !36, i64 0}
!36 = !{!"float", !33, i64 0}
!37 = !{!38, !38, i64 0}
!38 = !{!"int", !33, i64 0}
!39 = !DILocation(line: 18, column: 10, scope: !40)
!40 = distinct !DILexicalBlock(scope: !20, file: !21, line: 18, column: 9)
!41 = !DILocation(line: 18, column: 17, scope: !40)
!42 = !DILocation(line: 19, column: 1, scope: !20)
!43 = !DILocation(line: 30, column: 1, scope: !20)
!44 = distinct !DISubprogram(linkageName: ".omp_offloading.requires_reg", scope: !21, file: !21, type: !45, flags: DIFlagArtificial | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !46)
!45 = !DISubroutineType(types: !46)
!46 = !{}
