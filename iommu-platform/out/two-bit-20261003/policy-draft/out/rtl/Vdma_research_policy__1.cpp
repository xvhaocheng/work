// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_policy.h for the primary calling header

#include "Vdma_research_policy.h"
#include "Vdma_research_policy__Syms.h"

VL_INLINE_OPT void Vdma_research_policy::_combo__TOP__3(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_combo__TOP__3\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp72[3];
    WData/*95:0*/ __Vtemp73[3];
    WData/*95:0*/ __Vtemp74[3];
    WData/*95:0*/ __Vtemp75[3];
    WData/*95:0*/ __Vtemp76[3];
    WData/*95:0*/ __Vtemp77[3];
    WData/*95:0*/ __Vtemp79[3];
    WData/*95:0*/ __Vtemp80[3];
    WData/*95:0*/ __Vtemp81[3];
    WData/*95:0*/ __Vtemp82[3];
    WData/*95:0*/ __Vtemp83[3];
    WData/*95:0*/ __Vtemp85[3];
    WData/*95:0*/ __Vtemp86[3];
    WData/*95:0*/ __Vtemp87[3];
    WData/*95:0*/ __Vtemp88[3];
    WData/*95:0*/ __Vtemp90[3];
    WData/*95:0*/ __Vtemp91[3];
    WData/*95:0*/ __Vtemp92[3];
    WData/*95:0*/ __Vtemp93[3];
    WData/*95:0*/ __Vtemp94[3];
    WData/*95:0*/ __Vtemp95[3];
    WData/*95:0*/ __Vtemp96[3];
    WData/*95:0*/ __Vtemp98[3];
    WData/*95:0*/ __Vtemp99[3];
    WData/*95:0*/ __Vtemp100[3];
    WData/*95:0*/ __Vtemp103[3];
    WData/*95:0*/ __Vtemp104[3];
    WData/*95:0*/ __Vtemp108[3];
    WData/*95:0*/ __Vtemp109[3];
    WData/*95:0*/ __Vtemp110[3];
    WData/*95:0*/ __Vtemp114[3];
    WData/*95:0*/ __Vtemp115[3];
    WData/*95:0*/ __Vtemp116[3];
    WData/*95:0*/ __Vtemp117[3];
    WData/*95:0*/ __Vtemp119[3];
    WData/*95:0*/ __Vtemp120[3];
    WData/*95:0*/ __Vtemp121[3];
    WData/*95:0*/ __Vtemp125[3];
    WData/*95:0*/ __Vtemp126[3];
    WData/*95:0*/ __Vtemp127[3];
    WData/*95:0*/ __Vtemp130[3];
    WData/*95:0*/ __Vtemp131[3];
    WData/*95:0*/ __Vtemp132[3];
    WData/*95:0*/ __Vtemp133[3];
    WData/*95:0*/ __Vtemp134[3];
    WData/*95:0*/ __Vtemp135[3];
    WData/*95:0*/ __Vtemp136[3];
    WData/*95:0*/ __Vtemp137[3];
    WData/*95:0*/ __Vtemp138[3];
    WData/*95:0*/ __Vtemp139[3];
    WData/*95:0*/ __Vtemp140[3];
    WData/*95:0*/ __Vtemp141[3];
    WData/*95:0*/ __Vtemp142[3];
    WData/*95:0*/ __Vtemp143[3];
    WData/*95:0*/ __Vtemp144[3];
    WData/*95:0*/ __Vtemp145[3];
    WData/*95:0*/ __Vtemp146[3];
    WData/*95:0*/ __Vtemp147[3];
    WData/*95:0*/ __Vtemp148[3];
    WData/*95:0*/ __Vtemp149[3];
    WData/*95:0*/ __Vtemp150[3];
    WData/*95:0*/ __Vtemp151[3];
    WData/*95:0*/ __Vtemp152[3];
    WData/*95:0*/ __Vtemp153[3];
    WData/*95:0*/ __Vtemp154[3];
    WData/*95:0*/ __Vtemp155[3];
    WData/*95:0*/ __Vtemp156[3];
    WData/*95:0*/ __Vtemp157[3];
    WData/*95:0*/ __Vtemp158[3];
    WData/*95:0*/ __Vtemp159[3];
    WData/*95:0*/ __Vtemp160[3];
    WData/*95:0*/ __Vtemp161[3];
    WData/*95:0*/ __Vtemp162[3];
    WData/*95:0*/ __Vtemp163[3];
    WData/*95:0*/ __Vtemp164[3];
    WData/*95:0*/ __Vtemp165[3];
    WData/*95:0*/ __Vtemp166[3];
    WData/*95:0*/ __Vtemp167[3];
    WData/*95:0*/ __Vtemp168[3];
    WData/*95:0*/ __Vtemp169[3];
    WData/*95:0*/ __Vtemp170[3];
    WData/*95:0*/ __Vtemp171[3];
    WData/*95:0*/ __Vtemp172[3];
    WData/*95:0*/ __Vtemp173[3];
    WData/*95:0*/ __Vtemp174[3];
    WData/*95:0*/ __Vtemp175[3];
    WData/*95:0*/ __Vtemp176[3];
    WData/*95:0*/ __Vtemp177[3];
    WData/*95:0*/ __Vtemp178[3];
    WData/*95:0*/ __Vtemp179[3];
    WData/*95:0*/ __Vtemp180[3];
    WData/*95:0*/ __Vtemp181[3];
    WData/*95:0*/ __Vtemp182[3];
    WData/*95:0*/ __Vtemp183[3];
    WData/*95:0*/ __Vtemp184[3];
    WData/*95:0*/ __Vtemp185[3];
    WData/*95:0*/ __Vtemp186[3];
    WData/*95:0*/ __Vtemp187[3];
    WData/*95:0*/ __Vtemp188[3];
    WData/*95:0*/ __Vtemp189[3];
    WData/*95:0*/ __Vtemp190[3];
    WData/*95:0*/ __Vtemp191[3];
    WData/*95:0*/ __Vtemp192[3];
    WData/*95:0*/ __Vtemp193[3];
    WData/*95:0*/ __Vtemp194[3];
    WData/*95:0*/ __Vtemp195[3];
    WData/*95:0*/ __Vtemp196[3];
    WData/*95:0*/ __Vtemp197[3];
    WData/*95:0*/ __Vtemp198[3];
    WData/*95:0*/ __Vtemp199[3];
    WData/*95:0*/ __Vtemp200[3];
    WData/*95:0*/ __Vtemp201[3];
    WData/*95:0*/ __Vtemp202[3];
    WData/*95:0*/ __Vtemp203[3];
    WData/*95:0*/ __Vtemp204[3];
    WData/*95:0*/ __Vtemp205[3];
    WData/*95:0*/ __Vtemp206[3];
    WData/*95:0*/ __Vtemp207[3];
    WData/*95:0*/ __Vtemp208[3];
    WData/*95:0*/ __Vtemp209[3];
    WData/*95:0*/ __Vtemp210[3];
    WData/*95:0*/ __Vtemp211[3];
    WData/*95:0*/ __Vtemp212[3];
    WData/*95:0*/ __Vtemp213[3];
    WData/*95:0*/ __Vtemp214[3];
    WData/*95:0*/ __Vtemp215[3];
    WData/*95:0*/ __Vtemp216[3];
    WData/*95:0*/ __Vtemp217[3];
    WData/*95:0*/ __Vtemp218[3];
    WData/*95:0*/ __Vtemp219[3];
    WData/*95:0*/ __Vtemp220[3];
    WData/*95:0*/ __Vtemp221[3];
    WData/*95:0*/ __Vtemp222[3];
    WData/*95:0*/ __Vtemp223[3];
    WData/*95:0*/ __Vtemp224[3];
    WData/*95:0*/ __Vtemp225[3];
    WData/*95:0*/ __Vtemp226[3];
    WData/*95:0*/ __Vtemp227[3];
    WData/*95:0*/ __Vtemp228[3];
    WData/*95:0*/ __Vtemp229[3];
    WData/*95:0*/ __Vtemp230[3];
    WData/*95:0*/ __Vtemp231[3];
    WData/*95:0*/ __Vtemp232[3];
    WData/*95:0*/ __Vtemp233[3];
    WData/*95:0*/ __Vtemp234[3];
    WData/*95:0*/ __Vtemp235[3];
    WData/*95:0*/ __Vtemp236[3];
    WData/*95:0*/ __Vtemp237[3];
    WData/*95:0*/ __Vtemp238[3];
    WData/*95:0*/ __Vtemp239[3];
    WData/*95:0*/ __Vtemp240[3];
    WData/*95:0*/ __Vtemp241[3];
    WData/*95:0*/ __Vtemp242[3];
    WData/*95:0*/ __Vtemp243[3];
    WData/*95:0*/ __Vtemp244[3];
    WData/*95:0*/ __Vtemp245[3];
    // Body
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
    vlTOPp->dma_research_policy__DOT__main_inc = ((
                                                   ((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                                                    & (7U 
                                                       == vlTOPp->dma_research_policy__DOT__mode_q)) 
                                                   & (IData)(vlTOPp->dma_research_policy__DOT__ev_actual_a))
                                                   ? 1ULL
                                                   : 0ULL);
    vlTOPp->dma_research_policy__DOT__ev_lut_hit_a 
        = ((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_a) 
           & (IData)(vlTOPp->lookup_hit_i));
    vlTOPp->dma_research_policy__DOT__ev_lut_miss_a 
        = ((IData)(vlTOPp->dma_research_policy__DOT__ev_lut_a) 
           & (~ (IData)(vlTOPp->lookup_hit_i)));
    vlTOPp->dma_research_policy__DOT__win_complete 
        = ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
           & (IData)(vlTOPp->dma_research_policy__DOT__act_all_now));
    VL_EXTEND_WQ(66,64, __Vtemp72, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp73, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp74, __Vtemp72, __Vtemp73);
    __Vtemp75[0U] = __Vtemp74[0U];
    __Vtemp75[1U] = __Vtemp74[1U];
    __Vtemp75[2U] = (3U & __Vtemp74[2U]);
    VL_EXTEND_WI(66,32, __Vtemp76, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WQ(66,64, __Vtemp77, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp79, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp80, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp81, __Vtemp79, __Vtemp80);
    __Vtemp82[0U] = __Vtemp81[0U];
    __Vtemp82[1U] = __Vtemp81[1U];
    __Vtemp82[2U] = (3U & __Vtemp81[2U]);
    vlTOPp->dma_research_policy__DOT__fb_inc = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_LTS_IWW(1,66,66, __Vtemp75, __Vtemp76)
                                                 : 
                                                VL_GTS_IWW(1,66,66, __Vtemp77, __Vtemp82));
    VL_EXTEND_WQ(66,64, __Vtemp83, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp85, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp86, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp87, __Vtemp85, __Vtemp86);
    __Vtemp88[0U] = __Vtemp87[0U];
    __Vtemp88[1U] = __Vtemp87[1U];
    __Vtemp88[2U] = (3U & __Vtemp87[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp90, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp91, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp92, __Vtemp90, __Vtemp91);
    __Vtemp93[0U] = __Vtemp92[0U];
    __Vtemp93[1U] = __Vtemp92[1U];
    __Vtemp93[2U] = (3U & __Vtemp92[2U]);
    VL_EXTEND_WI(66,32, __Vtemp94, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__fb_dec = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_GTS_IWW(1,66,66, __Vtemp83, __Vtemp88)
                                                 : 
                                                VL_LTS_IWW(1,66,66, __Vtemp93, __Vtemp94));
    vlTOPp->dma_research_policy__DOT__nodone_max_next 
        = ((vlTOPp->dma_research_policy__DOT__nodone_run_next 
            > vlTOPp->dma_research_policy__DOT__nodone_max_q)
            ? vlTOPp->dma_research_policy__DOT__nodone_run_next
            : vlTOPp->dma_research_policy__DOT__nodone_max_q);
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
    VL_EXTEND_WI(65,32, __Vtemp95, vlTOPp->dma_research_policy__DOT__maxbud_q);
    VL_EXTEND_WI(65,32, __Vtemp96, vlTOPp->dma_research_policy__DOT__minbud_q);
    vlTOPp->dma_research_policy__DOT__b_next = ((IData)(vlTOPp->dma_research_policy__DOT__fb_inc)
                                                 ? 
                                                (VL_GTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_up_s, __Vtemp95)
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
                                                 (VL_LTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_dn_s, __Vtemp96)
                                                   ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__minbud_q))
                                                   : 
                                                  (((QData)((IData)(
                                                                    vlTOPp->dma_research_policy__DOT__b_dn_s[1U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_research_policy__DOT__b_dn_s[0U]))))
                                                  : vlTOPp->dma_research_policy__DOT__b_q));
    vlTOPp->dma_research_policy__DOT__abort_eff = ((IData)(vlTOPp->dma_research_policy__DOT__aborted_q) 
                                                   | (IData)(vlTOPp->dma_research_policy__DOT__do_abort));
    __Vtemp98[0U] = (IData)(vlTOPp->dma_research_policy__DOT__balance_q);
    __Vtemp98[1U] = (IData)((vlTOPp->dma_research_policy__DOT__balance_q 
                             >> 0x20U));
    __Vtemp98[2U] = (3U & (- (IData)((1U & (IData)(
                                                   (vlTOPp->dma_research_policy__DOT__balance_q 
                                                    >> 0x3fU))))));
    VL_EXTEND_WQ(66,64, __Vtemp99, vlTOPp->dma_research_policy__DOT__charge_now);
    VL_SUB_W(3, __Vtemp100, __Vtemp98, __Vtemp99);
    vlTOPp->dma_research_policy__DOT__bal_c[0U] = __Vtemp100[0U];
    vlTOPp->dma_research_policy__DOT__bal_c[1U] = __Vtemp100[1U];
    vlTOPp->dma_research_policy__DOT__bal_c[2U] = (3U 
                                                   & __Vtemp100[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp103, vlTOPp->dma_research_policy__DOT__refill_now);
    VL_ADD_W(3, __Vtemp104, vlTOPp->dma_research_policy__DOT__bal_c, __Vtemp103);
    vlTOPp->dma_research_policy__DOT__bal_r[0U] = __Vtemp104[0U];
    vlTOPp->dma_research_policy__DOT__bal_r[1U] = __Vtemp104[1U];
    vlTOPp->dma_research_policy__DOT__bal_r[2U] = (3U 
                                                   & __Vtemp104[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp108, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    __Vtemp109[0U] = __Vtemp108[0U];
    __Vtemp109[1U] = __Vtemp108[1U];
    __Vtemp109[2U] = (3U & __Vtemp108[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp110, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    if (((IData)(vlTOPp->dma_research_policy__DOT__refill_en) 
         & VL_GTS_IWW(1,66,66, vlTOPp->dma_research_policy__DOT__bal_r, __Vtemp109))) {
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[0U] 
            = __Vtemp110[0U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
            = __Vtemp110[1U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
            = (3U & __Vtemp110[2U]);
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
    VL_EXTEND_WQ(66,64, __Vtemp114, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp115, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[3U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp114, __Vtemp115))
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp116, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp117, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[4U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp116, __Vtemp117))
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
        VL_EXTEND_WQ(65,64, __Vtemp119, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp120, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
        VL_ADD_W(3, __Vtemp121, __Vtemp119, __Vtemp120);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
            = __Vtemp121[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
            = __Vtemp121[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
            = (1U & __Vtemp121[2U]);
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
        VL_EXTEND_WQ(65,64, __Vtemp125, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp126, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
        VL_ADD_W(3, __Vtemp127, __Vtemp125, __Vtemp126);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
            = __Vtemp127[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
            = __Vtemp127[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
            = (1U & __Vtemp127[2U]);
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
    __Vtemp130[0U] = 0xffffffffU;
    __Vtemp130[1U] = 0xffffffffU;
    __Vtemp130[2U] = 0U;
    __Vtemp131[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp131[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp131[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp130, __Vtemp131)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp132[0U] = 0xffffffffU;
    __Vtemp132[1U] = 0xffffffffU;
    __Vtemp132[2U] = 0U;
    __Vtemp133[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp133[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp133[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp132, __Vtemp133)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp134[0U] = 0xffffffffU;
    __Vtemp134[1U] = 0xffffffffU;
    __Vtemp134[2U] = 0U;
    __Vtemp135[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp135[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp135[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp134, __Vtemp135)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp136[0U] = 0xffffffffU;
    __Vtemp136[1U] = 0xffffffffU;
    __Vtemp136[2U] = 0U;
    __Vtemp137[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp137[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp137[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp136, __Vtemp137)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp138[0U] = 0xffffffffU;
    __Vtemp138[1U] = 0xffffffffU;
    __Vtemp138[2U] = 0U;
    __Vtemp139[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp139[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp139[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp138, __Vtemp139)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp140[0U] = 0xffffffffU;
    __Vtemp140[1U] = 0xffffffffU;
    __Vtemp140[2U] = 0U;
    __Vtemp141[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp141[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp141[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp140, __Vtemp141)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp142[0U] = 0xffffffffU;
    __Vtemp142[1U] = 0xffffffffU;
    __Vtemp142[2U] = 0U;
    __Vtemp143[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp143[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp143[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp142, __Vtemp143)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp144[0U] = 0xffffffffU;
    __Vtemp144[1U] = 0xffffffffU;
    __Vtemp144[2U] = 0U;
    __Vtemp145[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp145[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp145[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp144, __Vtemp145)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp146[0U] = 0xffffffffU;
    __Vtemp146[1U] = 0xffffffffU;
    __Vtemp146[2U] = 0U;
    __Vtemp147[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp147[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp147[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp146, __Vtemp147)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp148[0U] = 0xffffffffU;
    __Vtemp148[1U] = 0xffffffffU;
    __Vtemp148[2U] = 0U;
    __Vtemp149[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp149[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp149[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp148, __Vtemp149)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp150[0U] = 0xffffffffU;
    __Vtemp150[1U] = 0xffffffffU;
    __Vtemp150[2U] = 0U;
    __Vtemp151[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp151[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp151[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp150, __Vtemp151)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp152[0U] = 0xffffffffU;
    __Vtemp152[1U] = 0xffffffffU;
    __Vtemp152[2U] = 0U;
    __Vtemp153[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp153[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp153[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp152, __Vtemp153)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp154[0U] = 0xffffffffU;
    __Vtemp154[1U] = 0xffffffffU;
    __Vtemp154[2U] = 0U;
    __Vtemp155[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp155[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp155[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp154, __Vtemp155)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp156[0U] = 0xffffffffU;
    __Vtemp156[1U] = 0xffffffffU;
    __Vtemp156[2U] = 0U;
    __Vtemp157[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp157[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp157[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp156, __Vtemp157)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp158[0U] = 0xffffffffU;
    __Vtemp158[1U] = 0xffffffffU;
    __Vtemp158[2U] = 0U;
    __Vtemp159[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp159[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp159[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp158, __Vtemp159)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp160[0U] = 0xffffffffU;
    __Vtemp160[1U] = 0xffffffffU;
    __Vtemp160[2U] = 0U;
    __Vtemp161[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp161[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp161[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp160, __Vtemp161)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp162[0U] = 0xffffffffU;
    __Vtemp162[1U] = 0xffffffffU;
    __Vtemp162[2U] = 0U;
    __Vtemp163[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp163[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp163[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp162, __Vtemp163)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp164[0U] = 0xffffffffU;
    __Vtemp164[1U] = 0xffffffffU;
    __Vtemp164[2U] = 0U;
    __Vtemp165[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp165[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp165[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp164, __Vtemp165)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp166[0U] = 0xffffffffU;
    __Vtemp166[1U] = 0xffffffffU;
    __Vtemp166[2U] = 0U;
    __Vtemp167[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp167[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp167[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp166, __Vtemp167)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp168[0U] = 0xffffffffU;
    __Vtemp168[1U] = 0xffffffffU;
    __Vtemp168[2U] = 0U;
    __Vtemp169[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp169[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp169[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp168, __Vtemp169)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp170[0U] = 0xffffffffU;
    __Vtemp170[1U] = 0xffffffffU;
    __Vtemp170[2U] = 0U;
    __Vtemp171[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp171[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp171[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp170, __Vtemp171)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp172[0U] = 0xffffffffU;
    __Vtemp172[1U] = 0xffffffffU;
    __Vtemp172[2U] = 0U;
    __Vtemp173[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp173[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp173[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp172, __Vtemp173)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp174[0U] = 0xffffffffU;
    __Vtemp174[1U] = 0xffffffffU;
    __Vtemp174[2U] = 0U;
    __Vtemp175[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp175[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp175[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp174, __Vtemp175)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp176[0U] = 0xffffffffU;
    __Vtemp176[1U] = 0xffffffffU;
    __Vtemp176[2U] = 0U;
    __Vtemp177[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp177[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp177[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp176, __Vtemp177)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp178[0U] = 0xffffffffU;
    __Vtemp178[1U] = 0xffffffffU;
    __Vtemp178[2U] = 0U;
    __Vtemp179[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp179[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp179[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp178, __Vtemp179)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp180[0U] = 0xffffffffU;
    __Vtemp180[1U] = 0xffffffffU;
    __Vtemp180[2U] = 0U;
    __Vtemp181[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp181[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp181[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp180, __Vtemp181)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp182[0U] = 0xffffffffU;
    __Vtemp182[1U] = 0xffffffffU;
    __Vtemp182[2U] = 0U;
    __Vtemp183[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp183[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp183[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp182, __Vtemp183)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp184[0U] = 0xffffffffU;
    __Vtemp184[1U] = 0xffffffffU;
    __Vtemp184[2U] = 0U;
    __Vtemp185[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp185[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp185[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp184, __Vtemp185)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp186[0U] = 0xffffffffU;
    __Vtemp186[1U] = 0xffffffffU;
    __Vtemp186[2U] = 0U;
    __Vtemp187[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp187[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp187[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp186, __Vtemp187)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp188[0U] = 0xffffffffU;
    __Vtemp188[1U] = 0xffffffffU;
    __Vtemp188[2U] = 0U;
    __Vtemp189[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp189[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp189[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp188, __Vtemp189)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp190[0U] = 0xffffffffU;
    __Vtemp190[1U] = 0xffffffffU;
    __Vtemp190[2U] = 0U;
    __Vtemp191[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp191[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp191[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp190, __Vtemp191)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp192[0U] = 0xffffffffU;
    __Vtemp192[1U] = 0xffffffffU;
    __Vtemp192[2U] = 0U;
    __Vtemp193[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp193[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp193[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp192, __Vtemp193)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp194[0U] = 0xffffffffU;
    __Vtemp194[1U] = 0xffffffffU;
    __Vtemp194[2U] = 0U;
    __Vtemp195[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp195[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp195[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp194, __Vtemp195)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp196[0U] = 0xffffffffU;
    __Vtemp196[1U] = 0xffffffffU;
    __Vtemp196[2U] = 0U;
    __Vtemp197[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp197[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp197[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp196, __Vtemp197)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp198[0U] = 0xffffffffU;
    __Vtemp198[1U] = 0xffffffffU;
    __Vtemp198[2U] = 0U;
    __Vtemp199[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp199[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp199[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp198, __Vtemp199)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp200[0U] = 0xffffffffU;
    __Vtemp200[1U] = 0xffffffffU;
    __Vtemp200[2U] = 0U;
    __Vtemp201[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp201[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp201[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp200, __Vtemp201)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp202[0U] = 0xffffffffU;
    __Vtemp202[1U] = 0xffffffffU;
    __Vtemp202[2U] = 0U;
    __Vtemp203[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp203[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp203[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp202, __Vtemp203)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp204[0U] = 0xffffffffU;
    __Vtemp204[1U] = 0xffffffffU;
    __Vtemp204[2U] = 0U;
    __Vtemp205[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp205[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp205[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp204, __Vtemp205)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp206[0U] = 0xffffffffU;
    __Vtemp206[1U] = 0xffffffffU;
    __Vtemp206[2U] = 0U;
    __Vtemp207[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp207[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp207[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp206, __Vtemp207)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp208[0U] = 0xffffffffU;
    __Vtemp208[1U] = 0xffffffffU;
    __Vtemp208[2U] = 0U;
    __Vtemp209[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp209[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp209[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp208, __Vtemp209)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp210[0U] = 0xffffffffU;
    __Vtemp210[1U] = 0xffffffffU;
    __Vtemp210[2U] = 0U;
    __Vtemp211[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp211[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp211[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp210, __Vtemp211)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp212[0U] = 0xffffffffU;
    __Vtemp212[1U] = 0xffffffffU;
    __Vtemp212[2U] = 0U;
    __Vtemp213[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp213[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp213[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp212, __Vtemp213)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp214[0U] = 0xffffffffU;
    __Vtemp214[1U] = 0xffffffffU;
    __Vtemp214[2U] = 0U;
    __Vtemp215[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp215[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp215[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp214, __Vtemp215)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp216[0U] = 0xffffffffU;
    __Vtemp216[1U] = 0xffffffffU;
    __Vtemp216[2U] = 0U;
    __Vtemp217[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp217[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp217[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp216, __Vtemp217)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp218[0U] = 0xffffffffU;
    __Vtemp218[1U] = 0xffffffffU;
    __Vtemp218[2U] = 0U;
    __Vtemp219[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp219[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp219[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp218, __Vtemp219)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp220[0U] = 0xffffffffU;
    __Vtemp220[1U] = 0xffffffffU;
    __Vtemp220[2U] = 0U;
    __Vtemp221[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp221[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp221[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp220, __Vtemp221)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp222[0U] = 0xffffffffU;
    __Vtemp222[1U] = 0xffffffffU;
    __Vtemp222[2U] = 0U;
    __Vtemp223[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp223[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp223[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp222, __Vtemp223)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp224[0U] = 0xffffffffU;
    __Vtemp224[1U] = 0xffffffffU;
    __Vtemp224[2U] = 0U;
    __Vtemp225[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp225[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp225[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp224, __Vtemp225)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp226[0U] = 0xffffffffU;
    __Vtemp226[1U] = 0xffffffffU;
    __Vtemp226[2U] = 0U;
    __Vtemp227[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp227[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp227[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp226, __Vtemp227)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp228[0U] = 0xffffffffU;
    __Vtemp228[1U] = 0xffffffffU;
    __Vtemp228[2U] = 0U;
    __Vtemp229[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp229[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp229[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp228, __Vtemp229)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp230[0U] = 0xffffffffU;
    __Vtemp230[1U] = 0xffffffffU;
    __Vtemp230[2U] = 0U;
    __Vtemp231[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp231[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp231[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp230, __Vtemp231)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp232[0U] = 0xffffffffU;
    __Vtemp232[1U] = 0xffffffffU;
    __Vtemp232[2U] = 0U;
    __Vtemp233[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp233[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp233[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp232, __Vtemp233)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp234[0U] = 0xffffffffU;
    __Vtemp234[1U] = 0xffffffffU;
    __Vtemp234[2U] = 0U;
    __Vtemp235[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp235[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp235[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp234, __Vtemp235)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp236[0U] = 0xffffffffU;
    __Vtemp236[1U] = 0xffffffffU;
    __Vtemp236[2U] = 0U;
    __Vtemp237[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp237[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp237[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp236, __Vtemp237)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp238[0U] = 0xffffffffU;
    __Vtemp238[1U] = 0xffffffffU;
    __Vtemp238[2U] = 0U;
    __Vtemp239[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp239[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp239[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp238, __Vtemp239)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp240[0U] = 0xffffffffU;
    __Vtemp240[1U] = 0xffffffffU;
    __Vtemp240[2U] = 0U;
    __Vtemp241[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp241[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp241[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp240, __Vtemp241)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp242[0U] = 0xffffffffU;
    __Vtemp242[1U] = 0xffffffffU;
    __Vtemp242[2U] = 0U;
    __Vtemp243[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp243[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp243[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp242, __Vtemp243)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp244[0U] = 0xffffffffU;
    __Vtemp244[1U] = 0xffffffffU;
    __Vtemp244[2U] = 0U;
    __Vtemp245[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp245[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp245[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp244, __Vtemp245)) {
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

void Vdma_research_policy::_eval(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__2(vlSymsp);
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vdma_research_policy::_change_request(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_change_request\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_research_policy::_change_request_1(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_change_request_1\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_research_policy::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((cfg_valid_i & 0xfeU))) {
        Verilated::overWidthError("cfg_valid_i");}
    if (VL_UNLIKELY((cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("cfg_write_i");}
    if (VL_UNLIKELY((index_i & 0xe000U))) {
        Verilated::overWidthError("index_i");}
    if (VL_UNLIKELY((quiet_i & 0xfeU))) {
        Verilated::overWidthError("quiet_i");}
    if (VL_UNLIKELY((supported_i & 0xfeU))) {
        Verilated::overWidthError("supported_i");}
    if (VL_UNLIKELY((v_active_i & 0xfeU))) {
        Verilated::overWidthError("v_active_i");}
    if (VL_UNLIKELY((a_issue_i & 0xfeU))) {
        Verilated::overWidthError("a_issue_i");}
    if (VL_UNLIKELY((a_done_i & 0xfeU))) {
        Verilated::overWidthError("a_done_i");}
    if (VL_UNLIKELY((v_done_i & 0xfeU))) {
        Verilated::overWidthError("v_done_i");}
    if (VL_UNLIKELY((ctl_state_i & 0xf8U))) {
        Verilated::overWidthError("ctl_state_i");}
    if (VL_UNLIKELY((ctl_did_i & 0xff000000U))) {
        Verilated::overWidthError("ctl_did_i");}
    if (VL_UNLIKELY((select_fire_i & 0xfeU))) {
        Verilated::overWidthError("select_fire_i");}
    if (VL_UNLIKELY((select_did_i & 0xff000000U))) {
        Verilated::overWidthError("select_did_i");}
    if (VL_UNLIKELY((lookup_i & 0xfeU))) {
        Verilated::overWidthError("lookup_i");}
    if (VL_UNLIKELY((lookup_hit_i & 0xfeU))) {
        Verilated::overWidthError("lookup_hit_i");}
    if (VL_UNLIKELY((lookup_did_i & 0xff000000U))) {
        Verilated::overWidthError("lookup_did_i");}
    if (VL_UNLIKELY((ptw_r_fire_i & 0xfeU))) {
        Verilated::overWidthError("ptw_r_fire_i");}
    if (VL_UNLIKELY((response_error_i & 0xfeU))) {
        Verilated::overWidthError("response_error_i");}
}
#endif  // VL_DEBUG
