// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpath_stage_test_top.h for the primary calling header

#include "Vpath_stage_test_top.h"
#include "Vpath_stage_test_top__Syms.h"

//==========

VL_CTOR_IMP(Vpath_stage_test_top) {
    Vpath_stage_test_top__Syms* __restrict vlSymsp = __VlSymsp = new Vpath_stage_test_top__Syms(this, name());
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vpath_stage_test_top::__Vconfigure(Vpath_stage_test_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vpath_stage_test_top::~Vpath_stage_test_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vpath_stage_test_top::_settle__TOP__2(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_settle__TOP__2\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_next 
        = (0x3ffU & ((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_q) 
                     + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q)));
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_next 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q;
    if (((IData)(vlTOPp->enter_i) & (~ (IData)(vlTOPp->leave_i)))) {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q)));
    }
    if (((IData)(vlTOPp->leave_i) & (~ (IData)(vlTOPp->enter_i)))) {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->path_stage_test_top__DOT__i_stage__DOT__born_in_window
           [vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q]);
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched 
        = ((IData)(vlTOPp->leave_i) & ((0U != (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q)) 
                                       | (IData)(vlTOPp->enter_i)));
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad 
        = (((IData)(vlTOPp->leave_i) & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched))) 
           | (((IData)(vlTOPp->enter_i) & (4U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q))) 
              & (~ (IData)(vlTOPp->leave_i))));
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q))
            ? 0U : (0xffU & ((IData)(vlTOPp->time_i) 
                             - vlTOPp->path_stage_test_top__DOT__i_stage__DOT__stamps
                             [vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q])));
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_next 
        = (0x3ffU & ((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_q) 
                     + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age)));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__entered_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__full_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__carry_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[3U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__max_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__opening_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__closing_q));
    vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o[7U] 
        = (QData)((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_q));
    vlTOPp->data_o[0U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [0U];
    vlTOPp->data_o[1U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [1U];
    vlTOPp->data_o[2U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [2U];
    vlTOPp->data_o[3U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [3U];
    vlTOPp->data_o[4U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [4U];
    vlTOPp->data_o[5U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [5U];
    vlTOPp->data_o[6U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [6U];
    vlTOPp->data_o[7U] = vlTOPp->path_stage_test_top__DOT____Vcellout__i_stage__data_o
        [7U];
}

void Vpath_stage_test_top::_eval_initial(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_eval_initial\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vpath_stage_test_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::final\n"); );
    // Variables
    Vpath_stage_test_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vpath_stage_test_top::_eval_settle(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_eval_settle\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vpath_stage_test_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    clear_i = VL_RAND_RESET_I(1);
    run_i = VL_RAND_RESET_I(1);
    active_i = VL_RAND_RESET_I(1);
    enter_i = VL_RAND_RESET_I(1);
    leave_i = VL_RAND_RESET_I(1);
    time_i = VL_RAND_RESET_I(8);
    error_o = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            path_stage_test_top__DOT____Vcellout__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            path_stage_test_top__DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            path_stage_test_top__DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    path_stage_test_top__DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    path_stage_test_top__DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    path_stage_test_top__DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__sum_q = VL_RAND_RESET_I(9);
    path_stage_test_top__DOT__i_stage__DOT__area_q = VL_RAND_RESET_I(9);
    path_stage_test_top__DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(8);
    path_stage_test_top__DOT__i_stage__DOT__age = VL_RAND_RESET_I(8);
    path_stage_test_top__DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    path_stage_test_top__DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    path_stage_test_top__DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    path_stage_test_top__DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    path_stage_test_top__DOT__i_stage__DOT__sum_next = VL_RAND_RESET_I(10);
    path_stage_test_top__DOT__i_stage__DOT__area_next = VL_RAND_RESET_I(10);
}
