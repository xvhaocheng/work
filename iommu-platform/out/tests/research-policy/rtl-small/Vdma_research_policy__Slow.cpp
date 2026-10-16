// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_policy.h for the primary calling header

#include "Vdma_research_policy.h"
#include "Vdma_research_policy__Syms.h"

//==========

VL_CTOR_IMP(Vdma_research_policy) {
    Vdma_research_policy__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_research_policy__Syms(this, name());
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_research_policy::__Vconfigure(Vdma_research_policy__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_research_policy::~Vdma_research_policy() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_research_policy::_settle__TOP__3(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_settle__TOP__3\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp525[3];
    WData/*95:0*/ __Vtemp526[3];
    WData/*95:0*/ __Vtemp527[3];
    WData/*95:0*/ __Vtemp530[3];
    WData/*95:0*/ __Vtemp531[3];
    WData/*95:0*/ __Vtemp532[3];
    WData/*95:0*/ __Vtemp535[3];
    WData/*95:0*/ __Vtemp536[3];
    WData/*95:0*/ __Vtemp537[3];
    WData/*95:0*/ __Vtemp538[3];
    WData/*95:0*/ __Vtemp539[3];
    WData/*95:0*/ __Vtemp540[3];
    WData/*95:0*/ __Vtemp542[3];
    WData/*95:0*/ __Vtemp543[3];
    WData/*95:0*/ __Vtemp544[3];
    WData/*95:0*/ __Vtemp545[3];
    WData/*95:0*/ __Vtemp546[3];
    WData/*95:0*/ __Vtemp548[3];
    WData/*95:0*/ __Vtemp549[3];
    WData/*95:0*/ __Vtemp550[3];
    WData/*95:0*/ __Vtemp551[3];
    WData/*95:0*/ __Vtemp553[3];
    WData/*95:0*/ __Vtemp554[3];
    WData/*95:0*/ __Vtemp555[3];
    WData/*95:0*/ __Vtemp556[3];
    WData/*95:0*/ __Vtemp557[3];
    WData/*95:0*/ __Vtemp558[3];
    WData/*95:0*/ __Vtemp559[3];
    WData/*95:0*/ __Vtemp561[3];
    WData/*95:0*/ __Vtemp562[3];
    WData/*95:0*/ __Vtemp563[3];
    WData/*95:0*/ __Vtemp566[3];
    WData/*95:0*/ __Vtemp567[3];
    WData/*95:0*/ __Vtemp571[3];
    WData/*95:0*/ __Vtemp572[3];
    WData/*95:0*/ __Vtemp573[3];
    WData/*95:0*/ __Vtemp577[3];
    WData/*95:0*/ __Vtemp578[3];
    WData/*95:0*/ __Vtemp579[3];
    WData/*95:0*/ __Vtemp580[3];
    WData/*95:0*/ __Vtemp582[3];
    WData/*95:0*/ __Vtemp583[3];
    WData/*95:0*/ __Vtemp584[3];
    WData/*95:0*/ __Vtemp587[3];
    WData/*95:0*/ __Vtemp588[3];
    WData/*95:0*/ __Vtemp589[3];
    WData/*95:0*/ __Vtemp592[3];
    WData/*95:0*/ __Vtemp593[3];
    WData/*95:0*/ __Vtemp594[3];
    WData/*95:0*/ __Vtemp597[3];
    WData/*95:0*/ __Vtemp598[3];
    WData/*95:0*/ __Vtemp599[3];
    WData/*95:0*/ __Vtemp602[3];
    WData/*95:0*/ __Vtemp603[3];
    WData/*95:0*/ __Vtemp604[3];
    WData/*95:0*/ __Vtemp607[3];
    WData/*95:0*/ __Vtemp608[3];
    WData/*95:0*/ __Vtemp609[3];
    WData/*95:0*/ __Vtemp612[3];
    WData/*95:0*/ __Vtemp613[3];
    WData/*95:0*/ __Vtemp614[3];
    WData/*95:0*/ __Vtemp617[3];
    WData/*95:0*/ __Vtemp618[3];
    WData/*95:0*/ __Vtemp619[3];
    WData/*95:0*/ __Vtemp622[3];
    WData/*95:0*/ __Vtemp623[3];
    WData/*95:0*/ __Vtemp624[3];
    WData/*95:0*/ __Vtemp627[3];
    WData/*95:0*/ __Vtemp628[3];
    WData/*95:0*/ __Vtemp629[3];
    WData/*95:0*/ __Vtemp632[3];
    WData/*95:0*/ __Vtemp633[3];
    WData/*95:0*/ __Vtemp634[3];
    WData/*95:0*/ __Vtemp637[3];
    WData/*95:0*/ __Vtemp638[3];
    WData/*95:0*/ __Vtemp639[3];
    WData/*95:0*/ __Vtemp642[3];
    WData/*95:0*/ __Vtemp643[3];
    WData/*95:0*/ __Vtemp644[3];
    WData/*95:0*/ __Vtemp647[3];
    WData/*95:0*/ __Vtemp648[3];
    WData/*95:0*/ __Vtemp649[3];
    WData/*95:0*/ __Vtemp652[3];
    WData/*95:0*/ __Vtemp653[3];
    WData/*95:0*/ __Vtemp654[3];
    WData/*95:0*/ __Vtemp657[3];
    WData/*95:0*/ __Vtemp658[3];
    WData/*95:0*/ __Vtemp659[3];
    WData/*95:0*/ __Vtemp662[3];
    WData/*95:0*/ __Vtemp663[3];
    WData/*95:0*/ __Vtemp664[3];
    WData/*95:0*/ __Vtemp667[3];
    WData/*95:0*/ __Vtemp668[3];
    WData/*95:0*/ __Vtemp669[3];
    WData/*95:0*/ __Vtemp672[3];
    WData/*95:0*/ __Vtemp673[3];
    WData/*95:0*/ __Vtemp674[3];
    WData/*95:0*/ __Vtemp677[3];
    WData/*95:0*/ __Vtemp678[3];
    WData/*95:0*/ __Vtemp679[3];
    WData/*95:0*/ __Vtemp682[3];
    WData/*95:0*/ __Vtemp683[3];
    WData/*95:0*/ __Vtemp684[3];
    WData/*95:0*/ __Vtemp687[3];
    WData/*95:0*/ __Vtemp688[3];
    WData/*95:0*/ __Vtemp689[3];
    WData/*95:0*/ __Vtemp692[3];
    WData/*95:0*/ __Vtemp693[3];
    WData/*95:0*/ __Vtemp694[3];
    WData/*95:0*/ __Vtemp697[3];
    WData/*95:0*/ __Vtemp698[3];
    WData/*95:0*/ __Vtemp699[3];
    WData/*95:0*/ __Vtemp702[3];
    WData/*95:0*/ __Vtemp703[3];
    WData/*95:0*/ __Vtemp704[3];
    WData/*95:0*/ __Vtemp707[3];
    WData/*95:0*/ __Vtemp708[3];
    WData/*95:0*/ __Vtemp709[3];
    WData/*95:0*/ __Vtemp712[3];
    WData/*95:0*/ __Vtemp713[3];
    WData/*95:0*/ __Vtemp714[3];
    WData/*95:0*/ __Vtemp717[3];
    WData/*95:0*/ __Vtemp718[3];
    WData/*95:0*/ __Vtemp719[3];
    WData/*95:0*/ __Vtemp722[3];
    WData/*95:0*/ __Vtemp723[3];
    WData/*95:0*/ __Vtemp724[3];
    WData/*95:0*/ __Vtemp727[3];
    WData/*95:0*/ __Vtemp728[3];
    WData/*95:0*/ __Vtemp729[3];
    WData/*95:0*/ __Vtemp732[3];
    WData/*95:0*/ __Vtemp733[3];
    WData/*95:0*/ __Vtemp734[3];
    WData/*95:0*/ __Vtemp737[3];
    WData/*95:0*/ __Vtemp738[3];
    WData/*95:0*/ __Vtemp739[3];
    WData/*95:0*/ __Vtemp742[3];
    WData/*95:0*/ __Vtemp743[3];
    WData/*95:0*/ __Vtemp744[3];
    WData/*95:0*/ __Vtemp747[3];
    WData/*95:0*/ __Vtemp748[3];
    WData/*95:0*/ __Vtemp749[3];
    WData/*95:0*/ __Vtemp752[3];
    WData/*95:0*/ __Vtemp753[3];
    WData/*95:0*/ __Vtemp754[3];
    WData/*95:0*/ __Vtemp757[3];
    WData/*95:0*/ __Vtemp758[3];
    WData/*95:0*/ __Vtemp759[3];
    WData/*95:0*/ __Vtemp762[3];
    WData/*95:0*/ __Vtemp763[3];
    WData/*95:0*/ __Vtemp764[3];
    WData/*95:0*/ __Vtemp767[3];
    WData/*95:0*/ __Vtemp768[3];
    WData/*95:0*/ __Vtemp769[3];
    WData/*95:0*/ __Vtemp772[3];
    WData/*95:0*/ __Vtemp773[3];
    WData/*95:0*/ __Vtemp774[3];
    WData/*95:0*/ __Vtemp777[3];
    WData/*95:0*/ __Vtemp778[3];
    WData/*95:0*/ __Vtemp779[3];
    WData/*95:0*/ __Vtemp782[3];
    WData/*95:0*/ __Vtemp783[3];
    WData/*95:0*/ __Vtemp784[3];
    WData/*95:0*/ __Vtemp787[3];
    WData/*95:0*/ __Vtemp788[3];
    WData/*95:0*/ __Vtemp789[3];
    WData/*95:0*/ __Vtemp792[3];
    WData/*95:0*/ __Vtemp793[3];
    WData/*95:0*/ __Vtemp794[3];
    WData/*95:0*/ __Vtemp797[3];
    WData/*95:0*/ __Vtemp798[3];
    WData/*95:0*/ __Vtemp799[3];
    WData/*95:0*/ __Vtemp802[3];
    WData/*95:0*/ __Vtemp803[3];
    WData/*95:0*/ __Vtemp804[3];
    WData/*95:0*/ __Vtemp807[3];
    WData/*95:0*/ __Vtemp808[3];
    WData/*95:0*/ __Vtemp809[3];
    WData/*95:0*/ __Vtemp812[3];
    WData/*95:0*/ __Vtemp813[3];
    WData/*95:0*/ __Vtemp814[3];
    WData/*95:0*/ __Vtemp817[3];
    WData/*95:0*/ __Vtemp818[3];
    WData/*95:0*/ __Vtemp819[3];
    WData/*95:0*/ __Vtemp822[3];
    WData/*95:0*/ __Vtemp823[3];
    WData/*95:0*/ __Vtemp824[3];
    WData/*95:0*/ __Vtemp827[3];
    WData/*95:0*/ __Vtemp828[3];
    WData/*95:0*/ __Vtemp829[3];
    WData/*95:0*/ __Vtemp832[3];
    WData/*95:0*/ __Vtemp833[3];
    WData/*95:0*/ __Vtemp834[3];
    WData/*95:0*/ __Vtemp837[3];
    WData/*95:0*/ __Vtemp838[3];
    WData/*95:0*/ __Vtemp839[3];
    WData/*95:0*/ __Vtemp842[3];
    WData/*95:0*/ __Vtemp843[3];
    WData/*95:0*/ __Vtemp844[3];
    WData/*95:0*/ __Vtemp847[3];
    WData/*95:0*/ __Vtemp848[3];
    WData/*95:0*/ __Vtemp849[3];
    WData/*95:0*/ __Vtemp852[3];
    WData/*95:0*/ __Vtemp853[3];
    WData/*95:0*/ __Vtemp854[3];
    WData/*95:0*/ __Vtemp857[3];
    WData/*95:0*/ __Vtemp858[3];
    WData/*95:0*/ __Vtemp859[3];
    WData/*95:0*/ __Vtemp862[3];
    WData/*95:0*/ __Vtemp863[3];
    WData/*95:0*/ __Vtemp864[3];
    WData/*95:0*/ __Vtemp867[3];
    WData/*95:0*/ __Vtemp868[3];
    WData/*95:0*/ __Vtemp869[3];
    WData/*95:0*/ __Vtemp871[3];
    WData/*95:0*/ __Vtemp872[3];
    WData/*95:0*/ __Vtemp873[3];
    WData/*95:0*/ __Vtemp874[3];
    WData/*95:0*/ __Vtemp875[3];
    WData/*95:0*/ __Vtemp876[3];
    WData/*95:0*/ __Vtemp877[3];
    WData/*95:0*/ __Vtemp878[3];
    WData/*95:0*/ __Vtemp879[3];
    WData/*95:0*/ __Vtemp880[3];
    WData/*95:0*/ __Vtemp881[3];
    WData/*95:0*/ __Vtemp882[3];
    WData/*95:0*/ __Vtemp883[3];
    WData/*95:0*/ __Vtemp884[3];
    WData/*95:0*/ __Vtemp885[3];
    WData/*95:0*/ __Vtemp886[3];
    WData/*95:0*/ __Vtemp887[3];
    WData/*95:0*/ __Vtemp888[3];
    WData/*95:0*/ __Vtemp889[3];
    WData/*95:0*/ __Vtemp890[3];
    WData/*95:0*/ __Vtemp891[3];
    WData/*95:0*/ __Vtemp892[3];
    WData/*95:0*/ __Vtemp893[3];
    WData/*95:0*/ __Vtemp894[3];
    WData/*95:0*/ __Vtemp895[3];
    WData/*95:0*/ __Vtemp896[3];
    WData/*95:0*/ __Vtemp897[3];
    WData/*95:0*/ __Vtemp898[3];
    WData/*95:0*/ __Vtemp899[3];
    WData/*95:0*/ __Vtemp900[3];
    WData/*95:0*/ __Vtemp901[3];
    WData/*95:0*/ __Vtemp902[3];
    WData/*95:0*/ __Vtemp903[3];
    WData/*95:0*/ __Vtemp904[3];
    WData/*95:0*/ __Vtemp905[3];
    WData/*95:0*/ __Vtemp906[3];
    WData/*95:0*/ __Vtemp907[3];
    WData/*95:0*/ __Vtemp908[3];
    WData/*95:0*/ __Vtemp909[3];
    WData/*95:0*/ __Vtemp910[3];
    WData/*95:0*/ __Vtemp911[3];
    WData/*95:0*/ __Vtemp912[3];
    WData/*95:0*/ __Vtemp913[3];
    WData/*95:0*/ __Vtemp914[3];
    WData/*95:0*/ __Vtemp915[3];
    WData/*95:0*/ __Vtemp916[3];
    WData/*95:0*/ __Vtemp917[3];
    WData/*95:0*/ __Vtemp918[3];
    WData/*95:0*/ __Vtemp919[3];
    WData/*95:0*/ __Vtemp920[3];
    WData/*95:0*/ __Vtemp921[3];
    WData/*95:0*/ __Vtemp922[3];
    WData/*95:0*/ __Vtemp923[3];
    WData/*95:0*/ __Vtemp924[3];
    WData/*95:0*/ __Vtemp925[3];
    WData/*95:0*/ __Vtemp926[3];
    WData/*95:0*/ __Vtemp927[3];
    WData/*95:0*/ __Vtemp928[3];
    WData/*95:0*/ __Vtemp929[3];
    WData/*95:0*/ __Vtemp930[3];
    WData/*95:0*/ __Vtemp931[3];
    WData/*95:0*/ __Vtemp932[3];
    WData/*95:0*/ __Vtemp933[3];
    WData/*95:0*/ __Vtemp934[3];
    WData/*95:0*/ __Vtemp935[3];
    WData/*95:0*/ __Vtemp936[3];
    WData/*95:0*/ __Vtemp937[3];
    WData/*95:0*/ __Vtemp938[3];
    WData/*95:0*/ __Vtemp939[3];
    WData/*95:0*/ __Vtemp940[3];
    WData/*95:0*/ __Vtemp941[3];
    WData/*95:0*/ __Vtemp942[3];
    WData/*95:0*/ __Vtemp943[3];
    WData/*95:0*/ __Vtemp944[3];
    WData/*95:0*/ __Vtemp945[3];
    WData/*95:0*/ __Vtemp946[3];
    WData/*95:0*/ __Vtemp947[3];
    WData/*95:0*/ __Vtemp948[3];
    WData/*95:0*/ __Vtemp949[3];
    WData/*95:0*/ __Vtemp950[3];
    WData/*95:0*/ __Vtemp951[3];
    WData/*95:0*/ __Vtemp952[3];
    WData/*95:0*/ __Vtemp953[3];
    WData/*95:0*/ __Vtemp954[3];
    WData/*95:0*/ __Vtemp955[3];
    WData/*95:0*/ __Vtemp956[3];
    WData/*95:0*/ __Vtemp957[3];
    WData/*95:0*/ __Vtemp958[3];
    WData/*95:0*/ __Vtemp959[3];
    WData/*95:0*/ __Vtemp960[3];
    WData/*95:0*/ __Vtemp961[3];
    WData/*95:0*/ __Vtemp962[3];
    WData/*95:0*/ __Vtemp963[3];
    WData/*95:0*/ __Vtemp964[3];
    WData/*95:0*/ __Vtemp965[3];
    WData/*95:0*/ __Vtemp966[3];
    WData/*95:0*/ __Vtemp967[3];
    WData/*95:0*/ __Vtemp968[3];
    WData/*95:0*/ __Vtemp969[3];
    WData/*95:0*/ __Vtemp970[3];
    WData/*95:0*/ __Vtemp971[3];
    WData/*95:0*/ __Vtemp972[3];
    WData/*95:0*/ __Vtemp973[3];
    WData/*95:0*/ __Vtemp974[3];
    WData/*95:0*/ __Vtemp975[3];
    WData/*95:0*/ __Vtemp976[3];
    WData/*95:0*/ __Vtemp977[3];
    WData/*95:0*/ __Vtemp978[3];
    WData/*95:0*/ __Vtemp979[3];
    WData/*95:0*/ __Vtemp980[3];
    WData/*95:0*/ __Vtemp981[3];
    WData/*95:0*/ __Vtemp982[3];
    WData/*95:0*/ __Vtemp983[3];
    WData/*95:0*/ __Vtemp984[3];
    WData/*95:0*/ __Vtemp985[3];
    WData/*95:0*/ __Vtemp986[3];
    // Body
    vlTOPp->dma_research_policy__DOT__wr_any = ((IData)(vlTOPp->cfg_valid_i) 
                                                & (IData)(vlTOPp->cfg_write_i));
    vlTOPp->dma_research_policy__DOT__ev_sel_a = ((IData)(vlTOPp->select_fire_i) 
                                                  & (0xbU 
                                                     == vlTOPp->select_did_i));
    vlTOPp->dma_research_policy__DOT__ev_ptw_a = ((IData)(vlTOPp->ptw_r_fire_i) 
                                                  & (0xbU 
                                                     == vlTOPp->ctl_did_i));
    vlTOPp->dma_research_policy__DOT__ev_actual_a = 
        (((0xbU == vlTOPp->ctl_did_i) & ((1U == (IData)(vlTOPp->ctl_state_i)) 
                                         | (2U == (IData)(vlTOPp->ctl_state_i)))) 
         & (4U != (IData)(vlTOPp->ctl_state_i)));
    vlTOPp->dma_research_policy__DOT__ev_lut_a = ((IData)(vlTOPp->lookup_i) 
                                                  & (0xbU 
                                                     == vlTOPp->lookup_did_i));
    vlTOPp->dma_research_policy__DOT__act_all_now = 
        ((IData)(vlTOPp->dma_research_policy__DOT__win_allact_q) 
         & (IData)(vlTOPp->v_active_i));
    vlTOPp->dma_research_policy__DOT__vdone_now = (vlTOPp->dma_research_policy__DOT__win_vdone_q 
                                                   + 
                                                   ((IData)(vlTOPp->v_done_i)
                                                     ? 1ULL
                                                     : 0ULL));
    VL_EXTEND_WQ(65,64, __Vtemp525, vlTOPp->dma_research_policy__DOT__b_q);
    VL_EXTEND_WI(65,32, __Vtemp526, vlTOPp->dma_research_policy__DOT__step_q);
    VL_ADD_W(3, __Vtemp527, __Vtemp525, __Vtemp526);
    vlTOPp->dma_research_policy__DOT__b_up_s[0U] = 
        __Vtemp527[0U];
    vlTOPp->dma_research_policy__DOT__b_up_s[1U] = 
        __Vtemp527[1U];
    vlTOPp->dma_research_policy__DOT__b_up_s[2U] = 
        (1U & __Vtemp527[2U]);
    VL_EXTEND_WQ(65,64, __Vtemp530, vlTOPp->dma_research_policy__DOT__b_q);
    VL_EXTEND_WI(65,32, __Vtemp531, vlTOPp->dma_research_policy__DOT__step_q);
    VL_SUB_W(3, __Vtemp532, __Vtemp530, __Vtemp531);
    vlTOPp->dma_research_policy__DOT__b_dn_s[0U] = 
        __Vtemp532[0U];
    vlTOPp->dma_research_policy__DOT__b_dn_s[1U] = 
        __Vtemp532[1U];
    vlTOPp->dma_research_policy__DOT__b_dn_s[2U] = 
        (1U & __Vtemp532[2U]);
    vlTOPp->dma_research_policy__DOT__cfg_bad = (((
                                                   (((((((7U 
                                                          < vlTOPp->dma_research_policy__DOT__mode_q) 
                                                         | (0U 
                                                            == vlTOPp->dma_research_policy__DOT__window_q)) 
                                                        | (0xffffffU 
                                                           < vlTOPp->dma_research_policy__DOT__window_q)) 
                                                       | ((QData)((IData)(vlTOPp->dma_research_policy__DOT__minbud_q)) 
                                                          > (QData)((IData)(vlTOPp->dma_research_policy__DOT__maxbud_q)))) 
                                                      | ((QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q)) 
                                                         < (QData)((IData)(vlTOPp->dma_research_policy__DOT__minbud_q)))) 
                                                     | ((QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q)) 
                                                        > (QData)((IData)(vlTOPp->dma_research_policy__DOT__maxbud_q)))) 
                                                    | (0U 
                                                       == vlTOPp->dma_research_policy__DOT__step_q)) 
                                                   | ((QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)) 
                                                      < (QData)((IData)(vlTOPp->dma_research_policy__DOT__maxbud_q)))) 
                                                  | (((0U 
                                                       != vlTOPp->dma_research_policy__DOT__mode_q) 
                                                      & (6U 
                                                         != vlTOPp->dma_research_policy__DOT__mode_q)) 
                                                     & (0U 
                                                        == vlTOPp->dma_research_policy__DOT__minbud_q))) 
                                                 | ((4U 
                                                     == vlTOPp->dma_research_policy__DOT__mode_q) 
                                                    & (0ULL 
                                                       == 
                                                       (((QData)((IData)(vlTOPp->dma_research_policy__DOT__whit_q)) 
                                                         + (QData)((IData)(vlTOPp->dma_research_policy__DOT__wmiss_q))) 
                                                        + (QData)((IData)(vlTOPp->dma_research_policy__DOT__wptw_q))))));
    vlTOPp->dma_research_policy__DOT__sup_drop_now 
        = ((IData)(vlTOPp->dma_research_policy__DOT__armed_q) 
           & (~ (IData)(vlTOPp->supported_i)));
    vlTOPp->dma_research_policy__DOT__obs_err_now = 
        (((IData)(vlTOPp->dma_research_policy__DOT__armed_q) 
          & (~ (IData)(vlTOPp->dma_research_policy__DOT__frozen_q))) 
         & (IData)(vlTOPp->response_error_i));
    vlTOPp->rdata_o = 0ULL;
    if ((0xeU == (0x1fU & ((IData)(vlTOPp->index_i) 
                           >> 8U)))) {
        if ((0x80U & (IData)(vlTOPp->index_i))) {
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v 
                = vlTOPp->dma_research_policy__DOT__hist_q
                [(0x1fU & (IData)(vlTOPp->index_i))];
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r = 0ULL;
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v)))));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 1U)))) 
                      << 1U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 2U)))) 
                      << 2U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 3U)))) 
                      << 3U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 4U)))) 
                      << 4U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 5U)))) 
                      << 5U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 6U)))) 
                      << 6U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r 
                = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r) 
                   | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__v) 
                                             >> 7U)))) 
                      << 7U));
            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__Vfuncout 
                = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__r;
            vlTOPp->rdata_o = (((0x40U <= (0xffU & (IData)(vlTOPp->index_i))) 
                                & (0x5fU >= (0xffU 
                                             & (IData)(vlTOPp->index_i))))
                                ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__4__Vfuncout
                                : 0ULL);
        } else {
            if ((0x40U & (IData)(vlTOPp->index_i))) {
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v 
                    = vlTOPp->dma_research_policy__DOT__hist_q
                    [(0x1fU & (IData)(vlTOPp->index_i))];
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r = 0ULL;
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v)))));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 1U)))) 
                          << 1U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 2U)))) 
                          << 2U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 3U)))) 
                          << 3U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 4U)))) 
                          << 4U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 5U)))) 
                          << 5U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 6U)))) 
                          << 6U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r 
                    = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r) 
                       | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__v) 
                                                 >> 7U)))) 
                          << 7U));
                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__Vfuncout 
                    = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__r;
                vlTOPp->rdata_o = (((0x40U <= (0xffU 
                                               & (IData)(vlTOPp->index_i))) 
                                    & (0x5fU >= (0xffU 
                                                 & (IData)(vlTOPp->index_i))))
                                    ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__5__Vfuncout
                                    : 0ULL);
            } else {
                if ((0x20U & (IData)(vlTOPp->index_i))) {
                    if ((0x10U & (IData)(vlTOPp->index_i))) {
                        if ((8U & (IData)(vlTOPp->index_i))) {
                            if ((4U & (IData)(vlTOPp->index_i))) {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v 
                                            = vlTOPp->dma_research_policy__DOT__hist_q
                                            [(0x1fU 
                                              & (IData)(vlTOPp->index_i))];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__r;
                                        vlTOPp->rdata_o 
                                            = (((0x40U 
                                                 <= 
                                                 (0xffU 
                                                  & (IData)(vlTOPp->index_i))) 
                                                & (0x5fU 
                                                   >= 
                                                   (0xffU 
                                                    & (IData)(vlTOPp->index_i))))
                                                ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__6__Vfuncout
                                                : 0ULL);
                                    } else {
                                        vlTOPp->rdata_o 
                                            = vlTOPp->dma_research_policy__DOT__nodone_max_q;
                                    }
                                } else {
                                    vlTOPp->rdata_o 
                                        = ((1U & (IData)(vlTOPp->index_i))
                                            ? vlTOPp->dma_research_policy__DOT__b_q
                                            : vlTOPp->dma_research_policy__DOT__bmax_q);
                                }
                            } else {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    vlTOPp->rdata_o 
                                        = ((1U & (IData)(vlTOPp->index_i))
                                            ? vlTOPp->dma_research_policy__DOT__bmin_q
                                            : vlTOPp->dma_research_policy__DOT__balance_q);
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x19U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__7__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x18U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__8__Vfuncout;
                                    }
                                }
                            }
                        } else {
                            if ((4U & (IData)(vlTOPp->index_i))) {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x17U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__9__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x16U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__10__Vfuncout;
                                    }
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x15U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__11__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x14U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__12__Vfuncout;
                                    }
                                }
                            } else {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x13U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__13__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x12U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__14__Vfuncout;
                                    }
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x11U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__15__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0x10U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__16__Vfuncout;
                                    }
                                }
                            }
                        }
                    } else {
                        if ((8U & (IData)(vlTOPp->index_i))) {
                            if ((4U & (IData)(vlTOPp->index_i))) {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0xfU];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__17__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0xeU];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__18__Vfuncout;
                                    }
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0xdU];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__19__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0xcU];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__20__Vfuncout;
                                    }
                                }
                            } else {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0xbU];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__21__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0xaU];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__22__Vfuncout;
                                    }
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [9U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__23__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [8U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__24__Vfuncout;
                                    }
                                }
                            }
                        } else {
                            if ((4U & (IData)(vlTOPp->index_i))) {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [7U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__25__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [6U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__26__Vfuncout;
                                    }
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [5U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__27__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [4U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__28__Vfuncout;
                                    }
                                }
                            } else {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [3U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__29__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [2U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__30__Vfuncout;
                                    }
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [1U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__31__Vfuncout;
                                    } else {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v 
                                            = vlTOPp->dma_research_policy__DOT__stat_q
                                            [0U];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__r;
                                        vlTOPp->rdata_o 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__32__Vfuncout;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if ((0x10U & (IData)(vlTOPp->index_i))) {
                        if ((8U & (IData)(vlTOPp->index_i))) {
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v 
                                = vlTOPp->dma_research_policy__DOT__hist_q
                                [(0x1fU & (IData)(vlTOPp->index_i))];
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r = 0ULL;
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xfffffffffffffffeULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v)))));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xfffffffffffffffdULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 1U)))) 
                                      << 1U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xfffffffffffffffbULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 2U)))) 
                                      << 2U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xfffffffffffffff7ULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 3U)))) 
                                      << 3U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xffffffffffffffefULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 4U)))) 
                                      << 4U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xffffffffffffffdfULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 5U)))) 
                                      << 5U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xffffffffffffffbfULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 6U)))) 
                                      << 6U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r 
                                = ((0xffffffffffffff7fULL 
                                    & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r) 
                                   | ((QData)((IData)(
                                                      (1U 
                                                       & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__v) 
                                                          >> 7U)))) 
                                      << 7U));
                            vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__Vfuncout 
                                = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__r;
                            vlTOPp->rdata_o = (((0x40U 
                                                 <= 
                                                 (0xffU 
                                                  & (IData)(vlTOPp->index_i))) 
                                                & (0x5fU 
                                                   >= 
                                                   (0xffU 
                                                    & (IData)(vlTOPp->index_i))))
                                                ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__33__Vfuncout
                                                : 0ULL);
                        } else {
                            if ((4U & (IData)(vlTOPp->index_i))) {
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v 
                                    = vlTOPp->dma_research_policy__DOT__hist_q
                                    [(0x1fU & (IData)(vlTOPp->index_i))];
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r = 0ULL;
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xfffffffffffffffeULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | (IData)((IData)(
                                                         (1U 
                                                          & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v)))));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xfffffffffffffffdULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 1U)))) 
                                          << 1U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xfffffffffffffffbULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 2U)))) 
                                          << 2U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xfffffffffffffff7ULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 3U)))) 
                                          << 3U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xffffffffffffffefULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 4U)))) 
                                          << 4U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xffffffffffffffdfULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 5U)))) 
                                          << 5U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xffffffffffffffbfULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 6U)))) 
                                          << 6U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r 
                                    = ((0xffffffffffffff7fULL 
                                        & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__v) 
                                                              >> 7U)))) 
                                          << 7U));
                                vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__Vfuncout 
                                    = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__r;
                                vlTOPp->rdata_o = (
                                                   ((0x40U 
                                                     <= 
                                                     (0xffU 
                                                      & (IData)(vlTOPp->index_i))) 
                                                    & (0x5fU 
                                                       >= 
                                                       (0xffU 
                                                        & (IData)(vlTOPp->index_i))))
                                                    ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__34__Vfuncout
                                                    : 0ULL);
                            } else {
                                if ((2U & (IData)(vlTOPp->index_i))) {
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v 
                                        = vlTOPp->dma_research_policy__DOT__hist_q
                                        [(0x1fU & (IData)(vlTOPp->index_i))];
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r = 0ULL;
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xfffffffffffffffeULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | (IData)((IData)(
                                                             (1U 
                                                              & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v)))));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xfffffffffffffffdULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 1U)))) 
                                              << 1U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xfffffffffffffffbULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 2U)))) 
                                              << 2U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xfffffffffffffff7ULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 3U)))) 
                                              << 3U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xffffffffffffffefULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 4U)))) 
                                              << 4U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xffffffffffffffdfULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 5U)))) 
                                              << 5U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xffffffffffffffbfULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 6U)))) 
                                              << 6U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r 
                                        = ((0xffffffffffffff7fULL 
                                            & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r) 
                                           | ((QData)((IData)(
                                                              (1U 
                                                               & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__v) 
                                                                  >> 7U)))) 
                                              << 7U));
                                    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__Vfuncout 
                                        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__r;
                                    vlTOPp->rdata_o 
                                        = (((0x40U 
                                             <= (0xffU 
                                                 & (IData)(vlTOPp->index_i))) 
                                            & (0x5fU 
                                               >= (0xffU 
                                                   & (IData)(vlTOPp->index_i))))
                                            ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__35__Vfuncout
                                            : 0ULL);
                                } else {
                                    if ((1U & (IData)(vlTOPp->index_i))) {
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v 
                                            = vlTOPp->dma_research_policy__DOT__hist_q
                                            [(0x1fU 
                                              & (IData)(vlTOPp->index_i))];
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r = 0ULL;
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xfffffffffffffffeULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | (IData)((IData)(
                                                                 (1U 
                                                                  & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v)))));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xfffffffffffffffdULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 1U)))) 
                                                  << 1U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xfffffffffffffffbULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 2U)))) 
                                                  << 2U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xfffffffffffffff7ULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 3U)))) 
                                                  << 3U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xffffffffffffffefULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 4U)))) 
                                                  << 4U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xffffffffffffffdfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 5U)))) 
                                                  << 5U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xffffffffffffffbfULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 6U)))) 
                                                  << 6U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r 
                                            = ((0xffffffffffffff7fULL 
                                                & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r) 
                                               | ((QData)((IData)(
                                                                  (1U 
                                                                   & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__v) 
                                                                      >> 7U)))) 
                                                  << 7U));
                                        vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__Vfuncout 
                                            = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__r;
                                        vlTOPp->rdata_o 
                                            = (((0x40U 
                                                 <= 
                                                 (0xffU 
                                                  & (IData)(vlTOPp->index_i))) 
                                                & (0x5fU 
                                                   >= 
                                                   (0xffU 
                                                    & (IData)(vlTOPp->index_i))))
                                                ? vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__36__Vfuncout
                                                : 0ULL);
                                    } else {
                                        vlTOPp->rdata_o 
                                            = (QData)((IData)(vlTOPp->dma_research_policy__DOT__wptw_q));
                                    }
                                }
                            }
                        }
                    } else {
                        vlTOPp->rdata_o = ((8U & (IData)(vlTOPp->index_i))
                                            ? ((4U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__wmiss_q))
                                                     : (QData)((IData)(vlTOPp->dma_research_policy__DOT__whit_q)))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q))
                                                     : (QData)((IData)(vlTOPp->dma_research_policy__DOT__step_q))))
                                                : (
                                                   (2U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__maxbud_q))
                                                     : (QData)((IData)(vlTOPp->dma_research_policy__DOT__minbud_q)))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q))
                                                     : (QData)((IData)(vlTOPp->dma_research_policy__DOT__deadband_q)))))
                                            : ((4U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__target_q))
                                                     : (QData)((IData)(vlTOPp->dma_research_policy__DOT__window_q)))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->dma_research_policy__DOT__mode_q))
                                                     : 0ULL))
                                                : (
                                                   (2U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? 
                                                    (((QData)((IData)(vlTOPp->dma_research_policy__DOT__aborted_q)) 
                                                      << 2U) 
                                                     | (QData)((IData)(
                                                                       (((IData)(vlTOPp->dma_research_policy__DOT__frozen_q) 
                                                                         << 1U) 
                                                                        | (IData)(vlTOPp->dma_research_policy__DOT__armed_q)))))
                                                     : vlTOPp->dma_research_policy__DOT__err_q)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? 1ULL
                                                     : 0x49444d414d435431ULL))));
                    }
                }
            }
        }
    }
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
    VL_EXTEND_WQ(66,64, __Vtemp535, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp536, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp537, __Vtemp535, __Vtemp536);
    __Vtemp538[0U] = __Vtemp537[0U];
    __Vtemp538[1U] = __Vtemp537[1U];
    __Vtemp538[2U] = (3U & __Vtemp537[2U]);
    VL_EXTEND_WI(66,32, __Vtemp539, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WQ(66,64, __Vtemp540, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp542, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp543, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp544, __Vtemp542, __Vtemp543);
    __Vtemp545[0U] = __Vtemp544[0U];
    __Vtemp545[1U] = __Vtemp544[1U];
    __Vtemp545[2U] = (3U & __Vtemp544[2U]);
    vlTOPp->dma_research_policy__DOT__fb_inc = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_LTS_IWW(1,66,66, __Vtemp538, __Vtemp539)
                                                 : 
                                                VL_GTS_IWW(1,66,66, __Vtemp540, __Vtemp545));
    VL_EXTEND_WQ(66,64, __Vtemp546, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp548, vlTOPp->dma_research_policy__DOT__target_q);
    VL_EXTEND_WI(66,32, __Vtemp549, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp550, __Vtemp548, __Vtemp549);
    __Vtemp551[0U] = __Vtemp550[0U];
    __Vtemp551[1U] = __Vtemp550[1U];
    __Vtemp551[2U] = (3U & __Vtemp550[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp553, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp554, vlTOPp->dma_research_policy__DOT__deadband_q);
    VL_ADD_W(3, __Vtemp555, __Vtemp553, __Vtemp554);
    __Vtemp556[0U] = __Vtemp555[0U];
    __Vtemp556[1U] = __Vtemp555[1U];
    __Vtemp556[2U] = (3U & __Vtemp555[2U]);
    VL_EXTEND_WI(66,32, __Vtemp557, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__fb_dec = ((6U 
                                                 == vlTOPp->dma_research_policy__DOT__mode_q)
                                                 ? 
                                                VL_GTS_IWW(1,66,66, __Vtemp546, __Vtemp551)
                                                 : 
                                                VL_LTS_IWW(1,66,66, __Vtemp556, __Vtemp557));
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
    VL_EXTEND_WI(65,32, __Vtemp558, vlTOPp->dma_research_policy__DOT__maxbud_q);
    VL_EXTEND_WI(65,32, __Vtemp559, vlTOPp->dma_research_policy__DOT__minbud_q);
    vlTOPp->dma_research_policy__DOT__b_next = ((IData)(vlTOPp->dma_research_policy__DOT__fb_inc)
                                                 ? 
                                                (VL_GTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_up_s, __Vtemp558)
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
                                                 (VL_LTS_IWW(1,65,65, vlTOPp->dma_research_policy__DOT__b_dn_s, __Vtemp559)
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
    vlTOPp->policy_abort_o = ((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                              & ((IData)(vlTOPp->dma_research_policy__DOT__abort_eff) 
                                 | (0ULL != vlTOPp->dma_research_policy__DOT__err_q)));
    __Vtemp561[0U] = (IData)(vlTOPp->dma_research_policy__DOT__balance_q);
    __Vtemp561[1U] = (IData)((vlTOPp->dma_research_policy__DOT__balance_q 
                              >> 0x20U));
    __Vtemp561[2U] = (3U & (- (IData)((1U & (IData)(
                                                    (vlTOPp->dma_research_policy__DOT__balance_q 
                                                     >> 0x3fU))))));
    VL_EXTEND_WQ(66,64, __Vtemp562, vlTOPp->dma_research_policy__DOT__charge_now);
    VL_SUB_W(3, __Vtemp563, __Vtemp561, __Vtemp562);
    vlTOPp->dma_research_policy__DOT__bal_c[0U] = __Vtemp563[0U];
    vlTOPp->dma_research_policy__DOT__bal_c[1U] = __Vtemp563[1U];
    vlTOPp->dma_research_policy__DOT__bal_c[2U] = (3U 
                                                   & __Vtemp563[2U]);
    VL_EXTEND_WQ(66,64, __Vtemp566, vlTOPp->dma_research_policy__DOT__refill_now);
    VL_ADD_W(3, __Vtemp567, vlTOPp->dma_research_policy__DOT__bal_c, __Vtemp566);
    vlTOPp->dma_research_policy__DOT__bal_r[0U] = __Vtemp567[0U];
    vlTOPp->dma_research_policy__DOT__bal_r[1U] = __Vtemp567[1U];
    vlTOPp->dma_research_policy__DOT__bal_r[2U] = (3U 
                                                   & __Vtemp567[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp571, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    __Vtemp572[0U] = __Vtemp571[0U];
    __Vtemp572[1U] = __Vtemp571[1U];
    __Vtemp572[2U] = (3U & __Vtemp571[2U]);
    VL_EXTENDS_WQ(66,34, __Vtemp573, (QData)((IData)(vlTOPp->dma_research_policy__DOT__cap_q)));
    if (((IData)(vlTOPp->dma_research_policy__DOT__refill_en) 
         & VL_GTS_IWW(1,66,66, vlTOPp->dma_research_policy__DOT__bal_r, __Vtemp572))) {
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[0U] 
            = __Vtemp573[0U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[1U] 
            = __Vtemp573[1U];
        vlTOPp->dma_research_policy__DOT__bal_r_clamped[2U] 
            = (3U & __Vtemp573[2U]);
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
    VL_EXTEND_WQ(66,64, __Vtemp577, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp578, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[3U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp577, __Vtemp578))
            ? 1ULL : 0ULL);
    VL_EXTEND_WQ(66,64, __Vtemp579, (vlTOPp->dma_research_policy__DOT__vdone_now 
                                     << 0xcU));
    VL_EXTEND_WI(66,32, __Vtemp580, vlTOPp->dma_research_policy__DOT__target_q);
    vlTOPp->dma_research_policy__DOT__stat_inc[4U] 
        = (((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
            & VL_LTS_IWW(1,66,66, __Vtemp579, __Vtemp580))
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
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp582, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp583, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp584, __Vtemp582, __Vtemp583);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp584[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp584[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp584[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp587, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp588, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp589, __Vtemp587, __Vtemp588);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp589[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp589[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp589[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[1U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[1U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[1U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp592, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp593, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp594, __Vtemp592, __Vtemp593);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp594[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp594[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp594[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[2U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[2U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[2U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [3U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[3U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp597, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp598, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp599, __Vtemp597, __Vtemp598);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp599[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp599[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp599[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[3U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[3U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[3U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [4U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[4U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp602, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp603, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp604, __Vtemp602, __Vtemp603);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp604[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp604[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp604[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[4U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[4U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[4U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [5U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[5U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp607, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp608, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp609, __Vtemp607, __Vtemp608);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp609[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp609[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp609[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[5U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[5U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[5U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [6U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[6U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp612, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp613, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp614, __Vtemp612, __Vtemp613);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp614[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp614[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp614[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[6U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[6U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[6U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [7U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[7U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp617, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp618, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp619, __Vtemp617, __Vtemp618);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp619[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp619[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp619[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[7U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[7U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[7U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [8U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[8U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp622, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp623, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp624, __Vtemp622, __Vtemp623);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp624[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp624[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp624[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[8U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[8U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[8U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [9U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[9U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp627, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp628, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp629, __Vtemp627, __Vtemp628);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp629[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp629[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp629[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[9U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[9U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[9U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0xaU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0xaU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp632, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp633, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp634, __Vtemp632, __Vtemp633);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp634[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp634[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp634[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0xaU][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xaU][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xaU][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0xbU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0xbU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp637, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp638, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp639, __Vtemp637, __Vtemp638);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp639[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp639[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp639[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0xbU][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xbU][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xbU][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0xcU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0xcU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp642, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp643, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp644, __Vtemp642, __Vtemp643);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp644[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp644[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp644[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0xcU][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xcU][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xcU][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0xdU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0xdU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp647, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp648, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp649, __Vtemp647, __Vtemp648);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp649[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp649[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp649[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0xdU][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xdU][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xdU][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0xeU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0xeU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp652, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp653, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp654, __Vtemp652, __Vtemp653);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp654[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp654[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp654[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0xeU][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xeU][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xeU][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0xfU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0xfU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp657, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp658, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp659, __Vtemp657, __Vtemp658);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp659[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp659[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp659[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0xfU][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xfU][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0xfU][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x10U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x10U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp662, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp663, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp664, __Vtemp662, __Vtemp663);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp664[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp664[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp664[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x10U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x10U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x10U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x11U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x11U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp667, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp668, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp669, __Vtemp667, __Vtemp668);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp669[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp669[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp669[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x11U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x11U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x11U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x12U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x12U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp672, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp673, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp674, __Vtemp672, __Vtemp673);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp674[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp674[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp674[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x12U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x12U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x12U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x13U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x13U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp677, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp678, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp679, __Vtemp677, __Vtemp678);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp679[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp679[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp679[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x13U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x13U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x13U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x14U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x14U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp682, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp683, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp684, __Vtemp682, __Vtemp683);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp684[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp684[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp684[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x14U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x14U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x14U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x15U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x15U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp687, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp688, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp689, __Vtemp687, __Vtemp688);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp689[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp689[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp689[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x15U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x15U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x15U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x16U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x16U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp692, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp693, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp694, __Vtemp692, __Vtemp693);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp694[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp694[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp694[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x16U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x16U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x16U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x17U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x17U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp697, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp698, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp699, __Vtemp697, __Vtemp698);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp699[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp699[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp699[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x17U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x17U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x17U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x18U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x18U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp702, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp703, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp704, __Vtemp702, __Vtemp703);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp704[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp704[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp704[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x18U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x18U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x18U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc 
        = vlTOPp->dma_research_policy__DOT__stat_inc
        [0x19U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c 
        = vlTOPp->dma_research_policy__DOT__stat_q[0x19U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__r;
    VL_EXTEND_WQ(65,64, __Vtemp707, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp708, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp709, __Vtemp707, __Vtemp708);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp709[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp709[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp709[2U]);
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
    vlTOPp->dma_research_policy__DOT__stat_sum[0x19U][0U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[0U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x19U][1U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[1U];
    vlTOPp->dma_research_policy__DOT__stat_sum[0x19U][2U] 
        = vlTOPp->dma_research_policy__DOT____Vlvbound2[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp712, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp713, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp714, __Vtemp712, __Vtemp713);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp714[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp714[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp714[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp717, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp718, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp719, __Vtemp717, __Vtemp718);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp719[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp719[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp719[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[1U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[1U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[1U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp722, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp723, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp724, __Vtemp722, __Vtemp723);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp724[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp724[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp724[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[2U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[2U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[2U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [3U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[3U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp727, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp728, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp729, __Vtemp727, __Vtemp728);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp729[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp729[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp729[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[3U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[3U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[3U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [4U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[4U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp732, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp733, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp734, __Vtemp732, __Vtemp733);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp734[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp734[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp734[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[4U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[4U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[4U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [5U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[5U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp737, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp738, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp739, __Vtemp737, __Vtemp738);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp739[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp739[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp739[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[5U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[5U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[5U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [6U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[6U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp742, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp743, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp744, __Vtemp742, __Vtemp743);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp744[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp744[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp744[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[6U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[6U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[6U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [7U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[7U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp747, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp748, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp749, __Vtemp747, __Vtemp748);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp749[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp749[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp749[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[7U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[7U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[7U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [8U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[8U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp752, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp753, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp754, __Vtemp752, __Vtemp753);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp754[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp754[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp754[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[8U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[8U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[8U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [9U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[9U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp757, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp758, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp759, __Vtemp757, __Vtemp758);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp759[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp759[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp759[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[9U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[9U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[9U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0xaU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0xaU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp762, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp763, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp764, __Vtemp762, __Vtemp763);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp764[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp764[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp764[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xaU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xaU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xaU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0xbU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0xbU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp767, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp768, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp769, __Vtemp767, __Vtemp768);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp769[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp769[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp769[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xbU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xbU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xbU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0xcU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0xcU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp772, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp773, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp774, __Vtemp772, __Vtemp773);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp774[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp774[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp774[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xcU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xcU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xcU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0xdU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0xdU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp777, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp778, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp779, __Vtemp777, __Vtemp778);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp779[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp779[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp779[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xdU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xdU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xdU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0xeU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0xeU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp782, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp783, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp784, __Vtemp782, __Vtemp783);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp784[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp784[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp784[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xeU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xeU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xeU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0xfU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0xfU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp787, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp788, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp789, __Vtemp787, __Vtemp788);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp789[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp789[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp789[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xfU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xfU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0xfU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x10U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x10U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp792, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp793, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp794, __Vtemp792, __Vtemp793);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp794[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp794[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp794[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x10U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x10U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x10U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x11U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x11U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp797, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp798, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp799, __Vtemp797, __Vtemp798);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp799[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp799[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp799[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x11U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x11U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x11U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x12U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x12U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp802, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp803, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp804, __Vtemp802, __Vtemp803);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp804[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp804[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp804[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x12U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x12U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x12U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x13U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x13U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp807, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp808, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp809, __Vtemp807, __Vtemp808);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp809[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp809[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp809[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x13U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x13U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x13U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x14U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x14U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp812, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp813, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp814, __Vtemp812, __Vtemp813);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp814[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp814[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp814[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x14U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x14U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x14U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x15U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x15U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp817, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp818, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp819, __Vtemp817, __Vtemp818);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp819[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp819[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp819[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x15U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x15U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x15U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x16U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x16U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp822, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp823, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp824, __Vtemp822, __Vtemp823);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp824[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp824[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp824[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x16U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x16U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x16U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x17U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x17U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp827, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp828, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp829, __Vtemp827, __Vtemp828);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp829[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp829[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp829[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x17U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x17U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x17U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x18U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x18U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp832, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp833, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp834, __Vtemp832, __Vtemp833);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp834[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp834[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp834[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x18U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x18U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x18U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x19U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x19U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp837, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp838, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp839, __Vtemp837, __Vtemp838);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp839[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp839[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp839[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x19U][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x19U][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x19U][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x1aU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x1aU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp842, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp843, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp844, __Vtemp842, __Vtemp843);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp844[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp844[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp844[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1aU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1aU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1aU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x1bU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x1bU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp847, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp848, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp849, __Vtemp847, __Vtemp848);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp849[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp849[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp849[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1bU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1bU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1bU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x1cU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x1cU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp852, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp853, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp854, __Vtemp852, __Vtemp853);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp854[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp854[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp854[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1cU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1cU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1cU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x1dU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x1dU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp857, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp858, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp859, __Vtemp857, __Vtemp858);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp859[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp859[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp859[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1dU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1dU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1dU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x1eU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x1eU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp862, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp863, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp864, __Vtemp862, __Vtemp863);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp864[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp864[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp864[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1eU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1eU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1eU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc 
        = vlTOPp->dma_research_policy__DOT__hist_inc
        [0x1fU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c 
        = vlTOPp->dma_research_policy__DOT__hist_q[0x1fU];
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__c;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r = 0ULL;
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffeULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | (IData)((IData)((1U & (IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v)))));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffdULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 1U)))) << 1U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffffbULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 2U)))) << 2U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xfffffffffffffff7ULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 3U)))) << 3U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffefULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 4U)))) << 4U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffdfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 5U)))) << 5U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffffbfULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 6U)))) << 6U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r 
        = ((0xffffffffffffff7fULL & vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r) 
           | ((QData)((IData)((1U & ((IData)(vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__v) 
                                     >> 7U)))) << 7U));
    vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__r;
    VL_EXTEND_WQ(65,64, __Vtemp867, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp868, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp869, __Vtemp867, __Vtemp868);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp869[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp869[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp869[2U]);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1fU][0U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[0U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1fU][1U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[1U];
    vlTOPp->dma_research_policy__DOT__hist_sum[0x1fU][2U] 
        = vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__Vfuncout[2U];
    __Vtemp871[0U] = 0xffU;
    __Vtemp871[1U] = 0U;
    __Vtemp871[2U] = 0U;
    __Vtemp872[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp872[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp872[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp871, __Vtemp872)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp873[0U] = 0xffU;
    __Vtemp873[1U] = 0U;
    __Vtemp873[2U] = 0U;
    __Vtemp874[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp874[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp874[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp873, __Vtemp874)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp875[0U] = 0xffU;
    __Vtemp875[1U] = 0U;
    __Vtemp875[2U] = 0U;
    __Vtemp876[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp876[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp876[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp875, __Vtemp876)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp877[0U] = 0xffU;
    __Vtemp877[1U] = 0U;
    __Vtemp877[2U] = 0U;
    __Vtemp878[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp878[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp878[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp877, __Vtemp878)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp879[0U] = 0xffU;
    __Vtemp879[1U] = 0U;
    __Vtemp879[2U] = 0U;
    __Vtemp880[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp880[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp880[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp879, __Vtemp880)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp881[0U] = 0xffU;
    __Vtemp881[1U] = 0U;
    __Vtemp881[2U] = 0U;
    __Vtemp882[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp882[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp882[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp881, __Vtemp882)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp883[0U] = 0xffU;
    __Vtemp883[1U] = 0U;
    __Vtemp883[2U] = 0U;
    __Vtemp884[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp884[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp884[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp883, __Vtemp884)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp885[0U] = 0xffU;
    __Vtemp885[1U] = 0U;
    __Vtemp885[2U] = 0U;
    __Vtemp886[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp886[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp886[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp885, __Vtemp886)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp887[0U] = 0xffU;
    __Vtemp887[1U] = 0U;
    __Vtemp887[2U] = 0U;
    __Vtemp888[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp888[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp888[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp887, __Vtemp888)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp889[0U] = 0xffU;
    __Vtemp889[1U] = 0U;
    __Vtemp889[2U] = 0U;
    __Vtemp890[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp890[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp890[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp889, __Vtemp890)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp891[0U] = 0xffU;
    __Vtemp891[1U] = 0U;
    __Vtemp891[2U] = 0U;
    __Vtemp892[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp892[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp892[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp891, __Vtemp892)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp893[0U] = 0xffU;
    __Vtemp893[1U] = 0U;
    __Vtemp893[2U] = 0U;
    __Vtemp894[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp894[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp894[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp893, __Vtemp894)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp895[0U] = 0xffU;
    __Vtemp895[1U] = 0U;
    __Vtemp895[2U] = 0U;
    __Vtemp896[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp896[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp896[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp895, __Vtemp896)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp897[0U] = 0xffU;
    __Vtemp897[1U] = 0U;
    __Vtemp897[2U] = 0U;
    __Vtemp898[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp898[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp898[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp897, __Vtemp898)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp899[0U] = 0xffU;
    __Vtemp899[1U] = 0U;
    __Vtemp899[2U] = 0U;
    __Vtemp900[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp900[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp900[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp899, __Vtemp900)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp901[0U] = 0xffU;
    __Vtemp901[1U] = 0U;
    __Vtemp901[2U] = 0U;
    __Vtemp902[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp902[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp902[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp901, __Vtemp902)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp903[0U] = 0xffU;
    __Vtemp903[1U] = 0U;
    __Vtemp903[2U] = 0U;
    __Vtemp904[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp904[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp904[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp903, __Vtemp904)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp905[0U] = 0xffU;
    __Vtemp905[1U] = 0U;
    __Vtemp905[2U] = 0U;
    __Vtemp906[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp906[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp906[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp905, __Vtemp906)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp907[0U] = 0xffU;
    __Vtemp907[1U] = 0U;
    __Vtemp907[2U] = 0U;
    __Vtemp908[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp908[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp908[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp907, __Vtemp908)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp909[0U] = 0xffU;
    __Vtemp909[1U] = 0U;
    __Vtemp909[2U] = 0U;
    __Vtemp910[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp910[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp910[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp909, __Vtemp910)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp911[0U] = 0xffU;
    __Vtemp911[1U] = 0U;
    __Vtemp911[2U] = 0U;
    __Vtemp912[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp912[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp912[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp911, __Vtemp912)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp913[0U] = 0xffU;
    __Vtemp913[1U] = 0U;
    __Vtemp913[2U] = 0U;
    __Vtemp914[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp914[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp914[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp913, __Vtemp914)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp915[0U] = 0xffU;
    __Vtemp915[1U] = 0U;
    __Vtemp915[2U] = 0U;
    __Vtemp916[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp916[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp916[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp915, __Vtemp916)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp917[0U] = 0xffU;
    __Vtemp917[1U] = 0U;
    __Vtemp917[2U] = 0U;
    __Vtemp918[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp918[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp918[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp917, __Vtemp918)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp919[0U] = 0xffU;
    __Vtemp919[1U] = 0U;
    __Vtemp919[2U] = 0U;
    __Vtemp920[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp920[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp920[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp919, __Vtemp920)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp921[0U] = 0xffU;
    __Vtemp921[1U] = 0U;
    __Vtemp921[2U] = 0U;
    __Vtemp922[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp922[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp922[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp921, __Vtemp922)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp923[0U] = 0xffU;
    __Vtemp923[1U] = 0U;
    __Vtemp923[2U] = 0U;
    __Vtemp924[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp924[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp924[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp923, __Vtemp924)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp925[0U] = 0xffU;
    __Vtemp925[1U] = 0U;
    __Vtemp925[2U] = 0U;
    __Vtemp926[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp926[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp926[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp925, __Vtemp926)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp927[0U] = 0xffU;
    __Vtemp927[1U] = 0U;
    __Vtemp927[2U] = 0U;
    __Vtemp928[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp928[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp928[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp927, __Vtemp928)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp929[0U] = 0xffU;
    __Vtemp929[1U] = 0U;
    __Vtemp929[2U] = 0U;
    __Vtemp930[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp930[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp930[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp929, __Vtemp930)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp931[0U] = 0xffU;
    __Vtemp931[1U] = 0U;
    __Vtemp931[2U] = 0U;
    __Vtemp932[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp932[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp932[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp931, __Vtemp932)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp933[0U] = 0xffU;
    __Vtemp933[1U] = 0U;
    __Vtemp933[2U] = 0U;
    __Vtemp934[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp934[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp934[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp933, __Vtemp934)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp935[0U] = 0xffU;
    __Vtemp935[1U] = 0U;
    __Vtemp935[2U] = 0U;
    __Vtemp936[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp936[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp936[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp935, __Vtemp936)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp937[0U] = 0xffU;
    __Vtemp937[1U] = 0U;
    __Vtemp937[2U] = 0U;
    __Vtemp938[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp938[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp938[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp937, __Vtemp938)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp939[0U] = 0xffU;
    __Vtemp939[1U] = 0U;
    __Vtemp939[2U] = 0U;
    __Vtemp940[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp940[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp940[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp939, __Vtemp940)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp941[0U] = 0xffU;
    __Vtemp941[1U] = 0U;
    __Vtemp941[2U] = 0U;
    __Vtemp942[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp942[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp942[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp941, __Vtemp942)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp943[0U] = 0xffU;
    __Vtemp943[1U] = 0U;
    __Vtemp943[2U] = 0U;
    __Vtemp944[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp944[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp944[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp943, __Vtemp944)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp945[0U] = 0xffU;
    __Vtemp945[1U] = 0U;
    __Vtemp945[2U] = 0U;
    __Vtemp946[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp946[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp946[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp945, __Vtemp946)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp947[0U] = 0xffU;
    __Vtemp947[1U] = 0U;
    __Vtemp947[2U] = 0U;
    __Vtemp948[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp948[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp948[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp947, __Vtemp948)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp949[0U] = 0xffU;
    __Vtemp949[1U] = 0U;
    __Vtemp949[2U] = 0U;
    __Vtemp950[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp950[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp950[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp949, __Vtemp950)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp951[0U] = 0xffU;
    __Vtemp951[1U] = 0U;
    __Vtemp951[2U] = 0U;
    __Vtemp952[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp952[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp952[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp951, __Vtemp952)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp953[0U] = 0xffU;
    __Vtemp953[1U] = 0U;
    __Vtemp953[2U] = 0U;
    __Vtemp954[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp954[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp954[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp953, __Vtemp954)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp955[0U] = 0xffU;
    __Vtemp955[1U] = 0U;
    __Vtemp955[2U] = 0U;
    __Vtemp956[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp956[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp956[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp955, __Vtemp956)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp957[0U] = 0xffU;
    __Vtemp957[1U] = 0U;
    __Vtemp957[2U] = 0U;
    __Vtemp958[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp958[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp958[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp957, __Vtemp958)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp959[0U] = 0xffU;
    __Vtemp959[1U] = 0U;
    __Vtemp959[2U] = 0U;
    __Vtemp960[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp960[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp960[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp959, __Vtemp960)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp961[0U] = 0xffU;
    __Vtemp961[1U] = 0U;
    __Vtemp961[2U] = 0U;
    __Vtemp962[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp962[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp962[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp961, __Vtemp962)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp963[0U] = 0xffU;
    __Vtemp963[1U] = 0U;
    __Vtemp963[2U] = 0U;
    __Vtemp964[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp964[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp964[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp963, __Vtemp964)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp965[0U] = 0xffU;
    __Vtemp965[1U] = 0U;
    __Vtemp965[2U] = 0U;
    __Vtemp966[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp966[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp966[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp965, __Vtemp966)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp967[0U] = 0xffU;
    __Vtemp967[1U] = 0U;
    __Vtemp967[2U] = 0U;
    __Vtemp968[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp968[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp968[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp967, __Vtemp968)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp969[0U] = 0xffU;
    __Vtemp969[1U] = 0U;
    __Vtemp969[2U] = 0U;
    __Vtemp970[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp970[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp970[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp969, __Vtemp970)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp971[0U] = 0xffU;
    __Vtemp971[1U] = 0U;
    __Vtemp971[2U] = 0U;
    __Vtemp972[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp972[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp972[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp971, __Vtemp972)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp973[0U] = 0xffU;
    __Vtemp973[1U] = 0U;
    __Vtemp973[2U] = 0U;
    __Vtemp974[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp974[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp974[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp973, __Vtemp974)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp975[0U] = 0xffU;
    __Vtemp975[1U] = 0U;
    __Vtemp975[2U] = 0U;
    __Vtemp976[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp976[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp976[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp975, __Vtemp976)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp977[0U] = 0xffU;
    __Vtemp977[1U] = 0U;
    __Vtemp977[2U] = 0U;
    __Vtemp978[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp978[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp978[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp977, __Vtemp978)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp979[0U] = 0xffU;
    __Vtemp979[1U] = 0U;
    __Vtemp979[2U] = 0U;
    __Vtemp980[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp980[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp980[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp979, __Vtemp980)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp981[0U] = 0xffU;
    __Vtemp981[1U] = 0U;
    __Vtemp981[2U] = 0U;
    __Vtemp982[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp982[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp982[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp981, __Vtemp982)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp983[0U] = 0xffU;
    __Vtemp983[1U] = 0U;
    __Vtemp983[2U] = 0U;
    __Vtemp984[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp984[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp984[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp983, __Vtemp984)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp985[0U] = 0xffU;
    __Vtemp985[1U] = 0U;
    __Vtemp985[2U] = 0U;
    __Vtemp986[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp986[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp986[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp985, __Vtemp986)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    vlTOPp->policy_allow_o = ((((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                                & (~ (IData)(vlTOPp->dma_research_policy__DOT__abort_eff))) 
                               & (~ ((0ULL != vlTOPp->dma_research_policy__DOT__err_q) 
                                     | (((((0ULL != vlTOPp->dma_research_policy__DOT__new_err) 
                                           | (IData)(vlTOPp->dma_research_policy__DOT__obs_err_now)) 
                                          | (IData)(vlTOPp->dma_research_policy__DOT__sup_drop_now)) 
                                         | (IData)(vlTOPp->dma_research_policy__DOT__cnt_ovf)) 
                                        | (IData)(vlTOPp->dma_research_policy__DOT__bal_ovf))))) 
                              & (IData)(vlTOPp->dma_research_policy__DOT__allow_core));
}
