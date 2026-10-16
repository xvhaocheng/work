// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_observer.h for the primary calling header

#include "Vdma_research_observer.h"
#include "Vdma_research_observer__Syms.h"

void Vdma_research_observer::_eval_initial(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_eval_initial\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_research_observer::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::final\n"); );
    // Variables
    Vdma_research_observer__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_research_observer::_eval_settle(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_eval_settle\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vdma_research_observer::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    cfg_valid_i = VL_RAND_RESET_I(1);
    cfg_write_i = VL_RAND_RESET_I(1);
    index_i = VL_RAND_RESET_I(13);
    wdata_i = VL_RAND_RESET_Q(64);
    rdata_o = VL_RAND_RESET_Q(64);
    quiet_i = VL_RAND_RESET_I(1);
    supported_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, entries_i[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, entries_next_i[__Vi0]);
    }}
    plru_i = VL_RAND_RESET_I(32);
    update_i = VL_RAND_RESET_I(1);
    invalidate_i = VL_RAND_RESET_I(1);
    replace_mask_i = VL_RAND_RESET_I(16);
    update_did_i = VL_RAND_RESET_I(24);
    lookup_i = VL_RAND_RESET_I(1);
    lookup_hit_i = VL_RAND_RESET_I(1);
    lookup_did_i = VL_RAND_RESET_I(24);
    lookup_iova_i = VL_RAND_RESET_Q(64);
    lookup_pscid_i = VL_RAND_RESET_I(20);
    lookup_gscid_i = VL_RAND_RESET_I(16);
    lookup_stage1_i = VL_RAND_RESET_I(1);
    lookup_stage2_i = VL_RAND_RESET_I(1);
    ctl_state_i = VL_RAND_RESET_I(3);
    ctl_did_i = VL_RAND_RESET_I(24);
    select_fire_i = VL_RAND_RESET_I(1);
    select_did_i = VL_RAND_RESET_I(24);
    response_error_i = VL_RAND_RESET_I(1);
    issue_i = VL_RAND_RESET_I(2);
    done_i = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            issue_id_i[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            done_id_i[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    p4m_arm_i = VL_RAND_RESET_I(1);
    p4m_open_i = VL_RAND_RESET_I(1);
    p4m_close_i = VL_RAND_RESET_I(1);
    reverse_order_o = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__mode_q = VL_RAND_RESET_I(2);
    dma_research_observer__DOT__cmd_q = VL_RAND_RESET_I(4);
    dma_research_observer__DOT__e2_phase_q = VL_RAND_RESET_I(3);
    dma_research_observer__DOT__reverse_order_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__epoch_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__a_elapsed_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__astart_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__deadline_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__probestart_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__freeze_ts_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__event_count_q = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__inval_count_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__update_count_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__quiet_timeout_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__e2_ovf_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__e2_err_q = VL_RAND_RESET_I(16);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_research_observer__DOT__target_key_q[__Vi0]);
    }}
    dma_research_observer__DOT__target_valid_q = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_research_observer__DOT__target_ctx_q[__Vi0]);
    }}
    dma_research_observer__DOT__target_ctx_valid_q = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_time_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_hit_q[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_live_self_q[__Vi0] = VL_RAND_RESET_I(16);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_live_oth_q[__Vi0] = VL_RAND_RESET_I(16);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_research_observer__DOT__fp_key_q[__Vi0]);
    }}
    dma_research_observer__DOT__fp_done_q = VL_RAND_RESET_I(2);
    dma_research_observer__DOT__snapshot_valid_q = VL_RAND_RESET_I(3);
    { int __Vi0=0; for (; __Vi0<3; ++__Vi0) {
            dma_research_observer__DOT__plru_snap_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    VL_RAND_RESET_W(2048, dma_research_observer__DOT__snap_warm_q);
    VL_RAND_RESET_W(2048, dma_research_observer__DOT__snap_deadline_q);
    VL_RAND_RESET_W(2048, dma_research_observer__DOT__snap_freeze_q);
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            VL_RAND_RESET_W(384, dma_research_observer__DOT__event_ram_q[__Vi0]);
    }}
    dma_research_observer__DOT__e2a_issue_cnt_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__e2a_done_cnt_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__e2a_first_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__e2a_last_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__e2a_first_seen_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__src_iova_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__dst_iova_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__src_set_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__dst_set_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__lane_active_q = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__lane_id_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT__lane_in_window_q = VL_RAND_RESET_I(2);
    dma_research_observer__DOT__cost_active_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cost_err_q = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__cost_cycles_q = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_main_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_comp_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_sel_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_hit_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_miss_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_issue_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_done_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT__cost_owner_err_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__cost_a_done_all_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__window_open_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__window_closed_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__win_cycles_q = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_main_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_comp_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_sel_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_hit_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_miss_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT__carry_in_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__carry_out_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__eligible_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__sample_count_q = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__sample_ovf_q = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cur_a_main_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_req_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_miss_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_comp_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_dur_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_flags_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_v_main_q = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_v_comp_q = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            VL_RAND_RESET_W(256, dma_research_observer__DOT__sample_ram_q[__Vi0]);
    }}
    dma_research_observer__DOT__mode_n = VL_RAND_RESET_I(2);
    dma_research_observer__DOT__cmd_n = VL_RAND_RESET_I(4);
    dma_research_observer__DOT__e2_phase_n = VL_RAND_RESET_I(3);
    dma_research_observer__DOT__epoch_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__a_elapsed_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__astart_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__deadline_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__probestart_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__freeze_ts_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__event_count_n = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__inval_count_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__update_count_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__quiet_timeout_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__e2_ovf_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__e2_err_n = VL_RAND_RESET_I(16);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_research_observer__DOT__target_key_n[__Vi0]);
    }}
    dma_research_observer__DOT__target_valid_n = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_research_observer__DOT__target_ctx_n[__Vi0]);
    }}
    dma_research_observer__DOT__target_ctx_valid_n = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_time_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_hit_n[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_live_self_n[__Vi0] = VL_RAND_RESET_I(16);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__fp_live_oth_n[__Vi0] = VL_RAND_RESET_I(16);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_research_observer__DOT__fp_key_n[__Vi0]);
    }}
    dma_research_observer__DOT__fp_done_n = VL_RAND_RESET_I(2);
    dma_research_observer__DOT__snapshot_valid_n = VL_RAND_RESET_I(3);
    { int __Vi0=0; for (; __Vi0<3; ++__Vi0) {
            dma_research_observer__DOT__plru_snap_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    dma_research_observer__DOT__e2a_issue_cnt_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__e2a_done_cnt_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__e2a_first_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__e2a_last_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__e2a_first_seen_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__src_iova_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__dst_iova_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__src_set_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__dst_set_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__lane_active_n = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__lane_id_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT__lane_in_window_n = VL_RAND_RESET_I(2);
    dma_research_observer__DOT__cost_active_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cost_err_n = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__cost_cycles_n = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_main_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_comp_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_sel_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_hit_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_miss_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_issue_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__cost_done_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT__cost_owner_err_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__cost_a_done_all_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__window_open_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__window_closed_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__win_cycles_n = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_main_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_comp_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_sel_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_hit_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_research_observer__DOT__win_miss_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT__carry_in_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__carry_out_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__eligible_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__sample_count_n = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__sample_ovf_n = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cur_a_main_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_req_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_miss_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_comp_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_dur_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_a_flags_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_v_main_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__cur_v_comp_n = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__ctx_ok_both = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__e2_active_phase = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cmd_wr = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__acc_warm = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__acc_a = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__acc_probe = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__acc_freeze = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__acc_clear = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__abort_cmd = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__lanes_en = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__a_issue_ok = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__a_done_ok = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__lane_proto_err = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__win_act = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__main_cyc = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__win_act_issue = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_count_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_sum_main_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_sum_comp_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_sum_dur_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_sum_req_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_sum_miss_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_min_main_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_max_main_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_min_comp_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_max_comp_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_min_dur_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__agg_max_dur_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    dma_research_observer__DOT__agg_clear = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_research_observer__DOT____Vcellinp__u_dma_research_cost_groups__commit_i = VL_RAND_RESET_I(1);
    dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__lookup_v_i = VL_RAND_RESET_I(1);
    dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__main_known = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__sel_known = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__lu_known = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__changed_mask = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__changed_count = VL_RAND_RESET_I(5);
    dma_research_observer__DOT__changed_index = VL_RAND_RESET_I(4);
    dma_research_observer__DOT__slot_oh_valid = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__slot_idx = VL_RAND_RESET_I(4);
    dma_research_observer__DOT__tgt_live0 = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__tgt_live1 = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__tgt_present0 = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__tgt_present1 = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cap_found = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__cap_bad_geom = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, dma_research_observer__DOT__cap_key);
    dma_research_observer__DOT__evt_meta = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__status_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT____Vlvbound1 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT____Vlvbound2 = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_q = VL_RAND_RESET_I(6);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_q = VL_RAND_RESET_I(16);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_n = VL_RAND_RESET_I(6);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp = VL_RAND_RESET_I(3);
    VL_RAND_RESET_W(65, dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT__main_plus_comp = VL_RAND_RESET_Q(33);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound25 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound26 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound27 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound28 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound29 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound30 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound31 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound32 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound33 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound34 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound35 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound36 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound37 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound38 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound39 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound40 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound41 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound42 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound43 = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 = VL_RAND_RESET_I(32);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q = VL_RAND_RESET_I(3);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n = VL_RAND_RESET_I(3);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_q = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n = VL_RAND_RESET_I(16);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_n = VL_RAND_RESET_Q(64);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_edge = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open = VL_RAND_RESET_I(1);
    dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__ctx_match__0__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__0__a);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__0__b);
    __Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__1__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__1__k);
    __Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ordinary4k__2__e);
    __Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__3__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__3__k);
    __Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ordinary4k__4__e);
    __Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__5__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__5__k);
    __Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ordinary4k__6__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__7__k);
    __Vfunc_dma_research_observer__DOT__id_match__8__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__8__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__8__k);
    __Vfunc_dma_research_observer__DOT__id_match__9__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__9__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__9__k);
    __Vfunc_dma_research_observer__DOT__id_match__10__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__10__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__10__k);
    __Vfunc_dma_research_observer__DOT__id_match__11__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__11__e);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__id_match__11__k);
    __Vfunc_dma_research_observer__DOT__page_match__12__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__12__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__12__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__13__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__13__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__13__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__14__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__14__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__14__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__15__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__15__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__15__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__16__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__16__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__16__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__ctx_match__17__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__17__a);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__17__b);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__18__k);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__19__k);
    __Vfunc_dma_research_observer__DOT__page_match__20__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__20__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__20__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__ctx_match__21__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__21__a);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__21__b);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__22__k);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__23__k);
    __Vfunc_dma_research_observer__DOT__page_match__24__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__24__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__24__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__ctx_match__25__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__25__a);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__25__b);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__26__k);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__27__k);
    __Vfunc_dma_research_observer__DOT__page_match__28__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_dma_research_observer__DOT__page_match__28__a = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__page_match__28__b = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_observer__DOT__ctx_match__29__Vfuncout = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__29__a);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_match__29__b);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__30__k);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout);
    VL_RAND_RESET_W(128, __Vfunc_dma_research_observer__DOT__ctx_key_128__31__k);
}
