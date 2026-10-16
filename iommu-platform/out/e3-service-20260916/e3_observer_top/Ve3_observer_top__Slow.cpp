// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Ve3_observer_top.h for the primary calling header

#include "Ve3_observer_top.h"
#include "Ve3_observer_top__Syms.h"

//==========

VL_CTOR_IMP(Ve3_observer_top) {
    Ve3_observer_top__Syms* __restrict vlSymsp = __VlSymsp = new Ve3_observer_top__Syms(this, name());
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Ve3_observer_top::__Vconfigure(Ve3_observer_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Ve3_observer_top::~Ve3_observer_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Ve3_observer_top::_settle__TOP__4(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_settle__TOP__4\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i 
        = (1U & (((IData)(vlTOPp->out_valid_i) & (IData)(vlTOPp->out_ready_i)) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i[0U] = 0x1000ULL;
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i[1U] = 0x1000ULL;
    vlTOPp->e3_observer_top__DOT__tl[0U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[2U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[3U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x1ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xfffffe00U 
                                               & ((IData)(vlTOPp->lookup_i) 
                                                  << 9U)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x2ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xffffff00U 
                                               & ((IData)(vlTOPp->hit_i) 
                                                  << 8U)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = (0x80U | 
                                            vlTOPp->e3_observer_top__DOT__tl[3U]);
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0x3fffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffffc000U 
                                               & (((0U 
                                                    == (IData)(vlTOPp->id_i))
                                                    ? 0xaU
                                                    : 0xbU) 
                                                  << 0xeU)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x3c0U 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0x3fffU 
                                               & (((0U 
                                                    == (IData)(vlTOPp->id_i))
                                                    ? 0xaU
                                                    : 0xbU) 
                                                  >> 0x12U)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffffdfffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffffe000U 
                                               & ((IData)(vlTOPp->capture_i) 
                                                  << 0xdU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffffefffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xfffff000U 
                                               & ((IData)(vlTOPp->available_i) 
                                                  << 0xcU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xfffff7ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xfffff800U 
                                               & ((IData)(vlTOPp->consume_i) 
                                                  << 0xbU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xfffffc03U 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xfffffffcU 
                                               & ((IData)(vlTOPp->extra_i) 
                                                  << 2U)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = (1U | (0xfffffffcU 
                                                  & vlTOPp->e3_observer_top__DOT__tl[2U]));
    vlTOPp->e3_observer_top__DOT__tl[0U] = 0x80001000U;
    vlTOPp->e3_observer_top__DOT__tl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i[0U] 
        = vlTOPp->return_id_i[0U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i[1U] 
        = vlTOPp->return_id_i[1U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i[0U] 
        = vlTOPp->tr_id_i[0U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i[1U] 
        = vlTOPp->tr_id_i[1U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i[0U] 
        = vlTOPp->out_id_i[0U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i[1U] 
        = vlTOPp->out_id_i[1U];
    vlTOPp->e3_observer_top__DOT__ctl[0U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[2U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[3U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[4U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[5U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[6U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0xffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0xffffff00U 
                                                & ((IData)(vlTOPp->select_i) 
                                                   << 8U)));
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0x17fU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0xffffff80U 
                                                & ((IData)(vlTOPp->write_i) 
                                                   << 7U)));
    vlTOPp->e3_observer_top__DOT__ctl[5U] = ((0x7fffffffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[5U]) 
                                             | (0x80000000U 
                                                & ((IData)(vlTOPp->id_i) 
                                                   << 0x1fU)));
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0x180U 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0x7fffffffU 
                                                & ((IData)(vlTOPp->id_i) 
                                                   >> 1U)));
    vlTOPp->e3_observer_top__DOT__ctl[5U] = ((0x8000007fU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[5U]) 
                                             | (0xffffff80U 
                                                & (((0U 
                                                     == (IData)(vlTOPp->id_i))
                                                     ? 0xaU
                                                     : 0xbU) 
                                                   << 7U)));
    vlTOPp->e3_observer_top__DOT__ctl[3U] = (0x80000U 
                                             | (0x7fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[3U]));
    vlTOPp->e3_observer_top__DOT__ctl[4U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[5U] = (0xffffff80U 
                                             & vlTOPp->e3_observer_top__DOT__ctl[5U]);
    vlTOPp->e3_observer_top__DOT__ctl[3U] = (0x40U 
                                             | vlTOPp->e3_observer_top__DOT__ctl[3U]);
    vlTOPp->e3_observer_top__DOT__ctl[2U] = (0x280U 
                                             | (0xc000003fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[2U]));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = (0x40000U 
                                             | (0x3fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[0U]));
    vlTOPp->e3_observer_top__DOT__ctl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[2U] = (0xffffffc0U 
                                             & vlTOPp->e3_observer_top__DOT__ctl[2U]);
    vlTOPp->e3_observer_top__DOT__ctl[0U] = (0x10U 
                                             | (0xffffffc7U 
                                                & vlTOPp->e3_observer_top__DOT__ctl[0U]));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = ((0xfffffffbU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[0U]) 
                                             | (0xfffffffcU 
                                                & ((IData)(vlTOPp->request_i) 
                                                   << 2U)));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = ((0xfffffffdU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[0U]) 
                                             | (0xfffffffeU 
                                                & ((IData)(vlTOPp->response_i) 
                                                   << 1U)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q;
    if (vlTOPp->wlast_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfffeU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfffdU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfffbU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfff7U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xffefU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o) 
              << 4U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xffdfU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 5U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xffbfU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 6U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xff7fU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 7U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfeffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o) 
              << 8U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfdffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 9U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfbffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 0xaU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xf7ffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 0xbU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xefffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o) 
              << 0xcU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xdfffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 0xdU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xbfffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 0xeU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0x7fffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 0xfU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[8U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [8U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[9U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [9U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xaU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xaU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xbU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xbU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xcU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xcU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xdU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xdU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xeU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xeU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xfU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xfU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x10U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x10U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x11U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x11U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x12U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x12U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x13U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x13U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x14U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x14U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x15U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x15U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x16U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x16U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x17U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x17U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x18U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x18U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x19U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x19U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1aU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1aU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1bU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1bU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1cU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1cU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1dU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1dU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1eU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1eU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1fU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1fU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_n 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q;
    if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__write_error;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w = 0ULL;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b = 0ULL;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b = 0ULL;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [0U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [0U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [0U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [1U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [1U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [1U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [2U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [2U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [2U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [3U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [3U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [3U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [4U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[4U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [4U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[4U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [4U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[4U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [5U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[5U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [5U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[5U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [5U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[5U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [6U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[6U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [6U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[6U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [6U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[6U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [7U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[7U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [7U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[7U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [7U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[7U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[8U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [8U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[9U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [9U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xaU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xaU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xbU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xbU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xcU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xdU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xdU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xeU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xeU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xfU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xfU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x10U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x10U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x11U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x11U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x12U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x12U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x13U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x13U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x14U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x14U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x15U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x15U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x16U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x16U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x17U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x17U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x18U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x18U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x19U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x19U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1aU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1aU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1bU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1bU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1cU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1cU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1dU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1dU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1eU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1eU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1fU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1fU];
    if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role 
            = (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
               [1U]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q;
        if ((1U & ((((0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                      [1U]) & (4U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                               [1U])) | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
                   | (4U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q
                      [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[(7U 
                                                                          & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (1U | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
               [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = ((6U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
               | (IData)(vlTOPp->active_i));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)][0U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[(7U 
                                                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[(1U 
                                                                        & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)][vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[(1U 
                                                                                & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[(1U 
                                                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[(1U 
                                                                        & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (7U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]));
        if (vlTOPp->active_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                           & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role))] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
                   [(0x1fU & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role))]);
        }
    }
    if (vlTOPp->wlast_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q;
        if ((2U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
             [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)])) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[(7U 
                                                                          & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (2U | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
               [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = ((5U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 1U));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)][1U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
    }
    if ((2U & (IData)(vlTOPp->return_i))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role 
            = (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
               [1U]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
            [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]
            [vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
            [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]];
        if ((1U & ((((((0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                        [1U]) & (4U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                 [1U])) | (0U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                                           [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)])) 
                     | (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                        [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)])) 
                    | (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                          [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
                          >> 1U))) | (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                      [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
                                      >> 2U)))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[(7U 
                                                                          & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (4U | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
               [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = ((3U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 2U));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)][2U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[(1U 
                                                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[(1U 
                                                                        & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (7U & (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
                     - (IData)(1U)));
        if (vlTOPp->active_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                           & ((IData)(1U) 
                                                                              + 
                                                                              VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)))] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
                   [(0x1fU & ((IData)(1U) + VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)))]);
        }
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [0U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [0U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [0U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [0U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [0U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [0U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [0U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [0U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [0U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [0U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[0U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [1U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [1U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [1U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [1U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [1U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [1U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [1U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [1U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [1U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [1U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[1U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [2U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [2U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [2U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [2U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [2U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [2U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [2U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [2U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [2U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [2U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[2U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [3U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [3U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [3U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [3U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [3U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [3U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [3U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [3U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [3U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [3U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[3U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [4U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [4U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [4U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [4U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [4U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [4U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [4U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [4U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [4U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [4U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[4U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [5U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [5U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [5U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [5U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [5U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [5U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [5U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [5U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [5U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [5U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[5U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [6U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [6U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [6U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [6U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [6U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [6U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [6U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [6U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [6U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [6U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[6U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [7U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [7U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [7U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [7U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [7U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [7U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [7U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [7U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [7U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [7U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[7U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[7U] = 0U;
    }
    if (vlTOPp->open_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xdU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [0U]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1dU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [1U]));
    }
    if (vlTOPp->close_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xbU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [0U]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1bU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [1U]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] = 0ULL;
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [0U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [0U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [1U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [1U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [2U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [2U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [3U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [3U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [4U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [4U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [5U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [5U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [6U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [6U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [7U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [7U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [1U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [2U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [2U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [3U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [3U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [4U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [4U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [5U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [5U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [6U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [6U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [7U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [7U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [8U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [8U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [9U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [9U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0xaU] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0xaU])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x10U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x10U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x11U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x11U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x12U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x12U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x13U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x13U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x14U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x14U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x15U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x15U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x16U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x16U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x17U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x17U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x18U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x18U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x19U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x19U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x1aU] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x1aU])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane 
        = (((4U == (0xffU & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                              << 1U) | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                        >> 0x1fU)))) 
            << 1U) | (1U & (vlTOPp->e3_observer_top__DOT__ctl[6U] 
                            >> 7U)));
    vlTOPp->error_o = (1U & ((((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q) 
                               | (0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error))) 
                              | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__write_error)) 
                             | (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                >> 0xaU)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[8U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [8U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[9U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [9U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xaU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xaU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xbU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xbU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xcU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xcU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xdU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xdU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xeU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xeU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xfU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xfU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x10U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x10U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x11U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x11U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x12U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x12U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x13U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x13U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x14U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x14U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x15U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x15U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x16U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x16U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x17U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x17U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x18U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x18U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x19U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x19U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1aU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1aU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1bU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1bU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1cU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1cU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1dU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1dU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1eU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1eU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1fU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1fU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xeU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | ((IData)(vlTOPp->tr_fire_i) & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                          [0U])));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xeU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                       >> 8U) 
                                                      & (0U 
                                                         == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffffeU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffffdU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffffbU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffff7U & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = (((IData)(vlTOPp->out_valid_i) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q))) 
           & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
              [0U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfffefU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 4U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xdU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | (0xfffffffeU & ((IData)(vlTOPp->tr_fire_i) 
                           & ((0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                               [1U]) << 1U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xdU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | (0x1fffffeU 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                          >> 7U) 
                                                         & ((1U 
                                                             == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                                                            << 1U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfffdfU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1) 
              << 5U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfffbfU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 6U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (1U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfff7fU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 7U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (1U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffeffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 8U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = ((((IData)(vlTOPp->out_valid_i) >> 1U) & 
            (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q) 
                >> 1U))) & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                            [1U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffdffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 9U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xbU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | (((IData)(vlTOPp->tr_fire_i) & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                           [0U])) << 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xbU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | (0x3fffffcU 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                          >> 6U) 
                                                         & ((2U 
                                                             == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                                                            << 2U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                 >> 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffbffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1) 
              << 0xaU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                 >> 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xff7ffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 0xbU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (2U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfefffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 0xcU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (2U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfdfffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 0xdU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = (((IData)(vlTOPp->out_valid_i) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q))) 
           & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
              [0U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfbfffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 0xeU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((7U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | (0xfffffff8U & (((IData)(vlTOPp->tr_fire_i) 
                            << 2U) & ((4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                       [1U]) << 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (7U 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | (0x7fffff8U 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                          >> 5U) 
                                                         & ((3U 
                                                             == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                                                            << 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                 >> 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xf7fffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1) 
              << 0xfU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                 >> 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xeffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 0x10U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (3U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xdffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 0x11U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (3U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xbffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 0x12U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = ((((IData)(vlTOPp->out_valid_i) >> 1U) & 
            (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q) 
                >> 1U))) & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                            [1U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0x7ffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 0x13U));
    vlTOPp->data_o = 0ULL;
    if (((0x60U <= (IData)(vlTOPp->index_i)) & (0xe0U 
                                                > (IData)(vlTOPp->index_i)))) {
        vlTOPp->data_o = vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data
            [(3U & (((IData)(vlTOPp->index_i) - (IData)(0x60U)) 
                    >> 5U))][(3U & (((IData)(vlTOPp->index_i) 
                                     - (IData)(0x60U)) 
                                    >> 3U))][(7U & (IData)(vlTOPp->index_i))];
    } else {
        if ((0xe0U <= (IData)(vlTOPp->index_i))) {
            vlTOPp->data_o = vlTOPp->e3_observer_top__DOT__dut__DOT__write_data
                [(0x1fU & (IData)(vlTOPp->index_i))];
        } else {
            if ((0x20U <= (IData)(vlTOPp->index_i))) {
                vlTOPp->data_o = vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                    [(1U & (((IData)(vlTOPp->index_i) 
                             - (IData)(0x20U)) >> 5U))]
                    [(0x1fU & (IData)(vlTOPp->index_i))];
            } else {
                if (((((((((0U == (IData)(vlTOPp->index_i)) 
                           | (1U == (IData)(vlTOPp->index_i))) 
                          | (2U == (IData)(vlTOPp->index_i))) 
                         | (3U == (IData)(vlTOPp->index_i))) 
                        | (4U == (IData)(vlTOPp->index_i))) 
                       | (5U == (IData)(vlTOPp->index_i))) 
                      | (6U == (IData)(vlTOPp->index_i))) 
                     | (7U == (IData)(vlTOPp->index_i)))) {
                    vlTOPp->data_o = ((0U == (IData)(vlTOPp->index_i))
                                       ? 0x49444d4145330001ULL
                                       : ((1U == (IData)(vlTOPp->index_i))
                                           ? (((QData)((IData)(
                                                               (1U 
                                                                & (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                                                   >> 0xaU)))) 
                                               << 3U) 
                                              | (QData)((IData)(
                                                                (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__write_error) 
                                                                  << 2U) 
                                                                 | (((IData)(
                                                                             (0U 
                                                                              != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error))) 
                                                                     << 1U) 
                                                                    | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q))))))
                                           : ((2U == (IData)(vlTOPp->index_i))
                                               ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q))
                                               : ((3U 
                                                   == (IData)(vlTOPp->index_i))
                                                   ? 
                                                  ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__delay_q)
                                                    ? 0x80ULL
                                                    : 0ULL)
                                                   : 
                                                  ((4U 
                                                    == (IData)(vlTOPp->index_i))
                                                    ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__capture_q))
                                                    : 
                                                   ((5U 
                                                     == (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__available_q))
                                                     : 
                                                    ((6U 
                                                      == (IData)(vlTOPp->index_i))
                                                      ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__consume_q))
                                                      : (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__extra_q)))))))));
                } else {
                    if (((((((((8U == (IData)(vlTOPp->index_i)) 
                               | (9U == (IData)(vlTOPp->index_i))) 
                              | (0xaU == (IData)(vlTOPp->index_i))) 
                             | (0xbU == (IData)(vlTOPp->index_i))) 
                            | (0xcU == (IData)(vlTOPp->index_i))) 
                           | (0xdU == (IData)(vlTOPp->index_i))) 
                          | (0xeU == (IData)(vlTOPp->index_i))) 
                         | (0xfU == (IData)(vlTOPp->index_i)))) {
                        vlTOPp->data_o = ((8U == (IData)(vlTOPp->index_i))
                                           ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__level_q))
                                           : ((9U == (IData)(vlTOPp->index_i))
                                               ? vlTOPp->e3_observer_top__DOT__dut__DOT__pte_q
                                               : ((0xaU 
                                                   == (IData)(vlTOPp->index_i))
                                                   ? 
                                                  vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                                  [1U]
                                                  [0U]
                                                   : 
                                                  ((0xbU 
                                                    == (IData)(vlTOPp->index_i))
                                                    ? 
                                                   vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                                   [0U]
                                                   [0U]
                                                    : 
                                                   ((0xcU 
                                                     == (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__inserted_q))
                                                     : 
                                                    ((0xdU 
                                                      == (IData)(vlTOPp->index_i))
                                                      ? 
                                                     (((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__consume_seen_q)) 
                                                       << 1U) 
                                                      | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__available_seen_q)))
                                                      : 
                                                     ((0xeU 
                                                       == (IData)(vlTOPp->index_i))
                                                       ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__state_at_capture_q))
                                                       : (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__waiting_q)))))))));
                    } else {
                        if ((0x10U == (IData)(vlTOPp->index_i))) {
                            vlTOPp->data_o = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        } else {
                            if (((((0x11U == (IData)(vlTOPp->index_i)) 
                                   | (0x12U == (IData)(vlTOPp->index_i))) 
                                  | (0x13U == (IData)(vlTOPp->index_i))) 
                                 | (0x14U == (IData)(vlTOPp->index_i)))) {
                                vlTOPp->data_o = (QData)((IData)(
                                                                 vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q
                                                                 [
                                                                 (3U 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     - (IData)(1U)))]));
                            } else {
                                if (((((0x15U == (IData)(vlTOPp->index_i)) 
                                       | (0x16U == (IData)(vlTOPp->index_i))) 
                                      | (0x17U == (IData)(vlTOPp->index_i))) 
                                     | (0x18U == (IData)(vlTOPp->index_i)))) {
                                    vlTOPp->data_o 
                                        = (QData)((IData)(
                                                          vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q
                                                          [
                                                          (3U 
                                                           & ((IData)(vlTOPp->index_i) 
                                                              - (IData)(1U)))]));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
               & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                     >> 1U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__events)))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 1U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 2U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 2U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 1U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 2U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 3U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 3U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 2U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 3U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 4U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 4U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 3U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 5U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 6U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 6U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 5U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 6U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 7U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 7U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 6U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 7U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 8U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 8U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 7U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 8U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 9U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 9U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 8U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xaU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xbU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xbU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xaU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xbU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xcU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xcU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xbU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xcU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xdU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xdU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xcU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xdU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xeU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xeU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xdU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xfU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0x10U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x10U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0xfU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x10U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x11U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x11U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x10U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x11U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x12U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x12U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x11U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x12U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x13U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x13U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x12U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 1U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                            | vlTOPp->e3_observer_top__DOT__dut__DOT__events)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 1U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                     & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 1U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 2U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 1U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 2U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 1U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 2U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 3U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 2U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 3U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 2U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 3U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 4U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 4U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 3U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 4U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 6U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 5U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 6U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 5U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 6U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 7U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 6U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 7U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 6U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 7U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 8U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 7U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 8U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 7U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 8U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 9U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 8U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 9U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 8U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 9U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xbU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xaU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xbU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xaU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xbU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xcU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xbU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xcU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xbU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xcU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xdU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xcU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xdU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xcU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xdU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xeU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xdU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xeU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xdU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xeU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x10U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0xfU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x10U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xfU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x10U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x11U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0x10U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x11U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0x10U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x11U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x12U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0x11U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x12U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0x11U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x12U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x13U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0x12U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x13U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0x12U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x13U)))));
}

void Ve3_observer_top::_settle__TOP__5(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_settle__TOP__5\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][3U];
    if ((0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
         [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[3U] 
            = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
               [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[2U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[0U] 
            = (IData)(vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                      [(1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[1U] 
            = (IData)((vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                       [(1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))] 
                       >> 0x20U));
    }
}

void Ve3_observer_top::_eval_initial(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_eval_initial\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Ve3_observer_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::final\n"); );
    // Variables
    Ve3_observer_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Ve3_observer_top::_eval_settle(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_eval_settle\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__4(vlSymsp);
    vlTOPp->_settle__TOP__5(vlSymsp);
}

void Ve3_observer_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    arm_i = VL_RAND_RESET_I(1);
    clear_i = VL_RAND_RESET_I(1);
    active_i = VL_RAND_RESET_I(1);
    open_i = VL_RAND_RESET_I(1);
    close_i = VL_RAND_RESET_I(1);
    abort_i = VL_RAND_RESET_I(1);
    delay128_i = VL_RAND_RESET_I(1);
    select_i = VL_RAND_RESET_I(1);
    write_i = VL_RAND_RESET_I(1);
    request_i = VL_RAND_RESET_I(1);
    response_i = VL_RAND_RESET_I(1);
    lookup_i = VL_RAND_RESET_I(1);
    hit_i = VL_RAND_RESET_I(1);
    capture_i = VL_RAND_RESET_I(1);
    available_i = VL_RAND_RESET_I(1);
    consume_i = VL_RAND_RESET_I(1);
    id_i = VL_RAND_RESET_I(4);
    extra_i = VL_RAND_RESET_I(8);
    index_i = VL_RAND_RESET_I(8);
    tr_fire_i = VL_RAND_RESET_I(2);
    out_valid_i = VL_RAND_RESET_I(2);
    out_ready_i = VL_RAND_RESET_I(2);
    return_i = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            tr_id_i[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            out_id_i[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            return_id_i[__Vi0] = VL_RAND_RESET_I(4);
    }}
    wlast_i = VL_RAND_RESET_I(1);
    data_o = VL_RAND_RESET_Q(64);
    error_o = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(201, e3_observer_top__DOT__ctl);
    VL_RAND_RESET_W(106, e3_observer_top__DOT__tl);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT____Vcellinp__dut__tr_address_i[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT____Vcellinp__dut__return_id_i[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT____Vcellinp__dut__out_id_i[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT____Vcellinp__dut__tr_id_i[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<4; ++__Vi1) {
                    VL_RAND_RESET_W(128, e3_observer_top__DOT__dut__DOT__queue_mem[__Vi0][__Vi1]);
            }}
    }}
    VL_RAND_RESET_W(128, e3_observer_top__DOT__dut__DOT__selected);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__head_q[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__tail_q[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__count_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__seq_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__comp_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__returned_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    e3_observer_top__DOT__dut__DOT__push = VL_RAND_RESET_I(4);
    e3_observer_top__DOT__dut__DOT__pop = VL_RAND_RESET_I(4);
    e3_observer_top__DOT__dut__DOT__events = VL_RAND_RESET_I(20);
    e3_observer_top__DOT__dut__DOT__selected_lane = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__current_lane_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__out_hold_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__time_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__run_q = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__bad_q = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__delay_q = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__current_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<32; ++__Vi1) {
                    e3_observer_top__DOT__dut__DOT__shot_q[__Vi0][__Vi1] = VL_RAND_RESET_Q(64);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<4; ++__Vi1) {
                    { int __Vi2=0; for (; __Vi2<8; ++__Vi2) {
                            e3_observer_top__DOT__dut__DOT__stage_data[__Vi0][__Vi1][__Vi2] = VL_RAND_RESET_Q(64);
                    }}
            }}
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__write_data[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT__stage_error = VL_RAND_RESET_I(16);
    e3_observer_top__DOT__dut__DOT__write_error = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__triggers_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__inserted_q = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__hits_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__misses_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    e3_observer_top__DOT__dut__DOT__capture_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__available_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__consume_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__extra_q = VL_RAND_RESET_I(8);
    e3_observer_top__DOT__dut__DOT__level_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__pte_q = VL_RAND_RESET_Q(64);
    e3_observer_top__DOT__dut__DOT__state_at_capture_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__waiting_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__available_seen_q = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__consume_seen_q = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vlvbound1 = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vlvbound2 = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vlvbound3 = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vlvbound4 = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT____Vlvbound5 = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<3; ++__Vi1) {
                    e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[__Vi0][__Vi1] = VL_RAND_RESET_I(32);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<8; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<3; ++__Vi1) {
                    e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[__Vi0][__Vi1] = VL_RAND_RESET_I(32);
            }}
    }}
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_n = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n = VL_RAND_RESET_I(3);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<4; ++__Vi1) {
                    e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[__Vi0][__Vi1] = VL_RAND_RESET_I(3);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            { int __Vi1=0; for (; __Vi1<4; ++__Vi1) {
                    e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[__Vi0][__Vi1] = VL_RAND_RESET_I(3);
            }}
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[__Vi0] = VL_RAND_RESET_I(3);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__role = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__base = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w = VL_RAND_RESET_Q(64);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b = VL_RAND_RESET_Q(64);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b = VL_RAND_RESET_Q(64);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[__Vi0] = VL_RAND_RESET_I(1);
    }}
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = VL_RAND_RESET_Q(48);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age = VL_RAND_RESET_I(32);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = VL_RAND_RESET_I(3);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad = VL_RAND_RESET_I(1);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next = VL_RAND_RESET_Q(49);
    e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next = VL_RAND_RESET_Q(49);
    __Vdly__e3_observer_top__DOT__dut__DOT__run_q = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__time_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__current_lane_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__triggers_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__inserted_q = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v0 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v5 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v64 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v66 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v68 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v70 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v1 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v5 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v6 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v7 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v5 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v72 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v74 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v5 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v76 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v78 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v2 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v6 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v8 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v9 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v6 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v80 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v82 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v6 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v84 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v86 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v3 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v7 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v10 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v11 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v7 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v88 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v90 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v7 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v92 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v94 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v32 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v33 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v96 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v73 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v98 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v100 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v75 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__hits_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__misses_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v102 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v105 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v78 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v79 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v108 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v110 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v112 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2 = VL_RAND_RESET_I(2);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v3 = VL_RAND_RESET_I(2);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2 = VL_RAND_RESET_I(2);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v3 = VL_RAND_RESET_I(2);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v9 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v10 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v11 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v12 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v13 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v14 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v15 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v16 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v0 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v64 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0 = VL_RAND_RESET_I(1);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v1 = VL_RAND_RESET_I(1);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v2 = VL_RAND_RESET_I(1);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v3 = VL_RAND_RESET_I(1);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v4 = VL_RAND_RESET_I(1);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v5 = VL_RAND_RESET_I(1);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v6 = VL_RAND_RESET_I(1);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v7 = VL_RAND_RESET_I(1);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v8 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v1 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v2 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v3 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v4 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v5 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v6 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v7 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v8 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v3 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v9 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v10 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v11 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v12 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v13 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v14 = VL_RAND_RESET_I(3);
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v15 = VL_RAND_RESET_I(3);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v16 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23 = 0;
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v24 = 0;
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = VL_RAND_RESET_I(32);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = VL_RAND_RESET_I(2);
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = VL_RAND_RESET_I(32);
    __Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0;
    __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = 0;
    __Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 = VL_RAND_RESET_I(1);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = VL_RAND_RESET_I(2);
    __Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = VL_RAND_RESET_I(32);
}
