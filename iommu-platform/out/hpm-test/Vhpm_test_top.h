// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VHPM_TEST_TOP_H_
#define _VHPM_TEST_TOP_H_  // guard

#include "verilated.h"

//==========

class Vhpm_test_top__Syms;

//----------

VL_MODULE(Vhpm_test_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(valid,0,0);
    VL_IN8(inhibit,0,0);
    VL_IN8(filter_did,0,0);
    VL_IN(did,23,0);
    VL_OUT64(misses,63,0);
    VL_OUT64(cycles,62,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__increment_ctr;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__did_match;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__pid_match;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__gscid_match;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__pscid_match;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__edged_event_q;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT__edged_event_n;
    IData/*31:0*/ hpm_test_top__DOT__inh;
    IData/*31:0*/ hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k;
    IData/*31:0*/ hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k;
    IData/*31:0*/ hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k;
    IData/*31:0*/ hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k;
    WData/*92:0*/ hpm_test_top__DOT__event_data[3];
    QData/*63:0*/ hpm_test_top__DOT__cyc_in;
    QData/*63:0*/ hpm_test_top__DOT__evt_in;
    WData/*65:0*/ hpm_test_top__DOT__cyc_out[3];
    WData/*92:0*/ hpm_test_top__DOT__dut__DOT__event_q[3];
    WData/*92:0*/ hpm_test_top__DOT__dut__DOT__event_n[3];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound2;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound3;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound4;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound5;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound6;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound7;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound8;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound9;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound10;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound11;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound12;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound13;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound15;
    CData/*0:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound16;
    CData/*0:0*/ __Vfunc_hpm_event_match__0__Vfuncout;
    CData/*0:0*/ __Vclklast__TOP__clk;
    CData/*0:0*/ __Vclklast__TOP__rst_n;
    SData/*14:0*/ __Vfunc_hpm_event_match__0__ctr_event;
    SData/*8:0*/ __Vfunc_hpm_event_match__0__mask;
    SData/*8:0*/ __Vfunc_hpm_event_match__0__masked_event;
    WData/*64:0*/ hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[3];
    QData/*63:0*/ hpm_test_top__DOT__dut__DOT____Vlvbound20;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vhpm_test_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vhpm_test_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vhpm_test_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vhpm_test_top();
    
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
    static void _eval_initial_loop(Vhpm_test_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vhpm_test_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vhpm_test_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vhpm_test_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(Vhpm_test_top__Syms* __restrict vlSymsp);
    static void _combo__TOP__5(Vhpm_test_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vhpm_test_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vhpm_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vhpm_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__3(Vhpm_test_top__Syms* __restrict vlSymsp);
    static void _sequent__TOP__4(Vhpm_test_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vhpm_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
