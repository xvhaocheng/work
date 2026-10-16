// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_RESEARCH_POLICY_H_
#define _VDMA_RESEARCH_POLICY_H_  // guard

#include "verilated.h"

//==========

class Vdma_research_policy__Syms;

//----------

VL_MODULE(Vdma_research_policy) {
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
    VL_IN8(v_active_i,0,0);
    VL_IN8(a_issue_i,0,0);
    VL_IN8(a_done_i,0,0);
    VL_IN8(v_done_i,0,0);
    VL_IN8(ctl_state_i,2,0);
    VL_IN8(select_fire_i,0,0);
    VL_IN8(lookup_i,0,0);
    VL_IN8(lookup_hit_i,0,0);
    VL_IN8(ptw_r_fire_i,0,0);
    VL_IN8(response_error_i,0,0);
    VL_OUT8(policy_enable_o,0,0);
    VL_OUT8(policy_allow_o,0,0);
    VL_OUT8(policy_abort_o,0,0);
    VL_IN16(index_i,12,0);
    VL_IN(ctl_did_i,23,0);
    VL_IN(select_did_i,23,0);
    VL_IN(lookup_did_i,23,0);
    VL_IN64(wdata_i,63,0);
    VL_OUT64(rdata_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ dma_research_policy__DOT__armed_q;
        CData/*0:0*/ dma_research_policy__DOT__frozen_q;
        CData/*0:0*/ dma_research_policy__DOT__aborted_q;
        CData/*0:0*/ dma_research_policy__DOT__win_allact_q;
        CData/*0:0*/ dma_research_policy__DOT__ev_sel_a;
        CData/*0:0*/ dma_research_policy__DOT__ev_lut_a;
        CData/*0:0*/ dma_research_policy__DOT__ev_lut_hit_a;
        CData/*0:0*/ dma_research_policy__DOT__ev_lut_miss_a;
        CData/*0:0*/ dma_research_policy__DOT__ev_ptw_a;
        CData/*0:0*/ dma_research_policy__DOT__ev_actual_a;
        CData/*0:0*/ dma_research_policy__DOT__win_run;
        CData/*0:0*/ dma_research_policy__DOT__win_last;
        CData/*0:0*/ dma_research_policy__DOT__act_all_now;
        CData/*0:0*/ dma_research_policy__DOT__win_complete;
        CData/*0:0*/ dma_research_policy__DOT__stale_cyc;
        CData/*0:0*/ dma_research_policy__DOT__refill_en;
        CData/*0:0*/ dma_research_policy__DOT__bal_ovf;
        CData/*0:0*/ dma_research_policy__DOT__fb_inc;
        CData/*0:0*/ dma_research_policy__DOT__fb_dec;
        CData/*0:0*/ dma_research_policy__DOT__gap_load;
        CData/*0:0*/ dma_research_policy__DOT__cnt_ovf;
        CData/*0:0*/ dma_research_policy__DOT__wr_any;
        CData/*0:0*/ dma_research_policy__DOT__wr_ok;
        CData/*0:0*/ dma_research_policy__DOT__obs_err_now;
        CData/*0:0*/ dma_research_policy__DOT__sup_drop_now;
        CData/*0:0*/ dma_research_policy__DOT__do_arm;
        CData/*0:0*/ dma_research_policy__DOT__do_abort;
        CData/*0:0*/ dma_research_policy__DOT__do_freeze;
        CData/*0:0*/ dma_research_policy__DOT__do_clear;
        CData/*0:0*/ dma_research_policy__DOT__cfg_wr;
        CData/*0:0*/ dma_research_policy__DOT__cfg_bad;
        CData/*0:0*/ dma_research_policy__DOT__allow_core;
        CData/*0:0*/ dma_research_policy__DOT__abort_eff;
        IData/*31:0*/ dma_research_policy__DOT__mode_q;
        IData/*31:0*/ dma_research_policy__DOT__window_q;
        IData/*31:0*/ dma_research_policy__DOT__target_q;
        IData/*31:0*/ dma_research_policy__DOT__deadband_q;
        IData/*31:0*/ dma_research_policy__DOT__initbud_q;
        IData/*31:0*/ dma_research_policy__DOT__minbud_q;
        IData/*31:0*/ dma_research_policy__DOT__maxbud_q;
        IData/*31:0*/ dma_research_policy__DOT__step_q;
        IData/*31:0*/ dma_research_policy__DOT__cap_q;
        IData/*31:0*/ dma_research_policy__DOT__whit_q;
        IData/*31:0*/ dma_research_policy__DOT__wmiss_q;
        IData/*31:0*/ dma_research_policy__DOT__wptw_q;
        WData/*65:0*/ dma_research_policy__DOT__bal_c[3];
        WData/*65:0*/ dma_research_policy__DOT__bal_r[3];
        WData/*65:0*/ dma_research_policy__DOT__bal_r_clamped[3];
        WData/*64:0*/ dma_research_policy__DOT__b_up_s[3];
        WData/*64:0*/ dma_research_policy__DOT__b_dn_s[3];
        QData/*63:0*/ dma_research_policy__DOT__err_q;
        QData/*63:0*/ dma_research_policy__DOT__balance_q;
        QData/*63:0*/ dma_research_policy__DOT__b_q;
        QData/*63:0*/ dma_research_policy__DOT__bmin_q;
        QData/*63:0*/ dma_research_policy__DOT__bmax_q;
        QData/*63:0*/ dma_research_policy__DOT__prev_main_q;
        QData/*63:0*/ dma_research_policy__DOT__win_main_q;
        QData/*63:0*/ dma_research_policy__DOT__win_cnt_q;
        QData/*63:0*/ dma_research_policy__DOT__win_vdone_q;
        QData/*63:0*/ dma_research_policy__DOT__nodone_run_q;
        QData/*63:0*/ dma_research_policy__DOT__nodone_max_q;
        QData/*63:0*/ dma_research_policy__DOT__m6_cd_q;
        QData/*63:0*/ dma_research_policy__DOT__vdone_now;
        QData/*63:0*/ dma_research_policy__DOT__charge_now;
    };
    struct {
        QData/*63:0*/ dma_research_policy__DOT__refill_now;
        QData/*63:0*/ dma_research_policy__DOT__main_inc;
        QData/*63:0*/ dma_research_policy__DOT__bal_next;
        QData/*63:0*/ dma_research_policy__DOT__b_next;
        QData/*63:0*/ dma_research_policy__DOT__m6_cd_next;
        QData/*63:0*/ dma_research_policy__DOT__new_err;
        QData/*63:0*/ dma_research_policy__DOT__nodone_run_next;
        QData/*63:0*/ dma_research_policy__DOT__nodone_max_next;
        CData/*7:0*/ dma_research_policy__DOT__stat_q[26];
        CData/*7:0*/ dma_research_policy__DOT__hist_q[32];
        QData/*63:0*/ dma_research_policy__DOT__stat_inc[26];
        WData/*64:0*/ dma_research_policy__DOT__stat_sum[26][3];
        QData/*63:0*/ dma_research_policy__DOT__hist_inc[32];
        WData/*64:0*/ dma_research_policy__DOT__hist_sum[32][3];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*7:0*/ dma_research_policy__DOT____Vlvbound3;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__cadd__0__c;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__1__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__cadd__2__c;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__3__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__4__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__5__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__6__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__7__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__8__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__9__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__10__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__11__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__12__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__13__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__14__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__15__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__16__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__17__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__18__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__19__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__20__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__21__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__22__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__23__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__24__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__25__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__26__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__27__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__28__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__29__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__30__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__31__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__32__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__33__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__34__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__35__v;
        CData/*7:0*/ __Vfunc_dma_research_policy__DOT__zext64__36__v;
        CData/*0:0*/ __Vclklast__TOP__clk_i;
        CData/*0:0*/ __Vclklast__TOP__rst_ni;
        WData/*64:0*/ dma_research_policy__DOT____Vlvbound2[3];
        WData/*64:0*/ __Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[3];
        WData/*64:0*/ __Vfunc_dma_research_policy__DOT__cadd__0__w[3];
        WData/*64:0*/ __Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[3];
        WData/*64:0*/ __Vfunc_dma_research_policy__DOT__cadd__2__w[3];
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__cadd__0__inc;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__1__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__cadd__2__inc;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__3__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__4__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__4__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__5__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__5__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__6__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__6__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__7__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__7__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__8__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__8__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__9__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__9__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__10__Vfuncout;
    };
    struct {
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__10__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__11__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__11__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__12__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__12__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__13__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__13__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__14__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__14__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__15__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__15__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__16__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__16__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__17__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__17__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__18__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__18__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__19__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__19__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__20__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__20__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__21__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__21__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__22__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__22__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__23__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__23__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__24__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__24__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__25__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__25__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__26__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__26__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__27__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__27__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__28__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__28__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__29__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__29__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__30__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__30__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__31__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__31__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__32__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__32__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__33__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__33__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__34__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__34__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__35__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__35__r;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__36__Vfuncout;
        QData/*63:0*/ __Vfunc_dma_research_policy__DOT__zext64__36__r;
    };
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_research_policy__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_research_policy);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_research_policy(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_research_policy();
    
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
    static void _eval_initial_loop(Vdma_research_policy__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_research_policy__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_research_policy__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_research_policy__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__2(Vdma_research_policy__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_research_policy__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_research_policy__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_research_policy__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vdma_research_policy__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vdma_research_policy__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
