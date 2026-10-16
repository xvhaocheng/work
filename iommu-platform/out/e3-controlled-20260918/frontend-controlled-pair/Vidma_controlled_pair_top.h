// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VIDMA_CONTROLLED_PAIR_TOP_H_
#define _VIDMA_CONTROLLED_PAIR_TOP_H_  // guard

#include "verilated.h"

//==========

class Vidma_controlled_pair_top__Syms;

//----------

VL_MODULE(Vidma_controlled_pair_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(v_cpu_valid_i,0,0);
    VL_IN8(v_cpu_write_i,0,0);
    VL_IN8(v_cpu_wstrb_i,7,0);
    VL_OUT8(v_cpu_ready_o,0,0);
    VL_OUT8(v_cpu_error_o,0,0);
    VL_IN8(a_cpu_valid_i,0,0);
    VL_IN8(a_cpu_write_i,0,0);
    VL_IN8(a_cpu_wstrb_i,7,0);
    VL_OUT8(a_cpu_ready_o,0,0);
    VL_OUT8(a_cpu_error_o,0,0);
    VL_IN8(v_backend_ready_i,0,0);
    VL_IN8(v_backend_idle_i,0,0);
    VL_IN8(v_trans_complete_i,0,0);
    VL_OUT8(v_valid_o,0,0);
    VL_OUT8(v_obs_issue_o,0,0);
    VL_OUT8(v_obs_done_o,0,0);
    VL_IN8(a_backend_ready_i,0,0);
    VL_IN8(a_backend_idle_i,0,0);
    VL_IN8(a_trans_complete_i,0,0);
    VL_OUT8(a_valid_o,0,0);
    VL_OUT8(a_obs_issue_o,0,0);
    VL_OUT8(a_obs_done_o,0,0);
    VL_IN8(selected_i,0,0);
    VL_IN8(select_write_i,0,0);
    VL_IN8(select_id_i,7,0);
    VL_OUT8(launch_hold_o,3,0);
    VL_OUT8(launch_go_o,3,0);
    VL_OUT8(launch_cancel_o,3,0);
    VL_OUT8(frontend_held_o,3,0);
    VL_OUT8(frontend_active_o,3,0);
    VL_OUT8(frontend_cancelled_o,3,0);
    VL_OUT8(frontend_error_o,3,0);
    VL_OUT8(launcher_valid_o,0,0);
    VL_OUT8(launcher_write_o,0,0);
    VL_OUT8(launcher_addr_o,7,0);
    VL_IN(v_cpu_addr_i,31,0);
    VL_IN(a_cpu_addr_i,31,0);
    VL_IN(select_did_i,23,0);
    VL_IN64(v_cpu_wdata_i,63,0);
    VL_OUT64(v_cpu_rdata_o,63,0);
    VL_IN64(a_cpu_wdata_i,63,0);
    VL_OUT64(a_cpu_rdata_o,63,0);
    VL_OUT64(v_burst_src_addr_o,63,0);
    VL_OUT64(v_burst_dst_addr_o,63,0);
    VL_OUT64(v_burst_length_o,63,0);
    VL_OUT64(v_issue_id_o,63,0);
    VL_OUT64(v_done_id_o,63,0);
    VL_OUT64(a_burst_src_addr_o,63,0);
    VL_OUT64(a_burst_dst_addr_o,63,0);
    VL_OUT64(a_burst_length_o,63,0);
    VL_OUT64(a_issue_id_o,63,0);
    VL_OUT64(a_done_id_o,63,0);
    VL_IN64(select_iova_i,63,0);
    VL_OUT64(launcher_rdata_o,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ idma_controlled_pair_top__DOT__v_obs_cfg_valid;
        CData/*3:0*/ idma_controlled_pair_top__DOT__fe_held;
        CData/*3:0*/ idma_controlled_pair_top__DOT__fe_active;
        CData/*3:0*/ idma_controlled_pair_top__DOT__fe_cancelled;
        CData/*3:0*/ idma_controlled_pair_top__DOT__fe_error;
        CData/*3:0*/ idma_controlled_pair_top__DOT__launcher_go;
        CData/*3:0*/ idma_controlled_pair_top__DOT__launcher_cancel;
        CData/*2:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q;
        CData/*2:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__trans_complete;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_read;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__observer_access;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_locked_write;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we;
        CData/*2:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q;
        CData/*2:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__trans_complete;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_read;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_locked_write;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_re;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wdata_we;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we;
        CData/*1:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__state_q;
        CData/*1:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__state_n;
    };
    struct {
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_n;
        CData/*7:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__index_q;
        CData/*7:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__index_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n;
        CData/*7:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_q;
        CData/*7:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_n;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_q;
        CData/*3:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_n;
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff;
        SData/*12:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q;
        SData/*9:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit;
        SData/*9:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_n;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_n;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__error_q;
        IData/*31:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__error_n;
        IData/*23:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_q;
        IData/*23:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_n;
        WData/*105:0*/ idma_controlled_pair_top__DOT__v_req[4];
        WData/*105:0*/ idma_controlled_pair_top__DOT__a_req[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__v_obs_cfg_rdata;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n;
        WData/*523:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_n;
        WData/*105:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[4];
        QData/*32:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_n;
    };
    struct {
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n;
        WData/*523:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[17];
        WData/*193:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[7];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_n;
        WData/*105:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next;
        WData/*65:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[3];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_q;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_n;
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__status_word;
        QData/*63:0*/ idma_controlled_pair_top__DOT__launcher_issue_id[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__launcher_done_id[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[4];
        QData/*63:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[4];
        CData/*0:0*/ idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[4];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*3:0*/ idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i;
    CData/*3:0*/ idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i;
    CData/*0:0*/ idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
    CData/*1:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
    CData/*3:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q;
    CData/*0:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q;
    CData/*1:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q;
    CData/*3:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q;
    CData/*0:0*/ __Vclklast__TOP__clk_i;
    CData/*0:0*/ __Vclklast__TOP__rst_ni;
    WData/*287:0*/ idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[9];
    WData/*65:0*/ idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[3];
    WData/*287:0*/ idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[9];
    WData/*65:0*/ idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[3];
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q;
    QData/*63:0*/ idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i[4];
    QData/*63:0*/ idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i[4];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vidma_controlled_pair_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vidma_controlled_pair_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vidma_controlled_pair_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vidma_controlled_pair_top();
    
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
    static void _eval_initial_loop(Vidma_controlled_pair_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vidma_controlled_pair_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vidma_controlled_pair_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vidma_controlled_pair_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vidma_controlled_pair_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vidma_controlled_pair_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(Vidma_controlled_pair_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
