// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_policy.h for the primary calling header

#include "Vdma_research_policy.h"
#include "Vdma_research_policy__Syms.h"

void Vdma_research_policy::_eval_initial(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval_initial\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_research_policy::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::final\n"); );
    // Variables
    Vdma_research_policy__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_research_policy::_eval_settle(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval_settle\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
}

void Vdma_research_policy::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_ctor_var_reset\n"); );
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
    v_active_i = VL_RAND_RESET_I(1);
    a_issue_i = VL_RAND_RESET_I(1);
    a_done_i = VL_RAND_RESET_I(1);
    v_done_i = VL_RAND_RESET_I(1);
    ctl_state_i = VL_RAND_RESET_I(3);
    ctl_did_i = VL_RAND_RESET_I(24);
    select_fire_i = VL_RAND_RESET_I(1);
    select_did_i = VL_RAND_RESET_I(24);
    lookup_i = VL_RAND_RESET_I(1);
    lookup_hit_i = VL_RAND_RESET_I(1);
    lookup_did_i = VL_RAND_RESET_I(24);
    ptw_r_fire_i = VL_RAND_RESET_I(1);
    response_error_i = VL_RAND_RESET_I(1);
    policy_enable_o = VL_RAND_RESET_I(1);
    policy_allow_o = VL_RAND_RESET_I(1);
    policy_abort_o = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__armed_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__frozen_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__aborted_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__err_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__mode_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__window_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__target_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__deadband_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__initbud_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__minbud_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__maxbud_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__step_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__cap_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__whit_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__wmiss_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__wptw_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__balance_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__b_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__bmin_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__bmax_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__prev_main_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_main_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_cnt_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_vdone_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_allact_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__nodone_run_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__nodone_max_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__m6_cd_q = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            dma_research_policy__DOT__stat_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            dma_research_policy__DOT__hist_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            dma_research_policy__DOT__stat_inc[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            VL_RAND_RESET_W(65, dma_research_policy__DOT__stat_sum[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            dma_research_policy__DOT__hist_inc[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            VL_RAND_RESET_W(65, dma_research_policy__DOT__hist_sum[__Vi0]);
    }}
    dma_research_policy__DOT__ev_sel_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_lut_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_lut_hit_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_lut_miss_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_ptw_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_actual_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__win_run = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__win_last = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__act_all_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__win_complete = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__vdone_now = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__charge_now = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__refill_now = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__main_inc = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__stale_cyc = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__refill_en = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(66, dma_research_policy__DOT__bal_c);
    VL_RAND_RESET_W(66, dma_research_policy__DOT__bal_r);
    VL_RAND_RESET_W(66, dma_research_policy__DOT__bal_r_clamped);
    dma_research_policy__DOT__bal_ovf = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__bal_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__fb_inc = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__fb_dec = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__b_next = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, dma_research_policy__DOT__b_up_s);
    VL_RAND_RESET_W(65, dma_research_policy__DOT__b_dn_s);
    dma_research_policy__DOT__m6_cd_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__gap_load = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cnt_ovf = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__wr_any = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__wr_ok = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__obs_err_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__sup_drop_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__new_err = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__do_arm = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_abort = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_freeze = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_clear = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cfg_wr = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cfg_bad = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__allow_core = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__abort_eff = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__nodone_run_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__nodone_max_next = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, dma_research_policy__DOT____Vlvbound2);
    dma_research_policy__DOT____Vlvbound3 = VL_RAND_RESET_I(8);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout);
    __Vfunc_dma_research_policy__DOT__cadd__0__c = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__cadd__0__inc = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__0__w);
    __Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__1__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__1__r = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout);
    __Vfunc_dma_research_policy__DOT__cadd__2__c = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__cadd__2__inc = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__2__w);
    __Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__3__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__3__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__4__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__4__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__4__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__5__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__5__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__5__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__6__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__6__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__6__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__7__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__7__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__7__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__8__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__8__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__8__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__9__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__9__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__9__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__10__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__10__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__10__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__11__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__11__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__11__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__12__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__12__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__12__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__13__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__13__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__13__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__14__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__14__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__14__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__15__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__15__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__15__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__16__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__16__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__16__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__17__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__17__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__17__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__18__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__18__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__18__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__19__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__19__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__19__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__20__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__20__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__20__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__21__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__21__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__21__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__22__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__22__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__22__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__23__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__23__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__23__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__24__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__24__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__24__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__25__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__25__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__25__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__26__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__26__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__26__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__27__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__27__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__27__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__28__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__28__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__28__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__29__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__29__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__29__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__30__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__30__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__30__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__31__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__31__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__31__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__32__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__32__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__32__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__33__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__33__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__33__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__34__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__34__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__34__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__35__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__35__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__35__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__36__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__36__v = VL_RAND_RESET_I(8);
    __Vfunc_dma_research_policy__DOT__zext64__36__r = VL_RAND_RESET_Q(64);
}
