// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Ve3_delay_top.h for the primary calling header

#include "Ve3_delay_top.h"
#include "Ve3_delay_top__Syms.h"

//==========

VL_CTOR_IMP(Ve3_delay_top) {
    Ve3_delay_top__Syms* __restrict vlSymsp = __VlSymsp = new Ve3_delay_top__Syms(this, name());
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Ve3_delay_top::__Vconfigure(Ve3_delay_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Ve3_delay_top::~Ve3_delay_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Ve3_delay_top::_settle__TOP__2(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_settle__TOP__2\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->e3_delay_top__DOT__mem_resp[0U] = ((0xffffffc0U 
                                                & ((IData)(vlTOPp->in_data_i) 
                                                   << 6U)) 
                                               | (((IData)(vlTOPp->in_id_i) 
                                                   << 2U) 
                                                  | (((IData)(vlTOPp->in_last_i) 
                                                      << 1U) 
                                                     | (IData)(vlTOPp->in_valid_i))));
    vlTOPp->e3_delay_top__DOT__mem_resp[1U] = ((0x3fU 
                                                & ((IData)(vlTOPp->in_data_i) 
                                                   >> 0x1aU)) 
                                               | (0xffffffc0U 
                                                  & ((IData)(
                                                             (vlTOPp->in_data_i 
                                                              >> 0x20U)) 
                                                     << 6U)));
    vlTOPp->e3_delay_top__DOT__mem_resp[2U] = (0x3fU 
                                               & ((IData)(
                                                          (vlTOPp->in_data_i 
                                                           >> 0x20U)) 
                                                  >> 0x1aU));
    vlTOPp->extra_o = vlTOPp->e3_delay_top__DOT__dut__DOT__held_q;
    vlTOPp->available_o = ((((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q) 
                             & (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__selected_q)) 
                            & (0U == (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q))) 
                           & (~ (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__announced_q)));
    vlTOPp->e3_delay_top__DOT__dut__DOT__give = (((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q) 
                                                  & (0U 
                                                     == (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q))) 
                                                 & (IData)(vlTOPp->out_ready_i));
    vlTOPp->consume_o = ((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__give) 
                         & (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__selected_q));
    vlTOPp->e3_delay_top__DOT____Vcellout__dut__memory_req_o 
        = vlTOPp->out_ready_i;
    if (((IData)(vlTOPp->enable_i) | (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q))) {
        vlTOPp->e3_delay_top__DOT____Vcellout__dut__memory_req_o 
            = (1U & (~ (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q)));
    }
    vlTOPp->e3_delay_top__DOT__dut__DOT__take = ((((IData)(vlTOPp->enable_i) 
                                                   | (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q)) 
                                                  & vlTOPp->e3_delay_top__DOT__mem_resp[0U]) 
                                                 & (IData)(vlTOPp->e3_delay_top__DOT____Vcellout__dut__memory_req_o));
    vlTOPp->capture_o = (((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__take) 
                          & (IData)(vlTOPp->eligible_i)) 
                         & (~ (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__used_q)));
    vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U] 
        = vlTOPp->e3_delay_top__DOT__mem_resp[0U];
    vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[1U] 
        = vlTOPp->e3_delay_top__DOT__mem_resp[1U];
    vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[2U] 
        = vlTOPp->e3_delay_top__DOT__mem_resp[2U];
    if (((IData)(vlTOPp->enable_i) | (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q))) {
        vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U] 
            = ((1U & vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U]) 
               | (0xfffffffeU & vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[0U]));
        vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[1U] 
            = ((1U & vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[1U]) 
               | (0xfffffffeU & vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[1U]));
        vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[2U] 
            = ((1U & vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[2U]) 
               | (0x3eU & vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[2U]));
        vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U] 
            = ((0xfffffffeU & vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U]) 
               | ((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q) 
                  & (0U == (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q))));
    }
    vlTOPp->in_ready_o = vlTOPp->e3_delay_top__DOT____Vcellout__dut__memory_req_o;
    vlTOPp->out_valid_o = (1U & vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U]);
    vlTOPp->out_data_o = (((QData)((IData)(vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[2U])) 
                           << 0x3aU) | (((QData)((IData)(
                                                         vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[1U])) 
                                         << 0x1aU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U])) 
                                           >> 6U)));
    vlTOPp->out_id_o = (0xfU & ((vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[1U] 
                                 << 0x1eU) | (vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U] 
                                              >> 2U)));
    vlTOPp->out_last_o = (1U & (vlTOPp->e3_delay_top__DOT____Vcellout__dut__walker_resp_o[0U] 
                                >> 1U));
}

void Ve3_delay_top::_eval_initial(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_eval_initial\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Ve3_delay_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::final\n"); );
    // Variables
    Ve3_delay_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Ve3_delay_top::_eval_settle(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_eval_settle\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Ve3_delay_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    enable_i = VL_RAND_RESET_I(1);
    arm_i = VL_RAND_RESET_I(1);
    eligible_i = VL_RAND_RESET_I(1);
    delay128_i = VL_RAND_RESET_I(1);
    in_valid_i = VL_RAND_RESET_I(1);
    out_ready_i = VL_RAND_RESET_I(1);
    in_last_i = VL_RAND_RESET_I(1);
    in_data_i = VL_RAND_RESET_Q(64);
    in_id_i = VL_RAND_RESET_I(4);
    in_ready_o = VL_RAND_RESET_I(1);
    out_valid_o = VL_RAND_RESET_I(1);
    out_last_o = VL_RAND_RESET_I(1);
    out_data_o = VL_RAND_RESET_Q(64);
    out_id_o = VL_RAND_RESET_I(4);
    capture_o = VL_RAND_RESET_I(1);
    available_o = VL_RAND_RESET_I(1);
    consume_o = VL_RAND_RESET_I(1);
    error_o = VL_RAND_RESET_I(1);
    extra_o = VL_RAND_RESET_I(8);
    VL_RAND_RESET_W(70, e3_delay_top__DOT__mem_resp);
    e3_delay_top__DOT____Vcellout__dut__memory_req_o = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(70, e3_delay_top__DOT____Vcellout__dut__walker_resp_o);
    VL_RAND_RESET_W(70, e3_delay_top__DOT__dut__DOT__saved_q);
    e3_delay_top__DOT__dut__DOT__full_q = VL_RAND_RESET_I(1);
    e3_delay_top__DOT__dut__DOT__selected_q = VL_RAND_RESET_I(1);
    e3_delay_top__DOT__dut__DOT__used_q = VL_RAND_RESET_I(1);
    e3_delay_top__DOT__dut__DOT__announced_q = VL_RAND_RESET_I(1);
    e3_delay_top__DOT__dut__DOT__remaining_q = VL_RAND_RESET_I(8);
    e3_delay_top__DOT__dut__DOT__held_q = VL_RAND_RESET_I(8);
    e3_delay_top__DOT__dut__DOT__take = VL_RAND_RESET_I(1);
    e3_delay_top__DOT__dut__DOT__give = VL_RAND_RESET_I(1);
}
