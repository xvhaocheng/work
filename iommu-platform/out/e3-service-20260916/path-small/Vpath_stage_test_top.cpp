// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpath_stage_test_top.h for the primary calling header

#include "Vpath_stage_test_top.h"
#include "Vpath_stage_test_top__Syms.h"

//==========

void Vpath_stage_test_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vpath_stage_test_top::eval\n"); );
    Vpath_stage_test_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/path_stage_test_top.sv", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vpath_stage_test_top::_eval_initial_loop(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/path_stage_test_top.sv", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vpath_stage_test_top::_sequent__TOP__1(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_sequent__TOP__1\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*2:0*/ __Vdly__path_stage_test_top__DOT__i_stage__DOT__entered_q;
    CData/*2:0*/ __Vdly__path_stage_test_top__DOT__i_stage__DOT__full_q;
    CData/*2:0*/ __Vdly__path_stage_test_top__DOT__i_stage__DOT__carry_q;
    CData/*1:0*/ __Vdly__path_stage_test_top__DOT__i_stage__DOT__tail_q;
    CData/*1:0*/ __Vdlyvdim0__path_stage_test_top__DOT__i_stage__DOT__stamps__v0;
    CData/*7:0*/ __Vdlyvval__path_stage_test_top__DOT__i_stage__DOT__stamps__v0;
    CData/*0:0*/ __Vdlyvset__path_stage_test_top__DOT__i_stage__DOT__stamps__v0;
    CData/*1:0*/ __Vdlyvdim0__path_stage_test_top__DOT__i_stage__DOT__born_in_window__v0;
    CData/*0:0*/ __Vdlyvval__path_stage_test_top__DOT__i_stage__DOT__born_in_window__v0;
    CData/*1:0*/ __Vdly__path_stage_test_top__DOT__i_stage__DOT__head_q;
    CData/*7:0*/ __Vdly__path_stage_test_top__DOT__i_stage__DOT__max_q;
    // Body
    __Vdly__path_stage_test_top__DOT__i_stage__DOT__head_q 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q;
    __Vdly__path_stage_test_top__DOT__i_stage__DOT__tail_q 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__tail_q;
    __Vdlyvset__path_stage_test_top__DOT__i_stage__DOT__stamps__v0 = 0U;
    __Vdly__path_stage_test_top__DOT__i_stage__DOT__max_q 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__max_q;
    __Vdly__path_stage_test_top__DOT__i_stage__DOT__carry_q 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__carry_q;
    __Vdly__path_stage_test_top__DOT__i_stage__DOT__full_q 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__full_q;
    __Vdly__path_stage_test_top__DOT__i_stage__DOT__entered_q 
        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__entered_q;
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdly__path_stage_test_top__DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->leave_i) {
                        __Vdly__path_stage_test_top__DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        __Vdly__path_stage_test_top__DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q 
                        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdly__path_stage_test_top__DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->enter_i) {
                        __Vdlyvval__path_stage_test_top__DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->time_i;
                        __Vdlyvset__path_stage_test_top__DOT__i_stage__DOT__stamps__v0 = 1U;
                        __Vdlyvdim0__path_stage_test_top__DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__tail_q;
                        __Vdlyvval__path_stage_test_top__DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        __Vdlyvdim0__path_stage_test_top__DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__tail_q;
                        __Vdly__path_stage_test_top__DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        __Vdly__path_stage_test_top__DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->path_stage_test_top__DOT__i_stage__DOT__opening_q = 0U;
        }
    } else {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->path_stage_test_top__DOT__i_stage__DOT__closing_q = 0U;
        }
    } else {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_q 
                            = (0x1ffU & (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_next));
                    }
                }
            }
        }
    } else {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched) {
                            if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__whole) {
                                vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_q 
                                    = (0x1ffU & (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_next));
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdly__path_stage_test_top__DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched) {
                            if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__whole) {
                                if (((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age) 
                                     > (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__max_q))) {
                                    __Vdly__path_stage_test_top__DOT__i_stage__DOT__max_q 
                                        = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        __Vdly__path_stage_test_top__DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->error_o = 0U;
            __Vdly__path_stage_test_top__DOT__i_stage__DOT__entered_q = 0U;
            __Vdly__path_stage_test_top__DOT__i_stage__DOT__full_q = 0U;
            __Vdly__path_stage_test_top__DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad) {
                    vlTOPp->error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((0x200U & (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_next))) {
                            vlTOPp->error_o = 1U;
                        }
                        if (vlTOPp->enter_i) {
                            __Vdly__path_stage_test_top__DOT__i_stage__DOT__entered_q 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__entered_q)));
                            if ((7U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__entered_q))) {
                                vlTOPp->error_o = 1U;
                            }
                        }
                        if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched) {
                            if (vlTOPp->path_stage_test_top__DOT__i_stage__DOT__whole) {
                                __Vdly__path_stage_test_top__DOT__i_stage__DOT__full_q 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__full_q)));
                                if ((1U & ((7U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__full_q)) 
                                           | ((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_next) 
                                              >> 9U)))) {
                                    vlTOPp->error_o = 1U;
                                }
                            } else {
                                __Vdly__path_stage_test_top__DOT__i_stage__DOT__carry_q 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__carry_q)));
                                if ((7U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__carry_q))) {
                                    vlTOPp->error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->error_o = 0U;
        __Vdly__path_stage_test_top__DOT__i_stage__DOT__entered_q = 0U;
        __Vdly__path_stage_test_top__DOT__i_stage__DOT__full_q = 0U;
        __Vdly__path_stage_test_top__DOT__i_stage__DOT__carry_q = 0U;
    }
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q 
        = __Vdly__path_stage_test_top__DOT__i_stage__DOT__head_q;
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__tail_q 
        = __Vdly__path_stage_test_top__DOT__i_stage__DOT__tail_q;
    if (__Vdlyvset__path_stage_test_top__DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__born_in_window[__Vdlyvdim0__path_stage_test_top__DOT__i_stage__DOT__born_in_window__v0] 
            = __Vdlyvval__path_stage_test_top__DOT__i_stage__DOT__born_in_window__v0;
    }
    if (__Vdlyvset__path_stage_test_top__DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__stamps[__Vdlyvdim0__path_stage_test_top__DOT__i_stage__DOT__stamps__v0] 
            = __Vdlyvval__path_stage_test_top__DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__max_q 
        = __Vdly__path_stage_test_top__DOT__i_stage__DOT__max_q;
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__entered_q 
        = __Vdly__path_stage_test_top__DOT__i_stage__DOT__entered_q;
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__full_q 
        = __Vdly__path_stage_test_top__DOT__i_stage__DOT__full_q;
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__carry_q 
        = __Vdly__path_stage_test_top__DOT__i_stage__DOT__carry_q;
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_next 
        = (0x3ffU & ((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__area_q) 
                     + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q)));
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

VL_INLINE_OPT void Vpath_stage_test_top::_combo__TOP__3(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_combo__TOP__3\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
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
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched 
        = ((IData)(vlTOPp->leave_i) & ((0U != (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q)) 
                                       | (IData)(vlTOPp->enter_i)));
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__bad 
        = (((IData)(vlTOPp->leave_i) & (~ (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__matched))) 
           | (((IData)(vlTOPp->enter_i) & (4U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q))) 
              & (~ (IData)(vlTOPp->leave_i))));
    if ((0U == (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__count_q))) {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__whole 
            = vlTOPp->active_i;
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age = 0U;
    } else {
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__whole 
            = vlTOPp->path_stage_test_top__DOT__i_stage__DOT__born_in_window
            [vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q];
        vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age 
            = (0xffU & ((IData)(vlTOPp->time_i) - vlTOPp->path_stage_test_top__DOT__i_stage__DOT__stamps
                        [vlTOPp->path_stage_test_top__DOT__i_stage__DOT__head_q]));
    }
    vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_next 
        = (0x3ffU & ((IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__sum_q) 
                     + (IData)(vlTOPp->path_stage_test_top__DOT__i_stage__DOT__age)));
}

void Vpath_stage_test_top::_eval(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_eval\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

VL_INLINE_OPT QData Vpath_stage_test_top::_change_request(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_change_request\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vpath_stage_test_top::_change_request_1(Vpath_stage_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_change_request_1\n"); );
    Vpath_stage_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vpath_stage_test_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpath_stage_test_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((clear_i & 0xfeU))) {
        Verilated::overWidthError("clear_i");}
    if (VL_UNLIKELY((run_i & 0xfeU))) {
        Verilated::overWidthError("run_i");}
    if (VL_UNLIKELY((active_i & 0xfeU))) {
        Verilated::overWidthError("active_i");}
    if (VL_UNLIKELY((enter_i & 0xfeU))) {
        Verilated::overWidthError("enter_i");}
    if (VL_UNLIKELY((leave_i & 0xfeU))) {
        Verilated::overWidthError("leave_i");}
}
#endif  // VL_DEBUG
