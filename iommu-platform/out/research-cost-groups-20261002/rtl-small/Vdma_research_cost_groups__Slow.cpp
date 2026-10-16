// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_cost_groups.h for the primary calling header

#include "Vdma_research_cost_groups.h"
#include "Vdma_research_cost_groups__Syms.h"

//==========

VL_CTOR_IMP(Vdma_research_cost_groups) {
    Vdma_research_cost_groups__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_research_cost_groups__Syms(this, name());
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_research_cost_groups::__Vconfigure(Vdma_research_cost_groups__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_research_cost_groups::~Vdma_research_cost_groups() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_research_cost_groups::_settle__TOP__2(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_settle__TOP__2\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp2[3];
    WData/*95:0*/ __Vtemp3[3];
    WData/*95:0*/ __Vtemp4[3];
    WData/*95:0*/ __Vtemp7[3];
    WData/*95:0*/ __Vtemp8[3];
    WData/*95:0*/ __Vtemp9[3];
    WData/*95:0*/ __Vtemp12[3];
    WData/*95:0*/ __Vtemp13[3];
    WData/*95:0*/ __Vtemp14[3];
    WData/*95:0*/ __Vtemp17[3];
    WData/*95:0*/ __Vtemp18[3];
    WData/*95:0*/ __Vtemp19[3];
    WData/*95:0*/ __Vtemp22[3];
    WData/*95:0*/ __Vtemp23[3];
    WData/*95:0*/ __Vtemp24[3];
    // Body
    vlTOPp->err_o = vlTOPp->dma_research_cost_groups__DOT__err_q;
    vlTOPp->total_o = vlTOPp->dma_research_cost_groups__DOT__total_q;
    vlTOPp->dma_research_cost_groups__DOT__total_n 
        = vlTOPp->dma_research_cost_groups__DOT__total_q;
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_cost_groups__DOT__total_n = 0U;
    } else {
        if (vlTOPp->commit_i) {
            vlTOPp->dma_research_cost_groups__DOT__total_n 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_cost_groups__DOT__total_q)));
        }
    }
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [0U];
    vlTOPp->max_dur_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [1U];
    vlTOPp->max_dur_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [2U];
    vlTOPp->max_dur_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [3U];
    vlTOPp->max_dur_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [4U];
    vlTOPp->max_dur_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [5U];
    vlTOPp->max_dur_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [0U];
    vlTOPp->max_main_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [1U];
    vlTOPp->max_main_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [2U];
    vlTOPp->max_main_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [3U];
    vlTOPp->max_main_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [4U];
    vlTOPp->max_main_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [5U];
    vlTOPp->max_main_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [0U];
    vlTOPp->min_dur_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [1U];
    vlTOPp->min_dur_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [2U];
    vlTOPp->min_dur_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [3U];
    vlTOPp->min_dur_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [4U];
    vlTOPp->min_dur_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [5U];
    vlTOPp->min_dur_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [0U];
    vlTOPp->max_comp_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [1U];
    vlTOPp->max_comp_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [2U];
    vlTOPp->max_comp_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [3U];
    vlTOPp->max_comp_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [4U];
    vlTOPp->max_comp_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [5U];
    vlTOPp->max_comp_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [0U];
    vlTOPp->min_comp_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [1U];
    vlTOPp->min_comp_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [2U];
    vlTOPp->min_comp_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [3U];
    vlTOPp->min_comp_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [4U];
    vlTOPp->min_comp_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [5U];
    vlTOPp->min_comp_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [0U];
    vlTOPp->min_main_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [1U];
    vlTOPp->min_main_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [2U];
    vlTOPp->min_main_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [3U];
    vlTOPp->min_main_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [4U];
    vlTOPp->min_main_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [5U];
    vlTOPp->min_main_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [0U];
    vlTOPp->sum_miss_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [1U];
    vlTOPp->sum_miss_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [2U];
    vlTOPp->sum_miss_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [3U];
    vlTOPp->sum_miss_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [4U];
    vlTOPp->sum_miss_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [5U];
    vlTOPp->sum_miss_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [0U];
    vlTOPp->sum_req_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [1U];
    vlTOPp->sum_req_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [2U];
    vlTOPp->sum_req_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [3U];
    vlTOPp->sum_req_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [4U];
    vlTOPp->sum_req_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [5U];
    vlTOPp->sum_req_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [0U];
    vlTOPp->sum_dur_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [1U];
    vlTOPp->sum_dur_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [2U];
    vlTOPp->sum_dur_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [3U];
    vlTOPp->sum_dur_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [4U];
    vlTOPp->sum_dur_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [5U];
    vlTOPp->sum_dur_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [0U];
    vlTOPp->sum_comp_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [1U];
    vlTOPp->sum_comp_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [2U];
    vlTOPp->sum_comp_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [3U];
    vlTOPp->sum_comp_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [4U];
    vlTOPp->sum_comp_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [5U];
    vlTOPp->sum_comp_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [0U];
    vlTOPp->sum_main_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [1U];
    vlTOPp->sum_main_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [2U];
    vlTOPp->sum_main_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [3U];
    vlTOPp->sum_main_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [4U];
    vlTOPp->sum_main_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [5U];
    vlTOPp->sum_main_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [0U];
    vlTOPp->count_o[0U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [1U];
    vlTOPp->count_o[1U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [2U];
    vlTOPp->count_o[2U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [3U];
    vlTOPp->count_o[3U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [4U];
    vlTOPp->count_o[4U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [5U];
    vlTOPp->count_o[5U] = vlTOPp->dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__count_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__sum_main_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__sum_req_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__min_main_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__max_main_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [0U];
    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[0U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__count_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__sum_main_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__sum_req_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__min_main_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__max_main_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [1U];
    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[1U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__count_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__sum_main_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__sum_req_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__min_main_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__max_main_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [2U];
    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[2U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__count_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__sum_main_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__sum_req_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__min_main_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__max_main_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [3U];
    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[3U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__count_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__sum_main_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__sum_req_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__min_main_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__max_main_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [4U];
    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[4U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_cost_groups__DOT__count_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__count_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_cost_groups__DOT__sum_main_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__sum_main_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_cost_groups__DOT__sum_req_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__sum_req_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_cost_groups__DOT__min_main_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__min_main_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_cost_groups__DOT__max_main_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__max_main_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_cost_groups__DOT__min_comp_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_cost_groups__DOT__max_comp_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_cost_groups__DOT__min_dur_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_cost_groups__DOT__max_dur_q
        [5U];
    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[5U] 
        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_cost_groups__DOT__seen_n = vlTOPp->dma_research_cost_groups__DOT__seen_q;
    vlTOPp->dma_research_cost_groups__DOT__err_n = vlTOPp->dma_research_cost_groups__DOT__err_q;
    vlTOPp->dma_research_cost_groups__DOT__grp = 5U;
    vlTOPp->dma_research_cost_groups__DOT__ext[0U] = 0U;
    vlTOPp->dma_research_cost_groups__DOT__ext[1U] = 0U;
    vlTOPp->dma_research_cost_groups__DOT__ext[2U] = 0U;
    vlTOPp->dma_research_cost_groups__DOT__main_plus_comp = 0ULL;
    if (((4U == vlTOPp->req_i) & (4U >= vlTOPp->miss_i))) {
        vlTOPp->dma_research_cost_groups__DOT__grp 
            = (7U & vlTOPp->miss_i);
    }
    if (vlTOPp->clear_i) {
        vlTOPp->dma_research_cost_groups__DOT__count_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__seen_n = 0U;
        vlTOPp->dma_research_cost_groups__DOT__err_n = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[5U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[5U] = 0U;
    } else {
        if (vlTOPp->commit_i) {
            if ((vlTOPp->miss_i > vlTOPp->req_i)) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (4U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT__main_plus_comp 
                = (0x1ffffffffULL & ((QData)((IData)(vlTOPp->main_i)) 
                                     + (QData)((IData)(vlTOPp->comp_i))));
            if ((vlTOPp->dma_research_cost_groups__DOT__main_plus_comp 
                 > (QData)((IData)(vlTOPp->dur_i)))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (8U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            if ((0xffU == (IData)(vlTOPp->dma_research_cost_groups__DOT__total_q))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (1U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            if ((0xffU == ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                            ? vlTOPp->dma_research_cost_groups__DOT__count_q
                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                            : 0U))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (1U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound25 
                = (0xffU & ((IData)(1U) + ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__count_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U)));
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__count_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_cost_groups__DOT____Vlvbound25;
            }
            VL_EXTEND_WI(65,8, __Vtemp2, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                           ? vlTOPp->dma_research_cost_groups__DOT__sum_main_q
                                          [vlTOPp->dma_research_cost_groups__DOT__grp]
                                           : 0U));
            VL_EXTEND_WI(65,32, __Vtemp3, vlTOPp->main_i);
            VL_ADD_W(3, __Vtemp4, __Vtemp2, __Vtemp3);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp4[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp4[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp4[2U]);
            if ((0U != (0x1ffffffffffffffULL & (((QData)((IData)(
                                                                 vlTOPp->dma_research_cost_groups__DOT__ext[2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->dma_research_cost_groups__DOT__ext[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->dma_research_cost_groups__DOT__ext[0U])) 
                                                      >> 8U)))))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound26 
                = (0xffU & vlTOPp->dma_research_cost_groups__DOT__ext[0U]);
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__sum_main_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_cost_groups__DOT____Vlvbound26;
            }
            VL_EXTEND_WI(65,8, __Vtemp7, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                           ? vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
                                          [vlTOPp->dma_research_cost_groups__DOT__grp]
                                           : 0U));
            VL_EXTEND_WI(65,32, __Vtemp8, vlTOPp->comp_i);
            VL_ADD_W(3, __Vtemp9, __Vtemp7, __Vtemp8);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp9[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp9[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp9[2U]);
            if ((0U != (0x1ffffffffffffffULL & (((QData)((IData)(
                                                                 vlTOPp->dma_research_cost_groups__DOT__ext[2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->dma_research_cost_groups__DOT__ext[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->dma_research_cost_groups__DOT__ext[0U])) 
                                                      >> 8U)))))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound27 
                = (0xffU & vlTOPp->dma_research_cost_groups__DOT__ext[0U]);
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__sum_comp_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_cost_groups__DOT____Vlvbound27;
            }
            VL_EXTEND_WI(65,8, __Vtemp12, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp13, vlTOPp->dur_i);
            VL_ADD_W(3, __Vtemp14, __Vtemp12, __Vtemp13);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp14[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp14[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp14[2U]);
            if ((0U != (0x1ffffffffffffffULL & (((QData)((IData)(
                                                                 vlTOPp->dma_research_cost_groups__DOT__ext[2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->dma_research_cost_groups__DOT__ext[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->dma_research_cost_groups__DOT__ext[0U])) 
                                                      >> 8U)))))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound28 
                = (0xffU & vlTOPp->dma_research_cost_groups__DOT__ext[0U]);
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__sum_dur_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_cost_groups__DOT____Vlvbound28;
            }
            VL_EXTEND_WI(65,8, __Vtemp17, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_req_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp18, vlTOPp->req_i);
            VL_ADD_W(3, __Vtemp19, __Vtemp17, __Vtemp18);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp19[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp19[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp19[2U]);
            if ((0U != (0x1ffffffffffffffULL & (((QData)((IData)(
                                                                 vlTOPp->dma_research_cost_groups__DOT__ext[2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->dma_research_cost_groups__DOT__ext[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->dma_research_cost_groups__DOT__ext[0U])) 
                                                      >> 8U)))))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound29 
                = (0xffU & vlTOPp->dma_research_cost_groups__DOT__ext[0U]);
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__sum_req_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_cost_groups__DOT____Vlvbound29;
            }
            VL_EXTEND_WI(65,8, __Vtemp22, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp23, vlTOPp->miss_i);
            VL_ADD_W(3, __Vtemp24, __Vtemp22, __Vtemp23);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp24[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp24[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp24[2U]);
            if ((0U != (0x1ffffffffffffffULL & (((QData)((IData)(
                                                                 vlTOPp->dma_research_cost_groups__DOT__ext[2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->dma_research_cost_groups__DOT__ext[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->dma_research_cost_groups__DOT__ext[0U])) 
                                                      >> 8U)))))) {
                vlTOPp->dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound30 
                = (0xffU & vlTOPp->dma_research_cost_groups__DOT__ext[0U]);
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__sum_miss_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_cost_groups__DOT____Vlvbound30;
            }
            if (((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp)) 
                 & ((IData)(vlTOPp->dma_research_cost_groups__DOT__seen_q) 
                    >> (IData)(vlTOPp->dma_research_cost_groups__DOT__grp)))) {
                if ((vlTOPp->main_i < ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                        ? vlTOPp->dma_research_cost_groups__DOT__min_main_q
                                       [vlTOPp->dma_research_cost_groups__DOT__grp]
                                        : 0U))) {
                    vlTOPp->dma_research_cost_groups__DOT____Vlvbound31 
                        = vlTOPp->main_i;
                    if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_cost_groups__DOT__min_main_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound31;
                    }
                }
                if ((vlTOPp->main_i > ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                        ? vlTOPp->dma_research_cost_groups__DOT__max_main_q
                                       [vlTOPp->dma_research_cost_groups__DOT__grp]
                                        : 0U))) {
                    vlTOPp->dma_research_cost_groups__DOT____Vlvbound32 
                        = vlTOPp->main_i;
                    if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_cost_groups__DOT__max_main_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound32;
                    }
                }
                if ((vlTOPp->comp_i < ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                        ? vlTOPp->dma_research_cost_groups__DOT__min_comp_q
                                       [vlTOPp->dma_research_cost_groups__DOT__grp]
                                        : 0U))) {
                    vlTOPp->dma_research_cost_groups__DOT____Vlvbound33 
                        = vlTOPp->comp_i;
                    if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_cost_groups__DOT__min_comp_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound33;
                    }
                }
                if ((vlTOPp->comp_i > ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                        ? vlTOPp->dma_research_cost_groups__DOT__max_comp_q
                                       [vlTOPp->dma_research_cost_groups__DOT__grp]
                                        : 0U))) {
                    vlTOPp->dma_research_cost_groups__DOT____Vlvbound34 
                        = vlTOPp->comp_i;
                    if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_cost_groups__DOT__max_comp_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound34;
                    }
                }
                if ((vlTOPp->dur_i < ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                       ? vlTOPp->dma_research_cost_groups__DOT__min_dur_q
                                      [vlTOPp->dma_research_cost_groups__DOT__grp]
                                       : 0U))) {
                    vlTOPp->dma_research_cost_groups__DOT____Vlvbound35 
                        = vlTOPp->dur_i;
                    if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_cost_groups__DOT__min_dur_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound35;
                    }
                }
                if ((vlTOPp->dur_i > ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                       ? vlTOPp->dma_research_cost_groups__DOT__max_dur_q
                                      [vlTOPp->dma_research_cost_groups__DOT__grp]
                                       : 0U))) {
                    vlTOPp->dma_research_cost_groups__DOT____Vlvbound36 
                        = vlTOPp->dur_i;
                    if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_cost_groups__DOT__max_dur_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound36;
                    }
                }
            } else {
                vlTOPp->dma_research_cost_groups__DOT____Vlvbound37 
                    = vlTOPp->main_i;
                if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_cost_groups__DOT__min_main_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound37;
                }
                vlTOPp->dma_research_cost_groups__DOT____Vlvbound38 
                    = vlTOPp->main_i;
                if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_cost_groups__DOT__max_main_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound38;
                }
                vlTOPp->dma_research_cost_groups__DOT____Vlvbound39 
                    = vlTOPp->comp_i;
                if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_cost_groups__DOT__min_comp_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound39;
                }
                vlTOPp->dma_research_cost_groups__DOT____Vlvbound40 
                    = vlTOPp->comp_i;
                if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_cost_groups__DOT__max_comp_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound40;
                }
                vlTOPp->dma_research_cost_groups__DOT____Vlvbound41 
                    = vlTOPp->dur_i;
                if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_cost_groups__DOT__min_dur_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound41;
                }
                vlTOPp->dma_research_cost_groups__DOT____Vlvbound42 
                    = vlTOPp->dur_i;
                if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_cost_groups__DOT__max_dur_n[vlTOPp->dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_cost_groups__DOT____Vlvbound42;
                }
            }
            vlTOPp->dma_research_cost_groups__DOT____Vlvbound43 = 1U;
            if ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_cost_groups__DOT__seen_n 
                    = (((~ ((IData)(1U) << (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))) 
                        & (IData)(vlTOPp->dma_research_cost_groups__DOT__seen_n)) 
                       | ((IData)(vlTOPp->dma_research_cost_groups__DOT____Vlvbound43) 
                          << (IData)(vlTOPp->dma_research_cost_groups__DOT__grp)));
            }
        }
    }
}

void Vdma_research_cost_groups::_eval_initial(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_eval_initial\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vdma_research_cost_groups::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::final\n"); );
    // Variables
    Vdma_research_cost_groups__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_research_cost_groups::_eval_settle(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_eval_settle\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vdma_research_cost_groups::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    clear_i = VL_RAND_RESET_I(1);
    commit_i = VL_RAND_RESET_I(1);
    main_i = VL_RAND_RESET_I(32);
    comp_i = VL_RAND_RESET_I(32);
    dur_i = VL_RAND_RESET_I(32);
    req_i = VL_RAND_RESET_I(32);
    miss_i = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            count_o[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            sum_main_o[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            sum_comp_o[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            sum_dur_o[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            sum_req_o[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            sum_miss_o[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            min_main_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            max_main_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            min_comp_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            max_comp_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            min_dur_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            max_dur_o[__Vi0] = VL_RAND_RESET_I(32);
    }}
    total_o = VL_RAND_RESET_I(8);
    err_o = VL_RAND_RESET_I(16);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__count_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_main_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_comp_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_dur_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_req_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_miss_q[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__min_main_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__max_main_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__min_comp_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__max_comp_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__min_dur_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__max_dur_q[__Vi0] = VL_RAND_RESET_I(32);
    }}
    dma_research_cost_groups__DOT__seen_q = VL_RAND_RESET_I(6);
    dma_research_cost_groups__DOT__total_q = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT__err_q = VL_RAND_RESET_I(16);
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__count_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_main_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_comp_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_dur_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_req_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__sum_miss_n[__Vi0] = VL_RAND_RESET_I(8);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__min_main_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__max_main_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__min_comp_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__max_comp_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__min_dur_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<6; ++__Vi0) {
            dma_research_cost_groups__DOT__max_dur_n[__Vi0] = VL_RAND_RESET_I(32);
    }}
    dma_research_cost_groups__DOT__seen_n = VL_RAND_RESET_I(6);
    dma_research_cost_groups__DOT__total_n = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT__err_n = VL_RAND_RESET_I(16);
    dma_research_cost_groups__DOT__grp = VL_RAND_RESET_I(3);
    VL_RAND_RESET_W(65, dma_research_cost_groups__DOT__ext);
    dma_research_cost_groups__DOT__main_plus_comp = VL_RAND_RESET_Q(33);
    dma_research_cost_groups__DOT____Vlvbound1 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound2 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound3 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound4 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound5 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound6 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound7 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound8 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound9 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound10 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound11 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound12 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound25 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound26 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound27 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound28 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound29 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound30 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound31 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound32 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound33 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound34 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound35 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound36 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound37 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound38 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound39 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound40 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound41 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound42 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound43 = VL_RAND_RESET_I(1);
    dma_research_cost_groups__DOT____Vlvbound44 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound45 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound46 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound47 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound48 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound49 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound50 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound51 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound52 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound53 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound54 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound55 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound68 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound69 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound70 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound71 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound72 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound73 = VL_RAND_RESET_I(8);
    dma_research_cost_groups__DOT____Vlvbound74 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound75 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound76 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound77 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound78 = VL_RAND_RESET_I(32);
    dma_research_cost_groups__DOT____Vlvbound79 = VL_RAND_RESET_I(32);
}
