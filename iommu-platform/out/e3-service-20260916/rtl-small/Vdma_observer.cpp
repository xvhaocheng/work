// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_observer.h for the primary calling header

#include "Vdma_observer.h"
#include "Vdma_observer__Syms.h"

//==========

void Vdma_observer::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_observer::eval\n"); );
    Vdma_observer__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_observer.sv", 3, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_observer::_eval_initial_loop(Vdma_observer__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_observer.sv", 3, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_observer::_sequent__TOP__1(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_sequent__TOP__1\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__writes_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__writes_q__v0;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__writes_q__v1;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__writes_q__v2;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__writes_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__writes_q__v4;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__reads_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__reads_q__v0;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__reads_q__v1;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__reads_q__v2;
    CData/*2:0*/ __Vdlyvval__dma_observer__DOT__reads_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__reads_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v1;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v3;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v4;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v5;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v7;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v7;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v8;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v9;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v9;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v10;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v10;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v11;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v11;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v12;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v12;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v13;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v13;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v14;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v14;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v15;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v15;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v16;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v17;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v17;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v18;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v18;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v19;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v19;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v20;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v20;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v21;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v21;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v22;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v22;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v23;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v23;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v24;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v24;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v25;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v25;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v26;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v26;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v27;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v27;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v28;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v28;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v29;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v29;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v30;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v30;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v31;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v31;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v32;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v32;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v33;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v33;
    CData/*7:0*/ __Vdlyvval__dma_observer__DOT__count_q__v34;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v34;
    CData/*0:0*/ __Vdlyvset__dma_observer__DOT__count_q__v35;
    // Body
    __Vdlyvset__dma_observer__DOT__reads_q__v0 = 0U;
    __Vdlyvset__dma_observer__DOT__reads_q__v4 = 0U;
    __Vdlyvset__dma_observer__DOT__writes_q__v0 = 0U;
    __Vdlyvset__dma_observer__DOT__writes_q__v4 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v0 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v1 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v2 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v3 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v4 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v5 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v6 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v7 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v8 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v9 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v10 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v11 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v12 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v13 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v14 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v15 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v16 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v17 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v18 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v19 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v20 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v21 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v22 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v23 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v24 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v25 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v26 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v27 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v28 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v29 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v30 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v31 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v32 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v33 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v34 = 0U;
    __Vdlyvset__dma_observer__DOT__count_q__v35 = 0U;
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_observer__DOT__index_q = vlTOPp->dma_observer__DOT__index_n;
        vlTOPp->dma_observer__DOT__e3_q = ((IData)(vlTOPp->dma_observer__DOT__e3_n) 
                                           & 1U);
        vlTOPp->dma_observer__DOT__delay_q = ((IData)(vlTOPp->dma_observer__DOT__delay_n) 
                                              & 1U);
        vlTOPp->dma_observer__DOT__expected_q = vlTOPp->dma_observer__DOT__expected_n;
        vlTOPp->dma_observer__DOT__aggressor_q = vlTOPp->dma_observer__DOT__aggressor_n;
        vlTOPp->dma_observer__DOT__has_a_q = ((IData)(vlTOPp->dma_observer__DOT__has_a_n) 
                                              & 1U);
        vlTOPp->dma_observer__DOT__batch_q = ((IData)(vlTOPp->dma_observer__DOT__batch_n) 
                                              & 1U);
        vlTOPp->dma_observer__DOT__victim_q = vlTOPp->dma_observer__DOT__victim_n;
        vlTOPp->dma_observer__DOT__track_bad_q = vlTOPp->dma_observer__DOT__track_bad_n;
        vlTOPp->dma_observer__DOT__segment_q = vlTOPp->dma_observer__DOT__segment_n;
        vlTOPp->dma_observer__DOT__error_q = vlTOPp->dma_observer__DOT__error_n;
        vlTOPp->dma_observer__DOT__target_q = vlTOPp->dma_observer__DOT__target_n;
    } else {
        vlTOPp->dma_observer__DOT__index_q = 0U;
        vlTOPp->dma_observer__DOT__e3_q = 0U;
        vlTOPp->dma_observer__DOT__delay_q = 0U;
        vlTOPp->dma_observer__DOT__expected_q = 0ULL;
        vlTOPp->dma_observer__DOT__aggressor_q = 0U;
        vlTOPp->dma_observer__DOT__has_a_q = 0U;
        vlTOPp->dma_observer__DOT__batch_q = 0U;
        vlTOPp->dma_observer__DOT__victim_q = 0U;
        vlTOPp->dma_observer__DOT__track_bad_q = 0U;
        vlTOPp->dma_observer__DOT__segment_q = 0U;
        vlTOPp->dma_observer__DOT__error_q = 0U;
        vlTOPp->dma_observer__DOT__target_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_observer__DOT__reads_q__v0 
            = vlTOPp->dma_observer__DOT__reads_n[0U];
        __Vdlyvset__dma_observer__DOT__reads_q__v0 = 1U;
        __Vdlyvval__dma_observer__DOT__reads_q__v1 
            = vlTOPp->dma_observer__DOT__reads_n[1U];
        __Vdlyvval__dma_observer__DOT__reads_q__v2 
            = vlTOPp->dma_observer__DOT__reads_n[2U];
        __Vdlyvval__dma_observer__DOT__reads_q__v3 
            = vlTOPp->dma_observer__DOT__reads_n[3U];
    } else {
        __Vdlyvset__dma_observer__DOT__reads_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_observer__DOT__writes_q__v0 
            = vlTOPp->dma_observer__DOT__writes_n[0U];
        __Vdlyvset__dma_observer__DOT__writes_q__v0 = 1U;
        __Vdlyvval__dma_observer__DOT__writes_q__v1 
            = vlTOPp->dma_observer__DOT__writes_n[1U];
        __Vdlyvval__dma_observer__DOT__writes_q__v2 
            = vlTOPp->dma_observer__DOT__writes_n[2U];
        __Vdlyvval__dma_observer__DOT__writes_q__v3 
            = vlTOPp->dma_observer__DOT__writes_n[3U];
    } else {
        __Vdlyvset__dma_observer__DOT__writes_q__v4 = 1U;
    }
    vlTOPp->dma_observer__DOT__phase_q = ((IData)(vlTOPp->rst_ni)
                                           ? (IData)(vlTOPp->dma_observer__DOT__phase_n)
                                           : 0U);
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0U];
        __Vdlyvval__dma_observer__DOT__count_q__v0 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v0 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [1U];
        __Vdlyvval__dma_observer__DOT__count_q__v1 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v1 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [2U];
        __Vdlyvval__dma_observer__DOT__count_q__v2 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v2 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [3U];
        __Vdlyvval__dma_observer__DOT__count_q__v3 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v3 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [4U];
        __Vdlyvval__dma_observer__DOT__count_q__v4 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v4 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [5U];
        __Vdlyvval__dma_observer__DOT__count_q__v5 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v5 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [6U];
        __Vdlyvval__dma_observer__DOT__count_q__v6 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v6 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [7U];
        __Vdlyvval__dma_observer__DOT__count_q__v7 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v7 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [8U];
        __Vdlyvval__dma_observer__DOT__count_q__v8 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v8 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [9U];
        __Vdlyvval__dma_observer__DOT__count_q__v9 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v9 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0xaU];
        __Vdlyvval__dma_observer__DOT__count_q__v10 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v10 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0xbU];
        __Vdlyvval__dma_observer__DOT__count_q__v11 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v11 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0xcU];
        __Vdlyvval__dma_observer__DOT__count_q__v12 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v12 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0xdU];
        __Vdlyvval__dma_observer__DOT__count_q__v13 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v13 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0xeU];
        __Vdlyvval__dma_observer__DOT__count_q__v14 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v14 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0xfU];
        __Vdlyvval__dma_observer__DOT__count_q__v15 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v15 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x10U];
        __Vdlyvval__dma_observer__DOT__count_q__v16 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v16 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x11U];
        __Vdlyvval__dma_observer__DOT__count_q__v17 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v17 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x12U];
        __Vdlyvval__dma_observer__DOT__count_q__v18 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v18 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x13U];
        __Vdlyvval__dma_observer__DOT__count_q__v19 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v19 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x14U];
        __Vdlyvval__dma_observer__DOT__count_q__v20 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v20 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x15U];
        __Vdlyvval__dma_observer__DOT__count_q__v21 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v21 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x16U];
        __Vdlyvval__dma_observer__DOT__count_q__v22 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v22 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x17U];
        __Vdlyvval__dma_observer__DOT__count_q__v23 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v23 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x18U];
        __Vdlyvval__dma_observer__DOT__count_q__v24 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v24 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x19U];
        __Vdlyvval__dma_observer__DOT__count_q__v25 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v25 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x1aU];
        __Vdlyvval__dma_observer__DOT__count_q__v26 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v26 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x1bU];
        __Vdlyvval__dma_observer__DOT__count_q__v27 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v27 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x1cU];
        __Vdlyvval__dma_observer__DOT__count_q__v28 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v28 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x1dU];
        __Vdlyvval__dma_observer__DOT__count_q__v29 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v29 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x1eU];
        __Vdlyvval__dma_observer__DOT__count_q__v30 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v30 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x1fU];
        __Vdlyvval__dma_observer__DOT__count_q__v31 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v31 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x20U];
        __Vdlyvval__dma_observer__DOT__count_q__v32 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v32 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x21U];
        __Vdlyvval__dma_observer__DOT__count_q__v33 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v33 = 1U;
        vlTOPp->dma_observer__DOT____Vlvbound4 = vlTOPp->dma_observer__DOT__count_n
            [0x22U];
        __Vdlyvval__dma_observer__DOT__count_q__v34 
            = vlTOPp->dma_observer__DOT____Vlvbound4;
        __Vdlyvset__dma_observer__DOT__count_q__v34 = 1U;
    } else {
        __Vdlyvset__dma_observer__DOT__count_q__v35 = 1U;
    }
    if (__Vdlyvset__dma_observer__DOT__reads_q__v0) {
        vlTOPp->dma_observer__DOT__reads_q[0U] = __Vdlyvval__dma_observer__DOT__reads_q__v0;
        vlTOPp->dma_observer__DOT__reads_q[1U] = __Vdlyvval__dma_observer__DOT__reads_q__v1;
        vlTOPp->dma_observer__DOT__reads_q[2U] = __Vdlyvval__dma_observer__DOT__reads_q__v2;
        vlTOPp->dma_observer__DOT__reads_q[3U] = __Vdlyvval__dma_observer__DOT__reads_q__v3;
    }
    if (__Vdlyvset__dma_observer__DOT__reads_q__v4) {
        vlTOPp->dma_observer__DOT__reads_q[0U] = 0U;
        vlTOPp->dma_observer__DOT__reads_q[1U] = 0U;
        vlTOPp->dma_observer__DOT__reads_q[2U] = 0U;
        vlTOPp->dma_observer__DOT__reads_q[3U] = 0U;
    }
    if (__Vdlyvset__dma_observer__DOT__writes_q__v0) {
        vlTOPp->dma_observer__DOT__writes_q[0U] = __Vdlyvval__dma_observer__DOT__writes_q__v0;
        vlTOPp->dma_observer__DOT__writes_q[1U] = __Vdlyvval__dma_observer__DOT__writes_q__v1;
        vlTOPp->dma_observer__DOT__writes_q[2U] = __Vdlyvval__dma_observer__DOT__writes_q__v2;
        vlTOPp->dma_observer__DOT__writes_q[3U] = __Vdlyvval__dma_observer__DOT__writes_q__v3;
    }
    if (__Vdlyvset__dma_observer__DOT__writes_q__v4) {
        vlTOPp->dma_observer__DOT__writes_q[0U] = 0U;
        vlTOPp->dma_observer__DOT__writes_q[1U] = 0U;
        vlTOPp->dma_observer__DOT__writes_q[2U] = 0U;
        vlTOPp->dma_observer__DOT__writes_q[3U] = 0U;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v0) {
        vlTOPp->dma_observer__DOT__count_q[0U] = __Vdlyvval__dma_observer__DOT__count_q__v0;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v1) {
        vlTOPp->dma_observer__DOT__count_q[1U] = __Vdlyvval__dma_observer__DOT__count_q__v1;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v2) {
        vlTOPp->dma_observer__DOT__count_q[2U] = __Vdlyvval__dma_observer__DOT__count_q__v2;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v3) {
        vlTOPp->dma_observer__DOT__count_q[3U] = __Vdlyvval__dma_observer__DOT__count_q__v3;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v4) {
        vlTOPp->dma_observer__DOT__count_q[4U] = __Vdlyvval__dma_observer__DOT__count_q__v4;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v5) {
        vlTOPp->dma_observer__DOT__count_q[5U] = __Vdlyvval__dma_observer__DOT__count_q__v5;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v6) {
        vlTOPp->dma_observer__DOT__count_q[6U] = __Vdlyvval__dma_observer__DOT__count_q__v6;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v7) {
        vlTOPp->dma_observer__DOT__count_q[7U] = __Vdlyvval__dma_observer__DOT__count_q__v7;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v8) {
        vlTOPp->dma_observer__DOT__count_q[8U] = __Vdlyvval__dma_observer__DOT__count_q__v8;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v9) {
        vlTOPp->dma_observer__DOT__count_q[9U] = __Vdlyvval__dma_observer__DOT__count_q__v9;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v10) {
        vlTOPp->dma_observer__DOT__count_q[0xaU] = __Vdlyvval__dma_observer__DOT__count_q__v10;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v11) {
        vlTOPp->dma_observer__DOT__count_q[0xbU] = __Vdlyvval__dma_observer__DOT__count_q__v11;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v12) {
        vlTOPp->dma_observer__DOT__count_q[0xcU] = __Vdlyvval__dma_observer__DOT__count_q__v12;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v13) {
        vlTOPp->dma_observer__DOT__count_q[0xdU] = __Vdlyvval__dma_observer__DOT__count_q__v13;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v14) {
        vlTOPp->dma_observer__DOT__count_q[0xeU] = __Vdlyvval__dma_observer__DOT__count_q__v14;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v15) {
        vlTOPp->dma_observer__DOT__count_q[0xfU] = __Vdlyvval__dma_observer__DOT__count_q__v15;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v16) {
        vlTOPp->dma_observer__DOT__count_q[0x10U] = __Vdlyvval__dma_observer__DOT__count_q__v16;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v17) {
        vlTOPp->dma_observer__DOT__count_q[0x11U] = __Vdlyvval__dma_observer__DOT__count_q__v17;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v18) {
        vlTOPp->dma_observer__DOT__count_q[0x12U] = __Vdlyvval__dma_observer__DOT__count_q__v18;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v19) {
        vlTOPp->dma_observer__DOT__count_q[0x13U] = __Vdlyvval__dma_observer__DOT__count_q__v19;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v20) {
        vlTOPp->dma_observer__DOT__count_q[0x14U] = __Vdlyvval__dma_observer__DOT__count_q__v20;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v21) {
        vlTOPp->dma_observer__DOT__count_q[0x15U] = __Vdlyvval__dma_observer__DOT__count_q__v21;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v22) {
        vlTOPp->dma_observer__DOT__count_q[0x16U] = __Vdlyvval__dma_observer__DOT__count_q__v22;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v23) {
        vlTOPp->dma_observer__DOT__count_q[0x17U] = __Vdlyvval__dma_observer__DOT__count_q__v23;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v24) {
        vlTOPp->dma_observer__DOT__count_q[0x18U] = __Vdlyvval__dma_observer__DOT__count_q__v24;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v25) {
        vlTOPp->dma_observer__DOT__count_q[0x19U] = __Vdlyvval__dma_observer__DOT__count_q__v25;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v26) {
        vlTOPp->dma_observer__DOT__count_q[0x1aU] = __Vdlyvval__dma_observer__DOT__count_q__v26;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v27) {
        vlTOPp->dma_observer__DOT__count_q[0x1bU] = __Vdlyvval__dma_observer__DOT__count_q__v27;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v28) {
        vlTOPp->dma_observer__DOT__count_q[0x1cU] = __Vdlyvval__dma_observer__DOT__count_q__v28;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v29) {
        vlTOPp->dma_observer__DOT__count_q[0x1dU] = __Vdlyvval__dma_observer__DOT__count_q__v29;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v30) {
        vlTOPp->dma_observer__DOT__count_q[0x1eU] = __Vdlyvval__dma_observer__DOT__count_q__v30;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v31) {
        vlTOPp->dma_observer__DOT__count_q[0x1fU] = __Vdlyvval__dma_observer__DOT__count_q__v31;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v32) {
        vlTOPp->dma_observer__DOT__count_q[0x20U] = __Vdlyvval__dma_observer__DOT__count_q__v32;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v33) {
        vlTOPp->dma_observer__DOT__count_q[0x21U] = __Vdlyvval__dma_observer__DOT__count_q__v33;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v34) {
        vlTOPp->dma_observer__DOT__count_q[0x22U] = __Vdlyvval__dma_observer__DOT__count_q__v34;
    }
    if (__Vdlyvset__dma_observer__DOT__count_q__v35) {
        vlTOPp->dma_observer__DOT__count_q[0U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[1U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[2U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[3U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[4U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[5U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[6U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[7U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[8U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[9U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0xaU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0xbU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0xcU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0xdU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0xeU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0xfU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x10U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x11U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x12U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x13U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x14U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x15U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x16U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x17U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x18U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x19U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x1aU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x1bU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x1cU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x1dU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x1eU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x1fU] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x20U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x21U] = 0U;
        vlTOPp->dma_observer__DOT__count_q[0x22U] = 0U;
    }
    vlTOPp->e3_index_o = (0xffU & ((IData)(vlTOPp->dma_observer__DOT__index_q) 
                                   - (IData)(0xc0U)));
    vlTOPp->dma_observer__DOT__active = (((2U == (IData)(vlTOPp->dma_observer__DOT__phase_q)) 
                                          | (3U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                         | (4U == (IData)(vlTOPp->dma_observer__DOT__phase_q)));
    vlTOPp->dma_observer__DOT__busy = ((IData)(vlTOPp->dma_observer__DOT__active) 
                                       | (6U == (IData)(vlTOPp->dma_observer__DOT__phase_q)));
}

VL_INLINE_OPT void Vdma_observer::_combo__TOP__3(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_combo__TOP__3\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_observer__DOT__index_n = vlTOPp->dma_observer__DOT__index_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x23ULL > vlTOPp->cfg_wdata_i)) {
                        vlTOPp->dma_observer__DOT__index_n 
                            = (0x1ffU & (IData)(vlTOPp->cfg_wdata_i));
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__reads_n[0U] = vlTOPp->dma_observer__DOT__reads_q
        [0U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       << 1U)) | (1U & (IData)(vlTOPp->rlast_fire_i))))) {
        vlTOPp->dma_observer__DOT__reads_n[0U] = (7U 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [0U]));
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           << 1U)) | (1U & (IData)(vlTOPp->rlast_fire_i))))) {
            vlTOPp->dma_observer__DOT__reads_n[0U] 
                = (7U & (vlTOPp->dma_observer__DOT__reads_q
                         [0U] - (IData)(1U)));
        }
    }
    vlTOPp->dma_observer__DOT__reads_n[1U] = vlTOPp->dma_observer__DOT__reads_q
        [1U];
    if ((2U == ((2U & ((IData)(vlTOPp->ar_valid_i) 
                       & (IData)(vlTOPp->ar_ready_i))) 
                | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                         >> 1U))))) {
        vlTOPp->dma_observer__DOT__reads_n[1U] = (7U 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [1U]));
    } else {
        if ((1U == ((2U & ((IData)(vlTOPp->ar_valid_i) 
                           & (IData)(vlTOPp->ar_ready_i))) 
                    | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                             >> 1U))))) {
            vlTOPp->dma_observer__DOT__reads_n[1U] 
                = (7U & (vlTOPp->dma_observer__DOT__reads_q
                         [1U] - (IData)(1U)));
        }
    }
    vlTOPp->dma_observer__DOT__reads_n[2U] = vlTOPp->dma_observer__DOT__reads_q
        [2U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       >> 1U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                        >> 2U))))) {
        vlTOPp->dma_observer__DOT__reads_n[2U] = (7U 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [2U]));
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           >> 1U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                            >> 2U))))) {
            vlTOPp->dma_observer__DOT__reads_n[2U] 
                = (7U & (vlTOPp->dma_observer__DOT__reads_q
                         [2U] - (IData)(1U)));
        }
    }
    vlTOPp->dma_observer__DOT__reads_n[3U] = vlTOPp->dma_observer__DOT__reads_q
        [3U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       >> 2U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                        >> 3U))))) {
        vlTOPp->dma_observer__DOT__reads_n[3U] = (7U 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__reads_q
                                                     [3U]));
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           >> 2U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                            >> 3U))))) {
            vlTOPp->dma_observer__DOT__reads_n[3U] 
                = (7U & (vlTOPp->dma_observer__DOT__reads_q
                         [3U] - (IData)(1U)));
        }
    }
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
    vlTOPp->dma_observer__DOT__target_n = vlTOPp->dma_observer__DOT__target_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U == (IData)(vlTOPp->cfg_addr_i))) {
                if (((~ (IData)(vlTOPp->dma_observer__DOT__busy)) 
                     & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q)))) {
                    vlTOPp->dma_observer__DOT__target_n 
                        = vlTOPp->cfg_wdata_i;
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
                                if (VL_GTS_III(1,32,32, 0x23U, (IData)(vlTOPp->dma_observer__DOT__index_q))) {
                                    vlTOPp->cfg_rdata_o 
                                        = (QData)((IData)(
                                                          ((0x22U 
                                                            >= 
                                                            (0x3fU 
                                                             & (IData)(vlTOPp->dma_observer__DOT__index_q)))
                                                            ? 
                                                           vlTOPp->dma_observer__DOT__count_q
                                                           [
                                                           (0x3fU 
                                                            & (IData)(vlTOPp->dma_observer__DOT__index_q))]
                                                            : 0U)));
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
                                       | ((QData)((IData)(vlTOPp->dma_observer__DOT__busy)) 
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
                                vlTOPp->cfg_rdata_o 
                                    = ((0xffffffbfffffffffULL 
                                        & vlTOPp->cfg_rdata_o) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & (vlTOPp->dma_observer__DOT__error_q 
                                                              >> 6U)))) 
                                          << 0x26U));
                                vlTOPp->cfg_rdata_o 
                                    = ((0xffffff7fffffffffULL 
                                        & vlTOPp->cfg_rdata_o) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((vlTOPp->dma_observer__DOT__error_q 
                                                               >> 7U) 
                                                              | ((IData)(vlTOPp->dma_observer__DOT__e3_q) 
                                                                 & (IData)(vlTOPp->e3_error_i)))))) 
                                          << 0x27U));
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
                                vlTOPp->cfg_rdata_o = 0x49444d414f425302ULL;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__e3_n = vlTOPp->dma_observer__DOT__e3_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if ((1U & (~ ((IData)(vlTOPp->dma_observer__DOT__busy) 
                                              | (1U 
                                                 == (IData)(vlTOPp->dma_observer__DOT__phase_q)))))) {
                                    vlTOPp->dma_observer__DOT__e3_n = 0U;
                                }
                            } else {
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
                                    vlTOPp->dma_observer__DOT__e3_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)));
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
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
    vlTOPp->dma_observer__DOT__batch_n = vlTOPp->dma_observer__DOT__batch_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
                                    vlTOPp->dma_observer__DOT__batch_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x19U)));
                                }
                            }
                        }
                    }
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
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
    vlTOPp->dma_observer__DOT__path_clear = 0U;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if ((1U & (~ ((IData)(vlTOPp->dma_observer__DOT__busy) 
                                              | (1U 
                                                 == (IData)(vlTOPp->dma_observer__DOT__phase_q)))))) {
                                    vlTOPp->dma_observer__DOT__path_clear = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__e3_arm = 0U;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
                                    vlTOPp->dma_observer__DOT__e3_arm 
                                        = (1U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_observer__DOT__delay_n = vlTOPp->dma_observer__DOT__delay_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((0xffU == (IData)(vlTOPp->cfg_wstrb_i))) {
            if ((0x68U != (IData)(vlTOPp->cfg_addr_i))) {
                if ((0x70U != (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL != vlTOPp->cfg_wdata_i)) {
                            if ((4ULL != vlTOPp->cfg_wdata_i)) {
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
                                    vlTOPp->dma_observer__DOT__delay_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1cU)));
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
    vlTOPp->dma_observer__DOT____Vlvbound2 = vlTOPp->dma_observer__DOT__count_q
        [0x22U];
    vlTOPp->dma_observer__DOT__count_n[0x22U] = vlTOPp->dma_observer__DOT____Vlvbound2;
    vlTOPp->dma_observer__DOT__segment_n = vlTOPp->dma_observer__DOT__segment_q;
    vlTOPp->dma_observer__DOT__error_n = vlTOPp->dma_observer__DOT__error_q;
    vlTOPp->dma_observer__DOT__track_bad_n = vlTOPp->dma_observer__DOT__track_bad_q;
    vlTOPp->dma_observer__DOT__writes_n[0U] = vlTOPp->dma_observer__DOT__writes_q
        [0U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       << 1U)) | (1U & (IData)(vlTOPp->rlast_fire_i))))) {
        if ((7U == vlTOPp->dma_observer__DOT__reads_q
             [0U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (1U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           << 1U)) | (1U & (IData)(vlTOPp->rlast_fire_i))))) {
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
        vlTOPp->dma_observer__DOT__writes_n[0U] = (7U 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [0U]));
        if ((7U == vlTOPp->dma_observer__DOT__writes_q
             [0U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (1U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->aw_valid_i) 
                            & (IData)(vlTOPp->aw_ready_i)) 
                           << 1U)) | (1U & (IData)(vlTOPp->b_fire_i))))) {
            vlTOPp->dma_observer__DOT__writes_n[0U] 
                = (7U & (vlTOPp->dma_observer__DOT__writes_q
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
    vlTOPp->dma_observer__DOT__writes_n[1U] = vlTOPp->dma_observer__DOT__writes_q
        [1U];
    if ((2U == ((2U & ((IData)(vlTOPp->ar_valid_i) 
                       & (IData)(vlTOPp->ar_ready_i))) 
                | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                         >> 1U))))) {
        if ((7U == vlTOPp->dma_observer__DOT__reads_q
             [1U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (2U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & ((IData)(vlTOPp->ar_valid_i) 
                           & (IData)(vlTOPp->ar_ready_i))) 
                    | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                             >> 1U))))) {
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
        vlTOPp->dma_observer__DOT__writes_n[1U] = (7U 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [1U]));
        if ((7U == vlTOPp->dma_observer__DOT__writes_q
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
                = (7U & (vlTOPp->dma_observer__DOT__writes_q
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
    vlTOPp->dma_observer__DOT__writes_n[2U] = vlTOPp->dma_observer__DOT__writes_q
        [2U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       >> 1U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                        >> 2U))))) {
        if ((7U == vlTOPp->dma_observer__DOT__reads_q
             [2U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (4U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           >> 1U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                            >> 2U))))) {
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
        vlTOPp->dma_observer__DOT__writes_n[2U] = (7U 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [2U]));
        if ((7U == vlTOPp->dma_observer__DOT__writes_q
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
                = (7U & (vlTOPp->dma_observer__DOT__writes_q
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
    vlTOPp->dma_observer__DOT__writes_n[3U] = vlTOPp->dma_observer__DOT__writes_q
        [3U];
    if ((2U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                        & (IData)(vlTOPp->ar_ready_i)) 
                       >> 2U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                        >> 3U))))) {
        if ((7U == vlTOPp->dma_observer__DOT__reads_q
             [3U])) {
            vlTOPp->dma_observer__DOT__track_bad_n 
                = (8U | (IData)(vlTOPp->dma_observer__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (((IData)(vlTOPp->ar_valid_i) 
                            & (IData)(vlTOPp->ar_ready_i)) 
                           >> 2U)) | (1U & ((IData)(vlTOPp->rlast_fire_i) 
                                            >> 3U))))) {
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
        vlTOPp->dma_observer__DOT__writes_n[3U] = (7U 
                                                   & ((IData)(1U) 
                                                      + 
                                                      vlTOPp->dma_observer__DOT__writes_q
                                                      [3U]));
        if ((7U == vlTOPp->dma_observer__DOT__writes_q
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
                = (7U & (vlTOPp->dma_observer__DOT__writes_q
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
        vlTOPp->dma_observer__DOT__count_n[7U] = 1U;
        vlTOPp->dma_observer__DOT__segment_n = 0U;
        if ((0ULL == vlTOPp->issue_id_i[vlTOPp->dma_observer__DOT__victim_q])) {
            vlTOPp->dma_observer__DOT__error_n = (2U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->dma_observer__DOT__count_n[0x1eU] = 
            vlTOPp->dma_observer__DOT__writes_n[vlTOPp->dma_observer__DOT__victim_q];
        vlTOPp->dma_observer__DOT__count_n[0x1fU] = 
            ((IData)(vlTOPp->dma_observer__DOT__has_a_q)
              ? vlTOPp->dma_observer__DOT__writes_n
             [vlTOPp->dma_observer__DOT__aggressor_q]
              : 0U);
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
    if (((((IData)(vlTOPp->dma_observer__DOT__batch_q) 
           & ((IData)(vlTOPp->dma_observer__DOT__active) 
              | ((1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)) 
                 & ((IData)(vlTOPp->issue_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q))))) 
          & (IData)(vlTOPp->dma_observer__DOT__has_a_q)) 
         & (~ ((IData)(vlTOPp->batch_active_i) >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q))))) {
        vlTOPp->dma_observer__DOT__error_n = (0x20U 
                                              | vlTOPp->dma_observer__DOT__error_n);
    }
    if ((6U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
        vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot = 0x22U;
        vlTOPp->dma_observer__DOT____Vlvbound1 = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__count_q
                                                     [0x22U]));
        vlTOPp->dma_observer__DOT__count_n[(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot)] 
            = vlTOPp->dma_observer__DOT____Vlvbound1;
        if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot))
                        ? vlTOPp->dma_observer__DOT__count_q
                       [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__0__slot)]
                        : 0U))) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        if ((1U & (((IData)(vlTOPp->issue_i) | (IData)(vlTOPp->done_i)) 
                   >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))) {
            vlTOPp->dma_observer__DOT__error_n = (2U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        if ((((IData)(vlTOPp->poll_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
             & (vlTOPp->poll_id_i[vlTOPp->dma_observer__DOT__victim_q] 
                == vlTOPp->dma_observer__DOT__expected_q))) {
            vlTOPp->dma_observer__DOT__count_n[9U] = 1U;
            vlTOPp->dma_observer__DOT__phase_n = 5U;
        }
    }
    if (vlTOPp->dma_observer__DOT__active) {
        vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot = 0U;
        vlTOPp->dma_observer__DOT____Vlvbound1 = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     vlTOPp->dma_observer__DOT__count_q
                                                     [0U]));
        vlTOPp->dma_observer__DOT__count_n[(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot)] 
            = vlTOPp->dma_observer__DOT____Vlvbound1;
        if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot))
                        ? vlTOPp->dma_observer__DOT__count_q
                       [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__1__slot)]
                        : 0U))) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->dma_observer__DOT__segment_n = (0xffU 
                                                & ((IData)(1U) 
                                                   + (IData)(vlTOPp->dma_observer__DOT__segment_q)));
        if ((0xffU == (IData)(vlTOPp->dma_observer__DOT__segment_q))) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot 
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
        vlTOPp->dma_observer__DOT____Vlvbound1 = (0xffU 
                                                  & ((IData)(1U) 
                                                     + 
                                                     ((0x22U 
                                                       >= 
                                                       (0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))
                                                       ? 
                                                      vlTOPp->dma_observer__DOT__count_q
                                                      [
                                                      (0x3fU 
                                                       & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)]
                                                       : 0U)));
        if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))) {
            vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)] 
                = vlTOPp->dma_observer__DOT____Vlvbound1;
        }
        if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot))
                        ? vlTOPp->dma_observer__DOT__count_q
                       [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__2__slot)]
                        : 0U))) {
            vlTOPp->dma_observer__DOT__error_n = (4U 
                                                  | vlTOPp->dma_observer__DOT__error_n);
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable 
            = (1U & (((IData)(vlTOPp->ar_valid_i) & (IData)(vlTOPp->ar_ready_i)) 
                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot = 0xeU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable 
            = (1U & (((IData)(vlTOPp->aw_valid_i) & (IData)(vlTOPp->aw_ready_i)) 
                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot = 0xfU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable 
            = (1U & ((IData)(vlTOPp->rlast_fire_i) 
                     >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot = 0x10U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable 
            = (1U & ((IData)(vlTOPp->b_fire_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot = 0x11U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable 
            = (1U & (((IData)(vlTOPp->ar_valid_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
                     & (~ ((IData)(vlTOPp->ar_ready_i) 
                           >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))));
        vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot = 0x12U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__8__enable 
            = (1U & (((IData)(vlTOPp->aw_valid_i) >> (IData)(vlTOPp->dma_observer__DOT__victim_q)) 
                     & (~ ((IData)(vlTOPp->aw_ready_i) 
                           >> (IData)(vlTOPp->dma_observer__DOT__victim_q)))));
        vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot = 0x13U;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__8__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        if (vlTOPp->dma_observer__DOT__has_a_q) {
            vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable 
                = (1U & (((IData)(vlTOPp->ar_valid_i) 
                          & (IData)(vlTOPp->ar_ready_i)) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot = 0x14U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__3__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                                                : 0U)));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__3__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable 
                = (1U & (((IData)(vlTOPp->aw_valid_i) 
                          & (IData)(vlTOPp->aw_ready_i)) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot = 0x15U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__4__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                                                : 0U)));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__4__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable 
                = (1U & ((IData)(vlTOPp->rlast_fire_i) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot = 0x16U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__5__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                                                : 0U)));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__5__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable 
                = (1U & ((IData)(vlTOPp->b_fire_i) 
                         >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)));
            vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot = 0x17U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__6__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                                                : 0U)));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__6__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable 
                = (1U & (((IData)(vlTOPp->ar_valid_i) 
                          >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)) 
                         & (~ ((IData)(vlTOPp->ar_ready_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)))));
            vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot = 0x18U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__7__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                                                : 0U)));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__7__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_dma_observer__DOT__bump__8__enable 
                = (1U & (((IData)(vlTOPp->aw_valid_i) 
                          >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)) 
                         & (~ ((IData)(vlTOPp->aw_ready_i) 
                               >> (IData)(vlTOPp->dma_observer__DOT__aggressor_q)))));
            vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot = 0x19U;
            if (vlTOPp->__Vtask_dma_observer__DOT__bump__8__enable) {
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))
                                                ? vlTOPp->dma_observer__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)]
                                                : 0U)));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))) {
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                }
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__8__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__9__enable 
            = ((IData)(vlTOPp->shared_ar_valid_i) & (IData)(vlTOPp->shared_ar_ready_i));
        vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot = 0x1aU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__9__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__9__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__10__enable 
            = ((IData)(vlTOPp->shared_aw_valid_i) & (IData)(vlTOPp->shared_aw_ready_i));
        vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot = 0x1bU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__10__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__10__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__11__enable 
            = ((IData)(vlTOPp->shared_ar_valid_i) & 
               (~ (IData)(vlTOPp->shared_ar_ready_i)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot = 0x1cU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__11__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__11__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_dma_observer__DOT__bump__12__enable 
            = ((IData)(vlTOPp->shared_aw_valid_i) & 
               (~ (IData)(vlTOPp->shared_aw_ready_i)));
        vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot = 0x1dU;
        if (vlTOPp->__Vtask_dma_observer__DOT__bump__12__enable) {
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot))
                                            ? vlTOPp->dma_observer__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot)]
                                            : 0U)));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot))) {
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
            }
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__12__slot)]
                            : 0U))) {
                vlTOPp->dma_observer__DOT__error_n 
                    = (4U | vlTOPp->dma_observer__DOT__error_n);
            }
        }
        if ((2U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
            vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot = 1U;
            vlTOPp->dma_observer__DOT____Vlvbound1 
                = (0xffU & ((IData)(1U) + vlTOPp->dma_observer__DOT__count_q
                            [1U]));
            vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                & vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot)] 
                = vlTOPp->dma_observer__DOT____Vlvbound1;
            if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot))
                            ? vlTOPp->dma_observer__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__13__slot)]
                            : 0U))) {
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
                vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot = 8U;
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + vlTOPp->dma_observer__DOT__count_q
                                [8U]));
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__14__slot)]
                                : 0U))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (4U | vlTOPp->dma_observer__DOT__error_n);
                }
                if (((IData)(vlTOPp->dma_observer__DOT__segment_n) 
                     > vlTOPp->dma_observer__DOT__count_q
                     [4U])) {
                    vlTOPp->dma_observer__DOT__count_n[4U] 
                        = vlTOPp->dma_observer__DOT__segment_n;
                }
                vlTOPp->dma_observer__DOT__segment_n = 0U;
                if (vlTOPp->dma_observer__DOT__batch_q) {
                    if (((QData)((IData)(vlTOPp->dma_observer__DOT__count_n
                                         [8U])) == vlTOPp->dma_observer__DOT__target_q)) {
                        vlTOPp->dma_observer__DOT__phase_n = 6U;
                        vlTOPp->dma_observer__DOT__count_n[0x20U] 
                            = vlTOPp->dma_observer__DOT__writes_n
                            [vlTOPp->dma_observer__DOT__victim_q];
                        vlTOPp->dma_observer__DOT__count_n[0x21U] 
                            = ((IData)(vlTOPp->dma_observer__DOT__has_a_q)
                                ? vlTOPp->dma_observer__DOT__writes_n
                               [vlTOPp->dma_observer__DOT__aggressor_q]
                                : 0U);
                    } else {
                        vlTOPp->dma_observer__DOT__phase_n = 4U;
                    }
                } else {
                    vlTOPp->dma_observer__DOT__phase_n = 3U;
                }
            }
        } else {
            if ((3U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
                vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot = 2U;
                vlTOPp->dma_observer__DOT____Vlvbound1 
                    = (0xffU & ((IData)(1U) + vlTOPp->dma_observer__DOT__count_q
                                [2U]));
                vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                    & vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot)] 
                    = vlTOPp->dma_observer__DOT____Vlvbound1;
                if ((0xffU == ((0x22U >= (0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot))
                                ? vlTOPp->dma_observer__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__15__slot)]
                                : 0U))) {
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
                    vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot = 9U;
                    vlTOPp->dma_observer__DOT____Vlvbound1 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_observer__DOT__count_q
                                    [9U]));
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                    if ((0xffU == ((0x22U >= (0x3fU 
                                              & vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot))
                                    ? vlTOPp->dma_observer__DOT__count_q
                                   [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__16__slot)]
                                    : 0U))) {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (4U | vlTOPp->dma_observer__DOT__error_n);
                    }
                    if (((IData)(vlTOPp->dma_observer__DOT__segment_n) 
                         > vlTOPp->dma_observer__DOT__count_q
                         [5U])) {
                        vlTOPp->dma_observer__DOT__count_n[5U] 
                            = vlTOPp->dma_observer__DOT__segment_n;
                    }
                    vlTOPp->dma_observer__DOT__segment_n = 0U;
                    if (((QData)((IData)(vlTOPp->dma_observer__DOT__count_n
                                         [9U])) == vlTOPp->dma_observer__DOT__target_q)) {
                        vlTOPp->dma_observer__DOT__phase_n = 5U;
                        vlTOPp->dma_observer__DOT__count_n[0x20U] 
                            = vlTOPp->dma_observer__DOT__writes_n
                            [vlTOPp->dma_observer__DOT__victim_q];
                        vlTOPp->dma_observer__DOT__count_n[0x21U] 
                            = ((IData)(vlTOPp->dma_observer__DOT__has_a_q)
                                ? vlTOPp->dma_observer__DOT__writes_n
                               [vlTOPp->dma_observer__DOT__aggressor_q]
                                : 0U);
                    } else {
                        vlTOPp->dma_observer__DOT__phase_n = 4U;
                    }
                }
            } else {
                if ((4U == (IData)(vlTOPp->dma_observer__DOT__phase_q))) {
                    vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot = 3U;
                    vlTOPp->dma_observer__DOT____Vlvbound1 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_observer__DOT__count_q
                                    [3U]));
                    vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                        & vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot)] 
                        = vlTOPp->dma_observer__DOT____Vlvbound1;
                    if ((0xffU == ((0x22U >= (0x3fU 
                                              & vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot))
                                    ? vlTOPp->dma_observer__DOT__count_q
                                   [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__17__slot)]
                                    : 0U))) {
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
                        vlTOPp->__Vtask_dma_observer__DOT__bump__18__slot = 7U;
                        vlTOPp->dma_observer__DOT____Vlvbound1 
                            = (0xffU & ((IData)(1U) 
                                        + vlTOPp->dma_observer__DOT__count_q
                                        [7U]));
                        vlTOPp->dma_observer__DOT__count_n[(0x3fU 
                                                            & vlTOPp->__Vtask_dma_observer__DOT__bump__18__slot)] 
                            = vlTOPp->dma_observer__DOT____Vlvbound1;
                        if ((0xffU == ((0x22U >= (0x3fU 
                                                  & vlTOPp->__Vtask_dma_observer__DOT__bump__18__slot))
                                        ? vlTOPp->dma_observer__DOT__count_q
                                       [(0x3fU & vlTOPp->__Vtask_dma_observer__DOT__bump__18__slot)]
                                        : 0U))) {
                            vlTOPp->dma_observer__DOT__error_n 
                                = (4U | vlTOPp->dma_observer__DOT__error_n);
                        }
                        if (((IData)(vlTOPp->dma_observer__DOT__segment_n) 
                             > vlTOPp->dma_observer__DOT__count_q
                             [6U])) {
                            vlTOPp->dma_observer__DOT__count_n[6U] 
                                = vlTOPp->dma_observer__DOT__segment_n;
                        }
                        vlTOPp->dma_observer__DOT__segment_n = 0U;
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
                if ((1U & (~ ((~ (IData)(vlTOPp->dma_observer__DOT__busy)) 
                              & (1U != (IData)(vlTOPp->dma_observer__DOT__phase_q)))))) {
                    vlTOPp->dma_observer__DOT__error_n 
                        = (1U | vlTOPp->dma_observer__DOT__error_n);
                }
            } else {
                if ((0x70U == (IData)(vlTOPp->cfg_addr_i))) {
                    if ((0x23ULL <= vlTOPp->cfg_wdata_i)) {
                        vlTOPp->dma_observer__DOT__error_n 
                            = (1U | vlTOPp->dma_observer__DOT__error_n);
                    }
                } else {
                    if ((0x60U == (IData)(vlTOPp->cfg_addr_i))) {
                        if ((2ULL == vlTOPp->cfg_wdata_i)) {
                            if (((IData)(vlTOPp->dma_observer__DOT__busy) 
                                 | (1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)))) {
                                vlTOPp->dma_observer__DOT__phase_n = 5U;
                                vlTOPp->dma_observer__DOT__error_n 
                                    = (0x10U | vlTOPp->dma_observer__DOT__error_n);
                            }
                        } else {
                            if ((4ULL == vlTOPp->cfg_wdata_i)) {
                                if (((IData)(vlTOPp->dma_observer__DOT__busy) 
                                     | (1U == (IData)(vlTOPp->dma_observer__DOT__phase_q)))) {
                                    vlTOPp->dma_observer__DOT__error_n 
                                        = (1U | vlTOPp->dma_observer__DOT__error_n);
                                } else {
                                    vlTOPp->dma_observer__DOT__phase_n = 0U;
                                    vlTOPp->dma_observer__DOT__error_n = 0U;
                                }
                            } else {
                                if (((((((((((0ULL 
                                              == (0xffffffffe0fcfcfeULL 
                                                  & vlTOPp->cfg_wdata_i)) 
                                             & (IData)(vlTOPp->cfg_wdata_i)) 
                                            & ((~ (IData)(
                                                          (vlTOPp->cfg_wdata_i 
                                                           >> 0x1cU))) 
                                               | (IData)(
                                                         (vlTOPp->cfg_wdata_i 
                                                          >> 0x1bU)))) 
                                           & (~ (IData)(
                                                        (vlTOPp->cfg_wdata_i 
                                                         >> 0x1bU)))) 
                                          & (~ (IData)(
                                                       (vlTOPp->cfg_wdata_i 
                                                        >> 0x1aU)))) 
                                         & (~ (IData)(vlTOPp->dma_observer__DOT__busy))) 
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
                                    vlTOPp->dma_observer__DOT__segment_n = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[1U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[2U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[3U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[4U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[5U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[6U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[7U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[8U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[9U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0xaU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0xbU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0xcU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0xdU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0xeU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0xfU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x10U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x11U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x12U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x13U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x14U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x15U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x16U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x17U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x18U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x19U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x1aU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x1bU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x1cU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x1dU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x1eU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x1fU] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x20U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x21U] = 0U;
                                    vlTOPp->dma_observer__DOT__count_n[0x22U] = 0U;
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
    vlTOPp->e3_control_o = (((IData)(vlTOPp->dma_observer__DOT__delay_n) 
                             << 7U) | (((((IData)(vlTOPp->dma_observer__DOT__e3_q) 
                                          & (5U == (IData)(vlTOPp->dma_observer__DOT__phase_n))) 
                                         & (5U != (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                        << 6U) | ((
                                                   (((IData)(vlTOPp->dma_observer__DOT__e3_q) 
                                                     & (IData)(vlTOPp->dma_observer__DOT__active)) 
                                                    & (6U 
                                                       == (IData)(vlTOPp->dma_observer__DOT__phase_n))) 
                                                   << 5U) 
                                                  | (((((IData)(vlTOPp->dma_observer__DOT__e3_q) 
                                                        & (1U 
                                                           == (IData)(vlTOPp->dma_observer__DOT__phase_q))) 
                                                       & ((IData)(vlTOPp->issue_i) 
                                                          >> (IData)(vlTOPp->dma_observer__DOT__victim_q))) 
                                                      << 4U) 
                                                     | ((((IData)(vlTOPp->dma_observer__DOT__e3_q) 
                                                          & (IData)(vlTOPp->dma_observer__DOT__active)) 
                                                         << 3U) 
                                                        | (((IData)(vlTOPp->dma_observer__DOT__path_clear) 
                                                            << 2U) 
                                                           | (((IData)(vlTOPp->dma_observer__DOT__e3_arm) 
                                                               << 1U) 
                                                              | ((IData)(vlTOPp->dma_observer__DOT__e3_q) 
                                                                 | (IData)(vlTOPp->dma_observer__DOT__e3_arm)))))))));
}

void Vdma_observer::_eval(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_eval\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

VL_INLINE_OPT QData Vdma_observer::_change_request(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_change_request\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_observer::_change_request_1(Vdma_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_change_request_1\n"); );
    Vdma_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_observer::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_observer::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((cfg_valid_i & 0xfeU))) {
        Verilated::overWidthError("cfg_valid_i");}
    if (VL_UNLIKELY((cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("cfg_write_i");}
    if (VL_UNLIKELY((cfg_addr_i & 0x80U))) {
        Verilated::overWidthError("cfg_addr_i");}
    if (VL_UNLIKELY((issue_i & 0xf0U))) {
        Verilated::overWidthError("issue_i");}
    if (VL_UNLIKELY((done_i & 0xf0U))) {
        Verilated::overWidthError("done_i");}
    if (VL_UNLIKELY((poll_i & 0xf0U))) {
        Verilated::overWidthError("poll_i");}
    if (VL_UNLIKELY((batch_active_i & 0xf0U))) {
        Verilated::overWidthError("batch_active_i");}
    if (VL_UNLIKELY((ar_valid_i & 0xf0U))) {
        Verilated::overWidthError("ar_valid_i");}
    if (VL_UNLIKELY((aw_valid_i & 0xf0U))) {
        Verilated::overWidthError("aw_valid_i");}
    if (VL_UNLIKELY((ar_ready_i & 0xf0U))) {
        Verilated::overWidthError("ar_ready_i");}
    if (VL_UNLIKELY((aw_ready_i & 0xf0U))) {
        Verilated::overWidthError("aw_ready_i");}
    if (VL_UNLIKELY((rlast_fire_i & 0xf0U))) {
        Verilated::overWidthError("rlast_fire_i");}
    if (VL_UNLIKELY((b_fire_i & 0xf0U))) {
        Verilated::overWidthError("b_fire_i");}
    if (VL_UNLIKELY((bad_id_i & 0xf0U))) {
        Verilated::overWidthError("bad_id_i");}
    if (VL_UNLIKELY((shared_ar_valid_i & 0xfeU))) {
        Verilated::overWidthError("shared_ar_valid_i");}
    if (VL_UNLIKELY((shared_aw_valid_i & 0xfeU))) {
        Verilated::overWidthError("shared_aw_valid_i");}
    if (VL_UNLIKELY((shared_ar_ready_i & 0xfeU))) {
        Verilated::overWidthError("shared_ar_ready_i");}
    if (VL_UNLIKELY((shared_aw_ready_i & 0xfeU))) {
        Verilated::overWidthError("shared_aw_ready_i");}
    if (VL_UNLIKELY((out_valid_i & 0xfcU))) {
        Verilated::overWidthError("out_valid_i");}
    if (VL_UNLIKELY((out_ready_i & 0xfcU))) {
        Verilated::overWidthError("out_ready_i");}
    if (VL_UNLIKELY((return_i & 0xfcU))) {
        Verilated::overWidthError("return_i");}
    if (VL_UNLIKELY((e3_error_i & 0xfeU))) {
        Verilated::overWidthError("e3_error_i");}
}
#endif  // VL_DEBUG
