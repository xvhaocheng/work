// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Ve3_observer_top.h for the primary calling header

#include "Ve3_observer_top.h"
#include "Ve3_observer_top__Syms.h"

void Ve3_observer_top::_eval(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_eval\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Ve3_observer_top::_change_request(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_change_request\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Ve3_observer_top::_change_request_1(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_change_request_1\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Ve3_observer_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((arm_i & 0xfeU))) {
        Verilated::overWidthError("arm_i");}
    if (VL_UNLIKELY((clear_i & 0xfeU))) {
        Verilated::overWidthError("clear_i");}
    if (VL_UNLIKELY((active_i & 0xfeU))) {
        Verilated::overWidthError("active_i");}
    if (VL_UNLIKELY((open_i & 0xfeU))) {
        Verilated::overWidthError("open_i");}
    if (VL_UNLIKELY((close_i & 0xfeU))) {
        Verilated::overWidthError("close_i");}
    if (VL_UNLIKELY((abort_i & 0xfeU))) {
        Verilated::overWidthError("abort_i");}
    if (VL_UNLIKELY((delay128_i & 0xfeU))) {
        Verilated::overWidthError("delay128_i");}
    if (VL_UNLIKELY((select_i & 0xfeU))) {
        Verilated::overWidthError("select_i");}
    if (VL_UNLIKELY((write_i & 0xfeU))) {
        Verilated::overWidthError("write_i");}
    if (VL_UNLIKELY((request_i & 0xfeU))) {
        Verilated::overWidthError("request_i");}
    if (VL_UNLIKELY((response_i & 0xfeU))) {
        Verilated::overWidthError("response_i");}
    if (VL_UNLIKELY((lookup_i & 0xfeU))) {
        Verilated::overWidthError("lookup_i");}
    if (VL_UNLIKELY((hit_i & 0xfeU))) {
        Verilated::overWidthError("hit_i");}
    if (VL_UNLIKELY((capture_i & 0xfeU))) {
        Verilated::overWidthError("capture_i");}
    if (VL_UNLIKELY((available_i & 0xfeU))) {
        Verilated::overWidthError("available_i");}
    if (VL_UNLIKELY((consume_i & 0xfeU))) {
        Verilated::overWidthError("consume_i");}
    if (VL_UNLIKELY((id_i & 0xf0U))) {
        Verilated::overWidthError("id_i");}
    if (VL_UNLIKELY((tr_fire_i & 0xfcU))) {
        Verilated::overWidthError("tr_fire_i");}
    if (VL_UNLIKELY((out_valid_i & 0xfcU))) {
        Verilated::overWidthError("out_valid_i");}
    if (VL_UNLIKELY((out_ready_i & 0xfcU))) {
        Verilated::overWidthError("out_ready_i");}
    if (VL_UNLIKELY((return_i & 0xfcU))) {
        Verilated::overWidthError("return_i");}
    if (VL_UNLIKELY((wlast_i & 0xfeU))) {
        Verilated::overWidthError("wlast_i");}
}
#endif  // VL_DEBUG
