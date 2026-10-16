// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_E3_WRITE_OBSERVER_H_
#define _VDMA_E3_WRITE_OBSERVER_H_  // guard

#include "verilated.h"

//==========

class Vdma_e3_write_observer__Syms;

//----------

VL_MODULE(Vdma_e3_write_observer) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(clear_i,0,0);
    VL_IN8(run_i,0,0);
    VL_IN8(active_i,0,0);
    VL_IN8(open_i,0,0);
    VL_IN8(close_i,0,0);
    VL_IN8(aw_i,0,0);
    VL_IN8(wlast_i,0,0);
    VL_IN8(b_i,0,0);
    VL_IN8(aw_id_i,3,0);
    VL_IN8(b_id_i,3,0);
    VL_OUT8(error_o,0,0);
    VL_IN(time_i,31,0);
    VL_OUT64(data_o[32],63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*2:0*/ dma_e3_write_observer__DOT__aw_tail_q;
    CData/*2:0*/ dma_e3_write_observer__DOT__aw_tail_n;
    CData/*2:0*/ dma_e3_write_observer__DOT__w_tail_q;
    CData/*2:0*/ dma_e3_write_observer__DOT__w_tail_n;
    CData/*0:0*/ dma_e3_write_observer__DOT__bad;
    IData/*31:0*/ dma_e3_write_observer__DOT__role;
    IData/*31:0*/ dma_e3_write_observer__DOT__slot;
    IData/*31:0*/ dma_e3_write_observer__DOT__base;
    QData/*63:0*/ dma_e3_write_observer__DOT__aw_w;
    QData/*63:0*/ dma_e3_write_observer__DOT__w_b;
    QData/*63:0*/ dma_e3_write_observer__DOT__aw_b;
    CData/*2:0*/ dma_e3_write_observer__DOT__present_q[8];
    CData/*2:0*/ dma_e3_write_observer__DOT__present_n[8];
    CData/*2:0*/ dma_e3_write_observer__DOT__inside_q[8];
    CData/*2:0*/ dma_e3_write_observer__DOT__inside_n[8];
    CData/*0:0*/ dma_e3_write_observer__DOT__role_q[8];
    CData/*0:0*/ dma_e3_write_observer__DOT__role_n[8];
    IData/*31:0*/ dma_e3_write_observer__DOT__stamps_q[8][3];
    IData/*31:0*/ dma_e3_write_observer__DOT__stamps_n[8][3];
    CData/*2:0*/ dma_e3_write_observer__DOT__order_q[2][4];
    CData/*2:0*/ dma_e3_write_observer__DOT__order_n[2][4];
    CData/*1:0*/ dma_e3_write_observer__DOT__head_q[2];
    CData/*1:0*/ dma_e3_write_observer__DOT__head_n[2];
    CData/*1:0*/ dma_e3_write_observer__DOT__tail_q[2];
    CData/*1:0*/ dma_e3_write_observer__DOT__tail_n[2];
    CData/*2:0*/ dma_e3_write_observer__DOT__count_q[2];
    CData/*2:0*/ dma_e3_write_observer__DOT__count_n[2];
    QData/*63:0*/ dma_e3_write_observer__DOT__data_q[32];
    QData/*63:0*/ dma_e3_write_observer__DOT__data_n[32];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    IData/*31:0*/ dma_e3_write_observer__DOT____Vlvbound1;
    IData/*31:0*/ dma_e3_write_observer__DOT____Vlvbound2;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_e3_write_observer__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_e3_write_observer);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_e3_write_observer(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_e3_write_observer();
    
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
    static void _eval_initial_loop(Vdma_e3_write_observer__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_e3_write_observer__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_e3_write_observer__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_e3_write_observer__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vdma_e3_write_observer__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_e3_write_observer__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_e3_write_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_e3_write_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vdma_e3_write_observer__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vdma_e3_write_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
