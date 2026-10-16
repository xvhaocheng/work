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
        CData/*0:0*/ dma_observer__DOT__busy;
        CData/*0:0*/ dma_observer__DOT__active;
        CData/*0:0*/ dma_observer__DOT__path_q;
        CData/*0:0*/ dma_observer__DOT__path_n;
        CData/*0:0*/ dma_observer__DOT__path_arm;
        CData/*0:0*/ dma_observer__DOT__path_clear;
        CData/*0:0*/ dma_observer__DOT__e3_q;
        CData/*0:0*/ dma_observer__DOT__e3_n;
        CData/*0:0*/ dma_observer__DOT__delay_q;
        CData/*0:0*/ dma_observer__DOT__delay_n;
        CData/*0:0*/ dma_observer__DOT__e3_arm;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__run_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__bad_q;
        CData/*3:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__in_hold_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__out_hold_q;
        IData/*19:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__event_chain;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    };
    struct {
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
    };
    struct {
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
        CData/*1:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q;
    };
    struct {
        CData/*2:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad;
        SData/*12:0*/ dma_observer__DOT__index_q;
        SData/*12:0*/ dma_observer__DOT__index_n;
        SData/*15:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__stage_error;
        IData/*31:0*/ dma_observer__DOT__error_q;
        IData/*31:0*/ dma_observer__DOT__error_n;
        IData/*31:0*/ dma_observer__DOT__p4m_error_q;
        IData/*31:0*/ dma_observer__DOT__p4m_error_n;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__time_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    };
    struct {
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
        QData/*63:0*/ dma_observer__DOT__segment_q;
        QData/*63:0*/ dma_observer__DOT__segment_n;
        QData/*63:0*/ dma_observer__DOT__target_q;
        QData/*63:0*/ dma_observer__DOT__target_n;
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
        QData/*63:0*/ dma_observer__DOT__p4m_cycles_q;
        QData/*63:0*/ dma_observer__DOT__p4m_cycles_n;
        QData/*63:0*/ dma_observer__DOT__p4m_a_issue_q;
        QData/*63:0*/ dma_observer__DOT__p4m_a_issue_n;
        QData/*63:0*/ dma_observer__DOT__p4m_a_done_q;
        QData/*63:0*/ dma_observer__DOT__p4m_a_done_n;
        QData/*63:0*/ dma_observer__DOT__p4m_window_issue_q;
        QData/*63:0*/ dma_observer__DOT__p4m_window_issue_n;
        QData/*63:0*/ dma_observer__DOT__p4m_window_done_q;
        QData/*63:0*/ dma_observer__DOT__p4m_window_done_n;
        QData/*63:0*/ dma_observer__DOT__p4m_data;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next;
    };
    struct {
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
        QData/*47:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next;
        QData/*48:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next;
        QData/*63:0*/ dma_observer__DOT__count_q[35];
        QData/*63:0*/ dma_observer__DOT__count_n[35];
        CData/*7:0*/ dma_observer__DOT__reads_q[4];
        CData/*7:0*/ dma_observer__DOT__reads_n[4];
        CData/*7:0*/ dma_observer__DOT__writes_q[4];
        CData/*7:0*/ dma_observer__DOT__writes_n[4];
        CData/*2:0*/ dma_observer__DOT__lane_err_q[4];
        CData/*2:0*/ dma_observer__DOT__lane_err_n[4];
        QData/*63:0*/ dma_observer__DOT__lane_id_q[4];
        QData/*63:0*/ dma_observer__DOT__lane_id_n[4];
        CData/*3:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__out_held_id_q[2];
    };
    struct {
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__result[4][4][8];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[4];
        IData/*31:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[4];
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[4];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*1:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__tr_ready_i;
        CData/*1:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__tr_valid_i;
        CData/*7:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__in_ready_i;
        CData/*7:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__in_valid_i;
        CData/*0:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__close_i;
        CData/*0:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__open_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o;
        CData/*0:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i;
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
        CData/*0:0*/ __Vdly__dma_observer__DOT__gen_path__DOT__i_path__DOT__run_q;
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
    };
    struct {
        IData/*31:0*/ __Vtask_dma_observer__DOT__bump__13__slot;
        IData/*31:0*/ __Vtask_dma_observer__DOT__bump__14__slot;
        IData/*31:0*/ __Vtask_dma_observer__DOT__bump__15__slot;
        IData/*31:0*/ __Vtask_dma_observer__DOT__bump__16__slot;
        IData/*31:0*/ __Vtask_dma_observer__DOT__bump__17__slot;
        IData/*31:0*/ __Vtask_dma_observer__DOT__bump__18__slot;
        QData/*63:0*/ dma_observer__DOT____Vlvbound1;
        QData/*63:0*/ dma_observer__DOT____Vlvbound2;
        QData/*63:0*/ dma_observer__DOT____Vlvbound4;
        CData/*3:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__return_id_i[2];
        CData/*3:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__out_id_i[2];
        CData/*3:0*/ dma_observer__DOT____Vcellinp__gen_path__DOT__i_path__tr_id_i[2];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[8];
        QData/*63:0*/ dma_observer__DOT__gen_path__DOT__i_path__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[8];
    };
    
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
    static void _combo__TOP__1(Vdma_observer__Syms* __restrict vlSymsp);
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
    static void _sequent__TOP__3(Vdma_observer__Syms* __restrict vlSymsp);
    static void _sequent__TOP__4(Vdma_observer__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vdma_observer__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
