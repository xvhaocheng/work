// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VPATH_STAGE_TEST_TOP_H_
#define _VPATH_STAGE_TEST_TOP_H_  // guard

#include "verilated.h"

//==========

class Vpath_stage_test_top__Syms;

//----------

VL_MODULE(Vpath_stage_test_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(clear_i,0,0);
    VL_IN8(run_i,0,0);
    VL_IN8(active_i,0,0);
    VL_IN8(enter_i,0,0);
    VL_IN8(leave_i,0,0);
    VL_IN8(time_i,7,0);
    VL_OUT8(error_o,0,0);
    VL_OUT64(data_o[8],63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*1:0*/ path_stage_test_top__DOT__i_stage__DOT__head_q;
    CData/*1:0*/ path_stage_test_top__DOT__i_stage__DOT__tail_q;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__count_q;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__count_next;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__entered_q;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__full_q;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__carry_q;
    CData/*7:0*/ path_stage_test_top__DOT__i_stage__DOT__max_q;
    CData/*7:0*/ path_stage_test_top__DOT__i_stage__DOT__age;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__opening_q;
    CData/*2:0*/ path_stage_test_top__DOT__i_stage__DOT__closing_q;
    CData/*0:0*/ path_stage_test_top__DOT__i_stage__DOT__matched;
    CData/*0:0*/ path_stage_test_top__DOT__i_stage__DOT__whole;
    CData/*0:0*/ path_stage_test_top__DOT__i_stage__DOT__bad;
    SData/*8:0*/ path_stage_test_top__DOT__i_stage__DOT__sum_q;
    SData/*8:0*/ path_stage_test_top__DOT__i_stage__DOT__area_q;
    SData/*9:0*/ path_stage_test_top__DOT__i_stage__DOT__sum_next;
    SData/*9:0*/ path_stage_test_top__DOT__i_stage__DOT__area_next;
    CData/*7:0*/ path_stage_test_top__DOT__i_stage__DOT__stamps[4];
    CData/*0:0*/ path_stage_test_top__DOT__i_stage__DOT__born_in_window[4];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    QData/*63:0*/ path_stage_test_top__DOT____Vcellout__i_stage__data_o[8];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vpath_stage_test_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vpath_stage_test_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vpath_stage_test_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vpath_stage_test_top();
    
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
    static void _eval_initial_loop(Vpath_stage_test_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vpath_stage_test_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vpath_stage_test_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vpath_stage_test_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vpath_stage_test_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vpath_stage_test_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vpath_stage_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vpath_stage_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vpath_stage_test_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vpath_stage_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
