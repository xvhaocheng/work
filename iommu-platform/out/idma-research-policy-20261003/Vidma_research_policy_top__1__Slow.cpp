// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_research_policy_top.h for the primary calling header

#include "Vidma_research_policy_top.h"
#include "Vidma_research_policy_top__Syms.h"

void Vidma_research_policy_top::_settle__TOP__2(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_settle__TOP__2\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp16[3];
    WData/*95:0*/ __Vtemp17[3];
    WData/*95:0*/ __Vtemp18[3];
    WData/*95:0*/ __Vtemp19[3];
    WData/*95:0*/ __Vtemp20[3];
    WData/*95:0*/ __Vtemp21[3];
    WData/*95:0*/ __Vtemp23[3];
    WData/*95:0*/ __Vtemp24[3];
    WData/*95:0*/ __Vtemp25[3];
    WData/*95:0*/ __Vtemp26[3];
    WData/*95:0*/ __Vtemp27[3];
    WData/*95:0*/ __Vtemp29[3];
    WData/*95:0*/ __Vtemp30[3];
    WData/*95:0*/ __Vtemp31[3];
    WData/*95:0*/ __Vtemp32[3];
    WData/*95:0*/ __Vtemp34[3];
    WData/*95:0*/ __Vtemp35[3];
    WData/*95:0*/ __Vtemp36[3];
    WData/*95:0*/ __Vtemp37[3];
    WData/*95:0*/ __Vtemp38[3];
    WData/*95:0*/ __Vtemp39[3];
    WData/*95:0*/ __Vtemp40[3];
    WData/*223:0*/ __Vtemp45[7];
    WData/*287:0*/ __Vtemp46[9];
    WData/*287:0*/ __Vtemp47[9];
    WData/*351:0*/ __Vtemp48[11];
    WData/*95:0*/ __Vtemp57[3];
    WData/*95:0*/ __Vtemp58[3];
    WData/*95:0*/ __Vtemp59[3];
    WData/*95:0*/ __Vtemp62[3];
    WData/*95:0*/ __Vtemp63[3];
    WData/*95:0*/ __Vtemp67[3];
    WData/*95:0*/ __Vtemp68[3];
    WData/*95:0*/ __Vtemp69[3];
    WData/*95:0*/ __Vtemp73[3];
    WData/*95:0*/ __Vtemp74[3];
    WData/*95:0*/ __Vtemp75[3];
    WData/*95:0*/ __Vtemp76[3];
    WData/*95:0*/ __Vtemp78[3];
    WData/*95:0*/ __Vtemp79[3];
    WData/*95:0*/ __Vtemp80[3];
    WData/*95:0*/ __Vtemp84[3];
    WData/*95:0*/ __Vtemp85[3];
    WData/*95:0*/ __Vtemp86[3];
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
    // Body
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q)));
    vlTOPp->batch_active_o = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                              & (0U != (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok 
        = (((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_any) 
              & (0xeU == (0x1fU & ((IData)(vlTOPp->p_index_i) 
                                   >> 8U)))) & (4U 
                                                <= 
                                                (0xffU 
                                                 & (IData)(vlTOPp->p_index_i)))) 
            & (0x10U >= (0xffU & (IData)(vlTOPp->p_index_i)))) 
           & (0U == (IData)((vlTOPp->p_wdata_i >> 0x20U))));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_hit_a 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_a) 
           & (IData)(vlTOPp->p_lookup_hit_i));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_a) 
           & (~ (IData)(vlTOPp->p_lookup_hit_i)));
    VL_EXTEND_WQ(66,64, __Vtemp16, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp17, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp18, __Vtemp16, __Vtemp17);
    __Vtemp19[0U] = __Vtemp18[0U];
    __Vtemp19[1U] = __Vtemp18[1U];
    __Vtemp19[2U] = (3U & __Vtemp18[2U]);
    VL_EXTEND_WI(66,32, __Vtemp20, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    VL_EXTEND_WQ(66,64, __Vtemp21, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp23, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp24, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp25, __Vtemp23, __Vtemp24);
    __Vtemp26[0U] = __Vtemp25[0U];
    __Vtemp26[1U] = __Vtemp25[1U];
    __Vtemp26[2U] = (3U & __Vtemp25[2U]);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_inc 
        = ((6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
            ? VL_LTS_IWW(1,66,66, __Vtemp19, __Vtemp20)
            : VL_GTS_IWW(1,66,66, __Vtemp21, __Vtemp26));
    VL_EXTEND_WQ(66,64, __Vtemp27, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp29, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp30, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp31, __Vtemp29, __Vtemp30);
    __Vtemp32[0U] = __Vtemp31[0U];
    __Vtemp32[1U] = __Vtemp31[1U];
    __Vtemp32[2U] = (3U & __Vtemp31[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp34, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp35, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp36, __Vtemp34, __Vtemp35);
    __Vtemp37[0U] = __Vtemp36[0U];
    __Vtemp37[1U] = __Vtemp36[1U];
    __Vtemp37[2U] = (3U & __Vtemp36[2U]);
    VL_EXTEND_WI(66,32, __Vtemp38, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_dec 
        = ((6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
            ? VL_GTS_IWW(1,66,66, __Vtemp27, __Vtemp32)
            : VL_LTS_IWW(1,66,66, __Vtemp37, __Vtemp38));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_next_id_read 
        = (((vlTOPp->idma_research_policy_top__DOT__req[2U] 
             >> 0x11U) & (~ (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                             >> 0x10U))) & (0x28U == 
                                            (0xffU 
                                             & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                >> 8U))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_access 
        = (1U & ((vlTOPp->idma_research_policy_top__DOT__req[2U] 
                  >> 0x11U) & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                               >> 0xfU)));
    vlTOPp->request_o = vlTOPp->idma_research_policy_top__DOT__request;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__main_inc 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
             & (7U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_actual_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
             & (IData)(vlTOPp->p_v_active_i)) & (~ (IData)(vlTOPp->p_v_done_i)))
            ? (1ULL + vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_q)
            : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (0U != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__window_q)) 
           & (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_cnt_q 
              == ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__window_q)) 
                  - 1ULL)));
    vlTOPp->policy_enable_o = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core;
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_task_done 
        = (((IData)(vlTOPp->batch_active_o) & (2U == (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) 
           & (IData)(vlTOPp->complete_i));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_task_issue 
        = (((IData)(vlTOPp->batch_active_o) & (1U == (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) 
           & (IData)(vlTOPp->ready_i));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cfg_wr = 0U;
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok) {
        if ((4U != (0xffU & (IData)(vlTOPp->p_index_i)))) {
            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cfg_wr = 1U;
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_clear = 0U;
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->p_index_i)))) {
            if ((1U != (IData)(vlTOPp->p_wdata_i))) {
                if ((2U != (IData)(vlTOPp->p_wdata_i))) {
                    if ((3U != (IData)(vlTOPp->p_wdata_i))) {
                        if ((4U == (IData)(vlTOPp->p_wdata_i))) {
                            if ((1U & (~ ((~ (IData)(vlTOPp->p_quiet_i)) 
                                          | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q))))) {
                                vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_clear = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_freeze = 0U;
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->p_index_i)))) {
            if ((1U != (IData)(vlTOPp->p_wdata_i))) {
                if ((2U != (IData)(vlTOPp->p_wdata_i))) {
                    if ((3U == (IData)(vlTOPp->p_wdata_i))) {
                        if ((1U & (~ ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q)) 
                                      | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q))))) {
                            if (vlTOPp->p_quiet_i) {
                                vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_freeze = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_arm = 0U;
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->p_index_i)))) {
            if ((1U == (IData)(vlTOPp->p_wdata_i))) {
                if (vlTOPp->p_supported_i) {
                    if ((1U & (~ (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q) 
                                   | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q)) 
                                  | (~ (IData)(vlTOPp->p_quiet_i)))))) {
                        if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cfg_bad)))) {
                            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_arm = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err = 0ULL;
    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_any) 
         & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok)))) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
            = (1ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
    }
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->p_index_i)))) {
            if ((1U == (IData)(vlTOPp->p_wdata_i))) {
                if (vlTOPp->p_supported_i) {
                    if ((1U & (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q) 
                                | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q)) 
                               | (~ (IData)(vlTOPp->p_quiet_i))))) {
                        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                            = (0x100ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                    } else {
                        if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cfg_bad) {
                            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                                = (0x400ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                        }
                    }
                } else {
                    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                        = (4ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                }
            } else {
                if ((2U == (IData)(vlTOPp->p_wdata_i))) {
                    if ((1U & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q)) 
                               | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q)))) {
                        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                            = (0x100ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                    } else {
                        if ((1U & (~ (IData)(vlTOPp->p_supported_i)))) {
                            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                                = (4ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                        }
                    }
                } else {
                    if ((3U == (IData)(vlTOPp->p_wdata_i))) {
                        if ((1U & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q)) 
                                   | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q)))) {
                            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                                = (0x100ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                        } else {
                            if ((1U & (~ (IData)(vlTOPp->p_quiet_i)))) {
                                vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                                    = (0x80ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                            }
                        }
                    } else {
                        if ((4U == (IData)(vlTOPp->p_wdata_i))) {
                            if ((1U & ((~ (IData)(vlTOPp->p_quiet_i)) 
                                       | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q)))) {
                                vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                                    = (0x40ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                            }
                        } else {
                            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                                = (2ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
                        }
                    }
                }
            }
        } else {
            if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q) {
                vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err 
                    = (8ULL | vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err);
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_abort = 0U;
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wr_ok) {
        if ((4U == (0xffU & (IData)(vlTOPp->p_index_i)))) {
            if ((1U != (IData)(vlTOPp->p_wdata_i))) {
                if ((2U == (IData)(vlTOPp->p_wdata_i))) {
                    if ((1U & (~ ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__armed_q)) 
                                  | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__frozen_q))))) {
                        if (vlTOPp->p_supported_i) {
                            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_abort = 1U;
                        }
                    }
                }
            }
        }
    }
    VL_EXTEND_WI(65,32, __Vtemp39, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__maxbud_q);
    VL_EXTEND_WI(65,32, __Vtemp40, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__minbud_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_next 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_inc)
            ? (VL_GTS_IWW(1,65,65, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_up_s, __Vtemp39)
                ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__maxbud_q))
                : (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_up_s[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_up_s[0U]))))
            : ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_dec)
                ? (VL_LTS_IWW(1,65,65, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_dn_s, __Vtemp40)
                    ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__minbud_q))
                    : (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_dn_s[1U])) 
                        << 0x20U) | (QData)((IData)(
                                                    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_dn_s[0U]))))
                : vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_q));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write 
        = ((((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_access) 
               & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                  >> 0x10U)) & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q))) 
             & (0xffU == (0xffU & vlTOPp->idma_research_policy_top__DOT__req[0U]))) 
            & (0x88U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                  >> 8U)))) & (2ULL 
                                               == (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_research_policy_top__DOT__req[2U])) 
                                                    << 0x38U) 
                                                   | (((QData)((IData)(
                                                                       vlTOPp->idma_research_policy_top__DOT__req[1U])) 
                                                       << 0x18U) 
                                                      | ((QData)((IData)(
                                                                         vlTOPp->idma_research_policy_top__DOT__req[0U])) 
                                                         >> 8U)))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_locked_write 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) 
           & (((vlTOPp->idma_research_policy_top__DOT__req[2U] 
                >> 0x11U) & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                             >> 0x10U)) & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_access))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__locked_write 
        = ((((IData)(vlTOPp->batch_active_o) & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                >> 0x11U)) 
            & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
               >> 0x10U)) & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_access)));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_max_next 
        = ((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next 
            > vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_max_q)
            ? vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next
            : vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_max_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last) 
           & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__act_all_now));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_en 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last) 
            & (0U != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
           & (6U != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stale_cyc 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last) 
           & (7U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__abort_eff 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__aborted_q) 
           | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__do_abort));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U] 
        = vlTOPp->idma_research_policy_top__DOT__req[0U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[1U] 
        = vlTOPp->idma_research_policy_top__DOT__req[1U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
        = vlTOPp->idma_research_policy_top__DOT__req[2U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
        = ((0x1ffffU & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U]) 
           | (0x20000U & ((((vlTOPp->idma_research_policy_top__DOT__req[2U] 
                             & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_access)) 
                                << 0x11U)) & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__locked_write)) 
                                              << 0x11U)) 
                           & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_locked_write)) 
                              << 0x11U)) & ((~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) 
                                                & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_next_id_read))) 
                                            << 0x11U))));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[1U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[2U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[3U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[4U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[5U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[6U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[7U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[8U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[9U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0xaU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0xbU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0xcU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0xdU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0xeU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0xfU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x10U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x11U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x12U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x13U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x14U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x15U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x16U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x17U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x18U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x19U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x1aU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x1bU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x1cU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x1dU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x1eU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[0x1fU] = 0ULL;
    if (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc[(
                                                                        (0x1fULL 
                                                                         <= vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now)
                                                                         ? 0x1fU
                                                                         : 
                                                                        (0x1fU 
                                                                         & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now)))] = 1ULL;
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_now 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_en)
            ? vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_q
            : 0ULL);
    vlTOPp->policy_abort_o = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
                              & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__abort_eff) 
                                 | (0ULL != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__err_q)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                  >> 0x11U) & (~ (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 0x10U))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                  >> 0x11U) & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                               >> 0x10U)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                              >> 8U))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                               >> 8U))) << 1U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 2U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 3U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 4U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 5U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 6U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 7U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 8U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[2U] 
                                  >> 8U))) << 9U));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__base_req[0U]))))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err));
    __Vtemp45[0U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp45[1U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                             >> 0x20U));
    __Vtemp45[2U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp45[3U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                             >> 0x20U));
    __Vtemp45[4U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp45[5U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                             >> 0x20U));
    __Vtemp45[6U] = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 6U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re)) 
                     & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp46, __Vtemp45);
    __Vtemp47[0U] = __Vtemp46[0U];
    __Vtemp47[1U] = __Vtemp46[1U];
    __Vtemp47[2U] = __Vtemp46[2U];
    __Vtemp47[3U] = __Vtemp46[3U];
    __Vtemp47[4U] = __Vtemp46[4U];
    __Vtemp47[5U] = __Vtemp46[5U];
    __Vtemp47[6U] = __Vtemp46[6U];
    __Vtemp47[7U] = __Vtemp46[7U];
    __Vtemp47[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                      >> 4U) & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                << 1U)) 
                                    & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                       << 1U))) | __Vtemp46[8U]);
    VL_EXTEND_WW(322,258, __Vtemp48, __Vtemp47);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0U] 
        = __Vtemp48[0U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[1U] 
        = __Vtemp48[1U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[2U] 
        = __Vtemp48[2U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[3U] 
        = __Vtemp48[3U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[4U] 
        = __Vtemp48[4U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[5U] 
        = __Vtemp48[5U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[6U] 
        = __Vtemp48[6U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[7U] 
        = __Vtemp48[7U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U] 
        = __Vtemp48[8U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[9U] 
        = __Vtemp48[9U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp48[0xaU])))))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->type_o = (3U & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
                             << 0x1aU) | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                                          >> 6U)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_n 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if ((1U & (~ (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                               | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write)) 
                              | (IData)(vlTOPp->policy_abort_o))))) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                         & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                                            | (IData)(vlTOPp->policy_abort_o))))))) {
                            if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                 & (0U != vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q))) {
                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_n 
                                    = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
                if ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_o))) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire = 1U;
                }
            } else {
                if ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_o))) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                                   | (IData)(vlTOPp->policy_abort_o))))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[3U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[4U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[5U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[6U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[7U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[8U] = 0U;
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[7U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                               >> 0xcU))));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[8U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[5U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0x10U])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xfU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                                               >> 0xcU))));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[6U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0x10U])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xfU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[3U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xdU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                               >> 0xcU))));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[4U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xdU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U] 
        = (0xfffffffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U] 
        = (0x10U | (0xf000000fU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U]));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U] 
        = ((0xfffffff7U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U]) 
           | (8U & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 5U)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = ((0x7fffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]) 
           | (0xffff8000U & (((0x100U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])
                               ? 1U : 0U) << 0xfU)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U] 
        = ((0xfffffff8U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[2U]) 
           | (0x7fffU & (((0x100U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])
                           ? 1U : 0U) >> 0x11U)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = (0x2000U | (0xffff9fffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = ((0xffffe1ffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]) 
           | (0x1e00U & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                         << 7U)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = (0xfffffeffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = (0xffffff1fU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = (0xffffffe1U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0x1fffffffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U] 
        = (0xfffffffeU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[1U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0x8000000U | (0xe7ffffffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = ((0xf87fffffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]) 
           | (0x7800000U & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                            << 0x15U)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xffbfffffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xffc7ffffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xfff87fffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xffff87ffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xfffffbffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = ((0xfffffdffU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]) 
           | (0x200U & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                        >> 2U)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xfffffe3fU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = (0xffffffc7U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]);
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = ((0xfffffffbU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]) 
           | (4U & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 8U)));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U] 
        = ((0xfffffffdU & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[0U]) 
           | (2U & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                    >> 9U)));
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[5U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0x10U])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xfU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q));
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[6U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0x10U])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xfU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q) 
                               >> 0x20U));
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[3U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xdU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q));
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[4U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xdU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q) 
                               >> 0x20U));
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                                   | (IData)(vlTOPp->policy_abort_o))))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_error_q)) 
                  & (0ULL != vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__id_retire) {
            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->length_o = (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[8U])) 
                         << 0x20U) | (QData)((IData)(
                                                     vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[7U])));
    vlTOPp->src_o = (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[6U])) 
                      << 0x20U) | (QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[5U])));
    vlTOPp->dst_o = (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[4U])) 
                      << 0x20U) | (QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o[3U])));
    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) {
        vlTOPp->done_id_o = (1ULL + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q);
        vlTOPp->done_o = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_task_done;
    } else {
        vlTOPp->done_id_o = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) 
                              & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__trans_complete))
                              ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_id_q
                              : ((0xffffffffffffffffULL 
                                  == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                                  ? 2ULL : (1ULL + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q)));
        vlTOPp->done_o = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__trans_complete;
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_n 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_n 
                        = (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q 
                           - 1ULL);
                }
            } else {
                if ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_n 
                                = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)
                                    ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q
                                    : (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[1U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0U]))));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                         & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_reverse_q))) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n 
                            = ((0ULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q)
                                ? (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q)) 
                                    - 1ULL) << 0xcU)
                                : (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q 
                                   - 0x1000ULL));
                    } else {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n 
                            = (0x1000ULL + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q);
                        if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                             & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n 
                                == ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q)) 
                                    << 0xcU)))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n = 0ULL;
                        }
                    }
                }
            } else {
                if ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n 
                                = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                    & (IData)(vlTOPp->policy_enable_o))
                                    ? (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                        & (IData)(vlTOPp->batch_reverse_i))
                                        ? (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q)) 
                                            - 1ULL) 
                                           << 0xcU)
                                        : 0ULL) : (
                                                   ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                                    & (IData)(vlTOPp->batch_reverse_i))
                                                    ? 
                                                   (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q)) 
                                                     - 1ULL) 
                                                    << 0xcU)
                                                    : 0ULL));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U]) 
           | (2U & ((~ (IData)(vlTOPp->backend_idle_i)) 
                    << 1U)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U] 
                                = ((1U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U]) 
                                   | (0xfffffffeU & 
                                      ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                       << 1U)));
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[5U] 
                                = ((1U & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                          >> 0x1fU)) 
                                   | (0xfffffffeU & 
                                      ((IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                >> 0x20U)) 
                                       << 1U)));
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U] 
                                = ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U]) 
                                   | (1U & ((IData)(
                                                    (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                     >> 0x20U)) 
                                            >> 0x1fU)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__issue = 1U;
                            if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                          & (IData)(vlTOPp->policy_enable_o))))) {
                                if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                     & (~ (IData)(vlTOPp->ready_i)))) {
                                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__issue = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__gap_load = 0U;
    if ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
          & (6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
         & (IData)(vlTOPp->done_o))) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__gap_load = 1U;
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__m6_cd_next 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (0ULL != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__m6_cd_q))
            ? (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__m6_cd_q 
               - 1ULL) : vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__m6_cd_q);
    if ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
          & (6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
         & (IData)(vlTOPp->done_o))) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__m6_cd_next 
            = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_q;
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__issue) {
            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->issue_o = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)
                        ? (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_task_issue)
                        : ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q)
                            ? (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) 
                                & (1U == (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) 
                               & (IData)(vlTOPp->ready_i))
                            : (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__issue)));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                   >> 0x20U));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__charge_now 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core)
             ? ((0U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
                 ? 0ULL : ((1U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
                            ? ((IData)(vlTOPp->issue_o)
                                ? 0x1000ULL : 0ULL)
                            : ((2U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
                                ? ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_sel_a)
                                    ? 1ULL : 0ULL) : 
                               ((3U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
                                 ? ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a)
                                     ? 1ULL : 0ULL)
                                 : ((4U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
                                     ? ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_hit_a)
                                           ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__whit_q))
                                           : 0ULL) 
                                         + ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a)
                                             ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wmiss_q))
                                             : 0ULL)) 
                                        + ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_ptw_a)
                                            ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__wptw_q))
                                            : 0ULL))
                                     : ((5U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
                                         ? ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_actual_a)
                                             ? 1ULL
                                             : 0ULL)
                                         : 0ULL))))))
             : 0ULL) + ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stale_cyc)
                         ? vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__prev_main_q
                         : 0ULL));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_read 
        = (((vlTOPp->idma_research_policy_top__DOT__req[2U] 
             >> 0x11U) & (~ (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                             >> 0x10U))) & (0x50U == 
                                            (0xffU 
                                             & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                >> 8U))));
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_read) {
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_q);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_q 
                       >> 0x20U));
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__locked_write) {
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_locked_write) {
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U] = 0U;
    }
    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) 
         & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_next_id_read))) {
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U] = 0U;
    }
    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_access) {
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U] = 0U;
        if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) 
             & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                >> 0x10U))) {
            vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                = (1U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
        } else {
            if (((((((((0x80U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                           >> 8U))) 
                       | (0x88U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                             >> 8U)))) 
                      | (0x90U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                            >> 8U)))) 
                     | (0x98U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                           >> 8U)))) 
                    | (0xa0U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                          >> 8U)))) 
                   | (0xa8U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                         >> 8U)))) 
                  | (0xd0U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                        >> 8U)))) | 
                 (0xd8U == (0xffU & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                     >> 8U))))) {
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U] 
                    = (IData)(((0x80U == (0xffU & (
                                                   vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                   >> 8U)))
                                ? 0x49444d4142544301ULL
                                : ((0x88U == (0xffU 
                                              & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                 >> 8U)))
                                    ? (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_error_q)) 
                                        << 0x20U) | (QData)((IData)(
                                                                    (((((0ULL 
                                                                         != vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q) 
                                                                        & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q 
                                                                           == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q)) 
                                                                       & (~ (IData)(vlTOPp->batch_active_o))) 
                                                                      << 3U) 
                                                                     | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stopped_q) 
                                                                         << 2U) 
                                                                        | (((IData)(vlTOPp->batch_active_o) 
                                                                            << 1U) 
                                                                           | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)))))))
                                    : ((0x90U == (0xffU 
                                                  & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                     >> 8U)))
                                        ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q
                                        : ((0x98U == 
                                            (0xffU 
                                             & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                >> 8U)))
                                            ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q))
                                            : ((0xa0U 
                                                == 
                                                (0xffU 
                                                 & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                    >> 8U)))
                                                ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q
                                                : (
                                                   (0xa8U 
                                                    == 
                                                    (0xffU 
                                                     & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                        >> 8U)))
                                                    ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_issued_q
                                                    : 
                                                   ((0xd0U 
                                                     == 
                                                     (0xffU 
                                                      & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                         >> 8U)))
                                                     ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q))
                                                     : 0x49444d4147415001ULL))))))));
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U] 
                    = (IData)((((0x80U == (0xffU & 
                                           (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                            >> 8U)))
                                 ? 0x49444d4142544301ULL
                                 : ((0x88U == (0xffU 
                                               & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                  >> 8U)))
                                     ? (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_error_q)) 
                                         << 0x20U) 
                                        | (QData)((IData)(
                                                          (((((0ULL 
                                                               != vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q) 
                                                              & (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q 
                                                                 == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q)) 
                                                             & (~ (IData)(vlTOPp->batch_active_o))) 
                                                            << 3U) 
                                                           | (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stopped_q) 
                                                               << 2U) 
                                                              | (((IData)(vlTOPp->batch_active_o) 
                                                                  << 1U) 
                                                                 | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)))))))
                                     : ((0x90U == (0xffU 
                                                   & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                      >> 8U)))
                                         ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q
                                         : ((0x98U 
                                             == (0xffU 
                                                 & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                    >> 8U)))
                                             ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q))
                                             : ((0xa0U 
                                                 == 
                                                 (0xffU 
                                                  & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                     >> 8U)))
                                                 ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q
                                                 : 
                                                ((0xa8U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                      >> 8U)))
                                                  ? vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_issued_q
                                                  : 
                                                 ((0xd0U 
                                                   == 
                                                   (0xffU 
                                                    & (vlTOPp->idma_research_policy_top__DOT__req[2U] 
                                                       >> 8U)))
                                                   ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q))
                                                   : 0x49444d4147415001ULL))))))) 
                               >> 0x20U));
            } else {
                vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                    = (1U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
            }
        }
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U])) {
                    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                         & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok)))) {
                        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                            = (1U | vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
                    } else {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                                = ((1U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]) 
                                   | (0xfffffffeU & 
                                      ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                         & (IData)(vlTOPp->policy_enable_o)) 
                                        | (IData)(vlTOPp->ready_i)) 
                                       << 1U)));
                        }
                    }
                }
            }
        }
    }
    __Vtemp57[0U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__balance_q);
    __Vtemp57[1U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__balance_q 
                             >> 0x20U));
    __Vtemp57[2U] = (3U & (- (IData)((1U & (IData)(
                                                   (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__balance_q 
                                                    >> 0x3fU))))));
    VL_EXTEND_WQ(66,64, __Vtemp58, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__charge_now);
    VL_SUB_W(3, __Vtemp59, __Vtemp57, __Vtemp58);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[0U] 
        = __Vtemp59[0U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[1U] 
        = __Vtemp59[1U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[2U] 
        = (3U & __Vtemp59[2U]);
    vlTOPp->rdata_o = (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U])) 
                        << 0x20U) | (QData)((IData)(
                                                    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U])));
    vlTOPp->ready_o = (1U & (vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                             >> 1U));
    vlTOPp->error_o = (1U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp62, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_now);
    VL_ADD_W(3, __Vtemp63, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c, __Vtemp62);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[0U] 
        = __Vtemp63[0U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[1U] 
        = __Vtemp63[1U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[2U] 
        = (3U & __Vtemp63[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp67, (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cap_q)));
    __Vtemp68[0U] = __Vtemp67[0U];
    __Vtemp68[1U] = __Vtemp67[1U];
    __Vtemp68[2U] = (3U & __Vtemp67[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp69, (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cap_q)));
    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_en) 
         & VL_GTS_IWW(1,66,66, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r, __Vtemp68))) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[0U] 
            = __Vtemp69[0U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[1U] 
            = __Vtemp69[1U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[2U] 
            = (3U & __Vtemp69[2U]);
    } else {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[0U] 
            = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[0U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[1U] 
            = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[1U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[2U] 
            = (3U & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[2U]);
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_ovf 
        = (((0U != (7U & ((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[2U] 
                           << 1U) | (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[1U] 
                                     >> 0x1fU)))) & 
            (7U != (7U & ((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[2U] 
                           << 1U) | (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[1U] 
                                     >> 0x1fU))))) 
           | ((0U != (7U & ((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[2U] 
                             << 1U) | (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[1U] 
                                       >> 0x1fU)))) 
              & (7U != (7U & ((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[2U] 
                               << 1U) | (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[1U] 
                                         >> 0x1fU))))));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_next 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_ovf)
            ? ((2U & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[2U])
                ? 0x8000000000000000ULL : 0x7fffffffffffffffULL)
            : (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[1U])) 
                << 0x20U) | (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[0U]))));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__allow_core 
        = ((0U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q) 
           | ((6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
               ? (0ULL == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__m6_cd_next)
               : VL_LTES_IQQ(1,64,64, 0ULL, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_next)));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[1U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[2U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[3U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[4U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[5U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[6U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[7U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[8U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[9U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xaU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xbU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xcU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xdU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xeU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xfU] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x10U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x11U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x12U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x13U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x14U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x15U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x16U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x17U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x18U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x19U] = 0ULL;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x19U] 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core)
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[5U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__abort_eff) 
                | (0ULL != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__err_q)) 
               | (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__allow_core))))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[6U] 
        = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__charge_now;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xaU] 
        = ((0ULL != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__charge_now)
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[7U] 
        = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_now;
    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
         & (IData)(vlTOPp->p_v_done_i))) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[1U] = 1ULL;
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[2U] = 0x1000ULL;
    } else {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[1U] = 0ULL;
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[2U] = 0ULL;
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[8U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->issue_o)) ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[9U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->done_o)) ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xbU] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_hit_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xcU] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xdU] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_ptw_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xeU] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (0xbU == vlTOPp->p_ctl_did_i)) ? 1ULL
            : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0xfU] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_sel_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x10U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x11U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x12U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_actual_a))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0U] 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete)
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x13U] 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last)
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x14U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last) 
            & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__act_all_now)))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x17U] 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__gap_load)
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x18U] 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stale_cyc)
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp73, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp74, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[3U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp73, __Vtemp74))
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp75, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                    << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp76, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[4U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp75, __Vtemp76))
            ? ((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q)) 
               - (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                  << 0xcU)) : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x15U] 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) 
             & (0U != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_inc))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[0x16U] 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) 
             & (0U != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_dec))
            ? 1ULL : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 0U;
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x1aU, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)) {
        if ((0x19U >= (0x1fU & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i))) {
            vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__inc 
                = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc
                [(0x1fU & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)];
            vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__c 
                = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_q
                [(0x1fU & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)];
        } else {
            vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__inc = 0ULL;
            vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__c = 0ULL;
        }
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__c;
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r = 0ULL;
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v)))));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 1U))))) 
                  << 1U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 2U))))) 
                  << 2U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 3U))))) 
                  << 3U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 4U))))) 
                  << 4U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 5U))))) 
                  << 5U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 6U))))) 
                  << 6U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 7U))))) 
                  << 7U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffeffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 8U))))) 
                  << 8U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffdffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 9U))))) 
                  << 9U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffffbffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0xaU))))) 
                  << 0xaU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffff7ffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0xbU))))) 
                  << 0xbU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffefffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0xcU))))) 
                  << 0xcU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffdfffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0xdU))))) 
                  << 0xdU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffffbfffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0xeU))))) 
                  << 0xeU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffff7fffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0xfU))))) 
                  << 0xfU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffeffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x10U))))) 
                  << 0x10U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffdffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x11U))))) 
                  << 0x11U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffffbffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x12U))))) 
                  << 0x12U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffff7ffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x13U))))) 
                  << 0x13U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffefffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x14U))))) 
                  << 0x14U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffdfffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x15U))))) 
                  << 0x15U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffffbfffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x16U))))) 
                  << 0x16U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffff7fffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x17U))))) 
                  << 0x17U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffeffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x18U))))) 
                  << 0x18U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffdffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x19U))))) 
                  << 0x19U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffffbffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x1aU))))) 
                  << 0x1aU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffff7ffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x1bU))))) 
                  << 0x1bU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffefffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x1cU))))) 
                  << 0x1cU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffdfffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x1dU))))) 
                  << 0x1dU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffffbfffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x1eU))))) 
                  << 0x1eU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffff7fffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x1fU))))) 
                  << 0x1fU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffeffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x20U))))) 
                  << 0x20U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffdffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x21U))))) 
                  << 0x21U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffffbffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x22U))))) 
                  << 0x22U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffff7ffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x23U))))) 
                  << 0x23U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffefffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x24U))))) 
                  << 0x24U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffdfffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x25U))))) 
                  << 0x25U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffffbfffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x26U))))) 
                  << 0x26U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffff7fffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x27U))))) 
                  << 0x27U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffeffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x28U))))) 
                  << 0x28U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffdffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x29U))))) 
                  << 0x29U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffffbffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x2aU))))) 
                  << 0x2aU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffff7ffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x2bU))))) 
                  << 0x2bU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffefffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x2cU))))) 
                  << 0x2cU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffdfffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x2dU))))) 
                  << 0x2dU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffffbfffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x2eU))))) 
                  << 0x2eU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffff7fffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x2fU))))) 
                  << 0x2fU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffeffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x30U))))) 
                  << 0x30U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffdffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x31U))))) 
                  << 0x31U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfffbffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x32U))))) 
                  << 0x32U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfff7ffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x33U))))) 
                  << 0x33U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffefffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x34U))))) 
                  << 0x34U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffdfffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x35U))))) 
                  << 0x35U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xffbfffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x36U))))) 
                  << 0x36U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xff7fffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x37U))))) 
                  << 0x37U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfeffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x38U))))) 
                  << 0x38U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfdffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x39U))))) 
                  << 0x39U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xfbffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x3aU))))) 
                  << 0x3aU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xf7ffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x3bU))))) 
                  << 0x3bU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xefffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x3cU))))) 
                  << 0x3cU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xdfffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x3dU))))) 
                  << 0x3dU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0xbfffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x3eU))))) 
                  << 0x3eU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r 
            = ((0x7fffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v 
                                                 >> 0x3fU))))) 
                  << 0x3fU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__Vfuncout 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r;
        VL_EXTEND_WQ(65,64, __Vtemp78, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp79, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__inc);
        VL_ADD_W(3, __Vtemp80, __Vtemp78, __Vtemp79);
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[0U] 
            = __Vtemp80[0U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[1U] 
            = __Vtemp80[1U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[2U] 
            = (1U & __Vtemp80[2U]);
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[0U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[0U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[1U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[1U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[2U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[2U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[0U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[0U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[1U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[1U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[2U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout[2U];
        if (VL_LIKELY((0x19U >= (0x1fU & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)))) {
            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum[(0x1fU 
                                                                            & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)][0U] 
                = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[0U];
            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum[(0x1fU 
                                                                            & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)][1U] 
                = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[1U];
            vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum[(0x1fU 
                                                                            & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i)][2U] 
                = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2[2U];
        }
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i 
            = ((IData)(1U) + vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i);
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x20U, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i)) {
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__inc 
            = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_inc
            [(0x1fU & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i)];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__c 
            = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_q
            [(0x1fU & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i)];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__c;
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r = 0ULL;
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v)))));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 1U))))) 
                  << 1U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 2U))))) 
                  << 2U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 3U))))) 
                  << 3U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 4U))))) 
                  << 4U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 5U))))) 
                  << 5U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 6U))))) 
                  << 6U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 7U))))) 
                  << 7U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffeffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 8U))))) 
                  << 8U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffdffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 9U))))) 
                  << 9U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffffbffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0xaU))))) 
                  << 0xaU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffff7ffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0xbU))))) 
                  << 0xbU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffefffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0xcU))))) 
                  << 0xcU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffdfffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0xdU))))) 
                  << 0xdU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffffbfffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0xeU))))) 
                  << 0xeU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffff7fffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0xfU))))) 
                  << 0xfU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffeffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x10U))))) 
                  << 0x10U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffdffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x11U))))) 
                  << 0x11U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffffbffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x12U))))) 
                  << 0x12U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffff7ffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x13U))))) 
                  << 0x13U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffefffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x14U))))) 
                  << 0x14U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffdfffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x15U))))) 
                  << 0x15U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffffbfffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x16U))))) 
                  << 0x16U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffff7fffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x17U))))) 
                  << 0x17U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffeffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x18U))))) 
                  << 0x18U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffdffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x19U))))) 
                  << 0x19U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffffbffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x1aU))))) 
                  << 0x1aU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffff7ffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x1bU))))) 
                  << 0x1bU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffefffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x1cU))))) 
                  << 0x1cU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffdfffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x1dU))))) 
                  << 0x1dU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffffbfffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x1eU))))) 
                  << 0x1eU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffff7fffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x1fU))))) 
                  << 0x1fU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffeffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x20U))))) 
                  << 0x20U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffdffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x21U))))) 
                  << 0x21U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffffbffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x22U))))) 
                  << 0x22U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffff7ffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x23U))))) 
                  << 0x23U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffefffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x24U))))) 
                  << 0x24U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffdfffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x25U))))) 
                  << 0x25U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffffbfffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x26U))))) 
                  << 0x26U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffff7fffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x27U))))) 
                  << 0x27U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffeffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x28U))))) 
                  << 0x28U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffdffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x29U))))) 
                  << 0x29U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffffbffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x2aU))))) 
                  << 0x2aU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffff7ffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x2bU))))) 
                  << 0x2bU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffefffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x2cU))))) 
                  << 0x2cU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffdfffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x2dU))))) 
                  << 0x2dU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffffbfffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x2eU))))) 
                  << 0x2eU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffff7fffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x2fU))))) 
                  << 0x2fU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffeffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x30U))))) 
                  << 0x30U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffdffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x31U))))) 
                  << 0x31U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfffbffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x32U))))) 
                  << 0x32U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfff7ffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x33U))))) 
                  << 0x33U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffefffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x34U))))) 
                  << 0x34U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffdfffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x35U))))) 
                  << 0x35U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xffbfffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x36U))))) 
                  << 0x36U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xff7fffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x37U))))) 
                  << 0x37U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfeffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x38U))))) 
                  << 0x38U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfdffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x39U))))) 
                  << 0x39U));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xfbffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x3aU))))) 
                  << 0x3aU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xf7ffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x3bU))))) 
                  << 0x3bU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xefffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x3cU))))) 
                  << 0x3cU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xdfffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x3dU))))) 
                  << 0x3dU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0xbfffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x3eU))))) 
                  << 0x3eU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r 
            = ((0x7fffffffffffffffULL & vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r) 
               | ((QData)((IData)((1U & (IData)((vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v 
                                                 >> 0x3fU))))) 
                  << 0x3fU));
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__Vfuncout 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r;
        VL_EXTEND_WQ(65,64, __Vtemp84, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp85, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__inc);
        VL_ADD_W(3, __Vtemp86, __Vtemp84, __Vtemp85);
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[0U] 
            = __Vtemp86[0U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[1U] 
            = __Vtemp86[1U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[2U] 
            = (1U & __Vtemp86[2U]);
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[0U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[0U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[1U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[1U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[2U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[2U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum[(0x1fU 
                                                                        & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i)][0U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[0U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum[(0x1fU 
                                                                        & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i)][1U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[1U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum[(0x1fU 
                                                                        & vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i)][2U] 
            = vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout[2U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i 
            = ((IData)(1U) + vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i);
    }
    __Vtemp89[0U] = 0xffffffffU;
    __Vtemp89[1U] = 0xffffffffU;
    __Vtemp89[2U] = 0U;
    __Vtemp90[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp90[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp90[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp89, __Vtemp90)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp91[0U] = 0xffffffffU;
    __Vtemp91[1U] = 0xffffffffU;
    __Vtemp91[2U] = 0U;
    __Vtemp92[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp92[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp92[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp91, __Vtemp92)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp93[0U] = 0xffffffffU;
    __Vtemp93[1U] = 0xffffffffU;
    __Vtemp93[2U] = 0U;
    __Vtemp94[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp94[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp94[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp93, __Vtemp94)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp95[0U] = 0xffffffffU;
    __Vtemp95[1U] = 0xffffffffU;
    __Vtemp95[2U] = 0U;
    __Vtemp96[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp96[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp96[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp95, __Vtemp96)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp97[0U] = 0xffffffffU;
    __Vtemp97[1U] = 0xffffffffU;
    __Vtemp97[2U] = 0U;
    __Vtemp98[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp98[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp98[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp97, __Vtemp98)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp99[0U] = 0xffffffffU;
    __Vtemp99[1U] = 0xffffffffU;
    __Vtemp99[2U] = 0U;
    __Vtemp100[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp100[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp100[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp99, __Vtemp100)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp101[0U] = 0xffffffffU;
    __Vtemp101[1U] = 0xffffffffU;
    __Vtemp101[2U] = 0U;
    __Vtemp102[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp102[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp102[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp101, __Vtemp102)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp103[0U] = 0xffffffffU;
    __Vtemp103[1U] = 0xffffffffU;
    __Vtemp103[2U] = 0U;
    __Vtemp104[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp104[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp104[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp103, __Vtemp104)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp105[0U] = 0xffffffffU;
    __Vtemp105[1U] = 0xffffffffU;
    __Vtemp105[2U] = 0U;
    __Vtemp106[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp106[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp106[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp105, __Vtemp106)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp107[0U] = 0xffffffffU;
    __Vtemp107[1U] = 0xffffffffU;
    __Vtemp107[2U] = 0U;
    __Vtemp108[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp108[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp108[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp107, __Vtemp108)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp109[0U] = 0xffffffffU;
    __Vtemp109[1U] = 0xffffffffU;
    __Vtemp109[2U] = 0U;
    __Vtemp110[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp110[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp110[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp109, __Vtemp110)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp111[0U] = 0xffffffffU;
    __Vtemp111[1U] = 0xffffffffU;
    __Vtemp111[2U] = 0U;
    __Vtemp112[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp112[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp112[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp111, __Vtemp112)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp113[0U] = 0xffffffffU;
    __Vtemp113[1U] = 0xffffffffU;
    __Vtemp113[2U] = 0U;
    __Vtemp114[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp114[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp114[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp113, __Vtemp114)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp115[0U] = 0xffffffffU;
    __Vtemp115[1U] = 0xffffffffU;
    __Vtemp115[2U] = 0U;
    __Vtemp116[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp116[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp116[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp115, __Vtemp116)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp117[0U] = 0xffffffffU;
    __Vtemp117[1U] = 0xffffffffU;
    __Vtemp117[2U] = 0U;
    __Vtemp118[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp118[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp118[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp117, __Vtemp118)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp119[0U] = 0xffffffffU;
    __Vtemp119[1U] = 0xffffffffU;
    __Vtemp119[2U] = 0U;
    __Vtemp120[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp120[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp120[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp119, __Vtemp120)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp121[0U] = 0xffffffffU;
    __Vtemp121[1U] = 0xffffffffU;
    __Vtemp121[2U] = 0U;
    __Vtemp122[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp122[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp122[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp121, __Vtemp122)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp123[0U] = 0xffffffffU;
    __Vtemp123[1U] = 0xffffffffU;
    __Vtemp123[2U] = 0U;
    __Vtemp124[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp124[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp124[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp123, __Vtemp124)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp125[0U] = 0xffffffffU;
    __Vtemp125[1U] = 0xffffffffU;
    __Vtemp125[2U] = 0U;
    __Vtemp126[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp126[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp126[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp125, __Vtemp126)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp127[0U] = 0xffffffffU;
    __Vtemp127[1U] = 0xffffffffU;
    __Vtemp127[2U] = 0U;
    __Vtemp128[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp128[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp128[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp127, __Vtemp128)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp129[0U] = 0xffffffffU;
    __Vtemp129[1U] = 0xffffffffU;
    __Vtemp129[2U] = 0U;
    __Vtemp130[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp130[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp130[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp129, __Vtemp130)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp131[0U] = 0xffffffffU;
    __Vtemp131[1U] = 0xffffffffU;
    __Vtemp131[2U] = 0U;
    __Vtemp132[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp132[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp132[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp131, __Vtemp132)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp133[0U] = 0xffffffffU;
    __Vtemp133[1U] = 0xffffffffU;
    __Vtemp133[2U] = 0U;
    __Vtemp134[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp134[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp134[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp133, __Vtemp134)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp135[0U] = 0xffffffffU;
    __Vtemp135[1U] = 0xffffffffU;
    __Vtemp135[2U] = 0U;
    __Vtemp136[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp136[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp136[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp135, __Vtemp136)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp137[0U] = 0xffffffffU;
    __Vtemp137[1U] = 0xffffffffU;
    __Vtemp137[2U] = 0U;
    __Vtemp138[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp138[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp138[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp137, __Vtemp138)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp139[0U] = 0xffffffffU;
    __Vtemp139[1U] = 0xffffffffU;
    __Vtemp139[2U] = 0U;
    __Vtemp140[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp140[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp140[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp139, __Vtemp140)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp141[0U] = 0xffffffffU;
    __Vtemp141[1U] = 0xffffffffU;
    __Vtemp141[2U] = 0U;
    __Vtemp142[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp142[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp142[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp141, __Vtemp142)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp143[0U] = 0xffffffffU;
    __Vtemp143[1U] = 0xffffffffU;
    __Vtemp143[2U] = 0U;
    __Vtemp144[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp144[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp144[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp143, __Vtemp144)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp145[0U] = 0xffffffffU;
    __Vtemp145[1U] = 0xffffffffU;
    __Vtemp145[2U] = 0U;
    __Vtemp146[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp146[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp146[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp145, __Vtemp146)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp147[0U] = 0xffffffffU;
    __Vtemp147[1U] = 0xffffffffU;
    __Vtemp147[2U] = 0U;
    __Vtemp148[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp148[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp148[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp147, __Vtemp148)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp149[0U] = 0xffffffffU;
    __Vtemp149[1U] = 0xffffffffU;
    __Vtemp149[2U] = 0U;
    __Vtemp150[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp150[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp150[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp149, __Vtemp150)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp151[0U] = 0xffffffffU;
    __Vtemp151[1U] = 0xffffffffU;
    __Vtemp151[2U] = 0U;
    __Vtemp152[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp152[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp152[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp151, __Vtemp152)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp153[0U] = 0xffffffffU;
    __Vtemp153[1U] = 0xffffffffU;
    __Vtemp153[2U] = 0U;
    __Vtemp154[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp154[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp154[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp153, __Vtemp154)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp155[0U] = 0xffffffffU;
    __Vtemp155[1U] = 0xffffffffU;
    __Vtemp155[2U] = 0U;
    __Vtemp156[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp156[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp156[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp155, __Vtemp156)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp157[0U] = 0xffffffffU;
    __Vtemp157[1U] = 0xffffffffU;
    __Vtemp157[2U] = 0U;
    __Vtemp158[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp158[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp158[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp157, __Vtemp158)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp159[0U] = 0xffffffffU;
    __Vtemp159[1U] = 0xffffffffU;
    __Vtemp159[2U] = 0U;
    __Vtemp160[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp160[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp160[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp159, __Vtemp160)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp161[0U] = 0xffffffffU;
    __Vtemp161[1U] = 0xffffffffU;
    __Vtemp161[2U] = 0U;
    __Vtemp162[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp162[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp162[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp161, __Vtemp162)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp163[0U] = 0xffffffffU;
    __Vtemp163[1U] = 0xffffffffU;
    __Vtemp163[2U] = 0U;
    __Vtemp164[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp164[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp164[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp163, __Vtemp164)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp165[0U] = 0xffffffffU;
    __Vtemp165[1U] = 0xffffffffU;
    __Vtemp165[2U] = 0U;
    __Vtemp166[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp166[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp166[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp165, __Vtemp166)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp167[0U] = 0xffffffffU;
    __Vtemp167[1U] = 0xffffffffU;
    __Vtemp167[2U] = 0U;
    __Vtemp168[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp168[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp168[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp167, __Vtemp168)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp169[0U] = 0xffffffffU;
    __Vtemp169[1U] = 0xffffffffU;
    __Vtemp169[2U] = 0U;
    __Vtemp170[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp170[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp170[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp169, __Vtemp170)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp171[0U] = 0xffffffffU;
    __Vtemp171[1U] = 0xffffffffU;
    __Vtemp171[2U] = 0U;
    __Vtemp172[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp172[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp172[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp171, __Vtemp172)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp173[0U] = 0xffffffffU;
    __Vtemp173[1U] = 0xffffffffU;
    __Vtemp173[2U] = 0U;
    __Vtemp174[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp174[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp174[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp173, __Vtemp174)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp175[0U] = 0xffffffffU;
    __Vtemp175[1U] = 0xffffffffU;
    __Vtemp175[2U] = 0U;
    __Vtemp176[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp176[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp176[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp175, __Vtemp176)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp177[0U] = 0xffffffffU;
    __Vtemp177[1U] = 0xffffffffU;
    __Vtemp177[2U] = 0U;
    __Vtemp178[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp178[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp178[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp177, __Vtemp178)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp179[0U] = 0xffffffffU;
    __Vtemp179[1U] = 0xffffffffU;
    __Vtemp179[2U] = 0U;
    __Vtemp180[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp180[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp180[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp179, __Vtemp180)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp181[0U] = 0xffffffffU;
    __Vtemp181[1U] = 0xffffffffU;
    __Vtemp181[2U] = 0U;
    __Vtemp182[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp182[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp182[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp181, __Vtemp182)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp183[0U] = 0xffffffffU;
    __Vtemp183[1U] = 0xffffffffU;
    __Vtemp183[2U] = 0U;
    __Vtemp184[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp184[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp184[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp183, __Vtemp184)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp185[0U] = 0xffffffffU;
    __Vtemp185[1U] = 0xffffffffU;
    __Vtemp185[2U] = 0U;
    __Vtemp186[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp186[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp186[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp185, __Vtemp186)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp187[0U] = 0xffffffffU;
    __Vtemp187[1U] = 0xffffffffU;
    __Vtemp187[2U] = 0U;
    __Vtemp188[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp188[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp188[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp187, __Vtemp188)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp189[0U] = 0xffffffffU;
    __Vtemp189[1U] = 0xffffffffU;
    __Vtemp189[2U] = 0U;
    __Vtemp190[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp190[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp190[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp189, __Vtemp190)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp191[0U] = 0xffffffffU;
    __Vtemp191[1U] = 0xffffffffU;
    __Vtemp191[2U] = 0U;
    __Vtemp192[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp192[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp192[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp191, __Vtemp192)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp193[0U] = 0xffffffffU;
    __Vtemp193[1U] = 0xffffffffU;
    __Vtemp193[2U] = 0U;
    __Vtemp194[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp194[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp194[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp193, __Vtemp194)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp195[0U] = 0xffffffffU;
    __Vtemp195[1U] = 0xffffffffU;
    __Vtemp195[2U] = 0U;
    __Vtemp196[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp196[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp196[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp195, __Vtemp196)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp197[0U] = 0xffffffffU;
    __Vtemp197[1U] = 0xffffffffU;
    __Vtemp197[2U] = 0U;
    __Vtemp198[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp198[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp198[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp197, __Vtemp198)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp199[0U] = 0xffffffffU;
    __Vtemp199[1U] = 0xffffffffU;
    __Vtemp199[2U] = 0U;
    __Vtemp200[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp200[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp200[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp199, __Vtemp200)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp201[0U] = 0xffffffffU;
    __Vtemp201[1U] = 0xffffffffU;
    __Vtemp201[2U] = 0U;
    __Vtemp202[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp202[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp202[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp201, __Vtemp202)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp203[0U] = 0xffffffffU;
    __Vtemp203[1U] = 0xffffffffU;
    __Vtemp203[2U] = 0U;
    __Vtemp204[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp204[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp204[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp203, __Vtemp204)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    vlTOPp->policy_allow_o = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
                                & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__abort_eff))) 
                               & (~ ((0ULL != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__err_q) 
                                     | (((((0ULL != vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__new_err) 
                                           | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__obs_err_now)) 
                                          | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__sup_drop_now)) 
                                         | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf)) 
                                        | (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_ovf))))) 
                              & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__allow_core));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
                if ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_o))) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 0U;
                } else {
                    if (((IData)(vlTOPp->backend_idle_i) 
                         & (IData)(vlTOPp->policy_allow_o))) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 1U;
                    }
                }
            } else {
                if ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_o))) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_q)) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n 
                            = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->policy_enable_o))
                                ? 5U : 1U);
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n 
                        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                        & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q) 
                                           | (IData)(vlTOPp->policy_abort_o))))
                                     ? 0U : (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                              & (0U 
                                                 != vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q))
                                              ? 4U : 
                                             (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                               & (IData)(vlTOPp->policy_enable_o))
                                               ? 5U
                                               : 1U))));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 2U;
                }
            } else {
                if ((2U & vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                 & (IData)(vlTOPp->policy_enable_o))) {
                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 5U;
                            } else {
                                vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 1U;
                                if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q) 
                                     & (~ (IData)(vlTOPp->ready_i)))) {
                                    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_n = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Vidma_research_policy_top::_eval_initial(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_eval_initial\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vidma_research_policy_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::final\n"); );
    // Variables
    Vidma_research_policy_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vidma_research_policy_top::_eval_settle(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_eval_settle\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__1(vlSymsp);
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vidma_research_policy_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    valid_i = VL_RAND_RESET_I(1);
    write_i = VL_RAND_RESET_I(1);
    addr_i = VL_RAND_RESET_I(8);
    wdata_i = VL_RAND_RESET_Q(64);
    wstrb_i = VL_RAND_RESET_I(8);
    ready_i = VL_RAND_RESET_I(1);
    complete_i = VL_RAND_RESET_I(1);
    backend_idle_i = VL_RAND_RESET_I(1);
    batch_reverse_i = VL_RAND_RESET_I(1);
    rdata_o = VL_RAND_RESET_Q(64);
    ready_o = VL_RAND_RESET_I(1);
    error_o = VL_RAND_RESET_I(1);
    request_o = VL_RAND_RESET_I(1);
    length_o = VL_RAND_RESET_Q(64);
    src_o = VL_RAND_RESET_Q(64);
    dst_o = VL_RAND_RESET_Q(64);
    type_o = VL_RAND_RESET_I(2);
    batch_active_o = VL_RAND_RESET_I(1);
    issue_o = VL_RAND_RESET_I(1);
    done_o = VL_RAND_RESET_I(1);
    issue_id_o = VL_RAND_RESET_Q(64);
    done_id_o = VL_RAND_RESET_Q(64);
    p_cfg_valid_i = VL_RAND_RESET_I(1);
    p_cfg_write_i = VL_RAND_RESET_I(1);
    p_index_i = VL_RAND_RESET_I(13);
    p_wdata_i = VL_RAND_RESET_Q(64);
    p_rdata_o = VL_RAND_RESET_Q(64);
    p_quiet_i = VL_RAND_RESET_I(1);
    p_supported_i = VL_RAND_RESET_I(1);
    p_v_active_i = VL_RAND_RESET_I(1);
    p_v_done_i = VL_RAND_RESET_I(1);
    p_ctl_state_i = VL_RAND_RESET_I(3);
    p_ctl_did_i = VL_RAND_RESET_I(24);
    p_select_fire_i = VL_RAND_RESET_I(1);
    p_select_did_i = VL_RAND_RESET_I(24);
    p_lookup_i = VL_RAND_RESET_I(1);
    p_lookup_hit_i = VL_RAND_RESET_I(1);
    p_lookup_did_i = VL_RAND_RESET_I(24);
    p_ptw_r_fire_i = VL_RAND_RESET_I(1);
    p_response_error_i = VL_RAND_RESET_I(1);
    policy_enable_o = VL_RAND_RESET_I(1);
    policy_allow_o = VL_RAND_RESET_I(1);
    policy_abort_o = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(82, idma_research_policy_top__DOT__req);
    idma_research_policy_top__DOT__request = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(288, idma_research_policy_top__DOT____Vcellout__u_frontend__burst_req_o);
    VL_RAND_RESET_W(66, idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o);
    idma_research_policy_top__DOT__u_frontend__DOT__state_q = VL_RAND_RESET_I(3);
    idma_research_policy_top__DOT__u_frontend__DOT__state_n = VL_RAND_RESET_I(3);
    idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__transf_counter_n = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__transf_offset_n = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(524, idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw);
    VL_RAND_RESET_W(194, idma_research_policy_top__DOT__u_frontend__DOT__dma_hw2reg);
    idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_n = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__issue = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__trans_complete = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__id_retire = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__active_cycles_read = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(82, idma_research_policy_top__DOT__u_frontend__DOT__base_req);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_access = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__locked_write = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_enable_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_stopped_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_error_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_target_q = VL_RAND_RESET_Q(33);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_issued_q = VL_RAND_RESET_Q(33);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_completed_q = VL_RAND_RESET_Q(33);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_pages_q = VL_RAND_RESET_I(13);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_reverse_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_cfg_ok = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_task_issue = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_task_done = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__gap_cycles_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_frontend__DOT__gap_countdown_n = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_frontend__DOT__batch_stop_write = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__launch_next_id_read = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__launch_locked_write = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_n = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__launch_id_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__launch_id_n = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_rdata_next = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(66, idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_intf_rsp);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__src_addr_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__dst_addr_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__num_bytes_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_decouple_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_deburst_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_serialize_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_pv_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_cache_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__conf_ttype_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wdata_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__n_trans_we = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = VL_RAND_RESET_I(2);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = VL_RAND_RESET_I(4);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit = VL_RAND_RESET_I(10);
    idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__armed_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__frozen_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__aborted_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__err_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__mode_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__window_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__target_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__deadband_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__initbud_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__minbud_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__maxbud_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__step_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__cap_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__whit_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__wmiss_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__wptw_q = VL_RAND_RESET_I(32);
    idma_research_policy_top__DOT__u_policy__DOT__balance_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__b_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__bmin_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__bmax_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__prev_main_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__win_main_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__win_cnt_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__win_vdone_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__win_allact_q = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__nodone_run_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__nodone_max_q = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__m6_cd_q = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            idma_research_policy_top__DOT__u_policy__DOT__stat_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            idma_research_policy_top__DOT__u_policy__DOT__hist_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            idma_research_policy_top__DOT__u_policy__DOT__stat_inc[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<26; ++__Vi0) {
            VL_RAND_RESET_W(65, idma_research_policy_top__DOT__u_policy__DOT__stat_sum[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            idma_research_policy_top__DOT__u_policy__DOT__hist_inc[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            VL_RAND_RESET_W(65, idma_research_policy_top__DOT__u_policy__DOT__hist_sum[__Vi0]);
    }}
    idma_research_policy_top__DOT__u_policy__DOT__ev_sel_a = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__ev_lut_a = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__ev_lut_hit_a = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__ev_lut_miss_a = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__ev_ptw_a = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__ev_actual_a = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__win_last = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__act_all_now = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__win_complete = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__vdone_now = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__charge_now = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__refill_now = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__main_inc = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__stale_cyc = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__refill_en = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(66, idma_research_policy_top__DOT__u_policy__DOT__bal_c);
    VL_RAND_RESET_W(66, idma_research_policy_top__DOT__u_policy__DOT__bal_r);
    VL_RAND_RESET_W(66, idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped);
    idma_research_policy_top__DOT__u_policy__DOT__bal_ovf = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__bal_next = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__fb_inc = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__fb_dec = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__b_next = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, idma_research_policy_top__DOT__u_policy__DOT__b_up_s);
    VL_RAND_RESET_W(65, idma_research_policy_top__DOT__u_policy__DOT__b_dn_s);
    idma_research_policy_top__DOT__u_policy__DOT__m6_cd_next = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__gap_load = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__wr_any = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__wr_ok = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__obs_err_now = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__sup_drop_now = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__new_err = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__do_arm = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__do_abort = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__do_freeze = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__do_clear = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__cfg_wr = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__cfg_bad = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__enable_core = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__allow_core = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__abort_eff = VL_RAND_RESET_I(1);
    idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__nodone_max_next = VL_RAND_RESET_Q(64);
    idma_research_policy_top__DOT__u_policy__DOT__unnamedblk4__DOT__i = 0;
    idma_research_policy_top__DOT__u_policy__DOT__unnamedblk5__DOT__i = 0;
    VL_RAND_RESET_W(65, idma_research_policy_top__DOT__u_policy__DOT____Vlvbound2);
    idma_research_policy_top__DOT__u_policy__DOT____Vlvbound3 = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__Vfuncout);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__c = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__inc = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__r = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__Vfuncout);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__c = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__inc = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__4__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__4__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__4__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__5__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__5__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__5__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__6__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__6__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__6__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__7__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__7__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__7__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__8__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__8__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__8__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__9__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__9__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__9__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__10__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__10__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__10__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__11__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__11__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__11__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__12__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__12__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__12__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__13__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__13__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__13__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__14__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__14__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__14__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__15__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__15__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__15__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__16__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__16__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__16__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__17__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__17__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__17__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__18__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__18__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__18__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__19__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__19__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__19__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__20__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__20__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__20__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__21__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__21__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__21__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__22__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__22__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__22__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__23__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__23__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__23__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__24__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__24__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__24__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__25__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__25__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__25__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__26__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__26__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__26__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__27__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__27__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__27__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__28__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__28__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__28__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__29__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__29__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__29__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__30__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__30__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__30__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__31__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__31__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__31__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__32__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__32__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__32__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__33__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__33__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__33__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__34__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__34__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__34__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__35__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__35__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__35__r = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__36__Vfuncout = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__36__v = VL_RAND_RESET_Q(64);
    __Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__36__r = VL_RAND_RESET_Q(64);
}
