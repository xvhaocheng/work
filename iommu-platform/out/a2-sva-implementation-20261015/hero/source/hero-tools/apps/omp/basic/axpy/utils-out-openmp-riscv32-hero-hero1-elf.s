	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_zfh1p0_xdma0p1_xfalthalf0p1_xfaltquarter0p1_xfauxalthalf0p1_xfauxaltquarter0p1_xfauxhalf0p1_xfauxquarter0p1_xfauxvecalthalf0p1_xfauxvecaltquarter0p1_xfauxvechalf0p1_xfauxvecquarter0p1_xfauxvecsingle0p1_xfexpauxvecalthalf0p1_xfexpauxvecaltquarter0p1_xfexpauxvechalf0p1_xfexpauxvecquarter0p1_xfquarter0p1_xfrep0p1_xfvecalthalf0p1_xfvecaltquarter0p1_xfvechalf0p1_xfvecquarter0p1_xfvecsingle0p1_xssr0p1"
	.file	"utils.c"
	.weak	__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9 # -- Begin function __wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9
	.p2align	2
	.type	__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9,@function
__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9: # @__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9
.Lfunc_begin0:
	.file	0 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" "utils.c" md5 0x41dab1e1e78654b5843ae00cf03cf08d
	.file	1 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp" "common/utils.c"
	.loc	1 9 0                           # common/utils.c:9:0
	.cfi_startproc
# %bb.0:
	.loc	1 10 5 prologue_end             # common/utils.c:10:5
	#APP
	nop
	#NO_APP
.Ltmp0:
	.loc	1 9 1                           # common/utils.c:9:1
	ret
.Ltmp1:
.Lfunc_end0:
	.size	__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9, .Lfunc_end0-__wrapped_omp__omp_offloading_10303_71206ba_init_omp_target_l9
	.cfi_endproc
                                        # -- End function
	.weak	__omp_offloading_10303_71206ba_init_omp_target_l9 # -- Begin function __omp_offloading_10303_71206ba_init_omp_target_l9
	.p2align	2
	.type	__omp_offloading_10303_71206ba_init_omp_target_l9,@function
__omp_offloading_10303_71206ba_init_omp_target_l9: # @__omp_offloading_10303_71206ba_init_omp_target_l9
.Lfunc_begin1:
	.cfi_startproc
# %bb.0:                                # %entry
	#APP
	nop
	#NO_APP
	ret
.Lfunc_end1:
	.size	__omp_offloading_10303_71206ba_init_omp_target_l9, .Lfunc_end1-__omp_offloading_10303_71206ba_init_omp_target_l9
	.cfi_endproc
                                        # -- End function
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
	.word	__omp_offloading_10303_71206ba_init_omp_target_l9
	.word	.omp_offloading.entry_name
	.word	0                               # 0x0
	.word	0                               # 0x0
	.word	0                               # 0x0
	.size	.omp_offloading.entry.__omp_offloading_10303_71206ba_init_omp_target_l9, 20

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
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	2                               # Abbreviation Code
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
	.byte	3                               # Abbreviation Code
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
	.byte	4                               # Abbreviation Code
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
	.byte	0                               # EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.word	.Ldebug_info_end0-.Ldebug_info_start0 # Length of Unit
.Ldebug_info_start0:
	.half	5                               # DWARF version number
	.byte	1                               # DWARF Unit Type
	.byte	4                               # Address Size (in bytes)
	.word	.debug_abbrev                   # Offset Into Abbrev. Section
	.byte	1                               # Abbrev [1] 0xc:0x35 DW_TAG_compile_unit
	.byte	0                               # DW_AT_producer
	.half	12                              # DW_AT_language
	.byte	1                               # DW_AT_name
	.word	.Lstr_offsets_base0             # DW_AT_str_offsets_base
	.word	.Lline_table_start0             # DW_AT_stmt_list
	.byte	2                               # DW_AT_comp_dir
	.byte	0                               # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.word	.Laddr_table_base0              # DW_AT_addr_base
	.byte	2                               # Abbrev [2] 0x23:0x4 DW_TAG_subprogram
	.byte	3                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	10                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_inline
	.byte	3                               # Abbrev [3] 0x27:0x19 DW_TAG_subprogram
	.byte	0                               # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	4                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	9                               # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	4                               # Abbrev [4] 0x32:0xd DW_TAG_inlined_subroutine
	.word	35                              # DW_AT_abstract_origin
	.byte	0                               # DW_AT_low_pc
	.word	.Ltmp0-.Lfunc_begin0            # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	9                               # DW_AT_call_line
	.byte	1                               # DW_AT_call_column
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_str_offsets,"",@progbits
	.word	24                              # Length of String Offsets Set
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
	.asciz	"__omp_offloading_10303_71206ba_init_omp_target_l9_debug__" # string offset=237
.Linfo_string4:
	.asciz	"__omp_offloading_10303_71206ba_init_omp_target_l9" # string offset=295
	.section	.debug_str_offsets,"",@progbits
	.word	.Linfo_string0
	.word	.Linfo_string1
	.word	.Linfo_string2
	.word	.Linfo_string3
	.word	.Linfo_string4
	.section	.debug_addr,"",@progbits
	.word	.Ldebug_addr_end0-.Ldebug_addr_start0 # Length of contribution
.Ldebug_addr_start0:
	.half	5                               # DWARF version number
	.byte	4                               # Address size
	.byte	0                               # Segment selector size
.Laddr_table_base0:
	.word	.Lfunc_begin0
.Ldebug_addr_end0:
	.ident	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __omp_offloading_10303_71206ba_init_omp_target_l9
	.section	.debug_line,"",@progbits
.Lline_table_start0:
