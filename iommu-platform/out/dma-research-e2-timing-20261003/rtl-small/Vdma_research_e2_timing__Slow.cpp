// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_e2_timing.h for the primary calling header

#include "Vdma_research_e2_timing.h"
#include "Vdma_research_e2_timing__Syms.h"

//==========

VL_CTOR_IMP(Vdma_research_e2_timing) {
    Vdma_research_e2_timing__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_research_e2_timing__Syms(this, name());
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_research_e2_timing::__Vconfigure(Vdma_research_e2_timing__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_research_e2_timing::~Vdma_research_e2_timing() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_research_e2_timing::_settle__TOP__3(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_settle__TOP__3\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_research_e2_timing__DOT__phase_frozen 
        = ((4U == (IData)(vlTOPp->phase_i)) | (5U == (IData)(vlTOPp->phase_i)));
    vlTOPp->error_o = vlTOPp->dma_research_e2_timing__DOT__err_q;
    vlTOPp->flags_o = vlTOPp->dma_research_e2_timing__DOT__flags_q;
    vlTOPp->issue_epoch_o = vlTOPp->dma_research_e2_timing__DOT__issue_epoch_q;
    vlTOPp->first_select_epoch_o = vlTOPp->dma_research_e2_timing__DOT__first_select_epoch_q;
    vlTOPp->done_epoch_o = vlTOPp->dma_research_e2_timing__DOT__done_epoch_q;
    vlTOPp->done_issue_cycles_o = vlTOPp->dma_research_e2_timing__DOT__done_issue_q;
    vlTOPp->first_select_issue_cycles_o = vlTOPp->dma_research_e2_timing__DOT__first_select_issue_q;
    vlTOPp->v_main_cycles_o = (QData)((IData)(vlTOPp->dma_research_e2_timing__DOT__v_main_q));
    vlTOPp->v_comp_cycles_o = (QData)((IData)(vlTOPp->dma_research_e2_timing__DOT__v_comp_q));
    vlTOPp->v_select_count_o = (QData)((IData)(vlTOPp->dma_research_e2_timing__DOT__v_sel_q));
    vlTOPp->v_hit_count_o = (QData)((IData)(vlTOPp->dma_research_e2_timing__DOT__v_hit_q));
    vlTOPp->v_miss_count_o = (QData)((IData)(vlTOPp->dma_research_e2_timing__DOT__v_miss_q));
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
        vlTOPp->dma_research_e2_timing__DOT__v_main_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if ((2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         ((1U == (IData)(vlTOPp->ctl_state_i)) 
                          | (2U == (IData)(vlTOPp->ctl_state_i))))) {
                        vlTOPp->dma_research_e2_timing__DOT__v_main_n 
                            = (0xffU & ((IData)(1U) 
                                        + (IData)(vlTOPp->dma_research_e2_timing__DOT__v_main_q)));
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_comp_n = vlTOPp->dma_research_e2_timing__DOT__v_comp_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_comp_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if ((2U == (IData)(vlTOPp->dma_research_e2_timing__DOT__state_q))) {
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         (4U == (IData)(vlTOPp->ctl_state_i)))) {
                        vlTOPp->dma_research_e2_timing__DOT__v_comp_n 
                            = (0xffU & ((IData)(1U) 
                                        + (IData)(vlTOPp->dma_research_e2_timing__DOT__v_comp_q)));
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
        vlTOPp->dma_research_e2_timing__DOT__v_sel_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        vlTOPp->dma_research_e2_timing__DOT__v_sel_n 
                            = (0xffU & ((IData)(1U) 
                                        + (IData)(vlTOPp->dma_research_e2_timing__DOT__v_sel_q)));
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_hit_n = vlTOPp->dma_research_e2_timing__DOT__v_hit_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_hit_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->lookup_v_i) {
                        if (vlTOPp->lookup_hit_i) {
                            vlTOPp->dma_research_e2_timing__DOT__v_hit_n 
                                = (0xffU & ((IData)(1U) 
                                            + (IData)(vlTOPp->dma_research_e2_timing__DOT__v_hit_q)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_e2_timing__DOT__v_miss_n = vlTOPp->dma_research_e2_timing__DOT__v_miss_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_e2_timing__DOT__v_miss_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->phase_i))) {
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->lookup_v_i) {
                        if ((1U & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                            vlTOPp->dma_research_e2_timing__DOT__v_miss_n 
                                = (0xffU & ((IData)(1U) 
                                            + (IData)(vlTOPp->dma_research_e2_timing__DOT__v_miss_q)));
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
                        if ((0xffU == (IData)(vlTOPp->dma_research_e2_timing__DOT__v_main_q))) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                    }
                    if (((IData)(vlTOPp->ctl_v_i) & 
                         (4U == (IData)(vlTOPp->ctl_state_i)))) {
                        if ((0xffU == (IData)(vlTOPp->dma_research_e2_timing__DOT__v_comp_q))) {
                            vlTOPp->dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                        }
                    }
                }
                if (vlTOPp->dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->select_v_i) {
                        if ((0xffU == (IData)(vlTOPp->dma_research_e2_timing__DOT__v_sel_q))) {
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
                            if ((0xffU == (IData)(vlTOPp->dma_research_e2_timing__DOT__v_hit_q))) {
                                vlTOPp->dma_research_e2_timing__DOT__err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_e2_timing__DOT__err_n));
                            }
                        } else {
                            if ((0xffU == (IData)(vlTOPp->dma_research_e2_timing__DOT__v_miss_q))) {
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

void Vdma_research_e2_timing::_eval_initial(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_eval_initial\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_research_e2_timing::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::final\n"); );
    // Variables
    Vdma_research_e2_timing__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_research_e2_timing::_eval_settle(Vdma_research_e2_timing__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_eval_settle\n"); );
    Vdma_research_e2_timing* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
}

void Vdma_research_e2_timing::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_e2_timing::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    clear_i = VL_RAND_RESET_I(1);
    probe_accept_i = VL_RAND_RESET_I(1);
    phase_i = VL_RAND_RESET_I(3);
    epoch_i = VL_RAND_RESET_Q(64);
    issue_v_i = VL_RAND_RESET_I(1);
    done_v_i = VL_RAND_RESET_I(1);
    select_v_i = VL_RAND_RESET_I(1);
    ctl_v_i = VL_RAND_RESET_I(1);
    ctl_state_i = VL_RAND_RESET_I(3);
    lookup_v_i = VL_RAND_RESET_I(1);
    lookup_hit_i = VL_RAND_RESET_I(1);
    response_error_i = VL_RAND_RESET_I(1);
    unsupported_i = VL_RAND_RESET_I(1);
    error_o = VL_RAND_RESET_I(16);
    flags_o = VL_RAND_RESET_Q(64);
    issue_epoch_o = VL_RAND_RESET_Q(64);
    first_select_epoch_o = VL_RAND_RESET_Q(64);
    done_epoch_o = VL_RAND_RESET_Q(64);
    done_issue_cycles_o = VL_RAND_RESET_Q(64);
    first_select_issue_cycles_o = VL_RAND_RESET_Q(64);
    v_main_cycles_o = VL_RAND_RESET_Q(64);
    v_comp_cycles_o = VL_RAND_RESET_Q(64);
    v_select_count_o = VL_RAND_RESET_Q(64);
    v_hit_count_o = VL_RAND_RESET_Q(64);
    v_miss_count_o = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__state_q = VL_RAND_RESET_I(3);
    dma_research_e2_timing__DOT__state_n = VL_RAND_RESET_I(3);
    dma_research_e2_timing__DOT__err_q = VL_RAND_RESET_I(16);
    dma_research_e2_timing__DOT__err_n = VL_RAND_RESET_I(16);
    dma_research_e2_timing__DOT__flags_q = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__flags_n = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__issue_epoch_q = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__issue_epoch_n = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__first_select_epoch_q = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__first_select_epoch_n = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__done_epoch_q = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__done_epoch_n = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__done_issue_q = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__done_issue_n = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__first_select_issue_q = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__first_select_issue_n = VL_RAND_RESET_Q(64);
    dma_research_e2_timing__DOT__v_main_q = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_main_n = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_comp_q = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_comp_n = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_sel_q = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_sel_n = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_hit_q = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_hit_n = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_miss_q = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__v_miss_n = VL_RAND_RESET_I(8);
    dma_research_e2_timing__DOT__phase_frozen = VL_RAND_RESET_I(1);
    dma_research_e2_timing__DOT__issue_edge = VL_RAND_RESET_I(1);
    dma_research_e2_timing__DOT__interval_open = VL_RAND_RESET_I(1);
    dma_research_e2_timing__DOT__issue_epoch_now = VL_RAND_RESET_Q(64);
}
