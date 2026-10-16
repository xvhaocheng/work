// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_controlled_pair_top.h for the primary calling header

#include "Vidma_controlled_pair_top.h"
#include "Vidma_controlled_pair_top__Syms.h"

VL_INLINE_OPT QData Vidma_controlled_pair_top::_change_request(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_controlled_pair_top::_change_request\n"); );
    Vidma_controlled_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vidma_controlled_pair_top::_change_request_1(Vidma_controlled_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_controlled_pair_top::_change_request_1\n"); );
    Vidma_controlled_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vidma_controlled_pair_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_controlled_pair_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((v_cpu_valid_i & 0xfeU))) {
        Verilated::overWidthError("v_cpu_valid_i");}
    if (VL_UNLIKELY((v_cpu_write_i & 0xfeU))) {
        Verilated::overWidthError("v_cpu_write_i");}
    if (VL_UNLIKELY((a_cpu_valid_i & 0xfeU))) {
        Verilated::overWidthError("a_cpu_valid_i");}
    if (VL_UNLIKELY((a_cpu_write_i & 0xfeU))) {
        Verilated::overWidthError("a_cpu_write_i");}
    if (VL_UNLIKELY((v_backend_ready_i & 0xfeU))) {
        Verilated::overWidthError("v_backend_ready_i");}
    if (VL_UNLIKELY((v_backend_idle_i & 0xfeU))) {
        Verilated::overWidthError("v_backend_idle_i");}
    if (VL_UNLIKELY((v_trans_complete_i & 0xfeU))) {
        Verilated::overWidthError("v_trans_complete_i");}
    if (VL_UNLIKELY((a_backend_ready_i & 0xfeU))) {
        Verilated::overWidthError("a_backend_ready_i");}
    if (VL_UNLIKELY((a_backend_idle_i & 0xfeU))) {
        Verilated::overWidthError("a_backend_idle_i");}
    if (VL_UNLIKELY((a_trans_complete_i & 0xfeU))) {
        Verilated::overWidthError("a_trans_complete_i");}
    if (VL_UNLIKELY((selected_i & 0xfeU))) {
        Verilated::overWidthError("selected_i");}
    if (VL_UNLIKELY((select_write_i & 0xfeU))) {
        Verilated::overWidthError("select_write_i");}
    if (VL_UNLIKELY((select_did_i & 0xff000000U))) {
        Verilated::overWidthError("select_did_i");}
}
#endif  // VL_DEBUG
