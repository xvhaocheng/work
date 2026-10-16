// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Ve3_observer_top.h for the primary calling header

#include "Ve3_observer_top.h"
#include "Ve3_observer_top__Syms.h"

VL_INLINE_OPT void Ve3_observer_top::_combo__TOP__3(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_combo__TOP__3\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i 
        = (1U & (((IData)(vlTOPp->out_valid_i) & (IData)(vlTOPp->out_ready_i)) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__tl[0U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[2U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[3U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x7ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xfffff800U 
                                               & ((IData)(vlTOPp->lookup_i) 
                                                  << 0xbU)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0xbffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xfffffc00U 
                                               & ((IData)(vlTOPp->hit_i) 
                                                  << 0xaU)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = (0x200U 
                                            | vlTOPp->e3_observer_top__DOT__tl[3U]);
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffff0000U 
                                               & (((0U 
                                                    == (IData)(vlTOPp->id_i))
                                                    ? 0xaU
                                                    : 0xbU) 
                                                  << 0x10U)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0xf00U 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xffffU 
                                               & (((0U 
                                                    == (IData)(vlTOPp->id_i))
                                                    ? 0xaU
                                                    : 0xbU) 
                                                  >> 0x10U)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffff7fffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffff8000U 
                                               & (((IData)(vlTOPp->capture_i) 
                                                   | (IData)(vlTOPp->ptw_r_i)) 
                                                  << 0xfU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffffbfffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffffc000U 
                                               & (((IData)(vlTOPp->capture_i) 
                                                   | (IData)(vlTOPp->a_miss_i)) 
                                                  << 0xeU)));
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
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q;
    if (vlTOPp->wlast_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__ctl[0U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[2U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[3U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[4U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[5U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[6U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[7U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[7U] = ((0x1ffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[7U]) 
                                             | (0xfffffe00U 
                                                & ((IData)(vlTOPp->select_i) 
                                                   << 9U)));
    vlTOPp->e3_observer_top__DOT__ctl[7U] = ((0x2ffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[7U]) 
                                             | (0xffffff00U 
                                                & ((IData)(vlTOPp->write_i) 
                                                   << 8U)));
    vlTOPp->e3_observer_top__DOT__ctl[7U] = ((0x300U 
                                              & vlTOPp->e3_observer_top__DOT__ctl[7U]) 
                                             | (IData)(vlTOPp->id_i));
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0xffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0xffffff00U 
                                                & (((0U 
                                                     == (IData)(vlTOPp->id_i))
                                                     ? 0xaU
                                                     : 0xbU) 
                                                   << 8U)));
    vlTOPp->e3_observer_top__DOT__ctl[4U] = (0x100000U 
                                             | (0xffU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[4U]));
    vlTOPp->e3_observer_top__DOT__ctl[5U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[6U] = (0xffffff00U 
                                             & vlTOPp->e3_observer_top__DOT__ctl[6U]);
    vlTOPp->e3_observer_top__DOT__ctl[3U] = (0x40U 
                                             | vlTOPp->e3_observer_top__DOT__ctl[3U]);
    vlTOPp->e3_observer_top__DOT__ctl[2U] = ((0xc000003fU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[2U]) 
                                             | (0xffffffc0U 
                                                & (((IData)(vlTOPp->head_a_i)
                                                     ? 0xbU
                                                     : 0xaU) 
                                                   << 6U)));
    vlTOPp->e3_observer_top__DOT__ctl[2U] = ((0x3fffffffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[2U]) 
                                             | (0xc0000000U 
                                                & (((IData)(vlTOPp->head_a_i)
                                                     ? 4U
                                                     : 0U) 
                                                   << 0x1eU)));
    vlTOPp->e3_observer_top__DOT__ctl[3U] = ((0xffffffc0U 
                                              & vlTOPp->e3_observer_top__DOT__ctl[3U]) 
                                             | (0x3fffffffU 
                                                & (((IData)(vlTOPp->head_a_i)
                                                     ? 4U
                                                     : 0U) 
                                                   >> 2U)));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = (0x40000U 
                                             | (0x3fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[0U]));
    vlTOPp->e3_observer_top__DOT__ctl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[2U] = (0xffffffc0U 
                                             & vlTOPp->e3_observer_top__DOT__ctl[2U]);
    vlTOPp->e3_observer_top__DOT__ctl[0U] = (0x10U 
                                             | (0xffffffc7U 
                                                & vlTOPp->e3_observer_top__DOT__ctl[0U]));
    vlTOPp->e3_observer_top__DOT__ctl[3U] = ((0xffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[3U]) 
                                             | (0xffffff00U 
                                                & (((IData)(vlTOPp->e3_observer_top__DOT__i_queue__DOT__count_q) 
                                                    + 
                                                    ((IData)(vlTOPp->arm_dirty_i)
                                                      ? 1U
                                                      : 0U)) 
                                                   << 8U)));
    vlTOPp->e3_observer_top__DOT__ctl[4U] = ((0xffffff00U 
                                              & vlTOPp->e3_observer_top__DOT__ctl[4U]) 
                                             | (0xffU 
                                                & (((IData)(vlTOPp->e3_observer_top__DOT__i_queue__DOT__count_q) 
                                                    + 
                                                    ((IData)(vlTOPp->arm_dirty_i)
                                                      ? 1U
                                                      : 0U)) 
                                                   >> 0x18U)));
    vlTOPp->e3_observer_top__DOT__ctl[3U] = ((0xffffff7fU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[3U]) 
                                             | (0xffffff80U 
                                                & (((IData)(vlTOPp->e3_observer_top__DOT__queue_error) 
                                                    | (IData)(vlTOPp->queue_bad_i)) 
                                                   << 7U)));
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
    vlTOPp->error_o = (1U & (((((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q) 
                                | (0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error))) 
                               | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__write_error)) 
                              | (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                 >> 0xaU)) | (vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                              >> 7U)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__head_v 
        = (((vlTOPp->e3_observer_top__DOT__ctl[3U] 
             >> 6U) & (0xaU == (0xffffffU & ((vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                              << 0x1aU) 
                                             | (vlTOPp->e3_observer_top__DOT__ctl[2U] 
                                                >> 6U))))) 
           & (0U == (0xffU & ((vlTOPp->e3_observer_top__DOT__ctl[3U] 
                               << 2U) | (vlTOPp->e3_observer_top__DOT__ctl[2U] 
                                         >> 0x1eU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane 
        = (((4U == (0xffU & vlTOPp->e3_observer_top__DOT__ctl[7U])) 
            << 1U) | (1U & (vlTOPp->e3_observer_top__DOT__ctl[7U] 
                            >> 8U)));
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
                                       ? 0x49444d4145330002ULL
                                       : ((1U == (IData)(vlTOPp->index_i))
                                           ? (((QData)((IData)(
                                                               (1U 
                                                                & (vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                                                   >> 7U)))) 
                                               << 4U) 
                                              | (QData)((IData)(
                                                                ((8U 
                                                                  & (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                                                     >> 7U)) 
                                                                 | (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__write_error) 
                                                                     << 2U) 
                                                                    | (((IData)(
                                                                                (0U 
                                                                                != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error))) 
                                                                        << 1U) 
                                                                       | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q)))))))
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
                                } else {
                                    if (((((((0x19U 
                                              == (IData)(vlTOPp->index_i)) 
                                             | (0x1aU 
                                                == (IData)(vlTOPp->index_i))) 
                                            | (0x1bU 
                                               == (IData)(vlTOPp->index_i))) 
                                           | (0x1cU 
                                              == (IData)(vlTOPp->index_i))) 
                                          | (0x1dU 
                                             == (IData)(vlTOPp->index_i))) 
                                         | (0x1eU == (IData)(vlTOPp->index_i)))) {
                                        vlTOPp->data_o 
                                            = (QData)((IData)(
                                                              ((5U 
                                                                >= 
                                                                (7U 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    - (IData)(1U))))
                                                                ? 
                                                               vlTOPp->e3_observer_top__DOT__dut__DOT__gate_q
                                                               [
                                                               (7U 
                                                                & ((IData)(vlTOPp->index_i) 
                                                                   - (IData)(1U)))]
                                                                : 0U)));
                                    } else {
                                        if ((0x1fU 
                                             == (IData)(vlTOPp->index_i))) {
                                            vlTOPp->data_o 
                                                = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__capture_head_v_q));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event 
        = ((0x3eU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event)) 
           | ((IData)(vlTOPp->active_i) & (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                           >> 0xfU)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event 
        = ((0x3dU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event)) 
           | (2U & (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event) 
                     << 1U) & (vlTOPp->e3_observer_top__DOT__tl[2U] 
                               >> 0xdU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event 
        = ((0x3bU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event)) 
           | (0xfffffffcU & (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event) 
                              << 1U) & ((0U != ((vlTOPp->e3_observer_top__DOT__ctl[4U] 
                                                 << 0x18U) 
                                                | (vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                                   >> 8U))) 
                                        << 2U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event 
        = ((0x37U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event)) 
           | (0xfffffff8U & (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event) 
                              << 2U) & ((0U == ((vlTOPp->e3_observer_top__DOT__ctl[4U] 
                                                 << 0x18U) 
                                                | (vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                                   >> 8U))) 
                                        << 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event 
        = ((0x2fU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event)) 
           | (0x10U & (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event) 
                        << 2U) & ((~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__head_v)) 
                                  << 4U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event 
        = ((0x1fU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gate_event)) 
           | (0x7fffe0U & ((((IData)(vlTOPp->active_i) 
                             << 5U) & (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                       >> 9U)) & ((0U 
                                                   != 
                                                   ((vlTOPp->e3_observer_top__DOT__ctl[4U] 
                                                     << 0x18U) 
                                                    | (vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                                       >> 8U))) 
                                                  << 5U))));
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
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xeU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | ((IData)(vlTOPp->tr_fire_i) & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                          [0U])));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xeU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | ((vlTOPp->e3_observer_top__DOT__ctl[7U] 
                                                       >> 9U) 
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
                                                   | (0xfffffeU 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[7U] 
                                                          >> 8U) 
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
                                                   | (0x1fffffcU 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[7U] 
                                                          >> 7U) 
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
                                                   | (0x3fffff8U 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[7U] 
                                                          >> 6U) 
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
    if (VL_UNLIKELY((head_a_i & 0xfeU))) {
        Verilated::overWidthError("head_a_i");}
    if (VL_UNLIKELY((ptw_r_i & 0xfeU))) {
        Verilated::overWidthError("ptw_r_i");}
    if (VL_UNLIKELY((a_miss_i & 0xfeU))) {
        Verilated::overWidthError("a_miss_i");}
    if (VL_UNLIKELY((queue_bad_i & 0xfeU))) {
        Verilated::overWidthError("queue_bad_i");}
    if (VL_UNLIKELY((arm_dirty_i & 0xfeU))) {
        Verilated::overWidthError("arm_dirty_i");}
}
#endif  // VL_DEBUG
