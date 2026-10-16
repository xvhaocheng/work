// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_RESEARCH_COST_GROUPS_H_
#define _VDMA_RESEARCH_COST_GROUPS_H_  // guard

#include "verilated.h"

//==========

class Vdma_research_cost_groups__Syms;

//----------

VL_MODULE(Vdma_research_cost_groups) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(clear_i,0,0);
    VL_IN8(commit_i,0,0);
    VL_OUT8(total_o,7,0);
    VL_OUT16(err_o,15,0);
    VL_IN(main_i,31,0);
    VL_IN(comp_i,31,0);
    VL_IN(dur_i,31,0);
    VL_IN(req_i,31,0);
    VL_IN(miss_i,31,0);
    VL_OUT8(count_o[6],7,0);
    VL_OUT8(sum_main_o[6],7,0);
    VL_OUT8(sum_comp_o[6],7,0);
    VL_OUT8(sum_dur_o[6],7,0);
    VL_OUT8(sum_req_o[6],7,0);
    VL_OUT8(sum_miss_o[6],7,0);
    VL_OUT(min_main_o[6],31,0);
    VL_OUT(max_main_o[6],31,0);
    VL_OUT(min_comp_o[6],31,0);
    VL_OUT(max_comp_o[6],31,0);
    VL_OUT(min_dur_o[6],31,0);
    VL_OUT(max_dur_o[6],31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*5:0*/ dma_research_cost_groups__DOT__seen_q;
    CData/*7:0*/ dma_research_cost_groups__DOT__total_q;
    CData/*5:0*/ dma_research_cost_groups__DOT__seen_n;
    CData/*7:0*/ dma_research_cost_groups__DOT__total_n;
    CData/*2:0*/ dma_research_cost_groups__DOT__grp;
    SData/*15:0*/ dma_research_cost_groups__DOT__err_q;
    SData/*15:0*/ dma_research_cost_groups__DOT__err_n;
    WData/*64:0*/ dma_research_cost_groups__DOT__ext[3];
    QData/*32:0*/ dma_research_cost_groups__DOT__main_plus_comp;
    CData/*7:0*/ dma_research_cost_groups__DOT__count_q[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_main_q[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_comp_q[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_dur_q[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_req_q[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_miss_q[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__min_main_q[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__max_main_q[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__min_comp_q[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__max_comp_q[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__min_dur_q[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__max_dur_q[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__count_n[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_main_n[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_comp_n[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_dur_n[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_req_n[6];
    CData/*7:0*/ dma_research_cost_groups__DOT__sum_miss_n[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__min_main_n[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__max_main_n[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__min_comp_n[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__max_comp_n[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__min_dur_n[6];
    IData/*31:0*/ dma_research_cost_groups__DOT__max_dur_n[6];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound1;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound2;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound3;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound4;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound5;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound6;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound25;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound26;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound27;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound28;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound29;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound30;
    CData/*0:0*/ dma_research_cost_groups__DOT____Vlvbound43;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound44;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound45;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound46;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound47;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound48;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound49;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound68;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound69;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound70;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound71;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound72;
    CData/*7:0*/ dma_research_cost_groups__DOT____Vlvbound73;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound7;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound8;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound9;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound10;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound11;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound12;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound31;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound32;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound33;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound34;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound35;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound36;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound37;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound38;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound39;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound40;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound41;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound42;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound50;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound51;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound52;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound53;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound54;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound55;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound74;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound75;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound76;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound77;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound78;
    IData/*31:0*/ dma_research_cost_groups__DOT____Vlvbound79;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_research_cost_groups__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_research_cost_groups);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_research_cost_groups(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_research_cost_groups();
    
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
    static void _eval_initial_loop(Vdma_research_cost_groups__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_research_cost_groups__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_research_cost_groups__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_research_cost_groups__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vdma_research_cost_groups__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_research_cost_groups__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_research_cost_groups__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_research_cost_groups__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vdma_research_cost_groups__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vdma_research_cost_groups__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
