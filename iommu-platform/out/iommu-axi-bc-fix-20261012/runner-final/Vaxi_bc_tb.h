// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VAXI_BC_TB_H_
#define _VAXI_BC_TB_H_  // guard

#include "verilated.h"

//==========

class Vaxi_bc_tb__Syms;

//----------

VL_MODULE(Vaxi_bc_tb) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(check_i,0,0);
    VL_IN8(burst_type_i,1,0);
    VL_IN8(burst_length_i,7,0);
    VL_IN8(n_bytes_i,2,0);
    VL_OUT8(allow_request_o,0,0);
    VL_IN64(addr_i,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    IData/*16:0*/ axi_bc_tb__DOT__i_axi_bc__DOT__burst_bytes;
    IData/*16:0*/ axi_bc_tb__DOT__i_axi_bc__DOT__aligned_offset;
    IData/*16:0*/ axi_bc_tb__DOT__i_axi_bc__DOT__wrap_boundary;
    IData/*16:0*/ axi_bc_tb__DOT__i_axi_bc__DOT__wrap_faddr;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vaxi_bc_tb__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vaxi_bc_tb);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vaxi_bc_tb(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vaxi_bc_tb();
    
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
    static void _eval_initial_loop(Vaxi_bc_tb__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vaxi_bc_tb__Syms* symsp, bool first);
  private:
    static QData _change_request(Vaxi_bc_tb__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vaxi_bc_tb__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(Vaxi_bc_tb__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vaxi_bc_tb__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vaxi_bc_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vaxi_bc_tb__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
