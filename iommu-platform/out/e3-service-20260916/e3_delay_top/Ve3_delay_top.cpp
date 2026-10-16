// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Ve3_delay_top.h for the primary calling header

#include "Ve3_delay_top.h"
#include "Ve3_delay_top__Syms.h"

//==========

void Ve3_delay_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Ve3_delay_top::eval\n"); );
    Ve3_delay_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/e3_delay_top.sv", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Ve3_delay_top::_eval_initial_loop(Ve3_delay_top__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/e3_delay_top.sv", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Ve3_delay_top::_sequent__TOP__1(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_sequent__TOP__1\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*7:0*/ __Vdly__e3_delay_top__DOT__dut__DOT__remaining_q;
    CData/*7:0*/ __Vdly__e3_delay_top__DOT__dut__DOT__held_q;
    // Body
    __Vdly__e3_delay_top__DOT__dut__DOT__held_q = vlTOPp->e3_delay_top__DOT__dut__DOT__held_q;
    __Vdly__e3_delay_top__DOT__dut__DOT__remaining_q 
        = vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q;
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__take) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__announced_q = 0U;
        }
        if (vlTOPp->available_o) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__announced_q = 1U;
        }
    } else {
        vlTOPp->e3_delay_top__DOT__dut__DOT__announced_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__take) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__selected_q 
                = vlTOPp->capture_o;
        }
    } else {
        vlTOPp->e3_delay_top__DOT__dut__DOT__selected_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->arm_i) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__used_q = 0U;
        }
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__take) {
            if (vlTOPp->capture_o) {
                vlTOPp->e3_delay_top__DOT__dut__DOT__used_q = 1U;
            }
        }
    } else {
        vlTOPp->e3_delay_top__DOT__dut__DOT__used_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__take) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[0U] 
                = vlTOPp->e3_delay_top__DOT__mem_resp[0U];
            vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[1U] 
                = vlTOPp->e3_delay_top__DOT__mem_resp[1U];
            vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[2U] 
                = vlTOPp->e3_delay_top__DOT__mem_resp[2U];
        }
    } else {
        vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[0U] = 0U;
        vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[1U] = 0U;
        vlTOPp->e3_delay_top__DOT__dut__DOT__saved_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->arm_i) {
            vlTOPp->error_o = vlTOPp->e3_delay_top__DOT__dut__DOT__full_q;
        }
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__take) {
            __Vdly__e3_delay_top__DOT__dut__DOT__remaining_q 
                = (((IData)(vlTOPp->capture_o) & (IData)(vlTOPp->delay128_i))
                    ? 0x80U : 0U);
            __Vdly__e3_delay_top__DOT__dut__DOT__held_q = 0U;
            if ((1U & (~ (vlTOPp->e3_delay_top__DOT__mem_resp[0U] 
                          >> 1U)))) {
                vlTOPp->error_o = 1U;
            }
        }
        if (((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q) 
             & (0U != (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q)))) {
            __Vdly__e3_delay_top__DOT__dut__DOT__remaining_q 
                = (0xffU & ((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q) 
                            - (IData)(1U)));
            __Vdly__e3_delay_top__DOT__dut__DOT__held_q 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__held_q)));
        }
    } else {
        __Vdly__e3_delay_top__DOT__dut__DOT__remaining_q = 0U;
        __Vdly__e3_delay_top__DOT__dut__DOT__held_q = 0U;
        vlTOPp->error_o = 0U;
    }
    vlTOPp->e3_delay_top__DOT__dut__DOT__held_q = __Vdly__e3_delay_top__DOT__dut__DOT__held_q;
    vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q 
        = __Vdly__e3_delay_top__DOT__dut__DOT__remaining_q;
    vlTOPp->extra_o = vlTOPp->e3_delay_top__DOT__dut__DOT__held_q;
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__take) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__full_q = 1U;
        }
        if (vlTOPp->e3_delay_top__DOT__dut__DOT__give) {
            vlTOPp->e3_delay_top__DOT__dut__DOT__full_q = 0U;
        }
    } else {
        vlTOPp->e3_delay_top__DOT__dut__DOT__full_q = 0U;
    }
    vlTOPp->available_o = ((((IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__full_q) 
                             & (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__selected_q)) 
                            & (0U == (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__remaining_q))) 
                           & (~ (IData)(vlTOPp->e3_delay_top__DOT__dut__DOT__announced_q)));
}

VL_INLINE_OPT void Ve3_delay_top::_combo__TOP__3(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_combo__TOP__3\n"); );
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

void Ve3_delay_top::_eval(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_eval\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Ve3_delay_top::_change_request(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_change_request\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Ve3_delay_top::_change_request_1(Ve3_delay_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_change_request_1\n"); );
    Ve3_delay_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Ve3_delay_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_delay_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((enable_i & 0xfeU))) {
        Verilated::overWidthError("enable_i");}
    if (VL_UNLIKELY((arm_i & 0xfeU))) {
        Verilated::overWidthError("arm_i");}
    if (VL_UNLIKELY((eligible_i & 0xfeU))) {
        Verilated::overWidthError("eligible_i");}
    if (VL_UNLIKELY((delay128_i & 0xfeU))) {
        Verilated::overWidthError("delay128_i");}
    if (VL_UNLIKELY((in_valid_i & 0xfeU))) {
        Verilated::overWidthError("in_valid_i");}
    if (VL_UNLIKELY((out_ready_i & 0xfeU))) {
        Verilated::overWidthError("out_ready_i");}
    if (VL_UNLIKELY((in_last_i & 0xfeU))) {
        Verilated::overWidthError("in_last_i");}
    if (VL_UNLIKELY((in_id_i & 0xf0U))) {
        Verilated::overWidthError("in_id_i");}
}
#endif  // VL_DEBUG
