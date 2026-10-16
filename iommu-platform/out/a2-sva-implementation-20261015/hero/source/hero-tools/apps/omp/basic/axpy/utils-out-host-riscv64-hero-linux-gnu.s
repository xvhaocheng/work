	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.file	"utils.c"
	.file	0 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" "utils.c" md5 0xe9345ddfa45a81aedc3162cb3b57bc3a
	.file	1 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp" "common/utils.c"
	.globl	init_omp_target                 # -- Begin function init_omp_target
	.p2align	1
	.type	init_omp_target,@function
init_omp_target:                        # @init_omp_target
.Lfunc_begin0:
	.loc	1 8 0                           # common/utils.c:8:0
	.cfi_sections .debug_frame
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sd	ra, 24(sp)                      # 8-byte Folded Spill
	.cfi_offset ra, -8
	li	a0, -1
.Ltmp0:
	.loc	1 9 1 prologue_end              # common/utils.c:9:1
	sd	a0, 16(sp)
	sd	zero, 8(sp)
	lui	a0, %hi(.L__unnamed_1)
	addi	a0, a0, %lo(.L__unnamed_1)
	lui	a1, %hi(.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id)
	addi	a2, a1, %lo(.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id)
	li	a1, 1
	sd	zero, 0(sp)
	li	a3, 0
	li	a4, 0
	li	a5, 0
	li	a6, 0
	li	a7, 0
	call	__tgt_target_mapper@plt
.Ltmp1:
	sext.w	a0, a0
	beqz	a0, .LBB0_2
# %bb.1:
.Ltmp2:
	.loc	1 10 5                          # common/utils.c:10:5
	#APP
	nop
	#NO_APP
.Ltmp3:
.LBB0_2:
	.loc	1 11 1                          # common/utils.c:11:1
	ld	ra, 24(sp)                      # 8-byte Folded Reload
	addi	sp, sp, 32
	ret
.Ltmp4:
.Lfunc_end0:
	.size	init_omp_target, .Lfunc_end0-init_omp_target
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	1                               # -- Begin function .omp_offloading.requires_reg
	.type	.omp_offloading.requires_reg,@function
.omp_offloading.requires_reg:           # @.omp_offloading.requires_reg
.Lfunc_begin1:
	.cfi_startproc
# %bb.0:
	li	a0, 1
.Ltmp5:
	.loc	1 0 1 is_stmt 0                 # common/utils.c:0:1
	tail	__tgt_register_requires@plt
.Ltmp6:
.Lfunc_end1:
	.size	.omp_offloading.requires_reg, .Lfunc_end1-.omp_offloading.requires_reg
	.cfi_endproc
                                        # -- End function
	.type	.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id,@object # @.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id
	.section	.rodata,"a",@progbits
	.weak	.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id
.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id:
	.byte	0                               # 0x0
	.size	.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id, 1

	.type	.L__unnamed_2,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_2:
	.asciz	";/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/common/utils.c;init_omp_target;9;1;;"
	.size	.L__unnamed_2, 142

	.type	.L__unnamed_1,@object           # @1
	.section	.rodata,"a",@progbits
	.p2align	3
.L__unnamed_1:
	.word	0                               # 0x0
	.word	2                               # 0x2
	.word	0                               # 0x0
	.word	141                             # 0x8d
	.quad	.L__unnamed_2
	.size	.L__unnamed_1, 24

	.type	global_seed,@object             # @global_seed
	.section	.sdata,"aw",@progbits
	.globl	global_seed
	.p2align	2
global_seed:
	.word	2048                            # 0x800
	.size	global_seed, 4

	.type	.omp_offloading.entry_name,@object # @.omp_offloading.entry_name
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4
.omp_offloading.entry_name:
	.asciz	"__omp_offloading_10303_71206ba_init_omp_target_l9"
	.size	.omp_offloading.entry_name, 50

	.type	.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9,@object # @.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9
	.section	omp_offloading_entries,"a",@progbits
	.weak	.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9
.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9:
	.quad	.__omp_offloading_10303_71206ba_init_omp_target_l9.region_id
	.quad	.omp_offloading.entry_name
	.quad	0                               # 0x0
	.word	0                               # 0x0
	.word	0                               # 0x0
	.size	.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9, 32

	.section	.init_array.0,"aw",@init_array
	.p2align	3
	.quad	.omp_offloading.requires_reg
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
	.byte	1                               # DW_FORM_addr
	.byte	85                              # DW_AT_ranges
	.byte	35                              # DW_FORM_rnglistx
	.byte	115                             # DW_AT_addr_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	116                             # DW_AT_rnglists_base
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
	.byte	4                               # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	0                               # DW_CHILDREN_no
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
	.byte	5                               # Abbreviation Code
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
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	6                               # Abbreviation Code
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
	.byte	7                               # Abbreviation Code
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
	.byte	8                               # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	0                               # DW_CHILDREN_no
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.byte	122                             # DW_AT_call_all_calls
	.byte	25                              # DW_FORM_flag_present
	.byte	110                             # DW_AT_linkage_name
	.byte	37                              # DW_FORM_strx1
	.byte	52                              # DW_AT_artificial
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
	.byte	1                               # Abbrev [1] 0xc:0x67 DW_TAG_compile_unit
	.byte	0                               # DW_AT_producer
	.half	12                              # DW_AT_language
	.byte	1                               # DW_AT_name
	.word	.Lstr_offsets_base0             # DW_AT_str_offsets_base
	.word	.Lline_table_start0             # DW_AT_stmt_list
	.byte	2                               # DW_AT_comp_dir
	.quad	0                               # DW_AT_low_pc
	.byte	0                               # DW_AT_ranges
	.word	.Laddr_table_base0              # DW_AT_addr_base
	.word	.Lrnglists_table_base0          # DW_AT_rnglists_base
	.byte	2                               # Abbrev [2] 0x2b:0xb DW_TAG_variable
	.byte	3                               # DW_AT_name
	.word	54                              # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_decl_file
	.byte	13                              # DW_AT_decl_line
	.byte	2                               # DW_AT_location
	.byte	161
	.byte	0
	.byte	3                               # Abbrev [3] 0x36:0x4 DW_TAG_base_type
	.byte	4                               # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	4                               # Abbrev [4] 0x3a:0x4 DW_TAG_subprogram
	.byte	5                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	10                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_inline
	.byte	4                               # Abbrev [4] 0x3e:0x4 DW_TAG_subprogram
	.byte	6                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	9                               # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_inline
	.byte	5                               # Abbrev [5] 0x42:0x27 DW_TAG_subprogram
	.byte	1                               # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	7                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	8                               # DW_AT_decl_line
                                        # DW_AT_external
	.byte	6                               # Abbrev [6] 0x4d:0x1b DW_TAG_inlined_subroutine
	.word	62                              # DW_AT_abstract_origin
	.byte	2                               # DW_AT_low_pc
	.word	.Ltmp3-.Ltmp2                   # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	9                               # DW_AT_call_line
	.byte	1                               # DW_AT_call_column
	.byte	7                               # Abbrev [7] 0x5a:0xd DW_TAG_inlined_subroutine
	.word	58                              # DW_AT_abstract_origin
	.byte	2                               # DW_AT_low_pc
	.word	.Ltmp3-.Ltmp2                   # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	9                               # DW_AT_call_line
	.byte	1                               # DW_AT_call_column
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	8                               # Abbrev [8] 0x69:0x9 DW_TAG_subprogram
	.byte	3                               # DW_AT_low_pc
	.word	.Lfunc_end1-.Lfunc_begin1       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	8                               # DW_AT_linkage_name
                                        # DW_AT_artificial
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_rnglists,"",@progbits
	.word	.Ldebug_list_header_end0-.Ldebug_list_header_start0 # Length
.Ldebug_list_header_start0:
	.half	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.word	1                               # Offset entry count
.Lrnglists_table_base0:
	.word	.Ldebug_ranges0-.Lrnglists_table_base0
.Ldebug_ranges0:
	.byte	3                               # DW_RLE_startx_length
	.byte	1                               #   start index
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   length
	.byte	3                               # DW_RLE_startx_length
	.byte	3                               #   start index
	.uleb128 .Lfunc_end1-.Lfunc_begin1      #   length
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_list_header_end0:
	.section	.debug_str_offsets,"",@progbits
	.word	40                              # Length of String Offsets Set
	.half	5
	.half	0
.Lstr_offsets_base0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)" # string offset=0
.Linfo_string1:
	.asciz	"utils.c"                       # string offset=114
.Linfo_string2:
	.asciz	"/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" # string offset=122
.Linfo_string3:
	.asciz	"global_seed"                   # string offset=237
.Linfo_string4:
	.asciz	"int"                           # string offset=249
.Linfo_string5:
	.asciz	"__omp_offloading_10303_71206ba_init_omp_target_l9_debug__" # string offset=253
.Linfo_string6:
	.asciz	"__omp_offloading_10303_71206ba_init_omp_target_l9" # string offset=311
.Linfo_string7:
	.asciz	"init_omp_target"               # string offset=361
.Linfo_string8:
	.asciz	".omp_offloading.requires_reg"  # string offset=377
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
	.section	.debug_addr,"",@progbits
	.word	.Ldebug_addr_end0-.Ldebug_addr_start0 # Length of contribution
.Ldebug_addr_start0:
	.half	5                               # DWARF version number
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
.Laddr_table_base0:
	.quad	global_seed
	.quad	.Lfunc_begin0
	.quad	.Ltmp2
	.quad	.Lfunc_begin1
.Ldebug_addr_end0:
	.ident	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym .omp_offloading.requires_reg
	.addrsig_sym .__omp_offloading_10303_71206ba_init_omp_target_l9.region_id
	.section	.debug_line,"",@progbits
.Lline_table_start0:
