// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_RESEARCH_OBSERVER_H_
#define _VDMA_RESEARCH_OBSERVER_H_  // guard

#include "verilated.h"

//==========

class Vdma_research_observer__Syms;

//----------

VL_MODULE(Vdma_research_observer) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(cfg_valid_i,0,0);
    VL_IN8(cfg_write_i,0,0);
    VL_IN8(quiet_i,0,0);
    VL_IN8(supported_i,0,0);
    VL_IN8(update_i,0,0);
    VL_IN8(invalidate_i,0,0);
    VL_IN8(lookup_i,0,0);
    VL_IN8(lookup_hit_i,0,0);
    VL_IN8(lookup_stage1_i,0,0);
    VL_IN8(lookup_stage2_i,0,0);
    VL_IN8(ctl_state_i,2,0);
    VL_IN8(select_fire_i,0,0);
    VL_IN8(response_error_i,0,0);
    VL_IN8(issue_i,1,0);
    VL_IN8(done_i,1,0);
    VL_IN8(p4m_arm_i,0,0);
    VL_IN8(p4m_open_i,0,0);
    VL_IN8(p4m_close_i,0,0);
    VL_IN16(index_i,12,0);
    VL_IN16(replace_mask_i,15,0);
    VL_IN16(lookup_gscid_i,15,0);
    VL_IN(plru_i,31,0);
    VL_IN(update_did_i,23,0);
    VL_IN(lookup_did_i,23,0);
    VL_IN(lookup_pscid_i,19,0);
    VL_IN(ctl_did_i,23,0);
    VL_IN(select_did_i,23,0);
    VL_IN64(wdata_i,63,0);
    VL_OUT64(rdata_o,63,0);
    VL_IN64(lookup_iova_i,63,0);
    VL_INW(entries_i[16],127,0,4);
    VL_INW(entries_next_i[16],127,0,4);
    VL_IN64(issue_id_i[2],63,0);
    VL_IN64(done_id_i[2],63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*1:0*/ dma_research_observer__DOT__mode_q;
        CData/*3:0*/ dma_research_observer__DOT__cmd_q;
        CData/*2:0*/ dma_research_observer__DOT__e2_phase_q;
        CData/*0:0*/ dma_research_observer__DOT__e2_ovf_q;
        CData/*1:0*/ dma_research_observer__DOT__target_valid_q;
        CData/*1:0*/ dma_research_observer__DOT__target_ctx_valid_q;
        CData/*1:0*/ dma_research_observer__DOT__fp_done_q;
        CData/*2:0*/ dma_research_observer__DOT__snapshot_valid_q;
        CData/*0:0*/ dma_research_observer__DOT__e2a_first_seen_q;
        CData/*0:0*/ dma_research_observer__DOT__src_set_q;
        CData/*0:0*/ dma_research_observer__DOT__dst_set_q;
        CData/*1:0*/ dma_research_observer__DOT__lane_active_q;
        CData/*1:0*/ dma_research_observer__DOT__lane_in_window_q;
        CData/*0:0*/ dma_research_observer__DOT__cost_active_q;
        CData/*7:0*/ dma_research_observer__DOT__cost_cycles_q;
        CData/*7:0*/ dma_research_observer__DOT__cost_owner_err_q;
        CData/*7:0*/ dma_research_observer__DOT__cost_a_done_all_q;
        CData/*0:0*/ dma_research_observer__DOT__window_open_q;
        CData/*0:0*/ dma_research_observer__DOT__window_closed_q;
        CData/*7:0*/ dma_research_observer__DOT__win_cycles_q;
        CData/*0:0*/ dma_research_observer__DOT__carry_in_q;
        CData/*0:0*/ dma_research_observer__DOT__carry_out_q;
        CData/*7:0*/ dma_research_observer__DOT__eligible_q;
        CData/*0:0*/ dma_research_observer__DOT__sample_ovf_q;
        CData/*1:0*/ dma_research_observer__DOT__mode_n;
        CData/*3:0*/ dma_research_observer__DOT__cmd_n;
        CData/*2:0*/ dma_research_observer__DOT__e2_phase_n;
        CData/*0:0*/ dma_research_observer__DOT__e2_ovf_n;
        CData/*1:0*/ dma_research_observer__DOT__target_valid_n;
        CData/*1:0*/ dma_research_observer__DOT__target_ctx_valid_n;
        CData/*1:0*/ dma_research_observer__DOT__fp_done_n;
        CData/*2:0*/ dma_research_observer__DOT__snapshot_valid_n;
        CData/*0:0*/ dma_research_observer__DOT__e2a_first_seen_n;
        CData/*0:0*/ dma_research_observer__DOT__src_set_n;
        CData/*0:0*/ dma_research_observer__DOT__dst_set_n;
        CData/*1:0*/ dma_research_observer__DOT__lane_active_n;
        CData/*1:0*/ dma_research_observer__DOT__lane_in_window_n;
        CData/*0:0*/ dma_research_observer__DOT__cost_active_n;
        CData/*7:0*/ dma_research_observer__DOT__cost_cycles_n;
        CData/*7:0*/ dma_research_observer__DOT__cost_owner_err_n;
        CData/*7:0*/ dma_research_observer__DOT__cost_a_done_all_n;
        CData/*0:0*/ dma_research_observer__DOT__window_open_n;
        CData/*0:0*/ dma_research_observer__DOT__window_closed_n;
        CData/*7:0*/ dma_research_observer__DOT__win_cycles_n;
        CData/*0:0*/ dma_research_observer__DOT__carry_in_n;
        CData/*0:0*/ dma_research_observer__DOT__carry_out_n;
        CData/*7:0*/ dma_research_observer__DOT__eligible_n;
        CData/*0:0*/ dma_research_observer__DOT__sample_ovf_n;
        CData/*0:0*/ dma_research_observer__DOT__ctx_ok_both;
        CData/*0:0*/ dma_research_observer__DOT__e2_active_phase;
        CData/*0:0*/ dma_research_observer__DOT__cmd_wr;
        CData/*0:0*/ dma_research_observer__DOT__acc_warm;
        CData/*0:0*/ dma_research_observer__DOT__acc_a;
        CData/*0:0*/ dma_research_observer__DOT__acc_probe;
        CData/*0:0*/ dma_research_observer__DOT__acc_freeze;
        CData/*0:0*/ dma_research_observer__DOT__acc_clear;
        CData/*0:0*/ dma_research_observer__DOT__abort_cmd;
        CData/*0:0*/ dma_research_observer__DOT__lanes_en;
        CData/*0:0*/ dma_research_observer__DOT__a_issue_ok;
        CData/*0:0*/ dma_research_observer__DOT__a_done_ok;
        CData/*0:0*/ dma_research_observer__DOT__lane_proto_err;
        CData/*0:0*/ dma_research_observer__DOT__win_act;
        CData/*0:0*/ dma_research_observer__DOT__main_cyc;
        CData/*0:0*/ dma_research_observer__DOT__win_act_issue;
    };
    struct {
        CData/*0:0*/ dma_research_observer__DOT__main_known;
        CData/*0:0*/ dma_research_observer__DOT__sel_known;
        CData/*0:0*/ dma_research_observer__DOT__lu_known;
        CData/*4:0*/ dma_research_observer__DOT__changed_count;
        CData/*3:0*/ dma_research_observer__DOT__changed_index;
        CData/*0:0*/ dma_research_observer__DOT__slot_oh_valid;
        CData/*3:0*/ dma_research_observer__DOT__slot_idx;
        CData/*0:0*/ dma_research_observer__DOT__tgt_present0;
        CData/*0:0*/ dma_research_observer__DOT__tgt_present1;
        CData/*0:0*/ dma_research_observer__DOT__cap_found;
        CData/*0:0*/ dma_research_observer__DOT__cap_bad_geom;
        SData/*15:0*/ dma_research_observer__DOT__event_count_q;
        SData/*15:0*/ dma_research_observer__DOT__e2_err_q;
        SData/*15:0*/ dma_research_observer__DOT__cost_err_q;
        SData/*15:0*/ dma_research_observer__DOT__sample_count_q;
        SData/*15:0*/ dma_research_observer__DOT__event_count_n;
        SData/*15:0*/ dma_research_observer__DOT__e2_err_n;
        SData/*15:0*/ dma_research_observer__DOT__cost_err_n;
        SData/*15:0*/ dma_research_observer__DOT__sample_count_n;
        SData/*15:0*/ dma_research_observer__DOT__changed_mask;
        SData/*15:0*/ dma_research_observer__DOT__tgt_live0;
        SData/*15:0*/ dma_research_observer__DOT__tgt_live1;
        IData/*31:0*/ dma_research_observer__DOT__inval_count_q;
        IData/*31:0*/ dma_research_observer__DOT__update_count_q;
        IData/*31:0*/ dma_research_observer__DOT__quiet_timeout_q;
        IData/*31:0*/ dma_research_observer__DOT__e2a_issue_cnt_q;
        IData/*31:0*/ dma_research_observer__DOT__e2a_done_cnt_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_main_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_req_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_miss_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_comp_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_dur_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_flags_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_v_main_q;
        IData/*31:0*/ dma_research_observer__DOT__cur_v_comp_q;
        IData/*31:0*/ dma_research_observer__DOT__inval_count_n;
        IData/*31:0*/ dma_research_observer__DOT__update_count_n;
        IData/*31:0*/ dma_research_observer__DOT__quiet_timeout_n;
        IData/*31:0*/ dma_research_observer__DOT__e2a_issue_cnt_n;
        IData/*31:0*/ dma_research_observer__DOT__e2a_done_cnt_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_main_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_req_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_miss_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_comp_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_dur_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_a_flags_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_v_main_n;
        IData/*31:0*/ dma_research_observer__DOT__cur_v_comp_n;
        WData/*127:0*/ dma_research_observer__DOT__cap_key[4];
        QData/*63:0*/ dma_research_observer__DOT__epoch_q;
        QData/*63:0*/ dma_research_observer__DOT__a_elapsed_q;
        QData/*63:0*/ dma_research_observer__DOT__astart_q;
        QData/*63:0*/ dma_research_observer__DOT__deadline_q;
        QData/*63:0*/ dma_research_observer__DOT__probestart_q;
        QData/*63:0*/ dma_research_observer__DOT__freeze_ts_q;
        QData/*63:0*/ dma_research_observer__DOT__e2a_first_q;
        QData/*63:0*/ dma_research_observer__DOT__e2a_last_q;
        QData/*63:0*/ dma_research_observer__DOT__src_iova_q;
        QData/*63:0*/ dma_research_observer__DOT__dst_iova_q;
        QData/*63:0*/ dma_research_observer__DOT__epoch_n;
        QData/*63:0*/ dma_research_observer__DOT__a_elapsed_n;
        QData/*63:0*/ dma_research_observer__DOT__astart_n;
        QData/*63:0*/ dma_research_observer__DOT__deadline_n;
        QData/*63:0*/ dma_research_observer__DOT__probestart_n;
    };
    struct {
        QData/*63:0*/ dma_research_observer__DOT__freeze_ts_n;
        QData/*63:0*/ dma_research_observer__DOT__e2a_first_n;
        QData/*63:0*/ dma_research_observer__DOT__e2a_last_n;
        QData/*63:0*/ dma_research_observer__DOT__src_iova_n;
        QData/*63:0*/ dma_research_observer__DOT__dst_iova_n;
        QData/*63:0*/ dma_research_observer__DOT__evt_meta;
        QData/*63:0*/ dma_research_observer__DOT__status_q;
        WData/*127:0*/ dma_research_observer__DOT__target_key_q[2][4];
        WData/*127:0*/ dma_research_observer__DOT__target_ctx_q[2][4];
        QData/*63:0*/ dma_research_observer__DOT__fp_time_q[2];
        CData/*0:0*/ dma_research_observer__DOT__fp_hit_q[2];
        SData/*15:0*/ dma_research_observer__DOT__fp_live_self_q[2];
        SData/*15:0*/ dma_research_observer__DOT__fp_live_oth_q[2];
        WData/*127:0*/ dma_research_observer__DOT__fp_key_q[2][4];
        IData/*31:0*/ dma_research_observer__DOT__plru_snap_q[3];
        QData/*63:0*/ dma_research_observer__DOT__snap_warm_q[32];
        QData/*63:0*/ dma_research_observer__DOT__snap_deadline_q[32];
        QData/*63:0*/ dma_research_observer__DOT__snap_freeze_q[32];
        QData/*63:0*/ dma_research_observer__DOT__event_ram_q[1536];
        QData/*63:0*/ dma_research_observer__DOT__lane_id_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_main_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_comp_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_sel_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_hit_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_miss_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_issue_q[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_done_q[2];
        CData/*7:0*/ dma_research_observer__DOT__win_main_q[2];
        CData/*7:0*/ dma_research_observer__DOT__win_comp_q[2];
        CData/*7:0*/ dma_research_observer__DOT__win_sel_q[2];
        CData/*7:0*/ dma_research_observer__DOT__win_hit_q[2];
        CData/*7:0*/ dma_research_observer__DOT__win_miss_q[2];
        QData/*63:0*/ dma_research_observer__DOT__sample_ram_q[1024];
        WData/*127:0*/ dma_research_observer__DOT__target_key_n[2][4];
        WData/*127:0*/ dma_research_observer__DOT__target_ctx_n[2][4];
        QData/*63:0*/ dma_research_observer__DOT__fp_time_n[2];
        CData/*0:0*/ dma_research_observer__DOT__fp_hit_n[2];
        SData/*15:0*/ dma_research_observer__DOT__fp_live_self_n[2];
        SData/*15:0*/ dma_research_observer__DOT__fp_live_oth_n[2];
        WData/*127:0*/ dma_research_observer__DOT__fp_key_n[2][4];
        IData/*31:0*/ dma_research_observer__DOT__plru_snap_n[3];
        QData/*63:0*/ dma_research_observer__DOT__lane_id_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_main_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_comp_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_sel_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_hit_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_miss_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_issue_n[2];
        CData/*7:0*/ dma_research_observer__DOT__cost_done_n[2];
        CData/*7:0*/ dma_research_observer__DOT__win_main_n[2];
        CData/*7:0*/ dma_research_observer__DOT__win_comp_n[2];
        CData/*7:0*/ dma_research_observer__DOT__win_sel_n[2];
        CData/*7:0*/ dma_research_observer__DOT__win_hit_n[2];
        CData/*7:0*/ dma_research_observer__DOT__win_miss_n[2];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__0__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__8__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__9__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__10__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__id_match__11__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__12__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__13__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__14__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__15__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__16__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__17__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__20__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__21__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__24__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__25__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__page_match__28__Vfuncout;
        CData/*0:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__29__Vfuncout;
        CData/*0:0*/ __Vclklast__TOP__clk_i;
        CData/*0:0*/ __Vclklast__TOP__rst_ni;
        IData/*31:0*/ dma_research_observer__DOT____Vlvbound1;
        IData/*31:0*/ dma_research_observer__DOT____Vlvbound2;
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__0__a[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__0__b[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__1__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__1__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ordinary4k__2__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__3__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__3__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ordinary4k__4__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__5__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__5__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ordinary4k__6__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__8__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__8__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__9__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__9__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__10__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__10__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__11__e[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__id_match__11__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__17__a[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__17__b[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__21__a[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__21__b[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__25__a[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__25__b[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[4];
    };
    struct {
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__29__a[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_match__29__b[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[4];
        WData/*127:0*/ __Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[4];
        QData/*63:0*/ dma_research_observer__DOT____Vlvbound3;
        QData/*63:0*/ dma_research_observer__DOT____Vlvbound4;
        QData/*63:0*/ dma_research_observer__DOT____Vlvbound5;
        QData/*63:0*/ dma_research_observer__DOT____Vlvbound6;
        QData/*63:0*/ dma_research_observer__DOT____Vlvbound7;
        QData/*63:0*/ dma_research_observer__DOT____Vlvbound8;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__12__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__12__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__13__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__13__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__14__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__14__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__15__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__15__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__16__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__16__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__20__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__20__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__24__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__24__b;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__28__a;
        QData/*63:0*/ __Vfunc_dma_research_observer__DOT__page_match__28__b;
    };
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_research_observer__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_research_observer);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_research_observer(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_research_observer();
    
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
    static void _eval_initial_loop(Vdma_research_observer__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_research_observer__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_research_observer__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_research_observer__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(Vdma_research_observer__Syms* __restrict vlSymsp);
    static void _combo__TOP__4(Vdma_research_observer__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_research_observer__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_research_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_research_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__3(Vdma_research_observer__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vdma_research_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
