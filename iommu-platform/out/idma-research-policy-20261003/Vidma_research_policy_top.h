// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VIDMA_RESEARCH_POLICY_TOP_H_
#define _VIDMA_RESEARCH_POLICY_TOP_H_  // guard

#include "verilated.h"

//==========

class Vidma_research_policy_top__Syms;

//----------

VL_MODULE(Vidma_research_policy_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(valid_i,0,0);
    VL_IN8(write_i,0,0);
    VL_IN8(addr_i,7,0);
    VL_IN8(wstrb_i,7,0);
    VL_IN8(ready_i,0,0);
    VL_IN8(complete_i,0,0);
    VL_IN8(backend_idle_i,0,0);
    VL_IN8(batch_reverse_i,0,0);
    VL_OUT8(ready_o,0,0);
    VL_OUT8(error_o,0,0);
    VL_OUT8(request_o,0,0);
    VL_OUT8(type_o,1,0);
    VL_OUT8(batch_active_o,0,0);
    VL_OUT8(issue_o,0,0);
    VL_OUT8(done_o,0,0);
    VL_IN8(p_cfg_valid_i,0,0);
    VL_IN8(p_cfg_write_i,0,0);
    VL_IN8(p_quiet_i,0,0);
    VL_IN8(p_supported_i,0,0);
    VL_IN8(p_v_active_i,0,0);
    VL_IN8(p_v_done_i,0,0);
    VL_IN8(p_ctl_state_i,2,0);
    VL_IN8(p_select_fire_i,0,0);
    VL_IN8(p_lookup_i,0,0);
    VL_IN8(p_lookup_hit_i,0,0);
    VL_IN8(p_ptw_r_fire_i,0,0);
    VL_IN8(p_response_error_i,0,0);
    VL_OUT8(policy_enable_o,0,0);
    VL_OUT8(policy_allow_o,0,0);
    VL_OUT8(policy_abort_o,0,0);
    VL_IN16(p_index_i,12,0);
    VL_IN(p_ctl_did_i,23,0);
    VL_IN(p_select_did_i,23,0);
    VL_IN(p_lookup_did_i,23,0);
    VL_IN64(wdata_i,63,0);
    VL_OUT64(rdata_o,63,0);
    VL_OUT64(length_o,63,0);
    VL_OUT64(src_o,63,0);
    VL_OUT64(dst_o,63,0);
    VL_OUT64(issue_id_o,63,0);
    VL_OUT64(done_id_o,63,0);
    VL_IN64(p_wdata_i,63,0);
    VL_OUT64(p_rdata_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ idma_research_policy_top__DOT__request;
        CData/*2:0*/ idma_research_policy_top__DOT__u_frontend__DOT__state_q;
        CData/*2:0*/ idma_research_policy_top__DOT__u_frontend__DOT__state_n;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__issue;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__trans_complete;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__id_retire;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_read;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_access;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__locked_write;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_stopped_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_error_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_reverse_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_task_issue;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_task_done;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__launch_next_id_read;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__launch_locked_write;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_n;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__armed_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__frozen_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__aborted_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__win_allact_q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__ev_sel_a;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__ev_lut_a;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__ev_lut_hit_a;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__ev_ptw_a;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__ev_actual_a;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__win_last;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__act_all_now;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__win_complete;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__stale_cyc;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__refill_en;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__bal_ovf;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__fb_inc;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__fb_dec;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__gap_load;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__wr_any;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__wr_ok;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__obs_err_now;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__sup_drop_now;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__do_arm;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__do_abort;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__do_freeze;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__do_clear;
    };
    struct {
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__cfg_wr;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__cfg_bad;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__enable_core;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__allow_core;
        CData/*0:0*/ idma_research_policy_top__DOT__u_policy__DOT__abort_eff;
        SData/*12:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q;
        SData/*9:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit;
        IData/*31:0*/ idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_n;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__mode_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__window_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__target_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__deadband_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__initbud_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__minbud_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__maxbud_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__step_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__cap_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__whit_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__wmiss_q;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__wptw_q;
        WData/*65:0*/ idma_research_policy_top__DOT__u_policy__DOT__bal_c[3];
        WData/*65:0*/ idma_research_policy_top__DOT__u_policy__DOT__bal_r[3];
        WData/*65:0*/ idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[3];
        WData/*64:0*/ idma_research_policy_top__DOT__u_policy__DOT__b_up_s[3];
        WData/*64:0*/ idma_research_policy_top__DOT__u_policy__DOT__b_dn_s[3];
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i;
        IData/*31:0*/ idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i;
        WData/*81:0*/ idma_research_policy_top__DOT__req[3];
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_n;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n;
        WData/*523:0*/ idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_n;
        WData/*81:0*/ idma_research_policy_top__DOT__u_frontend__DOT__base_req[3];
        QData/*32:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q;
        QData/*32:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_issued_q;
        QData/*32:0*/ idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__launch_id_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__launch_id_n;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__err_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__balance_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__b_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__bmin_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__bmax_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__prev_main_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__win_main_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__win_cnt_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__win_vdone_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__nodone_run_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__nodone_max_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__m6_cd_q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__vdone_now;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__charge_now;
    };
    struct {
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__refill_now;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__main_inc;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__bal_next;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__b_next;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__m6_cd_next;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__new_err;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__nodone_max_next;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__stat_q[26];
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__hist_q[32];
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__stat_inc[26];
        WData/*64:0*/ idma_research_policy_top__DOT__u_policy__DOT__stat_sum[26][3];
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT__hist_inc[32];
        WData/*64:0*/ idma_research_policy_top__DOT__u_policy__DOT__hist_sum[32][3];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
        CData/*0:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
        CData/*1:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
        CData/*3:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
        CData/*0:0*/ __Vclklast__TOP__clk_i;
        CData/*0:0*/ __Vclklast__TOP__rst_ni;
        WData/*64:0*/ idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[3];
        WData/*64:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[3];
        WData/*64:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[3];
        WData/*64:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[3];
        WData/*64:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[3];
        WData/*287:0*/ idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[9];
        WData/*65:0*/ idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[3];
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
        QData/*63:0*/ idma_research_policy_top__DOT__u_policy__DOT____Vlvbound3;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__c;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__inc;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__c;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__inc;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__4__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__4__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__4__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__5__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__5__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__5__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__6__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__6__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__6__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__7__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__7__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__7__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__8__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__8__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__8__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__9__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__9__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__9__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__10__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__10__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__10__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__11__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__11__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__11__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__12__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__12__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__12__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__13__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__13__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__13__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__14__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__14__v;
    };
    struct {
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__14__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__15__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__15__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__15__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__16__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__16__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__16__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__17__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__17__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__17__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__18__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__18__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__18__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__19__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__19__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__19__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__20__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__20__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__20__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__21__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__21__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__21__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__22__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__22__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__22__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__23__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__23__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__23__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__24__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__24__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__24__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__25__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__25__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__25__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__26__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__26__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__26__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__27__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__27__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__27__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__28__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__28__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__28__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__29__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__29__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__29__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__30__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__30__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__30__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__31__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__31__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__31__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__32__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__32__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__32__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__33__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__33__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__33__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__34__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__34__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__34__r;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__35__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__35__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__35__r;
    };
    struct {
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__36__Vfuncout;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__36__v;
        QData/*63:0*/ __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__36__r;
    };
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vidma_research_policy_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vidma_research_policy_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vidma_research_policy_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vidma_research_policy_top();
    
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
    static void _eval_initial_loop(Vidma_research_policy_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vidma_research_policy_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vidma_research_policy_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vidma_research_policy_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__4(Vidma_research_policy_top__Syms* __restrict vlSymsp);
    static void _combo__TOP__5(Vidma_research_policy_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vidma_research_policy_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vidma_research_policy_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vidma_research_policy_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__3(Vidma_research_policy_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vidma_research_policy_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__2(Vidma_research_policy_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
