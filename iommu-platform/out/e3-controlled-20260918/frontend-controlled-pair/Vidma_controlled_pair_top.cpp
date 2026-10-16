// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_controlled_pair_top.h for the primary calling header

#include "Vidma_controlled_pair_top.h"
#include "Vidma_controlled_pair_top__Syms.h"

//==========

void Vidma_controlled_pair_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vidma_controlled_pair_top::eval\n"); );
    Vidma_controlled_pair_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vidma_controlled_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/idma_controlled_pair_top.sv", 4, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vidma_controlled_pair_top::_eval_initial_loop(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/idma_controlled_pair_top.sv", 4, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vidma_controlled_pair_top::_sequent__TOP__2(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_controlled_pair_top::_sequent__TOP__2\n"); );
    Vidma_controlled_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v4;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v4;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v4;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v4;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v4;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v4;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v4;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v4;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v4;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v1;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v2;
    CData/*0:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v3;
    CData/*0:0*/ __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v4;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v3;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v3;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v3;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v3;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v3;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v3;
    // Body
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v4 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0 = 0U;
    __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v4 = 0U;
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_q 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_n) 
               & 1U);
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_n;
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q = 0ULL;
    }
    if ((1U & (~ (IData)(vlTOPp->rst_ni)))) {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q = 0U;
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_n));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_n));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v4 = 1U;
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_n));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n));
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_q 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_n) 
               & 1U);
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_n) 
               & 1U);
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v4 = 1U;
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_q 
        = ((IData)(vlTOPp->rst_ni) ? vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n
            : 0U);
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n
            [0U];
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0 = 1U;
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v1 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n
            [1U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v2 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n
            [2U];
        __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v3 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n
            [3U];
    } else {
        __Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_n;
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q = 1ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q = 2ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q = 1ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q = 2ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q = 0ULL;
    }
    if ((1U & (~ (IData)(vlTOPp->rst_ni)))) {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n;
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->rst_ni)))) {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n) 
               & 1U);
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q 
            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n;
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U])) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[0U])));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U])) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[0U])));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = 0ULL;
    }
    if ((1U & (~ (IData)(vlTOPp->rst_ni)))) {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wdata_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wdata_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we)
                    ? (0xfU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U] 
                                << 0x12U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
                                             >> 0xeU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we)
                    ? (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U] 
                              << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
                                           >> 0xcU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
                      >> 0xbU));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
                      >> 0xaU));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
                      >> 9U));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
                      >> 8U));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wdata_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wdata_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we)
                    ? (0xfU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U] 
                                << 0x12U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
                                             >> 0xeU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we)
                    ? (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U] 
                              << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
                                           >> 0xcU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
                      >> 0xbU));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
                      >> 0xaU));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
                      >> 9U));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we) 
                   & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
                      >> 8U));
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = 0ULL;
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_n));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_n));
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[0U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[2U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[0U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[2U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[0U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[2U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[0U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[2U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q[3U] = 0U;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[0U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[1U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[2U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q[3U] = 0ULL;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[0U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[1U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[2U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q[3U] = 0ULL;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[0U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[1U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[2U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q[3U] = 0ULL;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[0U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[1U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[2U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q[3U] = 0ULL;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[0U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[1U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[2U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q[3U] = 0ULL;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[0U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v0;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[1U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v1;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[2U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v2;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[3U] 
            = __Vdlyvval__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v3;
    }
    if (__Vdlyvset__idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q__v4) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[0U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[1U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[2U] = 0ULL;
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q[3U] = 0ULL;
    }
    vlTOPp->launch_hold_o = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q;
    vlTOPp->a_valid_o = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                vlTOPp->a_valid_o = 1U;
            }
        }
    }
    vlTOPp->v_valid_o = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                vlTOPp->v_valid_o = 1U;
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__fe_active 
        = ((((((3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)) 
               | (1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) 
              | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) 
             | (4U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) 
            << 1U) | ((((3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)) 
                        | (1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) 
                       | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) 
                      | (4U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))));
    vlTOPp->idma_controlled_pair_top__DOT__fe_held 
        = (((3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)) 
            << 1U) | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)));
    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) {
        vlTOPp->a_issue_id_o = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q);
        vlTOPp->v_issue_id_o = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q);
    } else {
        vlTOPp->a_issue_id_o = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
                                       | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                                          >> 1U))) ? 
                                ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q)
                                  ? vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_q
                                  : vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q)
                                 : vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q);
        vlTOPp->v_issue_id_o = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
                                       | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)))
                                 ? ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q)
                                     ? vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_q
                                     : vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q)
                                 : vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q);
    }
    vlTOPp->frontend_active_o = vlTOPp->idma_controlled_pair_top__DOT__fe_active;
    vlTOPp->frontend_held_o = vlTOPp->idma_controlled_pair_top__DOT__fe_held;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word = 0ULL;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xffffffffULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_q)) 
              << 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xffffffffffff0fffULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active)) 
              << 0xcU));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xfffffffffffff0ffULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)) 
              << 8U));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xfffffffffffffff7ULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q)) 
              << 3U));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xfffffffffffffffbULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | ((QData)((IData)((3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) 
              << 2U));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xfffffffffffffffdULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | ((QData)((IData)((2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) 
              << 1U));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word 
        = ((0xfffffffffffffffeULL & vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word) 
           | (IData)((IData)((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))));
    vlTOPp->idma_controlled_pair_top__DOT__launcher_issue_id[1U] 
        = vlTOPp->a_issue_id_o;
    vlTOPp->idma_controlled_pair_top__DOT__launcher_issue_id[0U] 
        = vlTOPp->v_issue_id_o;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_issue_id
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_issue_id
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_issue_id
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_issue_id
        [3U];
}

VL_INLINE_OPT void Vidma_controlled_pair_top::_combo__TOP__3(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_controlled_pair_top::_combo__TOP__3\n"); );
    Vidma_controlled_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*223:0*/ __Vtemp53[7];
    WData/*287:0*/ __Vtemp54[9];
    WData/*287:0*/ __Vtemp55[9];
    WData/*351:0*/ __Vtemp56[11];
    WData/*223:0*/ __Vtemp68[7];
    WData/*287:0*/ __Vtemp69[9];
    WData/*287:0*/ __Vtemp70[9];
    WData/*351:0*/ __Vtemp71[11];
    // Body
    vlTOPp->idma_controlled_pair_top__DOT__a_req[0U] 
        = ((0xffffff00U & ((IData)(vlTOPp->a_cpu_wdata_i) 
                           << 8U)) | (IData)(vlTOPp->a_cpu_wstrb_i));
    vlTOPp->idma_controlled_pair_top__DOT__a_req[1U] 
        = ((0xffU & ((IData)(vlTOPp->a_cpu_wdata_i) 
                     >> 0x18U)) | (0xffffff00U & ((IData)(
                                                          (vlTOPp->a_cpu_wdata_i 
                                                           >> 0x20U)) 
                                                  << 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__a_req[2U] 
        = ((0xffU & ((IData)((vlTOPp->a_cpu_wdata_i 
                              >> 0x20U)) >> 0x18U)) 
           | (0xffffff00U & (vlTOPp->a_cpu_addr_i << 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
        = ((0xfffffe00U & ((IData)(vlTOPp->a_cpu_valid_i) 
                           << 9U)) | ((0xffffff00U 
                                       & ((IData)(vlTOPp->a_cpu_write_i) 
                                          << 8U)) | 
                                      (0xffU & (vlTOPp->a_cpu_addr_i 
                                                >> 0x18U))));
    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U] 
        = ((0xffffff00U & ((IData)(vlTOPp->v_cpu_wdata_i) 
                           << 8U)) | (IData)(vlTOPp->v_cpu_wstrb_i));
    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U] 
        = ((0xffU & ((IData)(vlTOPp->v_cpu_wdata_i) 
                     >> 0x18U)) | (0xffffff00U & ((IData)(
                                                          (vlTOPp->v_cpu_wdata_i 
                                                           >> 0x20U)) 
                                                  << 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
        = ((0xffU & ((IData)((vlTOPp->v_cpu_wdata_i 
                              >> 0x20U)) >> 0x18U)) 
           | (0xffffff00U & (vlTOPp->v_cpu_addr_i << 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
        = ((0xfffffe00U & ((IData)(vlTOPp->v_cpu_valid_i) 
                           << 9U)) | ((0xffffff00U 
                                       & ((IData)(vlTOPp->v_cpu_write_i) 
                                          << 8U)) | 
                                      (0xffU & (vlTOPp->v_cpu_addr_i 
                                                >> 0x18U))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_n 
        = (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q 
           + (QData)((IData)((1U & (~ (IData)(vlTOPp->a_backend_idle_i))))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_n 
        = (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q 
           + (QData)((IData)((1U & (~ (IData)(vlTOPp->v_backend_idle_i))))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read 
        = (((vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
             >> 9U) & (~ (vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
                          >> 8U))) & (0x28U == (0xffU 
                                                & ((vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_controlled_pair_top__DOT__a_req[2U] 
                                                      >> 8U)))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_locked_write 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
           & ((vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
               >> 9U) & (vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
                         >> 8U)));
    vlTOPp->launcher_write_o = (1U & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                      >> 8U));
    vlTOPp->launcher_addr_o = (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read 
        = (((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
             >> 9U) & (~ (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                          >> 8U))) & (0x28U == (0xffU 
                                                & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                      >> 8U)))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access 
        = ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 9U) & (0xb0U <= (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                          << 0x18U) 
                                         | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                            >> 8U)))));
    vlTOPp->launcher_rdata_o = 0ULL;
    vlTOPp->launcher_rdata_o = ((0xb0U == (0xffU & 
                                           ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                             << 0x18U) 
                                            | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                               >> 8U))))
                                 ? 0x49444d414c4e4301ULL
                                 : ((0xb8U == (0xffU 
                                               & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                   << 0x18U) 
                                                  | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                     >> 8U))))
                                     ? vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word
                                     : ((0xc0U == (0xffU 
                                                   & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                       << 0x18U) 
                                                      | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                         >> 8U))))
                                         ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                         : ((0xc8U 
                                             == (0xffU 
                                                 & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                     << 0x18U) 
                                                    | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                       >> 8U))))
                                             ? ((0x80U 
                                                 & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                 ? 0ULL
                                                 : 
                                                ((0x40U 
                                                  & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                  ? 0ULL
                                                  : 
                                                 ((0x20U 
                                                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                   ? 0ULL
                                                   : 
                                                  ((0x10U 
                                                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                    ? 
                                                   ((8U 
                                                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 0ULL
                                                        : (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active)))
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held))
                                                        : (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q))))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
                                                       [1U]
                                                        : 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
                                                       [0U])
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
                                                       [1U]
                                                        : 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
                                                       [0U])))
                                                     : 
                                                    ((4U 
                                                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_q))
                                                        : vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_q)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_q))
                                                        : (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_q))))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))
                                                        : (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q)))
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q))
                                                        : (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))))))
                                                    : 
                                                   ((8U 
                                                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))
                                                        : 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q
                                                       [1U])
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q
                                                       [0U]
                                                        : 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
                                                       [1U]))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
                                                       [0U]
                                                        : 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q
                                                       [1U])
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q
                                                       [0U]
                                                        : 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q
                                                       [1U])))
                                                     : 
                                                    ((4U 
                                                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? 
                                                       vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q
                                                       [0U]
                                                        : vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_q)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_q
                                                        : vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_q))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q
                                                        : vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__status_word)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q))
                                                        ? (QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_q))
                                                        : 0x49444d414c4e4301ULL))))))))
                                             : 0ULL))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__a_req[0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__a_req[1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__a_req[2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__a_req[3U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
        = ((0x1ffU & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U]) 
           | (0x200U & ((vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
                         & ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_locked_write)) 
                            << 9U)) & ((~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
                                           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) 
                                       << 9U))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__observer_access 
        = (((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access)) 
            & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
               >> 9U)) & (((((0x58U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                  << 0x18U) 
                                                 | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                    >> 8U)))) 
                             | (0x60U == (0xffU & (
                                                   (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                      >> 8U))))) 
                            | (0x68U == (0xffU & ((
                                                   vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                   << 0x18U) 
                                                  | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                     >> 8U))))) 
                           | (0x70U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                  << 0x18U) 
                                                 | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                    >> 8U))))) 
                          | (0x78U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                 << 0x18U) 
                                                | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                   >> 8U))))));
    vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata 
        = ((0x8000U & vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])
            ? vlTOPp->launcher_rdata_o : 0ULL);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                  >> 9U) & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                               >> 8U))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                  >> 9U) & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                            >> 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                               << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                            >> 8U)))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                             >> 8U)))) 
              << 1U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 2U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 3U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 4U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 5U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 6U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 7U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 8U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 9U));
    vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_valid 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__observer_access) 
           | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_locked_write 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
           & ((((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                 >> 9U) & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                           >> 8U)) & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__observer_access))) 
              & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__base_req[0U]))))));
    vlTOPp->launcher_valid_o = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_valid) 
                                & (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                   >> 0xfU));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_valid) 
           & (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
              >> 0xfU));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__v_req[0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__v_req[1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__v_req[2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__v_req[3U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
        = ((0x1ffU & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U]) 
           | (0x200U & ((((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                           & ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__observer_access)) 
                              << 9U)) & ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access)) 
                                         << 9U)) & 
                         ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_locked_write)) 
                          << 9U)) & ((~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
                                         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) 
                                     << 9U))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    __Vtemp53[0U] = (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp53[1U] = (IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                             >> 0x20U));
    __Vtemp53[2U] = (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp53[3U] = (IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                             >> 0x20U));
    __Vtemp53[4U] = (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp53[5U] = (IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                             >> 0x20U));
    __Vtemp53[6U] = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 6U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_re)) 
                     & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp54, __Vtemp53);
    __Vtemp55[0U] = __Vtemp54[0U];
    __Vtemp55[1U] = __Vtemp54[1U];
    __Vtemp55[2U] = __Vtemp54[2U];
    __Vtemp55[3U] = __Vtemp54[3U];
    __Vtemp55[4U] = __Vtemp54[4U];
    __Vtemp55[5U] = __Vtemp54[5U];
    __Vtemp55[6U] = __Vtemp54[6U];
    __Vtemp55[7U] = __Vtemp54[7U];
    __Vtemp55[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                      >> 4U) & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                << 1U)) 
                                    & ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                       << 1U))) | __Vtemp54[8U]);
    VL_EXTEND_WW(322,258, __Vtemp56, __Vtemp55);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0U] 
        = __Vtemp56[0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[1U] 
        = __Vtemp56[1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[2U] 
        = __Vtemp56[2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[3U] 
        = __Vtemp56[3U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[4U] 
        = __Vtemp56[4U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[5U] 
        = __Vtemp56[5U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[6U] 
        = __Vtemp56[6U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[7U] 
        = __Vtemp56[7U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U] 
        = __Vtemp56[8U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[9U] 
        = __Vtemp56[9U];
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp56[0xaU])))))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((0xc0U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                             << 0x18U) 
                                            | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                               >> 8U))))) {
                        if ((0x20ULL > (((QData)((IData)(
                                                         vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                         << 0x38U) 
                                        | (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                            << 0x18U) 
                                           | ((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                              >> 8U))))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__index_n 
                                = (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[1U] 
                                             << 0x18U) 
                                            | (vlTOPp->idma_controlled_pair_top__DOT__v_req[0U] 
                                               >> 8U)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_n 
                                = (1U & (vlTOPp->idma_controlled_pair_top__DOT__v_req[0U] 
                                         >> 0x10U));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q;
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_n 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q) 
               | (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held));
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL == (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n = 1U;
                                } else {
                                    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n = 1U;
                                    }
                                }
                            } else {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((0xffffffffffffffffULL != vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q)) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_n 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q);
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_n = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_n = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q) 
           | (((((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
                 & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                    >> 8U)) & (0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) 
               & (0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                      << 0x18U) | (
                                                   vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                   >> 8U))))) 
              & (4ULL == (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                           << 0x38U) | (((QData)((IData)(
                                                         vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                         << 0x18U) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                           >> 8U))))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                  >> 9U) & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                               >> 8U))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                  >> 9U) & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                            >> 8U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                               << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                            >> 8U)))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                             >> 8U)))) 
              << 1U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 2U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 3U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 4U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 5U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 6U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 7U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 8U));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[3U] 
                                   << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[2U] 
                                                >> 8U)))) 
              << 9U));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if (vlTOPp->a_trans_complete_i) {
                    if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))))) {
                            if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                 & (0U != vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_n 
                                    = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[3U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[4U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[5U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[6U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[7U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[8U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[7U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                               >> 0xcU))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[8U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[5U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0x10U])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                                               >> 0xcU))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[6U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0x10U])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[3U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                               >> 0xcU))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[4U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U] 
        = (0xfffffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U] 
        = (0xb0U | (0xf000000fU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U]));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U] 
        = ((0xfffffff7U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U]) 
           | (8U & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 5U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = ((0x7fffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]) 
           | (0xffff8000U & (((0x100U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])
                               ? 0x15U : 0U) << 0xfU)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U] 
        = ((0xfffffff8U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[2U]) 
           | (0x7fffU & (((0x100U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])
                           ? 0x15U : 0U) >> 0x11U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = (0x2000U | (0xffff9fffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = ((0xffffe1ffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]) 
           | (0x1e00U & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                         << 7U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = (0xfffffeffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = (0xffffff1fU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = (0xffffffe1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0x1fffffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U] 
        = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0x8000000U | (0xe7ffffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = ((0xf87fffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]) 
           | (0x7800000U & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                            << 0x15U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xffbfffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xffc7ffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xfff87fffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xffff87ffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xfffffbffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = ((0xfffffdffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]) 
           | (0x200U & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                        >> 2U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xfffffe3fU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = (0xffffffc7U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = ((0xfffffffbU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]) 
           | (4U & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 8U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U] 
        = ((0xfffffffdU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[0U]) 
           | (2U & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 9U)));
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[5U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0x10U])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q));
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[6U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0x10U])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q) 
                               >> 0x20U));
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[3U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q));
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[4U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q) 
                               >> 0x20U));
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                  & (0ULL != vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if (vlTOPp->a_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok 
        = (((((((((((0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                                   << 0x1aU) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                                >> 6U)))) 
                    & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                          >> 8U))) & (0U == (0xfU & 
                                             ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                                               << 0x1eU) 
                                              | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                                 >> 2U))))) 
                  & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                        >> 0xbU))) & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                         >> 0xaU))) 
                & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                      >> 9U))) & (0x40ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                               << 0x34U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                                  << 0x14U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                    >> 0xcU))))) 
              & (0U == (0x3fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xfU] 
                                  << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xeU] 
                                               >> 0xcU))))) 
             & (0U == (0x3fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xdU] 
                                 << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU] 
                                              >> 0xcU))))) 
            & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))) 
           & (IData)(vlTOPp->a_backend_idle_i));
    vlTOPp->idma_controlled_pair_top__DOT__launcher_go 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)
            ? 0U : (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_q));
    vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel = 0U;
    if ((((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff))) {
        if ((1U & (((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)) 
                    & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) 
                   & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel 
                = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel));
        }
        if ((1U & ((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)) 
                     >> 1U) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                                  >> 1U))) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                                                 >> 1U))))) {
            vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel 
                = (2U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel));
        }
        if ((1U & ((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)) 
                     >> 2U) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                                  >> 2U))) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                                                 >> 2U))))) {
            vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel 
                = (4U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel));
        }
        if ((1U & ((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                      & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)) 
                     >> 3U) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                                  >> 3U))) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                                                 >> 3U))))) {
            vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel 
                = (8U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel));
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__base_req[0U]))))));
    vlTOPp->a_burst_src_addr_o = (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[6U])) 
                                   << 0x20U) | (QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[5U])));
    vlTOPp->a_burst_dst_addr_o = (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[4U])) 
                                   << 0x20U) | (QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[3U])));
    vlTOPp->a_burst_length_o = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[8U])) 
                                 << 0x20U) | (QData)((IData)(
                                                             vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__burst_req_o[7U])));
    vlTOPp->a_obs_done_o = ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                            & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__trans_complete));
    vlTOPp->a_done_id_o = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)
                            ? (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q)
                            : (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
                                & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__trans_complete))
                                ? vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_q
                                : ((0xffffffffffffffffULL 
                                    == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                                    ? 2ULL : (1ULL 
                                              + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q))));
    vlTOPp->launch_go_o = vlTOPp->idma_controlled_pair_top__DOT__launcher_go;
    vlTOPp->launch_cancel_o = vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel;
    vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled 
        = ((0xfffffffeU & (((3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)) 
                            << 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) 
           | ((3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)) 
              & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire = 1U;
                }
            } else {
                if (vlTOPp->a_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_n = 0U;
                }
            } else {
                if (vlTOPp->a_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_n = 1U;
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_n = 0ULL;
                }
            } else {
                if (vlTOPp->a_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_n = 0ULL;
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_id_n 
                            = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if (vlTOPp->a_backend_ready_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n 
                        = (0x1000ULL + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_q);
                    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                         & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n 
                            == ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q)) 
                                << 0xcU)))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n = 0ULL;
                    }
                }
            } else {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n = 0ULL;
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_offset_n = 0ULL;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if (vlTOPp->a_backend_ready_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_n 
                        = (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q 
                           - 1ULL);
                }
            } else {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_n 
                            = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[1U])) 
                                << 0x20U) | (QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0U])));
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_n 
                                    = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)
                                        ? vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q
                                        : (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[1U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0U]))));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 0U;
            } else {
                if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__gap_countdown_q)) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 1U;
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 0U;
                } else {
                    if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_go))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 1U;
                    }
                }
            } else {
                if (vlTOPp->a_trans_complete_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n 
                        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))
                                     ? 0U : (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                              & (0U 
                                                 != vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q))
                                              ? 4U : 1U)));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) {
                if (vlTOPp->a_backend_ready_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 2U;
                }
            } else {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 3U;
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 1U;
                                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                     & (~ (IData)(vlTOPp->a_backend_ready_i)))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_n = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U]) 
           | (2U & ((~ (IData)(vlTOPp->a_backend_idle_i)) 
                    << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U] 
                            = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U]) 
                               | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                                 << 1U)));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[5U] 
                            = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                      >> 0x1fU)) | 
                               (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                        >> 0x20U)) 
                                               << 1U)));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U] 
                            = ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U]) 
                               | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                 >> 0x20U)) 
                                        >> 0x1fU)));
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U] 
                                    = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U]) 
                                       | (0xfffffffeU 
                                          & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                             << 1U)));
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[5U] 
                                    = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                              >> 0x1fU)) 
                                       | (0xfffffffeU 
                                          & ((IData)(
                                                     (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                      >> 0x20U)) 
                                             << 1U)));
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U] 
                                    = ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U]) 
                                       | (1U & ((IData)(
                                                        (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                         >> 0x20U)) 
                                                >> 0x1fU)));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                   >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue = 1U;
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue = 1U;
                                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                     & (~ (IData)(vlTOPp->a_backend_ready_i)))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    __Vtemp68[0U] = (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp68[1U] = (IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                             >> 0x20U));
    __Vtemp68[2U] = (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp68[3U] = (IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                             >> 0x20U));
    __Vtemp68[4U] = (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp68[5U] = (IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                             >> 0x20U));
    __Vtemp68[6U] = ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 6U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_re)) 
                     & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp69, __Vtemp68);
    __Vtemp70[0U] = __Vtemp69[0U];
    __Vtemp70[1U] = __Vtemp69[1U];
    __Vtemp70[2U] = __Vtemp69[2U];
    __Vtemp70[3U] = __Vtemp69[3U];
    __Vtemp70[4U] = __Vtemp69[4U];
    __Vtemp70[5U] = __Vtemp69[5U];
    __Vtemp70[6U] = __Vtemp69[6U];
    __Vtemp70[7U] = __Vtemp69[7U];
    __Vtemp70[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                      >> 4U) & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                << 1U)) 
                                    & ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                       << 1U))) | __Vtemp69[8U]);
    VL_EXTEND_WW(322,258, __Vtemp71, __Vtemp70);
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0U] 
        = __Vtemp71[0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[1U] 
        = __Vtemp71[1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[2U] 
        = __Vtemp71[2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[3U] 
        = __Vtemp71[3U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[4U] 
        = __Vtemp71[4U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[5U] 
        = __Vtemp71[5U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[6U] 
        = __Vtemp71[6U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[7U] 
        = __Vtemp71[7U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U] 
        = __Vtemp71[8U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[9U] 
        = __Vtemp71[9U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp71[0xaU])))))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->idma_controlled_pair_top__DOT__launcher_done_id[1U] 
        = vlTOPp->a_done_id_o;
    vlTOPp->frontend_cancelled_o = vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled;
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__id_retire) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue) {
            vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->a_obs_issue_o = ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                             & ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
                                       | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                                          >> 1U))) ? 
                                (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
                                  & (1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q))) 
                                 & (IData)(vlTOPp->a_backend_ready_i))
                                 : (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__issue)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (vlTOPp->v_trans_complete_i) {
                    if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))))) {
                            if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                 & (0U != vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_n 
                                    = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire = 1U;
                }
            } else {
                if (vlTOPp->v_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[3U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[4U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[5U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[6U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[7U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[8U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[7U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                               >> 0xcU))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[8U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[5U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0x10U])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                                               >> 0xcU))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[6U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0x10U])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[3U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                               >> 0xcU))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[4U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U] 
        = (0xfffffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U] 
        = (0xa0U | (0xf000000fU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U]));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U] 
        = ((0xfffffff7U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U]) 
           | (8U & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 5U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = ((0x7fffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]) 
           | (0xffff8000U & (((0x100U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])
                               ? 0x14U : 0U) << 0xfU)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U] 
        = ((0xfffffff8U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[2U]) 
           | (0x7fffU & (((0x100U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])
                           ? 0x14U : 0U) >> 0x11U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = (0x2000U | (0xffff9fffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = ((0xffffe1ffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]) 
           | (0x1e00U & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                         << 7U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = (0xfffffeffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = (0xffffff1fU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = (0xffffffe1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0x1fffffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U] 
        = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[1U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0x8000000U | (0xe7ffffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = ((0xf87fffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]) 
           | (0x7800000U & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                            << 0x15U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xffbfffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xffc7ffffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xfff87fffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xffff87ffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xfffffbffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = ((0xfffffdffU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]) 
           | (0x200U & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                        >> 2U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xfffffe3fU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = (0xffffffc7U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = ((0xfffffffbU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]) 
           | (4U & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 8U)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U] 
        = ((0xfffffffdU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[0U]) 
           | (2U & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 9U)));
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[5U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0x10U])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q));
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[6U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0x10U])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q) 
                               >> 0x20U));
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[3U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q));
                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[4U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q) 
                               >> 0x20U));
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                  & (0ULL != vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (vlTOPp->v_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok 
        = (((((((((((0U == (3U & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                                   << 0x1aU) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                                >> 6U)))) 
                    & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                          >> 8U))) & (0U == (0xfU & 
                                             ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                                               << 0x1eU) 
                                              | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                                 >> 2U))))) 
                  & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                        >> 0xbU))) & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                         >> 0xaU))) 
                & (~ (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                      >> 9U))) & (0x40ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                               << 0x34U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                                  << 0x14U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                    >> 0xcU))))) 
              & (0U == (0x3fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xfU] 
                                  << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xeU] 
                                               >> 0xcU))))) 
             & (0U == (0x3fU & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xdU] 
                                 << 0x14U) | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU] 
                                              >> 0xcU))))) 
            & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))) 
           & (IData)(vlTOPp->v_backend_idle_i));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = ((0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                           << 1U)) | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__wr_err));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__id_retire) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->v_burst_src_addr_o = (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[6U])) 
                                   << 0x20U) | (QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[5U])));
    vlTOPp->v_burst_dst_addr_o = (((QData)((IData)(
                                                   vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[4U])) 
                                   << 0x20U) | (QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[3U])));
    vlTOPp->v_burst_length_o = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[8U])) 
                                 << 0x20U) | (QData)((IData)(
                                                             vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__burst_req_o[7U])));
    vlTOPp->v_obs_done_o = ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                            & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__trans_complete));
    vlTOPp->v_done_id_o = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)
                            ? (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q)
                            : (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
                                & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__trans_complete))
                                ? vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_q
                                : ((0xffffffffffffffffULL 
                                    == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                                    ? 2ULL : (1ULL 
                                              + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q))));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_n = 0U;
                }
            } else {
                if (vlTOPp->v_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_n = 1U;
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_n = 0ULL;
                }
            } else {
                if (vlTOPp->v_trans_complete_i) {
                    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_n = 0ULL;
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_id_n 
                            = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if (vlTOPp->v_backend_ready_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_n 
                        = (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q 
                           - 1ULL);
                }
            } else {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_n 
                            = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[1U])) 
                                << 0x20U) | (QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0U])));
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_n 
                                    = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)
                                        ? vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_target_q
                                        : (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[1U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0U]))));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if (vlTOPp->v_backend_ready_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n 
                        = (0x1000ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_q);
                    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                         & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n 
                            == ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_pages_q)) 
                                << 0xcU)))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n = 0ULL;
                    }
                }
            } else {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n = 0ULL;
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_offset_n = 0ULL;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 0U;
            } else {
                if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_countdown_q)) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 1U;
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 0U;
                } else {
                    if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_go))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 1U;
                    }
                }
            } else {
                if (vlTOPp->v_trans_complete_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n 
                        = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q))
                                     ? 0U : (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                              & (0U 
                                                 != vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__gap_cycles_q))
                                              ? 4U : 1U)));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) {
                if (vlTOPp->v_backend_ready_i) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 2U;
                }
            } else {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 3U;
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 1U;
                                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                     & (~ (IData)(vlTOPp->v_backend_ready_i)))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_n = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__fe_error 
        = ((((0xfffffffeU & (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)) 
                                << 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                              & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read) 
                                 << 1U)) & ((0xfffffffeU 
                                             & (IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel)) 
                                            | ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok)) 
                                               << 1U)))) 
             | (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
                 & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read)) 
                << 1U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_locked_write) 
                           << 1U)) | ((((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)) 
                                          & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                                         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read)) 
                                        & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                           | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))) 
                                       | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
                                          & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) 
                                      | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_locked_write)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U]) 
           | (2U & ((~ (IData)(vlTOPp->v_backend_idle_i)) 
                    << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U] 
                            = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U]) 
                               | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                                 << 1U)));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[5U] 
                            = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                      >> 0x1fU)) | 
                               (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                        >> 0x20U)) 
                                               << 1U)));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U] 
                            = ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U]) 
                               | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                 >> 0x20U)) 
                                        >> 0x1fU)));
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U] 
                                    = ((1U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U]) 
                                       | (0xfffffffeU 
                                          & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                             << 1U)));
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[5U] 
                                    = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                              >> 0x1fU)) 
                                       | (0xfffffffeU 
                                          & ((IData)(
                                                     (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                      >> 0x20U)) 
                                             << 1U)));
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U] 
                                    = ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U]) 
                                       | (1U & ((IData)(
                                                        (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                         >> 0x20U)) 
                                                >> 0x1fU)));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                  | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue = 1U;
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                      & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue = 1U;
                                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                                     & (~ (IData)(vlTOPp->v_backend_ready_i)))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_read 
        = (((vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
             >> 9U) & (~ (vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
                          >> 8U))) & (0x50U == (0xffU 
                                                & ((vlTOPp->idma_controlled_pair_top__DOT__a_req[3U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_controlled_pair_top__DOT__a_req[2U] 
                                                      >> 8U)))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_read) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
            = ((1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]) 
               | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q) 
                                 << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[1U] 
            = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q) 
                      >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q 
                                                            >> 0x20U)) 
                                                   << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
            = ((2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]) 
               | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__active_cycles_q 
                                 >> 0x20U)) >> 0x1fU)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
            = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_locked_write) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
            = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
            = (1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_ctrl_q) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
            = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
            = (1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__state_q)))) {
                if ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                      >> 1U) & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_next_id_read))) {
                    if ((1U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                                >> 1U) | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__launch_cfg_ok))))) {
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
                            = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
                    } else {
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
                            = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[8U])) {
                        if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                             & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__batch_cfg_ok)))) {
                            vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
                                = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]);
                            vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U] 
                                = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
                        } else {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_a_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
                                    = ((1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U]) 
                                       | (0xfffffffeU 
                                          & ((IData)(vlTOPp->a_backend_ready_i) 
                                             << 1U)));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i 
        = (((IData)(vlTOPp->a_obs_done_o) << 1U) | (IData)(vlTOPp->v_obs_done_o));
    vlTOPp->idma_controlled_pair_top__DOT__launcher_done_id[0U] 
        = vlTOPp->v_done_id_o;
    vlTOPp->frontend_error_o = vlTOPp->idma_controlled_pair_top__DOT__fe_error;
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue) {
            vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->v_obs_issue_o = ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q)) 
                             & ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
                                       | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)))
                                 ? (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
                                     & (1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q))) 
                                    & (IData)(vlTOPp->v_backend_ready_i))
                                 : (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__issue)));
    vlTOPp->a_cpu_ready_o = (1U & (vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U] 
                                   >> 1U));
    vlTOPp->a_cpu_rdata_o = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[2U])) 
                              << 0x3fU) | (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[1U])) 
                                            << 0x1fU) 
                                           | ((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U])) 
                                              >> 1U)));
    vlTOPp->a_cpu_error_o = (1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_a_frontend__dma_ctrl_rsp_o[0U]);
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[0U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[1U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[2U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[0U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[1U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[2U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[0U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
                   [0U]);
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[1U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
                   [1U]);
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[2U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
                   [2U]);
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[3U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_q
                   [3U]);
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[0U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[1U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[2U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[0U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[1U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[2U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_count_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[0U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[1U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[2U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[0U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[1U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[2U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q;
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n 
                = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n));
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n 
                = (2U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n));
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n 
                = (4U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n));
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n 
                = (8U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n));
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[0U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[1U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[2U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[3U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[0U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[1U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[2U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[0U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[1U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[2U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_ts_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_done_id
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_done_id
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_done_id
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__launcher_done_id
        [3U];
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = ((0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                           << 1U)) | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__wr_err));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i 
        = (((IData)(vlTOPp->a_obs_issue_o) << 1U) | (IData)(vlTOPp->v_obs_issue_o));
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[0U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                    [0U];
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[1U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                    [1U];
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[2U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                    [2U];
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[3U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                    [3U];
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[0U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[1U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[2U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[0U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[1U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[2U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_id_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_read 
        = (((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
             >> 9U) & (~ (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                          >> 8U))) & (0x50U == (0xffU 
                                                & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                      >> 8U)))));
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_read) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = ((1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]) 
               | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q) 
                                 << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U] 
            = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q) 
                      >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q 
                                                            >> 0x20U)) 
                                                   << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = ((2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]) 
               | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__active_cycles_q 
                                 >> 0x20U)) >> 0x1fU)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__observer_access) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = ((1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]) 
               | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata) 
                                 << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U] 
            = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata) 
                      >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata 
                                                            >> 0x20U)) 
                                                   << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = ((2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]) 
               | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata 
                                 >> 0x20U)) >> 0x1fU)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launcher_access) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = ((1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]) 
               | (0xfffffffeU & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata) 
                                 << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U] 
            = ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata) 
                      >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                           (vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata 
                                                            >> 0x20U)) 
                                                   << 1U)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = ((2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]) 
               | (1U & ((IData)((vlTOPp->idma_controlled_pair_top__DOT__v_obs_cfg_rdata 
                                 >> 0x20U)) >> 0x1fU)));
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_locked_write) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_ctrl_q) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
            = (1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U] = 0U;
        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q) 
                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_next_id_read))) {
                    if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__launcher_cancel) 
                               | (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__launch_cfg_ok))))) {
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
                            = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
                    } else {
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
                            = (0xfffffffeU & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
                    }
                } else {
                    if ((2U & vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[8U])) {
                        if (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_stop_q) 
                             & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__batch_cfg_ok)))) {
                            vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
                                = (2U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]);
                            vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U] 
                                = (1U | vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
                        } else {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__i_v_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
                                    = ((1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U]) 
                                       | (0xfffffffeU 
                                          & ((IData)(vlTOPp->v_backend_ready_i) 
                                             << 1U)));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[0U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[1U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[2U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[0U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[1U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[2U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q;
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n 
                = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n));
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n 
                = (2U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n));
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n 
                = (4U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n));
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n 
                = (8U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n));
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[0U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
                   [0U]);
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[1U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
                   [1U]);
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[2U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
                   [2U]);
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[3U] 
                = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_q
                   [3U]);
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[0U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[1U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[2U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[0U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[1U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[2U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_count_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[0U] = 1U;
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[1U] = 1U;
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[2U] = 1U;
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[3U] = 1U;
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[0U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[1U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[2U] = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[3U] = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[0U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[1U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[2U] = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_n[3U] = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[0U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[1U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[2U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[3U] 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[0U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[1U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[2U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[0U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[1U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[2U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_ts_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[0U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
        [0U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[1U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
        [1U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[2U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
        [2U];
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[3U] 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
        [3U];
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[0U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                    [0U];
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[1U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                    [1U];
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[2U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                    [2U];
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U & (~ vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                       [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[3U] 
                    = vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                    [3U];
            }
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[0U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[1U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[2U] = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[3U] = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[0U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[1U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[2U] = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_n[3U] = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff))) {
        if ((1U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i)))) {
                if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n 
                        = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n));
                }
            }
        }
        if ((2U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                          >> 1U)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                              >> 1U)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n 
                        = (2U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n));
                }
            }
        }
        if ((4U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                          >> 2U)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                              >> 2U)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n 
                        = (4U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n));
                }
            }
        }
        if ((8U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                              >> 3U)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n 
                        = (8U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_n));
                }
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_n = 1U;
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_q)))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_n = 1U;
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_id_n 
                = vlTOPp->select_id_i;
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_did_n 
                = vlTOPp->select_did_i;
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_n = 0U;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_write_n 
                = vlTOPp->select_write_i;
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_n = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_n = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__selected_iova_n 
                = vlTOPp->select_iova_i;
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n 
                                = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n));
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n 
                                = ((0xdU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n)) 
                                   | (2U & (vlTOPp->idma_controlled_pair_top__DOT__v_req[0U] 
                                            >> 0xfU)));
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n 
                                = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL == (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n = 0U;
                                }
                            } else {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n = 0U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff))) {
        if ((1U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i)))) {
                if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n 
                        = (0xeU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n));
                }
            }
        }
        if ((2U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                          >> 1U)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                              >> 1U)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n 
                        = (0xdU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n));
                }
            }
        }
        if ((4U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                          >> 2U)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                              >> 2U)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n 
                        = (0xbU & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n));
                }
            }
        }
        if ((8U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q) 
                              >> 3U)))) {
                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n 
                        = (7U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n));
                }
            }
        }
    }
    if (((2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q)))) {
        if ((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)) 
               & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q) 
                  == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))) 
              & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held) 
                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) 
             & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n = 0U;
        }
    }
    if (((2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q))) {
        if (((((0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))) 
               & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active) 
                         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) 
              & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                        & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))))) 
             & (0U == ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) 
                        & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q))) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_n = 0U;
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_q;
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU == (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            if ((0xb0U != (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_n = 0ULL;
                        }
                    } else {
                        if ((2ULL != (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((4ULL != (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_n = 0ULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_valid_q)))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_ts_n 
                    = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q;
            }
        }
    }
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n = 0U;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_q;
    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n 
        = vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q;
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((0xffffffffffffffffULL == vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q)) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x2000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
    }
    if ((((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
         & (0U != ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_error) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))))) {
        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
            = (0x20U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
    }
    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
        if ((0U != ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                    & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x40U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
        if ((0U != ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                    & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x80U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x40U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x800U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((0ULL == vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                 [0U])) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x200U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x80U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((1U & ((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                       & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i))))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x100U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x1000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                 [0U] & (vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                         [0U] != vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
                         [0U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x400U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((1U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x40U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x800U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((0ULL == vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                 [1U])) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x200U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x80U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((1U & ((~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                           >> 1U)) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                         >> 1U))))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x100U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x1000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                 [1U] & (vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                         [1U] != vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
                         [1U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x400U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((2U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x40U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x800U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((0ULL == vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                 [2U])) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x200U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x80U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((1U & ((~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                           >> 2U)) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                         >> 2U))))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x100U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x1000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                 [2U] & (vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                         [2U] != vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
                         [2U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x400U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((4U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x40U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((8U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x800U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((0ULL == vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_id_i
                 [3U])) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x200U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
            if ((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x80U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((1U & ((~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                           >> 3U)) & (~ ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                         >> 3U))))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x100U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((8U & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x1000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
            if ((vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_valid_q
                 [3U] & (vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_done_id_i
                         [3U] != vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issue_id_q
                         [3U]))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x400U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((8U & ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        }
    }
    if (((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__cfg_valid_i) 
         & (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
            >> 8U))) {
        if ((0xffU != (0xffU & vlTOPp->idma_controlled_pair_top__DOT__v_req[0U]))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                = (1U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
        } else {
            if ((0xb0U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                     << 0x18U) | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                  >> 8U))))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x10000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            } else {
                if ((0xb8U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                         << 0x18U) 
                                        | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                           >> 8U))))) {
                    if ((1ULL == (0xfffffffffffffeffULL 
                                  & (((QData)((IData)(
                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                      << 0x38U) | (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                      >> 8U)))))) {
                        if (((((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                               | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
                              & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active))) 
                             & (0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held)))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 1U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_n = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n = 0ULL;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_n = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_n = 0U;
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n = 0U;
                        } else {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                                = (4U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
                        }
                    } else {
                        if ((2ULL == (((QData)((IData)(
                                                       vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                       << 0x38U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                        << 0x18U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                        >> 8U))))) {
                            if ((((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                  & (0U == vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n)) 
                                 & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held) 
                                     & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)) 
                                    == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) {
                                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 2U;
                                if (vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n 
                                        = (2U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n));
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_n = 1U;
                                    if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_n 
                                            = (1ULL 
                                               + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q);
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_n = 1U;
                                    }
                                } else {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n 
                                        = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n));
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n = 1U;
                                    if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n 
                                            = (1ULL 
                                               + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q);
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n = 1U;
                                    }
                                }
                            } else {
                                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                                    = (8U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
                            }
                        } else {
                            if ((4ULL == (((QData)((IData)(
                                                           vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                >> 8U))))) {
                                if ((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 3U;
                                } else {
                                    if (((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 2U;
                                    } else {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                                            = (0x10U 
                                               | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
                                    }
                                }
                            } else {
                                if ((8ULL == (((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                                    >> 8U))))) {
                                    if (((0U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
                                         | (3U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)))) {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_n = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n = 0ULL;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_a_ts_valid_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_go_sent_n = 0U;
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n = 0U;
                                    } else {
                                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                                            = (2U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
                                    }
                                } else {
                                    vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                                        = (2U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
                                }
                            }
                        }
                    }
                } else {
                    if ((0xc0U == (0xffU & ((vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                             << 0x18U) 
                                            | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                               >> 8U))))) {
                        if ((0x20ULL <= (((QData)((IData)(
                                                          vlTOPp->idma_controlled_pair_top__DOT__v_req[2U])) 
                                          << 0x38U) 
                                         | (((QData)((IData)(
                                                             vlTOPp->idma_controlled_pair_top__DOT__v_req[1U])) 
                                             << 0x18U) 
                                            | ((QData)((IData)(
                                                               vlTOPp->idma_controlled_pair_top__DOT__v_req[0U])) 
                                               >> 8U))))) {
                            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                                = (0x4000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
                        }
                    } else {
                        vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                            = ((0xc8U == (0xffU & (
                                                   (vlTOPp->idma_controlled_pair_top__DOT__v_req[3U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_controlled_pair_top__DOT__v_req[2U] 
                                                      >> 8U))))
                                ? (0x8000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n)
                                : (2U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n));
                    }
                }
            }
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff)) 
            & (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
           & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__has_a_q)) 
          & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__a_select_seen_q))) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_q)))) {
        if ((((((IData)(vlTOPp->selected_i) & (~ (IData)(vlTOPp->select_write_i))) 
               & (4U == (IData)(vlTOPp->select_id_i))) 
              & (0xbU == vlTOPp->select_did_i)) & (
                                                   ((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i) 
                                                    | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q)) 
                                                   >> 1U))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n 
                = (1U | (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_pulse_n));
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__v_go_sent_n = 1U;
            if ((1U & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_q)))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_n 
                    = (1ULL + vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__ticks_q);
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__go_v_ts_valid_n = 1U;
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
          | (2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q))) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__abort_eff))) {
        if ((1U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((1U & (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((2U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((2U & (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((4U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((4U & (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
        if ((8U & (((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                    & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__hold_mask_q)) 
                   & (IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_cancelled)))) {
            if ((8U & (IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellinp__i_launcher__obs_issue_i))) {
                vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n 
                    = (0x20000U | vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__error_n);
            }
        }
    }
    if (((2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q)))) {
        if ((((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)) 
               & ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q) 
                  == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))) 
              & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held) 
                        & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) 
             & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 3U;
        }
    }
    if (((2U == (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_q)) 
         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__aborted_q))) {
        if (((((0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_held) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q))) 
               & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__fe_active) 
                         & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q)))) 
              & (0U == ((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q) 
                        & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__done_mask_q))))) 
             & (0U == ((((IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__expected_mask_q) 
                         & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__issued_mask_q))) 
                        & (~ (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__cancelled_mask_q))) 
                       & (IData)(vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__seen_held_q))))) {
            vlTOPp->idma_controlled_pair_top__DOT__i_launcher__DOT__state_n = 3U;
        }
    }
    vlTOPp->v_cpu_ready_o = (1U & (vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U] 
                                   >> 1U));
    vlTOPp->v_cpu_rdata_o = (((QData)((IData)(vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[2U])) 
                              << 0x3fU) | (((QData)((IData)(
                                                            vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[1U])) 
                                            << 0x1fU) 
                                           | ((QData)((IData)(
                                                              vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U])) 
                                              >> 1U)));
    vlTOPp->v_cpu_error_o = (1U & vlTOPp->idma_controlled_pair_top__DOT____Vcellout__i_v_frontend__dma_ctrl_rsp_o[0U]);
}

void Vidma_controlled_pair_top::_eval(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_controlled_pair_top::_eval\n"); );
    Vidma_controlled_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}
