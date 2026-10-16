// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_observer.h for the primary calling header

#include "Vdma_observer.h"
#include "Vdma_observer__Syms.h"

//==========

VL_CTOR_IMP(Vdma_observer) {
    Vdma_observer__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_observer__Syms(this, name());
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_observer::__Vconfigure(Vdma_observer__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_observer::~Vdma_observer() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_observer::_settle__TOP__2(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_settle__TOP__2\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_observer__DOT__index_n = vlTOPp->dma_observer__DOT__index_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x22ULL > vlTOPp->cfg_wdata_i)) {
                        vlTOPp->dma_observer__DOT__index_n 
                            = (0x3fU & (IData)(vlTOPp->cfg_wdata_i));
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__active = (((2U == (IData)(vlTOPp->dma_observer__DOT__phase_q)) 
                                          | (3U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                         | (4U == (IData)(vlTOPp->dma_observer__DOT__phase_q)));
    vlTOPp->dma_observer__DOT__expected_n = vlTOPp->dma_observer__DOT__expected_q;
    if (((1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)) 
         & ((IData)(vlTOPp->issue_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
        vlTOPp->dma_observer__DOT__expected_n = vlTOPp->issue_id_i
            [vlTOPp->dma_observer__DOT__victim_q];
    }
    if (vlTOPp->dma_observer__DOT__active) {
        if ((2U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
            if ((3U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
                if ((4U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
                    if ((1U & ((IData)(vlTOPp->issue_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
                        vlTOPp->dma_observer__DOT__expected_n 
                            = vlTOPp->issue_id_i[vlTOPp->dma_observer__DOT__victim_q];
                    }
                }
            }
        }
    }
    vlTOPp->cfg_rdata_o = 0ULL;
    if ((0x40U & (IData)(vlTOPp->cfg_addr_i))) {
        if ((0x20U & (IData)(vlTOPp->cfg_addr_i))) {
            if ((0x10U & (IData)(vlTOPp->cfg_addr_i))) {
                if ((8U & (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                  >> 2U)))) {
                        if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlTOPp->cfg_addr_i)))) {
                                if (VL_GTS_III(1,32,32, 0x22U, (IData)(vlTOPp->dma_observer__DOT__index_q))) {
                                    vlTOPp->cfg_rdata_o 
                                        = ((0x21U >= (IData)(vlTOPp->dma_observer__DOT__index_q))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [vlTOPp->dma_observer__DOT__index_q]
                                            : 0ULL);
                                }
                            }
                        }
                    }
                } else {
                    if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                  >> 2U)))) {
                        if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlTOPp->cfg_addr_i)))) {
                                vlTOPp->cfg_rdata_o 
                                    = (QData)((IData)(vlTOPp->dma_observer__DOT__index_q));
                            }
                        }
                    }
                }
            } else {
                if ((8U & (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                  >> 2U)))) {
                        if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlTOPp->cfg_addr_i)))) {
                                vlTOPp->cfg_rdata_o 
                                    = vlTOPp->dma_observer__DOT__target_q;
                            }
                        }
                    }
                } else {
                    if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                  >> 2U)))) {
                        if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlTOPp->cfg_addr_i)))) {
                                vlTOPp->cfg_rdata_o 
                                    = ((0xfffffffffffffffeULL 
                                        & vlTOPp->cfg_rdata_o) 
                                       | (IData)((IData)(
                                                         (1U 
                                                          == (IData)(vlTOPp->dma_observer__DOT__phase_q)))));
                                vlTOPp->cfg_rdata_o 
                                    = ((0xfffffffffffffffdULL 
                                        & vlTOPp->cfg_rdata_o) 
                                       | ((QData)((IData)(vlTOPp->dma_observer__DOT__active)) 
                                          << 1U));
                                vlTOPp->cfg_rdata_o 
                                    = ((0xfffffffffffffffbULL 
                                        & vlTOPp->cfg_rdata_o) 
                                       | ((QData)((IData)(
                                                          (5U 
                                                           == (IData)(vlTOPp->dma_observer__DOT__phase_q)))) 
                                          << 2U));
                                vlTOPp->cfg_rdata_o 
                                    = ((0xffffffffULL 
                                        & vlTOPp->cfg_rdata_o) 
                                       | ((QData)((IData)(vlTOPp->dma_observer__DOT__error_q)) 
                                          << 0x20U));
                            }
                        }
                    }
                }
            }
        } else {
            if ((0x10U & (IData)(vlTOPp->cfg_addr_i))) {
                if ((8U & (IData)(vlTOPp->cfg_addr_i))) {
                    if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                  >> 2U)))) {
                        if ((1U & (~ ((IData)(vlTOPp->cfg_addr_i) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlTOPp->cfg_addr_i)))) {
                                vlTOPp->cfg_rdata_o = 0x49444d414f425301ULL;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__target_n = vlTOPp->dma_observer__DOT__target_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U == (IData)(vlTOPp->cfg_addr_i))) {
                if (((~ (IData)(vlTOPp->dma_observer__DOT__active)) 
                     & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q)))) {
                    vlTOPp->dma_observer__DOT__target_n 
                        = vlTOPp->cfg_wdata_i;
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__has_a_n = vlTOPp->dma_observer__DOT__has_a_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((((((((0ULL == 
                                           (0xfffffffffefcfcfeULL 
                                            & vlTOPp->cfg_wdata_i)) 
                                          & (IData)(vlTOPp->cfg_wdata_i)) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__active))) 
                                        & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                       & (0ULL != vlTOPp->dma_observer__DOT__target_q)) 
                                      & (0x100000000ULL 
                                         >= vlTOPp->dma_observer__DOT__target_q)) 
                                     & ((~ (IData)(
                                                   (vlTOPp->cfg_wdata_i 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->cfg_wdata_i 
                                                             >> 0x10U))))))) {
                                    vlTOPp->dma_observer__DOT__has_a_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x18U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__aggressor_n = vlTOPp->dma_observer__DOT__aggressor_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((((((((0ULL == 
                                           (0xfffffffffefcfcfeULL 
                                            & vlTOPp->cfg_wdata_i)) 
                                          & (IData)(vlTOPp->cfg_wdata_i)) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__active))) 
                                        & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                       & (0ULL != vlTOPp->dma_observer__DOT__target_q)) 
                                      & (0x100000000ULL 
                                         >= vlTOPp->dma_observer__DOT__target_q)) 
                                     & ((~ (IData)(
                                                   (vlTOPp->cfg_wdata_i 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->cfg_wdata_i 
                                                             >> 0x10U))))))) {
                                    vlTOPp->dma_observer__DOT__aggressor_n 
                                        = (3U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x10U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__victim_n = vlTOPp->dma_observer__DOT__victim_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if ((((((((0ULL == 
                                           (0xfffffffffefcfcfeULL 
                                            & vlTOPp->cfg_wdata_i)) 
                                          & (IData)(vlTOPp->cfg_wdata_i)) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__active))) 
                                        & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                       & (0ULL != vlTOPp->dma_observer__DOT__target_q)) 
                                      & (0x100000000ULL 
                                         >= vlTOPp->dma_observer__DOT__target_q)) 
                                     & ((~ (IData)(
                                                   (vlTOPp->cfg_wdata_i 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->cfg_wdata_i 
                                                             >> 0x10U))))))) {
                                    vlTOPp->dma_observer__DOT__victim_n 
                                        = (3U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 8U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__phase_n = vlTOPp->dma_observer__DOT__phase_q;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0U];
    vlTOPp->dma_observer__DOT__count_n[0U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [1U];
    vlTOPp->dma_observer__DOT__count_n[1U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [2U];
    vlTOPp->dma_observer__DOT__count_n[2U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [3U];
    vlTOPp->dma_observer__DOT__count_n[3U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [4U];
    vlTOPp->dma_observer__DOT__count_n[4U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [5U];
    vlTOPp->dma_observer__DOT__count_n[5U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [6U];
    vlTOPp->dma_observer__DOT__count_n[6U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [7U];
    vlTOPp->dma_observer__DOT__count_n[7U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [8U];
    vlTOPp->dma_observer__DOT__count_n[8U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [9U];
    vlTOPp->dma_observer__DOT__count_n[9U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0xaU];
    vlTOPp->dma_observer__DOT__count_n[0xaU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0xbU];
    vlTOPp->dma_observer__DOT__count_n[0xbU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0xcU];
    vlTOPp->dma_observer__DOT__count_n[0xcU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0xdU];
    vlTOPp->dma_observer__DOT__count_n[0xdU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0xeU];
    vlTOPp->dma_observer__DOT__count_n[0xeU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0xfU];
    vlTOPp->dma_observer__DOT__count_n[0xfU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x10U];
    vlTOPp->dma_observer__DOT__count_n[0x10U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x11U];
    vlTOPp->dma_observer__DOT__count_n[0x11U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x12U];
    vlTOPp->dma_observer__DOT__count_n[0x12U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x13U];
    vlTOPp->dma_observer__DOT__count_n[0x13U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x14U];
    vlTOPp->dma_observer__DOT__count_n[0x14U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x15U];
    vlTOPp->dma_observer__DOT__count_n[0x15U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x16U];
    vlTOPp->dma_observer__DOT__count_n[0x16U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x17U];
    vlTOPp->dma_observer__DOT__count_n[0x17U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x18U];
    vlTOPp->dma_observer__DOT__count_n[0x18U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x19U];
    vlTOPp->dma_observer__DOT__count_n[0x19U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x1aU];
    vlTOPp->dma_observer__DOT__count_n[0x1aU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x1bU];
    vlTOPp->dma_observer__DOT__count_n[0x1bU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x1cU];
    vlTOPp->dma_observer__DOT__count_n[0x1cU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x1dU];
    vlTOPp->dma_observer__DOT__count_n[0x1dU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x1eU];
    vlTOPp->dma_observer__DOT__count_n[0x1eU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x1fU];
    vlTOPp->dma_observer__DOT__count_n[0x1fU] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x20U];
    vlTOPp->dma_observer__DOT__count_n[0x20U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x21U];
    vlTOPp->dma_observer__DOT__count_n[0x21U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT__segment_n = vlTOPp->dma_observer__DOT__segment_q;
    vlTOPp->dma_observer__DOT__error_n = vlTOPp->dma_observer__DOT__error_q;
    vlTOPp->dma_observer__DOT__track_bad_n = vlTOPp->dma_observer__DOT__track_bad_q;
    vlTOPp->dma_observer__DOT__reads_n[0U] = vlTOPp->dma_observer__DOT__reads_q
        [0U];
    vlTOPp->dma_observer__DOT__writes_n[0U] = vlTOPp->dma_observer__DOT__writes_q
        [0U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       << 1U)) | (1U & (IData)(vlTOPp->rlast_fire_i))))) {
        vlTOPp->dma_observer__DOT__reads_n[0U] = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [0U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__reads_q
             [0U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (1U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           << 1U)) | (1U & (IData)(vlTOPp->rlast_fire_i))))) {
            vlTOPp->dma_observer__DOT__reads_n[0U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__reads_q
                            [0U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__reads_q
                 [0U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (1U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((2U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                        & (IData)(vlTOPp->aw_ready_i)) 
                       << 1U)) | (1U & (IData)(vlTOPp->b_fire_i))))) {
        vlTOPp->dma_observer__DOT__writes_n[0U] = (0xffU 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [0U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__writes_q
             [0U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (1U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                            & (IData)(vlTOPp->aw_ready_i)) 
                           << 1U)) | (1U & (IData)(vlTOPp->b_fire_i))))) {
            vlTOPp->dma_observer__DOT__writes_n[0U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__writes_q
                            [0U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__writes_q
                 [0U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (1U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((1U & (IData)(vlTOPp->bad_id_i))) {
        vlTOPp->dma_observer__DOT__track_bad_n = (1U 
                                                  | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
    }
    vlTOPp->dma_observer__DOT__reads_n[1U] = vlTOPp->dma_observer__DOT__reads_q
        [1U];
    vlTOPp->dma_observer__DOT__writes_n[1U] = vlTOPp->dma_observer__DOT__writes_q
        [1U];
    if ((2U == ((2U & ((IData)(vlTOPp->ar_valid_i) 
                       & (IData)(vlTOPp->ar_ready_i))) 
                | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                         >> 1U))))) {
        vlTOPp->dma_observer__DOT__reads_n[1U] = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [1U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__reads_q
             [1U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (2U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & ((IData)(vlTOPp->ar_valid_i) 
                           & (IData)(vlTOPp->ar_ready_i))) 
                    | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                             >> 1U))))) {
            vlTOPp->dma_observer__DOT__reads_n[1U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__reads_q
                            [1U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__reads_q
                 [1U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (2U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((2U == ((2U & ((IData)(vlTOPp->aw_valid_i) 
                       & (IData)(vlTOPp->aw_ready_i))) 
                | (1U & ((IData)(vlTOPp->b_fire_i) 
                         >> 1U))))) {
        vlTOPp->dma_observer__DOT__writes_n[1U] = (0xffU 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [1U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__writes_q
             [1U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (2U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & ((IData)(vlTOPp->aw_valid_i) 
                           & (IData)(vlTOPp->aw_ready_i))) 
                    | (1U & ((IData)(vlTOPp->b_fire_i) 
                             >> 1U))))) {
            vlTOPp->dma_observer__DOT__writes_n[1U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__writes_q
                            [1U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__writes_q
                 [1U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (2U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((2U & (IData)(vlTOPp->bad_id_i))) {
        vlTOPp->dma_observer__DOT__track_bad_n = (2U 
                                                  | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
    }
    vlTOPp->dma_observer__DOT__reads_n[2U] = vlTOPp->dma_observer__DOT__reads_q
        [2U];
    vlTOPp->dma_observer__DOT__writes_n[2U] = vlTOPp->dma_observer__DOT__writes_q
        [2U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       >> 1U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                        >> 2U))))) {
        vlTOPp->dma_observer__DOT__reads_n[2U] = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [2U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__reads_q
             [2U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (4U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           >> 1U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                            >> 2U))))) {
            vlTOPp->dma_observer__DOT__reads_n[2U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__reads_q
                            [2U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__reads_q
                 [2U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (4U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((2U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                        & (IData)(vlTOPp->aw_ready_i)) 
                       >> 1U)) | (1U & ((IData)(vlTOPp->b_fire_i) 
                                        >> 2U))))) {
        vlTOPp->dma_observer__DOT__writes_n[2U] = (0xffU 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [2U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__writes_q
             [2U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (4U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                            & (IData)(vlTOPp->aw_ready_i)) 
                           >> 1U)) | (1U & ((IData)(vlTOPp->b_fire_i) 
                                            >> 2U))))) {
            vlTOPp->dma_observer__DOT__writes_n[2U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__writes_q
                            [2U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__writes_q
                 [2U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (4U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((4U & (IData)(vlTOPp->bad_id_i))) {
        vlTOPp->dma_observer__DOT__track_bad_n = (4U 
                                                  | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
    }
    vlTOPp->dma_observer__DOT__reads_n[3U] = vlTOPp->dma_observer__DOT__reads_q
        [3U];
    vlTOPp->dma_observer__DOT__writes_n[3U] = vlTOPp->dma_observer__DOT__writes_q
        [3U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       >> 2U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                        >> 3U))))) {
        vlTOPp->dma_observer__DOT__reads_n[3U] = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [3U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__reads_q
             [3U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (8U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           >> 2U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                            >> 3U))))) {
            vlTOPp->dma_observer__DOT__reads_n[3U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__reads_q
                            [3U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__reads_q
                 [3U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (8U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((2U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                        & (IData)(vlTOPp->aw_ready_i)) 
                       >> 2U)) | (1U & ((IData)(vlTOPp->b_fire_i) 
                                        >> 3U))))) {
        vlTOPp->dma_observer__DOT__writes_n[3U] = (0xffU 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [3U]));
        if ((0xffU == vlTOPp->dma_observer__DOT__writes_q
             [3U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (8U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                            & (IData)(vlTOPp->aw_ready_i)) 
                           >> 2U)) | (1U & ((IData)(vlTOPp->b_fire_i) 
                                            >> 3U))))) {
            vlTOPp->dma_observer__DOT__writes_n[3U] 
                = (0xffU & (vlTOPp->dma_observer__DOT__writes_q
                            [3U] - (IData)(1U)));
            if ((0U == vlTOPp->dma_observer__DOT__writes_q
                 [3U])) {
                vlTOPp->dma_observer__DOT__track_bad_n 
                    = (8U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
            }
        }
    }
    if ((8U & (IData)(vlTOPp->bad_id_i))) {
        vlTOPp->dma_observer__DOT__track_bad_n = (8U 
                                                  | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
    }
    if (((1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)) 
         & ((IData)(vlTOPp->issue_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
        vlTOPp->dma_observer__DOT__phase_n = 2U;
        vlTOPp->dma_observer__DOT__count_n[7U] = 1ULL;
        vlTOPp->dma_observer__DOT__segment_n = 0ULL;
        if ((0ULL == vlTOPp->issue_id_i[vlTOPp->dma_observer__DOT__victim_q])) {
            vlTOPp->dma_observer__DOT__error_n = (2U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->dma_observer__DOT__count_n[0x1eU] = 
            (((QData)((IData)(vlTOPp->dma_observer__DOT__reads_n
                              [vlTOPp->dma_observer__DOT__victim_q])) 
              << 0x10U) | (QData)((IData)(vlTOPp->dma_observer__DOT__writes_n
                                          [vlTOPp->dma_observer__DOT__victim_q])));
        vlTOPp->dma_observer__DOT__count_n[0x1fU] = 
            ((IData)(vlTOPp->dma_observer__DOT__has_a_q)
              ? (((QData)((IData)(vlTOPp->dma_observer__DOT__reads_n
                                  [vlTOPp->dma_observer__DOT__aggressor_q])) 
                  << 0x10U) | (QData)((IData)(vlTOPp->dma_observer__DOT__writes_n
                                              [vlTOPp->dma_observer__DOT__aggressor_q])))
              : 0ULL);
    }
    if (((1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)) 
         | (IData)(vlTOPp->dma_observer__DOT__active))) {
        if ((1U & (((IData)(vlTOPp->dma_observer__DOT__track_bad_n) 
                    >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
                   | ((IData)(vlTOPp->dma_observer__DOT__has_a_q) 
                      & ((IData)(vlTOPp->dma_observer__DOT__track_bad_n) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)))))) {
            vlTOPp->dma_observer__DOT__error_n = (8U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
    }
    if (vlTOPp->dma_observer__DOT__active) {
        vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot = 0U;
        vlTOPp->dma_observer__DOT____Vlvbound1 = (1ULL 
                                                  + 
                                                  vlTOPp->dma_observer__DOT__count_q
                                                  [0U]);
        vlTOPp->dma_observer__DOT__count_n[(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot)] 
            = vlTOPp->dma_observer__DOT____Vlvbound1;
        if ((0xffffffffffffffffULL == ((0x21U >= (0x3fU 
                                                  & vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot))
                                        ? vlTOPp->dma_observer__DOT__count_q
                                       [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot)]
                                        : 0ULL))) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->dma_observer__DOT__segment_n = (1ULL 
                                                + vlTOPp->dma_observer__DOT__segment_q);
        if ((0xffffffffffffffffULL == vlTOPp->dma_observer__DOT__segment_q)) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot 
            = ((IData)(0xaU) + ((((IData)(vlTOPp->dma_observer__DOT__has_a_q) 
                                  & ((((0U != vlTOPp->dma_observer__DOT__reads_q
                                        [vlTOPp->dma_observer__DOT__aggressor_q]) 
                                       | (0U != vlTOPp->dma_observer__DOT__writes_q
                                          [vlTOPp->dma_observer__DOT__aggressor_q])) 
                                      | ((IData)(vlTOPp->ar_valid_i) 
                                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q))) 
                                     | ((IData)(vlTOPp->aw_valid_i) 
                                        >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)))) 
                                 << 1U) | (1U & (((
                                                   (0U 
                                                    != 
                                                    vlTOPp->dma_observer__DOT__reads_q
                                                    [vlTOPp->dma_observer__DOT__victim_q]) 
                                                   | (0U 
                                                      != 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [vlTOPp->dma_observer__DOT__victim_q])) 
                                                  | ((IData)(vlTOPp->ar_valid_i) 
                                                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q))) 
                                                 | ((IData)(vlTOPp->aw_valid_i) 
                                                    >> (IData)(vlTOPp->dma_observer__DOT__victim_q))))));
        vlTOPp->dma_observer__DOT____Vlvbound1 = (1ULL 
                                                  + 
                                                  ((0x21U 
                                                    >= 
                                                    (0x3fU 
                                                     & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot))
                                                    ? 
                                                   vlTOPp->dma_observer__DOT__count_q
                                                   [
                                                   (0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot)]
                                                    : 0ULL));
        if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot))) {
            vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot)] 
                = vlTOPp->dma_observer__DOT____Vlvbound1;
        }
        if ((0xffffffffffffffffULL == ((0x21U >= (0x3fU 
                                                  & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot))
                                        ? vlTOPp->dma_observer__DOT__count_q
                                       [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot)]
                                        : 0ULL))) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__2__enable 
            = (1U & (((IData)(vlTOPp->ar_valid_i) & (IData)(vlTOPp->ar_ready_i)) 
                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot = 0xeU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__2__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable 
            = (1U & (((IData)(vlTOPp->aw_valid_i) & (IData)(vlTOPp->aw_ready_i)) 
                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot = 0xfU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable 
            = (1U & ((IData)(vlTOPp->rlast_fire_i) 
                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot = 0x10U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable 
            = (1U & ((IData)(vlTOPp->b_fire_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot = 0x11U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable 
            = (1U & (((IData)(vlTOPp->ar_valid_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
                     & (~ ((IData)(vlTOPp->ar_ready_i) 
                           >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))));
        vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot = 0x12U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable 
            = (1U & (((IData)(vlTOPp->aw_valid_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
                     & (~ ((IData)(vlTOPp->aw_ready_i) 
                           >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))));
        vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot = 0x13U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        if (vlTOPp->dma_observer__DOT__has_a_q) {
            vlTOPp->__Vtask_dma_observer__DOT__bump__2__enable 
                = (1U & (((IData)(vlTOPp->ar_valid_i) 
                          & (IData)(vlTOPp->ar_ready_i)) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot = 0x14U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__2__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)]
                                : 0ULL));
                if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable 
                = (1U & (((IData)(vlTOPp->aw_valid_i) 
                          & (IData)(vlTOPp->aw_ready_i)) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot = 0x15U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                                : 0ULL));
                if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable 
                = (1U & ((IData)(vlTOPp->rlast_fire_i) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot = 0x16U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                                : 0ULL));
                if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable 
                = (1U & ((IData)(vlTOPp->b_fire_i) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot = 0x17U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                                : 0ULL));
                if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable 
                = (1U & (((IData)(vlTOPp->ar_valid_i) 
                          >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)) 
                         & (~ ((IData)(vlTOPp->ar_ready_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)))));
            vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot = 0x18U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                                : 0ULL));
                if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable 
                = (1U & (((IData)(vlTOPp->aw_valid_i) 
                          >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)) 
                         & (~ ((IData)(vlTOPp->aw_ready_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)))));
            vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot = 0x19U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                                : 0ULL));
                if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__8__enable 
            = ((IData)(vlTOPp->shared_ar_valid_i) & (IData)(vlTOPp->shared_ar_ready_i));
        vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot = 0x1aU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__8__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__9__enable 
            = ((IData)(vlTOPp->shared_aw_valid_i) & (IData)(vlTOPp->shared_aw_ready_i));
        vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot = 0x1bU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__9__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__10__enable 
            = ((IData)(vlTOPp->shared_ar_valid_i) & 
               (~ (IData)(vlTOPp->shared_ar_ready_i)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot = 0x1cU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__10__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__11__enable 
            = ((IData)(vlTOPp->shared_aw_valid_i) & 
               (~ (IData)(vlTOPp->shared_aw_ready_i)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot = 0x1dU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__11__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot)]
                            : 0ULL));
            if ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        if ((2U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
            vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot = 1U;
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (1ULL + vlTOPp->dma_observer__DOT__count_q
                   [1U]);
            vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot)] 
                = vlTOPp->dma_observer__DOT____Vlvbound1;
            if ((0xffffffffffffffffULL == ((0x21U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot)]
                                            : 0ULL))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
            if ((1U & ((IData)(vlTOPp->issue_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (2U | vlTOPp->dma_observer__DOT__error_n);
            }
            if ((1U & ((IData)(vlTOPp->done_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
                if ((vlTOPp->done_id_i[vlTOPp->dma_observer__DOT__victim_q] 
                     != vlTOPp->dma_observer__DOT__expected_q)) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (2U | vlTOPp->dma_observer__DOT__error_n);
                }
                vlTOPp->dma_observer__DOT__phase_n = 3U;
                vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot = 8U;
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + vlTOPp->dma_observer__DOT__count_q
                       [8U]);
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
                if ((vlTOPp->dma_observer__DOT__segment_n 
                     > vlTOPp->dma_observer__DOT__count_q
                     [4U])) {
                    vlTOPp->dma_observer__DOT__count_n[4U] 
                        = vlTOPp->dma_observer__DOT__segment_n;
                }
                vlTOPp->dma_observer__DOT__segment_n = 0ULL;
            }
        } else {
            if ((3U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
                vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot = 2U;
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (1ULL + vlTOPp->dma_observer__DOT__count_q
                       [2U]);
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
                if ((0xffffffffffffffffULL == ((0x21U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot)]
                                                : 0ULL))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
                if ((1U & (((IData)(vlTOPp->issue_i) 
                            | (IData)(vlTOPp->done_i)) 
                           >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (2U | vlTOPp->dma_observer__DOT__error_n);
                }
                if ((((IData)(vlTOPp->poll_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
                     & (vlTOPp->poll_id_i[vlTOPp->dma_observer__DOT__victim_q] 
                        == vlTOPp->dma_observer__DOT__expected_q))) {
                    vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot = 9U;
                    vlTOPp->dma_observer__DOT____Vlvbound1 
                        = (1ULL + vlTOPp->dma_observer__DOT__count_q
                           [9U]);
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                    if ((0xffffffffffffffffULL == (
                                                   (0x21U 
                                                    >= 
                                                    (0x3fU 
                                                     & vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot))
                                                    ? 
                                                   vlTOPp->dma_observer__DOT__count_q
                                                   [
                                                   (0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot)]
                                                    : 0ULL))) {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (4U | vlTOPp->dma_observer__DOT__error_n);
                    }
                    if ((vlTOPp->dma_observer__DOT__segment_n 
                         > vlTOPp->dma_observer__DOT__count_q
                         [5U])) {
                        vlTOPp->dma_observer__DOT__count_n[5U] 
                            = vlTOPp->dma_observer__DOT__segment_n;
                    }
                    vlTOPp->dma_observer__DOT__segment_n = 0ULL;
                    if ((vlTOPp->dma_observer__DOT__count_n
                         [9U] == vlTOPp->dma_observer__DOT__target_q)) {
                        vlTOPp->dma_observer__DOT__phase_n = 5U;
                        vlTOPp->dma_observer__DOT__count_n[0x20U] 
                            = (((QData)((IData)(vlTOPp->dma_observer__DOT__reads_n
                                                [vlTOPp->dma_observer__DOT__victim_q])) 
                                << 0x10U) | (QData)((IData)(
                                                            vlTOPp->dma_observer__DOT__writes_n
                                                            [vlTOPp->dma_observer__DOT__victim_q])));
                        vlTOPp->dma_observer__DOT__count_n[0x21U] 
                            = ((IData)(vlTOPp->dma_observer__DOT__has_a_q)
                                ? (((QData)((IData)(
                                                    vlTOPp->dma_observer__DOT__reads_n
                                                    [vlTOPp->dma_observer__DOT__aggressor_q])) 
                                    << 0x10U) | (QData)((IData)(
                                                                vlTOPp->dma_observer__DOT__writes_n
                                                                [vlTOPp->dma_observer__DOT__aggressor_q])))
                                : 0ULL);
                    } else {
                        vlTOPp->dma_observer__DOT__phase_n = 4U;
                    }
                }
            } else {
                if ((4U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
                    vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot = 3U;
                    vlTOPp->dma_observer__DOT____Vlvbound1 
                        = (1ULL + vlTOPp->dma_observer__DOT__count_q
                           [3U]);
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                    if ((0xffffffffffffffffULL == (
                                                   (0x21U 
                                                    >= 
                                                    (0x3fU 
                                                     & vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot))
                                                    ? 
                                                   vlTOPp->dma_observer__DOT__count_q
                                                   [
                                                   (0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot)]
                                                    : 0ULL))) {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (4U | vlTOPp->dma_observer__DOT__error_n);
                    }
                    if ((1U & ((IData)(vlTOPp->done_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (2U | vlTOPp->dma_observer__DOT__error_n);
                    }
                    if ((1U & ((IData)(vlTOPp->issue_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
                        if (((0ULL == vlTOPp->issue_id_i
                              [vlTOPp->dma_observer__DOT__victim_q]) 
                             | (vlTOPp->issue_id_i[vlTOPp->dma_observer__DOT__victim_q] 
                                == vlTOPp->dma_observer__DOT__expected_q))) {
                            vlTOPp->dma_observer__DOT__error_n 
                                = (2U | vlTOPp->dma_observer__DOT__error_n);
                        }
                        vlTOPp->dma_observer__DOT__phase_n = 2U;
                        vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot = 7U;
                        vlTOPp->dma_observer__DOT____Vlvbound1 
                            = (1ULL + vlTOPp->dma_observer__DOT__count_q
                               [7U]);
                        vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                            & vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot)] 
                            = vlTOPp->dma_observer__DOT____Vlvbound1;
                        if ((0xffffffffffffffffULL 
                             == ((0x21U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot))
                                  ? vlTOPp->dma_observer__DOT__count_q
                                 [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot)]
                                  : 0ULL))) {
                            vlTOPp->dma_observer__DOT__error_n 
                                = (4U | vlTOPp->dma_observer__DOT__error_n);
                        }
                        if ((vlTOPp->dma_observer__DOT__segment_n 
                             > vlTOPp->dma_observer__DOT__count_q
                             [6U])) {
                            vlTOPp->dma_observer__DOT__count_n[6U] 
                                = vlTOPp->dma_observer__DOT__segment_n;
                        }
                        vlTOPp->dma_observer__DOT__segment_n = 0ULL;
                    }
                }
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU != (IData)(vlTOPp->cfg_wstrb_i))) {
            vlTOPp->dma_observer__DOT__error_n = (1U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        } else {
            if ((0x68U == (IData)(vlTOPp->cfg_addr_i))) {
                if ((1U & (~ ((~ (IData)(vlTOPp->dma_observer__DOT__active)) 
                              & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q)))))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (1U | vlTOPp->dma_observer__DOT__error_n);
                }
            } else {
                if ((0x70U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x22ULL <= vlTOPp->cfg_wdata_i)) {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (1U | vlTOPp->dma_observer__DOT__error_n);
                    }
                } else {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL == vlTOPp->cfg_wdata_i)) {
                            if (((IData)(vlTOPp->dma_observer__DOT__active) 
                                 | (1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)))) {
                                vlTOPp->dma_observer__DOT__phase_n = 5U;
                                vlTOPp->dma_observer__DOT__error_n 
                                    = (0x10U | vlTOPp->dma_observer__DOT__error_n);
                            }
                        } else {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if (((IData)(vlTOPp->dma_observer__DOT__active) 
                                     | (1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)))) {
                                    vlTOPp->dma_observer__DOT__error_n 
                                        = (1U | vlTOPp->dma_observer__DOT__error_n);
                                } else {
                                    vlTOPp->dma_observer__DOT__phase_n = 0U;
                                    vlTOPp->dma_observer__DOT__error_n = 0U;
                                }
                            } else {
                                if ((((((((0ULL == 
                                           (0xfffffffffefcfcfeULL 
                                            & vlTOPp->cfg_wdata_i)) 
                                          & (IData)(vlTOPp->cfg_wdata_i)) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__active))) 
                                        & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                       & (0ULL != vlTOPp->dma_observer__DOT__target_q)) 
                                      & (0x100000000ULL 
                                         >= vlTOPp->dma_observer__DOT__target_q)) 
                                     & ((~ (IData)(
                                                   (vlTOPp->cfg_wdata_i 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->cfg_wdata_i 
                                                             >> 0x10U))))))) {
                                    vlTOPp->dma_observer__DOT__phase_n = 1U;
                                    vlTOPp->dma_observer__DOT__error_n = 0U;
                                    vlTOPp->dma_observer__DOT__segment_n = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[1U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[2U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[3U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[4U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[5U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[6U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[7U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[8U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[9U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0xaU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0xbU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0xcU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0xdU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0xeU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0xfU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x10U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x11U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x12U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x13U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x14U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x15U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x16U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x17U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x18U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x19U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x1aU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x1bU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x1cU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x1dU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x1eU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x1fU] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x20U] = 0ULL;
                                    vlTOPp->dma_observer__DOT__count_n[0x21U] = 0ULL;
                                } else {
                                    vlTOPp->dma_observer__DOT__error_n 
                                        = (1U | vlTOPp->dma_observer__DOT__error_n);
                                }
                            }
                        }
                    } else {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (1U | vlTOPp->dma_observer__DOT__error_n);
                    }
                }
            }
        }
    }
}

void Vdma_observer::_eval_initial(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_eval_initial\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_observer::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::final\n"); );
    // Variables
    Vdma_observer__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_observer::_eval_settle(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_eval_settle\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vdma_observer::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    cfg_valid_i = VL_RAND_RESET_I(1);
    cfg_write_i = VL_RAND_RESET_I(1);
    cfg_addr_i = VL_RAND_RESET_I(7);
    cfg_wdata_i = VL_RAND_RESET_Q(64);
    cfg_wstrb_i = VL_RAND_RESET_I(8);
    cfg_rdata_o = VL_RAND_RESET_Q(64);
    issue_i = VL_RAND_RESET_I(4);
    done_i = VL_RAND_RESET_I(4);
    poll_i = VL_RAND_RESET_I(4);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            issue_id_i[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            done_id_i[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            poll_id_i[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    ar_valid_i = VL_RAND_RESET_I(4);
    aw_valid_i = VL_RAND_RESET_I(4);
    ar_ready_i = VL_RAND_RESET_I(4);
    aw_ready_i = VL_RAND_RESET_I(4);
    rlast_fire_i = VL_RAND_RESET_I(4);
    b_fire_i = VL_RAND_RESET_I(4);
    bad_id_i = VL_RAND_RESET_I(4);
    shared_ar_valid_i = VL_RAND_RESET_I(1);
    shared_aw_valid_i = VL_RAND_RESET_I(1);
    shared_ar_ready_i = VL_RAND_RESET_I(1);
    shared_aw_ready_i = VL_RAND_RESET_I(1);
    dma_observer__DOT__phase_q = VL_RAND_RESET_I(3);
    dma_observer__DOT__phase_n = VL_RAND_RESET_I(3);
    { int __Vi0=0; for (; __Vi0<34; ++__Vi0) {
            dma_observer__DOT__count_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<34; ++__Vi0) {
            dma_observer__DOT__count_n[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_observer__DOT__segment_q = VL_RAND_RESET_Q(64);
    dma_observer__DOT__segment_n = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            dma_observer__DOT__reads_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            dma_observer__DOT__reads_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            dma_observer__DOT__writes_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            dma_observer__DOT__writes_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    dma_observer__DOT__track_bad_q = VL_RAND_RESET_I(4);
    dma_observer__DOT__track_bad_n = VL_RAND_RESET_I(4);
    dma_observer__DOT__error_q = VL_RAND_RESET_I(32);
    dma_observer__DOT__error_n = VL_RAND_RESET_I(32);
    dma_observer__DOT__target_q = VL_RAND_RESET_Q(64);
    dma_observer__DOT__target_n = VL_RAND_RESET_Q(64);
    dma_observer__DOT__expected_q = VL_RAND_RESET_Q(64);
    dma_observer__DOT__expected_n = VL_RAND_RESET_Q(64);
    dma_observer__DOT__index_q = VL_RAND_RESET_I(6);
    dma_observer__DOT__index_n = VL_RAND_RESET_I(6);
    dma_observer__DOT__victim_q = VL_RAND_RESET_I(2);
    dma_observer__DOT__victim_n = VL_RAND_RESET_I(2);
    dma_observer__DOT__aggressor_q = VL_RAND_RESET_I(2);
    dma_observer__DOT__aggressor_n = VL_RAND_RESET_I(2);
    dma_observer__DOT__has_a_q = VL_RAND_RESET_I(1);
    dma_observer__DOT__has_a_n = VL_RAND_RESET_I(1);
    dma_observer__DOT__active = VL_RAND_RESET_I(1);
    dma_observer__DOT____Vlvbound1 = VL_RAND_RESET_Q(64);
    dma_observer__DOT____Vlvbound2 = VL_RAND_RESET_Q(64);
    dma_observer__DOT____Vlvbound4 = VL_RAND_RESET_Q(64);
    __Vtask_dma_observer__DOT__bump__0__slot = 0;
    __Vtask_dma_observer__DOT__bump__1__slot = 0;
    __Vtask_dma_observer__DOT__bump__2__slot = 0;
    __Vtask_dma_observer__DOT__bump__2__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__3__slot = 0;
    __Vtask_dma_observer__DOT__bump__3__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__4__slot = 0;
    __Vtask_dma_observer__DOT__bump__4__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__5__slot = 0;
    __Vtask_dma_observer__DOT__bump__5__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__6__slot = 0;
    __Vtask_dma_observer__DOT__bump__6__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__7__slot = 0;
    __Vtask_dma_observer__DOT__bump__7__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__8__slot = 0;
    __Vtask_dma_observer__DOT__bump__8__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__9__slot = 0;
    __Vtask_dma_observer__DOT__bump__9__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__10__slot = 0;
    __Vtask_dma_observer__DOT__bump__10__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__11__slot = 0;
    __Vtask_dma_observer__DOT__bump__11__enable = VL_RAND_RESET_I(1);
    __Vtask_dma_observer__DOT__bump__12__slot = 0;
    __Vtask_dma_observer__DOT__bump__13__slot = 0;
    __Vtask_dma_observer__DOT__bump__14__slot = 0;
    __Vtask_dma_observer__DOT__bump__15__slot = 0;
    __Vtask_dma_observer__DOT__bump__16__slot = 0;
    __Vtask_dma_observer__DOT__bump__17__slot = 0;
}
