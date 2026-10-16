// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_observer.h for the primary calling header

#include "Vdma_research_observer.h"
#include "Vdma_research_observer__Syms.h"

void Vdma_research_observer::_eval(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_eval\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__3(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vdma_research_observer::_change_request(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_change_request\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_research_observer::_change_request_1(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_change_request_1\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_research_observer::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((cfg_valid_i & 0xfeU))) {
        Verilated::overWidthError("cfg_valid_i");}
    if (VL_UNLIKELY((cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("cfg_write_i");}
    if (VL_UNLIKELY((index_i & 0xe000U))) {
        Verilated::overWidthError("index_i");}
    if (VL_UNLIKELY((quiet_i & 0xfeU))) {
        Verilated::overWidthError("quiet_i");}
    if (VL_UNLIKELY((supported_i & 0xfeU))) {
        Verilated::overWidthError("supported_i");}
    if (VL_UNLIKELY((update_i & 0xfeU))) {
        Verilated::overWidthError("update_i");}
    if (VL_UNLIKELY((invalidate_i & 0xfeU))) {
        Verilated::overWidthError("invalidate_i");}
    if (VL_UNLIKELY((update_did_i & 0xff000000U))) {
        Verilated::overWidthError("update_did_i");}
    if (VL_UNLIKELY((lookup_i & 0xfeU))) {
        Verilated::overWidthError("lookup_i");}
    if (VL_UNLIKELY((lookup_hit_i & 0xfeU))) {
        Verilated::overWidthError("lookup_hit_i");}
    if (VL_UNLIKELY((lookup_did_i & 0xff000000U))) {
        Verilated::overWidthError("lookup_did_i");}
    if (VL_UNLIKELY((lookup_pscid_i & 0xfff00000U))) {
        Verilated::overWidthError("lookup_pscid_i");}
    if (VL_UNLIKELY((lookup_stage1_i & 0xfeU))) {
        Verilated::overWidthError("lookup_stage1_i");}
    if (VL_UNLIKELY((lookup_stage2_i & 0xfeU))) {
        Verilated::overWidthError("lookup_stage2_i");}
    if (VL_UNLIKELY((ctl_state_i & 0xf8U))) {
        Verilated::overWidthError("ctl_state_i");}
    if (VL_UNLIKELY((ctl_did_i & 0xff000000U))) {
        Verilated::overWidthError("ctl_did_i");}
    if (VL_UNLIKELY((select_fire_i & 0xfeU))) {
        Verilated::overWidthError("select_fire_i");}
    if (VL_UNLIKELY((select_did_i & 0xff000000U))) {
        Verilated::overWidthError("select_did_i");}
    if (VL_UNLIKELY((response_error_i & 0xfeU))) {
        Verilated::overWidthError("response_error_i");}
    if (VL_UNLIKELY((issue_i & 0xfcU))) {
        Verilated::overWidthError("issue_i");}
    if (VL_UNLIKELY((done_i & 0xfcU))) {
        Verilated::overWidthError("done_i");}
    if (VL_UNLIKELY((p4m_arm_i & 0xfeU))) {
        Verilated::overWidthError("p4m_arm_i");}
    if (VL_UNLIKELY((p4m_open_i & 0xfeU))) {
        Verilated::overWidthError("p4m_open_i");}
    if (VL_UNLIKELY((p4m_close_i & 0xfeU))) {
        Verilated::overWidthError("p4m_close_i");}
}
#endif  // VL_DEBUG
