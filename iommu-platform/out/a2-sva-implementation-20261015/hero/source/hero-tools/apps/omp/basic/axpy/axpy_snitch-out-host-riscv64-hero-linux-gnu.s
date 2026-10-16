	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.file	"axpy_snitch.c"
	.file	0 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy" "axpy_snitch.c" md5 0xe4db55a76ea4d57c31fe6e72474c39e7
	.file	1 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/rv64-ubuntu-sysroot/usr/include/bits/types.h"
	.file	2 "/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero" "build/rv64-ubuntu-sysroot/usr/include/bits/stdint-uintn.h"
	.globl	axpy                            # -- Begin function axpy
	.p2align	1
	.type	axpy,@function
axpy:                                   # @axpy
.Lfunc_begin0:
	.file	3 "axpy_snitch.c"
	.loc	3 17 0                          # axpy_snitch.c:17:0
	.cfi_sections .debug_frame
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: axpy:x_phys <- $x10
	#DEBUG_VALUE: axpy:y_phys <- $x11
	#DEBUG_VALUE: axpy:alpha <- $f10_f
	#DEBUG_VALUE: axpy:n <- $x12
	addi	sp, sp, -128
	.cfi_def_cfa_offset 128
	sd	ra, 120(sp)                     # 8-byte Folded Spill
	.cfi_offset ra, -8
	sd	a0, 112(sp)
	sd	a1, 104(sp)
	fsw	fa0, 100(sp)
	sw	a2, 96(sp)
.Ltmp0:
	.loc	3 18 10 prologue_end            # axpy_snitch.c:18:10
	seqz	a0, a0
.Ltmp1:
	.loc	3 0 10 is_stmt 0                # axpy_snitch.c:0:10
	seqz	a1, a1
.Ltmp2:
	.loc	3 18 17                         # axpy_snitch.c:18:17
	or	a0, a0, a1
	seqz	a1, a2
	or	a0, a0, a1
	andi	a1, a2, 511
	snez	a1, a1
	or	a1, a1, a0
	li	a0, -1
	bnez	a1, .LBB0_2
.Ltmp3:
# %bb.1:
	#DEBUG_VALUE: axpy:n <- $x12
	#DEBUG_VALUE: axpy:alpha <- $f10_f
	.loc	3 0 17                          # axpy_snitch.c:0:17
	addi	a1, sp, 112
	.loc	3 19 1 is_stmt 1                # axpy_snitch.c:19:1
	sd	a1, 64(sp)
	sd	a1, 32(sp)
	addi	a1, sp, 104
	sd	a1, 72(sp)
	sd	a1, 40(sp)
	addi	a1, sp, 100
	sd	a1, 80(sp)
	sd	a1, 48(sp)
	addi	a1, sp, 96
	sd	a1, 88(sp)
	sd	a1, 56(sp)
	sd	a0, 16(sp)
	lui	a0, %hi(.L.offload_mapnames)
	addi	a0, a0, %lo(.L.offload_mapnames)
	sd	a0, 0(sp)
	lui	a0, %hi(.L__unnamed_1)
	addi	a0, a0, %lo(.L__unnamed_1)
	lui	a1, %hi(.__omp_offloading_10303_7120698_axpy_l19.region_id)
	addi	a2, a1, %lo(.__omp_offloading_10303_7120698_axpy_l19.region_id)
.Ltmp4:
	lui	a1, %hi(.L.offload_sizes)
	addi	a6, a1, %lo(.L.offload_sizes)
	lui	a1, %hi(.L.offload_maptypes)
	addi	a7, a1, %lo(.L.offload_maptypes)
	li	a1, 1
	li	a3, 4
	addi	a4, sp, 64
	addi	a5, sp, 32
	sd	zero, 8(sp)
	call	__tgt_target_mapper@plt
.Ltmp5:
	.loc	3 0 1 is_stmt 0                 # axpy_snitch.c:0:1
	li	a0, 0
	.loc	3 30 1 is_stmt 1                # axpy_snitch.c:30:1
	sext.w	a0, a0
	ld	ra, 120(sp)                     # 8-byte Folded Reload
	addi	sp, sp, 128
	ret
.LBB0_2:
.Ltmp6:
	#DEBUG_VALUE: axpy:n <- $x12
	#DEBUG_VALUE: axpy:alpha <- $f10_f
	.loc	3 0 1 is_stmt 0                 # axpy_snitch.c:0:1
	srli	a0, a0, 32
	.loc	3 30 1                          # axpy_snitch.c:30:1
	sext.w	a0, a0
	ld	ra, 120(sp)                     # 8-byte Folded Reload
	addi	sp, sp, 128
	ret
.Ltmp7:
.Lfunc_end0:
	.size	axpy, .Lfunc_end0-axpy
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
.Ltmp8:
	.loc	3 0 1                           # axpy_snitch.c:0:1
	tail	__tgt_register_requires@plt
.Ltmp9:
.Lfunc_end1:
	.size	.omp_offloading.requires_reg, .Lfunc_end1-.omp_offloading.requires_reg
	.cfi_endproc
                                        # -- End function
	.type	.__omp_offloading_10303_7120698_axpy_l19.region_id,@object # @.__omp_offloading_10303_7120698_axpy_l19.region_id
	.section	.rodata,"a",@progbits
	.weak	.__omp_offloading_10303_7120698_axpy_l19.region_id
.__omp_offloading_10303_7120698_axpy_l19.region_id:
	.byte	0                               # 0x0
	.size	.__omp_offloading_10303_7120698_axpy_l19.region_id, 1

	.type	.L.offload_sizes,@object        # @.offload_sizes
	.section	.rodata.cst32,"aM",@progbits,32
	.p2align	4
.L.offload_sizes:
	.quad	8                               # 0x8
	.quad	8                               # 0x8
	.quad	4                               # 0x4
	.quad	4                               # 0x4
	.size	.L.offload_sizes, 32

	.type	.L.offload_maptypes,@object     # @.offload_maptypes
	.p2align	4
.L.offload_maptypes:
	.quad	33                              # 0x21
	.quad	33                              # 0x21
	.quad	33                              # 0x21
	.quad	33                              # 0x21
	.size	.L.offload_maptypes, 32

	.type	.L__unnamed_2,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_2:
	.asciz	";x_phys;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;19;;"
	.size	.L__unnamed_2, 145

	.type	.L__unnamed_3,@object           # @1
.L__unnamed_3:
	.asciz	";y_phys;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;36;;"
	.size	.L__unnamed_3, 145

	.type	.L__unnamed_4,@object           # @2
.L__unnamed_4:
	.asciz	";alpha;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;50;;"
	.size	.L__unnamed_4, 144

	.type	.L__unnamed_5,@object           # @3
.L__unnamed_5:
	.asciz	";n;/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;17;66;;"
	.size	.L__unnamed_5, 140

	.type	.L.offload_mapnames,@object     # @.offload_mapnames
	.section	.rodata,"a",@progbits
	.p2align	4
.L.offload_mapnames:
	.quad	.L__unnamed_2
	.quad	.L__unnamed_3
	.quad	.L__unnamed_4
	.quad	.L__unnamed_5
	.size	.L.offload_mapnames, 32

	.type	.L__unnamed_6,@object           # @4
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_6:
	.asciz	";/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c;axpy;19;1;;"
	.size	.L__unnamed_6, 142

	.type	.L__unnamed_1,@object           # @5
	.section	.rodata,"a",@progbits
	.p2align	3
.L__unnamed_1:
	.word	0                               # 0x0
	.word	2                               # 0x2
	.word	0                               # 0x0
	.word	141                             # 0x8d
	.quad	.L__unnamed_6
	.size	.L__unnamed_1, 24

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
	.quad	.__omp_offloading_10303_7120698_axpy_l19.region_id
	.quad	.omp_offloading.entry_name
	.quad	0                               # 0x0
	.word	0                               # 0x0
	.word	0                               # 0x0
	.size	.omp_offloading.entry.__omp_offloading_10303_7120698_axpy_l19, 32

	.section	.init_array.0,"aw",@init_array
	.p2align	3
	.quad	.omp_offloading.requires_reg
	.section	.debug_loclists,"",@progbits
	.word	.Ldebug_list_header_end0-.Ldebug_list_header_start0 # Length
.Ldebug_list_header_start0:
	.half	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.word	4                               # Offset entry count
.Lloclists_table_base0:
	.word	.Ldebug_loc0-.Lloclists_table_base0
	.word	.Ldebug_loc1-.Lloclists_table_base0
	.word	.Ldebug_loc2-.Lloclists_table_base0
	.word	.Ldebug_loc3-.Lloclists_table_base0
.Ldebug_loc0:
	.byte	3                               # DW_LLE_startx_length
	.byte	0                               #   start index
	.uleb128 .Ltmp1-.Lfunc_begin0           #   length
	.byte	1                               # Loc expr size
	.byte	90                              # DW_OP_reg10
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc1:
	.byte	3                               # DW_LLE_startx_length
	.byte	0                               #   start index
	.uleb128 .Ltmp2-.Lfunc_begin0           #   length
	.byte	1                               # Loc expr size
	.byte	91                              # DW_OP_reg11
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc2:
	.byte	1                               # DW_LLE_base_addressx
	.byte	0                               #   base address index
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp5-.Lfunc_begin0           #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	42                              # 42
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp6-.Lfunc_begin0           #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	144                             # DW_OP_regx
	.byte	42                              # 42
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc3:
	.byte	1                               # DW_LLE_base_addressx
	.byte	0                               #   base address index
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp4-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	92                              # DW_OP_reg12
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp6-.Lfunc_begin0           #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	1                               # Loc expr size
	.byte	92                              # DW_OP_reg12
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
	.byte	1                               # DW_FORM_addr
	.byte	85                              # DW_AT_ranges
	.byte	35                              # DW_FORM_rnglistx
	.byte	115                             # DW_AT_addr_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	116                             # DW_AT_rnglists_base
	.byte	23                              # DW_FORM_sec_offset
	.ascii	"\214\001"                      # DW_AT_loclists_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	2                               # Abbreviation Code
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
	.byte	5                               # Abbreviation Code
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
	.byte	6                               # Abbreviation Code
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
	.byte	1                               # Abbrev [1] 0xc:0x91 DW_TAG_compile_unit
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
	.word	.Lloclists_table_base0          # DW_AT_loclists_base
	.byte	2                               # Abbrev [2] 0x2f:0x8 DW_TAG_typedef
	.word	55                              # DW_AT_type
	.byte	5                               # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	27                              # DW_AT_decl_line
	.byte	2                               # Abbrev [2] 0x37:0x8 DW_TAG_typedef
	.word	63                              # DW_AT_type
	.byte	4                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	45                              # DW_AT_decl_line
	.byte	3                               # Abbrev [3] 0x3f:0x4 DW_TAG_base_type
	.byte	3                               # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	3                               # Abbrev [3] 0x43:0x4 DW_TAG_base_type
	.byte	6                               # DW_AT_name
	.byte	4                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # Abbrev [2] 0x47:0x8 DW_TAG_typedef
	.word	79                              # DW_AT_type
	.byte	9                               # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.byte	2                               # Abbrev [2] 0x4f:0x8 DW_TAG_typedef
	.word	87                              # DW_AT_type
	.byte	8                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
	.byte	3                               # Abbrev [3] 0x57:0x4 DW_TAG_base_type
	.byte	7                               # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	4                               # Abbrev [4] 0x5b:0x34 DW_TAG_subprogram
	.byte	0                               # DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	10                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.word	152                             # DW_AT_type
                                        # DW_AT_external
	.byte	5                               # Abbrev [5] 0x6a:0x9 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_location
	.byte	13                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	47                              # DW_AT_type
	.byte	5                               # Abbrev [5] 0x73:0x9 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	14                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	47                              # DW_AT_type
	.byte	5                               # Abbrev [5] 0x7c:0x9 DW_TAG_formal_parameter
	.byte	2                               # DW_AT_location
	.byte	15                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	67                              # DW_AT_type
	.byte	5                               # Abbrev [5] 0x85:0x9 DW_TAG_formal_parameter
	.byte	3                               # DW_AT_location
	.byte	16                              # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	17                              # DW_AT_decl_line
	.word	71                              # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	6                               # Abbrev [6] 0x8f:0x9 DW_TAG_subprogram
	.byte	1                               # DW_AT_low_pc
	.word	.Lfunc_end1-.Lfunc_begin1       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	82
                                        # DW_AT_call_all_calls
	.byte	12                              # DW_AT_linkage_name
                                        # DW_AT_artificial
	.byte	3                               # Abbrev [3] 0x98:0x4 DW_TAG_base_type
	.byte	11                              # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_rnglists,"",@progbits
	.word	.Ldebug_list_header_end1-.Ldebug_list_header_start1 # Length
.Ldebug_list_header_start1:
	.half	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.word	1                               # Offset entry count
.Lrnglists_table_base0:
	.word	.Ldebug_ranges0-.Lrnglists_table_base0
.Ldebug_ranges0:
	.byte	3                               # DW_RLE_startx_length
	.byte	0                               #   start index
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   length
	.byte	3                               # DW_RLE_startx_length
	.byte	1                               #   start index
	.uleb128 .Lfunc_end1-.Lfunc_begin1      #   length
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_list_header_end1:
	.section	.debug_str_offsets,"",@progbits
	.word	72                              # Length of String Offsets Set
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
	.asciz	"unsigned long"                 # string offset=243
.Linfo_string4:
	.asciz	"__uint64_t"                    # string offset=257
.Linfo_string5:
	.asciz	"uint64_t"                      # string offset=268
.Linfo_string6:
	.asciz	"float"                         # string offset=277
.Linfo_string7:
	.asciz	"unsigned int"                  # string offset=283
.Linfo_string8:
	.asciz	"__uint32_t"                    # string offset=296
.Linfo_string9:
	.asciz	"uint32_t"                      # string offset=307
.Linfo_string10:
	.asciz	"axpy"                          # string offset=316
.Linfo_string11:
	.asciz	"int"                           # string offset=321
.Linfo_string12:
	.asciz	".omp_offloading.requires_reg"  # string offset=325
.Linfo_string13:
	.asciz	"x_phys"                        # string offset=354
.Linfo_string14:
	.asciz	"y_phys"                        # string offset=361
.Linfo_string15:
	.asciz	"alpha"                         # string offset=368
.Linfo_string16:
	.asciz	"n"                             # string offset=374
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
	.section	.debug_addr,"",@progbits
	.word	.Ldebug_addr_end0-.Ldebug_addr_start0 # Length of contribution
.Ldebug_addr_start0:
	.half	5                               # DWARF version number
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
.Laddr_table_base0:
	.quad	.Lfunc_begin0
	.quad	.Lfunc_begin1
.Ldebug_addr_end0:
	.ident	"clang version 15.0.0 (https://github.com/pulp-platform/llvm-project.git d8cffb8d7294483b45732283719e250116fbe6c0)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym .omp_offloading.requires_reg
	.addrsig_sym .__omp_offloading_10303_7120698_axpy_l19.region_id
	.addrsig_sym .L.offload_mapnames
	.section	.debug_line,"",@progbits
.Lline_table_start0:
