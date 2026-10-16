// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VIDMA_BATCH_PAIR_TOP_H_
#define _VIDMA_BATCH_PAIR_TOP_H_  // guard

#include "verilated.h"

//==========

class Vidma_batch_pair_top__Syms;

//----------

VL_MODULE(Vidma_batch_pair_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(valid_i,0,0);
    VL_IN8(write_i,0,0);
    VL_IN8(port_i,0,0);
    VL_IN8(addr_i,7,0);
    VL_OUT8(ready_o,0,0);
    VL_OUT8(error_o,0,0);
    VL_OUT8(running_o,1,0);
    VL_IN64(wdata_i,63,0);
    VL_OUT64(rdata_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*1:0*/ idma_batch_pair_top__DOT__issue;
        CData/*1:0*/ idma_batch_pair_top__DOT__done;
        CData/*1:0*/ idma_batch_pair_top__DOT__poll;
        CData/*1:0*/ idma_batch_pair_top__DOT__cfg_valid;
        CData/*1:0*/ idma_batch_pair_top__DOT__cfg_write;
        CData/*1:0*/ idma_batch_pair_top__DOT__request;
        CData/*1:0*/ idma_batch_pair_top__DOT__complete;
        CData/*4:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining;
        CData/*4:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining;
        CData/*2:0*/ idma_batch_pair_top__DOT__obs__DOT__phase_q;
        CData/*2:0*/ idma_batch_pair_top__DOT__obs__DOT__phase_n;
        CData/*1:0*/ idma_batch_pair_top__DOT__obs__DOT__track_bad_q;
        CData/*1:0*/ idma_batch_pair_top__DOT__obs__DOT__track_bad_n;
        CData/*5:0*/ idma_batch_pair_top__DOT__obs__DOT__index_q;
        CData/*5:0*/ idma_batch_pair_top__DOT__obs__DOT__index_n;
        CData/*1:0*/ idma_batch_pair_top__DOT__obs__DOT__victim_q;
        CData/*1:0*/ idma_batch_pair_top__DOT__obs__DOT__victim_n;
        CData/*1:0*/ idma_batch_pair_top__DOT__obs__DOT__aggressor_q;
        CData/*1:0*/ idma_batch_pair_top__DOT__obs__DOT__aggressor_n;
        CData/*0:0*/ idma_batch_pair_top__DOT__obs__DOT__has_a_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__obs__DOT__has_a_n;
        CData/*0:0*/ idma_batch_pair_top__DOT__obs__DOT__batch_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__obs__DOT__batch_n;
        CData/*0:0*/ idma_batch_pair_top__DOT__obs__DOT__busy;
        CData/*0:0*/ idma_batch_pair_top__DOT__obs__DOT__active;
        CData/*1:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q;
        CData/*1:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__trans_complete;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_read;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__locked_write;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_issue;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_done;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we;
        CData/*1:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q;
        CData/*1:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__trans_complete;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_read;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__locked_write;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q;
    };
    struct {
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_issue;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_done;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we;
        SData/*12:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q;
        SData/*9:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit;
        SData/*12:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q;
        SData/*9:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit;
        IData/*31:0*/ idma_batch_pair_top__DOT__obs__DOT__error_q;
        IData/*31:0*/ idma_batch_pair_top__DOT__obs__DOT__error_n;
        QData/*63:0*/ idma_batch_pair_top__DOT__cfg_rdata;
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__segment_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__segment_n;
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__target_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__target_n;
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__expected_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__expected_n;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_n;
        WData/*523:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_n;
        WData/*81:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[3];
        QData/*32:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q;
        QData/*32:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q;
        QData/*32:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_id_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_n;
        WData/*523:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_n;
        WData/*81:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[3];
        QData/*32:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q;
        QData/*32:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q;
        QData/*32:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_id_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d;
    };
    struct {
        QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q;
        WData/*81:0*/ idma_batch_pair_top__DOT__req[2][3];
        WData/*65:0*/ idma_batch_pair_top__DOT__rsp[2][3];
        QData/*63:0*/ idma_batch_pair_top__DOT__issue_id[2];
        QData/*63:0*/ idma_batch_pair_top__DOT__done_id[2];
        QData/*63:0*/ idma_batch_pair_top__DOT__poll_id[2];
        QData/*63:0*/ idma_batch_pair_top__DOT__cfg_wdata[2];
        CData/*6:0*/ idma_batch_pair_top__DOT__cfg_addr[2];
        CData/*7:0*/ idma_batch_pair_top__DOT__cfg_wstrb[2];
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__count_q[35];
        QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT__count_n[35];
        CData/*7:0*/ idma_batch_pair_top__DOT__obs__DOT__reads_q[2];
        CData/*7:0*/ idma_batch_pair_top__DOT__obs__DOT__reads_n[2];
        CData/*7:0*/ idma_batch_pair_top__DOT__obs__DOT__writes_q[2];
        CData/*7:0*/ idma_batch_pair_top__DOT__obs__DOT__writes_n[2];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o;
    CData/*0:0*/ idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__valid_o;
    CData/*0:0*/ idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o;
    CData/*0:0*/ idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__valid_o;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
    CData/*1:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
    CData/*3:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
    CData/*0:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
    CData/*1:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
    CData/*3:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
    CData/*0:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__enable;
    CData/*0:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__enable;
    CData/*0:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__enable;
    CData/*0:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__enable;
    CData/*0:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__enable;
    CData/*0:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__enable;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__0__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__1__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__13__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__14__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__15__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__16__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__17__slot;
    IData/*31:0*/ __Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__18__slot;
    WData/*65:0*/ idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[3];
    WData/*65:0*/ idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[3];
    QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
    QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    QData/*63:0*/ idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
    QData/*63:0*/ idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
    QData/*63:0*/ idma_batch_pair_top__DOT____Vcellinp__obs__poll_id_i[2];
    QData/*63:0*/ idma_batch_pair_top__DOT____Vcellinp__obs__done_id_i[2];
    QData/*63:0*/ idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i[2];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vidma_batch_pair_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vidma_batch_pair_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vidma_batch_pair_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vidma_batch_pair_top();
    
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
    static void _eval_initial_loop(Vidma_batch_pair_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vidma_batch_pair_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vidma_batch_pair_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vidma_batch_pair_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vidma_batch_pair_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vidma_batch_pair_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vidma_batch_pair_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vidma_batch_pair_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(Vidma_batch_pair_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vidma_batch_pair_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
