// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_e2_timing.h for the primary calling header

#include "Vdma_research_e2_timing.h"
#include "Vdma_research_e2_timing__Syms.h"

//==========

void Vdma_research_e2_timing::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_research_e2_timing::eval\n"); );
    Vdma_research_e2_timing__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_e2_timing.sv", 55, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_research_e2_timing::_eval_initial_loop(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_e2_timing.sv", 55, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_research_e2_timing::_sequent__TOP__1(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_sequent__TOP__1\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_e2_timing__DOT__done_issue_q 
            = vlTOPp->dma_research_e2_timing__DOT__done_issue_n;
        vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_q 
            = vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_n;
        vlTOPp->dma_research_e2_timing__DOT__done_epoch_q 
            = vlTOPp->dma_research_e2_timing__DOT__done_epoch_n;
        vlTOPp->dma_research_e2_timing__DOT__first_select_issue_q 
            = vlTOPp->dma_research_e2_timing__DOT__first_select_issue_n;
        vlTOPp->dma_research_e2_timing__DOT__err_q 
            = vlTOPp->dma_research_e2_timing__DOT__err_n;
        vlTOPp->dma_research_e2_timing__DOT__v_sel_q 
            = vlTOPp->dma_research_e2_timing__DOT__v_sel_n;
        vlTOPp->dma_research_e2_timing__DOT__v_miss_q 
            = vlTOPp->dma_research_e2_timing__DOT__v_miss_n;
        vlTOPp->dma_research_e2_timing__DOT__v_main_q 
            = vlTOPp->dma_research_e2_timing__DOT__v_main_n;
        vlTOPp->dma_research_e2_timing__DOT__v_comp_q 
            = vlTOPp->dma_research_e2_timing__DOT__v_comp_n;
        vlTOPp->dma_research_e2_timing__DOT__v_hit_q 
            = vlTOPp->dma_research_e2_timing__DOT__v_hit_n;
        vlTOPp->dma_research_e2_timing__DOT__flags_q 
            = vlTOPp->dma_research_e2_timing__DOT__flags_n;
        vlTOPp->dma_research_e2_timing__DOT__issue_epoch_q 
            = vlTOPp->dma_research_e2_timing__DOT__issue_epoch_n;
        vlTOPp->dma_research_e2_timing__DOT__state_q 
            = vlTOPp->dma_research_e2_timing__DOT__state_n;
    } else {
        vlTOPp->dma_research_e2_timing__DOT__done_issue_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__done_epoch_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__first_select_issue_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__err_q = 0U;
        vlTOPp->dma_research_e2_timing__DOT__v_sel_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__v_miss_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__v_main_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__v_comp_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__v_hit_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__flags_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__issue_epoch_q = 0ULL;
        vlTOPp->dma_research_e2_timing__DOT__state_q = 0U;
    }
    vlTOPp->done_issue_cycles_o = vlTOPp->dma_research_e2_timing__DOT__done_issue_q;
    vlTOPp->first_select_epoch_o = vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_q;
    vlTOPp->done_epoch_o = vlTOPp->dma_research_e2_timing__DOT__done_epoch_q;
    vlTOPp->first_select_issue_cycles_o = vlTOPp->dma_research_e2_timing__DOT__first_select_issue_q;
    vlTOPp->error_o = vlTOPp->dma_research_e2_timing__DOT__err_q;
    vlTOPp->v_select_count_o = vlTOPp->dma_research_e2_timing__DOT__v_sel_q;
    vlTOPp->v_miss_count_o = vlTOPp->dma_research_e2_timing__DOT__v_miss_q;
    vlTOPp->v_main_cycles_o = vlTOPp->dma_research_e2_timing__DOT__v_main_q;
    vlTOPp->v_comp_cycles_o = vlTOPp->dma_research_e2_timing__DOT__v_comp_q;
    vlTOPp->v_hit_count_o = vlTOPp->dma_research_e2_timing__DOT__v_hit_q;
    vlTOPp->flags_o = vlTOPp->dma_research_e2_timing__DOT__flags_q;
    vlTOPp->issue_epoch_o = vlTOPp->dma_research_e2_timing__DOT__issue_epoch_q;
}

VL_INLINE_OPT void Vdma_research_e2_timing::_combo__TOP__2(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_combo__TOP__2\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_research_e2_timing__DOT__phase_frozen 
        = ((4U == (IData)(vlTOPp->phase_i)) | (5U == (IData)(vlTOPp->phase_i)));
    vlTOPp->dma_research_e2_timing__DOT__issue_edge 
        = ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q)) 
           & (IData)(vlTOPp->issue_v_i));
    vlTOPp->dma_research_e2_timing__DOT__issue_epoch_n 
        = vlTOPp->dma_research_e2_timing__DOT__issue_epoch_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__issue_epoch_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->issue_v_i) {
                    if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                        vlTOPp->dma_research_e2_timing__DOT__issue_epoch_n 
                            = vlTOPp->epoch_i;
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_main_n = vlTOPp->dma_research_e2_timing__DOT__v_main_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_main_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if ((2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         ((1U == (IData)(vlTOPp->ctl_state_i)) 
                          | (2U == (IData)(vlTOPp->ctl_state_i))))) {
                        vlTOPp->dma_research_e2_timing__DOT__v_main_n 
                            = (1ULL + vlTOPp->dma_research_e2_timing__DOT__v_main_q);
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_comp_n = vlTOPp->dma_research_e2_timing__DOT__v_comp_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_comp_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if ((2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         (4U == (IData)(vlTOPp->ctl_state_i)))) {
                        vlTOPp->dma_research_e2_timing__DOT__v_comp_n 
                            = (1ULL + vlTOPp->dma_research_e2_timing__DOT__v_comp_q);
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now 
        = ((IData)(vlTOPp->dma_research_e2_timing__DOT__issue_edge)
            ? vlTOPp->epoch_i : vlTOPp->dma_research_e2_timing__DOT__issue_epoch_q);
    vlTOPp->dma_research_e2_timing__DOT__interval_open 
        = ((IData)(vlTOPp->dma_research_e2_timing__DOT__issue_edge) 
           | (2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q)));
    vlTOPp->dma_research_e2_timing__DOT__v_sel_n = vlTOPp->dma_research_e2_timing__DOT__v_sel_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_sel_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        vlTOPp->dma_research_e2_timing__DOT__v_sel_n 
                            = (1ULL + vlTOPp->dma_research_e2_timing__DOT__v_sel_q);
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_hit_n = vlTOPp->dma_research_e2_timing__DOT__v_hit_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_hit_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->lookup_v_i) {
                        if (vlTOPp->lookup_hit_i) {
                            vlTOPp->dma_research_e2_timing__DOT__v_hit_n 
                                = (1ULL + vlTOPp->dma_research_e2_timing__DOT__v_hit_q);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_miss_n = vlTOPp->dma_research_e2_timing__DOT__v_miss_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_miss_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->lookup_v_i) {
                        if ((1U & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                            vlTOPp->dma_research_e2_timing__DOT__v_miss_n 
                                = (1ULL + vlTOPp->dma_research_e2_timing__DOT__v_miss_q);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__done_issue_n 
        = vlTOPp->dma_research_e2_timing__DOT__done_issue_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__done_issue_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->done_v_i) {
                    if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_e2_timing__DOT__done_issue_n 
                            = ((vlTOPp->epoch_i < vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now)
                                ? 0ULL : (vlTOPp->epoch_i 
                                          - vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now));
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__state_n = vlTOPp->dma_research_e2_timing__DOT__state_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__state_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if (vlTOPp->probe_accept_i) {
                if ((0U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    vlTOPp->dma_research_e2_timing__DOT__state_n = 1U;
                }
            }
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->issue_v_i) {
                    if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                        vlTOPp->dma_research_e2_timing__DOT__state_n = 2U;
                    }
                }
                if (vlTOPp->done_v_i) {
                    if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_e2_timing__DOT__state_n = 3U;
                    } else {
                        if ((3U != (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                            if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_e2_timing__DOT__state_n = 3U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_n 
        = vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        if ((1U & (~ (IData)((vlTOPp->dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_n 
                                = vlTOPp->epoch_i;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__done_epoch_n 
        = vlTOPp->dma_research_e2_timing__DOT__done_epoch_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__done_epoch_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->done_v_i) {
                    if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_e2_timing__DOT__done_epoch_n 
                            = vlTOPp->epoch_i;
                    } else {
                        if ((3U != (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                            if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_e2_timing__DOT__done_epoch_n 
                                    = vlTOPp->epoch_i;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__flags_n = vlTOPp->dma_research_e2_timing__DOT__flags_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__flags_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->issue_v_i) {
                    if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                        vlTOPp->dma_research_e2_timing__DOT__flags_n 
                            = (1ULL | vlTOPp->dma_research_e2_timing__DOT__flags_n);
                    }
                }
                if (vlTOPp->done_v_i) {
                    if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_e2_timing__DOT__flags_n 
                            = (4ULL | vlTOPp->dma_research_e2_timing__DOT__flags_n);
                    } else {
                        if ((3U != (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                            if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_e2_timing__DOT__flags_n 
                                    = (4ULL | vlTOPp->dma_research_e2_timing__DOT__flags_n);
                            }
                        }
                    }
                }
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        if ((1U & (~ (IData)((vlTOPp->dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            vlTOPp->dma_research_e2_timing__DOT__flags_n 
                                = (2ULL | vlTOPp->dma_research_e2_timing__DOT__flags_n);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__first_select_issue_n 
        = vlTOPp->dma_research_e2_timing__DOT__first_select_issue_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__first_select_issue_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        if ((1U & (~ (IData)((vlTOPp->dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            vlTOPp->dma_research_e2_timing__DOT__first_select_issue_n 
                                = ((vlTOPp->epoch_i 
                                    < vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now)
                                    ? 0ULL : (vlTOPp->epoch_i 
                                              - vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__err_n = vlTOPp->dma_research_e2_timing__DOT__err_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__err_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if (vlTOPp->probe_accept_i) {
                if ((0U != (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    vlTOPp->dma_research_e2_timing__DOT__err_n 
                        = (0x80U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                }
            }
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (((IData)(vlTOPp->unsupported_i) 
                     & ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q)) 
                        | (2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))))) {
                    vlTOPp->dma_research_e2_timing__DOT__err_n 
                        = (0x20U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                }
                if (((IData)(vlTOPp->response_error_i) 
                     & (IData)(vlTOPp->dma_research_e2_timing__DOT__interval_open))) {
                    vlTOPp->dma_research_e2_timing__DOT__err_n 
                        = (0x10U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                }
                if (vlTOPp->issue_v_i) {
                    if ((1U != (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                        if (((2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q)) 
                             | (3U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q)))) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (2U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                    }
                }
                if (vlTOPp->done_v_i) {
                    if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                        if ((vlTOPp->epoch_i < vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now)) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (0x40U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                    } else {
                        if ((3U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (8U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        } else {
                            if ((1U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_e2_timing__DOT__err_n 
                                    = (4U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                            }
                        }
                    }
                }
                if ((2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         ((1U == (IData)(vlTOPp->ctl_state_i)) 
                          | (2U == (IData)(vlTOPp->ctl_state_i))))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_e2_timing__DOT__v_main_q)) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                    }
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         (4U == (IData)(vlTOPp->ctl_state_i)))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_e2_timing__DOT__v_comp_q)) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                    }
                }
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_e2_timing__DOT__v_sel_q)) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                        if ((1U & (~ (IData)((vlTOPp->dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            if ((vlTOPp->epoch_i < vlTOPp->dma_research_e2_timing__DOT__issue_epoch_now)) {
                                vlTOPp->dma_research_e2_timing__DOT__err_n 
                                    = (0x40U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                            }
                        }
                    }
                    if (vlTOPp->lookup_v_i) {
                        if (vlTOPp->lookup_hit_i) {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_research_e2_timing__DOT__v_hit_q)) {
                                vlTOPp->dma_research_e2_timing__DOT__err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                            }
                        } else {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_research_e2_timing__DOT__v_miss_q)) {
                                vlTOPp->dma_research_e2_timing__DOT__err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                            }
                        }
                    }
                }
            }
        }
    }
}

void Vdma_research_e2_timing::_eval(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_eval\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__2(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vdma_research_e2_timing::_change_request(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_change_request\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_research_e2_timing::_change_request_1(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_change_request_1\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_research_e2_timing::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((clear_i & 0xfeU))) {
        Verilated::overWidthError("clear_i");}
    if (VL_UNLIKELY((probe_accept_i & 0xfeU))) {
        Verilated::overWidthError("probe_accept_i");}
    if (VL_UNLIKELY((phase_i & 0xf8U))) {
        Verilated::overWidthError("phase_i");}
    if (VL_UNLIKELY((issue_v_i & 0xfeU))) {
        Verilated::overWidthError("issue_v_i");}
    if (VL_UNLIKELY((done_v_i & 0xfeU))) {
        Verilated::overWidthError("done_v_i");}
    if (VL_UNLIKELY((select_v_i & 0xfeU))) {
        Verilated::overWidthError("select_v_i");}
    if (VL_UNLIKELY((ctl_v_i & 0xfeU))) {
        Verilated::overWidthError("ctl_v_i");}
    if (VL_UNLIKELY((ctl_state_i & 0xf8U))) {
        Verilated::overWidthError("ctl_state_i");}
    if (VL_UNLIKELY((lookup_v_i & 0xfeU))) {
        Verilated::overWidthError("lookup_v_i");}
    if (VL_UNLIKELY((lookup_hit_i & 0xfeU))) {
        Verilated::overWidthError("lookup_hit_i");}
    if (VL_UNLIKELY((response_error_i & 0xfeU))) {
        Verilated::overWidthError("response_error_i");}
    if (VL_UNLIKELY((unsupported_i & 0xfeU))) {
        Verilated::overWidthError("unsupported_i");}
}
#endif  // VL_DEBUG
