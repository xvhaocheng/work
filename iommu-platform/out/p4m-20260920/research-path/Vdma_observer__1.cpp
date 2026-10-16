// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_observer.h for the primary calling header

#include "Vdma_observer.h"
#include "Vdma_observer__Syms.h"

void Vdma_observer::_eval(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_eval\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__3(vlSymsp);
        vlTOPp->_sequent__TOP__4(vlSymsp);
    }
    vlTOPp->_combo__TOP__5(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vdma_observer::_change_request(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_change_request\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_observer::_change_request_1(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_change_request_1\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_observer::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((cfg_valid_i & 0xfeU))) {
        Verilated::overWidthError("cfg_valid_i");}
    if (VL_UNLIKELY((cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("cfg_write_i");}
    if (VL_UNLIKELY((cfg_addr_i & 0x80U))) {
        Verilated::overWidthError("cfg_addr_i");}
    if (VL_UNLIKELY((issue_i & 0xf0U))) {
        Verilated::overWidthError("issue_i");}
    if (VL_UNLIKELY((done_i & 0xf0U))) {
        Verilated::overWidthError("done_i");}
    if (VL_UNLIKELY((poll_i & 0xf0U))) {
        Verilated::overWidthError("poll_i");}
    if (VL_UNLIKELY((batch_active_i & 0xf0U))) {
        Verilated::overWidthError("batch_active_i");}
    if (VL_UNLIKELY((ar_valid_i & 0xf0U))) {
        Verilated::overWidthError("ar_valid_i");}
    if (VL_UNLIKELY((aw_valid_i & 0xf0U))) {
        Verilated::overWidthError("aw_valid_i");}
    if (VL_UNLIKELY((ar_ready_i & 0xf0U))) {
        Verilated::overWidthError("ar_ready_i");}
    if (VL_UNLIKELY((aw_ready_i & 0xf0U))) {
        Verilated::overWidthError("aw_ready_i");}
    if (VL_UNLIKELY((rlast_fire_i & 0xf0U))) {
        Verilated::overWidthError("rlast_fire_i");}
    if (VL_UNLIKELY((b_fire_i & 0xf0U))) {
        Verilated::overWidthError("b_fire_i");}
    if (VL_UNLIKELY((bad_id_i & 0xf0U))) {
        Verilated::overWidthError("bad_id_i");}
    if (VL_UNLIKELY((shared_ar_valid_i & 0xfeU))) {
        Verilated::overWidthError("shared_ar_valid_i");}
    if (VL_UNLIKELY((shared_aw_valid_i & 0xfeU))) {
        Verilated::overWidthError("shared_aw_valid_i");}
    if (VL_UNLIKELY((shared_ar_ready_i & 0xfeU))) {
        Verilated::overWidthError("shared_ar_ready_i");}
    if (VL_UNLIKELY((shared_aw_ready_i & 0xfeU))) {
        Verilated::overWidthError("shared_aw_ready_i");}
    if (VL_UNLIKELY((out_valid_i & 0xfcU))) {
        Verilated::overWidthError("out_valid_i");}
    if (VL_UNLIKELY((out_ready_i & 0xfcU))) {
        Verilated::overWidthError("out_ready_i");}
    if (VL_UNLIKELY((return_i & 0xfcU))) {
        Verilated::overWidthError("return_i");}
    if (VL_UNLIKELY((e3_error_i & 0xfeU))) {
        Verilated::overWidthError("e3_error_i");}
}
#endif  // VL_DEBUG
