// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VIDMA_FRONTEND_OBSERVER_TOP_H_
#define _VIDMA_FRONTEND_OBSERVER_TOP_H_  // guard

#include "verilated.h"

//==========

class Vidma_frontend_observer_top__Syms;

//----------

VL_MODULE(Vidma_frontend_observer_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(valid_i,0,0);
    VL_IN8(write_i,0,0);
    VL_IN8(addr_i,7,0);
    VL_IN8(ready_i,0,0);
    VL_IN8(complete_i,0,0);
    VL_OUT8(ready_o,0,0);
    VL_OUT8(error_o,0,0);
    VL_OUT8(unchanged_o,0,0);
    VL_OUT8(issue_o,0,0);
    VL_OUT8(done_o,0,0);
    VL_OUT8(poll_o,0,0);
    VL_OUT8(cfg_valid_o,0,0);
    VL_OUT8(batch_active_o,0,0);
    VL_OUT8(type_o,1,0);
    VL_OUT8(request_o,0,0);
    VL_IN64(wdata_i,63,0);
    VL_OUT64(rdata_o,63,0);
    VL_OUT64(issue_id_o,63,0);
    VL_OUT64(done_id_o,63,0);
    VL_OUT64(poll_id_o,63,0);
    VL_OUT64(length_o,63,0);
    VL_OUT64(src_o,63,0);
    VL_OUT64(dst_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ idma_frontend_observer_top__DOT__request;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold_request;
        CData/*1:0*/ idma_frontend_observer_top__DOT__dut__DOT__state_q;
        CData/*1:0*/ idma_frontend_observer_top__DOT__dut__DOT__state_n;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__issue;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__trans_complete;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__id_retire;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__active_cycles_read;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__observer_access;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_access;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__locked_write;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_enable_q;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_stop_q;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_stopped_q;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_error_q;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_cfg_ok;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_task_issue;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_task_done;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__launch_ctrl_q;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__launch_ctrl_n;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__n_trans_we;
        CData/*1:0*/ idma_frontend_observer_top__DOT__gold__DOT__state_q;
        CData/*1:0*/ idma_frontend_observer_top__DOT__gold__DOT__state_n;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__issue;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__trans_complete;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__active_cycles_read;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__n_trans_we;
        SData/*12:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_pages_q;
        SData/*9:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit;
        SData/*9:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__addr_hit;
        WData/*81:0*/ idma_frontend_observer_top__DOT__req[3];
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__transf_counter_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__transf_counter_n;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__transf_offset_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__transf_offset_n;
        WData/*523:0*/ idma_frontend_observer_top__DOT__dut__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_frontend_observer_top__DOT__dut__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__active_cycles_q;
    };
    struct {
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__active_cycles_n;
        WData/*81:0*/ idma_frontend_observer_top__DOT__dut__DOT__base_req[3];
        QData/*32:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_target_q;
        QData/*32:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_issued_q;
        QData/*32:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_completed_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__batch_id_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__transf_counter_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__transf_counter_n;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__transf_offset_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__transf_offset_n;
        WData/*523:0*/ idma_frontend_observer_top__DOT__gold__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_frontend_observer_top__DOT__gold__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__active_cycles_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__active_cycles_n;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
    CData/*1:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
    CData/*3:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
    CData/*0:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
    CData/*1:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
    CData/*3:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    WData/*287:0*/ idma_frontend_observer_top__DOT____Vcellout__dut__burst_req_o[9];
    WData/*65:0*/ idma_frontend_observer_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[3];
    WData/*287:0*/ idma_frontend_observer_top__DOT____Vcellout__gold__burst_req_o[9];
    WData/*65:0*/ idma_frontend_observer_top__DOT____Vcellout__gold__dma_ctrl_rsp_o[3];
    QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
    QData/*63:0*/ idma_frontend_observer_top__DOT__gold__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vidma_frontend_observer_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vidma_frontend_observer_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vidma_frontend_observer_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vidma_frontend_observer_top();
    
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
    static void _eval_initial_loop(Vidma_frontend_observer_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vidma_frontend_observer_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vidma_frontend_observer_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vidma_frontend_observer_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vidma_frontend_observer_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vidma_frontend_observer_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vidma_frontend_observer_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vidma_frontend_observer_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(Vidma_frontend_observer_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vidma_frontend_observer_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
