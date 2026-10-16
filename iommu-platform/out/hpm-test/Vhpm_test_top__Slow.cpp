// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vhpm_test_top.h for the primary calling header

#include "Vhpm_test_top.h"
#include "Vhpm_test_top__Syms.h"

//==========

VL_CTOR_IMP(Vhpm_test_top) {
    Vhpm_test_top__Syms* __restrict vlSymsp = __VlSymsp = new Vhpm_test_top__Syms(this, name());
    Vhpm_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vhpm_test_top::__Vconfigure(Vhpm_test_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vhpm_test_top::~Vhpm_test_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vhpm_test_top::_settle__TOP__2(Vhpm_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhpm_test_top::_settle__TOP__2\n"); );
    Vhpm_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->hpm_test_top__DOT__event_data[0U] = 0U;
    vlTOPp->hpm_test_top__DOT__event_data[1U] = 0U;
    vlTOPp->hpm_test_top__DOT__event_data[2U] = 0U;
    vlTOPp->hpm_test_top__DOT__event_data[2U] = ((0xfffffffU 
                                                  & vlTOPp->hpm_test_top__DOT__event_data[2U]) 
                                                 | (0xf0000000U 
                                                    & ((IData)(vlTOPp->valid) 
                                                       << 0x1cU)));
    vlTOPp->hpm_test_top__DOT__event_data[2U] = (0x800000U 
                                                 | (0x1007ffffU 
                                                    & vlTOPp->hpm_test_top__DOT__event_data[2U]));
    vlTOPp->hpm_test_top__DOT__event_data[1U] = ((0x7ffffffU 
                                                  & vlTOPp->hpm_test_top__DOT__event_data[1U]) 
                                                 | (0xf8000000U 
                                                    & (vlTOPp->did 
                                                       << 0x1bU)));
    vlTOPp->hpm_test_top__DOT__event_data[2U] = ((0x1ff80000U 
                                                  & vlTOPp->hpm_test_top__DOT__event_data[2U]) 
                                                 | (0x7ffffffU 
                                                    & (vlTOPp->did 
                                                       >> 5U)));
    vlTOPp->hpm_test_top__DOT__inh = 0U;
    vlTOPp->hpm_test_top__DOT__inh = ((0xfffffffeU 
                                       & vlTOPp->hpm_test_top__DOT__inh) 
                                      | (IData)(vlTOPp->inhibit));
    vlTOPp->hpm_test_top__DOT__evt_in = 0ULL;
    vlTOPp->hpm_test_top__DOT__evt_in = (0x8000000000000ULL 
                                         | (0x1ffffffffffffULL 
                                            & vlTOPp->hpm_test_top__DOT__evt_in));
    vlTOPp->hpm_test_top__DOT__evt_in = ((0xfffffffffffffffbULL 
                                          & vlTOPp->hpm_test_top__DOT__evt_in) 
                                         | ((QData)((IData)(vlTOPp->filter_did)) 
                                            << 2U));
    vlTOPp->hpm_test_top__DOT__evt_in = (0xa0ULL | 
                                         (0xfffffffff000000fULL 
                                          & vlTOPp->hpm_test_top__DOT__evt_in));
    vlTOPp->hpm_test_top__DOT__cyc_in = 0ULL;
    vlTOPp->hpm_test_top__DOT__cyc_in = ((1ULL & vlTOPp->hpm_test_top__DOT__cyc_in) 
                                         | (vlTOPp->cycles 
                                            << 1U));
    vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_n 
        = vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_q;
    if ((1U & ((vlTOPp->hpm_test_top__DOT__event_data[2U] 
                >> 0x1cU) & (~ (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_q))))) {
        vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_n = 1U;
    }
    if (((~ (vlTOPp->hpm_test_top__DOT__event_data[2U] 
             >> 0x1cU)) & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_q))) {
        vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_n = 0U;
    }
    vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[0U] 
        = vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[0U];
    vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[1U] 
        = vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U];
    vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[2U] 
        = vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U];
    if ((1U & ((vlTOPp->hpm_test_top__DOT__event_data[2U] 
                >> 0x1cU) & (~ (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_q))))) {
        vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[0U] 
            = vlTOPp->hpm_test_top__DOT__event_data[0U];
        vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[1U] 
            = vlTOPp->hpm_test_top__DOT__event_data[1U];
        vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[2U] 
            = vlTOPp->hpm_test_top__DOT__event_data[2U];
    }
    if (((vlTOPp->hpm_test_top__DOT__event_data[2U] 
          >> 0x1cU) & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_q))) {
        vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[2U] 
            = (0xfffffffU & vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[2U]);
    }
    if (((~ (vlTOPp->hpm_test_top__DOT__event_data[2U] 
             >> 0x1cU)) & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__edged_event_q))) {
        vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[2U] 
            = (0xfffffffU & vlTOPp->hpm_test_top__DOT__dut__DOT__event_n[2U]);
    }
    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr = 0U;
    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound2 
        = ((0xffffffU & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U] 
                          << 5U) | (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                                    >> 0x1bU))) == 
           (0xffffffU & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                 >> 4U))));
    vlTOPp->hpm_test_top__DOT__dut__DOT__did_match 
        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound2;
    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
            >> 0x1aU) & ((0xfffffU & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U] 
                                       << 0x1aU) | 
                                      (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                                       >> 6U))) == 
                         (0xfffffU & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                              >> 0x1cU)))));
    vlTOPp->hpm_test_top__DOT__dut__DOT__pid_match 
        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound3;
    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound4 
        = ((0xffffU & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                        << 0xbU) | (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[0U] 
                                    >> 0x15U))) == 
           (0xffffU & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                               >> 4U))));
    vlTOPp->hpm_test_top__DOT__dut__DOT__gscid_match 
        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound4;
    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound5 
        = (1U & ((~ (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[0U] 
                     >> 0x14U)) | ((0xfffffU & vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[0U]) 
                                   == (0xfffffU & (IData)(
                                                          (vlTOPp->hpm_test_top__DOT__evt_in 
                                                           >> 0x1cU))))));
    vlTOPp->hpm_test_top__DOT__dut__DOT__pscid_match 
        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound5;
    vlTOPp->__Vfunc_hpm_event_match__0__mask = (0x1ffU 
                                                & (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U] 
                                                   >> 0x13U));
    vlTOPp->__Vfunc_hpm_event_match__0__ctr_event = 
        (0x7fffU & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                            >> 0x31U)));
    vlTOPp->__Vfunc_hpm_event_match__0__masked_event 
        = ((IData)(vlTOPp->__Vfunc_hpm_event_match__0__mask) 
           & ((8U >= (IData)(vlTOPp->__Vfunc_hpm_event_match__0__ctr_event))
               ? ((IData)(1U) << (IData)(vlTOPp->__Vfunc_hpm_event_match__0__ctr_event))
               : 0U));
    vlTOPp->__Vfunc_hpm_event_match__0__Vfuncout = 
        (0U != (IData)(vlTOPp->__Vfunc_hpm_event_match__0__masked_event));
    if (((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U] 
          >> 0x1cU) & (IData)(vlTOPp->__Vfunc_hpm_event_match__0__Vfuncout))) {
        if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                           >> 1U)))) {
            if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                               >> 2U)))) {
                if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                   >> 3U)))) {
                    if ((0x20U & vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U])) {
                        if (((IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                      >> 0x30U)) & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__pscid_match))) {
                            vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k = 0U;
                            {
                                while ((0x10U > vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k)) {
                                    if ((1U & (~ (IData)(
                                                         (vlTOPp->hpm_test_top__DOT__evt_in 
                                                          >> 
                                                          (0x3fU 
                                                           & ((IData)(4U) 
                                                              + 
                                                              (0x1fU 
                                                               & vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k)))))))) {
                                        vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound15 
                                            = (((0xfU 
                                                 >= 
                                                 ((IData)(1U) 
                                                  + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k))
                                                 ? 
                                                (0xffffU 
                                                 & ((0xffffU 
                                                     & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                                                         << 0xbU) 
                                                        | (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[0U] 
                                                           >> 0x15U))) 
                                                    >> 
                                                    ((IData)(1U) 
                                                     + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k)))
                                                 : 0U) 
                                               == (
                                                   (0xfU 
                                                    >= 
                                                    ((IData)(1U) 
                                                     + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k))
                                                    ? 
                                                   (0xffffU 
                                                    & ((0xffffU 
                                                        & (IData)(
                                                                  (vlTOPp->hpm_test_top__DOT__evt_in 
                                                                   >> 4U))) 
                                                       >> 
                                                       ((IData)(1U) 
                                                        + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k)))
                                                    : 0U));
                                        vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                                            = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound15;
                                        goto __Vlabel1;
                                    }
                                    vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k 
                                        = ((IData)(1U) 
                                           + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k);
                                }
                                __Vlabel1: ;
                            }
                        } else {
                            vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound16 
                                = ((IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__gscid_match) 
                                   & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__pscid_match));
                            vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                                = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound16;
                        }
                    } else {
                        vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr = 1U;
                    }
                } else {
                    if ((0x20U & vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U])) {
                        if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                           >> 0x30U)))) {
                            vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k = 0U;
                            {
                                while ((0x10U > vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k)) {
                                    if ((1U & (~ (IData)(
                                                         (vlTOPp->hpm_test_top__DOT__evt_in 
                                                          >> 
                                                          (0x3fU 
                                                           & ((IData)(4U) 
                                                              + 
                                                              (0x1fU 
                                                               & vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k)))))))) {
                                        vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound12 
                                            = (((0xfU 
                                                 >= 
                                                 ((IData)(1U) 
                                                  + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k))
                                                 ? 
                                                (0xffffU 
                                                 & ((0xffffU 
                                                     & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                                                         << 0xbU) 
                                                        | (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[0U] 
                                                           >> 0x15U))) 
                                                    >> 
                                                    ((IData)(1U) 
                                                     + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k)))
                                                 : 0U) 
                                               == (
                                                   (0xfU 
                                                    >= 
                                                    ((IData)(1U) 
                                                     + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k))
                                                    ? 
                                                   (0xffffU 
                                                    & ((0xffffU 
                                                        & (IData)(
                                                                  (vlTOPp->hpm_test_top__DOT__evt_in 
                                                                   >> 4U))) 
                                                       >> 
                                                       ((IData)(1U) 
                                                        + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k)))
                                                    : 0U));
                                        vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                                            = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound12;
                                        goto __Vlabel2;
                                    }
                                    vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k 
                                        = ((IData)(1U) 
                                           + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k);
                                }
                                __Vlabel2: ;
                            }
                        } else {
                            vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound13 
                                = vlTOPp->hpm_test_top__DOT__dut__DOT__gscid_match;
                            vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                                = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound13;
                        }
                    } else {
                        vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr = 1U;
                    }
                }
            } else {
                if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                   >> 3U)))) {
                    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound11 
                        = vlTOPp->hpm_test_top__DOT__dut__DOT__pscid_match;
                    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound11;
                } else {
                    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr = 1U;
                }
            }
        } else {
            if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                               >> 2U)))) {
                if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                   >> 3U)))) {
                    if (((IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                  >> 0x30U)) & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__pid_match))) {
                        vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k = 0U;
                        {
                            while ((0x18U > vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k)) {
                                if ((1U & (~ (IData)(
                                                     (vlTOPp->hpm_test_top__DOT__evt_in 
                                                      >> 
                                                      (0x3fU 
                                                       & ((IData)(4U) 
                                                          + 
                                                          (0x1fU 
                                                           & vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k)))))))) {
                                    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound9 
                                        = (((0x17U 
                                             >= ((IData)(1U) 
                                                 + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k))
                                             ? (0xffffffU 
                                                & ((0xffffffU 
                                                    & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U] 
                                                        << 5U) 
                                                       | (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                                                          >> 0x1bU))) 
                                                   >> 
                                                   ((IData)(1U) 
                                                    + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k)))
                                             : 0U) 
                                           == ((0x17U 
                                                >= 
                                                ((IData)(1U) 
                                                 + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k))
                                                ? (0xffffffU 
                                                   & ((0xffffffU 
                                                       & (IData)(
                                                                 (vlTOPp->hpm_test_top__DOT__evt_in 
                                                                  >> 4U))) 
                                                      >> 
                                                      ((IData)(1U) 
                                                       + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k)))
                                                : 0U));
                                    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                                        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound9;
                                    goto __Vlabel3;
                                }
                                vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k 
                                    = ((IData)(1U) 
                                       + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k);
                            }
                            __Vlabel3: ;
                        }
                    } else {
                        vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound10 
                            = ((IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__did_match) 
                               & (IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__pid_match));
                        vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                            = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound10;
                    }
                } else {
                    if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                       >> 0x30U)))) {
                        vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k = 0U;
                        {
                            while ((0x18U > vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k)) {
                                if ((1U & (~ (IData)(
                                                     (vlTOPp->hpm_test_top__DOT__evt_in 
                                                      >> 
                                                      (0x3fU 
                                                       & ((IData)(4U) 
                                                          + 
                                                          (0x1fU 
                                                           & vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k)))))))) {
                                    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound7 
                                        = (((0x17U 
                                             >= ((IData)(1U) 
                                                 + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k))
                                             ? (0xffffffU 
                                                & ((0xffffffU 
                                                    & ((vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[2U] 
                                                        << 5U) 
                                                       | (vlTOPp->hpm_test_top__DOT__dut__DOT__event_q[1U] 
                                                          >> 0x1bU))) 
                                                   >> 
                                                   ((IData)(1U) 
                                                    + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k)))
                                             : 0U) 
                                           == ((0x17U 
                                                >= 
                                                ((IData)(1U) 
                                                 + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k))
                                                ? (0xffffffU 
                                                   & ((0xffffffU 
                                                       & (IData)(
                                                                 (vlTOPp->hpm_test_top__DOT__evt_in 
                                                                  >> 4U))) 
                                                      >> 
                                                      ((IData)(1U) 
                                                       + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k)))
                                                : 0U));
                                    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                                        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound7;
                                    goto __Vlabel4;
                                }
                                vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k 
                                    = ((IData)(1U) 
                                       + vlTOPp->hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k);
                            }
                            __Vlabel4: ;
                        }
                    } else {
                        vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound8 
                            = vlTOPp->hpm_test_top__DOT__dut__DOT__did_match;
                        vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                            = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound8;
                    }
                }
            } else {
                if ((1U & (IData)((vlTOPp->hpm_test_top__DOT__evt_in 
                                   >> 3U)))) {
                    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound6 
                        = vlTOPp->hpm_test_top__DOT__dut__DOT__pid_match;
                    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr 
                        = vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound6;
                } else {
                    vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr = 1U;
                }
            }
        }
    }
    vlTOPp->hpm_test_top__DOT__cyc_out[0U] = ((0xfffffffbU 
                                               & vlTOPp->hpm_test_top__DOT__cyc_out[0U]) 
                                              | (4U 
                                                 & ((~ 
                                                     (vlTOPp->hpm_test_top__DOT__inh 
                                                      >> 0x1fU)) 
                                                    << 2U)));
    vlTOPp->hpm_test_top__DOT__cyc_out[0U] = ((7U & 
                                               vlTOPp->hpm_test_top__DOT__cyc_out[0U]) 
                                              | (0xfffffff8U 
                                                 & ((IData)(
                                                            (0x7fffffffffffffffULL 
                                                             & (1ULL 
                                                                + 
                                                                (vlTOPp->hpm_test_top__DOT__cyc_in 
                                                                 >> 1U)))) 
                                                    << 3U)));
    vlTOPp->hpm_test_top__DOT__cyc_out[1U] = ((7U & 
                                               ((IData)(
                                                        (0x7fffffffffffffffULL 
                                                         & (1ULL 
                                                            + 
                                                            (vlTOPp->hpm_test_top__DOT__cyc_in 
                                                             >> 1U)))) 
                                                >> 0x1dU)) 
                                              | (0xfffffff8U 
                                                 & ((IData)(
                                                            ((0x7fffffffffffffffULL 
                                                              & (1ULL 
                                                                 + 
                                                                 (vlTOPp->hpm_test_top__DOT__cyc_in 
                                                                  >> 1U))) 
                                                             >> 0x20U)) 
                                                    << 3U)));
    vlTOPp->hpm_test_top__DOT__cyc_out[2U] = (7U & 
                                              ((IData)(
                                                       ((0x7fffffffffffffffULL 
                                                         & (1ULL 
                                                            + 
                                                            (vlTOPp->hpm_test_top__DOT__cyc_in 
                                                             >> 1U))) 
                                                        >> 0x20U)) 
                                               >> 0x1dU));
    vlTOPp->hpm_test_top__DOT__cyc_out[0U] = ((0xfffffffeU 
                                               & vlTOPp->hpm_test_top__DOT__cyc_out[0U]) 
                                              | ((~ 
                                                  (vlTOPp->hpm_test_top__DOT__inh 
                                                   >> 0x1fU)) 
                                                 & (0x7fffffffffffffffULL 
                                                    == 
                                                    (0x7fffffffffffffffULL 
                                                     & (vlTOPp->hpm_test_top__DOT__cyc_in 
                                                        >> 1U)))));
    vlTOPp->hpm_test_top__DOT__cyc_out[0U] = (2U | 
                                              vlTOPp->hpm_test_top__DOT__cyc_out[0U]);
    vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[0U] 
        = (0xfffffffeU & vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[0U]);
    vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound20 
        = (1ULL + vlTOPp->misses);
    vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[0U] 
        = ((1U & vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[0U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound20) 
                             << 1U)));
    vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[1U] 
        = ((1U & ((IData)(vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound20) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound20 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[2U] 
        = (1U & ((IData)((vlTOPp->hpm_test_top__DOT__dut__DOT____Vlvbound20 
                          >> 0x20U)) >> 0x1fU));
    if (((IData)(vlTOPp->hpm_test_top__DOT__dut__DOT__increment_ctr) 
         & (~ vlTOPp->hpm_test_top__DOT__inh))) {
        vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[0U] 
            = (1U | vlTOPp->hpm_test_top__DOT____Vcellout__dut__iohpmctr_o[0U]);
    }
}

void Vhpm_test_top::_eval_initial(Vhpm_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhpm_test_top::_eval_initial\n"); );
    Vhpm_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

void Vhpm_test_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhpm_test_top::final\n"); );
    // Variables
    Vhpm_test_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vhpm_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vhpm_test_top::_eval_settle(Vhpm_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhpm_test_top::_eval_settle\n"); );
    Vhpm_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vhpm_test_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhpm_test_top::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst_n = VL_RAND_RESET_I(1);
    valid = VL_RAND_RESET_I(1);
    inhibit = VL_RAND_RESET_I(1);
    filter_did = VL_RAND_RESET_I(1);
    did = VL_RAND_RESET_I(24);
    misses = VL_RAND_RESET_Q(64);
    cycles = VL_RAND_RESET_Q(63);
    VL_RAND_RESET_W(93, hpm_test_top__DOT__event_data);
    hpm_test_top__DOT__inh = VL_RAND_RESET_I(32);
    hpm_test_top__DOT__cyc_in = VL_RAND_RESET_Q(64);
    hpm_test_top__DOT__evt_in = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(66, hpm_test_top__DOT__cyc_out);
    VL_RAND_RESET_W(65, hpm_test_top__DOT____Vcellout__dut__iohpmctr_o);
    hpm_test_top__DOT__dut__DOT__increment_ctr = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT__did_match = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT__pid_match = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT__gscid_match = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT__pscid_match = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT__edged_event_q = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT__edged_event_n = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(93, hpm_test_top__DOT__dut__DOT__event_q);
    VL_RAND_RESET_W(93, hpm_test_top__DOT__dut__DOT__event_n);
    hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk2__DOT__k = 0;
    hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk3__DOT__k = 0;
    hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk4__DOT__k = 0;
    hpm_test_top__DOT__dut__DOT__event_logic__DOT__unnamedblk1__DOT__unnamedblk5__DOT__k = 0;
    hpm_test_top__DOT__dut__DOT____Vlvbound2 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound3 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound4 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound5 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound6 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound7 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound8 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound9 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound10 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound11 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound12 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound13 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound15 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound16 = VL_RAND_RESET_I(1);
    hpm_test_top__DOT__dut__DOT____Vlvbound20 = VL_RAND_RESET_Q(64);
    __Vfunc_hpm_event_match__0__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_hpm_event_match__0__ctr_event = VL_RAND_RESET_I(15);
    __Vfunc_hpm_event_match__0__mask = VL_RAND_RESET_I(9);
    __Vfunc_hpm_event_match__0__masked_event = VL_RAND_RESET_I(9);
}
