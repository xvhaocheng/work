// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_policy.h for the primary calling header

#include "Vdma_research_policy.h"
#include "Vdma_research_policy__Syms.h"

void Vdma_research_policy::_settle__TOP__5(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_settle__TOP__5\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp257[3];
    WData/*95:0*/ __Vtemp258[3];
    WData/*95:0*/ __Vtemp259[3];
    WData/*95:0*/ __Vtemp260[3];
    WData/*95:0*/ __Vtemp261[3];
    WData/*95:0*/ __Vtemp262[3];
    WData/*95:0*/ __Vtemp264[3];
    WData/*95:0*/ __Vtemp265[3];
    WData/*95:0*/ __Vtemp266[3];
    WData/*95:0*/ __Vtemp267[3];
    WData/*95:0*/ __Vtemp268[3];
    WData/*95:0*/ __Vtemp270[3];
    WData/*95:0*/ __Vtemp271[3];
    WData/*95:0*/ __Vtemp272[3];
    WData/*95:0*/ __Vtemp273[3];
    WData/*95:0*/ __Vtemp275[3];
    WData/*95:0*/ __Vtemp276[3];
    WData/*95:0*/ __Vtemp277[3];
    WData/*95:0*/ __Vtemp278[3];
    WData/*95:0*/ __Vtemp279[3];
    WData/*95:0*/ __Vtemp280[3];
    WData/*95:0*/ __Vtemp281[3];
    WData/*95:0*/ __Vtemp283[3];
    WData/*95:0*/ __Vtemp284[3];
    WData/*95:0*/ __Vtemp285[3];
    WData/*95:0*/ __Vtemp288[3];
    WData/*95:0*/ __Vtemp289[3];
    WData/*95:0*/ __Vtemp293[3];
    WData/*95:0*/ __Vtemp294[3];
    WData/*95:0*/ __Vtemp295[3];
    WData/*95:0*/ __Vtemp299[3];
    WData/*95:0*/ __Vtemp300[3];
    WData/*95:0*/ __Vtemp301[3];
    WData/*95:0*/ __Vtemp302[3];
    WData/*95:0*/ __Vtemp304[3];
    WData/*95:0*/ __Vtemp305[3];
    WData/*95:0*/ __Vtemp306[3];
    WData/*95:0*/ __Vtemp310[3];
    WData/*95:0*/ __Vtemp311[3];
    WData/*95:0*/ __Vtemp312[3];
    WData/*95:0*/ __Vtemp315[3];
    WData/*95:0*/ __Vtemp316[3];
    WData/*95:0*/ __Vtemp317[3];
    WData/*95:0*/ __Vtemp318[3];
    WData/*95:0*/ __Vtemp319[3];
    WData/*95:0*/ __Vtemp320[3];
    WData/*95:0*/ __Vtemp321[3];
    WData/*95:0*/ __Vtemp322[3];
    WData/*95:0*/ __Vtemp323[3];
    WData/*95:0*/ __Vtemp324[3];
    WData/*95:0*/ __Vtemp325[3];
    WData/*95:0*/ __Vtemp326[3];
    WData/*95:0*/ __Vtemp327[3];
    WData/*95:0*/ __Vtemp328[3];
    WData/*95:0*/ __Vtemp329[3];
    WData/*95:0*/ __Vtemp330[3];
    WData/*95:0*/ __Vtemp331[3];
    WData/*95:0*/ __Vtemp332[3];
    WData/*95:0*/ __Vtemp333[3];
    WData/*95:0*/ __Vtemp334[3];
    WData/*95:0*/ __Vtemp335[3];
    WData/*95:0*/ __Vtemp336[3];
    WData/*95:0*/ __Vtemp337[3];
    WData/*95:0*/ __Vtemp338[3];
    WData/*95:0*/ __Vtemp339[3];
    WData/*95:0*/ __Vtemp340[3];
    WData/*95:0*/ __Vtemp341[3];
    WData/*95:0*/ __Vtemp342[3];
    WData/*95:0*/ __Vtemp343[3];
    WData/*95:0*/ __Vtemp344[3];
    WData/*95:0*/ __Vtemp345[3];
    WData/*95:0*/ __Vtemp346[3];
    WData/*95:0*/ __Vtemp347[3];
    WData/*95:0*/ __Vtemp348[3];
    WData/*95:0*/ __Vtemp349[3];
    WData/*95:0*/ __Vtemp350[3];
    WData/*95:0*/ __Vtemp351[3];
    WData/*95:0*/ __Vtemp352[3];
    WData/*95:0*/ __Vtemp353[3];
    WData/*95:0*/ __Vtemp354[3];
    WData/*95:0*/ __Vtemp355[3];
    WData/*95:0*/ __Vtemp356[3];
    WData/*95:0*/ __Vtemp357[3];
    WData/*95:0*/ __Vtemp358[3];
    WData/*95:0*/ __Vtemp359[3];
    WData/*95:0*/ __Vtemp360[3];
    WData/*95:0*/ __Vtemp361[3];
    WData/*95:0*/ __Vtemp362[3];
    WData/*95:0*/ __Vtemp363[3];
    WData/*95:0*/ __Vtemp364[3];
    WData/*95:0*/ __Vtemp365[3];
    WData/*95:0*/ __Vtemp366[3];
    WData/*95:0*/ __Vtemp367[3];
    WData/*95:0*/ __Vtemp368[3];
    WData/*95:0*/ __Vtemp369[3];
    WData/*95:0*/ __Vtemp370[3];
    WData/*95:0*/ __Vtemp371[3];
    WData/*95:0*/ __Vtemp372[3];
    WData/*95:0*/ __Vtemp373[3];
    WData/*95:0*/ __Vtemp374[3];
    WData/*95:0*/ __Vtemp375[3];
    WData/*95:0*/ __Vtemp376[3];
    WData/*95:0*/ __Vtemp377[3];
    WData/*95:0*/ __Vtemp378[3];
    WData/*95:0*/ __Vtemp379[3];
    WData/*95:0*/ __Vtemp380[3];
    WData/*95:0*/ __Vtemp381[3];
    WData/*95:0*/ __Vtemp382[3];
    WData/*95:0*/ __Vtemp383[3];
    WData/*95:0*/ __Vtemp384[3];
    WData/*95:0*/ __Vtemp385[3];
    WData/*95:0*/ __Vtemp386[3];
    WData/*95:0*/ __Vtemp387[3];
    WData/*95:0*/ __Vtemp388[3];
    WData/*95:0*/ __Vtemp389[3];
    WData/*95:0*/ __Vtemp390[3];
    WData/*95:0*/ __Vtemp391[3];
    WData/*95:0*/ __Vtemp392[3];
    WData/*95:0*/ __Vtemp393[3];
    WData/*95:0*/ __Vtemp394[3];
    WData/*95:0*/ __Vtemp395[3];
    WData/*95:0*/ __Vtemp396[3];
    WData/*95:0*/ __Vtemp397[3];
    WData/*95:0*/ __Vtemp398[3];
    WData/*95:0*/ __Vtemp399[3];
    WData/*95:0*/ __Vtemp400[3];
    WData/*95:0*/ __Vtemp401[3];
    WData/*95:0*/ __Vtemp402[3];
    WData/*95:0*/ __Vtemp403[3];
    WData/*95:0*/ __Vtemp404[3];
    WData/*95:0*/ __Vtemp405[3];
    WData/*95:0*/ __Vtemp406[3];
    WData/*95:0*/ __Vtemp407[3];
    WData/*95:0*/ __Vtemp408[3];
    WData/*95:0*/ __Vtemp409[3];
    WData/*95:0*/ __Vtemp410[3];
    WData/*95:0*/ __Vtemp411[3];
    WData/*95:0*/ __Vtemp412[3];
    WData/*95:0*/ __Vtemp413[3];
    WData/*95:0*/ __Vtemp414[3];
    WData/*95:0*/ __Vtemp415[3];
    WData/*95:0*/ __Vtemp416[3];
    WData/*95:0*/ __Vtemp417[3];
    WData/*95:0*/ __Vtemp418[3];
    WData/*95:0*/ __Vtemp419[3];
    WData/*95:0*/ __Vtemp420[3];
    WData/*95:0*/ __Vtemp421[3];
    WData/*95:0*/ __Vtemp422[3];
    WData/*95:0*/ __Vtemp423[3];
    WData/*95:0*/ __Vtemp424[3];
    WData/*95:0*/ __Vtemp425[3];
    WData/*95:0*/ __Vtemp426[3];
    WData/*95:0*/ __Vtemp427[3];
    WData/*95:0*/ __Vtemp428[3];
    WData/*95:0*/ __Vtemp429[3];
    WData/*95:0*/ __Vtemp430[3];
    // Body
    vlTOPp->dma_research_policy__DOT__win_run = ((IData)(vlTOPp->dma_research_policy__DOT__armed_q) 
                                                 & (~ (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)));
    vlTOPp->dma_research_policy__DOT__wr_ok = (((((IData)(vlTOPp->dma_research_policy__DOT__wr_any) 
                                                  & (0xeU 
                                                     == 
                                                     (0x1fU 
                                                      & ((IData)(vlTOPp->index_i) 
                                                         >> 8U)))) 
                                                 & (4U 
                                                    <= 
                                                    (0xffU 
                                                     & (IData)(vlTOPp->index_i)))) 
                                                & (0x10U 
                                                   >= 
                                                   (0xffU 
                                                    & (IData)(vlTOPp->index_i)))) 
                                               & (0U 
                                                  == (IData)(
                                                             (vlTOPp->wdata_i 
                                                              >> 0x20U))));
    vlTOPp->dma_research_policy__DOT__ev_lut_hit_a 
        = ((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_a) 
           & (IData)(vlTOPp->lookup_hit_i));
    vlTOPp->dma_research_policy__DOT__ev_lut_miss_a 
        = ((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_a) 
           & (~ (IData)(vlTOPp->lookup_hit_i)));
    VL_EXTEND_WQ(66,64, __Vtemp257, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp258, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp259, __Vtemp257, __Vtemp258);
    __Vtemp260[0U] = __Vtemp259[0U];
    __Vtemp260[1U] = __Vtemp259[1U];
    __Vtemp260[2U] = (3U & __Vtemp259[2U]);
    VL_EXTEND_WI(66,32, __Vtemp261, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WQ(66,64, __Vtemp262, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp264, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp265, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp266, __Vtemp264, __Vtemp265);
    __Vtemp267[0U] = __Vtemp266[0U];
    __Vtemp267[1U] = __Vtemp266[1U];
    __Vtemp267[2U] = (3U & __Vtemp266[2U]);
    vlTOPp->dma_research_policy__DOT__fb_inc = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_LTS_IWW(1,66,66, __Vtemp260, __Vtemp261)
                                                 : 
                                                VL_GTS_IWW(1,66,66, __Vtemp262, __Vtemp267));
    VL_EXTEND_WQ(66,64, __Vtemp268, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp270, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp271, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp272, __Vtemp270, __Vtemp271);
    __Vtemp273[0U] = __Vtemp272[0U];
    __Vtemp273[1U] = __Vtemp272[1U];
    __Vtemp273[2U] = (3U & __Vtemp272[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp275, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp276, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp277, __Vtemp275, __Vtemp276);
    __Vtemp278[0U] = __Vtemp277[0U];
    __Vtemp278[1U] = __Vtemp277[1U];
    __Vtemp278[2U] = (3U & __Vtemp277[2U]);
    VL_EXTEND_WI(66,32, __Vtemp279, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__fb_dec = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_GTS_IWW(1,66,66, __Vtemp268, __Vtemp273)
                                                 : 
                                                VL_LTS_IWW(1,66,66, __Vtemp278, __Vtemp279));
    vlTOPp->policy_enable_o = vlTOPp->dma_research_policy__DOT__win_run;
    vlTOPp->dma_research_policy__DOT__main_inc = ((
                                                   ((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                                                    & (7U 
                                                       == vlTOPp->dma_research_policy__DOT__mode_q)) 
                                                   & (IData)(vlTOPp->dma_research_policy__DOT__ev_actual_a))
                                                   ? 1ULL
                                                   : 0ULL);
    vlTOPp->dma_research_policy__DOT__nodone_run_next 
        = ((((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
             & (IData)(vlTOPp->v_active_i)) & (~ (IData)(vlTOPp->v_done_i)))
            ? (1ULL + vlTOPp->dma_research_policy__DOT__nodone_run_q)
            : 0ULL);
    vlTOPp->dma_research_policy__DOT__gap_load = 0U;
    if ((((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
          & (6U == vlTOPp->dma_research_policy__DOT__mode_q)) 
         & (IData)(vlTOPp->a_done_i))) {
        vlTOPp->dma_research_policy__DOT__gap_load = 1U;
    }
    vlTOPp->dma_research_policy__DOT__m6_cd_next = 
        (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
          & (0ULL != vlTOPp->dma_research_policy__DOT__m6_cd_q))
          ? (vlTOPp->dma_research_policy__DOT__m6_cd_q 
             - 1ULL) : vlTOPp->dma_research_policy__DOT__m6_cd_q);
    if ((((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
          & (6U == vlTOPp->dma_research_policy__DOT__mode_q)) 
         & (IData)(vlTOPp->a_done_i))) {
        vlTOPp->dma_research_policy__DOT__m6_cd_next 
            = vlTOPp->dma_research_policy__DOT__b_q;
    }
    vlTOPp->dma_research_policy__DOT__win_last = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                                                   & (0U 
                                                      != vlTOPp->dma_research_policy__DOT__window_q)) 
                                                  & (vlTOPp->dma_research_policy__DOT__win_cnt_q 
                                                     == 
                                                     ((QData)((IData)(vlTOPp->dma_research_policy__DOT__window_q)) 
                                                      - 1ULL)));
    vlTOPp->dma_research_policy__DOT__cfg_wr = 0U;
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U != (0xffU & (IData)(vlTOPp->index_i)))) {
            vlTOPp->dma_research_policy__DOT__cfg_wr = 1U;
        }
    }
    vlTOPp->dma_research_policy__DOT__do_clear = 0U;
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->index_i)))) {
            if ((1U != (IData)(vlTOPp->wdata_i))) {
                if ((2U != (IData)(vlTOPp->wdata_i))) {
                    if ((3U != (IData)(vlTOPp->wdata_i))) {
                        if ((4U == (IData)(vlTOPp->wdata_i))) {
                            if ((1U & (~ ((~ (IData)(vlTOPp->quiet_i)) 
                                          | (IData)(vlTOPp->dma_research_policy__DOT__armed_q))))) {
                                vlTOPp->dma_research_policy__DOT__do_clear = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_policy__DOT__do_freeze = 0U;
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->index_i)))) {
            if ((1U != (IData)(vlTOPp->wdata_i))) {
                if ((2U != (IData)(vlTOPp->wdata_i))) {
                    if ((3U == (IData)(vlTOPp->wdata_i))) {
                        if ((1U & (~ ((~ (IData)(vlTOPp->dma_research_policy__DOT__armed_q)) 
                                      | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q))))) {
                            if (vlTOPp->quiet_i) {
                                vlTOPp->dma_research_policy__DOT__do_freeze = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_policy__DOT__do_arm = 0U;
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->index_i)))) {
            if ((1U == (IData)(vlTOPp->wdata_i))) {
                if (vlTOPp->supported_i) {
                    if ((1U & (~ (((IData)(vlTOPp->dma_research_policy__DOT__armed_q) 
                                   | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)) 
                                  | (~ (IData)(vlTOPp->quiet_i)))))) {
                        if ((1U & (~ (IData)(vlTOPp->dma_research_policy__DOT__cfg_bad)))) {
                            vlTOPp->dma_research_policy__DOT__do_arm = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_policy__DOT__new_err = 0ULL;
    if (((IData)(vlTOPp->dma_research_policy__DOT__wr_any) 
         & (~ (IData)(vlTOPp->dma_research_policy__DOT__wr_ok)))) {
        vlTOPp->dma_research_policy__DOT__new_err = 
            (1ULL | vlTOPp->dma_research_policy__DOT__new_err);
    }
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->index_i)))) {
            if ((1U == (IData)(vlTOPp->wdata_i))) {
                if (vlTOPp->supported_i) {
                    if ((1U & (((IData)(vlTOPp->dma_research_policy__DOT__armed_q) 
                                | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)) 
                               | (~ (IData)(vlTOPp->quiet_i))))) {
                        vlTOPp->dma_research_policy__DOT__new_err 
                            = (0x100ULL | vlTOPp->dma_research_policy__DOT__new_err);
                    } else {
                        if (vlTOPp->dma_research_policy__DOT__cfg_bad) {
                            vlTOPp->dma_research_policy__DOT__new_err 
                                = (0x400ULL | vlTOPp->dma_research_policy__DOT__new_err);
                        }
                    }
                } else {
                    vlTOPp->dma_research_policy__DOT__new_err 
                        = (4ULL | vlTOPp->dma_research_policy__DOT__new_err);
                }
            } else {
                if ((2U == (IData)(vlTOPp->wdata_i))) {
                    if ((1U & ((~ (IData)(vlTOPp->dma_research_policy__DOT__armed_q)) 
                               | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)))) {
                        vlTOPp->dma_research_policy__DOT__new_err 
                            = (0x100ULL | vlTOPp->dma_research_policy__DOT__new_err);
                    } else {
                        if ((1U & (~ (IData)(vlTOPp->supported_i)))) {
                            vlTOPp->dma_research_policy__DOT__new_err 
                                = (4ULL | vlTOPp->dma_research_policy__DOT__new_err);
                        }
                    }
                } else {
                    if ((3U == (IData)(vlTOPp->wdata_i))) {
                        if ((1U & ((~ (IData)(vlTOPp->dma_research_policy__DOT__armed_q)) 
                                   | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)))) {
                            vlTOPp->dma_research_policy__DOT__new_err 
                                = (0x100ULL | vlTOPp->dma_research_policy__DOT__new_err);
                        } else {
                            if ((1U & (~ (IData)(vlTOPp->quiet_i)))) {
                                vlTOPp->dma_research_policy__DOT__new_err 
                                    = (0x80ULL | vlTOPp->dma_research_policy__DOT__new_err);
                            }
                        }
                    } else {
                        if ((4U == (IData)(vlTOPp->wdata_i))) {
                            if ((1U & ((~ (IData)(vlTOPp->quiet_i)) 
                                       | (IData)(vlTOPp->dma_research_policy__DOT__armed_q)))) {
                                vlTOPp->dma_research_policy__DOT__new_err 
                                    = (0x40ULL | vlTOPp->dma_research_policy__DOT__new_err);
                            }
                        } else {
                            vlTOPp->dma_research_policy__DOT__new_err 
                                = (2ULL | vlTOPp->dma_research_policy__DOT__new_err);
                        }
                    }
                }
            }
        } else {
            if (vlTOPp->dma_research_policy__DOT__armed_q) {
                vlTOPp->dma_research_policy__DOT__new_err 
                    = (8ULL | vlTOPp->dma_research_policy__DOT__new_err);
            }
        }
    }
    vlTOPp->dma_research_policy__DOT__do_abort = 0U;
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->index_i)))) {
            if ((1U != (IData)(vlTOPp->wdata_i))) {
                if ((2U == (IData)(vlTOPp->wdata_i))) {
                    if ((1U & (~ ((~ (IData)(vlTOPp->dma_research_policy__DOT__armed_q)) 
                                  | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q))))) {
                        if (vlTOPp->supported_i) {
                            vlTOPp->dma_research_policy__DOT__do_abort = 1U;
                        }
                    }
                }
            }
        }
    }
    VL_EXTEND_WI(65,32, __Vtemp280, vlTOPp->dma_research_policy__DOT__maxbud_q);
    VL_EXTEND_WI(65,32, __Vtemp281, vlTOPp->dma_research_policy__DOT__minbud_q);
    vlTOPp->dma_research_policy__DOT__b_next = ((IData)(vlTOPp->dma_research_policy__DOT__fb_inc)
                                                 ? 
                                                (VL_GTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_up_s, __Vtemp280)
                                                  ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__maxbud_q))
                                                  : 
                                                 (((QData)((IData)(
                                                                   vlTOPp->dma_research_policy__DOT__b_up_s[1U])) 
                                                   << 0x20U) 
                                                  | (QData)((IData)(
                                                                    vlTOPp->dma_research_policy__DOT__b_up_s[0U]))))
                                                 : 
                                                ((IData)(vlTOPp->dma_research_policy__DOT__fb_dec)
                                                  ? 
                                                 (VL_LTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_dn_s, __Vtemp281)
                                                   ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__minbud_q))
                                                   : 
                                                  (((QData)((IData)(
                                                                    vlTOPp->dma_research_policy__DOT__b_dn_s[1U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_research_policy__DOT__b_dn_s[0U]))))
                                                  : vlTOPp->dma_research_policy__DOT__b_q));
    vlTOPp->dma_research_policy__DOT__nodone_max_next 
        = ((vlTOPp->dma_research_policy__DOT__nodone_run_next 
            > vlTOPp->dma_research_policy__DOT__nodone_max_q)
            ? vlTOPp->dma_research_policy__DOT__nodone_run_next
            : vlTOPp->dma_research_policy__DOT__nodone_max_q);
    vlTOPp->dma_research_policy__DOT__win_complete 
        = ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
           & (IData)(vlTOPp->dma_research_policy__DOT__act_all_now));
    vlTOPp->dma_research_policy__DOT__refill_en = (
                                                   ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
                                                    & (0U 
                                                       != vlTOPp->dma_research_policy__DOT__mode_q)) 
                                                   & (6U 
                                                      != vlTOPp->dma_research_policy__DOT__mode_q));
    vlTOPp->dma_research_policy__DOT__stale_cyc = ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
                                                   & (7U 
                                                      == vlTOPp->dma_research_policy__DOT__mode_q));
    vlTOPp->dma_research_policy__DOT__abort_eff = ((IData)(vlTOPp->dma_research_policy__DOT__aborted_q) 
                                                   | (IData)(vlTOPp->dma_research_policy__DOT__do_abort));
    vlTOPp->dma_research_policy__DOT__hist_inc[0U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[1U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[2U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[3U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[4U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[5U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[6U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[7U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[8U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[9U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0xaU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0xbU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0xcU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0xdU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0xeU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0xfU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x10U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x11U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x12U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x13U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x14U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x15U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x16U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x17U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x18U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x19U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x1aU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x1bU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x1cU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x1dU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x1eU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__hist_inc[0x1fU] = 0ULL;
    if (vlTOPp->dma_research_policy__DOT__win_complete) {
        vlTOPp->dma_research_policy__DOT__hist_inc[(
                                                    (0x1fULL 
                                                     <= vlTOPp->dma_research_policy__DOT__vdone_now)
                                                     ? 0x1fU
                                                     : 
                                                    (0x1fU 
                                                     & (IData)(vlTOPp->dma_research_policy__DOT__vdone_now)))] = 1ULL;
    }
    vlTOPp->dma_research_policy__DOT__refill_now = 
        ((IData)(vlTOPp->dma_research_policy__DOT__refill_en)
          ? vlTOPp->dma_research_policy__DOT__b_q : 0ULL);
    vlTOPp->dma_research_policy__DOT__charge_now = 
        (((IData)(vlTOPp->dma_research_policy__DOT__win_run)
           ? ((0U == vlTOPp->dma_research_policy__DOT__mode_q)
               ? 0ULL : ((1U == vlTOPp->dma_research_policy__DOT__mode_q)
                          ? ((IData)(vlTOPp->a_issue_i)
                              ? 0x1000ULL : 0ULL) : 
                         ((2U == vlTOPp->dma_research_policy__DOT__mode_q)
                           ? ((IData)(vlTOPp->dma_research_policy__DOT__ev_sel_a)
                               ? 1ULL : 0ULL) : ((3U 
                                                  == vlTOPp->dma_research_policy__DOT__mode_q)
                                                  ? 
                                                 ((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_miss_a)
                                                   ? 1ULL
                                                   : 0ULL)
                                                  : 
                                                 ((4U 
                                                   == vlTOPp->dma_research_policy__DOT__mode_q)
                                                   ? 
                                                  ((((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_hit_a)
                                                      ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__whit_q))
                                                      : 0ULL) 
                                                    + 
                                                    ((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_miss_a)
                                                      ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__wmiss_q))
                                                      : 0ULL)) 
                                                   + 
                                                   ((IData)(vlTOPp->dma_research_policy__DOT__ev_ptw_a)
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__wptw_q))
                                                     : 0ULL))
                                                   : 
                                                  ((5U 
                                                    == vlTOPp->dma_research_policy__DOT__mode_q)
                                                    ? 
                                                   ((IData)(vlTOPp->dma_research_policy__DOT__ev_actual_a)
                                                     ? 1ULL
                                                     : 0ULL)
                                                    : 0ULL))))))
           : 0ULL) + ((IData)(vlTOPp->dma_research_policy__DOT__stale_cyc)
                       ? vlTOPp->dma_research_policy__DOT__prev_main_q
                       : 0ULL));
    __Vtemp283[0U] = (IData)(vlTOPp->dma_research_policy__DOT__balance_q);
    __Vtemp283[1U] = (IData)((vlTOPp->dma_research_policy__DOT__balance_q 
                              >> 0x20U));
    __Vtemp283[2U] = (3U & (- (IData)((1U & (IData)(
                                                    (vlTOPp->dma_research_policy__DOT__balance_q 
                                                     >> 0x3fU))))));
    VL_EXTEND_WQ(66,64, __Vtemp284, vlTOPp->dma_research_policy__DOT__charge_now);
    VL_SUB_W(3, __Vtemp285, __Vtemp283, __Vtemp284);
    vlTOPp->dma_research_policy__DOT__bal_c[0U] = __Vtemp285[0U];
    vlTOPp->dma_research_policy__DOT__bal_c[1U] = __Vtemp285[1U];
    vlTOPp->dma_research_policy__DOT__bal_c[2U] = (3U 
                                                   & __Vtemp285[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp288, vlTOPp->dma_research_policy__DOT__refill_now);
    VL_ADD_W(3, __Vtemp289, vlTOPp->dma_research_policy__DOT__bal_c, __Vtemp288);
    vlTOPp->dma_research_policy__DOT__bal_r[0U] = __Vtemp289[0U];
    vlTOPp->dma_research_policy__DOT__bal_r[1U] = __Vtemp289[1U];
    vlTOPp->dma_research_policy__DOT__bal_r[2U] = (3U 
                                                   & __Vtemp289[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp293, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    __Vtemp294[0U] = __Vtemp293[0U];
    __Vtemp294[1U] = __Vtemp293[1U];
    __Vtemp294[2U] = (3U & __Vtemp293[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp295, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    if (((IData)(vlTOPp->dma_research_policy__DOT__refill_en) 
         & VL_GTS_IWW(1,66,66, vlTOPp->dma_research_policy__DOT__bal_r, __Vtemp294))) {
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[0U] 
            = __Vtemp295[0U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
            = __Vtemp295[1U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
            = (3U & __Vtemp295[2U]);
    } else {
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[0U] 
            = vlTOPp->dma_research_policy__DOT__bal_r[0U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
            = vlTOPp->dma_research_policy__DOT__bal_r[1U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
            = (3U & vlTOPp->dma_research_policy__DOT__bal_r[2U]);
    }
    vlTOPp->dma_research_policy__DOT__bal_ovf = (((0U 
                                                   != 
                                                   (7U 
                                                    & ((vlTOPp->dma_research_policy__DOT__bal_c[2U] 
                                                        << 1U) 
                                                       | (vlTOPp->dma_research_policy__DOT__bal_c[1U] 
                                                          >> 0x1fU)))) 
                                                  & (7U 
                                                     != 
                                                     (7U 
                                                      & ((vlTOPp->dma_research_policy__DOT__bal_c[2U] 
                                                          << 1U) 
                                                         | (vlTOPp->dma_research_policy__DOT__bal_c[1U] 
                                                            >> 0x1fU))))) 
                                                 | ((0U 
                                                     != 
                                                     (7U 
                                                      & ((vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
                                                          << 1U) 
                                                         | (vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
                                                            >> 0x1fU)))) 
                                                    & (7U 
                                                       != 
                                                       (7U 
                                                        & ((vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
                                                            << 1U) 
                                                           | (vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
                                                              >> 0x1fU))))));
    vlTOPp->dma_research_policy__DOT__bal_next = ((IData)(vlTOPp->dma_research_policy__DOT__bal_ovf)
                                                   ? 
                                                  ((2U 
                                                    & vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U])
                                                    ? 0x8000000000000000ULL
                                                    : 0x7fffffffffffffffULL)
                                                   : 
                                                  (((QData)((IData)(
                                                                    vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_research_policy__DOT__bal_r_clamped[0U]))));
    vlTOPp->dma_research_policy__DOT__allow_core = 
        ((0U == vlTOPp->dma_research_policy__DOT__mode_q) 
         | ((6U == vlTOPp->dma_research_policy__DOT__mode_q)
             ? (0ULL == vlTOPp->dma_research_policy__DOT__m6_cd_next)
             : VL_LTES_IQQ(1,64,64, 0ULL, vlTOPp->dma_research_policy__DOT__bal_next)));
    vlTOPp->dma_research_policy__DOT__stat_inc[0U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[1U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[2U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[3U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[4U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[5U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[6U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[7U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[8U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[9U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xaU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xbU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xcU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xdU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xeU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xfU] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x10U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x11U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x12U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x13U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x14U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x15U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x16U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x17U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x18U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x19U] = 0ULL;
    vlTOPp->dma_research_policy__DOT__stat_inc[0x19U] 
        = ((IData)(vlTOPp->dma_research_policy__DOT__win_run)
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[5U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (((IData)(vlTOPp->dma_research_policy__DOT__abort_eff) 
                | (0ULL != vlTOPp->dma_research_policy__DOT__err_q)) 
               | (~ (IData)(vlTOPp->dma_research_policy__DOT__allow_core))))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[6U] 
        = vlTOPp->dma_research_policy__DOT__charge_now;
    vlTOPp->dma_research_policy__DOT__stat_inc[0xaU] 
        = ((0ULL != vlTOPp->dma_research_policy__DOT__charge_now)
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[7U] 
        = vlTOPp->dma_research_policy__DOT__refill_now;
    if (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
         & (IData)(vlTOPp->v_done_i))) {
        vlTOPp->dma_research_policy__DOT__stat_inc[1U] = 1ULL;
        vlTOPp->dma_research_policy__DOT__stat_inc[2U] = 0x1000ULL;
    } else {
        vlTOPp->dma_research_policy__DOT__stat_inc[1U] = 0ULL;
        vlTOPp->dma_research_policy__DOT__stat_inc[2U] = 0ULL;
    }
    vlTOPp->dma_research_policy__DOT__stat_inc[8U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->a_issue_i)) ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[9U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->a_done_i)) ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0xbU] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_lut_hit_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0xcU] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_lut_miss_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0xdU] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_ptw_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0xeU] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (0xbU == vlTOPp->ctl_did_i)) ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0xfU] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_sel_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x10U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_lut_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x11U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_lut_miss_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x12U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
            & (IData)(vlTOPp->dma_research_policy__DOT__ev_actual_a))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0U] 
        = ((IData)(vlTOPp->dma_research_policy__DOT__win_complete)
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x13U] 
        = ((IData)(vlTOPp->dma_research_policy__DOT__win_last)
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x14U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
            & (~ (IData)(vlTOPp->dma_research_policy__DOT__act_all_now)))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x17U] 
        = ((IData)(vlTOPp->dma_research_policy__DOT__gap_load)
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x18U] 
        = ((IData)(vlTOPp->dma_research_policy__DOT__stale_cyc)
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp299, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp300, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[3U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp299, __Vtemp300))
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp301, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp302, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[4U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp301, __Vtemp302))
            ? ((QData)((IData)(vlTOPp->dma_research_policy__DOT__target_q)) 
               - (vlTOPp->dma_research_policy__DOT__vdone_now 
                  << 0xcU)) : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x15U] 
        = ((((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
             & (0U != vlTOPp->dma_research_policy__DOT__mode_q)) 
            & (IData)(vlTOPp->dma_research_policy__DOT__fb_inc))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__stat_inc[0x16U] 
        = ((((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
             & (0U != vlTOPp->dma_research_policy__DOT__mode_q)) 
            & (IData)(vlTOPp->dma_research_policy__DOT__fb_dec))
            ? 1ULL : 0ULL);
    vlTOPp->dma_research_policy__DOT__cnt_ovf = 0U;
    vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x1aU, vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)) {
        if ((0x19U >= (0x1fU & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i))) {
            vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
                = vlTOPp->dma_research_policy__DOT__stat_inc
                [(0x1fU & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)];
            vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
                = vlTOPp->dma_research_policy__DOT__stat_q
                [(0x1fU & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)];
        } else {
            vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc = 0ULL;
            vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c = 0ULL;
        }
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 1U))))) 
                  << 1U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 2U))))) 
                  << 2U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 3U))))) 
                  << 3U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 4U))))) 
                  << 4U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 5U))))) 
                  << 5U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 6U))))) 
                  << 6U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 7U))))) 
                  << 7U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffeffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 8U))))) 
                  << 8U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffdffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 9U))))) 
                  << 9U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffbffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0xaU))))) 
                  << 0xaU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffff7ffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0xbU))))) 
                  << 0xbU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffefffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0xcU))))) 
                  << 0xcU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffdfffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0xdU))))) 
                  << 0xdU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffffbfffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0xeU))))) 
                  << 0xeU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffff7fffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0xfU))))) 
                  << 0xfU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffeffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x10U))))) 
                  << 0x10U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffdffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x11U))))) 
                  << 0x11U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffffbffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x12U))))) 
                  << 0x12U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffff7ffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x13U))))) 
                  << 0x13U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffefffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x14U))))) 
                  << 0x14U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffdfffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x15U))))) 
                  << 0x15U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffffbfffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x16U))))) 
                  << 0x16U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffff7fffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x17U))))) 
                  << 0x17U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffeffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x18U))))) 
                  << 0x18U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffdffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x19U))))) 
                  << 0x19U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffffbffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x1aU))))) 
                  << 0x1aU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffff7ffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x1bU))))) 
                  << 0x1bU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffefffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x1cU))))) 
                  << 0x1cU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffdfffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x1dU))))) 
                  << 0x1dU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffffbfffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x1eU))))) 
                  << 0x1eU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffff7fffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x1fU))))) 
                  << 0x1fU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffeffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x20U))))) 
                  << 0x20U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffdffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x21U))))) 
                  << 0x21U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffffbffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x22U))))) 
                  << 0x22U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffff7ffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x23U))))) 
                  << 0x23U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffefffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x24U))))) 
                  << 0x24U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffdfffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x25U))))) 
                  << 0x25U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffffbfffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x26U))))) 
                  << 0x26U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffff7fffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x27U))))) 
                  << 0x27U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffeffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x28U))))) 
                  << 0x28U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffdffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x29U))))) 
                  << 0x29U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffffbffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x2aU))))) 
                  << 0x2aU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffff7ffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x2bU))))) 
                  << 0x2bU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffefffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x2cU))))) 
                  << 0x2cU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffdfffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x2dU))))) 
                  << 0x2dU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffffbfffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x2eU))))) 
                  << 0x2eU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffff7fffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x2fU))))) 
                  << 0x2fU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffeffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x30U))))) 
                  << 0x30U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffdffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x31U))))) 
                  << 0x31U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfffbffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x32U))))) 
                  << 0x32U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfff7ffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x33U))))) 
                  << 0x33U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffefffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x34U))))) 
                  << 0x34U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffdfffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x35U))))) 
                  << 0x35U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xffbfffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x36U))))) 
                  << 0x36U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xff7fffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x37U))))) 
                  << 0x37U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfeffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x38U))))) 
                  << 0x38U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfdffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x39U))))) 
                  << 0x39U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xfbffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x3aU))))) 
                  << 0x3aU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xf7ffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x3bU))))) 
                  << 0x3bU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xefffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x3cU))))) 
                  << 0x3cU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xdfffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x3dU))))) 
                  << 0x3dU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0xbfffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x3eU))))) 
                  << 0x3eU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
            = ((0x7fffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
                                                 >> 0x3fU))))) 
                  << 0x3fU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
        VL_EXTEND_WQ(65,64, __Vtemp304, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp305, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
        VL_ADD_W(3, __Vtemp306, __Vtemp304, __Vtemp305);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
            = __Vtemp306[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
            = __Vtemp306[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
            = (1U & __Vtemp306[2U]);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U];
        vlTOPp->dma_research_policy__DOT____Vlvbound2[0U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[0U];
        vlTOPp->dma_research_policy__DOT____Vlvbound2[1U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[1U];
        vlTOPp->dma_research_policy__DOT____Vlvbound2[2U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout[2U];
        if (VL_LIKELY((0x19U >= (0x1fU & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)))) {
            vlTOPp->dma_research_policy__DOT__stat_sum[(0x1fU 
                                                        & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)][0U] 
                = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
            vlTOPp->dma_research_policy__DOT__stat_sum[(0x1fU 
                                                        & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)][1U] 
                = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
            vlTOPp->dma_research_policy__DOT__stat_sum[(0x1fU 
                                                        & vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i)][2U] 
                = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
        }
        vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i 
            = ((IData)(1U) + vlTOPp->dma_research_policy__DOT__unnamedblk4__DOT__i);
    }
    vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x20U, vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i)) {
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
            = vlTOPp->dma_research_policy__DOT__hist_inc
            [(0x1fU & vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i)];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
            = vlTOPp->dma_research_policy__DOT__hist_q
            [(0x1fU & vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i)];
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 1U))))) 
                  << 1U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 2U))))) 
                  << 2U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 3U))))) 
                  << 3U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 4U))))) 
                  << 4U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 5U))))) 
                  << 5U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 6U))))) 
                  << 6U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 7U))))) 
                  << 7U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffeffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 8U))))) 
                  << 8U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffdffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 9U))))) 
                  << 9U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffbffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0xaU))))) 
                  << 0xaU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffff7ffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0xbU))))) 
                  << 0xbU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffefffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0xcU))))) 
                  << 0xcU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffdfffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0xdU))))) 
                  << 0xdU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffffbfffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0xeU))))) 
                  << 0xeU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffff7fffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0xfU))))) 
                  << 0xfU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffeffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x10U))))) 
                  << 0x10U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffdffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x11U))))) 
                  << 0x11U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffffbffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x12U))))) 
                  << 0x12U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffff7ffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x13U))))) 
                  << 0x13U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffefffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x14U))))) 
                  << 0x14U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffdfffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x15U))))) 
                  << 0x15U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffffbfffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x16U))))) 
                  << 0x16U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffff7fffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x17U))))) 
                  << 0x17U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffeffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x18U))))) 
                  << 0x18U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffdffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x19U))))) 
                  << 0x19U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffffbffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x1aU))))) 
                  << 0x1aU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffff7ffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x1bU))))) 
                  << 0x1bU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffefffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x1cU))))) 
                  << 0x1cU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffdfffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x1dU))))) 
                  << 0x1dU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffffbfffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x1eU))))) 
                  << 0x1eU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffff7fffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x1fU))))) 
                  << 0x1fU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffeffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x20U))))) 
                  << 0x20U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffdffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x21U))))) 
                  << 0x21U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffffbffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x22U))))) 
                  << 0x22U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffff7ffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x23U))))) 
                  << 0x23U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffefffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x24U))))) 
                  << 0x24U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffdfffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x25U))))) 
                  << 0x25U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffffbfffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x26U))))) 
                  << 0x26U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffff7fffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x27U))))) 
                  << 0x27U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffeffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x28U))))) 
                  << 0x28U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffdffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x29U))))) 
                  << 0x29U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffffbffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x2aU))))) 
                  << 0x2aU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffff7ffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x2bU))))) 
                  << 0x2bU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffefffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x2cU))))) 
                  << 0x2cU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffdfffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x2dU))))) 
                  << 0x2dU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffffbfffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x2eU))))) 
                  << 0x2eU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffff7fffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x2fU))))) 
                  << 0x2fU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffeffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x30U))))) 
                  << 0x30U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffdffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x31U))))) 
                  << 0x31U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfffbffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x32U))))) 
                  << 0x32U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfff7ffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x33U))))) 
                  << 0x33U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffefffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x34U))))) 
                  << 0x34U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffdfffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x35U))))) 
                  << 0x35U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xffbfffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x36U))))) 
                  << 0x36U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xff7fffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x37U))))) 
                  << 0x37U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfeffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x38U))))) 
                  << 0x38U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfdffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x39U))))) 
                  << 0x39U));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xfbffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x3aU))))) 
                  << 0x3aU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xf7ffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x3bU))))) 
                  << 0x3bU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xefffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x3cU))))) 
                  << 0x3cU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xdfffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x3dU))))) 
                  << 0x3dU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0xbfffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x3eU))))) 
                  << 0x3eU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
            = ((0x7fffffffffffffffULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
                                                 >> 0x3fU))))) 
                  << 0x3fU));
        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
        VL_EXTEND_WQ(65,64, __Vtemp310, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp311, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
        VL_ADD_W(3, __Vtemp312, __Vtemp310, __Vtemp311);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
            = __Vtemp312[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
            = __Vtemp312[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
            = (1U & __Vtemp312[2U]);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
        vlTOPp->dma_research_policy__DOT__hist_sum[(0x1fU 
                                                    & vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i)][0U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
        vlTOPp->dma_research_policy__DOT__hist_sum[(0x1fU 
                                                    & vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i)][1U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
        vlTOPp->dma_research_policy__DOT__hist_sum[(0x1fU 
                                                    & vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i)][2U] 
            = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
        vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i 
            = ((IData)(1U) + vlTOPp->dma_research_policy__DOT__unnamedblk5__DOT__i);
    }
    __Vtemp315[0U] = 0xffffffffU;
    __Vtemp315[1U] = 0xffffffffU;
    __Vtemp315[2U] = 0U;
    __Vtemp316[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp316[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp316[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp315, __Vtemp316)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp317[0U] = 0xffffffffU;
    __Vtemp317[1U] = 0xffffffffU;
    __Vtemp317[2U] = 0U;
    __Vtemp318[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp318[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp318[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp317, __Vtemp318)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp319[0U] = 0xffffffffU;
    __Vtemp319[1U] = 0xffffffffU;
    __Vtemp319[2U] = 0U;
    __Vtemp320[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp320[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp320[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp319, __Vtemp320)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp321[0U] = 0xffffffffU;
    __Vtemp321[1U] = 0xffffffffU;
    __Vtemp321[2U] = 0U;
    __Vtemp322[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp322[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp322[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp321, __Vtemp322)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp323[0U] = 0xffffffffU;
    __Vtemp323[1U] = 0xffffffffU;
    __Vtemp323[2U] = 0U;
    __Vtemp324[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp324[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp324[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp323, __Vtemp324)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp325[0U] = 0xffffffffU;
    __Vtemp325[1U] = 0xffffffffU;
    __Vtemp325[2U] = 0U;
    __Vtemp326[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp326[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp326[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp325, __Vtemp326)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp327[0U] = 0xffffffffU;
    __Vtemp327[1U] = 0xffffffffU;
    __Vtemp327[2U] = 0U;
    __Vtemp328[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp328[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp328[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp327, __Vtemp328)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp329[0U] = 0xffffffffU;
    __Vtemp329[1U] = 0xffffffffU;
    __Vtemp329[2U] = 0U;
    __Vtemp330[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp330[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp330[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp329, __Vtemp330)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp331[0U] = 0xffffffffU;
    __Vtemp331[1U] = 0xffffffffU;
    __Vtemp331[2U] = 0U;
    __Vtemp332[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp332[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp332[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp331, __Vtemp332)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp333[0U] = 0xffffffffU;
    __Vtemp333[1U] = 0xffffffffU;
    __Vtemp333[2U] = 0U;
    __Vtemp334[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp334[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp334[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp333, __Vtemp334)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp335[0U] = 0xffffffffU;
    __Vtemp335[1U] = 0xffffffffU;
    __Vtemp335[2U] = 0U;
    __Vtemp336[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp336[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp336[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp335, __Vtemp336)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp337[0U] = 0xffffffffU;
    __Vtemp337[1U] = 0xffffffffU;
    __Vtemp337[2U] = 0U;
    __Vtemp338[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp338[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp338[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp337, __Vtemp338)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp339[0U] = 0xffffffffU;
    __Vtemp339[1U] = 0xffffffffU;
    __Vtemp339[2U] = 0U;
    __Vtemp340[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp340[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp340[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp339, __Vtemp340)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp341[0U] = 0xffffffffU;
    __Vtemp341[1U] = 0xffffffffU;
    __Vtemp341[2U] = 0U;
    __Vtemp342[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp342[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp342[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp341, __Vtemp342)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp343[0U] = 0xffffffffU;
    __Vtemp343[1U] = 0xffffffffU;
    __Vtemp343[2U] = 0U;
    __Vtemp344[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp344[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp344[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp343, __Vtemp344)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp345[0U] = 0xffffffffU;
    __Vtemp345[1U] = 0xffffffffU;
    __Vtemp345[2U] = 0U;
    __Vtemp346[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp346[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp346[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp345, __Vtemp346)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp347[0U] = 0xffffffffU;
    __Vtemp347[1U] = 0xffffffffU;
    __Vtemp347[2U] = 0U;
    __Vtemp348[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp348[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp348[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp347, __Vtemp348)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp349[0U] = 0xffffffffU;
    __Vtemp349[1U] = 0xffffffffU;
    __Vtemp349[2U] = 0U;
    __Vtemp350[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp350[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp350[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp349, __Vtemp350)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp351[0U] = 0xffffffffU;
    __Vtemp351[1U] = 0xffffffffU;
    __Vtemp351[2U] = 0U;
    __Vtemp352[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp352[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp352[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp351, __Vtemp352)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp353[0U] = 0xffffffffU;
    __Vtemp353[1U] = 0xffffffffU;
    __Vtemp353[2U] = 0U;
    __Vtemp354[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp354[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp354[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp353, __Vtemp354)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp355[0U] = 0xffffffffU;
    __Vtemp355[1U] = 0xffffffffU;
    __Vtemp355[2U] = 0U;
    __Vtemp356[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp356[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp356[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp355, __Vtemp356)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp357[0U] = 0xffffffffU;
    __Vtemp357[1U] = 0xffffffffU;
    __Vtemp357[2U] = 0U;
    __Vtemp358[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp358[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp358[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp357, __Vtemp358)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp359[0U] = 0xffffffffU;
    __Vtemp359[1U] = 0xffffffffU;
    __Vtemp359[2U] = 0U;
    __Vtemp360[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp360[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp360[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp359, __Vtemp360)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp361[0U] = 0xffffffffU;
    __Vtemp361[1U] = 0xffffffffU;
    __Vtemp361[2U] = 0U;
    __Vtemp362[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp362[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp362[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp361, __Vtemp362)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp363[0U] = 0xffffffffU;
    __Vtemp363[1U] = 0xffffffffU;
    __Vtemp363[2U] = 0U;
    __Vtemp364[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp364[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp364[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp363, __Vtemp364)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp365[0U] = 0xffffffffU;
    __Vtemp365[1U] = 0xffffffffU;
    __Vtemp365[2U] = 0U;
    __Vtemp366[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp366[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp366[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp365, __Vtemp366)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp367[0U] = 0xffffffffU;
    __Vtemp367[1U] = 0xffffffffU;
    __Vtemp367[2U] = 0U;
    __Vtemp368[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp368[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp368[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp367, __Vtemp368)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp369[0U] = 0xffffffffU;
    __Vtemp369[1U] = 0xffffffffU;
    __Vtemp369[2U] = 0U;
    __Vtemp370[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp370[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp370[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp369, __Vtemp370)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp371[0U] = 0xffffffffU;
    __Vtemp371[1U] = 0xffffffffU;
    __Vtemp371[2U] = 0U;
    __Vtemp372[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp372[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp372[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp371, __Vtemp372)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp373[0U] = 0xffffffffU;
    __Vtemp373[1U] = 0xffffffffU;
    __Vtemp373[2U] = 0U;
    __Vtemp374[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp374[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp374[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp373, __Vtemp374)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp375[0U] = 0xffffffffU;
    __Vtemp375[1U] = 0xffffffffU;
    __Vtemp375[2U] = 0U;
    __Vtemp376[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp376[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp376[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp375, __Vtemp376)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp377[0U] = 0xffffffffU;
    __Vtemp377[1U] = 0xffffffffU;
    __Vtemp377[2U] = 0U;
    __Vtemp378[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp378[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp378[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp377, __Vtemp378)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp379[0U] = 0xffffffffU;
    __Vtemp379[1U] = 0xffffffffU;
    __Vtemp379[2U] = 0U;
    __Vtemp380[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp380[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp380[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp379, __Vtemp380)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp381[0U] = 0xffffffffU;
    __Vtemp381[1U] = 0xffffffffU;
    __Vtemp381[2U] = 0U;
    __Vtemp382[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp382[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp382[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp381, __Vtemp382)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp383[0U] = 0xffffffffU;
    __Vtemp383[1U] = 0xffffffffU;
    __Vtemp383[2U] = 0U;
    __Vtemp384[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp384[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp384[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp383, __Vtemp384)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp385[0U] = 0xffffffffU;
    __Vtemp385[1U] = 0xffffffffU;
    __Vtemp385[2U] = 0U;
    __Vtemp386[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp386[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp386[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp385, __Vtemp386)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp387[0U] = 0xffffffffU;
    __Vtemp387[1U] = 0xffffffffU;
    __Vtemp387[2U] = 0U;
    __Vtemp388[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp388[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp388[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp387, __Vtemp388)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp389[0U] = 0xffffffffU;
    __Vtemp389[1U] = 0xffffffffU;
    __Vtemp389[2U] = 0U;
    __Vtemp390[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp390[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp390[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp389, __Vtemp390)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp391[0U] = 0xffffffffU;
    __Vtemp391[1U] = 0xffffffffU;
    __Vtemp391[2U] = 0U;
    __Vtemp392[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp392[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp392[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp391, __Vtemp392)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp393[0U] = 0xffffffffU;
    __Vtemp393[1U] = 0xffffffffU;
    __Vtemp393[2U] = 0U;
    __Vtemp394[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp394[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp394[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp393, __Vtemp394)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp395[0U] = 0xffffffffU;
    __Vtemp395[1U] = 0xffffffffU;
    __Vtemp395[2U] = 0U;
    __Vtemp396[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp396[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp396[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp395, __Vtemp396)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp397[0U] = 0xffffffffU;
    __Vtemp397[1U] = 0xffffffffU;
    __Vtemp397[2U] = 0U;
    __Vtemp398[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp398[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp398[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp397, __Vtemp398)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp399[0U] = 0xffffffffU;
    __Vtemp399[1U] = 0xffffffffU;
    __Vtemp399[2U] = 0U;
    __Vtemp400[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp400[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp400[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp399, __Vtemp400)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp401[0U] = 0xffffffffU;
    __Vtemp401[1U] = 0xffffffffU;
    __Vtemp401[2U] = 0U;
    __Vtemp402[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp402[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp402[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp401, __Vtemp402)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp403[0U] = 0xffffffffU;
    __Vtemp403[1U] = 0xffffffffU;
    __Vtemp403[2U] = 0U;
    __Vtemp404[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp404[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp404[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp403, __Vtemp404)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp405[0U] = 0xffffffffU;
    __Vtemp405[1U] = 0xffffffffU;
    __Vtemp405[2U] = 0U;
    __Vtemp406[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp406[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp406[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp405, __Vtemp406)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp407[0U] = 0xffffffffU;
    __Vtemp407[1U] = 0xffffffffU;
    __Vtemp407[2U] = 0U;
    __Vtemp408[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp408[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp408[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp407, __Vtemp408)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp409[0U] = 0xffffffffU;
    __Vtemp409[1U] = 0xffffffffU;
    __Vtemp409[2U] = 0U;
    __Vtemp410[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp410[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp410[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp409, __Vtemp410)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp411[0U] = 0xffffffffU;
    __Vtemp411[1U] = 0xffffffffU;
    __Vtemp411[2U] = 0U;
    __Vtemp412[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp412[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp412[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp411, __Vtemp412)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp413[0U] = 0xffffffffU;
    __Vtemp413[1U] = 0xffffffffU;
    __Vtemp413[2U] = 0U;
    __Vtemp414[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp414[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp414[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp413, __Vtemp414)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp415[0U] = 0xffffffffU;
    __Vtemp415[1U] = 0xffffffffU;
    __Vtemp415[2U] = 0U;
    __Vtemp416[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp416[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp416[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp415, __Vtemp416)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp417[0U] = 0xffffffffU;
    __Vtemp417[1U] = 0xffffffffU;
    __Vtemp417[2U] = 0U;
    __Vtemp418[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp418[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp418[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp417, __Vtemp418)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp419[0U] = 0xffffffffU;
    __Vtemp419[1U] = 0xffffffffU;
    __Vtemp419[2U] = 0U;
    __Vtemp420[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp420[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp420[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp419, __Vtemp420)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp421[0U] = 0xffffffffU;
    __Vtemp421[1U] = 0xffffffffU;
    __Vtemp421[2U] = 0U;
    __Vtemp422[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp422[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp422[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp421, __Vtemp422)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp423[0U] = 0xffffffffU;
    __Vtemp423[1U] = 0xffffffffU;
    __Vtemp423[2U] = 0U;
    __Vtemp424[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp424[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp424[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp423, __Vtemp424)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp425[0U] = 0xffffffffU;
    __Vtemp425[1U] = 0xffffffffU;
    __Vtemp425[2U] = 0U;
    __Vtemp426[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp426[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp426[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp425, __Vtemp426)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp427[0U] = 0xffffffffU;
    __Vtemp427[1U] = 0xffffffffU;
    __Vtemp427[2U] = 0U;
    __Vtemp428[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp428[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp428[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp427, __Vtemp428)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp429[0U] = 0xffffffffU;
    __Vtemp429[1U] = 0xffffffffU;
    __Vtemp429[2U] = 0U;
    __Vtemp430[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp430[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp430[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp429, __Vtemp430)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    vlTOPp->dma_research_policy__DOT__block_all = (
                                                   (0ULL 
                                                    != vlTOPp->dma_research_policy__DOT__err_q) 
                                                   | (((((0ULL 
                                                          != vlTOPp->dma_research_policy__DOT__new_err) 
                                                         | (IData)(vlTOPp->dma_research_policy__DOT__obs_err_now)) 
                                                        | (IData)(vlTOPp->dma_research_policy__DOT__sup_drop_now)) 
                                                       | (IData)(vlTOPp->dma_research_policy__DOT__cnt_ovf)) 
                                                      | (IData)(vlTOPp->dma_research_policy__DOT__bal_ovf)));
    vlTOPp->policy_abort_o = ((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                              & ((IData)(vlTOPp->dma_research_policy__DOT__abort_eff) 
                                 | (IData)(vlTOPp->dma_research_policy__DOT__block_all)));
    vlTOPp->policy_allow_o = ((((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                                & (~ (IData)(vlTOPp->dma_research_policy__DOT__abort_eff))) 
                               & (~ (IData)(vlTOPp->dma_research_policy__DOT__block_all))) 
                              & (IData)(vlTOPp->dma_research_policy__DOT__allow_core));
}

void Vdma_research_policy::_eval_initial(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval_initial\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_research_policy::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::final\n"); );
    // Variables
    Vdma_research_policy__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_research_policy::_eval_settle(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval_settle\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__4(vlSymsp);
    vlTOPp->_settle__TOP__5(vlSymsp);
}

void Vdma_research_policy::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    cfg_valid_i = VL_RAND_RESET_I(1);
    cfg_write_i = VL_RAND_RESET_I(1);
    index_i = VL_RAND_RESET_I(13);
    wdata_i = VL_RAND_RESET_Q(64);
    rdata_o = VL_RAND_RESET_Q(64);
    quiet_i = VL_RAND_RESET_I(1);
    supported_i = VL_RAND_RESET_I(1);
    v_active_i = VL_RAND_RESET_I(1);
    a_issue_i = VL_RAND_RESET_I(1);
    a_done_i = VL_RAND_RESET_I(1);
    v_done_i = VL_RAND_RESET_I(1);
    ctl_state_i = VL_RAND_RESET_I(3);
    ctl_did_i = VL_RAND_RESET_I(24);
    select_fire_i = VL_RAND_RESET_I(1);
    select_did_i = VL_RAND_RESET_I(24);
    lookup_i = VL_RAND_RESET_I(1);
    lookup_hit_i = VL_RAND_RESET_I(1);
    lookup_did_i = VL_RAND_RESET_I(24);
    ptw_r_fire_i = VL_RAND_RESET_I(1);
    response_error_i = VL_RAND_RESET_I(1);
    policy_enable_o = VL_RAND_RESET_I(1);
    policy_allow_o = VL_RAND_RESET_I(1);
    policy_abort_o = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__armed_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__frozen_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__aborted_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__err_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__mode_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__window_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__target_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__deadband_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__initbud_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__minbud_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__maxbud_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__step_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__cap_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__whit_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__wmiss_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__wptw_q = VL_RAND_RESET_I(32);
    dma_research_policy__DOT__balance_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__b_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__bmin_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__bmax_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__prev_main_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_main_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_cnt_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_vdone_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__win_allact_q = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__nodone_run_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__nodone_max_q = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__m6_cd_q = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            dma_research_policy__DOT__stat_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            dma_research_policy__DOT__hist_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            dma_research_policy__DOT__stat_inc[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            VL_RAND_RESET_W(65, dma_research_policy__DOT__stat_sum[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            dma_research_policy__DOT__hist_inc[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            VL_RAND_RESET_W(65, dma_research_policy__DOT__hist_sum[__Vi0]);
    }}
    dma_research_policy__DOT__ev_sel_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_lut_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_lut_hit_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_lut_miss_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_ptw_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__ev_actual_a = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__win_run = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__win_last = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__act_all_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__win_complete = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__vdone_now = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__charge_now = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__refill_now = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__main_inc = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__stale_cyc = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__refill_en = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(66, dma_research_policy__DOT__bal_c);
    VL_RAND_RESET_W(66, dma_research_policy__DOT__bal_r);
    VL_RAND_RESET_W(66, dma_research_policy__DOT__bal_r_clamped);
    dma_research_policy__DOT__bal_ovf = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__bal_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__fb_inc = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__fb_dec = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__b_next = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, dma_research_policy__DOT__b_up_s);
    VL_RAND_RESET_W(65, dma_research_policy__DOT__b_dn_s);
    dma_research_policy__DOT__m6_cd_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__gap_load = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cnt_ovf = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__wr_any = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__wr_ok = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__obs_err_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__sup_drop_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__new_err = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__do_arm = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_abort = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_freeze = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_clear = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cfg_wr = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cfg_bad = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__allow_core = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__abort_eff = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__block_all = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__nodone_run_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__nodone_max_next = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__unnamedblk4__DOT__i = 0;
    dma_research_policy__DOT__unnamedblk5__DOT__i = 0;
    VL_RAND_RESET_W(65, dma_research_policy__DOT____Vlvbound2);
    dma_research_policy__DOT____Vlvbound3 = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__0__Vfuncout);
    __Vfunc_dma_research_policy__DOT__cadd__0__c = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__cadd__0__inc = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__0__w);
    __Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__1__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__1__r = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout);
    __Vfunc_dma_research_policy__DOT__cadd__2__c = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__cadd__2__inc = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_dma_research_policy__DOT__cadd__2__w);
    __Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__3__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__3__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__4__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__4__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__4__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__5__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__5__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__5__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__6__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__6__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__6__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__7__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__7__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__7__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__8__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__8__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__8__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__9__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__9__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__9__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__10__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__10__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__10__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__11__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__11__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__11__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__12__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__12__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__12__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__13__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__13__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__13__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__14__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__14__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__14__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__15__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__15__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__15__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__16__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__16__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__16__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__17__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__17__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__17__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__18__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__18__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__18__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__19__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__19__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__19__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__20__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__20__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__20__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__21__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__21__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__21__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__22__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__22__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__22__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__23__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__23__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__23__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__24__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__24__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__24__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__25__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__25__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__25__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__26__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__26__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__26__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__27__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__27__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__27__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__28__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__28__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__28__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__29__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__29__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__29__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__30__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__30__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__30__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__31__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__31__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__31__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__32__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__32__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__32__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__33__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__33__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__33__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__34__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__34__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__34__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__35__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__35__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__35__r = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__36__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__36__v = VL_RAND_RESET_Q(64);
    __Vfunc_dma_research_policy__DOT__zext64__36__r = VL_RAND_RESET_Q(64);
}
