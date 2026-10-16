// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VE3_DELAY_TOP_H_
#define _VE3_DELAY_TOP_H_  // guard

#include "verilated.h"

//==========

class Ve3_delay_top__Syms;

//----------

VL_MODULE(Ve3_delay_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(enable_i,0,0);
    VL_IN8(arm_i,0,0);
    VL_IN8(eligible_i,0,0);
    VL_IN8(delay128_i,0,0);
    VL_IN8(in_valid_i,0,0);
    VL_IN8(out_ready_i,0,0);
    VL_IN8(in_last_i,0,0);
    VL_IN8(in_id_i,3,0);
    VL_OUT8(in_ready_o,0,0);
    VL_OUT8(out_valid_o,0,0);
    VL_OUT8(out_last_o,0,0);
    VL_OUT8(out_id_o,3,0);
    VL_OUT8(capture_o,0,0);
    VL_OUT8(available_o,0,0);
    VL_OUT8(consume_o,0,0);
    VL_OUT8(error_o,0,0);
    VL_OUT8(extra_o,7,0);
    VL_IN64(in_data_i,63,0);
    VL_OUT64(out_data_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*0:0*/ e3_delay_top__DOT__dut__DOT__full_q;
    CData/*0:0*/ e3_delay_top__DOT__dut__DOT__selected_q;
    CData/*0:0*/ e3_delay_top__DOT__dut__DOT__used_q;
    CData/*0:0*/ e3_delay_top__DOT__dut__DOT__announced_q;
    CData/*7:0*/ e3_delay_top__DOT__dut__DOT__remaining_q;
    CData/*7:0*/ e3_delay_top__DOT__dut__DOT__held_q;
    CData/*0:0*/ e3_delay_top__DOT__dut__DOT__take;
    CData/*0:0*/ e3_delay_top__DOT__dut__DOT__give;
    WData/*69:0*/ e3_delay_top__DOT__mem_resp[3];
    WData/*69:0*/ e3_delay_top__DOT__dut__DOT__saved_q[3];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ e3_delay_top__DOT____Vcellout__dut__memory_req_o;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    WData/*69:0*/ e3_delay_top__DOT____Vcellout__dut__walker_resp_o[3];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Ve3_delay_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Ve3_delay_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Ve3_delay_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Ve3_delay_top();
    
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
    static void _eval_initial_loop(Ve3_delay_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Ve3_delay_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Ve3_delay_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Ve3_delay_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Ve3_delay_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Ve3_delay_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Ve3_delay_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Ve3_delay_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Ve3_delay_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Ve3_delay_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
