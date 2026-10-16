// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VNONBLOCK_BANKED_TOP_H_
#define _VNONBLOCK_BANKED_TOP_H_  // guard

#include "verilated_heavy.h"

//==========

class Vnonblock_banked_top__Syms;
class Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4;
class Vnonblock_banked_top_axi_demux_id_counters__pi33;


//----------

VL_MODULE(Vnonblock_banked_top) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4* __PVT__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank;
    Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4* __PVT__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank;
    Vnonblock_banked_top_axi_demux_id_counters__pi33* __PVT__nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter;
    Vnonblock_banked_top_axi_demux_id_counters__pi33* __PVT__nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter;
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(tr_aw_valid,0,0);
    VL_IN8(tr_aw_id,3,0);
    VL_IN8(tr_aw_len,7,0);
    VL_IN8(tr_aw_size,2,0);
    VL_IN8(tr_aw_burst,1,0);
    VL_IN8(tr_aw_prot,2,0);
    VL_IN8(tr_aw_user,0,0);
    VL_IN8(tr_aw_ssid_valid,0,0);
    VL_IN8(tr_aw_mmu_atst,0,0);
    VL_IN8(tr_aw_mmu_valid,0,0);
    VL_IN8(tr_ar_valid,0,0);
    VL_IN8(tr_ar_id,3,0);
    VL_IN8(tr_ar_len,7,0);
    VL_IN8(tr_ar_size,2,0);
    VL_IN8(tr_ar_burst,1,0);
    VL_IN8(tr_ar_prot,2,0);
    VL_IN8(tr_ar_user,0,0);
    VL_IN8(tr_ar_ssid_valid,0,0);
    VL_IN8(tr_ar_mmu_atst,0,0);
    VL_IN8(tr_ar_mmu_valid,0,0);
    VL_IN8(tr_w_valid,0,0);
    VL_IN8(tr_w_strb,7,0);
    VL_IN8(tr_w_last,0,0);
    VL_IN8(tr_w_user,0,0);
    VL_IN8(tr_b_ready,0,0);
    VL_IN8(tr_r_ready,0,0);
    VL_OUT8(tr_aw_ready,0,0);
    VL_OUT8(tr_ar_ready,0,0);
    VL_OUT8(tr_w_ready,0,0);
    VL_OUT8(tr_b_valid,0,0);
    VL_OUT8(tr_b_id,3,0);
    VL_OUT8(tr_b_resp,1,0);
    VL_OUT8(tr_r_valid,0,0);
    VL_OUT8(tr_r_id,3,0);
    VL_OUT8(tr_r_resp,1,0);
    VL_OUT8(tr_r_last,0,0);
    VL_OUT8(comp_aw_valid,0,0);
    VL_OUT8(comp_aw_id,3,0);
    VL_OUT8(comp_aw_len,7,0);
    VL_OUT8(comp_aw_size,2,0);
    VL_OUT8(comp_aw_burst,1,0);
    VL_OUT8(comp_aw_prot,2,0);
    VL_OUT8(comp_aw_user,0,0);
    VL_OUT8(comp_ar_valid,0,0);
    VL_OUT8(comp_ar_id,3,0);
    VL_OUT8(comp_ar_len,7,0);
    VL_OUT8(comp_ar_size,2,0);
    VL_OUT8(comp_ar_burst,1,0);
    VL_OUT8(comp_ar_prot,2,0);
    VL_OUT8(comp_ar_user,0,0);
    VL_OUT8(comp_w_valid,0,0);
    VL_OUT8(comp_w_strb,7,0);
    VL_OUT8(comp_w_last,0,0);
    VL_OUT8(comp_b_ready,0,0);
    VL_OUT8(comp_r_ready,0,0);
    VL_IN8(comp_aw_ready,0,0);
    VL_IN8(comp_ar_ready,0,0);
    VL_IN8(comp_w_ready,0,0);
    VL_IN8(comp_b_valid,0,0);
    VL_IN8(comp_b_id,3,0);
    VL_IN8(comp_b_resp,1,0);
    VL_IN8(comp_r_valid,0,0);
    VL_IN8(comp_r_id,3,0);
    VL_IN8(comp_r_resp,1,0);
    VL_IN8(comp_r_last,0,0);
    VL_OUT8(ddtw_ar_valid,0,0);
    VL_OUT8(ddtw_ar_id,3,0);
    VL_OUT8(ddtw_ar_len,7,0);
    VL_OUT8(ddtw_ar_size,2,0);
    VL_OUT8(ddtw_ar_burst,1,0);
    VL_IN8(ddtw_ar_ready,0,0);
    VL_OUT8(ddtw_r_ready,0,0);
    VL_IN8(ddtw_r_valid,0,0);
    VL_IN8(ddtw_r_id,3,0);
    VL_IN8(ddtw_r_resp,1,0);
    VL_IN8(ddtw_r_last,0,0);
    VL_OUT8(ptw_ar_valid,0,0);
    VL_OUT8(ptw_ar_id,3,0);
    VL_OUT8(ptw_ar_len,7,0);
    VL_OUT8(ptw_ar_size,2,0);
    VL_OUT8(ptw_ar_burst,1,0);
    VL_IN8(ptw_ar_ready,0,0);
    VL_OUT8(ptw_r_ready,0,0);
    VL_IN8(ptw_r_valid,0,0);
    VL_IN8(ptw_r_id,3,0);
    VL_IN8(ptw_r_resp,1,0);
    VL_IN8(ptw_r_last,0,0);
    VL_IN8(nonblock_enable_i,0,0);
    VL_OUT8(nonblock_fire_o,0,0);
    VL_OUT8(nonblock_busy_o,0,0);
    VL_OUT8(in_flight_o,0,0);
    VL_IN(tr_aw_did,23,0);
    VL_IN(tr_aw_subid,19,0);
    VL_IN(tr_ar_did,23,0);
    VL_IN(tr_ar_subid,19,0);
    VL_IN64(tr_aw_addr,63,0);
    VL_IN64(tr_ar_addr,63,0);
    VL_IN64(tr_w_data,63,0);
    VL_OUT64(tr_r_data,63,0);
    VL_OUT64(comp_aw_addr,63,0);
    VL_OUT64(comp_ar_addr,63,0);
    VL_OUT64(comp_w_data,63,0);
    VL_IN64(comp_r_data,63,0);
    VL_OUT64(ddtw_ar_addr,63,0);
    VL_IN64(ddtw_r_data,63,0);
    VL_OUT64(ptw_ar_addr,63,0);
    VL_IN64(ptw_r_data,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ nonblock_banked_top__DOT__trans_req_ready;
        CData/*0:0*/ nonblock_banked_top__DOT__trans_resp_ready;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_trans_req_valid;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_trans_req_ready;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_valid;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_ready;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_busy;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__w_accept;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__w_last_accept;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__b_return;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__w_partial_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__w_select;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies;
        CData/*3:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q;
        CData/*3:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
    };
    struct {
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__rr_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_aw_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_aw_readies;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_w_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_w_readies;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_b_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_b_readies;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_ar_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_ar_readies;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_r_valids;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_r_readies;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__mst_aw_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__aw_ready;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__lock_aw_valid_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__lock_aw_valid_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__load_aw_lock;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__w_fifo_push;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__w_fifo_pop;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__mst_w_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__rr_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__req_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__gate_clock;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__read_pointer_n;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__read_pointer_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__write_pointer_n;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__write_pointer_q;
        CData/*2:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__status_cnt_n;
        CData/*2:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__status_cnt_q;
        CData/*3:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__mem_n;
        CData/*3:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_w_fifo__DOT__mem_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__rr_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__req_d;
    };
    struct {
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_ar_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ats_trans_resp_valid_o;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__debug_resp_valid_o;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__state_q;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__state_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__slow_resp_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__normal_gate;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__n_intercept;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__n_accept;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__n_busy;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__n_ddtc_lu;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__cache_ddtc_hit;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__cache_iotlb_hit;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__cache_iotlb_en1;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__cache_iotlb_en2;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__arb_req_ready;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__req_valid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__req_ready;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__en_1S_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__en_1S_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__en_2S_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__en_2S_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__r_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__r_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__w_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__w_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__x_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__x_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__u_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__u_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__g_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__g_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__sum_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__sum_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__t2gpa_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__t2gpa_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__bypass_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__bypass_n;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__range_q;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__range_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddtc_lu;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddtc_lu_hit;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__iotlb_lu_hit;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddtw_init;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddtw_error;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ptw_init;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ptw_error;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddt_imp_trans_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddt_imp_trans_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__pdt_imp_trans_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__pdt_imp_trans_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__imp_done;
    };
    struct {
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__imp_err;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gpaddr_is_msi;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__is_translated;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__pid_invalid;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__is_store;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__is_rx;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__error_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__error_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ignore_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__ignore_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__is_mrif_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__is_mrif_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__iova_is_msi;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__msi_ptw_init;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_arbiter__DOT__i_arb__DOT__gen_prio_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gnt_nodes;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_arbiter__DOT__i_arb__DOT__gen_prio_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_nodes;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__rr_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
        CData/*7:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__lu_hit;
        CData/*7:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__replace_en;
        CData/*6:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__plru_tree_q;
        CData/*6:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__plru_tree_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__plru_replacement__DOT__unnamedblk6__DOT__unnamedblk7__DOT__en;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__state_q;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__state_n;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__ddt_lvl_q;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__ddt_lvl_n;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__entry_cnt_q;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__entry_cnt_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__wait_rlast_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__wait_rlast_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__en_msi_check;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__msi_check_error;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__imp_active_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__imp_active_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__edge_trigger_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__edge_trigger_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__plru_replacement__DOT__unnamedblk6__DOT__unnamedblk7__DOT__en;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__state_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__state_n;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__main_lvl_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__main_lvl_n;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__s1_lvl_n;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__s1_lvl_q;
    };
    struct {
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__ptw_stage_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__ptw_stage_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__global_mapping_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__global_mapping_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__gpaddr_is_msi;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__implicit_trans_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__implicit_trans_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__pf_excep_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__pf_excep_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__pt_data_corrupt_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__pt_data_corrupt_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__edge_trigger_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__edge_trigger_n;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__state_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__start;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__dc_ok;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_state_q;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_state_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_error;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_wait_rlast_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_wait_rlast_n;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__edge_trigger_q;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__edge_trigger_n;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__gscid_q;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__gscid_n;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__iotlb_lu_gscid;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__ptw_cause;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__error_code_q;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__error_code_n;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__cause_q;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__cause_n;
        SData/*8:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vpn0;
        SData/*8:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vpn1;
        SData/*10:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vpn2;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__lu_hit;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__replace_en;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__match_gscid;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__match_pscid;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__match_stage;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__is_1G;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__is_2M;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vaddr_vpn0_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vaddr_vpn1_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vaddr_vpn2_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vaddr_4k_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vaddr_2M_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__vaddr_1G_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gpaddr_gppn0_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gpaddr_gppn1_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gpaddr_gppn2_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gpaddr_4k_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gpaddr_2M_match;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gpaddr_1G_match;
        SData/*14:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__plru_tree_q;
        SData/*14:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__plru_tree_n;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__cause_q;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__cause_n;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_cause_q;
        SData/*11:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__bt_cause_n;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__gscid_q;
        SData/*15:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__gscid_n;
        IData/*31:0*/ nonblock_banked_top__DOT__i_banked__DOT__inflight_aw_q;
        IData/*31:0*/ nonblock_banked_top__DOT__i_banked__DOT__inflight_ar_q;
        IData/*31:0*/ nonblock_banked_top__DOT__i_banked__DOT__inflight_w_q;
    };
    struct {
        IData/*19:0*/ nonblock_banked_top__DOT__i_tl__DOT__pscid_q;
        IData/*19:0*/ nonblock_banked_top__DOT__i_tl__DOT__pscid_n;
        IData/*23:0*/ nonblock_banked_top__DOT__i_tl__DOT__ddtc_lu_did;
        IData/*28:0*/ nonblock_banked_top__DOT__i_tl__DOT__iotlb_lu_vpn;
        IData/*19:0*/ nonblock_banked_top__DOT__i_tl__DOT__iotlb_lu_pscid;
        WData/*199:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__tags_q[7];
        WData/*199:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__tags_n[7];
        WData/*463:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__gppn[15];
        IData/*31:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__update_flush__DOT__unnamedblk2__DOT__i;
        IData/*28:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__vpn_q;
        IData/*28:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__vpn_n;
        IData/*19:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__pscid_q;
        IData/*19:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__pscid_n;
        WData/*374:0*/ nonblock_banked_top__DOT__tr_req[12];
        WData/*83:0*/ nonblock_banked_top__DOT__comp_resp[3];
        WData/*83:0*/ nonblock_banked_top__DOT__ddtw_axi_resp[3];
        WData/*83:0*/ nonblock_banked_top__DOT__ptw_axi_resp[3];
        QData/*37:0*/ nonblock_banked_top__DOT__capabilities_i;
        QData/*47:0*/ nonblock_banked_top__DOT__ddtp_i;
        WData/*167:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_tr_resp[6];
        WData/*561:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_comp_req[18];
        WData/*235:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_trans_req_data[8];
        WData/*369:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[12];
        WData/*235:0*/ nonblock_banked_top__DOT__i_banked__DOT__bank_req_data[8];
        WData/*85:0*/ nonblock_banked_top__DOT__i_banked__DOT__mux_mst_resp[3];
        WData/*374:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__slv_req_cut[12];
        WData/*117:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_trans_req_arb__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__data_nodes[4];
        WData/*209:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_aw_chans[7];
        WData/*147:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_w_chans[5];
        WData/*197:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__slv_ar_chans[7];
        WData/*73:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__mst_w_chan[3];
        WData/*104:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT__gen_mux__DOT__i_aw_arbiter__DOT__gen_arbiter__DOT__data_nodes[4];
        WData/*184:0*/ nonblock_banked_top__DOT__i_tl__DOT__slow_resp_data[6];
        WData/*511:0*/ nonblock_banked_top__DOT__i_tl__DOT__dc_q[16];
        WData/*511:0*/ nonblock_banked_top__DOT__i_tl__DOT__dc_n[16];
        WData/*127:0*/ nonblock_banked_top__DOT__i_tl__DOT__pc_q[4];
        WData/*127:0*/ nonblock_banked_top__DOT__i_tl__DOT__pc_n[4];
        QData/*43:0*/ nonblock_banked_top__DOT__i_tl__DOT__iohgatp_ppn_q;
        QData/*43:0*/ nonblock_banked_top__DOT__i_tl__DOT__iohgatp_ppn_n;
        QData/*43:0*/ nonblock_banked_top__DOT__i_tl__DOT__iosatp_ppn_q;
        QData/*43:0*/ nonblock_banked_top__DOT__i_tl__DOT__iosatp_ppn_n;
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__spaddr_q;
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__spaddr_n;
        WData/*103:0*/ nonblock_banked_top__DOT__i_tl__DOT__iotlb_lu_content[4];
        WData/*169:0*/ nonblock_banked_top__DOT__i_tl__DOT__ptw_iotlb_update[6];
        WData/*169:0*/ nonblock_banked_top__DOT__i_tl__DOT__msi_iotlb_update[6];
        WData/*169:0*/ nonblock_banked_top__DOT__i_tl__DOT__iotlb_update[6];
        QData/*43:0*/ nonblock_banked_top__DOT__i_tl__DOT__pdt_gppn_q;
        QData/*43:0*/ nonblock_banked_top__DOT__i_tl__DOT__pdt_gppn_n;
        QData/*51:0*/ nonblock_banked_top__DOT__i_tl__DOT__msi_1S_content;
        WData/*101:0*/ nonblock_banked_top__DOT__i_tl__DOT__mrif_q[4];
        WData/*101:0*/ nonblock_banked_top__DOT__i_tl__DOT__mrif_n[4];
        QData/*51:0*/ nonblock_banked_top__DOT__i_tl__DOT__msi_1S_data_q;
        QData/*51:0*/ nonblock_banked_top__DOT__i_tl__DOT__msi_1S_data_n;
        WData/*353:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_arbiter__DOT__i_arb__DOT__gen_prio_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__data_nodes[12];
        WData/*117:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[4];
        WData/*117:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[4];
        WData/*184:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_nonblock_response_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__data_nodes[6];
        WData/*4095:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__content_q[128];
        WData/*4095:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT__content_n[128];
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__ddtw_pptr_q;
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__ddtw_pptr_n;
        WData/*511:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__up_dc_content[16];
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_tc_q;
    };
    struct {
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_tc_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_iohgatp_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_iohgatp_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_ta_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_ta_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_fsc_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__dc_fsc_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msiptp_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msiptp_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msi_addr_mask_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msi_addr_mask_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msi_addr_patt_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msi_addr_patt_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msiptp;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msi_addr_mask;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_msi_addr_patt;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtw__DOT__gen_msi_support__DOT__dc_reserved;
        WData/*1087:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__tags_q[34];
        WData/*1087:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__tags_n[34];
        WData/*1663:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__content_q[52];
        WData/*1663:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT__content_n[52];
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__iova_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__iova_n;
        QData/*40:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__gpa_x_q;
        QData/*40:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__gpa_x_n;
        QData/*40:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__gpaddr_q;
        QData/*40:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__gpaddr_n;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__leaf_1Spte_q;
        QData/*63:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__leaf_1Spte_n;
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__ptw_pptr_q;
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__ptw_pptr_n;
        QData/*55:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__ptw_fsm_comb__DOT__pptr;
        QData/*40:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ptw__DOT__ptw_fsm_comb__DOT__final_gpa;
        WData/*117:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__req_q[4];
        WData/*511:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__dc_q[16];
        QData/*47:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__ddtp_q;
        WData/*184:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_nonblock_hit__DOT__i_hit__DOT__resp_q[6];
        QData/*51:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__content_1S_q;
        QData/*51:0*/ nonblock_banked_top__DOT__i_tl__DOT__gen_msi_support_tl__DOT__i_rv_iommu_msiptw__DOT__content_1S_n;
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT____Vlvbound6;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT____Vlvbound7;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT____Vlvbound11;
        CData/*0:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT____Vcellout__gen_mux__DOT__i_w_fifo__data_o;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellout__i_req_arbiter__inp_ready_o;
        CData/*2:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellinp__i_req_arbiter__inp_valid_i;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellout__i_nonblock_response_arbiter__inp_ready_o;
        CData/*1:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellinp__i_nonblock_response_arbiter__inp_valid_i;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellinp__i_rv_iommu_ddtc__touch_i;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellinp__i_rv_iommu_iotlb__touch_i;
        CData/*0:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellinp__i_rv_iommu_ptw__imp_init_i;
        CData/*0:0*/ __Vfunc_is_trans_1G__1__Vfuncout;
        CData/*0:0*/ __Vfunc_is_trans_1G__1__S1_en;
        CData/*0:0*/ __Vfunc_is_trans_1G__1__S2_en;
        CData/*0:0*/ __Vfunc_is_trans_1G__1__is_1S_1G;
        CData/*0:0*/ __Vfunc_is_trans_1G__1__is_2S_1G;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__Vfuncout;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__S1_en;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__S2_en;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__is_1S_1G;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__is_1S_2M;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__is_2S_1G;
        CData/*0:0*/ __Vfunc_is_trans_2M__2__is_2S_2M;
        CData/*0:0*/ __Vfunc_make_gppn__3__S1_en;
        CData/*0:0*/ __Vfunc_make_gppn__3__is_1G;
        CData/*0:0*/ __Vfunc_make_gppn__3__is_2M;
        CData/*0:0*/ __Vclklast__TOP__clk_i;
        CData/*0:0*/ __Vclklast__TOP__rst_ni;
        CData/*2:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_tl__DOT__i_req_arbiter__DOT__i_arb__DOT__gen_prio_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gnt_nodes;
        CData/*2:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_tl__DOT__i_req_arbiter__DOT__i_arb__DOT__gen_prio_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_nodes;
        WData/*150:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT____Vlvbound1[5];
        WData/*73:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT____Vlvbound4[3];
        WData/*144:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT____Vlvbound8[5];
        IData/*24:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_ddtc__DOT____Vlvbound3;
        IData/*28:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT____Vlvbound1;
        IData/*28:0*/ __Vfunc_make_gppn__3__Vfuncout;
        IData/*26:0*/ __Vfunc_make_gppn__3__vpn;
        IData/*28:0*/ __Vfunc_make_gppn__3__gppn;
        IData/*31:0*/ __Vdly__nonblock_banked_top__DOT__i_banked__DOT__inflight_aw_q;
        IData/*31:0*/ __Vdly__nonblock_banked_top__DOT__i_banked__DOT__inflight_ar_q;
        IData/*31:0*/ __Vdly__nonblock_banked_top__DOT__i_banked__DOT__inflight_w_q;
        WData/*280:0*/ nonblock_banked_top__DOT____Vcellout__i_banked__comp_axi_req_o[9];
        WData/*83:0*/ nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_in_axi_demux__slv_resp_o[3];
        WData/*282:0*/ nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__mst_req_o[9];
        WData/*167:0*/ nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[6];
        WData/*749:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[24];
        WData/*83:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[3];
        WData/*98:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT____Vcellout__gen_mux__DOT__gen_id_prepend__BRA__0__KET____DOT__i_id_prepend__mst_ar_chans_o[4];
        WData/*104:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT____Vcellout__gen_mux__DOT__gen_id_prepend__BRA__0__KET____DOT__i_id_prepend__mst_aw_chans_o[4];
        WData/*98:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT____Vcellout__gen_mux__DOT__gen_id_prepend__BRA__1__KET____DOT__i_id_prepend__mst_ar_chans_o[4];
        WData/*104:0*/ nonblock_banked_top__DOT__i_banked__DOT__i_out_axi_mux__DOT____Vcellout__gen_mux__DOT__gen_id_prepend__BRA__1__KET____DOT__i_id_prepend__mst_aw_chans_o[4];
        WData/*511:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellout__i_rv_iommu_ddtc__lu_content_o[16];
        WData/*536:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellout__i_rv_iommu_ddtw__update_o[17];
        WData/*280:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellout__i_rv_iommu_ddtw__mem_req_o[9];
        WData/*280:0*/ nonblock_banked_top__DOT__i_tl__DOT____Vcellout__i_rv_iommu_ptw__mem_req_o[9];
        WData/*117:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_req_spill_register__DOT____Vcellout__spill_register_flushable_i__data_o[4];
        WData/*67:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT____Vlvbound13[3];
        WData/*103:0*/ nonblock_banked_top__DOT__i_tl__DOT__i_rv_iommu_iotlb__DOT____Vlvbound14[4];
        QData/*43:0*/ __Vfunc_make_gppn__3__ppn;
        WData/*83:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[3];
        WData/*353:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_tl__DOT__i_req_arbiter__DOT__i_arb__DOT__gen_prio_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__data_nodes[12];
        WData/*561:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[18];
        WData/*83:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[3];
        WData/*561:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[18];
    };
    struct {
        WData/*83:0*/ __Vchglast__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[3];
    };
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vnonblock_banked_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vnonblock_banked_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vnonblock_banked_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vnonblock_banked_top();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vnonblock_banked_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vnonblock_banked_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vnonblock_banked_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static void _combo__TOP__8(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static void _combo__TOP__9(Vnonblock_banked_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vnonblock_banked_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vnonblock_banked_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vnonblock_banked_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__10(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static void _sequent__TOP__4(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static void _sequent__TOP__5(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static void _sequent__TOP__6(Vnonblock_banked_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vnonblock_banked_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__3(Vnonblock_banked_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__7(Vnonblock_banked_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
