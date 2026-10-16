// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_cost_groups.h for the primary calling header

#include "Vdma_research_cost_groups.h"
#include "Vdma_research_cost_groups__Syms.h"

//==========

void Vdma_research_cost_groups::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_research_cost_groups::eval\n"); );
    Vdma_research_cost_groups__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/research-cost-groups-20261002/rtl-small/dma_research_cost_groups.sv", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_research_cost_groups::_eval_initial_loop(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/research-cost-groups-20261002/rtl-small/dma_research_cost_groups.sv", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_research_cost_groups::_sequent__TOP__1(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_sequent__TOP__1\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__count_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__count_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__count_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__count_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__count_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__count_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__count_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v6;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v5;
    // Body
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__count_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v6 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v0 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v1 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v2 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v3 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v4 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v5 = 0U;
    __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v6 = 0U;
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT__total_q 
            = vlTOPp->dma_research_cost_groups__DOT__total_n;
        vlTOPp->dma_research_cost_groups__DOT__seen_q 
            = vlTOPp->dma_research_cost_groups__DOT__seen_n;
        vlTOPp->dma_research_cost_groups__DOT__err_q 
            = vlTOPp->dma_research_cost_groups__DOT__err_n;
    } else {
        vlTOPp->dma_research_cost_groups__DOT__total_q = 0U;
        vlTOPp->dma_research_cost_groups__DOT__seen_q = 0U;
        vlTOPp->dma_research_cost_groups__DOT__err_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_cost_groups__DOT__sum_req_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_cost_groups__DOT__sum_req_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_cost_groups__DOT__sum_req_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_cost_groups__DOT__sum_req_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_cost_groups__DOT__sum_req_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_cost_groups__DOT__sum_req_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_cost_groups__DOT__count_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__count_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_cost_groups__DOT__count_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__count_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_cost_groups__DOT__count_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__count_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_cost_groups__DOT__count_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__count_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_cost_groups__DOT__count_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__count_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_cost_groups__DOT__count_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__count_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__count_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_cost_groups__DOT__sum_main_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_cost_groups__DOT__sum_main_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_cost_groups__DOT__sum_main_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_cost_groups__DOT__sum_main_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_cost_groups__DOT__sum_main_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_cost_groups__DOT__sum_main_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_cost_groups__DOT__sum_comp_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_cost_groups__DOT__sum_comp_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_cost_groups__DOT__sum_comp_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_cost_groups__DOT__sum_comp_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_cost_groups__DOT__sum_comp_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_cost_groups__DOT__sum_comp_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_cost_groups__DOT__sum_dur_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_cost_groups__DOT__sum_dur_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_cost_groups__DOT__sum_dur_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_cost_groups__DOT__sum_dur_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_cost_groups__DOT__sum_dur_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_cost_groups__DOT__sum_dur_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_cost_groups__DOT__sum_miss_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_cost_groups__DOT__sum_miss_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_cost_groups__DOT__sum_miss_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_cost_groups__DOT__sum_miss_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_cost_groups__DOT__sum_miss_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_cost_groups__DOT__sum_miss_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_cost_groups__DOT__min_main_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_cost_groups__DOT__min_main_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_cost_groups__DOT__min_main_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_cost_groups__DOT__min_main_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_cost_groups__DOT__min_main_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_cost_groups__DOT__min_main_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_cost_groups__DOT__max_main_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_cost_groups__DOT__max_main_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_cost_groups__DOT__max_main_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_cost_groups__DOT__max_main_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_cost_groups__DOT__max_main_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_cost_groups__DOT__max_main_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_cost_groups__DOT__min_comp_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_cost_groups__DOT__min_comp_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_cost_groups__DOT__min_comp_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_cost_groups__DOT__min_comp_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_cost_groups__DOT__min_comp_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_cost_groups__DOT__min_comp_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_cost_groups__DOT__max_comp_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_cost_groups__DOT__max_comp_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_cost_groups__DOT__max_comp_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_cost_groups__DOT__max_comp_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_cost_groups__DOT__max_comp_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_cost_groups__DOT__max_comp_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_cost_groups__DOT__min_dur_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_cost_groups__DOT__min_dur_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_cost_groups__DOT__min_dur_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_cost_groups__DOT__min_dur_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_cost_groups__DOT__min_dur_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_cost_groups__DOT__min_dur_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_cost_groups__DOT__max_dur_n
            [0U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v0 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v0 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_cost_groups__DOT__max_dur_n
            [1U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v1 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v1 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_cost_groups__DOT__max_dur_n
            [2U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v2 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v2 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_cost_groups__DOT__max_dur_n
            [3U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v3 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v3 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_cost_groups__DOT__max_dur_n
            [4U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v4 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v4 = 1U;
        vlTOPp->dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_cost_groups__DOT__max_dur_n
            [5U];
        __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v5 
            = vlTOPp->dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v6 = 1U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_req_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_req_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_req_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__count_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__count_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__count_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__count_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__count_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__count_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__count_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__count_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__count_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_main_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_main_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_main_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_comp_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_comp_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_comp_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_dur_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_dur_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_dur_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__sum_miss_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__sum_miss_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__sum_miss_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_main_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_main_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_main_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_main_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_main_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_main_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_comp_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_comp_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_comp_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_comp_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_comp_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_comp_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__min_dur_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__min_dur_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__min_dur_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v0) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[0U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v0;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v1) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[1U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v1;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v2) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[2U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v2;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v3) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[3U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v3;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v4) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[4U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v4;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v5) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[5U] 
            = __Vdlyvval__dma_research_cost_groups__DOT__max_dur_q__v5;
    }
    if (__Vdlyvset__dma_research_cost_groups__DOT__max_dur_q__v6) {
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[0U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[1U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[2U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[3U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[4U] = 0U;
        vlTOPp->dma_research_cost_groups__DOT__max_dur_q[5U] = 0U;
    }
    vlTOPp->total_o = vlTOPp->dma_research_cost_groups__DOT__total_q;
    vlTOPp->err_o = vlTOPp->dma_research_cost_groups__DOT__err_q;
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
}

VL_INLINE_OPT void Vdma_research_cost_groups::_combo__TOP__3(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_combo__TOP__3\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp27[3];
    WData/*95:0*/ __Vtemp28[3];
    WData/*95:0*/ __Vtemp29[3];
    WData/*95:0*/ __Vtemp32[3];
    WData/*95:0*/ __Vtemp33[3];
    WData/*95:0*/ __Vtemp34[3];
    WData/*95:0*/ __Vtemp37[3];
    WData/*95:0*/ __Vtemp38[3];
    WData/*95:0*/ __Vtemp39[3];
    WData/*95:0*/ __Vtemp42[3];
    WData/*95:0*/ __Vtemp43[3];
    WData/*95:0*/ __Vtemp44[3];
    WData/*95:0*/ __Vtemp47[3];
    WData/*95:0*/ __Vtemp48[3];
    WData/*95:0*/ __Vtemp49[3];
    // Body
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
            VL_EXTEND_WI(65,8, __Vtemp27, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_main_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp28, vlTOPp->main_i);
            VL_ADD_W(3, __Vtemp29, __Vtemp27, __Vtemp28);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp29[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp29[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp29[2U]);
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
            VL_EXTEND_WI(65,8, __Vtemp32, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_comp_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp33, vlTOPp->comp_i);
            VL_ADD_W(3, __Vtemp34, __Vtemp32, __Vtemp33);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp34[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp34[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp34[2U]);
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
            VL_EXTEND_WI(65,8, __Vtemp37, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_dur_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp38, vlTOPp->dur_i);
            VL_ADD_W(3, __Vtemp39, __Vtemp37, __Vtemp38);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp39[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp39[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp39[2U]);
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
            VL_EXTEND_WI(65,8, __Vtemp42, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_req_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp43, vlTOPp->req_i);
            VL_ADD_W(3, __Vtemp44, __Vtemp42, __Vtemp43);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp44[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp44[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp44[2U]);
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
            VL_EXTEND_WI(65,8, __Vtemp47, ((5U >= (IData)(vlTOPp->dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_cost_groups__DOT__sum_miss_q
                                           [vlTOPp->dma_research_cost_groups__DOT__grp]
                                            : 0U));
            VL_EXTEND_WI(65,32, __Vtemp48, vlTOPp->miss_i);
            VL_ADD_W(3, __Vtemp49, __Vtemp47, __Vtemp48);
            vlTOPp->dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp49[0U];
            vlTOPp->dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp49[1U];
            vlTOPp->dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp49[2U]);
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

void Vdma_research_cost_groups::_eval(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_eval\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vdma_research_cost_groups::_change_request(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_change_request\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_research_cost_groups::_change_request_1(Vdma_research_cost_groups__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_change_request_1\n"); );
    Vdma_research_cost_groups* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_research_cost_groups::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_cost_groups::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((clear_i & 0xfeU))) {
        Verilated::overWidthError("clear_i");}
    if (VL_UNLIKELY((commit_i & 0xfeU))) {
        Verilated::overWidthError("commit_i");}
}
#endif  // VL_DEBUG
