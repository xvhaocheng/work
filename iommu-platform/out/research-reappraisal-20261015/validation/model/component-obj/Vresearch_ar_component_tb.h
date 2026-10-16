// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VRESEARCH_AR_COMPONENT_TB_H_
#define _VRESEARCH_AR_COMPONENT_TB_H_  // guard

#include "verilated_heavy.h"

//==========

class Vresearch_ar_component_tb__Syms;

//----------

VL_MODULE(Vresearch_ar_component_tb) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(req,3,0);
    VL_IN8(ready,0,0);
    VL_IN8(tamper_i,0,0);
    VL_OUT8(valid,0,0);
    VL_OUT8(grant,3,0);
    VL_OUT8(index_o,1,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__rr_req_i;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__rr_gnt_o;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__ds_req_i;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__ds_gnt_o;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__rr_gnt_i;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__ds_gnt_i;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__ds_req_o;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__ds_idx_o;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__nd_rr_q;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__held_idx_q;
    CData/*2:0*/ research_ar_component_tb__DOT__dut__DOT__ds_streak_q;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__held_q;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__nd_pending;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__fresh_valid;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__ds_fire;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__fresh_idx;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_q;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_saw_ds_q;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__mon_ds_present_at_offer_q;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q;
    CData/*5:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes;
    CData/*2:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes;
    CData/*2:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d;
    CData/*1:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel;
    CData/*0:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel;
    CData/*7:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
    CData/*7:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp;
    CData/*7:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes;
    CData/*7:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
    CData/*3:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp;
    IData/*31:0*/ research_ar_component_tb__DOT__dut__DOT__mode;
    IData/*31:0*/ research_ar_component_tb__DOT__dut__DOT__mon_mode_q;
    IData/*31:0*/ research_ar_component_tb__DOT__dut__DOT__mon_streak_q;
    IData/*31:0*/ research_ar_component_tb__DOT__dut__DOT__mon_nd_cursor_q;
    WData/*327:0*/ research_ar_component_tb__DOT__data_i[11];
    WData/*81:0*/ research_ar_component_tb__DOT__dut__DOT__ds_data_o[3];
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_cycle_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_sequence_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_sequence_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_first_dual_head_cycle_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_ds_seen_count_q;
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_late_ds_count_q;
    WData/*81:0*/ research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[3];
    WData/*245:0*/ research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[8];
    QData/*63:0*/ research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[4];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*1:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__Vfuncout;
    CData/*2:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active;
    CData/*1:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__cursor;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    CData/*5:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes;
    CData/*2:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes;
    CData/*2:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes;
    CData/*3:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
    CData/*7:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
    CData/*3:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes;
    CData/*7:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
    WData/*81:0*/ research_ar_component_tb__DOT____Vcellout__dut__data_o[3];
    WData/*245:0*/ __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[8];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vresearch_ar_component_tb__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vresearch_ar_component_tb);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vresearch_ar_component_tb(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vresearch_ar_component_tb();
    
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
    static void _eval_initial_loop(Vresearch_ar_component_tb__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vresearch_ar_component_tb__Syms* symsp, bool first);
  private:
    static QData _change_request(Vresearch_ar_component_tb__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vresearch_ar_component_tb__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__4(Vresearch_ar_component_tb__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vresearch_ar_component_tb__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _final_TOP(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__3(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(Vresearch_ar_component_tb__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__5(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
