// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_composition_control.h for the primary calling header

#include "Vdma_composition_control.h"
#include "Vdma_composition_control__Syms.h"

//==========

void Vdma_composition_control::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_composition_control::eval\n"); );
    Vdma_composition_control__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/dma_composition_control.sv", 28, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_composition_control::_eval_initial_loop(Vdma_composition_control__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/dma_composition_control.sv", 28, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_composition_control::_combo__TOP__1(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_combo__TOP__1\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_composition_control__DOT__repl_idx = 0U;
    if ((1U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0U;
    }
    if ((2U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 1U;
    }
    if ((4U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 2U;
    }
    if ((8U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 3U;
    }
    if ((0x10U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 4U;
    }
    if ((0x20U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 5U;
    }
    if ((0x40U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 6U;
    }
    if ((0x80U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 7U;
    }
    if ((0x100U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 8U;
    }
    if ((0x200U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 9U;
    }
    if ((0x400U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xaU;
    }
    if ((0x800U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xbU;
    }
    if ((0x1000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xcU;
    }
    if ((0x2000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xdU;
    }
    if ((0x4000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xeU;
    }
    if ((0x8000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xfU;
    }
    vlTOPp->dma_composition_control__DOT__lookup_ok 
        = (((IData)(vlTOPp->normal_i) & (IData)(vlTOPp->stage1_i)) 
           & (~ (IData)(vlTOPp->stage2_i)));
}

VL_INLINE_OPT void Vdma_composition_control::_sequent__TOP__3(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_sequent__TOP__3\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v7;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v9;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v10;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v11;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v12;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v13;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v14;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v15;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v17;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v18;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v19;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v20;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v21;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v22;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v23;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v24;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v25;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v26;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v27;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v28;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v29;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v30;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v31;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v32;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v33;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v34;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v35;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v36;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v37;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v38;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v39;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__cnt_q__v40;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__entries_open_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__entries_open_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__entries_close_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_composition_control__DOT__entries_close_q__v16;
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[4];
    WData/*127:0*/ __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[4];
    IData/*31:0*/ __Vilp;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v6;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v7;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v8;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v9;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v10;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v11;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v12;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v13;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v14;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v15;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v16;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v17;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v18;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v19;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v20;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v21;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v22;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v23;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v24;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v25;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v26;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v27;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v28;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v29;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v30;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v31;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v32;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v33;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v34;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v35;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v36;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v37;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v38;
    QData/*63:0*/ __Vdlyvval__dma_composition_control__DOT__cnt_q__v39;
    // Body
    __Vdlyvset__dma_composition_control__DOT__entries_close_q__v0 = 0U;
    __Vdlyvset__dma_composition_control__DOT__entries_close_q__v16 = 0U;
    __Vdlyvset__dma_composition_control__DOT__entries_open_q__v0 = 0U;
    __Vdlyvset__dma_composition_control__DOT__entries_open_q__v16 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v0 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v1 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v2 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v3 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v4 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v5 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v6 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v7 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v8 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v9 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v10 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v11 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v12 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v13 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v14 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v15 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v16 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v17 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v18 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v19 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v20 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v21 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v22 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v23 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v24 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v25 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v26 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v27 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v28 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v29 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v30 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v31 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v32 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v33 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v34 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v35 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v36 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v37 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v38 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v39 = 0U;
    __Vdlyvset__dma_composition_control__DOT__cnt_q__v40 = 0U;
    vlTOPp->dma_composition_control__DOT__seen_lookup_a_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_composition_control__DOT__seen_lookup_a_d));
    vlTOPp->dma_composition_control__DOT__seen_lookup_v_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_composition_control__DOT__seen_lookup_v_d));
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_composition_control__DOT__win_gen_q 
            = vlTOPp->dma_composition_control__DOT__win_gen_d;
        vlTOPp->dma_composition_control__DOT__plru_open_q 
            = vlTOPp->dma_composition_control__DOT__plru_open_d;
        vlTOPp->dma_composition_control__DOT__plru_close_q 
            = vlTOPp->dma_composition_control__DOT__plru_close_d;
        vlTOPp->dma_composition_control__DOT__win_cfg_q 
            = vlTOPp->dma_composition_control__DOT__win_cfg_d;
        vlTOPp->dma_composition_control__DOT__pscid_v_q 
            = vlTOPp->dma_composition_control__DOT__pscid_v_d;
        vlTOPp->dma_composition_control__DOT__pscid_a_q 
            = vlTOPp->dma_composition_control__DOT__pscid_a_d;
        vlTOPp->dma_composition_control__DOT__seq_q 
            = vlTOPp->dma_composition_control__DOT__seq_d;
        vlTOPp->dma_composition_control__DOT__cycles_q 
            = vlTOPp->dma_composition_control__DOT__cycles_d;
        vlTOPp->dma_composition_control__DOT__err_q 
            = vlTOPp->dma_composition_control__DOT__err_d;
    } else {
        vlTOPp->dma_composition_control__DOT__win_gen_q = 0ULL;
        vlTOPp->dma_composition_control__DOT__plru_open_q = 0U;
        vlTOPp->dma_composition_control__DOT__plru_close_q = 0U;
        vlTOPp->dma_composition_control__DOT__win_cfg_q = 0ULL;
        vlTOPp->dma_composition_control__DOT__pscid_v_q = 0U;
        vlTOPp->dma_composition_control__DOT__pscid_a_q = 0U;
        vlTOPp->dma_composition_control__DOT__seq_q = 0ULL;
        vlTOPp->dma_composition_control__DOT__cycles_q = 0ULL;
        vlTOPp->dma_composition_control__DOT__err_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->cfg_write_i) & (2U == (IData)(vlTOPp->index_i)))) {
            if (vlTOPp->dma_composition_control__DOT__cfg_busy) {
                vlTOPp->dma_composition_control__DOT__last_status_q = 1U;
            } else {
                if ((1U & ((~ (IData)(vlTOPp->supported_i)) 
                           | (~ (IData)(vlTOPp->dma_composition_control__DOT__cfg_data_legal))))) {
                    vlTOPp->dma_composition_control__DOT__last_status_q = 2U;
                } else {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__generation_q)) {
                        vlTOPp->dma_composition_control__DOT__last_status_q = 3U;
                    } else {
                        vlTOPp->dma_composition_control__DOT__generation_q 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__generation_q);
                        vlTOPp->dma_composition_control__DOT__last_status_q = 0U;
                        vlTOPp->dma_composition_control__DOT__mode_q 
                            = (3U & (IData)(vlTOPp->wdata_i));
                        vlTOPp->dma_composition_control__DOT__protected_did_q 
                            = (0xffffffU & (IData)(
                                                   (vlTOPp->wdata_i 
                                                    >> 8U)));
                    }
                }
            }
        }
    } else {
        vlTOPp->dma_composition_control__DOT__generation_q = 0ULL;
        vlTOPp->dma_composition_control__DOT__mode_q = 0U;
        vlTOPp->dma_composition_control__DOT__protected_did_q = 0xaU;
        vlTOPp->dma_composition_control__DOT__last_status_q = 0U;
    }
    vlTOPp->dma_composition_control__DOT__state_q = 
        ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->dma_composition_control__DOT__state_d)
          : 0U);
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0U][3U];
        __Vdlyvset__dma_composition_control__DOT__entries_close_q__v0 = 1U;
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [1U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [1U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [1U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [1U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [2U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [2U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [2U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [2U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [3U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [3U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [3U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [3U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [4U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [4U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [4U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [4U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [5U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [5U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [5U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [5U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [6U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [6U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [6U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [6U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [7U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [7U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [7U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [7U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [8U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [8U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [8U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [8U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [9U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [9U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [9U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [9U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xaU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xaU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xaU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xaU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xbU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xbU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xbU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xbU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xcU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xcU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xcU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xcU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xdU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xdU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xdU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xdU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xeU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xeU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xeU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xeU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xfU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xfU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xfU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_close_d
            [0xfU][3U];
    } else {
        __Vdlyvset__dma_composition_control__DOT__entries_close_q__v16 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0U][3U];
        __Vdlyvset__dma_composition_control__DOT__entries_open_q__v0 = 1U;
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [1U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [1U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [1U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [1U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [2U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [2U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [2U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [2U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [3U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [3U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [3U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [3U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [4U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [4U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [4U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [4U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [5U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [5U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [5U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [5U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [6U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [6U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [6U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [6U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [7U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [7U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [7U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [7U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [8U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [8U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [8U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [8U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [9U][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [9U][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [9U][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [9U][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xaU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xaU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xaU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xaU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xbU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xbU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xbU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xbU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xcU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xcU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xcU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xcU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xdU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xdU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xdU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xdU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xeU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xeU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xeU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xeU][3U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[0U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xfU][0U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[1U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xfU][1U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[2U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xfU][2U];
        __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[3U] 
            = vlTOPp->dma_composition_control__DOT__entries_open_d
            [0xfU][3U];
    } else {
        __Vdlyvset__dma_composition_control__DOT__entries_open_q__v16 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v0 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v0 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [1U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v1 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v1 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [2U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v2 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v2 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [3U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v3 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v3 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [4U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v4 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v4 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [5U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v5 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v5 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [6U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v6 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v6 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [7U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v7 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v7 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [8U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v8 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v8 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [9U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v9 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v9 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0xaU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v10 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v10 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0xbU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v11 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v11 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0xcU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v12 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v12 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0xdU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v13 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v13 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0xeU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v14 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v14 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0xfU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v15 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v15 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x10U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v16 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v16 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x11U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v17 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v17 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x12U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v18 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v18 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x13U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v19 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v19 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x14U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v20 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v20 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x15U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v21 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v21 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x16U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v22 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v22 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x17U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v23 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v23 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x18U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v24 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v24 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x19U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v25 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v25 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x1aU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v26 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v26 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x1bU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v27 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v27 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x1cU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v28 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v28 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x1dU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v29 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v29 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x1eU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v30 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v30 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x1fU];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v31 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v31 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x20U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v32 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v32 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x21U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v33 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v33 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x22U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v34 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v34 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x23U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v35 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v35 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x24U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v36 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v36 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x25U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v37 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v37 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x26U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v38 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v38 = 1U;
        vlTOPp->dma_composition_control__DOT____Vlvbound3 
            = vlTOPp->dma_composition_control__DOT__cnt_d
            [0x27U];
        __Vdlyvval__dma_composition_control__DOT__cnt_q__v39 
            = vlTOPp->dma_composition_control__DOT____Vlvbound3;
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v39 = 1U;
    } else {
        __Vdlyvset__dma_composition_control__DOT__cnt_q__v40 = 1U;
    }
    if (__Vdlyvset__dma_composition_control__DOT__entries_close_q__v0) {
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v0[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v1[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v2[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v3[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v4[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v5[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v6[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v7[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v8[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v9[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v10[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v11[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v12[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v13[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v14[3U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[0U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[1U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[2U];
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_close_q__v15[3U];
    }
    if (__Vdlyvset__dma_composition_control__DOT__entries_close_q__v16) {
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[1U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[2U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[3U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[4U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[5U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[6U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[7U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[8U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[9U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xaU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xbU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xcU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xdU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xeU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_close_q[0xfU][3U] = 0U;
    }
    if (__Vdlyvset__dma_composition_control__DOT__entries_open_q__v0) {
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v0[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v1[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v2[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v3[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v4[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v5[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v6[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v7[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v8[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v9[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v10[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v11[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v12[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v13[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v14[3U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][0U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[0U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][1U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[1U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][2U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[2U];
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][3U] 
            = __Vdlyvval__dma_composition_control__DOT__entries_open_q__v15[3U];
    }
    if (__Vdlyvset__dma_composition_control__DOT__entries_open_q__v16) {
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[1U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[2U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[3U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[4U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[5U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[6U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[7U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[8U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[9U][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xaU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xbU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xcU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xdU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xeU][3U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][0U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][1U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][2U] = 0U;
        vlTOPp->dma_composition_control__DOT__entries_open_q[0xfU][3U] = 0U;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v0) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v0;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v1) {
        vlTOPp->dma_composition_control__DOT__cnt_q[1U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v1;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v2) {
        vlTOPp->dma_composition_control__DOT__cnt_q[2U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v2;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v3) {
        vlTOPp->dma_composition_control__DOT__cnt_q[3U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v3;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v4) {
        vlTOPp->dma_composition_control__DOT__cnt_q[4U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v4;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v5) {
        vlTOPp->dma_composition_control__DOT__cnt_q[5U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v5;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v6) {
        vlTOPp->dma_composition_control__DOT__cnt_q[6U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v6;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v7) {
        vlTOPp->dma_composition_control__DOT__cnt_q[7U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v7;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v8) {
        vlTOPp->dma_composition_control__DOT__cnt_q[8U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v8;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v9) {
        vlTOPp->dma_composition_control__DOT__cnt_q[9U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v9;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v10) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0xaU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v10;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v11) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0xbU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v11;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v12) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0xcU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v12;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v13) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0xdU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v13;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v14) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0xeU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v14;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v15) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0xfU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v15;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v16) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x10U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v16;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v17) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x11U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v17;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v18) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x12U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v18;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v19) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x13U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v19;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v20) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x14U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v20;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v21) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x15U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v21;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v22) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x16U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v22;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v23) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x17U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v23;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v24) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x18U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v24;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v25) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x19U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v25;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v26) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x1aU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v26;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v27) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x1bU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v27;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v28) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x1cU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v28;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v29) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x1dU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v29;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v30) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x1eU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v30;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v31) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x1fU] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v31;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v32) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x20U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v32;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v33) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x21U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v33;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v34) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x22U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v34;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v35) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x23U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v35;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v36) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x24U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v36;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v37) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x25U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v37;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v38) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x26U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v38;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v39) {
        vlTOPp->dma_composition_control__DOT__cnt_q[0x27U] 
            = __Vdlyvval__dma_composition_control__DOT__cnt_q__v39;
    }
    if (__Vdlyvset__dma_composition_control__DOT__cnt_q__v40) {
        __Vilp = 0U;
        while ((__Vilp <= 0x27U)) {
            vlTOPp->dma_composition_control__DOT__cnt_q[__Vilp] = 0ULL;
            __Vilp = ((IData)(1U) + __Vilp);
        }
    }
    vlTOPp->mode_o = vlTOPp->dma_composition_control__DOT__mode_q;
    vlTOPp->protected_did_o = vlTOPp->dma_composition_control__DOT__protected_did_q;
}

VL_INLINE_OPT void Vdma_composition_control::_combo__TOP__4(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_combo__TOP__4\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_composition_control__DOT__cfg_data_legal 
        = (((0U == (IData)((vlTOPp->wdata_i >> 0x20U))) 
            & (0U == (0x3fU & (IData)((vlTOPp->wdata_i 
                                       >> 2U))))) & 
           ((0xaU == (0xffffffU & (IData)((vlTOPp->wdata_i 
                                           >> 8U)))) 
            | (0xbU == (0xffffffU & (IData)((vlTOPp->wdata_i 
                                             >> 8U))))));
    vlTOPp->dma_composition_control__DOT__state_d = vlTOPp->dma_composition_control__DOT__state_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__state_d = 1U;
            }
        }
    }
    if (vlTOPp->open_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__state_d = 2U;
            }
        }
    }
    if (vlTOPp->close_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__state_d = 3U;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__seq_d = vlTOPp->dma_composition_control__DOT__seq_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                if ((0xffffffffffffffffULL != vlTOPp->dma_composition_control__DOT__seq_q)) {
                    vlTOPp->dma_composition_control__DOT__seq_d 
                        = (1ULL + vlTOPp->dma_composition_control__DOT__seq_q);
                }
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__plru_close_d 
        = vlTOPp->dma_composition_control__DOT__plru_close_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_close_d = 0U;
            }
        }
    }
    if (vlTOPp->close_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_close_d 
                    = vlTOPp->plru_i;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__win_gen_d 
        = vlTOPp->dma_composition_control__DOT__win_gen_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__win_gen_d 
                    = vlTOPp->dma_composition_control__DOT__generation_q;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__plru_open_d 
        = vlTOPp->dma_composition_control__DOT__plru_open_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_open_d = 0U;
            }
        }
    }
    if (vlTOPp->open_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_open_d 
                    = vlTOPp->plru_i;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__win_cfg_d 
        = vlTOPp->dma_composition_control__DOT__win_cfg_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__win_cfg_d 
                    = (((QData)((IData)(vlTOPp->dma_composition_control__DOT__protected_did_q)) 
                        << 8U) | (QData)((IData)(vlTOPp->dma_composition_control__DOT__mode_q)));
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__pscid_v_d 
        = vlTOPp->dma_composition_control__DOT__pscid_v_q;
    vlTOPp->dma_composition_control__DOT__seen_lookup_v_d 
        = vlTOPp->dma_composition_control__DOT__seen_lookup_v_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__seen_lookup_v_d = 0U;
                vlTOPp->dma_composition_control__DOT__pscid_v_d = 0U;
            }
        }
    }
    if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
        if (vlTOPp->lookup_i) {
            if ((0xaU == vlTOPp->lookup_did_i)) {
                if (vlTOPp->dma_composition_control__DOT__lookup_ok) {
                    if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__seen_lookup_v_d)))) {
                        vlTOPp->dma_composition_control__DOT__seen_lookup_v_d = 1U;
                        vlTOPp->dma_composition_control__DOT__pscid_v_d 
                            = vlTOPp->lookup_pscid_i;
                    }
                }
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__pscid_a_d 
        = vlTOPp->dma_composition_control__DOT__pscid_a_q;
    vlTOPp->dma_composition_control__DOT__seen_lookup_a_d 
        = vlTOPp->dma_composition_control__DOT__seen_lookup_a_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__seen_lookup_a_d = 0U;
                vlTOPp->dma_composition_control__DOT__pscid_a_d = 0U;
            }
        }
    }
    if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
        if (vlTOPp->lookup_i) {
            if ((0xaU != vlTOPp->lookup_did_i)) {
                if ((0xbU == vlTOPp->lookup_did_i)) {
                    if (vlTOPp->dma_composition_control__DOT__lookup_ok) {
                        if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__seen_lookup_a_d)))) {
                            vlTOPp->dma_composition_control__DOT__seen_lookup_a_d = 1U;
                            vlTOPp->dma_composition_control__DOT__pscid_a_d 
                                = vlTOPp->lookup_pscid_i;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__cfg_busy 
        = (1U & ((((((~ (IData)(vlTOPp->quiet_i)) | 
                     (1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) 
                    | (2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) 
                   | (IData)(vlTOPp->arm_i)) | (IData)(vlTOPp->open_i)) 
                 | (IData)(vlTOPp->close_i)));
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][3U];
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][3U] = 0U;
            }
        }
    }
    if (vlTOPp->close_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][0U] 
                    = vlTOPp->entries_next_i[0U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][1U] 
                    = vlTOPp->entries_next_i[0U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][2U] 
                    = vlTOPp->entries_next_i[0U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][3U] 
                    = vlTOPp->entries_next_i[0U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][0U] 
                    = vlTOPp->entries_next_i[1U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][1U] 
                    = vlTOPp->entries_next_i[1U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][2U] 
                    = vlTOPp->entries_next_i[1U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][3U] 
                    = vlTOPp->entries_next_i[1U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][0U] 
                    = vlTOPp->entries_next_i[2U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][1U] 
                    = vlTOPp->entries_next_i[2U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][2U] 
                    = vlTOPp->entries_next_i[2U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][3U] 
                    = vlTOPp->entries_next_i[2U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][0U] 
                    = vlTOPp->entries_next_i[3U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][1U] 
                    = vlTOPp->entries_next_i[3U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][2U] 
                    = vlTOPp->entries_next_i[3U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][3U] 
                    = vlTOPp->entries_next_i[3U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][0U] 
                    = vlTOPp->entries_next_i[4U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][1U] 
                    = vlTOPp->entries_next_i[4U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][2U] 
                    = vlTOPp->entries_next_i[4U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][3U] 
                    = vlTOPp->entries_next_i[4U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][0U] 
                    = vlTOPp->entries_next_i[5U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][1U] 
                    = vlTOPp->entries_next_i[5U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][2U] 
                    = vlTOPp->entries_next_i[5U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][3U] 
                    = vlTOPp->entries_next_i[5U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][0U] 
                    = vlTOPp->entries_next_i[6U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][1U] 
                    = vlTOPp->entries_next_i[6U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][2U] 
                    = vlTOPp->entries_next_i[6U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][3U] 
                    = vlTOPp->entries_next_i[6U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][0U] 
                    = vlTOPp->entries_next_i[7U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][1U] 
                    = vlTOPp->entries_next_i[7U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][2U] 
                    = vlTOPp->entries_next_i[7U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][3U] 
                    = vlTOPp->entries_next_i[7U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][0U] 
                    = vlTOPp->entries_next_i[8U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][1U] 
                    = vlTOPp->entries_next_i[8U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][2U] 
                    = vlTOPp->entries_next_i[8U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][3U] 
                    = vlTOPp->entries_next_i[8U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][0U] 
                    = vlTOPp->entries_next_i[9U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][1U] 
                    = vlTOPp->entries_next_i[9U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][2U] 
                    = vlTOPp->entries_next_i[9U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][3U] 
                    = vlTOPp->entries_next_i[9U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][0U] 
                    = vlTOPp->entries_next_i[0xaU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][1U] 
                    = vlTOPp->entries_next_i[0xaU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][2U] 
                    = vlTOPp->entries_next_i[0xaU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][3U] 
                    = vlTOPp->entries_next_i[0xaU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][0U] 
                    = vlTOPp->entries_next_i[0xbU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][1U] 
                    = vlTOPp->entries_next_i[0xbU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][2U] 
                    = vlTOPp->entries_next_i[0xbU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][3U] 
                    = vlTOPp->entries_next_i[0xbU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][0U] 
                    = vlTOPp->entries_next_i[0xcU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][1U] 
                    = vlTOPp->entries_next_i[0xcU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][2U] 
                    = vlTOPp->entries_next_i[0xcU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][3U] 
                    = vlTOPp->entries_next_i[0xcU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][0U] 
                    = vlTOPp->entries_next_i[0xdU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][1U] 
                    = vlTOPp->entries_next_i[0xdU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][2U] 
                    = vlTOPp->entries_next_i[0xdU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][3U] 
                    = vlTOPp->entries_next_i[0xdU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][0U] 
                    = vlTOPp->entries_next_i[0xeU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][1U] 
                    = vlTOPp->entries_next_i[0xeU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][2U] 
                    = vlTOPp->entries_next_i[0xeU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][3U] 
                    = vlTOPp->entries_next_i[0xeU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][0U] 
                    = vlTOPp->entries_next_i[0xfU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][1U] 
                    = vlTOPp->entries_next_i[0xfU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][2U] 
                    = vlTOPp->entries_next_i[0xfU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][3U] 
                    = vlTOPp->entries_next_i[0xfU][3U];
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][3U];
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][3U] = 0U;
            }
        }
    }
    if (vlTOPp->open_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][0U] 
                    = vlTOPp->entries_i[0U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][1U] 
                    = vlTOPp->entries_i[0U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][2U] 
                    = vlTOPp->entries_i[0U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][3U] 
                    = vlTOPp->entries_i[0U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][0U] 
                    = vlTOPp->entries_i[1U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][1U] 
                    = vlTOPp->entries_i[1U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][2U] 
                    = vlTOPp->entries_i[1U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][3U] 
                    = vlTOPp->entries_i[1U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][0U] 
                    = vlTOPp->entries_i[2U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][1U] 
                    = vlTOPp->entries_i[2U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][2U] 
                    = vlTOPp->entries_i[2U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][3U] 
                    = vlTOPp->entries_i[2U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][0U] 
                    = vlTOPp->entries_i[3U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][1U] 
                    = vlTOPp->entries_i[3U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][2U] 
                    = vlTOPp->entries_i[3U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][3U] 
                    = vlTOPp->entries_i[3U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][0U] 
                    = vlTOPp->entries_i[4U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][1U] 
                    = vlTOPp->entries_i[4U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][2U] 
                    = vlTOPp->entries_i[4U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][3U] 
                    = vlTOPp->entries_i[4U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][0U] 
                    = vlTOPp->entries_i[5U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][1U] 
                    = vlTOPp->entries_i[5U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][2U] 
                    = vlTOPp->entries_i[5U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][3U] 
                    = vlTOPp->entries_i[5U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][0U] 
                    = vlTOPp->entries_i[6U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][1U] 
                    = vlTOPp->entries_i[6U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][2U] 
                    = vlTOPp->entries_i[6U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][3U] 
                    = vlTOPp->entries_i[6U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][0U] 
                    = vlTOPp->entries_i[7U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][1U] 
                    = vlTOPp->entries_i[7U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][2U] 
                    = vlTOPp->entries_i[7U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][3U] 
                    = vlTOPp->entries_i[7U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][0U] 
                    = vlTOPp->entries_i[8U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][1U] 
                    = vlTOPp->entries_i[8U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][2U] 
                    = vlTOPp->entries_i[8U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][3U] 
                    = vlTOPp->entries_i[8U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][0U] 
                    = vlTOPp->entries_i[9U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][1U] 
                    = vlTOPp->entries_i[9U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][2U] 
                    = vlTOPp->entries_i[9U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][3U] 
                    = vlTOPp->entries_i[9U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][0U] 
                    = vlTOPp->entries_i[0xaU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][1U] 
                    = vlTOPp->entries_i[0xaU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][2U] 
                    = vlTOPp->entries_i[0xaU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][3U] 
                    = vlTOPp->entries_i[0xaU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][0U] 
                    = vlTOPp->entries_i[0xbU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][1U] 
                    = vlTOPp->entries_i[0xbU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][2U] 
                    = vlTOPp->entries_i[0xbU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][3U] 
                    = vlTOPp->entries_i[0xbU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][0U] 
                    = vlTOPp->entries_i[0xcU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][1U] 
                    = vlTOPp->entries_i[0xcU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][2U] 
                    = vlTOPp->entries_i[0xcU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][3U] 
                    = vlTOPp->entries_i[0xcU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][0U] 
                    = vlTOPp->entries_i[0xdU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][1U] 
                    = vlTOPp->entries_i[0xdU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][2U] 
                    = vlTOPp->entries_i[0xdU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][3U] 
                    = vlTOPp->entries_i[0xdU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][0U] 
                    = vlTOPp->entries_i[0xeU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][1U] 
                    = vlTOPp->entries_i[0xeU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][2U] 
                    = vlTOPp->entries_i[0xeU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][3U] 
                    = vlTOPp->entries_i[0xeU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][0U] 
                    = vlTOPp->entries_i[0xfU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][1U] 
                    = vlTOPp->entries_i[0xfU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][2U] 
                    = vlTOPp->entries_i[0xfU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][3U] 
                    = vlTOPp->entries_i[0xfU][3U];
            }
        }
    }
    vlTOPp->rdata_o = 0ULL;
    if ((0x40U & (IData)(vlTOPp->index_i))) {
        if (((0x40U <= (IData)(vlTOPp->index_i)) & 
             (0x5fU >= (IData)(vlTOPp->index_i)))) {
            vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                ? (((QData)((IData)(
                                                    vlTOPp->dma_composition_control__DOT__entries_open_q
                                                    [
                                                    (0xfU 
                                                     & ((IData)(vlTOPp->index_i) 
                                                        >> 1U))][3U])) 
                                    << 0x20U) | (QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][2U])))
                                : (((QData)((IData)(
                                                    vlTOPp->dma_composition_control__DOT__entries_open_q
                                                    [
                                                    (0xfU 
                                                     & ((IData)(vlTOPp->index_i) 
                                                        >> 1U))][1U])) 
                                    << 0x20U) | (QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][0U]))));
        } else {
            if ((0x60U <= (IData)(vlTOPp->index_i))) {
                vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                    ? (((QData)((IData)(
                                                        vlTOPp->dma_composition_control__DOT__entries_close_q
                                                        [
                                                        (0xfU 
                                                         & ((IData)(vlTOPp->index_i) 
                                                            >> 1U))][3U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][2U])))
                                    : (((QData)((IData)(
                                                        vlTOPp->dma_composition_control__DOT__entries_close_q
                                                        [
                                                        (0xfU 
                                                         & ((IData)(vlTOPp->index_i) 
                                                            >> 1U))][1U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][0U]))));
            }
        }
    } else {
        if ((0x20U & (IData)(vlTOPp->index_i))) {
            if ((0x10U & (IData)(vlTOPp->index_i))) {
                if (((0x40U <= (IData)(vlTOPp->index_i)) 
                     & (0x5fU >= (IData)(vlTOPp->index_i)))) {
                    vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                        ? (((QData)((IData)(
                                                            vlTOPp->dma_composition_control__DOT__entries_open_q
                                                            [
                                                            (0xfU 
                                                             & ((IData)(vlTOPp->index_i) 
                                                                >> 1U))][3U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->dma_composition_control__DOT__entries_open_q
                                                             [
                                                             (0xfU 
                                                              & ((IData)(vlTOPp->index_i) 
                                                                 >> 1U))][2U])))
                                        : (((QData)((IData)(
                                                            vlTOPp->dma_composition_control__DOT__entries_open_q
                                                            [
                                                            (0xfU 
                                                             & ((IData)(vlTOPp->index_i) 
                                                                >> 1U))][1U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->dma_composition_control__DOT__entries_open_q
                                                             [
                                                             (0xfU 
                                                              & ((IData)(vlTOPp->index_i) 
                                                                 >> 1U))][0U]))));
                } else {
                    if ((0x60U <= (IData)(vlTOPp->index_i))) {
                        vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                            ? (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][3U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][2U])))
                                            : (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][1U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][0U]))));
                    }
                }
            } else {
                if ((8U & (IData)(vlTOPp->index_i))) {
                    if (((0x40U <= (IData)(vlTOPp->index_i)) 
                         & (0x5fU >= (IData)(vlTOPp->index_i)))) {
                        vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                            ? (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][3U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][2U])))
                                            : (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][1U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][0U]))));
                    } else {
                        if ((0x60U <= (IData)(vlTOPp->index_i))) {
                            vlTOPp->rdata_o = ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (
                                                   ((QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][3U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                     [
                                                                     (0xfU 
                                                                      & ((IData)(vlTOPp->index_i) 
                                                                         >> 1U))][2U])))
                                                : (
                                                   ((QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][1U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                     [
                                                                     (0xfU 
                                                                      & ((IData)(vlTOPp->index_i) 
                                                                         >> 1U))][0U]))));
                        }
                    }
                } else {
                    vlTOPp->rdata_o = ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(vlTOPp->dma_composition_control__DOT__pscid_a_q))
                                                : (QData)((IData)(vlTOPp->dma_composition_control__DOT__pscid_v_q)))
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(vlTOPp->dma_composition_control__DOT__plru_close_q))
                                                : (QData)((IData)(vlTOPp->dma_composition_control__DOT__plru_open_q))))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? 0ULL
                                                : 3ULL)
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x21U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x20U])));
                }
            }
        } else {
            vlTOPp->rdata_o = ((0x10U & (IData)(vlTOPp->index_i))
                                ? ((8U & (IData)(vlTOPp->index_i))
                                    ? ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1fU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1eU])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1dU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1cU]))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1bU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1aU])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x19U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x18U])))
                                    : ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x17U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x16U])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x15U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x14U]))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x13U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x12U])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x11U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x10U]))))
                                : ((8U & (IData)(vlTOPp->index_i))
                                    ? ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xfU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xeU])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xdU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xcU]))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__win_gen_q
                                                : vlTOPp->dma_composition_control__DOT__win_cfg_q)
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(vlTOPp->dma_composition_control__DOT__err_q))
                                                : vlTOPp->dma_composition_control__DOT__cycles_q)))
                                    : ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__seq_q
                                                : (
                                                   ((QData)((IData)(
                                                                    (3U 
                                                                     == (IData)(vlTOPp->dma_composition_control__DOT__state_q)))) 
                                                    << 2U) 
                                                   | (QData)((IData)(
                                                                     (((2U 
                                                                        == (IData)(vlTOPp->dma_composition_control__DOT__state_q)) 
                                                                       << 1U) 
                                                                      | (1U 
                                                                         == (IData)(vlTOPp->dma_composition_control__DOT__state_q)))))))
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(
                                                                  (((((IData)(vlTOPp->quiet_i) 
                                                                      & (~ 
                                                                         ((1U 
                                                                           == (IData)(vlTOPp->dma_composition_control__DOT__state_q)) 
                                                                          | (2U 
                                                                             == (IData)(vlTOPp->dma_composition_control__DOT__state_q))))) 
                                                                     & (~ (IData)(vlTOPp->arm_i))) 
                                                                    & (~ (IData)(vlTOPp->open_i))) 
                                                                   & (~ (IData)(vlTOPp->close_i)))))
                                                : (QData)((IData)(vlTOPp->dma_composition_control__DOT__last_status_q))))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__generation_q
                                                : (
                                                   ((QData)((IData)(vlTOPp->dma_composition_control__DOT__protected_did_q)) 
                                                    << 8U) 
                                                   | (QData)((IData)(vlTOPp->dma_composition_control__DOT__mode_q))))
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? 1ULL
                                                : 0x49444d4143504e01ULL)))));
        }
    }
    vlTOPp->dma_composition_control__DOT__cycles_d 
        = vlTOPp->dma_composition_control__DOT__cycles_q;
    vlTOPp->dma_composition_control__DOT__err_d = vlTOPp->dma_composition_control__DOT__err_q;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [1U];
    vlTOPp->dma_composition_control__DOT__cnt_d[1U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [2U];
    vlTOPp->dma_composition_control__DOT__cnt_d[2U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [3U];
    vlTOPp->dma_composition_control__DOT__cnt_d[3U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [4U];
    vlTOPp->dma_composition_control__DOT__cnt_d[4U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [5U];
    vlTOPp->dma_composition_control__DOT__cnt_d[5U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [6U];
    vlTOPp->dma_composition_control__DOT__cnt_d[6U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [7U];
    vlTOPp->dma_composition_control__DOT__cnt_d[7U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [8U];
    vlTOPp->dma_composition_control__DOT__cnt_d[8U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [9U];
    vlTOPp->dma_composition_control__DOT__cnt_d[9U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xaU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xaU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xbU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xbU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xcU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xcU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xdU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xdU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xeU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xeU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xfU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xfU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x10U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x10U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x11U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x11U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x12U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x12U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x13U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x13U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x14U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x14U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x15U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x15U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x16U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x16U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x17U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x17U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x18U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x18U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x19U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x19U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1aU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1aU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1bU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1bU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1cU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1cU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1dU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1dU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1eU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1fU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1fU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x20U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x21U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x21U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x22U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x22U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x23U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x23U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x24U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x24U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x25U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x25U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x26U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x26U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x27U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x27U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    if (vlTOPp->arm_i) {
        if (((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__err_d = 0U;
                vlTOPp->dma_composition_control__DOT__cycles_d = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[1U] = 0ULL;
                if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__seq_q)) {
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                }
                vlTOPp->dma_composition_control__DOT__cnt_d[2U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[3U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[4U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[5U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[6U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[7U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[8U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[9U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xaU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xbU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xcU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xdU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xeU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xfU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x10U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x11U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x12U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x13U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x14U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x15U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x16U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x17U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x18U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x19U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1aU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1bU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1cU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1dU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1fU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x21U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x22U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x23U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x24U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x25U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x26U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x27U] = 0ULL;
            }
        }
    }
    if (vlTOPp->open_i) {
        if (vlTOPp->arm_i) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            if ((1U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
    }
    if (vlTOPp->close_i) {
        if (vlTOPp->arm_i) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
    }
    if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
        if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cycles_d)) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            vlTOPp->dma_composition_control__DOT__cycles_d 
                = (1ULL + vlTOPp->dma_composition_control__DOT__cycles_d);
        }
        if ((1U & (IData)(vlTOPp->gap_active_i))) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                 [0x18U])) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__cnt_d[0x18U] 
                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                       [0x18U]);
            }
        }
        if ((2U & (IData)(vlTOPp->gap_active_i))) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                 [0x19U])) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__cnt_d[0x19U] 
                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                       [0x19U]);
            }
        }
        if (vlTOPp->lookup_i) {
            if ((0xaU == vlTOPp->lookup_did_i)) {
                if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                     [0xcU])) {
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                } else {
                    vlTOPp->dma_composition_control__DOT__cnt_d[0xcU] 
                        = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                           [0xcU]);
                }
                if (vlTOPp->lookup_hit_i) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0xdU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0xdU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0xdU]);
                    }
                } else {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0xeU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0xeU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0xeU]);
                    }
                }
                if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__lookup_ok)))) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x20U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x20U]);
                    }
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                }
            } else {
                if ((0xbU == vlTOPp->lookup_did_i)) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0xfU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0xfU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0xfU]);
                    }
                    if (vlTOPp->lookup_hit_i) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x10U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x10U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x10U]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x11U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x11U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x11U]);
                        }
                    }
                    if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__lookup_ok)))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x20U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x20U]);
                        }
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    }
                } else {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x12U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x12U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x12U]);
                    }
                    if (vlTOPp->lookup_hit_i) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x13U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x13U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x13U]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x14U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x14U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x14U]);
                        }
                    }
                    if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__lookup_ok)))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x20U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x20U]);
                        }
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    }
                }
            }
        }
        if (vlTOPp->fast_i) {
            if (vlTOPp->accept_i) {
                if ((0xaU == vlTOPp->fast_did_i)) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x15U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x15U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x15U]);
                    }
                } else {
                    if ((0xbU == vlTOPp->fast_did_i)) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x16U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x16U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x16U]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x17U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x17U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x17U]);
                        }
                    }
                }
                if ((1U & (~ ((1U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)) 
                              | (3U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)))))) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x21U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x21U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x21U]);
                    }
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (4U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                }
            } else {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
        if (vlTOPp->update_i) {
            if (((0U != (IData)(vlTOPp->replace_mask_i)) 
                 & (0U == ((IData)(vlTOPp->replace_mask_i) 
                           & ((IData)(vlTOPp->replace_mask_i) 
                              - (IData)(1U)))))) {
                if ((0xaU == vlTOPp->update_did_i)) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x1aU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x1aU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x1aU]);
                    }
                } else {
                    if ((0xbU == vlTOPp->update_did_i)) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x1bU])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x1bU] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x1bU]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x1cU])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x1cU] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x1cU]);
                        }
                    }
                }
                if (((2U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)) 
                     | (3U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)))) {
                    if ((vlTOPp->update_did_i == vlTOPp->dma_composition_control__DOT__protected_did_q)) {
                        if ((2U <= (IData)(vlTOPp->dma_composition_control__DOT__repl_idx))) {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_composition_control__DOT__cnt_d
                                 [0x1eU])) {
                                vlTOPp->dma_composition_control__DOT__err_d 
                                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                            } else {
                                vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] 
                                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                       [0x1eU]);
                            }
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (0x10U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        }
                    } else {
                        if ((2U > (IData)(vlTOPp->dma_composition_control__DOT__repl_idx))) {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_composition_control__DOT__cnt_d
                                 [0x1eU])) {
                                vlTOPp->dma_composition_control__DOT__err_d 
                                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                            } else {
                                vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] 
                                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                       [0x1eU]);
                            }
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (0x10U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        }
                    }
                }
                if (((vlTOPp->entries_i[vlTOPp->dma_composition_control__DOT__repl_idx][1U] 
                      >> 0x13U) & ((0xfffffU & ((vlTOPp->entries_i
                                                 [vlTOPp->dma_composition_control__DOT__repl_idx][1U] 
                                                 << 3U) 
                                                | (vlTOPp->entries_i
                                                   [vlTOPp->dma_composition_control__DOT__repl_idx][0U] 
                                                   >> 0x1dU))) 
                                   != (0xfffffU & (
                                                   (vlTOPp->entries_next_i
                                                    [vlTOPp->dma_composition_control__DOT__repl_idx][1U] 
                                                    << 3U) 
                                                   | (vlTOPp->entries_next_i
                                                      [vlTOPp->dma_composition_control__DOT__repl_idx][0U] 
                                                      >> 0x1dU)))))) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x1dU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x1dU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x1dU]);
                    }
                }
            } else {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
        if (vlTOPp->invalidate_i) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                 [0x1fU])) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1fU] 
                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                       [0x1fU]);
            }
        }
    }
    vlTOPp->clear_o = (((((((IData)(vlTOPp->rst_ni) 
                            & (IData)(vlTOPp->cfg_write_i)) 
                           & (2U == (IData)(vlTOPp->index_i))) 
                          & (~ (IData)(vlTOPp->dma_composition_control__DOT__cfg_busy))) 
                         & (IData)(vlTOPp->supported_i)) 
                        & (IData)(vlTOPp->dma_composition_control__DOT__cfg_data_legal)) 
                       & (0xffffffffffffffffULL != vlTOPp->dma_composition_control__DOT__generation_q));
}

void Vdma_composition_control::_eval(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_eval\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if (((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i)))) {
        vlTOPp->_sequent__TOP__3(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
}

VL_INLINE_OPT QData Vdma_composition_control::_change_request(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_change_request\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_composition_control::_change_request_1(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_change_request_1\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_composition_control::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((cfg_write_i & 0xfeU))) {
        Verilated::overWidthError("cfg_write_i");}
    if (VL_UNLIKELY((index_i & 0x80U))) {
        Verilated::overWidthError("index_i");}
    if (VL_UNLIKELY((quiet_i & 0xfeU))) {
        Verilated::overWidthError("quiet_i");}
    if (VL_UNLIKELY((supported_i & 0xfeU))) {
        Verilated::overWidthError("supported_i");}
    if (VL_UNLIKELY((arm_i & 0xfeU))) {
        Verilated::overWidthError("arm_i");}
    if (VL_UNLIKELY((open_i & 0xfeU))) {
        Verilated::overWidthError("open_i");}
    if (VL_UNLIKELY((close_i & 0xfeU))) {
        Verilated::overWidthError("close_i");}
    if (VL_UNLIKELY((lookup_i & 0xfeU))) {
        Verilated::overWidthError("lookup_i");}
    if (VL_UNLIKELY((lookup_hit_i & 0xfeU))) {
        Verilated::overWidthError("lookup_hit_i");}
    if (VL_UNLIKELY((normal_i & 0xfeU))) {
        Verilated::overWidthError("normal_i");}
    if (VL_UNLIKELY((stage1_i & 0xfeU))) {
        Verilated::overWidthError("stage1_i");}
    if (VL_UNLIKELY((stage2_i & 0xfeU))) {
        Verilated::overWidthError("stage2_i");}
    if (VL_UNLIKELY((lookup_did_i & 0xff000000U))) {
        Verilated::overWidthError("lookup_did_i");}
    if (VL_UNLIKELY((lookup_pscid_i & 0xfff00000U))) {
        Verilated::overWidthError("lookup_pscid_i");}
    if (VL_UNLIKELY((accept_i & 0xfeU))) {
        Verilated::overWidthError("accept_i");}
    if (VL_UNLIKELY((fast_i & 0xfeU))) {
        Verilated::overWidthError("fast_i");}
    if (VL_UNLIKELY((fast_did_i & 0xff000000U))) {
        Verilated::overWidthError("fast_did_i");}
    if (VL_UNLIKELY((gap_active_i & 0xfcU))) {
        Verilated::overWidthError("gap_active_i");}
    if (VL_UNLIKELY((update_i & 0xfeU))) {
        Verilated::overWidthError("update_i");}
    if (VL_UNLIKELY((invalidate_i & 0xfeU))) {
        Verilated::overWidthError("invalidate_i");}
    if (VL_UNLIKELY((update_did_i & 0xff000000U))) {
        Verilated::overWidthError("update_did_i");}
}
#endif  // VL_DEBUG
