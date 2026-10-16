// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_RESEARCH_E2_TIMING_H_
#define _VDMA_RESEARCH_E2_TIMING_H_  // guard

#include "verilated.h"

//==========

class Vdma_research_e2_timing__Syms;

//----------

VL_MODULE(Vdma_research_e2_timing) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(clear_i,0,0);
    VL_IN8(probe_accept_i,0,0);
    VL_IN8(phase_i,2,0);
    VL_IN8(issue_v_i,0,0);
    VL_IN8(done_v_i,0,0);
    VL_IN8(select_v_i,0,0);
    VL_IN8(ctl_v_i,0,0);
    VL_IN8(ctl_state_i,2,0);
    VL_IN8(lookup_v_i,0,0);
    VL_IN8(lookup_hit_i,0,0);
    VL_IN8(response_error_i,0,0);
    VL_IN8(unsupported_i,0,0);
    VL_OUT16(error_o,15,0);
    VL_IN64(epoch_i,63,0);
    VL_OUT64(flags_o,63,0);
    VL_OUT64(issue_epoch_o,63,0);
    VL_OUT64(first_select_epoch_o,63,0);
    VL_OUT64(done_epoch_o,63,0);
    VL_OUT64(done_issue_cycles_o,63,0);
    VL_OUT64(first_select_issue_cycles_o,63,0);
    VL_OUT64(v_main_cycles_o,63,0);
    VL_OUT64(v_comp_cycles_o,63,0);
    VL_OUT64(v_select_count_o,63,0);
    VL_OUT64(v_hit_count_o,63,0);
    VL_OUT64(v_miss_count_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*2:0*/ dma_research_e2_timing__DOT__state_q;
    CData/*2:0*/ dma_research_e2_timing__DOT__state_n;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_main_q;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_main_n;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_comp_q;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_comp_n;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_sel_q;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_sel_n;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_hit_q;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_hit_n;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_miss_q;
    CData/*7:0*/ dma_research_e2_timing__DOT__v_miss_n;
    CData/*0:0*/ dma_research_e2_timing__DOT__phase_frozen;
    CData/*0:0*/ dma_research_e2_timing__DOT__issue_edge;
    CData/*0:0*/ dma_research_e2_timing__DOT__interval_open;
    SData/*15:0*/ dma_research_e2_timing__DOT__err_q;
    SData/*15:0*/ dma_research_e2_timing__DOT__err_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__flags_q;
    QData/*63:0*/ dma_research_e2_timing__DOT__flags_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__issue_epoch_q;
    QData/*63:0*/ dma_research_e2_timing__DOT__issue_epoch_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__first_select_epoch_q;
    QData/*63:0*/ dma_research_e2_timing__DOT__first_select_epoch_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__done_epoch_q;
    QData/*63:0*/ dma_research_e2_timing__DOT__done_epoch_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__done_issue_q;
    QData/*63:0*/ dma_research_e2_timing__DOT__done_issue_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__first_select_issue_q;
    QData/*63:0*/ dma_research_e2_timing__DOT__first_select_issue_n;
    QData/*63:0*/ dma_research_e2_timing__DOT__issue_epoch_now;
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_research_e2_timing__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_research_e2_timing);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_research_e2_timing(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_research_e2_timing();
    
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
    static void _eval_initial_loop(Vdma_research_e2_timing__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_research_e2_timing__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_research_e2_timing__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_research_e2_timing__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__2(Vdma_research_e2_timing__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_research_e2_timing__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_research_e2_timing__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_research_e2_timing__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vdma_research_e2_timing__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vdma_research_e2_timing__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
