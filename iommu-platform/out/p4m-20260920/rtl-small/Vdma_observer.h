// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDMA_OBSERVER_H_
#define _VDMA_OBSERVER_H_  // guard

#include "verilated.h"

//==========

class Vdma_observer__Syms;

//----------

VL_MODULE(Vdma_observer) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(cfg_valid_i,0,0);
    VL_IN8(cfg_write_i,0,0);
    VL_IN8(cfg_addr_i,6,0);
    VL_IN8(cfg_wstrb_i,7,0);
    VL_IN8(issue_i,3,0);
    VL_IN8(done_i,3,0);
    VL_IN8(poll_i,3,0);
    VL_IN8(batch_active_i,3,0);
    VL_IN8(ar_valid_i,3,0);
    VL_IN8(aw_valid_i,3,0);
    VL_IN8(ar_ready_i,3,0);
    VL_IN8(aw_ready_i,3,0);
    VL_IN8(rlast_fire_i,3,0);
    VL_IN8(b_fire_i,3,0);
    VL_IN8(bad_id_i,3,0);
    VL_IN8(shared_ar_valid_i,0,0);
    VL_IN8(shared_aw_valid_i,0,0);
    VL_IN8(shared_ar_ready_i,0,0);
    VL_IN8(shared_aw_ready_i,0,0);
    VL_IN8(out_valid_i,1,0);
    VL_IN8(out_ready_i,1,0);
    VL_IN8(return_i,1,0);
    VL_IN8(e3_error_i,0,0);
    VL_OUT8(e3_control_o,7,0);
    VL_OUT8(e3_index_o,7,0);
    VL_OUT8(research_write_o,0,0);
    VL_OUT8(research_quiet_o,0,0);
    VL_OUT8(research_arm_o,0,0);
    VL_OUT8(research_open_o,0,0);
    VL_OUT8(research_close_o,0,0);
    VL_OUT16(research_index_o,12,0);
    VL_IN64(cfg_wdata_i,63,0);
    VL_OUT64(cfg_rdata_o,63,0);
    VL_IN64(e3_data_i,63,0);
    VL_IN64(research_data_i,63,0);
    VL_OUT64(research_wdata_o,63,0);
    VL_IN64(issue_id_i[4],63,0);
    VL_IN64(done_id_i[4],63,0);
    VL_IN64(poll_id_i[4],63,0);
    VL_IN8(tr_id_i[2],3,0);
    VL_IN8(out_id_i[2],3,0);
    VL_IN8(return_id_i[2],3,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*2:0*/ dma_observer__DOT__phase_q;
        CData/*2:0*/ dma_observer__DOT__phase_n;
        CData/*7:0*/ dma_observer__DOT__segment_q;
        CData/*7:0*/ dma_observer__DOT__segment_n;
        CData/*3:0*/ dma_observer__DOT__track_bad_q;
        CData/*3:0*/ dma_observer__DOT__track_bad_n;
        CData/*1:0*/ dma_observer__DOT__victim_q;
        CData/*1:0*/ dma_observer__DOT__victim_n;
        CData/*1:0*/ dma_observer__DOT__aggressor_q;
        CData/*1:0*/ dma_observer__DOT__aggressor_n;
        CData/*0:0*/ dma_observer__DOT__has_a_q;
        CData/*0:0*/ dma_observer__DOT__has_a_n;
        CData/*0:0*/ dma_observer__DOT__batch_q;
        CData/*0:0*/ dma_observer__DOT__batch_n;
        CData/*0:0*/ dma_observer__DOT__finite_q;
        CData/*0:0*/ dma_observer__DOT__finite_n;
        CData/*0:0*/ dma_observer__DOT__p4m_q;
        CData/*0:0*/ dma_observer__DOT__p4m_n;
        CData/*3:0*/ dma_observer__DOT__lane_task_q;
        CData/*3:0*/ dma_observer__DOT__lane_task_n;
        CData/*0:0*/ dma_observer__DOT__p4m_open_q;
        CData/*0:0*/ dma_observer__DOT__p4m_open_n;
        CData/*0:0*/ dma_observer__DOT__p4m_closed_q;
        CData/*0:0*/ dma_observer__DOT__p4m_closed_n;
        CData/*0:0*/ dma_observer__DOT__p4m_carry_q;
        CData/*0:0*/ dma_observer__DOT__p4m_carry_n;
        CData/*0:0*/ dma_observer__DOT__p4m_carry_done_q;
        CData/*0:0*/ dma_observer__DOT__p4m_carry_done_n;
        CData/*0:0*/ dma_observer__DOT__p4m_start_q;
        CData/*0:0*/ dma_observer__DOT__p4m_start_n;
        CData/*0:0*/ dma_observer__DOT__p4m_end_q;
        CData/*0:0*/ dma_observer__DOT__p4m_end_n;
        CData/*0:0*/ dma_observer__DOT__p4m_open_issue_q;
        CData/*0:0*/ dma_observer__DOT__p4m_open_issue_n;
        CData/*0:0*/ dma_observer__DOT__a_issue_evt;
        CData/*0:0*/ dma_observer__DOT__a_done_evt;
        CData/*0:0*/ dma_observer__DOT__v_issue_evt;
        CData/*0:0*/ dma_observer__DOT__v_done_evt;
        CData/*7:0*/ dma_observer__DOT__p4m_cycles_q;
        CData/*7:0*/ dma_observer__DOT__p4m_cycles_n;
        CData/*7:0*/ dma_observer__DOT__p4m_a_issue_q;
        CData/*7:0*/ dma_observer__DOT__p4m_a_issue_n;
        CData/*7:0*/ dma_observer__DOT__p4m_a_done_q;
        CData/*7:0*/ dma_observer__DOT__p4m_a_done_n;
        CData/*7:0*/ dma_observer__DOT__p4m_window_issue_q;
        CData/*7:0*/ dma_observer__DOT__p4m_window_issue_n;
        CData/*7:0*/ dma_observer__DOT__p4m_window_done_q;
        CData/*7:0*/ dma_observer__DOT__p4m_window_done_n;
        CData/*0:0*/ dma_observer__DOT__busy;
        CData/*0:0*/ dma_observer__DOT__active;
        CData/*0:0*/ dma_observer__DOT__path_clear;
        CData/*0:0*/ dma_observer__DOT__e3_q;
        CData/*0:0*/ dma_observer__DOT__e3_n;
        CData/*0:0*/ dma_observer__DOT__delay_q;
        CData/*0:0*/ dma_observer__DOT__delay_n;
        CData/*0:0*/ dma_observer__DOT__e3_arm;
        SData/*12:0*/ dma_observer__DOT__index_q;
        SData/*12:0*/ dma_observer__DOT__index_n;
        IData/*31:0*/ dma_observer__DOT__error_q;
        IData/*31:0*/ dma_observer__DOT__error_n;
        IData/*31:0*/ dma_observer__DOT__p4m_error_q;
        IData/*31:0*/ dma_observer__DOT__p4m_error_n;
        QData/*63:0*/ dma_observer__DOT__target_q;
        QData/*63:0*/ dma_observer__DOT__target_n;
    };
    struct {
        QData/*63:0*/ dma_observer__DOT__expected_q;
        QData/*63:0*/ dma_observer__DOT__expected_n;
        QData/*63:0*/ dma_observer__DOT__p4m_open_id_q;
        QData/*63:0*/ dma_observer__DOT__p4m_open_id_n;
        QData/*63:0*/ dma_observer__DOT__p4m_close_id_q;
        QData/*63:0*/ dma_observer__DOT__p4m_close_id_n;
        QData/*63:0*/ dma_observer__DOT__p4m_first_done_id_q;
        QData/*63:0*/ dma_observer__DOT__p4m_first_done_id_n;
        QData/*63:0*/ dma_observer__DOT__p4m_last_done_id_q;
        QData/*63:0*/ dma_observer__DOT__p4m_last_done_id_n;
        QData/*63:0*/ dma_observer__DOT__p4m_data;
        CData/*7:0*/ dma_observer__DOT__count_q[35];
        CData/*7:0*/ dma_observer__DOT__count_n[35];
        CData/*2:0*/ dma_observer__DOT__reads_q[4];
        CData/*2:0*/ dma_observer__DOT__reads_n[4];
        CData/*2:0*/ dma_observer__DOT__writes_q[4];
        CData/*2:0*/ dma_observer__DOT__writes_n[4];
        CData/*2:0*/ dma_observer__DOT__lane_err_q[4];
        CData/*2:0*/ dma_observer__DOT__lane_err_n[4];
        QData/*63:0*/ dma_observer__DOT__lane_id_q[4];
        QData/*63:0*/ dma_observer__DOT__lane_id_n[4];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*7:0*/ dma_observer__DOT____Vlvbound1;
    CData/*7:0*/ dma_observer__DOT____Vlvbound2;
    CData/*7:0*/ dma_observer__DOT____Vlvbound4;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__3__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__4__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__5__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__6__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__7__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__8__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__9__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__10__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__11__enable;
    CData/*0:0*/ __Vtask_dma_observer__DOT__bump__12__enable;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__0__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__1__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__2__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__3__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__4__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__5__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__6__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__7__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__8__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__9__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__10__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__11__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__12__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__13__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__14__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__15__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__16__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__17__slot;
    IData/*31:0*/ __Vtask_dma_observer__DOT__bump__18__slot;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdma_observer__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdma_observer);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdma_observer(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdma_observer();
    
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
    static void _eval_initial_loop(Vdma_observer__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdma_observer__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdma_observer__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vdma_observer__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__2(Vdma_observer__Syms* __restrict vlSymsp);
    static void _combo__TOP__5(Vdma_observer__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdma_observer__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdma_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdma_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__1(Vdma_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__4(Vdma_observer__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vdma_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
