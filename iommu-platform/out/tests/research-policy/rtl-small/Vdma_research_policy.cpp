// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_policy.h for the primary calling header

#include "Vdma_research_policy.h"
#include "Vdma_research_policy__Syms.h"

//==========

void Vdma_research_policy::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_research_policy::eval\n"); );
    Vdma_research_policy__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_policy.sv", 87, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_research_policy::_eval_initial_loop(Vdma_research_policy__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_policy.sv", 87, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_research_policy::_sequent__TOP__1(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_sequent__TOP__1\n"); );
    Vdma_research_policy* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v5;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v7;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v7;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v8;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v9;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v9;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v10;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v10;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v11;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v11;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v12;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v12;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v13;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v13;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v14;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v14;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v15;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v15;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v16;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v17;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v17;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v18;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v18;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v19;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v19;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v20;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v20;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v21;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v21;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v22;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v22;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v23;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v23;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v24;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v24;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__stat_q__v25;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v25;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__stat_q__v26;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__hist_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v5;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v7;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v8;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v9;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v10;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v11;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v12;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v13;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v14;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v15;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v16;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v17;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v18;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v19;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v20;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v21;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v22;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v23;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v24;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v25;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v26;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v27;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v28;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v29;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v30;
    CData/*7:0*/ __Vdlyvval__dma_research_policy__DOT__hist_q__v31;
    CData/*0:0*/ __Vdlyvset__dma_research_policy__DOT__hist_q__v32;
    WData/*95:0*/ __Vtemp62[3];
    WData/*95:0*/ __Vtemp63[3];
    WData/*95:0*/ __Vtemp64[3];
    WData/*95:0*/ __Vtemp67[3];
    WData/*95:0*/ __Vtemp68[3];
    WData/*95:0*/ __Vtemp69[3];
    QData/*63:0*/ __Vdly__dma_research_policy__DOT__win_main_q;
    // Body
    __Vdlyvset__dma_research_policy__DOT__hist_q__v0 = 0U;
    __Vdlyvset__dma_research_policy__DOT__hist_q__v32 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v0 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v1 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v2 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v3 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v4 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v5 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v6 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v7 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v8 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v9 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v10 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v11 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v12 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v13 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v14 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v15 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v16 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v17 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v18 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v19 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v20 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v21 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v22 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v23 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v24 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v25 = 0U;
    __Vdlyvset__dma_research_policy__DOT__stat_q__v26 = 0U;
    __Vdly__dma_research_policy__DOT__win_main_q = vlTOPp->dma_research_policy__DOT__win_main_q;
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_policy__DOT__nodone_run_q 
            = vlTOPp->dma_research_policy__DOT__nodone_run_next;
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__nodone_run_q = 0ULL;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__nodone_run_q = 0ULL;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__nodone_run_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_policy__DOT__nodone_max_q 
            = vlTOPp->dma_research_policy__DOT__nodone_max_next;
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__nodone_max_q = 0ULL;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__nodone_max_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__aborted_q = 0U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_abort) {
            vlTOPp->dma_research_policy__DOT__aborted_q = 1U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__aborted_q = 0U;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__aborted_q = 0U;
    }
    vlTOPp->dma_research_policy__DOT__err_q = ((IData)(vlTOPp->rst_ni)
                                                ? ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                                                    ? 0ULL
                                                    : 
                                                   ((vlTOPp->dma_research_policy__DOT__err_q 
                                                     | vlTOPp->dma_research_policy__DOT__new_err) 
                                                    | (((((IData)(vlTOPp->dma_research_policy__DOT__obs_err_now)
                                                           ? 0x200ULL
                                                           : 0ULL) 
                                                         | ((IData)(vlTOPp->dma_research_policy__DOT__sup_drop_now)
                                                             ? 0x800ULL
                                                             : 0ULL)) 
                                                        | ((IData)(vlTOPp->dma_research_policy__DOT__cnt_ovf)
                                                            ? 0x10ULL
                                                            : 0ULL)) 
                                                       | ((IData)(vlTOPp->dma_research_policy__DOT__bal_ovf)
                                                           ? 0x20ULL
                                                           : 0ULL))))
                                                : 0ULL);
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_policy__DOT__hist_q__v0 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                        [0U][0U]));
        __Vdlyvset__dma_research_policy__DOT__hist_q__v0 = 1U;
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            __Vdlyvval__dma_research_policy__DOT__hist_q__v1 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v2 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v3 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v4 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v5 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v6 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v7 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v8 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v9 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v10 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v11 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v12 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v13 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v14 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v15 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v16 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v17 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v18 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v19 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v20 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v21 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v22 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v23 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v24 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v25 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v26 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v27 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v28 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v29 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v30 = 0U;
            __Vdlyvval__dma_research_policy__DOT__hist_q__v31 = 0U;
        } else {
            __Vdlyvval__dma_research_policy__DOT__hist_q__v1 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [1U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v2 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [2U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v3 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [3U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v4 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [4U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v5 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [5U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v6 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [6U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v7 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [7U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v8 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [8U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v9 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [9U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v10 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0xaU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v11 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0xbU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v12 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0xcU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v13 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0xdU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v14 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0xeU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v15 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0xfU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v16 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x10U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v17 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x11U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v18 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x12U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v19 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x13U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v20 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x14U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v21 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x15U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v22 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x16U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v23 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x17U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v24 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x18U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v25 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x19U][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v26 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x1aU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v27 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x1bU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v28 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x1cU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v29 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x1dU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v30 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x1eU][0U]);
            __Vdlyvval__dma_research_policy__DOT__hist_q__v31 
                = (0xffU & vlTOPp->dma_research_policy__DOT__hist_sum
                   [0x1fU][0U]);
        }
    } else {
        __Vdlyvset__dma_research_policy__DOT__hist_q__v32 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v0 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v0 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [1U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v1 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v1 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [2U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v2 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v2 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [3U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v3 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v3 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [4U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v4 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v4 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [5U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v5 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v5 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [6U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v6 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v6 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [7U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v7 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v7 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [8U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v8 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v8 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [9U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v9 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v9 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0xaU][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v10 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v10 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0xbU][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v11 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v11 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0xcU][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v12 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v12 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0xdU][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v13 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v13 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0xeU][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v14 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v14 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0xfU][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v15 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v15 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x10U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v16 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v16 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x11U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v17 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v17 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x12U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v18 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v18 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x13U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v19 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v19 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x14U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v20 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v20 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x15U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v21 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v21 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x16U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v22 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v22 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x17U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v23 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v23 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x18U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v24 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v24 = 1U;
        vlTOPp->dma_research_policy__DOT____Vlvbound3 
            = ((IData)(vlTOPp->dma_research_policy__DOT__do_clear)
                ? 0U : (0xffU & vlTOPp->dma_research_policy__DOT__stat_sum
                        [0x19U][0U]));
        __Vdlyvval__dma_research_policy__DOT__stat_q__v25 
            = vlTOPp->dma_research_policy__DOT____Vlvbound3;
        __Vdlyvset__dma_research_policy__DOT__stat_q__v25 = 1U;
    } else {
        __Vdlyvset__dma_research_policy__DOT__stat_q__v26 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__win_run) {
            vlTOPp->dma_research_policy__DOT__m6_cd_q 
                = vlTOPp->dma_research_policy__DOT__m6_cd_next;
        }
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__m6_cd_q = 0ULL;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__m6_cd_q = 0ULL;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__m6_cd_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__win_run) {
            vlTOPp->dma_research_policy__DOT__win_allact_q 
                = ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
                   | (IData)(vlTOPp->dma_research_policy__DOT__act_all_now));
        }
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__win_allact_q = 1U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__win_allact_q = 0U;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__win_allact_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__win_run) {
            vlTOPp->dma_research_policy__DOT__win_vdone_q 
                = ((IData)(vlTOPp->dma_research_policy__DOT__win_last)
                    ? 0ULL : vlTOPp->dma_research_policy__DOT__vdone_now);
        }
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__win_vdone_q = 0ULL;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__win_vdone_q = 0ULL;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__win_vdone_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__win_run) {
            if (vlTOPp->dma_research_policy__DOT__win_last) {
                vlTOPp->dma_research_policy__DOT__prev_main_q 
                    = (vlTOPp->dma_research_policy__DOT__win_main_q 
                       + vlTOPp->dma_research_policy__DOT__main_inc);
                __Vdly__dma_research_policy__DOT__win_main_q = 0ULL;
            } else {
                __Vdly__dma_research_policy__DOT__win_main_q 
                    = (vlTOPp->dma_research_policy__DOT__win_main_q 
                       + vlTOPp->dma_research_policy__DOT__main_inc);
            }
        }
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__prev_main_q = 0ULL;
            __Vdly__dma_research_policy__DOT__win_main_q = 0ULL;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__prev_main_q = 0ULL;
            __Vdly__dma_research_policy__DOT__win_main_q = 0ULL;
        }
    } else {
        __Vdly__dma_research_policy__DOT__win_main_q = 0ULL;
        vlTOPp->dma_research_policy__DOT__prev_main_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__frozen_q = 0U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_freeze) {
            vlTOPp->dma_research_policy__DOT__frozen_q = 1U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__frozen_q = 0U;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__frozen_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__win_run) {
            vlTOPp->dma_research_policy__DOT__win_cnt_q 
                = ((IData)(vlTOPp->dma_research_policy__DOT__win_last)
                    ? 0ULL : (1ULL + vlTOPp->dma_research_policy__DOT__win_cnt_q));
        }
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__win_cnt_q = 0ULL;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__win_cnt_q = 0ULL;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__win_cnt_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__win_run) {
            vlTOPp->dma_research_policy__DOT__balance_q 
                = vlTOPp->dma_research_policy__DOT__bal_next;
            if ((((IData)(vlTOPp->dma_research_policy__DOT__win_complete) 
                  & (0U != vlTOPp->dma_research_policy__DOT__mode_q)) 
                 & ((IData)(vlTOPp->dma_research_policy__DOT__fb_inc) 
                    | (IData)(vlTOPp->dma_research_policy__DOT__fb_dec)))) {
                if ((vlTOPp->dma_research_policy__DOT__b_next 
                     < vlTOPp->dma_research_policy__DOT__bmin_q)) {
                    vlTOPp->dma_research_policy__DOT__bmin_q 
                        = vlTOPp->dma_research_policy__DOT__b_next;
                }
                if ((vlTOPp->dma_research_policy__DOT__b_next 
                     > vlTOPp->dma_research_policy__DOT__bmax_q)) {
                    vlTOPp->dma_research_policy__DOT__bmax_q 
                        = vlTOPp->dma_research_policy__DOT__b_next;
                }
                vlTOPp->dma_research_policy__DOT__b_q 
                    = vlTOPp->dma_research_policy__DOT__b_next;
            }
        }
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__bmin_q 
                = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
            vlTOPp->dma_research_policy__DOT__bmax_q 
                = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
            vlTOPp->dma_research_policy__DOT__balance_q 
                = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
            vlTOPp->dma_research_policy__DOT__b_q = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__bmin_q 
                = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
            vlTOPp->dma_research_policy__DOT__bmax_q 
                = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
            vlTOPp->dma_research_policy__DOT__balance_q 
                = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
            vlTOPp->dma_research_policy__DOT__b_q = (QData)((IData)(vlTOPp->dma_research_policy__DOT__initbud_q));
        }
    } else {
        vlTOPp->dma_research_policy__DOT__bmin_q = 0x1f40ULL;
        vlTOPp->dma_research_policy__DOT__bmax_q = 0x1f40ULL;
        vlTOPp->dma_research_policy__DOT__balance_q = 0x1f40ULL;
        vlTOPp->dma_research_policy__DOT__b_q = 0x1f40ULL;
    }
    if (__Vdlyvset__dma_research_policy__DOT__hist_q__v0) {
        vlTOPp->dma_research_policy__DOT__hist_q[0U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v0;
        vlTOPp->dma_research_policy__DOT__hist_q[1U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v1;
        vlTOPp->dma_research_policy__DOT__hist_q[2U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v2;
        vlTOPp->dma_research_policy__DOT__hist_q[3U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v3;
        vlTOPp->dma_research_policy__DOT__hist_q[4U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v4;
        vlTOPp->dma_research_policy__DOT__hist_q[5U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v5;
        vlTOPp->dma_research_policy__DOT__hist_q[6U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v6;
        vlTOPp->dma_research_policy__DOT__hist_q[7U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v7;
        vlTOPp->dma_research_policy__DOT__hist_q[8U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v8;
        vlTOPp->dma_research_policy__DOT__hist_q[9U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v9;
        vlTOPp->dma_research_policy__DOT__hist_q[0xaU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v10;
        vlTOPp->dma_research_policy__DOT__hist_q[0xbU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v11;
        vlTOPp->dma_research_policy__DOT__hist_q[0xcU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v12;
        vlTOPp->dma_research_policy__DOT__hist_q[0xdU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v13;
        vlTOPp->dma_research_policy__DOT__hist_q[0xeU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v14;
        vlTOPp->dma_research_policy__DOT__hist_q[0xfU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v15;
        vlTOPp->dma_research_policy__DOT__hist_q[0x10U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v16;
        vlTOPp->dma_research_policy__DOT__hist_q[0x11U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v17;
        vlTOPp->dma_research_policy__DOT__hist_q[0x12U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v18;
        vlTOPp->dma_research_policy__DOT__hist_q[0x13U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v19;
        vlTOPp->dma_research_policy__DOT__hist_q[0x14U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v20;
        vlTOPp->dma_research_policy__DOT__hist_q[0x15U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v21;
        vlTOPp->dma_research_policy__DOT__hist_q[0x16U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v22;
        vlTOPp->dma_research_policy__DOT__hist_q[0x17U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v23;
        vlTOPp->dma_research_policy__DOT__hist_q[0x18U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v24;
        vlTOPp->dma_research_policy__DOT__hist_q[0x19U] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v25;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1aU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v26;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1bU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v27;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1cU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v28;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1dU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v29;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1eU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v30;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1fU] 
            = __Vdlyvval__dma_research_policy__DOT__hist_q__v31;
    }
    if (__Vdlyvset__dma_research_policy__DOT__hist_q__v32) {
        vlTOPp->dma_research_policy__DOT__hist_q[0U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[1U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[2U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[3U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[4U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[5U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[6U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[7U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[8U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[9U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0xaU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0xbU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0xcU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0xdU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0xeU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0xfU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x10U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x11U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x12U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x13U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x14U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x15U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x16U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x17U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x18U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x19U] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1aU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1bU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1cU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1dU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1eU] = 0U;
        vlTOPp->dma_research_policy__DOT__hist_q[0x1fU] = 0U;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v0) {
        vlTOPp->dma_research_policy__DOT__stat_q[0U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v0;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v1) {
        vlTOPp->dma_research_policy__DOT__stat_q[1U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v1;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v2) {
        vlTOPp->dma_research_policy__DOT__stat_q[2U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v2;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v3) {
        vlTOPp->dma_research_policy__DOT__stat_q[3U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v3;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v4) {
        vlTOPp->dma_research_policy__DOT__stat_q[4U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v4;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v5) {
        vlTOPp->dma_research_policy__DOT__stat_q[5U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v5;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v6) {
        vlTOPp->dma_research_policy__DOT__stat_q[6U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v6;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v7) {
        vlTOPp->dma_research_policy__DOT__stat_q[7U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v7;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v8) {
        vlTOPp->dma_research_policy__DOT__stat_q[8U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v8;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v9) {
        vlTOPp->dma_research_policy__DOT__stat_q[9U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v9;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v10) {
        vlTOPp->dma_research_policy__DOT__stat_q[0xaU] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v10;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v11) {
        vlTOPp->dma_research_policy__DOT__stat_q[0xbU] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v11;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v12) {
        vlTOPp->dma_research_policy__DOT__stat_q[0xcU] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v12;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v13) {
        vlTOPp->dma_research_policy__DOT__stat_q[0xdU] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v13;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v14) {
        vlTOPp->dma_research_policy__DOT__stat_q[0xeU] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v14;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v15) {
        vlTOPp->dma_research_policy__DOT__stat_q[0xfU] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v15;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v16) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x10U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v16;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v17) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x11U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v17;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v18) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x12U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v18;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v19) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x13U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v19;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v20) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x14U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v20;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v21) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x15U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v21;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v22) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x16U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v22;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v23) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x17U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v23;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v24) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x18U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v24;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v25) {
        vlTOPp->dma_research_policy__DOT__stat_q[0x19U] 
            = __Vdlyvval__dma_research_policy__DOT__stat_q__v25;
    }
    if (__Vdlyvset__dma_research_policy__DOT__stat_q__v26) {
        vlTOPp->dma_research_policy__DOT__stat_q[0U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[1U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[2U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[3U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[4U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[5U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[6U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[7U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[8U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[9U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0xaU] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0xbU] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0xcU] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0xdU] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0xeU] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0xfU] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x10U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x11U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x12U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x13U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x14U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x15U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x16U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x17U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x18U] = 0U;
        vlTOPp->dma_research_policy__DOT__stat_q[0x19U] = 0U;
    }
    vlTOPp->dma_research_policy__DOT__win_main_q = __Vdly__dma_research_policy__DOT__win_main_q;
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->dma_research_policy__DOT__cfg_wr) 
             & (~ (IData)(vlTOPp->dma_research_policy__DOT__armed_q)))) {
            if ((1U & (~ ((IData)(vlTOPp->index_i) 
                          >> 7U)))) {
                if ((1U & (~ ((IData)(vlTOPp->index_i) 
                              >> 6U)))) {
                    if ((1U & (~ ((IData)(vlTOPp->index_i) 
                                  >> 5U)))) {
                        if ((0x10U & (IData)(vlTOPp->index_i))) {
                            if ((1U & (~ ((IData)(vlTOPp->index_i) 
                                          >> 3U)))) {
                                if ((1U & (~ ((IData)(vlTOPp->index_i) 
                                              >> 2U)))) {
                                    if ((1U & (~ ((IData)(vlTOPp->index_i) 
                                                  >> 1U)))) {
                                        if ((1U & (~ (IData)(vlTOPp->index_i)))) {
                                            vlTOPp->dma_research_policy__DOT__wptw_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    }
                                }
                            }
                        } else {
                            if ((8U & (IData)(vlTOPp->index_i))) {
                                if ((4U & (IData)(vlTOPp->index_i))) {
                                    if ((2U & (IData)(vlTOPp->index_i))) {
                                        if ((1U & (IData)(vlTOPp->index_i))) {
                                            vlTOPp->dma_research_policy__DOT__wmiss_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        } else {
                                            vlTOPp->dma_research_policy__DOT__whit_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    } else {
                                        if ((1U & (IData)(vlTOPp->index_i))) {
                                            vlTOPp->dma_research_policy__DOT__cap_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        } else {
                                            vlTOPp->dma_research_policy__DOT__step_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    }
                                } else {
                                    if ((2U & (IData)(vlTOPp->index_i))) {
                                        if ((1U & (IData)(vlTOPp->index_i))) {
                                            vlTOPp->dma_research_policy__DOT__maxbud_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        } else {
                                            vlTOPp->dma_research_policy__DOT__minbud_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    } else {
                                        if ((1U & (IData)(vlTOPp->index_i))) {
                                            vlTOPp->dma_research_policy__DOT__initbud_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        } else {
                                            vlTOPp->dma_research_policy__DOT__deadband_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    }
                                }
                            } else {
                                if ((4U & (IData)(vlTOPp->index_i))) {
                                    if ((2U & (IData)(vlTOPp->index_i))) {
                                        if ((1U & (IData)(vlTOPp->index_i))) {
                                            vlTOPp->dma_research_policy__DOT__target_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        } else {
                                            vlTOPp->dma_research_policy__DOT__window_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    } else {
                                        if ((1U & (IData)(vlTOPp->index_i))) {
                                            vlTOPp->dma_research_policy__DOT__mode_q 
                                                = (IData)(vlTOPp->wdata_i);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->dma_research_policy__DOT__mode_q = 0U;
        vlTOPp->dma_research_policy__DOT__window_q = 0x4000U;
        vlTOPp->dma_research_policy__DOT__target_q = 0xc954U;
        vlTOPp->dma_research_policy__DOT__deadband_q = 0x1000U;
        vlTOPp->dma_research_policy__DOT__initbud_q = 0x1f40U;
        vlTOPp->dma_research_policy__DOT__minbud_q = 1U;
        vlTOPp->dma_research_policy__DOT__maxbud_q = 0x8000U;
        vlTOPp->dma_research_policy__DOT__step_q = 0x200U;
        vlTOPp->dma_research_policy__DOT__cap_q = 0x10000U;
        vlTOPp->dma_research_policy__DOT__whit_q = 1U;
        vlTOPp->dma_research_policy__DOT__wmiss_q = 0x64U;
        vlTOPp->dma_research_policy__DOT__wptw_q = 0x64U;
    }
    VL_EXTEND_WQ(65,64, __Vtemp62, vlTOPp->dma_research_policy__DOT__b_q);
    VL_EXTEND_WI(65,32, __Vtemp63, vlTOPp->dma_research_policy__DOT__step_q);
    VL_ADD_W(3, __Vtemp64, __Vtemp62, __Vtemp63);
    vlTOPp->dma_research_policy__DOT__b_up_s[0U] = 
        __Vtemp64[0U];
    vlTOPp->dma_research_policy__DOT__b_up_s[1U] = 
        __Vtemp64[1U];
    vlTOPp->dma_research_policy__DOT__b_up_s[2U] = 
        (1U & __Vtemp64[2U]);
    VL_EXTEND_WQ(65,64, __Vtemp67, vlTOPp->dma_research_policy__DOT__b_q);
    VL_EXTEND_WI(65,32, __Vtemp68, vlTOPp->dma_research_policy__DOT__step_q);
    VL_SUB_W(3, __Vtemp69, __Vtemp67, __Vtemp68);
    vlTOPp->dma_research_policy__DOT__b_dn_s[0U] = 
        __Vtemp69[0U];
    vlTOPp->dma_research_policy__DOT__b_dn_s[1U] = 
        __Vtemp69[1U];
    vlTOPp->dma_research_policy__DOT__b_dn_s[2U] = 
        (1U & __Vtemp69[2U]);
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
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_policy__DOT__do_arm) {
            vlTOPp->dma_research_policy__DOT__armed_q = 1U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_freeze) {
            vlTOPp->dma_research_policy__DOT__armed_q = 0U;
        }
        if (vlTOPp->dma_research_policy__DOT__do_clear) {
            vlTOPp->dma_research_policy__DOT__armed_q = 0U;
        }
    } else {
        vlTOPp->dma_research_policy__DOT__armed_q = 0U;
    }
    vlTOPp->dma_research_policy__DOT__win_run = ((IData)(vlTOPp->dma_research_policy__DOT__armed_q) 
                                                 & (~ (IData)(vlTOPp->dma_research_policy__DOT__frozen_q)));
    vlTOPp->policy_enable_o = vlTOPp->dma_research_policy__DOT__win_run;
    vlTOPp->dma_research_policy__DOT__win_last = (((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                                                   & (0U 
                                                      != vlTOPp->dma_research_policy__DOT__window_q)) 
                                                  & (vlTOPp->dma_research_policy__DOT__win_cnt_q 
                                                     == 
                                                     ((QData)((IData)(vlTOPp->dma_research_policy__DOT__window_q)) 
                                                      - 1ULL)));
    vlTOPp->dma_research_policy__DOT__refill_en = (
                                                   ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
                                                    & (0U 
                                                       != vlTOPp->dma_research_policy__DOT__mode_q)) 
                                                   & (6U 
                                                      != vlTOPp->dma_research_policy__DOT__mode_q));
    vlTOPp->dma_research_policy__DOT__stale_cyc = ((IData)(vlTOPp->dma_research_policy__DOT__win_last) 
                                                   & (7U 
                                                      == vlTOPp->dma_research_policy__DOT__mode_q));
    vlTOPp->dma_research_policy__DOT__refill_now = 
        ((IData)(vlTOPp->dma_research_policy__DOT__refill_en)
          ? vlTOPp->dma_research_policy__DOT__b_q : 0ULL);
}

VL_INLINE_OPT void Vdma_research_policy::_combo__TOP__2(Vdma_research_policy__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_policy::_combo__TOP__2\n"); );
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
    WData/*95:0*/ __Vtemp124[3];
    WData/*95:0*/ __Vtemp125[3];
    WData/*95:0*/ __Vtemp126[3];
    WData/*95:0*/ __Vtemp129[3];
    WData/*95:0*/ __Vtemp130[3];
    WData/*95:0*/ __Vtemp131[3];
    WData/*95:0*/ __Vtemp134[3];
    WData/*95:0*/ __Vtemp135[3];
    WData/*95:0*/ __Vtemp136[3];
    WData/*95:0*/ __Vtemp139[3];
    WData/*95:0*/ __Vtemp140[3];
    WData/*95:0*/ __Vtemp141[3];
    WData/*95:0*/ __Vtemp144[3];
    WData/*95:0*/ __Vtemp145[3];
    WData/*95:0*/ __Vtemp146[3];
    WData/*95:0*/ __Vtemp149[3];
    WData/*95:0*/ __Vtemp150[3];
    WData/*95:0*/ __Vtemp151[3];
    WData/*95:0*/ __Vtemp154[3];
    WData/*95:0*/ __Vtemp155[3];
    WData/*95:0*/ __Vtemp156[3];
    WData/*95:0*/ __Vtemp159[3];
    WData/*95:0*/ __Vtemp160[3];
    WData/*95:0*/ __Vtemp161[3];
    WData/*95:0*/ __Vtemp164[3];
    WData/*95:0*/ __Vtemp165[3];
    WData/*95:0*/ __Vtemp166[3];
    WData/*95:0*/ __Vtemp169[3];
    WData/*95:0*/ __Vtemp170[3];
    WData/*95:0*/ __Vtemp171[3];
    WData/*95:0*/ __Vtemp174[3];
    WData/*95:0*/ __Vtemp175[3];
    WData/*95:0*/ __Vtemp176[3];
    WData/*95:0*/ __Vtemp179[3];
    WData/*95:0*/ __Vtemp180[3];
    WData/*95:0*/ __Vtemp181[3];
    WData/*95:0*/ __Vtemp184[3];
    WData/*95:0*/ __Vtemp185[3];
    WData/*95:0*/ __Vtemp186[3];
    WData/*95:0*/ __Vtemp189[3];
    WData/*95:0*/ __Vtemp190[3];
    WData/*95:0*/ __Vtemp191[3];
    WData/*95:0*/ __Vtemp194[3];
    WData/*95:0*/ __Vtemp195[3];
    WData/*95:0*/ __Vtemp196[3];
    WData/*95:0*/ __Vtemp199[3];
    WData/*95:0*/ __Vtemp200[3];
    WData/*95:0*/ __Vtemp201[3];
    WData/*95:0*/ __Vtemp204[3];
    WData/*95:0*/ __Vtemp205[3];
    WData/*95:0*/ __Vtemp206[3];
    WData/*95:0*/ __Vtemp209[3];
    WData/*95:0*/ __Vtemp210[3];
    WData/*95:0*/ __Vtemp211[3];
    WData/*95:0*/ __Vtemp214[3];
    WData/*95:0*/ __Vtemp215[3];
    WData/*95:0*/ __Vtemp216[3];
    WData/*95:0*/ __Vtemp219[3];
    WData/*95:0*/ __Vtemp220[3];
    WData/*95:0*/ __Vtemp221[3];
    WData/*95:0*/ __Vtemp224[3];
    WData/*95:0*/ __Vtemp225[3];
    WData/*95:0*/ __Vtemp226[3];
    WData/*95:0*/ __Vtemp229[3];
    WData/*95:0*/ __Vtemp230[3];
    WData/*95:0*/ __Vtemp231[3];
    WData/*95:0*/ __Vtemp234[3];
    WData/*95:0*/ __Vtemp235[3];
    WData/*95:0*/ __Vtemp236[3];
    WData/*95:0*/ __Vtemp239[3];
    WData/*95:0*/ __Vtemp240[3];
    WData/*95:0*/ __Vtemp241[3];
    WData/*95:0*/ __Vtemp244[3];
    WData/*95:0*/ __Vtemp245[3];
    WData/*95:0*/ __Vtemp246[3];
    WData/*95:0*/ __Vtemp249[3];
    WData/*95:0*/ __Vtemp250[3];
    WData/*95:0*/ __Vtemp251[3];
    WData/*95:0*/ __Vtemp254[3];
    WData/*95:0*/ __Vtemp255[3];
    WData/*95:0*/ __Vtemp256[3];
    WData/*95:0*/ __Vtemp259[3];
    WData/*95:0*/ __Vtemp260[3];
    WData/*95:0*/ __Vtemp261[3];
    WData/*95:0*/ __Vtemp264[3];
    WData/*95:0*/ __Vtemp265[3];
    WData/*95:0*/ __Vtemp266[3];
    WData/*95:0*/ __Vtemp269[3];
    WData/*95:0*/ __Vtemp270[3];
    WData/*95:0*/ __Vtemp271[3];
    WData/*95:0*/ __Vtemp274[3];
    WData/*95:0*/ __Vtemp275[3];
    WData/*95:0*/ __Vtemp276[3];
    WData/*95:0*/ __Vtemp279[3];
    WData/*95:0*/ __Vtemp280[3];
    WData/*95:0*/ __Vtemp281[3];
    WData/*95:0*/ __Vtemp284[3];
    WData/*95:0*/ __Vtemp285[3];
    WData/*95:0*/ __Vtemp286[3];
    WData/*95:0*/ __Vtemp289[3];
    WData/*95:0*/ __Vtemp290[3];
    WData/*95:0*/ __Vtemp291[3];
    WData/*95:0*/ __Vtemp294[3];
    WData/*95:0*/ __Vtemp295[3];
    WData/*95:0*/ __Vtemp296[3];
    WData/*95:0*/ __Vtemp299[3];
    WData/*95:0*/ __Vtemp300[3];
    WData/*95:0*/ __Vtemp301[3];
    WData/*95:0*/ __Vtemp304[3];
    WData/*95:0*/ __Vtemp305[3];
    WData/*95:0*/ __Vtemp306[3];
    WData/*95:0*/ __Vtemp309[3];
    WData/*95:0*/ __Vtemp310[3];
    WData/*95:0*/ __Vtemp311[3];
    WData/*95:0*/ __Vtemp314[3];
    WData/*95:0*/ __Vtemp315[3];
    WData/*95:0*/ __Vtemp316[3];
    WData/*95:0*/ __Vtemp319[3];
    WData/*95:0*/ __Vtemp320[3];
    WData/*95:0*/ __Vtemp321[3];
    WData/*95:0*/ __Vtemp324[3];
    WData/*95:0*/ __Vtemp325[3];
    WData/*95:0*/ __Vtemp326[3];
    WData/*95:0*/ __Vtemp329[3];
    WData/*95:0*/ __Vtemp330[3];
    WData/*95:0*/ __Vtemp331[3];
    WData/*95:0*/ __Vtemp334[3];
    WData/*95:0*/ __Vtemp335[3];
    WData/*95:0*/ __Vtemp336[3];
    WData/*95:0*/ __Vtemp339[3];
    WData/*95:0*/ __Vtemp340[3];
    WData/*95:0*/ __Vtemp341[3];
    WData/*95:0*/ __Vtemp344[3];
    WData/*95:0*/ __Vtemp345[3];
    WData/*95:0*/ __Vtemp346[3];
    WData/*95:0*/ __Vtemp349[3];
    WData/*95:0*/ __Vtemp350[3];
    WData/*95:0*/ __Vtemp351[3];
    WData/*95:0*/ __Vtemp354[3];
    WData/*95:0*/ __Vtemp355[3];
    WData/*95:0*/ __Vtemp356[3];
    WData/*95:0*/ __Vtemp359[3];
    WData/*95:0*/ __Vtemp360[3];
    WData/*95:0*/ __Vtemp361[3];
    WData/*95:0*/ __Vtemp364[3];
    WData/*95:0*/ __Vtemp365[3];
    WData/*95:0*/ __Vtemp366[3];
    WData/*95:0*/ __Vtemp369[3];
    WData/*95:0*/ __Vtemp370[3];
    WData/*95:0*/ __Vtemp371[3];
    WData/*95:0*/ __Vtemp374[3];
    WData/*95:0*/ __Vtemp375[3];
    WData/*95:0*/ __Vtemp376[3];
    WData/*95:0*/ __Vtemp379[3];
    WData/*95:0*/ __Vtemp380[3];
    WData/*95:0*/ __Vtemp381[3];
    WData/*95:0*/ __Vtemp384[3];
    WData/*95:0*/ __Vtemp385[3];
    WData/*95:0*/ __Vtemp386[3];
    WData/*95:0*/ __Vtemp389[3];
    WData/*95:0*/ __Vtemp390[3];
    WData/*95:0*/ __Vtemp391[3];
    WData/*95:0*/ __Vtemp394[3];
    WData/*95:0*/ __Vtemp395[3];
    WData/*95:0*/ __Vtemp396[3];
    WData/*95:0*/ __Vtemp399[3];
    WData/*95:0*/ __Vtemp400[3];
    WData/*95:0*/ __Vtemp401[3];
    WData/*95:0*/ __Vtemp404[3];
    WData/*95:0*/ __Vtemp405[3];
    WData/*95:0*/ __Vtemp406[3];
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
    WData/*95:0*/ __Vtemp469[3];
    WData/*95:0*/ __Vtemp470[3];
    WData/*95:0*/ __Vtemp471[3];
    WData/*95:0*/ __Vtemp472[3];
    WData/*95:0*/ __Vtemp473[3];
    WData/*95:0*/ __Vtemp474[3];
    WData/*95:0*/ __Vtemp475[3];
    WData/*95:0*/ __Vtemp476[3];
    WData/*95:0*/ __Vtemp477[3];
    WData/*95:0*/ __Vtemp478[3];
    WData/*95:0*/ __Vtemp479[3];
    WData/*95:0*/ __Vtemp480[3];
    WData/*95:0*/ __Vtemp481[3];
    WData/*95:0*/ __Vtemp482[3];
    WData/*95:0*/ __Vtemp483[3];
    WData/*95:0*/ __Vtemp484[3];
    WData/*95:0*/ __Vtemp485[3];
    WData/*95:0*/ __Vtemp486[3];
    WData/*95:0*/ __Vtemp487[3];
    WData/*95:0*/ __Vtemp488[3];
    WData/*95:0*/ __Vtemp489[3];
    WData/*95:0*/ __Vtemp490[3];
    WData/*95:0*/ __Vtemp491[3];
    WData/*95:0*/ __Vtemp492[3];
    WData/*95:0*/ __Vtemp493[3];
    WData/*95:0*/ __Vtemp494[3];
    WData/*95:0*/ __Vtemp495[3];
    WData/*95:0*/ __Vtemp496[3];
    WData/*95:0*/ __Vtemp497[3];
    WData/*95:0*/ __Vtemp498[3];
    WData/*95:0*/ __Vtemp499[3];
    WData/*95:0*/ __Vtemp500[3];
    WData/*95:0*/ __Vtemp501[3];
    WData/*95:0*/ __Vtemp502[3];
    WData/*95:0*/ __Vtemp503[3];
    WData/*95:0*/ __Vtemp504[3];
    WData/*95:0*/ __Vtemp505[3];
    WData/*95:0*/ __Vtemp506[3];
    WData/*95:0*/ __Vtemp507[3];
    WData/*95:0*/ __Vtemp508[3];
    WData/*95:0*/ __Vtemp509[3];
    WData/*95:0*/ __Vtemp510[3];
    WData/*95:0*/ __Vtemp511[3];
    WData/*95:0*/ __Vtemp512[3];
    WData/*95:0*/ __Vtemp513[3];
    WData/*95:0*/ __Vtemp514[3];
    WData/*95:0*/ __Vtemp515[3];
    WData/*95:0*/ __Vtemp516[3];
    WData/*95:0*/ __Vtemp517[3];
    WData/*95:0*/ __Vtemp518[3];
    WData/*95:0*/ __Vtemp519[3];
    WData/*95:0*/ __Vtemp520[3];
    WData/*95:0*/ __Vtemp521[3];
    WData/*95:0*/ __Vtemp522[3];
    WData/*95:0*/ __Vtemp523[3];
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
    vlTOPp->policy_abort_o = ((IData)(vlTOPp->dma_research_policy__DOT__win_run) 
                              & ((IData)(vlTOPp->dma_research_policy__DOT__abort_eff) 
                                 | (0ULL != vlTOPp->dma_research_policy__DOT__err_q)));
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
    VL_EXTEND_WQ(65,64, __Vtemp124, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp125, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp126, __Vtemp124, __Vtemp125);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp126[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp126[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp126[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp129, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp130, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp131, __Vtemp129, __Vtemp130);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp131[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp131[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp131[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp134, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp135, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp136, __Vtemp134, __Vtemp135);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp136[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp136[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp136[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp139, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp140, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp141, __Vtemp139, __Vtemp140);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp141[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp141[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp141[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp144, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp145, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp146, __Vtemp144, __Vtemp145);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp146[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp146[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp146[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp149, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp150, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp151, __Vtemp149, __Vtemp150);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp151[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp151[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp151[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp154, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp155, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp156, __Vtemp154, __Vtemp155);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp156[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp156[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp156[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp159, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp160, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp161, __Vtemp159, __Vtemp160);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp161[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp161[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp161[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp164, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp165, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp166, __Vtemp164, __Vtemp165);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp166[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp166[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp166[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp169, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp170, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp171, __Vtemp169, __Vtemp170);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp171[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp171[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp171[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp174, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp175, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp176, __Vtemp174, __Vtemp175);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp176[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp176[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp176[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp179, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp180, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp181, __Vtemp179, __Vtemp180);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp181[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp181[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp181[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp184, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp185, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp186, __Vtemp184, __Vtemp185);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp186[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp186[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp186[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp189, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp190, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp191, __Vtemp189, __Vtemp190);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp191[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp191[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp191[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp194, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp195, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp196, __Vtemp194, __Vtemp195);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp196[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp196[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp196[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp199, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp200, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp201, __Vtemp199, __Vtemp200);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp201[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp201[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp201[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp204, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp205, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp206, __Vtemp204, __Vtemp205);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp206[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp206[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp206[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp209, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp210, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp211, __Vtemp209, __Vtemp210);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp211[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp211[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp211[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp214, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp215, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp216, __Vtemp214, __Vtemp215);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp216[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp216[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp216[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp219, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp220, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp221, __Vtemp219, __Vtemp220);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp221[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp221[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp221[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp224, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp225, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp226, __Vtemp224, __Vtemp225);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp226[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp226[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp226[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp229, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp230, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp231, __Vtemp229, __Vtemp230);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp231[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp231[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp231[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp234, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp235, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp236, __Vtemp234, __Vtemp235);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp236[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp236[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp236[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp239, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp240, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp241, __Vtemp239, __Vtemp240);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp241[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp241[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp241[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp244, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__1__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp245, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__inc);
    VL_ADD_W(3, __Vtemp246, __Vtemp244, __Vtemp245);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[0U] 
        = __Vtemp246[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[1U] 
        = __Vtemp246[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__0__w[2U] 
        = (1U & __Vtemp246[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp249, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp250, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp251, __Vtemp249, __Vtemp250);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp251[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp251[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp251[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp254, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp255, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp256, __Vtemp254, __Vtemp255);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp256[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp256[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp256[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp259, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp260, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp261, __Vtemp259, __Vtemp260);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp261[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp261[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp261[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp264, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp265, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp266, __Vtemp264, __Vtemp265);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp266[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp266[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp266[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp269, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp270, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp271, __Vtemp269, __Vtemp270);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp271[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp271[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp271[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp274, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp275, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp276, __Vtemp274, __Vtemp275);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp276[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp276[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp276[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp279, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp280, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp281, __Vtemp279, __Vtemp280);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp281[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp281[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp281[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp284, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp285, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp286, __Vtemp284, __Vtemp285);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp286[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp286[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp286[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp289, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp290, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp291, __Vtemp289, __Vtemp290);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp291[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp291[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp291[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp294, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp295, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp296, __Vtemp294, __Vtemp295);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp296[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp296[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp296[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp299, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp300, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp301, __Vtemp299, __Vtemp300);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp301[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp301[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp301[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp304, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp305, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp306, __Vtemp304, __Vtemp305);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp306[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp306[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp306[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp309, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp310, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp311, __Vtemp309, __Vtemp310);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp311[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp311[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp311[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp314, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp315, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp316, __Vtemp314, __Vtemp315);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp316[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp316[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp316[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp319, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp320, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp321, __Vtemp319, __Vtemp320);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp321[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp321[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp321[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp324, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp325, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp326, __Vtemp324, __Vtemp325);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp326[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp326[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp326[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp329, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp330, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp331, __Vtemp329, __Vtemp330);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp331[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp331[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp331[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp334, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp335, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp336, __Vtemp334, __Vtemp335);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp336[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp336[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp336[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp339, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp340, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp341, __Vtemp339, __Vtemp340);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp341[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp341[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp341[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp344, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp345, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp346, __Vtemp344, __Vtemp345);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp346[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp346[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp346[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp349, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp350, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp351, __Vtemp349, __Vtemp350);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp351[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp351[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp351[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp354, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp355, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp356, __Vtemp354, __Vtemp355);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp356[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp356[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp356[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp359, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp360, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp361, __Vtemp359, __Vtemp360);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp361[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp361[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp361[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp364, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp365, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp366, __Vtemp364, __Vtemp365);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp366[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp366[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp366[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp369, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp370, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp371, __Vtemp369, __Vtemp370);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp371[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp371[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp371[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp374, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp375, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp376, __Vtemp374, __Vtemp375);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp376[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp376[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp376[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp379, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp380, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp381, __Vtemp379, __Vtemp380);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp381[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp381[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp381[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp384, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp385, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp386, __Vtemp384, __Vtemp385);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp386[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp386[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp386[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp389, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp390, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp391, __Vtemp389, __Vtemp390);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp391[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp391[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp391[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp394, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp395, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp396, __Vtemp394, __Vtemp395);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp396[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp396[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp396[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp399, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp400, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp401, __Vtemp399, __Vtemp400);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp401[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp401[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp401[2U]);
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
    VL_EXTEND_WQ(65,64, __Vtemp404, vlTOPp->__Vfunc_dma_research_policy__DOT__zext64__3__Vfuncout);
    VL_EXTEND_WQ(65,64, __Vtemp405, vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__inc);
    VL_ADD_W(3, __Vtemp406, __Vtemp404, __Vtemp405);
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[0U] 
        = __Vtemp406[0U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[1U] 
        = __Vtemp406[1U];
    vlTOPp->__Vfunc_dma_research_policy__DOT__cadd__2__w[2U] 
        = (1U & __Vtemp406[2U]);
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
    __Vtemp408[0U] = 0xffU;
    __Vtemp408[1U] = 0U;
    __Vtemp408[2U] = 0U;
    __Vtemp409[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][0U];
    __Vtemp409[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][1U];
    __Vtemp409[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp408, __Vtemp409)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp410[0U] = 0xffU;
    __Vtemp410[1U] = 0U;
    __Vtemp410[2U] = 0U;
    __Vtemp411[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][0U];
    __Vtemp411[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][1U];
    __Vtemp411[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp410, __Vtemp411)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp412[0U] = 0xffU;
    __Vtemp412[1U] = 0U;
    __Vtemp412[2U] = 0U;
    __Vtemp413[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][0U];
    __Vtemp413[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][1U];
    __Vtemp413[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp412, __Vtemp413)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp414[0U] = 0xffU;
    __Vtemp414[1U] = 0U;
    __Vtemp414[2U] = 0U;
    __Vtemp415[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][0U];
    __Vtemp415[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][1U];
    __Vtemp415[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp414, __Vtemp415)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp416[0U] = 0xffU;
    __Vtemp416[1U] = 0U;
    __Vtemp416[2U] = 0U;
    __Vtemp417[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][0U];
    __Vtemp417[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][1U];
    __Vtemp417[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp416, __Vtemp417)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp418[0U] = 0xffU;
    __Vtemp418[1U] = 0U;
    __Vtemp418[2U] = 0U;
    __Vtemp419[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][0U];
    __Vtemp419[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][1U];
    __Vtemp419[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp418, __Vtemp419)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp420[0U] = 0xffU;
    __Vtemp420[1U] = 0U;
    __Vtemp420[2U] = 0U;
    __Vtemp421[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][0U];
    __Vtemp421[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][1U];
    __Vtemp421[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp420, __Vtemp421)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp422[0U] = 0xffU;
    __Vtemp422[1U] = 0U;
    __Vtemp422[2U] = 0U;
    __Vtemp423[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][0U];
    __Vtemp423[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][1U];
    __Vtemp423[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp422, __Vtemp423)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp424[0U] = 0xffU;
    __Vtemp424[1U] = 0U;
    __Vtemp424[2U] = 0U;
    __Vtemp425[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][0U];
    __Vtemp425[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][1U];
    __Vtemp425[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp424, __Vtemp425)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp426[0U] = 0xffU;
    __Vtemp426[1U] = 0U;
    __Vtemp426[2U] = 0U;
    __Vtemp427[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][0U];
    __Vtemp427[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][1U];
    __Vtemp427[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp426, __Vtemp427)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp428[0U] = 0xffU;
    __Vtemp428[1U] = 0U;
    __Vtemp428[2U] = 0U;
    __Vtemp429[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][0U];
    __Vtemp429[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][1U];
    __Vtemp429[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp428, __Vtemp429)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp430[0U] = 0xffU;
    __Vtemp430[1U] = 0U;
    __Vtemp430[2U] = 0U;
    __Vtemp431[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][0U];
    __Vtemp431[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][1U];
    __Vtemp431[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp430, __Vtemp431)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp432[0U] = 0xffU;
    __Vtemp432[1U] = 0U;
    __Vtemp432[2U] = 0U;
    __Vtemp433[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][0U];
    __Vtemp433[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][1U];
    __Vtemp433[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp432, __Vtemp433)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp434[0U] = 0xffU;
    __Vtemp434[1U] = 0U;
    __Vtemp434[2U] = 0U;
    __Vtemp435[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][0U];
    __Vtemp435[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][1U];
    __Vtemp435[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp434, __Vtemp435)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp436[0U] = 0xffU;
    __Vtemp436[1U] = 0U;
    __Vtemp436[2U] = 0U;
    __Vtemp437[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][0U];
    __Vtemp437[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][1U];
    __Vtemp437[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp436, __Vtemp437)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp438[0U] = 0xffU;
    __Vtemp438[1U] = 0U;
    __Vtemp438[2U] = 0U;
    __Vtemp439[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][0U];
    __Vtemp439[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][1U];
    __Vtemp439[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp438, __Vtemp439)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp440[0U] = 0xffU;
    __Vtemp440[1U] = 0U;
    __Vtemp440[2U] = 0U;
    __Vtemp441[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][0U];
    __Vtemp441[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][1U];
    __Vtemp441[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp440, __Vtemp441)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp442[0U] = 0xffU;
    __Vtemp442[1U] = 0U;
    __Vtemp442[2U] = 0U;
    __Vtemp443[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][0U];
    __Vtemp443[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][1U];
    __Vtemp443[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp442, __Vtemp443)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp444[0U] = 0xffU;
    __Vtemp444[1U] = 0U;
    __Vtemp444[2U] = 0U;
    __Vtemp445[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][0U];
    __Vtemp445[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][1U];
    __Vtemp445[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp444, __Vtemp445)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp446[0U] = 0xffU;
    __Vtemp446[1U] = 0U;
    __Vtemp446[2U] = 0U;
    __Vtemp447[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][0U];
    __Vtemp447[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][1U];
    __Vtemp447[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp446, __Vtemp447)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp448[0U] = 0xffU;
    __Vtemp448[1U] = 0U;
    __Vtemp448[2U] = 0U;
    __Vtemp449[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][0U];
    __Vtemp449[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][1U];
    __Vtemp449[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp448, __Vtemp449)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp450[0U] = 0xffU;
    __Vtemp450[1U] = 0U;
    __Vtemp450[2U] = 0U;
    __Vtemp451[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][0U];
    __Vtemp451[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][1U];
    __Vtemp451[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp450, __Vtemp451)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp452[0U] = 0xffU;
    __Vtemp452[1U] = 0U;
    __Vtemp452[2U] = 0U;
    __Vtemp453[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][0U];
    __Vtemp453[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][1U];
    __Vtemp453[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp452, __Vtemp453)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp454[0U] = 0xffU;
    __Vtemp454[1U] = 0U;
    __Vtemp454[2U] = 0U;
    __Vtemp455[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][0U];
    __Vtemp455[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][1U];
    __Vtemp455[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp454, __Vtemp455)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp456[0U] = 0xffU;
    __Vtemp456[1U] = 0U;
    __Vtemp456[2U] = 0U;
    __Vtemp457[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][0U];
    __Vtemp457[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][1U];
    __Vtemp457[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp456, __Vtemp457)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp458[0U] = 0xffU;
    __Vtemp458[1U] = 0U;
    __Vtemp458[2U] = 0U;
    __Vtemp459[0U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][0U];
    __Vtemp459[1U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][1U];
    __Vtemp459[2U] = vlTOPp->dma_research_policy__DOT__stat_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp458, __Vtemp459)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp460[0U] = 0xffU;
    __Vtemp460[1U] = 0U;
    __Vtemp460[2U] = 0U;
    __Vtemp461[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][0U];
    __Vtemp461[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][1U];
    __Vtemp461[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0U][2U];
    if (VL_LT_W(3, __Vtemp460, __Vtemp461)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp462[0U] = 0xffU;
    __Vtemp462[1U] = 0U;
    __Vtemp462[2U] = 0U;
    __Vtemp463[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][0U];
    __Vtemp463[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][1U];
    __Vtemp463[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [1U][2U];
    if (VL_LT_W(3, __Vtemp462, __Vtemp463)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp464[0U] = 0xffU;
    __Vtemp464[1U] = 0U;
    __Vtemp464[2U] = 0U;
    __Vtemp465[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][0U];
    __Vtemp465[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][1U];
    __Vtemp465[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [2U][2U];
    if (VL_LT_W(3, __Vtemp464, __Vtemp465)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp466[0U] = 0xffU;
    __Vtemp466[1U] = 0U;
    __Vtemp466[2U] = 0U;
    __Vtemp467[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][0U];
    __Vtemp467[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][1U];
    __Vtemp467[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [3U][2U];
    if (VL_LT_W(3, __Vtemp466, __Vtemp467)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp468[0U] = 0xffU;
    __Vtemp468[1U] = 0U;
    __Vtemp468[2U] = 0U;
    __Vtemp469[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][0U];
    __Vtemp469[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][1U];
    __Vtemp469[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [4U][2U];
    if (VL_LT_W(3, __Vtemp468, __Vtemp469)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp470[0U] = 0xffU;
    __Vtemp470[1U] = 0U;
    __Vtemp470[2U] = 0U;
    __Vtemp471[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][0U];
    __Vtemp471[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][1U];
    __Vtemp471[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [5U][2U];
    if (VL_LT_W(3, __Vtemp470, __Vtemp471)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp472[0U] = 0xffU;
    __Vtemp472[1U] = 0U;
    __Vtemp472[2U] = 0U;
    __Vtemp473[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][0U];
    __Vtemp473[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][1U];
    __Vtemp473[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [6U][2U];
    if (VL_LT_W(3, __Vtemp472, __Vtemp473)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp474[0U] = 0xffU;
    __Vtemp474[1U] = 0U;
    __Vtemp474[2U] = 0U;
    __Vtemp475[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][0U];
    __Vtemp475[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][1U];
    __Vtemp475[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [7U][2U];
    if (VL_LT_W(3, __Vtemp474, __Vtemp475)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp476[0U] = 0xffU;
    __Vtemp476[1U] = 0U;
    __Vtemp476[2U] = 0U;
    __Vtemp477[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][0U];
    __Vtemp477[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][1U];
    __Vtemp477[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [8U][2U];
    if (VL_LT_W(3, __Vtemp476, __Vtemp477)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp478[0U] = 0xffU;
    __Vtemp478[1U] = 0U;
    __Vtemp478[2U] = 0U;
    __Vtemp479[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][0U];
    __Vtemp479[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][1U];
    __Vtemp479[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [9U][2U];
    if (VL_LT_W(3, __Vtemp478, __Vtemp479)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp480[0U] = 0xffU;
    __Vtemp480[1U] = 0U;
    __Vtemp480[2U] = 0U;
    __Vtemp481[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][0U];
    __Vtemp481[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][1U];
    __Vtemp481[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xaU][2U];
    if (VL_LT_W(3, __Vtemp480, __Vtemp481)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp482[0U] = 0xffU;
    __Vtemp482[1U] = 0U;
    __Vtemp482[2U] = 0U;
    __Vtemp483[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][0U];
    __Vtemp483[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][1U];
    __Vtemp483[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xbU][2U];
    if (VL_LT_W(3, __Vtemp482, __Vtemp483)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp484[0U] = 0xffU;
    __Vtemp484[1U] = 0U;
    __Vtemp484[2U] = 0U;
    __Vtemp485[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][0U];
    __Vtemp485[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][1U];
    __Vtemp485[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xcU][2U];
    if (VL_LT_W(3, __Vtemp484, __Vtemp485)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp486[0U] = 0xffU;
    __Vtemp486[1U] = 0U;
    __Vtemp486[2U] = 0U;
    __Vtemp487[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][0U];
    __Vtemp487[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][1U];
    __Vtemp487[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xdU][2U];
    if (VL_LT_W(3, __Vtemp486, __Vtemp487)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp488[0U] = 0xffU;
    __Vtemp488[1U] = 0U;
    __Vtemp488[2U] = 0U;
    __Vtemp489[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][0U];
    __Vtemp489[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][1U];
    __Vtemp489[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xeU][2U];
    if (VL_LT_W(3, __Vtemp488, __Vtemp489)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp490[0U] = 0xffU;
    __Vtemp490[1U] = 0U;
    __Vtemp490[2U] = 0U;
    __Vtemp491[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][0U];
    __Vtemp491[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][1U];
    __Vtemp491[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0xfU][2U];
    if (VL_LT_W(3, __Vtemp490, __Vtemp491)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp492[0U] = 0xffU;
    __Vtemp492[1U] = 0U;
    __Vtemp492[2U] = 0U;
    __Vtemp493[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][0U];
    __Vtemp493[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][1U];
    __Vtemp493[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x10U][2U];
    if (VL_LT_W(3, __Vtemp492, __Vtemp493)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp494[0U] = 0xffU;
    __Vtemp494[1U] = 0U;
    __Vtemp494[2U] = 0U;
    __Vtemp495[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][0U];
    __Vtemp495[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][1U];
    __Vtemp495[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x11U][2U];
    if (VL_LT_W(3, __Vtemp494, __Vtemp495)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp496[0U] = 0xffU;
    __Vtemp496[1U] = 0U;
    __Vtemp496[2U] = 0U;
    __Vtemp497[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][0U];
    __Vtemp497[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][1U];
    __Vtemp497[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x12U][2U];
    if (VL_LT_W(3, __Vtemp496, __Vtemp497)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp498[0U] = 0xffU;
    __Vtemp498[1U] = 0U;
    __Vtemp498[2U] = 0U;
    __Vtemp499[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][0U];
    __Vtemp499[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][1U];
    __Vtemp499[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x13U][2U];
    if (VL_LT_W(3, __Vtemp498, __Vtemp499)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp500[0U] = 0xffU;
    __Vtemp500[1U] = 0U;
    __Vtemp500[2U] = 0U;
    __Vtemp501[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][0U];
    __Vtemp501[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][1U];
    __Vtemp501[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x14U][2U];
    if (VL_LT_W(3, __Vtemp500, __Vtemp501)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp502[0U] = 0xffU;
    __Vtemp502[1U] = 0U;
    __Vtemp502[2U] = 0U;
    __Vtemp503[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][0U];
    __Vtemp503[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][1U];
    __Vtemp503[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x15U][2U];
    if (VL_LT_W(3, __Vtemp502, __Vtemp503)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp504[0U] = 0xffU;
    __Vtemp504[1U] = 0U;
    __Vtemp504[2U] = 0U;
    __Vtemp505[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][0U];
    __Vtemp505[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][1U];
    __Vtemp505[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x16U][2U];
    if (VL_LT_W(3, __Vtemp504, __Vtemp505)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp506[0U] = 0xffU;
    __Vtemp506[1U] = 0U;
    __Vtemp506[2U] = 0U;
    __Vtemp507[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][0U];
    __Vtemp507[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][1U];
    __Vtemp507[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x17U][2U];
    if (VL_LT_W(3, __Vtemp506, __Vtemp507)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp508[0U] = 0xffU;
    __Vtemp508[1U] = 0U;
    __Vtemp508[2U] = 0U;
    __Vtemp509[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][0U];
    __Vtemp509[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][1U];
    __Vtemp509[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x18U][2U];
    if (VL_LT_W(3, __Vtemp508, __Vtemp509)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp510[0U] = 0xffU;
    __Vtemp510[1U] = 0U;
    __Vtemp510[2U] = 0U;
    __Vtemp511[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][0U];
    __Vtemp511[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][1U];
    __Vtemp511[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x19U][2U];
    if (VL_LT_W(3, __Vtemp510, __Vtemp511)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp512[0U] = 0xffU;
    __Vtemp512[1U] = 0U;
    __Vtemp512[2U] = 0U;
    __Vtemp513[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][0U];
    __Vtemp513[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][1U];
    __Vtemp513[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1aU][2U];
    if (VL_LT_W(3, __Vtemp512, __Vtemp513)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp514[0U] = 0xffU;
    __Vtemp514[1U] = 0U;
    __Vtemp514[2U] = 0U;
    __Vtemp515[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][0U];
    __Vtemp515[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][1U];
    __Vtemp515[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1bU][2U];
    if (VL_LT_W(3, __Vtemp514, __Vtemp515)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp516[0U] = 0xffU;
    __Vtemp516[1U] = 0U;
    __Vtemp516[2U] = 0U;
    __Vtemp517[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][0U];
    __Vtemp517[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][1U];
    __Vtemp517[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1cU][2U];
    if (VL_LT_W(3, __Vtemp516, __Vtemp517)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp518[0U] = 0xffU;
    __Vtemp518[1U] = 0U;
    __Vtemp518[2U] = 0U;
    __Vtemp519[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][0U];
    __Vtemp519[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][1U];
    __Vtemp519[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1dU][2U];
    if (VL_LT_W(3, __Vtemp518, __Vtemp519)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp520[0U] = 0xffU;
    __Vtemp520[1U] = 0U;
    __Vtemp520[2U] = 0U;
    __Vtemp521[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][0U];
    __Vtemp521[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][1U];
    __Vtemp521[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1eU][2U];
    if (VL_LT_W(3, __Vtemp520, __Vtemp521)) {
        vlTOPp->dma_research_policy__DOT__cnt_ovf = 1U;
    }
    __Vtemp522[0U] = 0xffU;
    __Vtemp522[1U] = 0U;
    __Vtemp522[2U] = 0U;
    __Vtemp523[0U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][0U];
    __Vtemp523[1U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][1U];
    __Vtemp523[2U] = vlTOPp->dma_research_policy__DOT__hist_sum
        [0x1fU][2U];
    if (VL_LT_W(3, __Vtemp522, __Vtemp523)) {
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
