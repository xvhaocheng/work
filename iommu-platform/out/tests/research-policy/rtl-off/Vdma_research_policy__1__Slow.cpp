// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_policy.h for the primary calling header

#include "Vdma_research_policy.h"
#include "Vdma_research_policy__Syms.h"

void Vdma_research_policy::_settle__TOP__4(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_settle__TOP__4\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp12[3];
    WData/*95:0*/ __Vtemp13[3];
    WData/*95:0*/ __Vtemp14[3];
    WData/*95:0*/ __Vtemp15[3];
    WData/*95:0*/ __Vtemp16[3];
    WData/*95:0*/ __Vtemp17[3];
    WData/*95:0*/ __Vtemp19[3];
    WData/*95:0*/ __Vtemp20[3];
    WData/*95:0*/ __Vtemp21[3];
    WData/*95:0*/ __Vtemp22[3];
    WData/*95:0*/ __Vtemp23[3];
    WData/*95:0*/ __Vtemp25[3];
    WData/*95:0*/ __Vtemp26[3];
    WData/*95:0*/ __Vtemp27[3];
    WData/*95:0*/ __Vtemp28[3];
    WData/*95:0*/ __Vtemp30[3];
    WData/*95:0*/ __Vtemp31[3];
    WData/*95:0*/ __Vtemp32[3];
    WData/*95:0*/ __Vtemp33[3];
    WData/*95:0*/ __Vtemp34[3];
    WData/*95:0*/ __Vtemp35[3];
    WData/*95:0*/ __Vtemp36[3];
    WData/*95:0*/ __Vtemp37[3];
    WData/*95:0*/ __Vtemp38[3];
    WData/*95:0*/ __Vtemp39[3];
    WData/*95:0*/ __Vtemp40[3];
    WData/*95:0*/ __Vtemp42[3];
    WData/*95:0*/ __Vtemp43[3];
    WData/*95:0*/ __Vtemp44[3];
    WData/*95:0*/ __Vtemp47[3];
    WData/*95:0*/ __Vtemp48[3];
    WData/*95:0*/ __Vtemp49[3];
    WData/*95:0*/ __Vtemp53[3];
    WData/*95:0*/ __Vtemp54[3];
    WData/*95:0*/ __Vtemp55[3];
    WData/*95:0*/ __Vtemp58[3];
    WData/*95:0*/ __Vtemp59[3];
    WData/*95:0*/ __Vtemp60[3];
    WData/*95:0*/ __Vtemp61[3];
    WData/*95:0*/ __Vtemp62[3];
    WData/*95:0*/ __Vtemp63[3];
    WData/*95:0*/ __Vtemp64[3];
    WData/*95:0*/ __Vtemp65[3];
    WData/*95:0*/ __Vtemp66[3];
    WData/*95:0*/ __Vtemp67[3];
    WData/*95:0*/ __Vtemp68[3];
    WData/*95:0*/ __Vtemp69[3];
    WData/*95:0*/ __Vtemp70[3];
    WData/*95:0*/ __Vtemp71[3];
    WData/*95:0*/ __Vtemp72[3];
    WData/*95:0*/ __Vtemp73[3];
    WData/*95:0*/ __Vtemp74[3];
    WData/*95:0*/ __Vtemp75[3];
    WData/*95:0*/ __Vtemp76[3];
    WData/*95:0*/ __Vtemp77[3];
    WData/*95:0*/ __Vtemp78[3];
    WData/*95:0*/ __Vtemp79[3];
    WData/*95:0*/ __Vtemp80[3];
    WData/*95:0*/ __Vtemp81[3];
    WData/*95:0*/ __Vtemp82[3];
    WData/*95:0*/ __Vtemp83[3];
    WData/*95:0*/ __Vtemp84[3];
    WData/*95:0*/ __Vtemp85[3];
    WData/*95:0*/ __Vtemp86[3];
    WData/*95:0*/ __Vtemp87[3];
    WData/*95:0*/ __Vtemp88[3];
    WData/*95:0*/ __Vtemp89[3];
    WData/*95:0*/ __Vtemp90[3];
    WData/*95:0*/ __Vtemp91[3];
    WData/*95:0*/ __Vtemp92[3];
    WData/*95:0*/ __Vtemp93[3];
    WData/*95:0*/ __Vtemp94[3];
    WData/*95:0*/ __Vtemp95[3];
    WData/*95:0*/ __Vtemp96[3];
    WData/*95:0*/ __Vtemp97[3];
    WData/*95:0*/ __Vtemp98[3];
    WData/*95:0*/ __Vtemp99[3];
    WData/*95:0*/ __Vtemp100[3];
    WData/*95:0*/ __Vtemp101[3];
    WData/*95:0*/ __Vtemp102[3];
    WData/*95:0*/ __Vtemp103[3];
    WData/*95:0*/ __Vtemp104[3];
    WData/*95:0*/ __Vtemp105[3];
    WData/*95:0*/ __Vtemp106[3];
    WData/*95:0*/ __Vtemp107[3];
    WData/*95:0*/ __Vtemp108[3];
    WData/*95:0*/ __Vtemp109[3];
    WData/*95:0*/ __Vtemp110[3];
    WData/*95:0*/ __Vtemp111[3];
    WData/*95:0*/ __Vtemp112[3];
    WData/*95:0*/ __Vtemp113[3];
    WData/*95:0*/ __Vtemp114[3];
    WData/*95:0*/ __Vtemp115[3];
    WData/*95:0*/ __Vtemp116[3];
    WData/*95:0*/ __Vtemp117[3];
    WData/*95:0*/ __Vtemp118[3];
    WData/*95:0*/ __Vtemp119[3];
    WData/*95:0*/ __Vtemp120[3];
    WData/*95:0*/ __Vtemp121[3];
    WData/*95:0*/ __Vtemp122[3];
    WData/*95:0*/ __Vtemp123[3];
    WData/*95:0*/ __Vtemp124[3];
    WData/*95:0*/ __Vtemp125[3];
    WData/*95:0*/ __Vtemp126[3];
    WData/*95:0*/ __Vtemp127[3];
    WData/*95:0*/ __Vtemp128[3];
    WData/*95:0*/ __Vtemp129[3];
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
    WData/*95:0*/ __Vtemp175[3];
    WData/*95:0*/ __Vtemp176[3];
    WData/*95:0*/ __Vtemp180[3];
    WData/*95:0*/ __Vtemp181[3];
    WData/*95:0*/ __Vtemp182[3];
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
    VL_EXTEND_WQ(66,64, __Vtemp12, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp13, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp14, __Vtemp12, __Vtemp13);
    __Vtemp15[0U] = __Vtemp14[0U];
    __Vtemp15[1U] = __Vtemp14[1U];
    __Vtemp15[2U] = (3U & __Vtemp14[2U]);
    VL_EXTEND_WI(66,32, __Vtemp16, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WQ(66,64, __Vtemp17, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp19, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp20, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp21, __Vtemp19, __Vtemp20);
    __Vtemp22[0U] = __Vtemp21[0U];
    __Vtemp22[1U] = __Vtemp21[1U];
    __Vtemp22[2U] = (3U & __Vtemp21[2U]);
    vlTOPp->dma_research_policy__DOT__fb_inc = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_LTS_IWW(1,66,66, __Vtemp15, __Vtemp16)
                                                 : 
                                                VL_GTS_IWW(1,66,66, __Vtemp17, __Vtemp22));
    VL_EXTEND_WQ(66,64, __Vtemp23, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp25, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp26, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp27, __Vtemp25, __Vtemp26);
    __Vtemp28[0U] = __Vtemp27[0U];
    __Vtemp28[1U] = __Vtemp27[1U];
    __Vtemp28[2U] = (3U & __Vtemp27[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp30, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp31, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp32, __Vtemp30, __Vtemp31);
    __Vtemp33[0U] = __Vtemp32[0U];
    __Vtemp33[1U] = __Vtemp32[1U];
    __Vtemp33[2U] = (3U & __Vtemp32[2U]);
    VL_EXTEND_WI(66,32, __Vtemp34, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__fb_dec = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_GTS_IWW(1,66,66, __Vtemp23, __Vtemp28)
                                                 : 
                                                VL_LTS_IWW(1,66,66, __Vtemp33, __Vtemp34));
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
    vlTOPp->dma_research_policy__DOT__new_err = 0ULL;
    if (((IData)(vlTOPp->dma_research_policy__DOT__wr_any) 
         & (~ (IData)(vlTOPp->dma_research_policy__DOT__wr_ok)))) {
        vlTOPp->dma_research_policy__DOT__new_err = 
            (1ULL | vlTOPp->dma_research_policy__DOT__new_err);
    }
    if (vlTOPp->dma_research_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->index_i)))) {
            if ((1U == (IData)(vlTOPp->wdata_i))) {
                vlTOPp->dma_research_policy__DOT__new_err 
                    = (4ULL | vlTOPp->dma_research_policy__DOT__new_err);
            } else {
                if ((2U == (IData)(vlTOPp->wdata_i))) {
                    vlTOPp->dma_research_policy__DOT__new_err 
                        = ((1U & ((~ (IData)(vlTOPp->dma_research_policy__DOT__armed_q)) 
                                  | (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)))
                            ? (0x100ULL | vlTOPp->dma_research_policy__DOT__new_err)
                            : (4ULL | vlTOPp->dma_research_policy__DOT__new_err));
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
    VL_EXTEND_WI(65,32, __Vtemp35, vlTOPp->dma_research_policy__DOT__maxbud_q);
    VL_EXTEND_WI(65,32, __Vtemp36, vlTOPp->dma_research_policy__DOT__minbud_q);
    vlTOPp->dma_research_policy__DOT__b_next = ((IData)(vlTOPp->dma_research_policy__DOT__fb_inc)
                                                 ? 
                                                (VL_GTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_up_s, __Vtemp35)
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
                                                 (VL_LTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_dn_s, __Vtemp36)
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
    vlTOPp->dma_research_policy__DOT__stat_inc[5U] = 0ULL;
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
    VL_EXTEND_WQ(66,64, __Vtemp37, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp38, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[3U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp37, __Vtemp38))
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp39, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp40, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[4U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp39, __Vtemp40))
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
    __Vtemp42[0U] = (IData)(vlTOPp->dma_research_policy__DOT__balance_q);
    __Vtemp42[1U] = (IData)((vlTOPp->dma_research_policy__DOT__balance_q 
                             >> 0x20U));
    __Vtemp42[2U] = (3U & (- (IData)((1U & (IData)(
                                                   (vlTOPp->dma_research_policy__DOT__balance_q 
                                                    >> 0x3fU))))));
    VL_EXTEND_WQ(66,64, __Vtemp43, vlTOPp->dma_research_policy__DOT__charge_now);
    VL_SUB_W(3, __Vtemp44, __Vtemp42, __Vtemp43);
    vlTOPp->dma_research_policy__DOT__bal_c[0U] = __Vtemp44[0U];
    vlTOPp->dma_research_policy__DOT__bal_c[1U] = __Vtemp44[1U];
    vlTOPp->dma_research_policy__DOT__bal_c[2U] = (3U 
                                                   & __Vtemp44[2U]);
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
        VL_EXTEND_WQ(65,64, __Vtemp47, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp48, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
        VL_ADD_W(3, __Vtemp49, __Vtemp47, __Vtemp48);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
            = __Vtemp49[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
            = __Vtemp49[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
            = (1U & __Vtemp49[2U]);
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
        VL_EXTEND_WQ(65,64, __Vtemp53, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp54, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
        VL_ADD_W(3, __Vtemp55, __Vtemp53, __Vtemp54);
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
            = __Vtemp55[0U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
            = __Vtemp55[1U];
        vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
            = (1U & __Vtemp55[2U]);
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
    __Vtemp58[0U] = 0xffffffffU;
    __Vtemp58[1U] = 0xffffffffU;
    __Vtemp58[2U] = 0U;
    __Vtemp59[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp59[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp59[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp58, __Vtemp59)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp60[0U] = 0xffffffffU;
    __Vtemp60[1U] = 0xffffffffU;
    __Vtemp60[2U] = 0U;
    __Vtemp61[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp61[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp61[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp60, __Vtemp61)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp62[0U] = 0xffffffffU;
    __Vtemp62[1U] = 0xffffffffU;
    __Vtemp62[2U] = 0U;
    __Vtemp63[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp63[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp63[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp62, __Vtemp63)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp64[0U] = 0xffffffffU;
    __Vtemp64[1U] = 0xffffffffU;
    __Vtemp64[2U] = 0U;
    __Vtemp65[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp65[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp65[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp64, __Vtemp65)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp66[0U] = 0xffffffffU;
    __Vtemp66[1U] = 0xffffffffU;
    __Vtemp66[2U] = 0U;
    __Vtemp67[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp67[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp67[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp66, __Vtemp67)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp68[0U] = 0xffffffffU;
    __Vtemp68[1U] = 0xffffffffU;
    __Vtemp68[2U] = 0U;
    __Vtemp69[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp69[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp69[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp68, __Vtemp69)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp70[0U] = 0xffffffffU;
    __Vtemp70[1U] = 0xffffffffU;
    __Vtemp70[2U] = 0U;
    __Vtemp71[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp71[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp71[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp70, __Vtemp71)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp72[0U] = 0xffffffffU;
    __Vtemp72[1U] = 0xffffffffU;
    __Vtemp72[2U] = 0U;
    __Vtemp73[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp73[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp73[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp72, __Vtemp73)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp74[0U] = 0xffffffffU;
    __Vtemp74[1U] = 0xffffffffU;
    __Vtemp74[2U] = 0U;
    __Vtemp75[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp75[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp75[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp74, __Vtemp75)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp76[0U] = 0xffffffffU;
    __Vtemp76[1U] = 0xffffffffU;
    __Vtemp76[2U] = 0U;
    __Vtemp77[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp77[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp77[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp76, __Vtemp77)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp78[0U] = 0xffffffffU;
    __Vtemp78[1U] = 0xffffffffU;
    __Vtemp78[2U] = 0U;
    __Vtemp79[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp79[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp79[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp78, __Vtemp79)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp80[0U] = 0xffffffffU;
    __Vtemp80[1U] = 0xffffffffU;
    __Vtemp80[2U] = 0U;
    __Vtemp81[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp81[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp81[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp80, __Vtemp81)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp82[0U] = 0xffffffffU;
    __Vtemp82[1U] = 0xffffffffU;
    __Vtemp82[2U] = 0U;
    __Vtemp83[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp83[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp83[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp82, __Vtemp83)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp84[0U] = 0xffffffffU;
    __Vtemp84[1U] = 0xffffffffU;
    __Vtemp84[2U] = 0U;
    __Vtemp85[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp85[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp85[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp84, __Vtemp85)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp86[0U] = 0xffffffffU;
    __Vtemp86[1U] = 0xffffffffU;
    __Vtemp86[2U] = 0U;
    __Vtemp87[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp87[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp87[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp86, __Vtemp87)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp88[0U] = 0xffffffffU;
    __Vtemp88[1U] = 0xffffffffU;
    __Vtemp88[2U] = 0U;
    __Vtemp89[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp89[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp89[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp88, __Vtemp89)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp90[0U] = 0xffffffffU;
    __Vtemp90[1U] = 0xffffffffU;
    __Vtemp90[2U] = 0U;
    __Vtemp91[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp91[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp91[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp90, __Vtemp91)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp92[0U] = 0xffffffffU;
    __Vtemp92[1U] = 0xffffffffU;
    __Vtemp92[2U] = 0U;
    __Vtemp93[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp93[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp93[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp92, __Vtemp93)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp94[0U] = 0xffffffffU;
    __Vtemp94[1U] = 0xffffffffU;
    __Vtemp94[2U] = 0U;
    __Vtemp95[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp95[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp95[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp94, __Vtemp95)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp96[0U] = 0xffffffffU;
    __Vtemp96[1U] = 0xffffffffU;
    __Vtemp96[2U] = 0U;
    __Vtemp97[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp97[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp97[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp96, __Vtemp97)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp98[0U] = 0xffffffffU;
    __Vtemp98[1U] = 0xffffffffU;
    __Vtemp98[2U] = 0U;
    __Vtemp99[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp99[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp99[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp98, __Vtemp99)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp100[0U] = 0xffffffffU;
    __Vtemp100[1U] = 0xffffffffU;
    __Vtemp100[2U] = 0U;
    __Vtemp101[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp101[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp101[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp100, __Vtemp101)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp102[0U] = 0xffffffffU;
    __Vtemp102[1U] = 0xffffffffU;
    __Vtemp102[2U] = 0U;
    __Vtemp103[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp103[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp103[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp102, __Vtemp103)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp104[0U] = 0xffffffffU;
    __Vtemp104[1U] = 0xffffffffU;
    __Vtemp104[2U] = 0U;
    __Vtemp105[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp105[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp105[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp104, __Vtemp105)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp106[0U] = 0xffffffffU;
    __Vtemp106[1U] = 0xffffffffU;
    __Vtemp106[2U] = 0U;
    __Vtemp107[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp107[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp107[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp106, __Vtemp107)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp108[0U] = 0xffffffffU;
    __Vtemp108[1U] = 0xffffffffU;
    __Vtemp108[2U] = 0U;
    __Vtemp109[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp109[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp109[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp108, __Vtemp109)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp110[0U] = 0xffffffffU;
    __Vtemp110[1U] = 0xffffffffU;
    __Vtemp110[2U] = 0U;
    __Vtemp111[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp111[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp111[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp110, __Vtemp111)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp112[0U] = 0xffffffffU;
    __Vtemp112[1U] = 0xffffffffU;
    __Vtemp112[2U] = 0U;
    __Vtemp113[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp113[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp113[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp112, __Vtemp113)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp114[0U] = 0xffffffffU;
    __Vtemp114[1U] = 0xffffffffU;
    __Vtemp114[2U] = 0U;
    __Vtemp115[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp115[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp115[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp114, __Vtemp115)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp116[0U] = 0xffffffffU;
    __Vtemp116[1U] = 0xffffffffU;
    __Vtemp116[2U] = 0U;
    __Vtemp117[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp117[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp117[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp116, __Vtemp117)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp118[0U] = 0xffffffffU;
    __Vtemp118[1U] = 0xffffffffU;
    __Vtemp118[2U] = 0U;
    __Vtemp119[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp119[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp119[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp118, __Vtemp119)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp120[0U] = 0xffffffffU;
    __Vtemp120[1U] = 0xffffffffU;
    __Vtemp120[2U] = 0U;
    __Vtemp121[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp121[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp121[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp120, __Vtemp121)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp122[0U] = 0xffffffffU;
    __Vtemp122[1U] = 0xffffffffU;
    __Vtemp122[2U] = 0U;
    __Vtemp123[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp123[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp123[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp122, __Vtemp123)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp124[0U] = 0xffffffffU;
    __Vtemp124[1U] = 0xffffffffU;
    __Vtemp124[2U] = 0U;
    __Vtemp125[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp125[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp125[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp124, __Vtemp125)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp126[0U] = 0xffffffffU;
    __Vtemp126[1U] = 0xffffffffU;
    __Vtemp126[2U] = 0U;
    __Vtemp127[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp127[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp127[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp126, __Vtemp127)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp128[0U] = 0xffffffffU;
    __Vtemp128[1U] = 0xffffffffU;
    __Vtemp128[2U] = 0U;
    __Vtemp129[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp129[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp129[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp128, __Vtemp129)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp130[0U] = 0xffffffffU;
    __Vtemp130[1U] = 0xffffffffU;
    __Vtemp130[2U] = 0U;
    __Vtemp131[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp131[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp131[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp130, __Vtemp131)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp132[0U] = 0xffffffffU;
    __Vtemp132[1U] = 0xffffffffU;
    __Vtemp132[2U] = 0U;
    __Vtemp133[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp133[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp133[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp132, __Vtemp133)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp134[0U] = 0xffffffffU;
    __Vtemp134[1U] = 0xffffffffU;
    __Vtemp134[2U] = 0U;
    __Vtemp135[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp135[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp135[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp134, __Vtemp135)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp136[0U] = 0xffffffffU;
    __Vtemp136[1U] = 0xffffffffU;
    __Vtemp136[2U] = 0U;
    __Vtemp137[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp137[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp137[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp136, __Vtemp137)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp138[0U] = 0xffffffffU;
    __Vtemp138[1U] = 0xffffffffU;
    __Vtemp138[2U] = 0U;
    __Vtemp139[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp139[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp139[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp138, __Vtemp139)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp140[0U] = 0xffffffffU;
    __Vtemp140[1U] = 0xffffffffU;
    __Vtemp140[2U] = 0U;
    __Vtemp141[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp141[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp141[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp140, __Vtemp141)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp142[0U] = 0xffffffffU;
    __Vtemp142[1U] = 0xffffffffU;
    __Vtemp142[2U] = 0U;
    __Vtemp143[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp143[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp143[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp142, __Vtemp143)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp144[0U] = 0xffffffffU;
    __Vtemp144[1U] = 0xffffffffU;
    __Vtemp144[2U] = 0U;
    __Vtemp145[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp145[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp145[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp144, __Vtemp145)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp146[0U] = 0xffffffffU;
    __Vtemp146[1U] = 0xffffffffU;
    __Vtemp146[2U] = 0U;
    __Vtemp147[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp147[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp147[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp146, __Vtemp147)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp148[0U] = 0xffffffffU;
    __Vtemp148[1U] = 0xffffffffU;
    __Vtemp148[2U] = 0U;
    __Vtemp149[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp149[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp149[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp148, __Vtemp149)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp150[0U] = 0xffffffffU;
    __Vtemp150[1U] = 0xffffffffU;
    __Vtemp150[2U] = 0U;
    __Vtemp151[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp151[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp151[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp150, __Vtemp151)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp152[0U] = 0xffffffffU;
    __Vtemp152[1U] = 0xffffffffU;
    __Vtemp152[2U] = 0U;
    __Vtemp153[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp153[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp153[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp152, __Vtemp153)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp154[0U] = 0xffffffffU;
    __Vtemp154[1U] = 0xffffffffU;
    __Vtemp154[2U] = 0U;
    __Vtemp155[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp155[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp155[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp154, __Vtemp155)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp156[0U] = 0xffffffffU;
    __Vtemp156[1U] = 0xffffffffU;
    __Vtemp156[2U] = 0U;
    __Vtemp157[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp157[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp157[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp156, __Vtemp157)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp158[0U] = 0xffffffffU;
    __Vtemp158[1U] = 0xffffffffU;
    __Vtemp158[2U] = 0U;
    __Vtemp159[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp159[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp159[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp158, __Vtemp159)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp160[0U] = 0xffffffffU;
    __Vtemp160[1U] = 0xffffffffU;
    __Vtemp160[2U] = 0U;
    __Vtemp161[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp161[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp161[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp160, __Vtemp161)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp162[0U] = 0xffffffffU;
    __Vtemp162[1U] = 0xffffffffU;
    __Vtemp162[2U] = 0U;
    __Vtemp163[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp163[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp163[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp162, __Vtemp163)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp164[0U] = 0xffffffffU;
    __Vtemp164[1U] = 0xffffffffU;
    __Vtemp164[2U] = 0U;
    __Vtemp165[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp165[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp165[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp164, __Vtemp165)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp166[0U] = 0xffffffffU;
    __Vtemp166[1U] = 0xffffffffU;
    __Vtemp166[2U] = 0U;
    __Vtemp167[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp167[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp167[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp166, __Vtemp167)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp168[0U] = 0xffffffffU;
    __Vtemp168[1U] = 0xffffffffU;
    __Vtemp168[2U] = 0U;
    __Vtemp169[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp169[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp169[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp168, __Vtemp169)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp170[0U] = 0xffffffffU;
    __Vtemp170[1U] = 0xffffffffU;
    __Vtemp170[2U] = 0U;
    __Vtemp171[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp171[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp171[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp170, __Vtemp171)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp172[0U] = 0xffffffffU;
    __Vtemp172[1U] = 0xffffffffU;
    __Vtemp172[2U] = 0U;
    __Vtemp173[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp173[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp173[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp172, __Vtemp173)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    VL_EXTEND_WQ(66,64, __Vtemp175, vlTOPp->dma_research_policy__DOT__refill_now);
    VL_ADD_W(3, __Vtemp176, vlTOPp->dma_research_policy__DOT__bal_c, __Vtemp175);
    vlTOPp->dma_research_policy__DOT__bal_r[0U] = __Vtemp176[0U];
    vlTOPp->dma_research_policy__DOT__bal_r[1U] = __Vtemp176[1U];
    vlTOPp->dma_research_policy__DOT__bal_r[2U] = (3U 
                                                   & __Vtemp176[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp180, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    __Vtemp181[0U] = __Vtemp180[0U];
    __Vtemp181[1U] = __Vtemp180[1U];
    __Vtemp181[2U] = (3U & __Vtemp180[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp182, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    if (((IData)(vlTOPp->dma_research_policy__DOT__refill_en) 
         & VL_GTS_IWW(1,66,66, vlTOPp->dma_research_policy__DOT__bal_r, __Vtemp181))) {
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[0U] 
            = __Vtemp182[0U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
            = __Vtemp182[1U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
            = (3U & __Vtemp182[2U]);
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
}

void Vdma_research_policy::_eval_initial(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_eval_initial\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
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
    vlTOPp->_settle__TOP__3(vlSymsp);
    vlTOPp->_settle__TOP__4(vlSymsp);
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
    dma_research_policy__DOT__gap_load = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cnt_ovf = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__wr_any = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__wr_ok = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__obs_err_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__sup_drop_now = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__new_err = VL_RAND_RESET_Q(64);
    dma_research_policy__DOT__do_freeze = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__do_clear = VL_RAND_RESET_I(1);
    dma_research_policy__DOT__cfg_wr = VL_RAND_RESET_I(1);
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
