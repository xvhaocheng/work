// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_COMPOSITION_CONTROL_H_
#define _VDMA_COMPOSITION_CONTROL_H_  // guard

#include "verilated.h"

//==========

class Vdma_composition_control__Syms;

//----------

VL_MODULE(Vdma_composition_control) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(cfg_write_i,0,0);
    VL_IN8(index_i,6,0);
    VL_IN8(quiet_i,0,0);
    VL_IN8(supported_i,0,0);
    VL_IN8(arm_i,0,0);
    VL_IN8(open_i,0,0);
    VL_IN8(close_i,0,0);
    VL_OUT8(mode_o,1,0);
    VL_OUT8(clear_o,0,0);
    VL_IN8(lookup_i,0,0);
    VL_IN8(lookup_hit_i,0,0);
    VL_IN8(normal_i,0,0);
    VL_IN8(stage1_i,0,0);
    VL_IN8(stage2_i,0,0);
    VL_IN8(accept_i,0,0);
    VL_IN8(fast_i,0,0);
    VL_IN8(gap_active_i,1,0);
    VL_IN8(update_i,0,0);
    VL_IN8(invalidate_i,0,0);
    VL_IN16(replace_mask_i,15,0);
    VL_OUT(protected_did_o,23,0);
    VL_IN(lookup_did_i,23,0);
    VL_IN(lookup_pscid_i,19,0);
    VL_IN(fast_did_i,23,0);
    VL_IN(update_did_i,23,0);
    VL_IN(plru_i,31,0);
    VL_IN64(wdata_i,63,0);
    VL_OUT64(rdata_o,63,0);
    VL_INW(entries_i[16],127,0,4);
    VL_INW(entries_next_i[16],127,0,4);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*1:0*/ dma_composition_control__DOT__state_q;
    CData/*1:0*/ dma_composition_control__DOT__state_d;
    CData/*4:0*/ dma_composition_control__DOT__err_q;
    CData/*4:0*/ dma_composition_control__DOT__err_d;
    CData/*0:0*/ dma_composition_control__DOT__seen_lookup_v_q;
    CData/*0:0*/ dma_composition_control__DOT__seen_lookup_v_d;
    CData/*0:0*/ dma_composition_control__DOT__seen_lookup_a_q;
    CData/*0:0*/ dma_composition_control__DOT__seen_lookup_a_d;
    CData/*1:0*/ dma_composition_control__DOT__mode_q;
    CData/*2:0*/ dma_composition_control__DOT__last_status_q;
    CData/*0:0*/ dma_composition_control__DOT__cfg_busy;
    CData/*0:0*/ dma_composition_control__DOT__cfg_data_legal;
    CData/*3:0*/ dma_composition_control__DOT__repl_idx;
    CData/*0:0*/ dma_composition_control__DOT__lookup_ok;
    IData/*31:0*/ dma_composition_control__DOT__plru_open_q;
    IData/*31:0*/ dma_composition_control__DOT__plru_open_d;
    IData/*31:0*/ dma_composition_control__DOT__plru_close_q;
    IData/*31:0*/ dma_composition_control__DOT__plru_close_d;
    IData/*19:0*/ dma_composition_control__DOT__pscid_v_q;
    IData/*19:0*/ dma_composition_control__DOT__pscid_v_d;
    IData/*19:0*/ dma_composition_control__DOT__pscid_a_q;
    IData/*19:0*/ dma_composition_control__DOT__pscid_a_d;
    IData/*23:0*/ dma_composition_control__DOT__protected_did_q;
    QData/*63:0*/ dma_composition_control__DOT__seq_q;
    QData/*63:0*/ dma_composition_control__DOT__seq_d;
    QData/*63:0*/ dma_composition_control__DOT__cycles_q;
    QData/*63:0*/ dma_composition_control__DOT__cycles_d;
    QData/*63:0*/ dma_composition_control__DOT__win_cfg_q;
    QData/*63:0*/ dma_composition_control__DOT__win_cfg_d;
    QData/*63:0*/ dma_composition_control__DOT__win_gen_q;
    QData/*63:0*/ dma_composition_control__DOT__win_gen_d;
    QData/*63:0*/ dma_composition_control__DOT__generation_q;
    QData/*63:0*/ dma_composition_control__DOT__cnt_q[40];
    QData/*63:0*/ dma_composition_control__DOT__cnt_d[40];
    WData/*127:0*/ dma_composition_control__DOT__entries_open_q[16][4];
    WData/*127:0*/ dma_composition_control__DOT__entries_open_d[16][4];
    WData/*127:0*/ dma_composition_control__DOT__entries_close_q[16][4];
    WData/*127:0*/ dma_composition_control__DOT__entries_close_d[16][4];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    QData/*63:0*/ dma_composition_control__DOT____Vlvbound1;
    QData/*63:0*/ dma_composition_control__DOT____Vlvbound3;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_composition_control__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_composition_control);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_composition_control(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_composition_control();
    
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
    static void _eval_initial_loop(Vdma_composition_control__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_composition_control__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_composition_control__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_composition_control__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(Vdma_composition_control__Syms* __restrict vlSymsp);
    static void _combo__TOP__4(Vdma_composition_control__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_composition_control__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_composition_control__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_composition_control__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__3(Vdma_composition_control__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vdma_composition_control__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
