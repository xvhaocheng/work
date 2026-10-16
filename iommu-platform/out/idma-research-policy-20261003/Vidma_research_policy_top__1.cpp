// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_research_policy_top.h for the primary calling header

#include "Vidma_research_policy_top.h"
#include "Vidma_research_policy_top__Syms.h"

VL_INLINE_OPT void Vidma_research_policy_top::_combo__TOP__5(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_combo__TOP__5\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp280[3];
    WData/*95:0*/ __Vtemp281[3];
    WData/*95:0*/ __Vtemp282[3];
    WData/*95:0*/ __Vtemp283[3];
    WData/*95:0*/ __Vtemp284[3];
    WData/*95:0*/ __Vtemp285[3];
    WData/*95:0*/ __Vtemp287[3];
    WData/*95:0*/ __Vtemp288[3];
    WData/*95:0*/ __Vtemp289[3];
    WData/*95:0*/ __Vtemp290[3];
    WData/*95:0*/ __Vtemp291[3];
    WData/*95:0*/ __Vtemp293[3];
    WData/*95:0*/ __Vtemp294[3];
    WData/*95:0*/ __Vtemp295[3];
    WData/*95:0*/ __Vtemp296[3];
    WData/*95:0*/ __Vtemp298[3];
    WData/*95:0*/ __Vtemp299[3];
    WData/*95:0*/ __Vtemp300[3];
    WData/*95:0*/ __Vtemp301[3];
    WData/*95:0*/ __Vtemp302[3];
    WData/*95:0*/ __Vtemp303[3];
    WData/*95:0*/ __Vtemp304[3];
    WData/*223:0*/ __Vtemp309[7];
    WData/*287:0*/ __Vtemp310[9];
    WData/*287:0*/ __Vtemp311[9];
    WData/*351:0*/ __Vtemp312[11];
    WData/*95:0*/ __Vtemp321[3];
    WData/*95:0*/ __Vtemp322[3];
    WData/*95:0*/ __Vtemp323[3];
    WData/*95:0*/ __Vtemp326[3];
    WData/*95:0*/ __Vtemp327[3];
    WData/*95:0*/ __Vtemp331[3];
    WData/*95:0*/ __Vtemp332[3];
    WData/*95:0*/ __Vtemp333[3];
    WData/*95:0*/ __Vtemp337[3];
    WData/*95:0*/ __Vtemp338[3];
    WData/*95:0*/ __Vtemp339[3];
    WData/*95:0*/ __Vtemp340[3];
    WData/*95:0*/ __Vtemp342[3];
    WData/*95:0*/ __Vtemp343[3];
    WData/*95:0*/ __Vtemp344[3];
    WData/*95:0*/ __Vtemp348[3];
    WData/*95:0*/ __Vtemp349[3];
    WData/*95:0*/ __Vtemp350[3];
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
    WData/*95:0*/ __Vtemp431[3];
    WData/*95:0*/ __Vtemp432[3];
    WData/*95:0*/ __Vtemp433[3];
    WData/*95:0*/ __Vtemp434[3];
    WData/*95:0*/ __Vtemp435[3];
    WData/*95:0*/ __Vtemp436[3];
    WData/*95:0*/ __Vtemp437[3];
    WData/*95:0*/ __Vtemp438[3];
    WData/*95:0*/ __Vtemp439[3];
    WData/*95:0*/ __Vtemp440[3];
    WData/*95:0*/ __Vtemp441[3];
    WData/*95:0*/ __Vtemp442[3];
    WData/*95:0*/ __Vtemp443[3];
    WData/*95:0*/ __Vtemp444[3];
    WData/*95:0*/ __Vtemp445[3];
    WData/*95:0*/ __Vtemp446[3];
    WData/*95:0*/ __Vtemp447[3];
    WData/*95:0*/ __Vtemp448[3];
    WData/*95:0*/ __Vtemp449[3];
    WData/*95:0*/ __Vtemp450[3];
    WData/*95:0*/ __Vtemp451[3];
    WData/*95:0*/ __Vtemp452[3];
    WData/*95:0*/ __Vtemp453[3];
    WData/*95:0*/ __Vtemp454[3];
    WData/*95:0*/ __Vtemp455[3];
    WData/*95:0*/ __Vtemp456[3];
    WData/*95:0*/ __Vtemp457[3];
    WData/*95:0*/ __Vtemp458[3];
    WData/*95:0*/ __Vtemp459[3];
    WData/*95:0*/ __Vtemp460[3];
    WData/*95:0*/ __Vtemp461[3];
    WData/*95:0*/ __Vtemp462[3];
    WData/*95:0*/ __Vtemp463[3];
    WData/*95:0*/ __Vtemp464[3];
    WData/*95:0*/ __Vtemp465[3];
    WData/*95:0*/ __Vtemp466[3];
    WData/*95:0*/ __Vtemp467[3];
    WData/*95:0*/ __Vtemp468[3];
    // Body
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_n 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_id_n 
        = vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_id_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__launch_id_n = 0ULL;
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
             & (IData)(vlTOPp->p_v_active_i)) & (~ (IData)(vlTOPp->p_v_done_i)))
            ? (1ULL + vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_q)
            : 0ULL);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_task_done 
        = (((IData)(vlTOPp->batch_active_o) & (2U == (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) 
           & (IData)(vlTOPp->complete_i));
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__batch_task_issue 
        = (((IData)(vlTOPp->batch_active_o) & (1U == (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__state_q))) 
           & (IData)(vlTOPp->ready_i));
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__main_inc 
        = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__enable_core) 
             & (7U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)) 
            & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__ev_actual_a))
            ? 1ULL : 0ULL);
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
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_last) 
           & (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__act_all_now));
    VL_EXTEND_WQ(66,64, __Vtemp280, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp281, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp282, __Vtemp280, __Vtemp281);
    __Vtemp283[0U] = __Vtemp282[0U];
    __Vtemp283[1U] = __Vtemp282[1U];
    __Vtemp283[2U] = (3U & __Vtemp282[2U]);
    VL_EXTEND_WI(66,32, __Vtemp284, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    VL_EXTEND_WQ(66,64, __Vtemp285, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp287, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp288, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp289, __Vtemp287, __Vtemp288);
    __Vtemp290[0U] = __Vtemp289[0U];
    __Vtemp290[1U] = __Vtemp289[1U];
    __Vtemp290[2U] = (3U & __Vtemp289[2U]);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_inc 
        = ((6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
            ? VL_LTS_IWW(1,66,66, __Vtemp283, __Vtemp284)
            : VL_GTS_IWW(1,66,66, __Vtemp285, __Vtemp290));
    VL_EXTEND_WQ(66,64, __Vtemp291, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp293, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp294, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp295, __Vtemp293, __Vtemp294);
    __Vtemp296[0U] = __Vtemp295[0U];
    __Vtemp296[1U] = __Vtemp295[1U];
    __Vtemp296[2U] = (3U & __Vtemp295[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp298, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp299, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp300, __Vtemp298, __Vtemp299);
    __Vtemp301[0U] = __Vtemp300[0U];
    __Vtemp301[1U] = __Vtemp300[1U];
    __Vtemp301[2U] = (3U & __Vtemp300[2U]);
    VL_EXTEND_WI(66,32, __Vtemp302, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_dec 
        = ((6U == vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__mode_q)
            ? VL_GTS_IWW(1,66,66, __Vtemp291, __Vtemp296)
            : VL_LTS_IWW(1,66,66, __Vtemp301, __Vtemp302));
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
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_max_next 
        = ((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next 
            > vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_max_q)
            ? vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_run_next
            : vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__nodone_max_q);
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
    VL_EXTEND_WI(65,32, __Vtemp303, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__maxbud_q);
    VL_EXTEND_WI(65,32, __Vtemp304, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__minbud_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_next 
        = ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_inc)
            ? (VL_GTS_IWW(1,65,65, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_up_s, __Vtemp303)
                ? (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__maxbud_q))
                : (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_up_s[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_up_s[0U]))))
            : ((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__fb_dec)
                ? (VL_LTS_IWW(1,65,65, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__b_dn_s, __Vtemp304)
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
    __Vtemp309[0U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp309[1U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                              >> 0x20U));
    __Vtemp309[2U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp309[3U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                              >> 0x20U));
    __Vtemp309[4U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp309[5U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                              >> 0x20U));
    __Vtemp309[6U] = ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 6U) & (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re)) 
                      & (~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp310, __Vtemp309);
    __Vtemp311[0U] = __Vtemp310[0U];
    __Vtemp311[1U] = __Vtemp310[1U];
    __Vtemp311[2U] = __Vtemp310[2U];
    __Vtemp311[3U] = __Vtemp310[3U];
    __Vtemp311[4U] = __Vtemp310[4U];
    __Vtemp311[5U] = __Vtemp310[5U];
    __Vtemp311[6U] = __Vtemp310[6U];
    __Vtemp311[7U] = __Vtemp310[7U];
    __Vtemp311[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                       >> 4U) & ((IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                 << 1U)) 
                                     & ((~ (IData)(vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                        << 1U))) | 
                      __Vtemp310[8U]);
    VL_EXTEND_WW(322,258, __Vtemp312, __Vtemp311);
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[0U] 
        = __Vtemp312[0U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[1U] 
        = __Vtemp312[1U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[2U] 
        = __Vtemp312[2U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[3U] 
        = __Vtemp312[3U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[4U] 
        = __Vtemp312[4U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[5U] 
        = __Vtemp312[5U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[6U] 
        = __Vtemp312[6U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[7U] 
        = __Vtemp312[7U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[8U] 
        = __Vtemp312[8U];
    vlTOPp->idma_research_policy_top__DOT__u_frontend__DOT__dma_reg2hw[9U] 
        = __Vtemp312[9U];
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
                                                       | __Vtemp312[0xaU])))))));
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
    __Vtemp321[0U] = (IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__balance_q);
    __Vtemp321[1U] = (IData)((vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__balance_q 
                              >> 0x20U));
    __Vtemp321[2U] = (3U & (- (IData)((1U & (IData)(
                                                    (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__balance_q 
                                                     >> 0x3fU))))));
    VL_EXTEND_WQ(66,64, __Vtemp322, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__charge_now);
    VL_SUB_W(3, __Vtemp323, __Vtemp321, __Vtemp322);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[0U] 
        = __Vtemp323[0U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[1U] 
        = __Vtemp323[1U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c[2U] 
        = (3U & __Vtemp323[2U]);
    vlTOPp->rdata_o = (((QData)((IData)(vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[1U])) 
                        << 0x20U) | (QData)((IData)(
                                                    vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[0U])));
    vlTOPp->ready_o = (1U & (vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U] 
                             >> 1U));
    vlTOPp->error_o = (1U & vlTOPp->idma_research_policy_top__DOT____Vcellout__u_frontend__dma_ctrl_rsp_o[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp326, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_now);
    VL_ADD_W(3, __Vtemp327, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_c, __Vtemp326);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[0U] 
        = __Vtemp327[0U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[1U] 
        = __Vtemp327[1U];
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r[2U] 
        = (3U & __Vtemp327[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp331, (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cap_q)));
    __Vtemp332[0U] = __Vtemp331[0U];
    __Vtemp332[1U] = __Vtemp331[1U];
    __Vtemp332[2U] = (3U & __Vtemp331[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp333, (QData)((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cap_q)));
    if (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__refill_en) 
         & VL_GTS_IWW(1,66,66, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r, __Vtemp332))) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[0U] 
            = __Vtemp333[0U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[1U] 
            = __Vtemp333[1U];
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__bal_r_clamped[2U] 
            = (3U & __Vtemp333[2U]);
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
    VL_EXTEND_WQ(66,64, __Vtemp337, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp338, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[3U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp337, __Vtemp338))
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp339, (vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp340, vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__target_q);
    vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_inc[4U] 
        = (((IData)(vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp339, __Vtemp340))
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
        VL_EXTEND_WQ(65,64, __Vtemp342, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__1__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp343, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__inc);
        VL_ADD_W(3, __Vtemp344, __Vtemp342, __Vtemp343);
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[0U] 
            = __Vtemp344[0U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[1U] 
            = __Vtemp344[1U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__0__w[2U] 
            = (1U & __Vtemp344[2U]);
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
        VL_EXTEND_WQ(65,64, __Vtemp348, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__zext64__3__Vfuncout);
        VL_EXTEND_WQ(65,64, __Vtemp349, vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__inc);
        VL_ADD_W(3, __Vtemp350, __Vtemp348, __Vtemp349);
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[0U] 
            = __Vtemp350[0U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[1U] 
            = __Vtemp350[1U];
        vlTOPp->__Vfunc_idma_research_policy_top__DOT__u_policy__DOT__cadd__2__w[2U] 
            = (1U & __Vtemp350[2U]);
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
    __Vtemp353[0U] = 0xffffffffU;
    __Vtemp353[1U] = 0xffffffffU;
    __Vtemp353[2U] = 0U;
    __Vtemp354[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp354[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp354[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp353, __Vtemp354)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp355[0U] = 0xffffffffU;
    __Vtemp355[1U] = 0xffffffffU;
    __Vtemp355[2U] = 0U;
    __Vtemp356[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp356[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp356[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp355, __Vtemp356)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp357[0U] = 0xffffffffU;
    __Vtemp357[1U] = 0xffffffffU;
    __Vtemp357[2U] = 0U;
    __Vtemp358[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp358[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp358[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp357, __Vtemp358)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp359[0U] = 0xffffffffU;
    __Vtemp359[1U] = 0xffffffffU;
    __Vtemp359[2U] = 0U;
    __Vtemp360[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp360[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp360[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp359, __Vtemp360)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp361[0U] = 0xffffffffU;
    __Vtemp361[1U] = 0xffffffffU;
    __Vtemp361[2U] = 0U;
    __Vtemp362[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp362[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp362[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp361, __Vtemp362)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp363[0U] = 0xffffffffU;
    __Vtemp363[1U] = 0xffffffffU;
    __Vtemp363[2U] = 0U;
    __Vtemp364[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp364[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp364[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp363, __Vtemp364)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp365[0U] = 0xffffffffU;
    __Vtemp365[1U] = 0xffffffffU;
    __Vtemp365[2U] = 0U;
    __Vtemp366[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp366[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp366[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp365, __Vtemp366)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp367[0U] = 0xffffffffU;
    __Vtemp367[1U] = 0xffffffffU;
    __Vtemp367[2U] = 0U;
    __Vtemp368[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp368[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp368[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp367, __Vtemp368)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp369[0U] = 0xffffffffU;
    __Vtemp369[1U] = 0xffffffffU;
    __Vtemp369[2U] = 0U;
    __Vtemp370[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp370[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp370[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp369, __Vtemp370)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp371[0U] = 0xffffffffU;
    __Vtemp371[1U] = 0xffffffffU;
    __Vtemp371[2U] = 0U;
    __Vtemp372[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp372[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp372[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp371, __Vtemp372)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp373[0U] = 0xffffffffU;
    __Vtemp373[1U] = 0xffffffffU;
    __Vtemp373[2U] = 0U;
    __Vtemp374[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp374[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp374[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp373, __Vtemp374)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp375[0U] = 0xffffffffU;
    __Vtemp375[1U] = 0xffffffffU;
    __Vtemp375[2U] = 0U;
    __Vtemp376[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp376[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp376[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp375, __Vtemp376)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp377[0U] = 0xffffffffU;
    __Vtemp377[1U] = 0xffffffffU;
    __Vtemp377[2U] = 0U;
    __Vtemp378[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp378[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp378[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp377, __Vtemp378)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp379[0U] = 0xffffffffU;
    __Vtemp379[1U] = 0xffffffffU;
    __Vtemp379[2U] = 0U;
    __Vtemp380[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp380[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp380[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp379, __Vtemp380)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp381[0U] = 0xffffffffU;
    __Vtemp381[1U] = 0xffffffffU;
    __Vtemp381[2U] = 0U;
    __Vtemp382[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp382[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp382[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp381, __Vtemp382)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp383[0U] = 0xffffffffU;
    __Vtemp383[1U] = 0xffffffffU;
    __Vtemp383[2U] = 0U;
    __Vtemp384[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp384[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp384[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp383, __Vtemp384)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp385[0U] = 0xffffffffU;
    __Vtemp385[1U] = 0xffffffffU;
    __Vtemp385[2U] = 0U;
    __Vtemp386[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp386[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp386[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp385, __Vtemp386)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp387[0U] = 0xffffffffU;
    __Vtemp387[1U] = 0xffffffffU;
    __Vtemp387[2U] = 0U;
    __Vtemp388[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp388[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp388[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp387, __Vtemp388)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp389[0U] = 0xffffffffU;
    __Vtemp389[1U] = 0xffffffffU;
    __Vtemp389[2U] = 0U;
    __Vtemp390[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp390[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp390[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp389, __Vtemp390)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp391[0U] = 0xffffffffU;
    __Vtemp391[1U] = 0xffffffffU;
    __Vtemp391[2U] = 0U;
    __Vtemp392[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp392[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp392[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp391, __Vtemp392)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp393[0U] = 0xffffffffU;
    __Vtemp393[1U] = 0xffffffffU;
    __Vtemp393[2U] = 0U;
    __Vtemp394[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp394[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp394[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp393, __Vtemp394)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp395[0U] = 0xffffffffU;
    __Vtemp395[1U] = 0xffffffffU;
    __Vtemp395[2U] = 0U;
    __Vtemp396[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp396[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp396[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp395, __Vtemp396)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp397[0U] = 0xffffffffU;
    __Vtemp397[1U] = 0xffffffffU;
    __Vtemp397[2U] = 0U;
    __Vtemp398[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp398[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp398[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp397, __Vtemp398)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp399[0U] = 0xffffffffU;
    __Vtemp399[1U] = 0xffffffffU;
    __Vtemp399[2U] = 0U;
    __Vtemp400[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp400[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp400[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp399, __Vtemp400)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp401[0U] = 0xffffffffU;
    __Vtemp401[1U] = 0xffffffffU;
    __Vtemp401[2U] = 0U;
    __Vtemp402[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp402[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp402[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp401, __Vtemp402)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp403[0U] = 0xffffffffU;
    __Vtemp403[1U] = 0xffffffffU;
    __Vtemp403[2U] = 0U;
    __Vtemp404[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp404[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp404[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp403, __Vtemp404)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp405[0U] = 0xffffffffU;
    __Vtemp405[1U] = 0xffffffffU;
    __Vtemp405[2U] = 0U;
    __Vtemp406[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp406[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp406[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp405, __Vtemp406)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp407[0U] = 0xffffffffU;
    __Vtemp407[1U] = 0xffffffffU;
    __Vtemp407[2U] = 0U;
    __Vtemp408[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp408[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp408[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp407, __Vtemp408)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp409[0U] = 0xffffffffU;
    __Vtemp409[1U] = 0xffffffffU;
    __Vtemp409[2U] = 0U;
    __Vtemp410[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp410[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp410[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp409, __Vtemp410)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp411[0U] = 0xffffffffU;
    __Vtemp411[1U] = 0xffffffffU;
    __Vtemp411[2U] = 0U;
    __Vtemp412[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp412[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp412[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp411, __Vtemp412)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp413[0U] = 0xffffffffU;
    __Vtemp413[1U] = 0xffffffffU;
    __Vtemp413[2U] = 0U;
    __Vtemp414[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp414[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp414[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp413, __Vtemp414)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp415[0U] = 0xffffffffU;
    __Vtemp415[1U] = 0xffffffffU;
    __Vtemp415[2U] = 0U;
    __Vtemp416[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp416[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp416[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp415, __Vtemp416)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp417[0U] = 0xffffffffU;
    __Vtemp417[1U] = 0xffffffffU;
    __Vtemp417[2U] = 0U;
    __Vtemp418[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp418[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp418[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp417, __Vtemp418)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp419[0U] = 0xffffffffU;
    __Vtemp419[1U] = 0xffffffffU;
    __Vtemp419[2U] = 0U;
    __Vtemp420[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp420[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp420[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp419, __Vtemp420)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp421[0U] = 0xffffffffU;
    __Vtemp421[1U] = 0xffffffffU;
    __Vtemp421[2U] = 0U;
    __Vtemp422[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp422[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp422[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp421, __Vtemp422)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp423[0U] = 0xffffffffU;
    __Vtemp423[1U] = 0xffffffffU;
    __Vtemp423[2U] = 0U;
    __Vtemp424[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp424[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp424[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp423, __Vtemp424)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp425[0U] = 0xffffffffU;
    __Vtemp425[1U] = 0xffffffffU;
    __Vtemp425[2U] = 0U;
    __Vtemp426[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp426[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp426[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp425, __Vtemp426)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp427[0U] = 0xffffffffU;
    __Vtemp427[1U] = 0xffffffffU;
    __Vtemp427[2U] = 0U;
    __Vtemp428[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp428[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp428[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp427, __Vtemp428)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp429[0U] = 0xffffffffU;
    __Vtemp429[1U] = 0xffffffffU;
    __Vtemp429[2U] = 0U;
    __Vtemp430[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp430[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp430[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp429, __Vtemp430)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp431[0U] = 0xffffffffU;
    __Vtemp431[1U] = 0xffffffffU;
    __Vtemp431[2U] = 0U;
    __Vtemp432[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp432[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp432[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp431, __Vtemp432)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp433[0U] = 0xffffffffU;
    __Vtemp433[1U] = 0xffffffffU;
    __Vtemp433[2U] = 0U;
    __Vtemp434[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp434[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp434[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp433, __Vtemp434)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp435[0U] = 0xffffffffU;
    __Vtemp435[1U] = 0xffffffffU;
    __Vtemp435[2U] = 0U;
    __Vtemp436[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp436[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp436[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp435, __Vtemp436)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp437[0U] = 0xffffffffU;
    __Vtemp437[1U] = 0xffffffffU;
    __Vtemp437[2U] = 0U;
    __Vtemp438[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp438[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp438[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp437, __Vtemp438)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp439[0U] = 0xffffffffU;
    __Vtemp439[1U] = 0xffffffffU;
    __Vtemp439[2U] = 0U;
    __Vtemp440[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp440[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp440[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp439, __Vtemp440)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp441[0U] = 0xffffffffU;
    __Vtemp441[1U] = 0xffffffffU;
    __Vtemp441[2U] = 0U;
    __Vtemp442[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp442[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp442[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp441, __Vtemp442)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp443[0U] = 0xffffffffU;
    __Vtemp443[1U] = 0xffffffffU;
    __Vtemp443[2U] = 0U;
    __Vtemp444[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp444[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp444[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp443, __Vtemp444)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp445[0U] = 0xffffffffU;
    __Vtemp445[1U] = 0xffffffffU;
    __Vtemp445[2U] = 0U;
    __Vtemp446[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp446[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp446[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp445, __Vtemp446)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp447[0U] = 0xffffffffU;
    __Vtemp447[1U] = 0xffffffffU;
    __Vtemp447[2U] = 0U;
    __Vtemp448[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp448[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp448[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp447, __Vtemp448)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp449[0U] = 0xffffffffU;
    __Vtemp449[1U] = 0xffffffffU;
    __Vtemp449[2U] = 0U;
    __Vtemp450[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp450[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp450[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp449, __Vtemp450)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp451[0U] = 0xffffffffU;
    __Vtemp451[1U] = 0xffffffffU;
    __Vtemp451[2U] = 0U;
    __Vtemp452[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp452[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp452[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp451, __Vtemp452)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp453[0U] = 0xffffffffU;
    __Vtemp453[1U] = 0xffffffffU;
    __Vtemp453[2U] = 0U;
    __Vtemp454[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp454[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp454[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp453, __Vtemp454)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp455[0U] = 0xffffffffU;
    __Vtemp455[1U] = 0xffffffffU;
    __Vtemp455[2U] = 0U;
    __Vtemp456[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp456[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp456[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp455, __Vtemp456)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp457[0U] = 0xffffffffU;
    __Vtemp457[1U] = 0xffffffffU;
    __Vtemp457[2U] = 0U;
    __Vtemp458[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp458[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp458[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp457, __Vtemp458)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp459[0U] = 0xffffffffU;
    __Vtemp459[1U] = 0xffffffffU;
    __Vtemp459[2U] = 0U;
    __Vtemp460[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp460[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp460[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp459, __Vtemp460)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp461[0U] = 0xffffffffU;
    __Vtemp461[1U] = 0xffffffffU;
    __Vtemp461[2U] = 0U;
    __Vtemp462[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp462[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp462[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp461, __Vtemp462)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp463[0U] = 0xffffffffU;
    __Vtemp463[1U] = 0xffffffffU;
    __Vtemp463[2U] = 0U;
    __Vtemp464[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp464[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp464[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp463, __Vtemp464)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp465[0U] = 0xffffffffU;
    __Vtemp465[1U] = 0xffffffffU;
    __Vtemp465[2U] = 0U;
    __Vtemp466[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp466[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp466[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp465, __Vtemp466)) {
        vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp467[0U] = 0xffffffffU;
    __Vtemp467[1U] = 0xffffffffU;
    __Vtemp467[2U] = 0U;
    __Vtemp468[0U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp468[1U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp468[2U] = vlTOPp->idma_research_policy_top__DOT__u_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp467, __Vtemp468)) {
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

void Vidma_research_policy_top::_eval(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_eval\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__3(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    vlTOPp->_combo__TOP__5(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vidma_research_policy_top::_change_request(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_change_request\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vidma_research_policy_top::_change_request_1(Vidma_research_policy_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_change_request_1\n"); );
    Vidma_research_policy_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vidma_research_policy_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_policy_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((valid_i & 0xfeU))) {
        Verilated::overWidthError("valid_i");}
    if (VL_UNLIKELY((write_i & 0xfeU))) {
        Verilated::overWidthError("write_i");}
    if (VL_UNLIKELY((ready_i & 0xfeU))) {
        Verilated::overWidthError("ready_i");}
    if (VL_UNLIKELY((complete_i & 0xfeU))) {
        Verilated::overWidthError("complete_i");}
    if (VL_UNLIKELY((backend_idle_i & 0xfeU))) {
        Verilated::overWidthError("backend_idle_i");}
    if (VL_UNLIKELY((batch_reverse_i & 0xfeU))) {
        Verilated::overWidthError("batch_reverse_i");}
    if (VL_UNLIKELY((p_cfg_valid_i & 0xfeU))) {
        Verilated::overWidthError("p_cfg_valid_i");}
    if (VL_UNLIKELY((p_cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("p_cfg_write_i");}
    if (VL_UNLIKELY((p_index_i & 0xe000U))) {
        Verilated::overWidthError("p_index_i");}
    if (VL_UNLIKELY((p_quiet_i & 0xfeU))) {
        Verilated::overWidthError("p_quiet_i");}
    if (VL_UNLIKELY((p_supported_i & 0xfeU))) {
        Verilated::overWidthError("p_supported_i");}
    if (VL_UNLIKELY((p_v_active_i & 0xfeU))) {
        Verilated::overWidthError("p_v_active_i");}
    if (VL_UNLIKELY((p_v_done_i & 0xfeU))) {
        Verilated::overWidthError("p_v_done_i");}
    if (VL_UNLIKELY((p_ctl_state_i & 0xf8U))) {
        Verilated::overWidthError("p_ctl_state_i");}
    if (VL_UNLIKELY((p_ctl_did_i & 0xff000000U))) {
        Verilated::overWidthError("p_ctl_did_i");}
    if (VL_UNLIKELY((p_select_fire_i & 0xfeU))) {
        Verilated::overWidthError("p_select_fire_i");}
    if (VL_UNLIKELY((p_select_did_i & 0xff000000U))) {
        Verilated::overWidthError("p_select_did_i");}
    if (VL_UNLIKELY((p_lookup_i & 0xfeU))) {
        Verilated::overWidthError("p_lookup_i");}
    if (VL_UNLIKELY((p_lookup_hit_i & 0xfeU))) {
        Verilated::overWidthError("p_lookup_hit_i");}
    if (VL_UNLIKELY((p_lookup_did_i & 0xff000000U))) {
        Verilated::overWidthError("p_lookup_did_i");}
    if (VL_UNLIKELY((p_ptw_r_fire_i & 0xfeU))) {
        Verilated::overWidthError("p_ptw_r_fire_i");}
    if (VL_UNLIKELY((p_response_error_i & 0xfeU))) {
        Verilated::overWidthError("p_response_error_i");}
}
#endif  // VL_DEBUG
