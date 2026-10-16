// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_e3_write_observer.h for the primary calling header

#include "Vdma_e3_write_observer.h"
#include "Vdma_e3_write_observer__Syms.h"

//==========

VL_CTOR_IMP(Vdma_e3_write_observer) {
    Vdma_e3_write_observer__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_e3_write_observer__Syms(this, name());
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_e3_write_observer::__Vconfigure(Vdma_e3_write_observer__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_e3_write_observer::~Vdma_e3_write_observer() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_e3_write_observer::_settle__TOP__2(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_settle__TOP__2\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->data_o[0U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0U];
    vlTOPp->data_o[1U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [1U];
    vlTOPp->data_o[2U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [2U];
    vlTOPp->data_o[3U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [3U];
    vlTOPp->data_o[4U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [4U];
    vlTOPp->data_o[5U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [5U];
    vlTOPp->data_o[6U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [6U];
    vlTOPp->data_o[7U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [7U];
    vlTOPp->data_o[8U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [8U];
    vlTOPp->data_o[9U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [9U];
    vlTOPp->data_o[0xaU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xaU];
    vlTOPp->data_o[0xbU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xbU];
    vlTOPp->data_o[0xcU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xcU];
    vlTOPp->data_o[0xdU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xdU];
    vlTOPp->data_o[0xeU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xeU];
    vlTOPp->data_o[0xfU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xfU];
    vlTOPp->data_o[0x10U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x10U];
    vlTOPp->data_o[0x11U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x11U];
    vlTOPp->data_o[0x12U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x12U];
    vlTOPp->data_o[0x13U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x13U];
    vlTOPp->data_o[0x14U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x14U];
    vlTOPp->data_o[0x15U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x15U];
    vlTOPp->data_o[0x16U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x16U];
    vlTOPp->data_o[0x17U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x17U];
    vlTOPp->data_o[0x18U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x18U];
    vlTOPp->data_o[0x19U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x19U];
    vlTOPp->data_o[0x1aU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1aU];
    vlTOPp->data_o[0x1bU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1bU];
    vlTOPp->data_o[0x1cU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1cU];
    vlTOPp->data_o[0x1dU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1dU];
    vlTOPp->data_o[0x1eU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1eU];
    vlTOPp->data_o[0x1fU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1fU];
    vlTOPp->dma_e3_write_observer__DOT__w_tail_n = vlTOPp->dma_e3_write_observer__DOT__w_tail_q;
    if (vlTOPp->wlast_i) {
        vlTOPp->dma_e3_write_observer__DOT__w_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->dma_e3_write_observer__DOT__w_tail_q)));
    }
    vlTOPp->dma_e3_write_observer__DOT__aw_tail_n = vlTOPp->dma_e3_write_observer__DOT__aw_tail_q;
    if (vlTOPp->aw_i) {
        vlTOPp->dma_e3_write_observer__DOT__aw_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->dma_e3_write_observer__DOT__aw_tail_q)));
    }
    vlTOPp->dma_e3_write_observer__DOT__bad = vlTOPp->error_o;
    vlTOPp->dma_e3_write_observer__DOT__role = 0U;
    vlTOPp->dma_e3_write_observer__DOT__slot = 0U;
    vlTOPp->dma_e3_write_observer__DOT__base = 0U;
    vlTOPp->dma_e3_write_observer__DOT__aw_w = 0ULL;
    vlTOPp->dma_e3_write_observer__DOT__w_b = 0ULL;
    vlTOPp->dma_e3_write_observer__DOT__aw_b = 0ULL;
    vlTOPp->dma_e3_write_observer__DOT__present_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [0U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[0U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [0U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[0U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [0U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[0U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [1U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[1U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [1U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[1U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [1U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[1U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [2U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[2U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [2U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[2U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [2U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[2U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [3U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[3U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [3U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[3U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [3U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[3U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [4U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[4U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [4U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[4U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [4U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[4U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [5U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[5U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [5U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[5U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [5U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[5U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [6U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[6U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [6U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[6U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [6U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[6U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [7U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[7U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [7U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[7U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [7U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[7U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__head_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__head_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__tail_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__tail_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__count_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__count_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][0U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][1U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][2U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][3U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][3U];
    vlTOPp->dma_e3_write_observer__DOT__head_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__head_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__tail_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__tail_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__count_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__count_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][0U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][1U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][2U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][3U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][3U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[8U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [8U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[9U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [9U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xaU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xaU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xbU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xbU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xcU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xdU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xdU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xeU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xeU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xfU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xfU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x10U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x10U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x11U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x11U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x12U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x12U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x13U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x13U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x14U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x14U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x15U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x15U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x16U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x16U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x17U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x17U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x18U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x18U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x19U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x19U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1aU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1aU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1bU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1bU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1cU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1cU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1dU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1dU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1eU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1eU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1fU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1fU];
    if (vlTOPp->aw_i) {
        vlTOPp->dma_e3_write_observer__DOT__role = 
            (4U == (IData)(vlTOPp->aw_id_i));
        vlTOPp->dma_e3_write_observer__DOT__slot = vlTOPp->dma_e3_write_observer__DOT__aw_tail_q;
        if ((1U & ((((0U != (IData)(vlTOPp->aw_id_i)) 
                     & (4U != (IData)(vlTOPp->aw_id_i))) 
                    | vlTOPp->dma_e3_write_observer__DOT__present_n
                    [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
                   | (4U == vlTOPp->dma_e3_write_observer__DOT__count_q
                      [(1U & vlTOPp->dma_e3_write_observer__DOT__role)])))) {
            vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[(7U 
                                                       & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (1U | vlTOPp->dma_e3_write_observer__DOT__present_n
               [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]);
        vlTOPp->dma_e3_write_observer__DOT__inside_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = ((6U & vlTOPp->dma_e3_write_observer__DOT__inside_n
                [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
               | (IData)(vlTOPp->active_i));
        vlTOPp->dma_e3_write_observer__DOT__stamps_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)][0U] 
            = vlTOPp->time_i;
        vlTOPp->dma_e3_write_observer__DOT__role_n[(7U 
                                                    & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (1U & vlTOPp->dma_e3_write_observer__DOT__role);
        vlTOPp->dma_e3_write_observer__DOT__order_n[(1U 
                                                     & vlTOPp->dma_e3_write_observer__DOT__role)][vlTOPp->dma_e3_write_observer__DOT__tail_q[(1U 
                                                                                & vlTOPp->dma_e3_write_observer__DOT__role)]] 
            = vlTOPp->dma_e3_write_observer__DOT__aw_tail_q;
        vlTOPp->dma_e3_write_observer__DOT__tail_n[(1U 
                                                    & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->dma_e3_write_observer__DOT__tail_q
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]));
        vlTOPp->dma_e3_write_observer__DOT__count_n[(1U 
                                                     & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (7U & ((IData)(1U) + vlTOPp->dma_e3_write_observer__DOT__count_n
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]));
        if (vlTOPp->active_i) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                        & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role))] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_q
                   [(0x1fU & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role))]);
        }
    }
    if (vlTOPp->wlast_i) {
        vlTOPp->dma_e3_write_observer__DOT__slot = vlTOPp->dma_e3_write_observer__DOT__w_tail_q;
        if ((2U & vlTOPp->dma_e3_write_observer__DOT__present_n
             [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)])) {
            vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[(7U 
                                                       & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (2U | vlTOPp->dma_e3_write_observer__DOT__present_n
               [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]);
        vlTOPp->dma_e3_write_observer__DOT__inside_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = ((5U & vlTOPp->dma_e3_write_observer__DOT__inside_n
                [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 1U));
        vlTOPp->dma_e3_write_observer__DOT__stamps_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)][1U] 
            = vlTOPp->time_i;
    }
    if (vlTOPp->b_i) {
        vlTOPp->dma_e3_write_observer__DOT__role = 
            (4U == (IData)(vlTOPp->b_id_i));
        vlTOPp->dma_e3_write_observer__DOT__slot = 
            vlTOPp->dma_e3_write_observer__DOT__order_n
            [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]
            [vlTOPp->dma_e3_write_observer__DOT__head_q
            [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]];
        if ((1U & ((((((0U != (IData)(vlTOPp->b_id_i)) 
                       & (4U != (IData)(vlTOPp->b_id_i))) 
                      | (0U == vlTOPp->dma_e3_write_observer__DOT__count_n
                         [(1U & vlTOPp->dma_e3_write_observer__DOT__role)])) 
                     | (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                        [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)])) 
                    | (~ (vlTOPp->dma_e3_write_observer__DOT__present_n
                          [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)] 
                          >> 1U))) | (vlTOPp->dma_e3_write_observer__DOT__present_n
                                      [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)] 
                                      >> 2U)))) {
            vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[(7U 
                                                       & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (4U | vlTOPp->dma_e3_write_observer__DOT__present_n
               [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]);
        vlTOPp->dma_e3_write_observer__DOT__inside_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = ((3U & vlTOPp->dma_e3_write_observer__DOT__inside_n
                [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 2U));
        vlTOPp->dma_e3_write_observer__DOT__stamps_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)][2U] 
            = vlTOPp->time_i;
        vlTOPp->dma_e3_write_observer__DOT__head_n[(1U 
                                                    & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->dma_e3_write_observer__DOT__head_q
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]));
        vlTOPp->dma_e3_write_observer__DOT__count_n[(1U 
                                                     & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (7U & (vlTOPp->dma_e3_write_observer__DOT__count_n
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)] 
                     - (IData)(1U)));
        if (vlTOPp->active_i) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                        & ((IData)(1U) 
                                                           + 
                                                           VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role)))] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_q
                   [(0x1fU & ((IData)(1U) + VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role)))]);
        }
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [0U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [0U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [0U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [0U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [0U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [0U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [0U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [0U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [0U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [0U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[0U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [1U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [1U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [1U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [1U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [1U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [1U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [1U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [1U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [1U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [1U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[1U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [2U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [2U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [2U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [2U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [2U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [2U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [2U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [2U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [2U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [2U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[2U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [3U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [3U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [3U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [3U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [3U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [3U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [3U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [3U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [3U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [3U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[3U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [4U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [4U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [4U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [4U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [4U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [4U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [4U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [4U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [4U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [4U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[4U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [5U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [5U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [5U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [5U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [5U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [5U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [5U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [5U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [5U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [5U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[5U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [6U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [6U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [6U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [6U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [6U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [6U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [6U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [6U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [6U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [6U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[6U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [7U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [7U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [7U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [7U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [7U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [7U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [7U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [7U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [7U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [7U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[7U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[7U] = 0U;
    }
    if (vlTOPp->open_i) {
        vlTOPp->dma_e3_write_observer__DOT__data_n[0xdU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [0U]));
        vlTOPp->dma_e3_write_observer__DOT__data_n[0x1dU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [1U]));
    }
    if (vlTOPp->close_i) {
        vlTOPp->dma_e3_write_observer__DOT__data_n[0xbU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [0U]));
        vlTOPp->dma_e3_write_observer__DOT__data_n[0x1bU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [1U]));
        vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] = 0ULL;
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [0U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [0U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [1U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [1U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [2U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [2U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [3U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [3U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [4U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [4U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [5U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [5U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [6U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [6U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [7U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [7U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [1U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [1U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [2U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [2U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [3U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [3U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [4U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [4U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [5U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [5U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [6U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [6U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [7U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [7U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [8U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [8U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [9U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [9U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0xaU] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0xaU])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x10U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x10U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x11U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x11U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x12U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x12U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x13U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x13U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x14U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x14U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x15U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x15U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x16U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x16U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x17U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x17U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x18U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x18U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x19U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x19U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x1aU] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x1aU])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
}

void Vdma_e3_write_observer::_eval_initial(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_eval_initial\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_e3_write_observer::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::final\n"); );
    // Variables
    Vdma_e3_write_observer__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_e3_write_observer::_eval_settle(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_eval_settle\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vdma_e3_write_observer::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    clear_i = VL_RAND_RESET_I(1);
    run_i = VL_RAND_RESET_I(1);
    active_i = VL_RAND_RESET_I(1);
    open_i = VL_RAND_RESET_I(1);
    close_i = VL_RAND_RESET_I(1);
    time_i = VL_RAND_RESET_I(32);
    aw_i = VL_RAND_RESET_I(1);
    wlast_i = VL_RAND_RESET_I(1);
    b_i = VL_RAND_RESET_I(1);
    aw_id_i = VL_RAND_RESET_I(4);
    b_id_i = VL_RAND_RESET_I(4);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    error_o = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            dma_e3_write_observer__DOT__present_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            dma_e3_write_observer__DOT__present_n[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            dma_e3_write_observer__DOT__inside_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            dma_e3_write_observer__DOT__inside_n[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            dma_e3_write_observer__DOT__role_q[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            dma_e3_write_observer__DOT__role_n[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<3; ++__Vi1) {
                    dma_e3_write_observer__DOT__stamps_q[__Vi0][__Vi1] = VL_RAND_RESET_I(32);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<3; ++__Vi1) {
                    dma_e3_write_observer__DOT__stamps_n[__Vi0][__Vi1] = VL_RAND_RESET_I(32);
            }}
    }}
    dma_e3_write_observer__DOT__aw_tail_q = VL_RAND_RESET_I(3);
    dma_e3_write_observer__DOT__aw_tail_n = VL_RAND_RESET_I(3);
    dma_e3_write_observer__DOT__w_tail_q = VL_RAND_RESET_I(3);
    dma_e3_write_observer__DOT__w_tail_n = VL_RAND_RESET_I(3);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<4; ++__Vi1) {
                    dma_e3_write_observer__DOT__order_q[__Vi0][__Vi1] = VL_RAND_RESET_I(3);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<4; ++__Vi1) {
                    dma_e3_write_observer__DOT__order_n[__Vi0][__Vi1] = VL_RAND_RESET_I(3);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_e3_write_observer__DOT__head_q[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_e3_write_observer__DOT__head_n[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_e3_write_observer__DOT__tail_q[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_e3_write_observer__DOT__tail_n[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_e3_write_observer__DOT__count_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            dma_e3_write_observer__DOT__count_n[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            dma_e3_write_observer__DOT__data_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            dma_e3_write_observer__DOT__data_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_e3_write_observer__DOT__bad = VL_RAND_RESET_I(1);
    dma_e3_write_observer__DOT__role = VL_RAND_RESET_I(32);
    dma_e3_write_observer__DOT__slot = VL_RAND_RESET_I(32);
    dma_e3_write_observer__DOT__base = VL_RAND_RESET_I(32);
    dma_e3_write_observer__DOT__aw_w = VL_RAND_RESET_Q(64);
    dma_e3_write_observer__DOT__w_b = VL_RAND_RESET_Q(64);
    dma_e3_write_observer__DOT__aw_b = VL_RAND_RESET_Q(64);
    dma_e3_write_observer__DOT____Vlvbound1 = VL_RAND_RESET_I(32);
    dma_e3_write_observer__DOT____Vlvbound2 = VL_RAND_RESET_I(32);
}
