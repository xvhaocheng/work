// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_e3_write_observer.h for the primary calling header

#include "Vdma_e3_write_observer.h"
#include "Vdma_e3_write_observer__Syms.h"

//==========

void Vdma_e3_write_observer::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_e3_write_observer::eval\n"); );
    Vdma_e3_write_observer__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_e3_write_observer.sv", 4, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_e3_write_observer::_eval_initial_loop(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_e3_write_observer.sv", 4, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_e3_write_observer::_sequent__TOP__1(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_sequent__TOP__1\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v0;
    CData/*1:0*/ __Vdlyvval__dma_e3_write_observer__DOT__tail_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v2;
    CData/*1:0*/ __Vdlyvval__dma_e3_write_observer__DOT__tail_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__head_q__v0;
    CData/*1:0*/ __Vdlyvval__dma_e3_write_observer__DOT__head_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__head_q__v2;
    CData/*1:0*/ __Vdlyvval__dma_e3_write_observer__DOT__head_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__head_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v0;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v8;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v9;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v10;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v11;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v12;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v13;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v14;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v15;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__data_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__data_q__v32;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__data_q__v64;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__role_q__v0;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v1;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v2;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v3;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v4;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v5;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v6;
    CData/*0:0*/ __Vdlyvval__dma_e3_write_observer__DOT__role_q__v7;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__role_q__v8;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__order_q__v0;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v1;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v2;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v3;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v4;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v5;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v6;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__order_q__v7;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__order_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__count_q__v0;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__count_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__count_q__v2;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__count_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__count_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__present_q__v0;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__present_q__v8;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v9;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v10;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v11;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v12;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v13;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v14;
    CData/*2:0*/ __Vdlyvval__dma_e3_write_observer__DOT__present_q__v15;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__present_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v7;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v8;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v9;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v10;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v11;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v12;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v13;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v14;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v15;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v16;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v17;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v18;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v19;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v20;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v21;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v22;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v23;
    CData/*0:0*/ __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v24;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v6;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v7;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v8;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v9;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v10;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v11;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v12;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v13;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v14;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v15;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v16;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v17;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v18;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v19;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v20;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v21;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v22;
    IData/*31:0*/ __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v23;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v32;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v33;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v34;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v35;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v36;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v37;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v38;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v39;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v40;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v41;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v42;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v43;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v44;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v45;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v46;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v47;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v48;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v49;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v50;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v51;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v52;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v53;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v54;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v55;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v56;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v57;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v58;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v59;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v60;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v61;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v62;
    QData/*63:0*/ __Vdlyvval__dma_e3_write_observer__DOT__data_q__v63;
    // Body
    __Vdlyvset__dma_e3_write_observer__DOT__count_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__count_q__v2 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__count_q__v4 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__head_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__head_q__v2 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__head_q__v4 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v2 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v4 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__order_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__order_q__v8 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__role_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__role_q__v8 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__present_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__present_q__v8 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__present_q__v16 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v8 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v16 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__data_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__data_q__v32 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__data_q__v64 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v0 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v1 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v2 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v3 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v4 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v5 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v6 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v7 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v8 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v9 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v10 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v11 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v12 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v13 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v14 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v15 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v16 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v17 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v18 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v19 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v20 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v21 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v22 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v23 = 0U;
    __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v24 = 0U;
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->dma_e3_write_observer__DOT__aw_tail_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                vlTOPp->dma_e3_write_observer__DOT__aw_tail_q 
                    = vlTOPp->dma_e3_write_observer__DOT__aw_tail_n;
            }
        }
    } else {
        vlTOPp->dma_e3_write_observer__DOT__aw_tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->dma_e3_write_observer__DOT__w_tail_q = 0U;
        } else {
            if (vlTOPp->run_i) {
                vlTOPp->dma_e3_write_observer__DOT__w_tail_q 
                    = vlTOPp->dma_e3_write_observer__DOT__w_tail_n;
            }
        }
    } else {
        vlTOPp->dma_e3_write_observer__DOT__w_tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            vlTOPp->error_o = 0U;
        } else {
            if (vlTOPp->run_i) {
                vlTOPp->error_o = vlTOPp->dma_e3_write_observer__DOT__bad;
            }
        }
    } else {
        vlTOPp->error_o = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdlyvset__dma_e3_write_observer__DOT__count_q__v0 = 1U;
        } else {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__count_q__v2 
                    = vlTOPp->dma_e3_write_observer__DOT__count_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__count_q__v2 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__count_q__v3 
                    = vlTOPp->dma_e3_write_observer__DOT__count_n
                    [1U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__count_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdlyvset__dma_e3_write_observer__DOT__head_q__v0 = 1U;
        } else {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__head_q__v2 
                    = vlTOPp->dma_e3_write_observer__DOT__head_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__head_q__v2 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__head_q__v3 
                    = vlTOPp->dma_e3_write_observer__DOT__head_n
                    [1U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__head_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v0 = 1U;
        } else {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__tail_q__v2 
                    = vlTOPp->dma_e3_write_observer__DOT__tail_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v2 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__tail_q__v3 
                    = vlTOPp->dma_e3_write_observer__DOT__tail_n
                    [1U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__tail_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(vlTOPp->clear_i)))) {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v0 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [0U][0U];
                __Vdlyvset__dma_e3_write_observer__DOT__order_q__v0 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v1 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [0U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v2 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [0U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v3 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [0U][3U];
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v4 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [1U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v5 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [1U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v6 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [1U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__order_q__v7 
                    = vlTOPp->dma_e3_write_observer__DOT__order_n
                    [1U][3U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__order_q__v8 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(vlTOPp->clear_i)))) {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v0 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__role_q__v0 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v1 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [1U];
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v2 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [2U];
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v3 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [3U];
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v4 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [4U];
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v5 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [5U];
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v6 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [6U];
                __Vdlyvval__dma_e3_write_observer__DOT__role_q__v7 
                    = vlTOPp->dma_e3_write_observer__DOT__role_n
                    [7U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__role_q__v8 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdlyvset__dma_e3_write_observer__DOT__present_q__v0 = 1U;
        } else {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v8 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__present_q__v8 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v9 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [1U];
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v10 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [2U];
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v11 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [3U];
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v12 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [4U];
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v13 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [5U];
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v14 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [6U];
                __Vdlyvval__dma_e3_write_observer__DOT__present_q__v15 
                    = vlTOPp->dma_e3_write_observer__DOT__present_n
                    [7U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__present_q__v16 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v0 = 1U;
        } else {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v8 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v8 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v9 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [1U];
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v10 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [2U];
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v11 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [3U];
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v12 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [4U];
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v13 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [5U];
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v14 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [6U];
                __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v15 
                    = vlTOPp->dma_e3_write_observer__DOT__inside_n
                    [7U];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__inside_q__v16 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->clear_i) {
            __Vdlyvset__dma_e3_write_observer__DOT__data_q__v0 = 1U;
        } else {
            if (vlTOPp->run_i) {
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v32 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0U];
                __Vdlyvset__dma_e3_write_observer__DOT__data_q__v32 = 1U;
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v33 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [1U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v34 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [2U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v35 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [3U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v36 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [4U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v37 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [5U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v38 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [6U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v39 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [7U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v40 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [8U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v41 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [9U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v42 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0xaU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v43 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0xbU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v44 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0xcU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v45 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0xdU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v46 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0xeU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v47 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0xfU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v48 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x10U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v49 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x11U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v50 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x12U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v51 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x13U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v52 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x14U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v53 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x15U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v54 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x16U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v55 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x17U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v56 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x18U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v57 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x19U];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v58 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x1aU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v59 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x1bU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v60 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x1cU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v61 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x1dU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v62 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x1eU];
                __Vdlyvval__dma_e3_write_observer__DOT__data_q__v63 
                    = vlTOPp->dma_e3_write_observer__DOT__data_n
                    [0x1fU];
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__data_q__v64 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(vlTOPp->clear_i)))) {
            if (vlTOPp->run_i) {
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [0U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v0 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v0 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [0U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v1 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v1 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [0U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v2 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v2 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [1U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v3 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v3 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [1U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v4 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v4 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [1U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v5 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v5 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [2U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v6 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v6 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [2U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v7 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v7 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [2U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v8 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v8 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [3U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v9 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v9 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [3U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v10 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v10 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [3U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v11 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v11 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [4U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v12 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v12 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [4U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v13 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v13 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [4U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v14 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v14 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [5U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v15 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v15 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [5U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v16 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v16 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [5U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v17 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v17 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [6U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v18 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v18 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [6U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v19 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v19 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [6U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v20 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v20 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [7U][0U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v21 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v21 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [7U][1U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v22 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v22 = 1U;
                vlTOPp->dma_e3_write_observer__DOT____Vlvbound2 
                    = vlTOPp->dma_e3_write_observer__DOT__stamps_n
                    [7U][2U];
                __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v23 
                    = vlTOPp->dma_e3_write_observer__DOT____Vlvbound2;
                __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v23 = 1U;
            }
        }
    } else {
        __Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v24 = 1U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__count_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__count_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__count_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__count_q__v2) {
        vlTOPp->dma_e3_write_observer__DOT__count_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__count_q__v2;
        vlTOPp->dma_e3_write_observer__DOT__count_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__count_q__v3;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__count_q__v4) {
        vlTOPp->dma_e3_write_observer__DOT__count_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__count_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__head_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__head_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__head_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__head_q__v2) {
        vlTOPp->dma_e3_write_observer__DOT__head_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__head_q__v2;
        vlTOPp->dma_e3_write_observer__DOT__head_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__head_q__v3;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__head_q__v4) {
        vlTOPp->dma_e3_write_observer__DOT__head_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__head_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__tail_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__tail_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__tail_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__tail_q__v2) {
        vlTOPp->dma_e3_write_observer__DOT__tail_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__tail_q__v2;
        vlTOPp->dma_e3_write_observer__DOT__tail_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__tail_q__v3;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__tail_q__v4) {
        vlTOPp->dma_e3_write_observer__DOT__tail_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__tail_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__order_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v0;
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v1;
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v2;
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][3U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v3;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v4;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v5;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v6;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][3U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__order_q__v7;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__order_q__v8) {
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[0U][3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__order_q[1U][3U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__role_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__role_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v0;
        vlTOPp->dma_e3_write_observer__DOT__role_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v1;
        vlTOPp->dma_e3_write_observer__DOT__role_q[2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v2;
        vlTOPp->dma_e3_write_observer__DOT__role_q[3U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v3;
        vlTOPp->dma_e3_write_observer__DOT__role_q[4U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v4;
        vlTOPp->dma_e3_write_observer__DOT__role_q[5U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v5;
        vlTOPp->dma_e3_write_observer__DOT__role_q[6U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v6;
        vlTOPp->dma_e3_write_observer__DOT__role_q[7U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__role_q__v7;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__role_q__v8) {
        vlTOPp->dma_e3_write_observer__DOT__role_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__role_q[7U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__present_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__present_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[7U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__present_q__v8) {
        vlTOPp->dma_e3_write_observer__DOT__present_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v8;
        vlTOPp->dma_e3_write_observer__DOT__present_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v9;
        vlTOPp->dma_e3_write_observer__DOT__present_q[2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v10;
        vlTOPp->dma_e3_write_observer__DOT__present_q[3U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v11;
        vlTOPp->dma_e3_write_observer__DOT__present_q[4U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v12;
        vlTOPp->dma_e3_write_observer__DOT__present_q[5U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v13;
        vlTOPp->dma_e3_write_observer__DOT__present_q[6U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v14;
        vlTOPp->dma_e3_write_observer__DOT__present_q[7U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__present_q__v15;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__present_q__v16) {
        vlTOPp->dma_e3_write_observer__DOT__present_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__present_q[7U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__inside_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__inside_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[7U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__inside_q__v8) {
        vlTOPp->dma_e3_write_observer__DOT__inside_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v8;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v9;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v10;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[3U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v11;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[4U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v12;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[5U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v13;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[6U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v14;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[7U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__inside_q__v15;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__inside_q__v16) {
        vlTOPp->dma_e3_write_observer__DOT__inside_q[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_q[7U] = 0U;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__data_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__data_q[0U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[1U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[2U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[3U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[4U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[5U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[6U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[7U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[8U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[9U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xaU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xbU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xcU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xdU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xeU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xfU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x10U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x11U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x12U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x13U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x14U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x15U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x16U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x17U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x18U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x19U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1aU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1bU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1cU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1dU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1eU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1fU] = 0ULL;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__data_q__v32) {
        vlTOPp->dma_e3_write_observer__DOT__data_q[0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v32;
        vlTOPp->dma_e3_write_observer__DOT__data_q[1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v33;
        vlTOPp->dma_e3_write_observer__DOT__data_q[2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v34;
        vlTOPp->dma_e3_write_observer__DOT__data_q[3U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v35;
        vlTOPp->dma_e3_write_observer__DOT__data_q[4U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v36;
        vlTOPp->dma_e3_write_observer__DOT__data_q[5U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v37;
        vlTOPp->dma_e3_write_observer__DOT__data_q[6U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v38;
        vlTOPp->dma_e3_write_observer__DOT__data_q[7U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v39;
        vlTOPp->dma_e3_write_observer__DOT__data_q[8U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v40;
        vlTOPp->dma_e3_write_observer__DOT__data_q[9U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v41;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xaU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v42;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xbU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v43;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xcU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v44;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xdU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v45;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xeU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v46;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xfU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v47;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x10U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v48;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x11U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v49;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x12U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v50;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x13U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v51;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x14U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v52;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x15U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v53;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x16U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v54;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x17U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v55;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x18U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v56;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x19U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v57;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1aU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v58;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1bU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v59;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1cU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v60;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1dU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v61;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1eU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v62;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1fU] 
            = __Vdlyvval__dma_e3_write_observer__DOT__data_q__v63;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__data_q__v64) {
        vlTOPp->dma_e3_write_observer__DOT__data_q[0U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[1U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[2U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[3U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[4U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[5U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[6U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[7U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[8U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[9U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xaU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xbU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xcU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xdU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xeU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0xfU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x10U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x11U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x12U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x13U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x14U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x15U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x16U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x17U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x18U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x19U] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1aU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1bU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1cU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1dU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1eU] = 0ULL;
        vlTOPp->dma_e3_write_observer__DOT__data_q[0x1fU] = 0ULL;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v0) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[0U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v0;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v1) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[0U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v1;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v2) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[0U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v2;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v3) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[1U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v3;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v4) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[1U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v4;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v5) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[1U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v5;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v6) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[2U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v6;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v7) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[2U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v7;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v8) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[2U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v8;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v9) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[3U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v9;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v10) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[3U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v10;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v11) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[3U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v11;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v12) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[4U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v12;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v13) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[4U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v13;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v14) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[4U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v14;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v15) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[5U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v15;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v16) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[5U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v16;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v17) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[5U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v17;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v18) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[6U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v18;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v19) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[6U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v19;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v20) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[6U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v20;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v21) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[7U][0U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v21;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v22) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[7U][1U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v22;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v23) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[7U][2U] 
            = __Vdlyvval__dma_e3_write_observer__DOT__stamps_q__v23;
    }
    if (__Vdlyvset__dma_e3_write_observer__DOT__stamps_q__v24) {
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[0U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[0U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[0U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[1U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[1U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[1U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[2U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[2U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[2U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[3U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[3U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[3U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[4U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[4U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[4U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[5U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[5U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[5U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[6U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[6U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[6U][2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[7U][0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[7U][1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__stamps_q[7U][2U] = 0U;
    }
    vlTOPp->data_o[0U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0U];
    vlTOPp->data_o[1U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [1U];
    vlTOPp->data_o[2U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [2U];
    vlTOPp->data_o[3U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [3U];
    vlTOPp->data_o[4U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [4U];
    vlTOPp->data_o[5U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [5U];
    vlTOPp->data_o[6U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [6U];
    vlTOPp->data_o[7U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [7U];
    vlTOPp->data_o[8U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [8U];
    vlTOPp->data_o[9U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [9U];
    vlTOPp->data_o[0xaU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xaU];
    vlTOPp->data_o[0xbU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xbU];
    vlTOPp->data_o[0xcU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xcU];
    vlTOPp->data_o[0xdU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xdU];
    vlTOPp->data_o[0xeU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xeU];
    vlTOPp->data_o[0xfU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xfU];
    vlTOPp->data_o[0x10U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x10U];
    vlTOPp->data_o[0x11U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x11U];
    vlTOPp->data_o[0x12U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x12U];
    vlTOPp->data_o[0x13U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x13U];
    vlTOPp->data_o[0x14U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x14U];
    vlTOPp->data_o[0x15U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x15U];
    vlTOPp->data_o[0x16U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x16U];
    vlTOPp->data_o[0x17U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x17U];
    vlTOPp->data_o[0x18U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x18U];
    vlTOPp->data_o[0x19U] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x19U];
    vlTOPp->data_o[0x1aU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1aU];
    vlTOPp->data_o[0x1bU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1bU];
    vlTOPp->data_o[0x1cU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1cU];
    vlTOPp->data_o[0x1dU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1dU];
    vlTOPp->data_o[0x1eU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1eU];
    vlTOPp->data_o[0x1fU] = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1fU];
}

VL_INLINE_OPT void Vdma_e3_write_observer::_combo__TOP__3(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_combo__TOP__3\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_e3_write_observer__DOT__aw_tail_n = vlTOPp->dma_e3_write_observer__DOT__aw_tail_q;
    if (vlTOPp->aw_i) {
        vlTOPp->dma_e3_write_observer__DOT__aw_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->dma_e3_write_observer__DOT__aw_tail_q)));
    }
    vlTOPp->dma_e3_write_observer__DOT__w_tail_n = vlTOPp->dma_e3_write_observer__DOT__w_tail_q;
    if (vlTOPp->wlast_i) {
        vlTOPp->dma_e3_write_observer__DOT__w_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->dma_e3_write_observer__DOT__w_tail_q)));
    }
    vlTOPp->dma_e3_write_observer__DOT__bad = vlTOPp->error_o;
    vlTOPp->dma_e3_write_observer__DOT__role = 0U;
    vlTOPp->dma_e3_write_observer__DOT__slot = 0U;
    vlTOPp->dma_e3_write_observer__DOT__base = 0U;
    vlTOPp->dma_e3_write_observer__DOT__aw_w = 0ULL;
    vlTOPp->dma_e3_write_observer__DOT__w_b = 0ULL;
    vlTOPp->dma_e3_write_observer__DOT__aw_b = 0ULL;
    vlTOPp->dma_e3_write_observer__DOT__present_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [0U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[0U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [0U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[0U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [0U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[0U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [1U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[1U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [1U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[1U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [1U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[1U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [2U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[2U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [2U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[2U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [2U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[2U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [3U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[3U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [3U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[3U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [3U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[3U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [4U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[4U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [4U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[4U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [4U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[4U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [5U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[5U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [5U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[5U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [5U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[5U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [6U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[6U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [6U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[6U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [6U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[6U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__present_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__present_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT__inside_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__inside_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT__role_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__role_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [7U][0U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[7U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [7U][1U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[7U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT____Vlvbound1 
        = vlTOPp->dma_e3_write_observer__DOT__stamps_q
        [7U][2U];
    vlTOPp->dma_e3_write_observer__DOT__stamps_n[7U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT____Vlvbound1;
    vlTOPp->dma_e3_write_observer__DOT__head_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__head_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__tail_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__tail_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__count_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__count_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][0U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][1U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][2U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[0U][3U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [0U][3U];
    vlTOPp->dma_e3_write_observer__DOT__head_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__head_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__tail_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__tail_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__count_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__count_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][0U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][0U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][1U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][1U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][2U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][2U];
    vlTOPp->dma_e3_write_observer__DOT__order_n[1U][3U] 
        = vlTOPp->dma_e3_write_observer__DOT__order_q
        [1U][3U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[1U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [1U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[2U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [2U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[3U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [3U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[4U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [4U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[5U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [5U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[6U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [6U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[7U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [7U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[8U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [8U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[9U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [9U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xaU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xaU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xbU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xbU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xcU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xdU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xdU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xeU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xeU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0xfU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0xfU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x10U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x10U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x11U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x11U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x12U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x12U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x13U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x13U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x14U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x14U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x15U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x15U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x16U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x16U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x17U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x17U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x18U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x18U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x19U] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x19U];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1aU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1aU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1bU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1bU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1cU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1cU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1dU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1dU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1eU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1eU];
    vlTOPp->dma_e3_write_observer__DOT__data_n[0x1fU] 
        = vlTOPp->dma_e3_write_observer__DOT__data_q
        [0x1fU];
    if (vlTOPp->aw_i) {
        vlTOPp->dma_e3_write_observer__DOT__role = 
            (4U == (IData)(vlTOPp->aw_id_i));
        vlTOPp->dma_e3_write_observer__DOT__slot = vlTOPp->dma_e3_write_observer__DOT__aw_tail_q;
        if ((1U & ((((0U != (IData)(vlTOPp->aw_id_i)) 
                     & (4U != (IData)(vlTOPp->aw_id_i))) 
                    | vlTOPp->dma_e3_write_observer__DOT__present_n
                    [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
                   | (4U == vlTOPp->dma_e3_write_observer__DOT__count_q
                      [(1U & vlTOPp->dma_e3_write_observer__DOT__role)])))) {
            vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[(7U 
                                                       & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (1U | vlTOPp->dma_e3_write_observer__DOT__present_n
               [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]);
        vlTOPp->dma_e3_write_observer__DOT__inside_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = ((6U & vlTOPp->dma_e3_write_observer__DOT__inside_n
                [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
               | (IData)(vlTOPp->active_i));
        vlTOPp->dma_e3_write_observer__DOT__stamps_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)][0U] 
            = vlTOPp->time_i;
        vlTOPp->dma_e3_write_observer__DOT__role_n[(7U 
                                                    & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (1U & vlTOPp->dma_e3_write_observer__DOT__role);
        vlTOPp->dma_e3_write_observer__DOT__order_n[(1U 
                                                     & vlTOPp->dma_e3_write_observer__DOT__role)][vlTOPp->dma_e3_write_observer__DOT__tail_q[(1U 
                                                                                & vlTOPp->dma_e3_write_observer__DOT__role)]] 
            = vlTOPp->dma_e3_write_observer__DOT__aw_tail_q;
        vlTOPp->dma_e3_write_observer__DOT__tail_n[(1U 
                                                    & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->dma_e3_write_observer__DOT__tail_q
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]));
        vlTOPp->dma_e3_write_observer__DOT__count_n[(1U 
                                                     & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (7U & ((IData)(1U) + vlTOPp->dma_e3_write_observer__DOT__count_n
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]));
        if (vlTOPp->active_i) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                        & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role))] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_q
                   [(0x1fU & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role))]);
        }
    }
    if (vlTOPp->wlast_i) {
        vlTOPp->dma_e3_write_observer__DOT__slot = vlTOPp->dma_e3_write_observer__DOT__w_tail_q;
        if ((2U & vlTOPp->dma_e3_write_observer__DOT__present_n
             [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)])) {
            vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[(7U 
                                                       & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (2U | vlTOPp->dma_e3_write_observer__DOT__present_n
               [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]);
        vlTOPp->dma_e3_write_observer__DOT__inside_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = ((5U & vlTOPp->dma_e3_write_observer__DOT__inside_n
                [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 1U));
        vlTOPp->dma_e3_write_observer__DOT__stamps_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)][1U] 
            = vlTOPp->time_i;
    }
    if (vlTOPp->b_i) {
        vlTOPp->dma_e3_write_observer__DOT__role = 
            (4U == (IData)(vlTOPp->b_id_i));
        vlTOPp->dma_e3_write_observer__DOT__slot = 
            vlTOPp->dma_e3_write_observer__DOT__order_n
            [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]
            [vlTOPp->dma_e3_write_observer__DOT__head_q
            [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]];
        if ((1U & ((((((0U != (IData)(vlTOPp->b_id_i)) 
                       & (4U != (IData)(vlTOPp->b_id_i))) 
                      | (0U == vlTOPp->dma_e3_write_observer__DOT__count_n
                         [(1U & vlTOPp->dma_e3_write_observer__DOT__role)])) 
                     | (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                        [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)])) 
                    | (~ (vlTOPp->dma_e3_write_observer__DOT__present_n
                          [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)] 
                          >> 1U))) | (vlTOPp->dma_e3_write_observer__DOT__present_n
                                      [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)] 
                                      >> 2U)))) {
            vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[(7U 
                                                       & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = (4U | vlTOPp->dma_e3_write_observer__DOT__present_n
               [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]);
        vlTOPp->dma_e3_write_observer__DOT__inside_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)] 
            = ((3U & vlTOPp->dma_e3_write_observer__DOT__inside_n
                [(7U & vlTOPp->dma_e3_write_observer__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 2U));
        vlTOPp->dma_e3_write_observer__DOT__stamps_n[(7U 
                                                      & vlTOPp->dma_e3_write_observer__DOT__slot)][2U] 
            = vlTOPp->time_i;
        vlTOPp->dma_e3_write_observer__DOT__head_n[(1U 
                                                    & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->dma_e3_write_observer__DOT__head_q
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)]));
        vlTOPp->dma_e3_write_observer__DOT__count_n[(1U 
                                                     & vlTOPp->dma_e3_write_observer__DOT__role)] 
            = (7U & (vlTOPp->dma_e3_write_observer__DOT__count_n
                     [(1U & vlTOPp->dma_e3_write_observer__DOT__role)] 
                     - (IData)(1U)));
        if (vlTOPp->active_i) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                        & ((IData)(1U) 
                                                           + 
                                                           VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role)))] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_q
                   [(0x1fU & ((IData)(1U) + VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->dma_e3_write_observer__DOT__role)))]);
        }
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [0U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [0U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [0U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [0U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [0U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [0U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [0U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [0U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [0U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [0U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[0U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[0U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [1U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [1U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [1U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [1U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [1U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [1U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [1U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [1U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [1U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [1U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[1U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[1U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [2U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [2U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [2U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [2U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [2U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [2U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [2U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [2U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [2U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [2U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[2U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[2U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [3U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [3U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [3U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [3U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [3U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [3U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [3U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [3U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [3U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [3U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[3U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[3U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [4U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [4U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [4U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [4U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [4U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [4U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [4U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [4U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [4U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [4U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[4U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[4U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [5U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [5U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [5U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [5U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [5U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [5U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [5U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [5U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [5U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [5U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[5U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[5U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [6U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [6U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [6U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [6U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [6U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [6U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [6U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [6U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [6U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [6U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[6U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[6U] = 0U;
    }
    if ((7U == vlTOPp->dma_e3_write_observer__DOT__present_n
         [7U])) {
        vlTOPp->dma_e3_write_observer__DOT__base = 
            (vlTOPp->dma_e3_write_observer__DOT__role_n
             [7U] ? 0x10U : 0U);
        if ((4U & vlTOPp->dma_e3_write_observer__DOT__inside_n
             [7U])) {
            if ((7U == vlTOPp->dma_e3_write_observer__DOT__inside_n
                 [7U])) {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(2U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                if ((vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [7U][1U] < vlTOPp->dma_e3_write_observer__DOT__stamps_n
                     [7U][0U])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(4U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
                } else {
                    vlTOPp->dma_e3_write_observer__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                           [7U][1U])) 
                           - (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                             [7U][0U])));
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(5U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = (vlTOPp->dma_e3_write_observer__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                           + vlTOPp->dma_e3_write_observer__DOT__aw_w);
                    if ((vlTOPp->dma_e3_write_observer__DOT__aw_w 
                         > vlTOPp->dma_e3_write_observer__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                        vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                    & ((IData)(6U) 
                                                                       + vlTOPp->dma_e3_write_observer__DOT__base))] 
                            = vlTOPp->dma_e3_write_observer__DOT__aw_w;
                    }
                }
                vlTOPp->dma_e3_write_observer__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [7U]
                                                                    [1U])));
                vlTOPp->dma_e3_write_observer__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->dma_e3_write_observer__DOT__stamps_n
                                                                    [7U]
                                                                    [0U])));
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(7U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__w_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__w_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(8U) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__w_b;
                }
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(9U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->dma_e3_write_observer__DOT__base))] 
                       + vlTOPp->dma_e3_write_observer__DOT__aw_b);
                if ((vlTOPp->dma_e3_write_observer__DOT__aw_b 
                     > vlTOPp->dma_e3_write_observer__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->dma_e3_write_observer__DOT__base))])) {
                    vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                                & ((IData)(0xaU) 
                                                                   + vlTOPp->dma_e3_write_observer__DOT__base))] 
                        = vlTOPp->dma_e3_write_observer__DOT__aw_b;
                }
            } else {
                vlTOPp->dma_e3_write_observer__DOT__data_n[(0x1fU 
                                                            & ((IData)(3U) 
                                                               + vlTOPp->dma_e3_write_observer__DOT__base))] 
                    = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->dma_e3_write_observer__DOT__base))]);
            }
        }
        vlTOPp->dma_e3_write_observer__DOT__present_n[7U] = 0U;
        vlTOPp->dma_e3_write_observer__DOT__inside_n[7U] = 0U;
    }
    if (vlTOPp->open_i) {
        vlTOPp->dma_e3_write_observer__DOT__data_n[0xdU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [0U]));
        vlTOPp->dma_e3_write_observer__DOT__data_n[0x1dU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [1U]));
    }
    if (vlTOPp->close_i) {
        vlTOPp->dma_e3_write_observer__DOT__data_n[0xbU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [0U]));
        vlTOPp->dma_e3_write_observer__DOT__data_n[0x1bU] 
            = (QData)((IData)(vlTOPp->dma_e3_write_observer__DOT__count_n
                              [1U]));
        vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] = 0ULL;
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [0U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [0U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [1U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [1U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [2U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [2U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [3U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [3U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [4U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [4U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [5U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [5U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [6U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [6U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->dma_e3_write_observer__DOT__present_n
                    [7U] >> 1U) & (~ vlTOPp->dma_e3_write_observer__DOT__present_n
                                   [7U])))) {
            vlTOPp->dma_e3_write_observer__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->dma_e3_write_observer__DOT__data_n
                   [0xcU]);
        }
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [1U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [1U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [2U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [2U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [3U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [3U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [4U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [4U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [5U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [5U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [6U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [6U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [7U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [7U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [8U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [8U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [9U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [9U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0xaU] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0xaU])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x10U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x10U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x11U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x11U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x12U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x12U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x13U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x13U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x14U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x14U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x15U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x15U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x16U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x16U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x17U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x17U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x18U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x18U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x19U] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x19U])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
    if ((vlTOPp->dma_e3_write_observer__DOT__data_n
         [0x1aU] < vlTOPp->dma_e3_write_observer__DOT__data_q
         [0x1aU])) {
        vlTOPp->dma_e3_write_observer__DOT__bad = 1U;
    }
}

void Vdma_e3_write_observer::_eval(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_eval\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

VL_INLINE_OPT QData Vdma_e3_write_observer::_change_request(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_change_request\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vdma_e3_write_observer::_change_request_1(Vdma_e3_write_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_change_request_1\n"); );
    Vdma_e3_write_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdma_e3_write_observer::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_e3_write_observer::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((clear_i & 0xfeU))) {
        Verilated::overWidthError("clear_i");}
    if (VL_UNLIKELY((run_i & 0xfeU))) {
        Verilated::overWidthError("run_i");}
    if (VL_UNLIKELY((active_i & 0xfeU))) {
        Verilated::overWidthError("active_i");}
    if (VL_UNLIKELY((open_i & 0xfeU))) {
        Verilated::overWidthError("open_i");}
    if (VL_UNLIKELY((close_i & 0xfeU))) {
        Verilated::overWidthError("close_i");}
    if (VL_UNLIKELY((aw_i & 0xfeU))) {
        Verilated::overWidthError("aw_i");}
    if (VL_UNLIKELY((wlast_i & 0xfeU))) {
        Verilated::overWidthError("wlast_i");}
    if (VL_UNLIKELY((b_i & 0xfeU))) {
        Verilated::overWidthError("b_i");}
    if (VL_UNLIKELY((aw_id_i & 0xf0U))) {
        Verilated::overWidthError("aw_id_i");}
    if (VL_UNLIKELY((b_id_i & 0xf0U))) {
        Verilated::overWidthError("b_id_i");}
}
#endif  // VL_DEBUG
