// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_controlled_launcher_top.h for the primary calling header

#include "Vdma_controlled_launcher_top.h"
#include "Vdma_controlled_launcher_top__Syms.h"

//==========

void Vdma_controlled_launcher_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_controlled_launcher_top::eval\n"); );
    Vdma_controlled_launcher_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_controlled_launcher_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/dma_controlled_launcher_top.sv", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_controlled_launcher_top::_eval_initial_loop(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/dma_controlled_launcher_top.sv", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_controlled_launcher_top::_sequent__TOP__1(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_controlled_launcher_top::_sequent__TOP__1\n"); );
    Vdma_controlled_launcher_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v4;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v4;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v4;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v4;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v3;
    // Body
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v4 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0 = 0U;
    __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v4 = 0U;
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_q 
            = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_n) 
               & 1U);
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_q 
            = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_n) 
               & 1U);
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_q 
            = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_n) 
               & 1U);
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_q 
            = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n) 
               & 1U);
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_q 
            = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_n) 
               & 1U);
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n;
    } else {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_q = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_q = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_q = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_q = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v4 = 1U;
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_n));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n));
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v4 = 1U;
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_n));
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n;
    } else {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v4 = 1U;
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n
            [0U];
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0 = 1U;
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v1 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n
            [1U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v2 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n
            [2U];
        __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v3 
            = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n
            [3U];
    } else {
        __Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v4 = 1U;
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n));
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[0U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[1U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[2U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[0U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[1U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[2U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[0U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[1U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[2U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[0U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[1U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[2U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q[3U] = 0ULL;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[0U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[1U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[2U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q[3U] = 0ULL;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[0U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[1U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[2U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q[3U] = 0ULL;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[0U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[1U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[2U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q[3U] = 0ULL;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[0U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[1U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[2U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q[3U] = 0ULL;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[0U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[1U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[2U] = 0U;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[0U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v0;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[1U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v1;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[2U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v2;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[3U] 
            = __Vdlyvval__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v3;
    }
    if (__Vdlyvset__dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q__v4) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[0U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[1U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[2U] = 0ULL;
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q[3U] = 0ULL;
    }
    vlTOPp->launch_hold_o = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q;
}

VL_INLINE_OPT void Vdma_controlled_launcher_top::_combo__TOP__2(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_controlled_launcher_top::_combo__TOP__2\n"); );
    Vdma_controlled_launcher_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i[0U] 
        = (((QData)((IData)(vlTOPp->obs_issue_id_i_flat[1U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_issue_id_i_flat[0U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i[0U] 
        = (((QData)((IData)(vlTOPp->obs_done_id_i_flat[1U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_done_id_i_flat[0U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i[1U] 
        = (((QData)((IData)(vlTOPp->obs_issue_id_i_flat[3U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_issue_id_i_flat[2U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i[1U] 
        = (((QData)((IData)(vlTOPp->obs_done_id_i_flat[3U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_done_id_i_flat[2U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i[2U] 
        = (((QData)((IData)(vlTOPp->obs_issue_id_i_flat[5U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_issue_id_i_flat[4U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i[2U] 
        = (((QData)((IData)(vlTOPp->obs_done_id_i_flat[5U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_done_id_i_flat[4U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i[3U] 
        = (((QData)((IData)(vlTOPp->obs_issue_id_i_flat[7U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_issue_id_i_flat[6U])));
    vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i[3U] 
        = (((QData)((IData)(vlTOPp->obs_done_id_i_flat[7U])) 
            << 0x20U) | (QData)((IData)(vlTOPp->obs_done_id_i_flat[6U])));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0xc0U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((0x20ULL > vlTOPp->cfg_wdata_i)) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_n 
                                = (0xffU & (IData)(vlTOPp->cfg_wdata_i));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_n 
                                = (1U & (IData)((vlTOPp->cfg_wdata_i 
                                                 >> 8U)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q;
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_n 
            = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q) 
               | (IData)(vlTOPp->frontend_held_i));
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((0xffffffffffffffffULL != vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q)) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_n 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q);
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_n = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_n = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[0U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[1U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[2U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[0U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[1U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[2U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[0U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[1U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[2U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[0U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[1U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[2U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q;
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n 
                = (1U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n));
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n 
                = (2U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n));
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n 
                = (4U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n));
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n 
                = (8U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n));
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[0U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
                   [0U]);
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[1U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
                   [1U]);
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[2U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
                   [2U]);
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[3U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
                   [3U]);
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[0U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[1U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[2U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[0U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[1U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[2U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[0U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
                   [0U]);
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[1U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
                   [1U]);
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[2U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
                   [2U]);
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[3U] 
                = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
                   [3U]);
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[0U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[1U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[2U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[0U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[1U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[2U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[0U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[1U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[2U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[0U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[1U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[2U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q;
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n 
                = (1U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n));
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n 
                = (2U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n));
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n 
                = (4U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n));
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n 
                = (8U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n));
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[0U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[1U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[2U] = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[0U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[1U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[2U] = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[0U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[1U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[2U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[3U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[0U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[1U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[2U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[0U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[1U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[2U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[0U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[1U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[2U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[3U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[0U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[1U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[2U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[0U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[1U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[2U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if ((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n = 1U;
                                } else {
                                    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n = 1U;
                                    }
                                }
                            } else {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word = 0ULL;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xffffffffULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_q)) 
              << 0x20U));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xffffffffffff0fffULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->frontend_active_i)) 
              << 0xcU));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xfffffffffffff0ffULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->frontend_held_i)) 
              << 8U));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xfffffffffffffff7ULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q)) 
              << 3U));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xfffffffffffffffbULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | ((QData)((IData)((3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) 
              << 2U));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xfffffffffffffffdULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | ((QData)((IData)((2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) 
              << 1U));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word 
        = ((0xfffffffffffffffeULL & vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word) 
           | (IData)((IData)((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))));
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff 
        = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q) 
           | (((((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i)) 
                & (0xffU == (IData)(vlTOPp->cfg_wstrb_i))) 
               & (0xb8U == (IData)(vlTOPp->cfg_addr_i))) 
              & (4ULL == vlTOPp->cfg_wdata_i)));
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_issue_id_i
        [3U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__obs_done_id_i
        [3U];
    vlTOPp->cfg_rdata_o = 0ULL;
    vlTOPp->cfg_rdata_o = ((0xb0U == (IData)(vlTOPp->cfg_addr_i))
                            ? 0x49444d414c4e4301ULL
                            : ((0xb8U == (IData)(vlTOPp->cfg_addr_i))
                                ? vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word
                                : ((0xc0U == (IData)(vlTOPp->cfg_addr_i))
                                    ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                    : ((0xc8U == (IData)(vlTOPp->cfg_addr_i))
                                        ? ((0x80U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                            ? 0ULL : 
                                           ((0x40U 
                                             & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                             ? 0ULL
                                             : ((0x20U 
                                                 & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                 ? 0ULL
                                                 : 
                                                ((0x10U 
                                                  & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                  ? 
                                                 ((8U 
                                                   & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 0ULL
                                                      : (QData)((IData)(vlTOPp->frontend_active_i)))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->frontend_held_i))
                                                      : (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q))))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
                                                     [1U]
                                                      : 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_count_q
                                                     [0U])
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
                                                     [1U]
                                                      : 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_count_q
                                                     [0U])))
                                                   : 
                                                  ((4U 
                                                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_q))
                                                      : vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_q)
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_q))
                                                      : (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_q))))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))
                                                      : (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q)))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q))
                                                      : (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))))))
                                                  : 
                                                 ((8U 
                                                   & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))
                                                      : 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q
                                                     [1U])
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q
                                                     [0U]
                                                      : 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
                                                     [1U]))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
                                                     [0U]
                                                      : 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q
                                                     [1U])
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_ts_q
                                                     [0U]
                                                      : 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q
                                                     [1U])))
                                                   : 
                                                  ((4U 
                                                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? 
                                                     vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_ts_q
                                                     [0U]
                                                      : vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_q)
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_q
                                                      : vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_q))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q
                                                      : vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__status_word)
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__index_q))
                                                      ? (QData)((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_q))
                                                      : 0x49444d414c4e4301ULL))))))))
                                        : 0ULL))));
    vlTOPp->launch_go_o = ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)
                            ? 0U : (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_q));
    vlTOPp->launch_cancel_o = 0U;
    if ((((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff))) {
        if ((1U & (((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                      & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                     & (IData)(vlTOPp->frontend_held_i)) 
                    & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) 
                   & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q))))) {
            vlTOPp->launch_cancel_o = (1U | (IData)(vlTOPp->launch_cancel_o));
        }
        if ((1U & ((((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                      & (IData)(vlTOPp->frontend_held_i)) 
                     >> 1U) & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                                  >> 1U))) & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                                                 >> 1U))))) {
            vlTOPp->launch_cancel_o = (2U | (IData)(vlTOPp->launch_cancel_o));
        }
        if ((1U & ((((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                      & (IData)(vlTOPp->frontend_held_i)) 
                     >> 2U) & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                                  >> 2U))) & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                                                 >> 2U))))) {
            vlTOPp->launch_cancel_o = (4U | (IData)(vlTOPp->launch_cancel_o));
        }
        if ((1U & ((((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                      & (IData)(vlTOPp->frontend_held_i)) 
                     >> 3U) & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                                  >> 3U))) & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                                                 >> 3U))))) {
            vlTOPp->launch_cancel_o = (8U | (IData)(vlTOPp->launch_cancel_o));
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff))) {
        if ((1U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ (IData)(vlTOPp->obs_issue_i)))) {
                if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n 
                        = (1U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n));
                }
            }
        }
        if ((2U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ ((IData)(vlTOPp->obs_issue_i) 
                          >> 1U)))) {
                if ((1U & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                              >> 1U)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n 
                        = (2U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n));
                }
            }
        }
        if ((4U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ ((IData)(vlTOPp->obs_issue_i) 
                          >> 2U)))) {
                if ((1U & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                              >> 2U)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n 
                        = (4U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n));
                }
            }
        }
        if ((8U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ ((IData)(vlTOPp->obs_issue_i) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                              >> 3U)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n 
                        = (8U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_n));
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_n = 1U;
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_q)))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_n = 1U;
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_id_n 
                = vlTOPp->select_id_i;
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_did_n 
                = vlTOPp->select_did_i;
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_n = 0U;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_write_n 
                = vlTOPp->select_write_i;
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_n = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_n = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__selected_iova_n 
                = vlTOPp->select_iova_i;
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n 
                                = (1U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n));
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n 
                                = ((0xdU & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n)) 
                                   | (2U & ((IData)(
                                                    (vlTOPp->cfg_wdata_i 
                                                     >> 8U)) 
                                            << 1U)));
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n 
                                = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if ((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n = 0U;
                                }
                            } else {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff))) {
        if ((1U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ (IData)(vlTOPp->obs_issue_i)))) {
                if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n 
                        = (0xeU & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n));
                }
            }
        }
        if ((2U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ ((IData)(vlTOPp->obs_issue_i) 
                          >> 1U)))) {
                if ((1U & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                              >> 1U)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n 
                        = (0xdU & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n));
                }
            }
        }
        if ((4U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ ((IData)(vlTOPp->obs_issue_i) 
                          >> 2U)))) {
                if ((1U & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                              >> 2U)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n 
                        = (0xbU & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n));
                }
            }
        }
        if ((8U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (~ ((IData)(vlTOPp->obs_issue_i) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q) 
                              >> 3U)))) {
                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n 
                        = (7U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n));
                }
            }
        }
    }
    if (((2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q)))) {
        if ((((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)) 
               & ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q) 
                  == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))) 
              & (0U == ((IData)(vlTOPp->frontend_held_i) 
                        & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) 
             & (0U == ((IData)(vlTOPp->frontend_active_i) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n = 0U;
        }
    }
    if (((2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q))) {
        if (((((0U == ((IData)(vlTOPp->frontend_held_i) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))) 
               & (0U == ((IData)(vlTOPp->frontend_active_i) 
                         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) 
              & (0U == ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                        & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))))) 
             & (0U == ((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) 
                        & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q))) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q))))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_n = 0U;
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_n = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_n = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_valid_q)))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_ts_n 
                    = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q;
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[0U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                    [0U];
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[1U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                    [1U];
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[2U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                    [2U];
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[3U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                    [3U];
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[0U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[1U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[2U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[0U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[1U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[2U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[0U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q
        [0U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[1U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q
        [1U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[2U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q
        [2U];
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[3U] 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_q
        [3U];
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[0U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                    [0U];
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[1U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                    [1U];
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[2U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                    [2U];
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_valid_q
                       [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[3U] 
                    = vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                    [3U];
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0xb0U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[0U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[1U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[2U] = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[0U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[1U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[2U] = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_id_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n = 0U;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_q;
    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n 
        = vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q;
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((0xffffffffffffffffULL == vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q)) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x2000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
    }
    if ((((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
         & (0U != ((IData)(vlTOPp->frontend_error_i) 
                   & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))))) {
        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
            = (0x20U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
    }
    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
        if ((0U != ((IData)(vlTOPp->obs_issue_i) & 
                    (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x40U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
        if ((0U != ((IData)(vlTOPp->obs_done_i) & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x80U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x40U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((1U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x800U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((0ULL == vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                 [0U])) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x200U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((1U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x80U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((1U & ((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                       & (~ (IData)(vlTOPp->obs_issue_i))))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x100U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((1U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x1000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                 [0U] & (vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                         [0U] != vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
                         [0U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x400U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((1U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->frontend_cancelled_i)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x40U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((2U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x800U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((0ULL == vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                 [1U])) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x200U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x80U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((1U & ((~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                           >> 1U)) & (~ ((IData)(vlTOPp->obs_issue_i) 
                                         >> 1U))))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x100U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((2U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x1000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                 [1U] & (vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                         [1U] != vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
                         [1U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x400U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((2U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->frontend_cancelled_i)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x40U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((4U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x800U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((0ULL == vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                 [2U])) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x200U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x80U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((1U & ((~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                           >> 2U)) & (~ ((IData)(vlTOPp->obs_issue_i) 
                                         >> 2U))))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x100U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((4U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x1000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                 [2U] & (vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                         [2U] != vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
                         [2U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x400U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((4U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->frontend_cancelled_i)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x40U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((8U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x800U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((0ULL == vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_issue_id_i
                 [3U])) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x200U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_done_i) & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x80U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((1U & ((~ ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                           >> 3U)) & (~ ((IData)(vlTOPp->obs_issue_i) 
                                         >> 3U))))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x100U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((8U & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x1000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
            if ((vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_valid_q
                 [3U] & (vlTOPp->dma_controlled_launcher_top__DOT____Vcellinp__i_dut__obs_done_id_i
                         [3U] != vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issue_id_q
                         [3U]))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x400U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((8U & ((IData)(vlTOPp->obs_issue_i) & (IData)(vlTOPp->frontend_cancelled_i)))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU != (IData)(vlTOPp->cfg_wstrb_i))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                = (1U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
        } else {
            if ((0xb0U == (IData)(vlTOPp->cfg_addr_i))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x10000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            } else {
                if ((0xb8U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & vlTOPp->cfg_wdata_i))) {
                        if (((((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->frontend_active_i))) 
                             & (0U == (IData)(vlTOPp->frontend_held_i)))) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 1U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_n = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n = 0ULL;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_n = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_n = 0U;
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n = 0U;
                        } else {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                                = (4U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
                        }
                    } else {
                        if ((2ULL == vlTOPp->cfg_wdata_i)) {
                            if ((((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                  & (0U == vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n)) 
                                 & (((IData)(vlTOPp->frontend_held_i) 
                                     & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)) 
                                    == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) {
                                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 2U;
                                if (vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q) {
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n 
                                        = (2U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n));
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_n = 1U;
                                    if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_n 
                                            = (1ULL 
                                               + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q);
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_n = 1U;
                                    }
                                } else {
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n 
                                        = (1U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n));
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n = 1U;
                                    if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n 
                                            = (1ULL 
                                               + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q);
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n = 1U;
                                    }
                                }
                            } else {
                                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                                    = (8U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
                            }
                        } else {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if ((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) {
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 3U;
                                } else {
                                    if (((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 2U;
                                    } else {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                                            = (0x10U 
                                               | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
                                    }
                                }
                            } else {
                                if ((8ULL == vlTOPp->cfg_wdata_i)) {
                                    if (((0U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)))) {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_n = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n = 0ULL;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_a_ts_valid_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_go_sent_n = 0U;
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n = 0U;
                                    } else {
                                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                                            = (2U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
                                    }
                                } else {
                                    vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                                        = (2U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
                                }
                            }
                        }
                    }
                } else {
                    if ((0xc0U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((0x20ULL <= vlTOPp->cfg_wdata_i)) {
                            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                                = (0x4000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
                        }
                    } else {
                        vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                            = ((0xc8U == (IData)(vlTOPp->cfg_addr_i))
                                ? (0x8000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n)
                                : (2U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n));
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
           & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->obs_issue_i) 
                                                    | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n 
                = (1U | (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_pulse_n));
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__v_go_sent_n = 1U;
            if ((1U & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_q)))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_n 
                    = (1ULL + vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__ticks_q);
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__go_v_ts_valid_n = 1U;
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q))) 
         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__abort_eff))) {
        if ((1U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((1U & (IData)(vlTOPp->obs_issue_i))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((2U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((2U & (IData)(vlTOPp->obs_issue_i))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((4U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((4U & (IData)(vlTOPp->obs_issue_i))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
        if ((8U & (((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->frontend_cancelled_i)))) {
            if ((8U & (IData)(vlTOPp->obs_issue_i))) {
                vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n 
                    = (0x20000U | vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__error_n);
            }
        }
    }
    if (((2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q)))) {
        if ((((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)) 
               & ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q) 
                  == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))) 
              & (0U == ((IData)(vlTOPp->frontend_held_i) 
                        & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) 
             & (0U == ((IData)(vlTOPp->frontend_active_i) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 3U;
        }
    }
    if (((2U == (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_q)) 
         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__aborted_q))) {
        if (((((0U == ((IData)(vlTOPp->frontend_held_i) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q))) 
               & (0U == ((IData)(vlTOPp->frontend_active_i) 
                         & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q)))) 
              & (0U == ((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q) 
                        & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__done_mask_q))))) 
             & (0U == ((((IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__expected_mask_q) 
                         & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__issued_mask_q))) 
                        & (~ (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__cancelled_mask_q))) 
                       & (IData)(vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__seen_held_q))))) {
            vlTOPp->dma_controlled_launcher_top__DOT__i_dut__DOT__state_n = 3U;
        }
    }
}

void Vdma_controlled_launcher_top::_eval(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_controlled_launcher_top::_eval\n"); );
    Vdma_controlled_launcher_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__2(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vdma_controlled_launcher_top::_change_request(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_controlled_launcher_top::_change_request\n"); );
    Vdma_controlled_launcher_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_controlled_launcher_top::_change_request_1(Vdma_controlled_launcher_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_controlled_launcher_top::_change_request_1\n"); );
    Vdma_controlled_launcher_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_controlled_launcher_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_controlled_launcher_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((cfg_valid_i & 0xfeU))) {
        Verilated::overWidthError("cfg_valid_i");}
    if (VL_UNLIKELY((cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("cfg_write_i");}
    if (VL_UNLIKELY((frontend_held_i & 0xf0U))) {
        Verilated::overWidthError("frontend_held_i");}
    if (VL_UNLIKELY((frontend_active_i & 0xf0U))) {
        Verilated::overWidthError("frontend_active_i");}
    if (VL_UNLIKELY((frontend_cancelled_i & 0xf0U))) {
        Verilated::overWidthError("frontend_cancelled_i");}
    if (VL_UNLIKELY((frontend_error_i & 0xf0U))) {
        Verilated::overWidthError("frontend_error_i");}
    if (VL_UNLIKELY((obs_issue_i & 0xf0U))) {
        Verilated::overWidthError("obs_issue_i");}
    if (VL_UNLIKELY((obs_done_i & 0xf0U))) {
        Verilated::overWidthError("obs_done_i");}
    if (VL_UNLIKELY((selected_i & 0xfeU))) {
        Verilated::overWidthError("selected_i");}
    if (VL_UNLIKELY((select_write_i & 0xfeU))) {
        Verilated::overWidthError("select_write_i");}
    if (VL_UNLIKELY((select_did_i & 0xff000000U))) {
        Verilated::overWidthError("select_did_i");}
}
#endif  // VL_DEBUG
