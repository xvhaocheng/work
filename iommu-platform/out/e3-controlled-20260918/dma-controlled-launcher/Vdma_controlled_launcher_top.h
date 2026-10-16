// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_CONTROLLED_LAUNCHER_TOP_H_
#define _VDMA_CONTROLLED_LAUNCHER_TOP_H_  // guard

#include "verilated.h"

//==========

class Vdma_controlled_launcher_top__Syms;

//----------

VL_MODULE(Vdma_controlled_launcher_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(cfg_valid_i,0,0);
    VL_IN8(cfg_write_i,0,0);
    VL_IN8(cfg_addr_i,7,0);
    VL_IN8(cfg_wstrb_i,7,0);
    VL_IN8(frontend_held_i,3,0);
    VL_IN8(frontend_active_i,3,0);
    VL_IN8(frontend_cancelled_i,3,0);
    VL_IN8(frontend_error_i,3,0);
    VL_IN8(obs_issue_i,3,0);
    VL_IN8(obs_done_i,3,0);
    VL_IN8(selected_i,0,0);
    VL_IN8(select_write_i,0,0);
    VL_IN8(select_id_i,7,0);
    VL_OUT8(launch_hold_o,3,0);
    VL_OUT8(launch_go_o,3,0);
    VL_OUT8(launch_cancel_o,3,0);
    VL_INW(obs_issue_id_i_flat,255,0,8);
    VL_INW(obs_done_id_i_flat,255,0,8);
    VL_IN(select_did_i,23,0);
    VL_IN64(cfg_wdata_i,63,0);
    VL_OUT64(cfg_rdata_o,63,0);
    VL_IN64(select_iova_i,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*1:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__state_q;
        CData/*1:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__state_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_n;
        CData/*7:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__index_q;
        CData/*7:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__index_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n;
        CData/*7:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_q;
        CData/*7:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_n;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_q;
        CData/*3:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_n;
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff;
        IData/*31:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__error_q;
        IData/*31:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__error_n;
        IData/*23:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_q;
        IData/*23:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_n;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_n;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_q;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_n;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_q;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_n;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_q;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_q;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_n;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__status_word;
        QData/*63:0*/ dma_controlled_launcher_top__DOT__obs_issue_id_i[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__obs_done_id_i[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[4];
    };
    struct {
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[4];
        QData/*63:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[4];
        CData/*0:0*/ dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[4];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    QData/*63:0*/ dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i[4];
    QData/*63:0*/ dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i[4];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_controlled_launcher_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_controlled_launcher_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_controlled_launcher_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_controlled_launcher_top();
    
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
    static void _eval_initial_loop(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_controlled_launcher_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__2(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
