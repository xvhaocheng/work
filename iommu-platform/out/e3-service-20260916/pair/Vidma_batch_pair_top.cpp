// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_batch_pair_top.h for the primary calling header

#include "Vidma_batch_pair_top.h"
#include "Vidma_batch_pair_top__Syms.h"

//==========

void Vidma_batch_pair_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vidma_batch_pair_top::eval\n"); );
    Vidma_batch_pair_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vidma_batch_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/idma_batch_pair_top.sv", 4, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vidma_batch_pair_top::_eval_initial_loop(Vidma_batch_pair_top__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/idma_batch_pair_top.sv", 4, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vidma_batch_pair_top::_sequent__TOP__2(Vidma_batch_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_batch_pair_top::_sequent__TOP__2\n"); );
    Vidma_batch_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*4:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining;
    CData/*4:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v2;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v1;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v2;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v3;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v4;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v5;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v6;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v7;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v8;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v9;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v10;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v11;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v12;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v13;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v14;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v15;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v16;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v17;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v18;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v19;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v20;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v21;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v22;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v23;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v24;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v25;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v26;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v27;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v28;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v29;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v30;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v31;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v32;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v33;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v34;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v35;
    CData/*7:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0;
    CData/*7:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__reads_q__v1;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v2;
    CData/*7:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0;
    CData/*7:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__writes_q__v1;
    CData/*0:0*/ __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v2;
    CData/*0:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q;
    CData/*0:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v0;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v1;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v2;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v3;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v4;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v5;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v6;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v7;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v8;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v9;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v10;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v11;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v12;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v13;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v14;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v15;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v16;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v17;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v18;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v19;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v20;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v21;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v22;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v23;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v24;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v25;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v26;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v27;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v28;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v29;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v30;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v31;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v32;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v33;
    QData/*63:0*/ __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v34;
    QData/*32:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q;
    QData/*32:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q;
    QData/*32:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q;
    QData/*32:0*/ __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q;
    // Body
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v2 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v2 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v2 = 0U;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v0 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v1 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v2 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v3 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v4 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v5 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v6 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v7 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v8 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v9 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v10 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v11 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v12 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v13 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v14 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v15 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v16 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v17 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v18 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v19 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v20 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v21 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v22 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v23 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v24 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v25 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v26 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v27 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v28 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v29 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v30 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v31 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v32 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v33 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v34 = 0U;
    __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v35 = 0U;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q;
    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q;
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_n;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_n;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_n;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_n;
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
            [0U];
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0 = 1U;
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__writes_q__v1 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
            [1U];
    } else {
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
            [0U];
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0 = 1U;
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__reads_q__v1 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
            [1U];
    } else {
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_n
            [0U];
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0 = 1U;
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v1 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_n
            [1U];
    } else {
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_n;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_q 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_n) 
               & 1U);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_n;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d;
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q = 1ULL;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q = 1ULL;
    }
    if (vlTOPp->rst_ni) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__request))) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining = 9U;
        } else {
            if ((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining))) {
                __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining 
                    = (0x1fU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining) 
                                - (IData)(1U)));
            }
        }
    } else {
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__request))) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining = 6U;
        } else {
            if ((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining))) {
                __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining 
                    = (0x1fU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining) 
                                - (IData)(1U)));
            }
        }
    } else {
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining = 0U;
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U])) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[0U])));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U])) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[0U])));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we)
                    ? (0xfU & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U] 
                                << 0x12U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
                                             >> 0xeU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we)
                    ? (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U] 
                              << 0x14U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
                                           >> 0xcU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
                      >> 0xbU));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
                      >> 0xaU));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
                      >> 9U));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
                      >> 8U));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we)
                    ? (0xfU & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U] 
                                << 0x12U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
                                             >> 0xeU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we)
                    ? (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U] 
                              << 0x14U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
                                           >> 0xcU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
                      >> 0xbU));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
                      >> 0xaU));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
                      >> 9U));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we) 
                   & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
                      >> 8U));
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v0 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v0 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [1U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v1 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v1 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [2U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v2 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v2 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [3U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v3 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v3 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [4U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v4 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [5U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v5 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v5 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [6U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v6 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v6 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [7U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v7 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v7 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [8U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v8 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v8 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [9U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v9 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v9 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0xaU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v10 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v10 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0xbU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v11 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v11 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0xcU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v12 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v12 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0xdU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v13 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v13 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0xeU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v14 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v14 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0xfU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v15 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v15 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x10U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v16 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v16 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x11U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v17 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v17 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x12U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v18 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v18 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x13U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v19 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v19 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x14U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v20 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v20 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x15U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v21 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v21 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x16U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v22 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v22 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x17U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v23 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v23 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x18U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v24 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v24 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x19U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v25 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v25 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x1aU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v26 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v26 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x1bU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v27 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v27 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x1cU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v28 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v28 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x1dU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v29 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v29 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x1eU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v30 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v30 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x1fU];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v31 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v31 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x20U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v32 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v32 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x21U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v33 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v33 = 1U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
            [0x22U];
        __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v34 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound4;
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v34 = 1U;
    } else {
        __Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v35 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U))))) {
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [1U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [1U][0U])) 
                                                    >> 8U))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q = 0U;
                                }
                            }
                        } else {
                            if ((0x90U != (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [1U][2U] 
                                            >> 8U)))) {
                                if ((0x98U != (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U)))) {
                                    if ((0xd0U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [1U][2U] 
                                                      >> 8U)))) {
                                        if ((0U == 
                                             ((vlTOPp->idma_batch_pair_top__DOT__req
                                               [1U][2U] 
                                               << 0x18U) 
                                              | (vlTOPp->idma_batch_pair_top__DOT__req
                                                 [1U][1U] 
                                                 >> 8U)))) {
                                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q 
                                                = (
                                                   (vlTOPp->idma_batch_pair_top__DOT__req
                                                    [1U][1U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [1U][0U] 
                                                      >> 8U));
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
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U != (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))) {
                            if ((0x90U != (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [1U][2U] 
                                            >> 8U)))) {
                                if ((0x98U == (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U)))) {
                                    if (((0ULL < (((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [1U][2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [1U][1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [1U][0U])) 
                                                        >> 8U)))) 
                                         & (0x1000ULL 
                                            >= (((QData)((IData)(
                                                                 vlTOPp->idma_batch_pair_top__DOT__req
                                                                 [1U][2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [1U][1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [1U][0U])) 
                                                      >> 8U)))))) {
                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q 
                                            = (0x1fffU 
                                               & ((vlTOPp->idma_batch_pair_top__DOT__req
                                                   [1U][1U] 
                                                   << 0x18U) 
                                                  | (vlTOPp->idma_batch_pair_top__DOT__req
                                                     [1U][0U] 
                                                     >> 8U)));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U != (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))) {
                            if ((0x90U == (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [1U][2U] 
                                            >> 8U)))) {
                                if (((0ULL < (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [1U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [1U][0U])) 
                                                    >> 8U)))) 
                                     & (0x100000000ULL 
                                        >= (((QData)((IData)(
                                                             vlTOPp->idma_batch_pair_top__DOT__req
                                                             [1U][2U])) 
                                             << 0x38U) 
                                            | (((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][1U])) 
                                                << 0x18U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][0U])) 
                                                  >> 8U)))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q 
                                        = (0x1ffffffffULL 
                                           & (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [1U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [1U][0U])) 
                                                    >> 8U))));
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__locked_write) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
        }
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x10U))) {
            if ((0xffU != (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][0U]))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
            } else {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
                    } else {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U))))) {
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [1U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [1U][0U])) 
                                                    >> 8U))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 0U;
                                }
                            } else {
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
                            }
                        } else {
                            if ((0x90U == (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [1U][2U] 
                                            >> 8U)))) {
                                if ((1U & (~ ((0ULL 
                                               < (((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [1U][2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [1U][1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [1U][0U])) 
                                                        >> 8U)))) 
                                              & (0x100000000ULL 
                                                 >= 
                                                 (((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [1U][2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [1U][1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [1U][0U])) 
                                                        >> 8U)))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
                                }
                            } else {
                                if ((0x98U == (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U)))) {
                                    if ((1U & (~ ((0ULL 
                                                   < 
                                                   (((QData)((IData)(
                                                                     vlTOPp->idma_batch_pair_top__DOT__req
                                                                     [1U][2U])) 
                                                     << 0x38U) 
                                                    | (((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [1U][1U])) 
                                                        << 0x18U) 
                                                       | ((QData)((IData)(
                                                                          vlTOPp->idma_batch_pair_top__DOT__req
                                                                          [1U][0U])) 
                                                          >> 8U)))) 
                                                  & (0x1000ULL 
                                                     >= 
                                                     (((QData)((IData)(
                                                                       vlTOPp->idma_batch_pair_top__DOT__req
                                                                       [1U][2U])) 
                                                       << 0x38U) 
                                                      | (((QData)((IData)(
                                                                          vlTOPp->idma_batch_pair_top__DOT__req
                                                                          [1U][1U])) 
                                                          << 0x18U) 
                                                         | ((QData)((IData)(
                                                                            vlTOPp->idma_batch_pair_top__DOT__req
                                                                            [1U][0U])) 
                                                            >> 8U)))))))) {
                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
                                    }
                                } else {
                                    if ((0xd0U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [1U][2U] 
                                                      >> 8U)))) {
                                        if ((0U != 
                                             ((vlTOPp->idma_batch_pair_top__DOT__req
                                               [1U][2U] 
                                               << 0x18U) 
                                              | (vlTOPp->idma_batch_pair_top__DOT__req
                                                 [1U][1U] 
                                                 >> 8U)))) {
                                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
                                        }
                                    } else {
                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 1U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [0U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [0U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U))))) {
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [0U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [0U][0U])) 
                                                    >> 8U))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q = 0U;
                                }
                            }
                        } else {
                            if ((0x90U != (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U)))) {
                                if ((0x98U != (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U)))) {
                                    if ((0xd0U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][2U] 
                                                      >> 8U)))) {
                                        if ((0U == 
                                             ((vlTOPp->idma_batch_pair_top__DOT__req
                                               [0U][2U] 
                                               << 0x18U) 
                                              | (vlTOPp->idma_batch_pair_top__DOT__req
                                                 [0U][1U] 
                                                 >> 8U)))) {
                                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q 
                                                = (
                                                   (vlTOPp->idma_batch_pair_top__DOT__req
                                                    [0U][1U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][0U] 
                                                      >> 8U));
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
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [0U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [0U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U != (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) {
                            if ((0x90U != (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U)))) {
                                if ((0x98U == (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U)))) {
                                    if (((0ULL < (((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [0U][2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [0U][1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [0U][0U])) 
                                                        >> 8U)))) 
                                         & (0x1000ULL 
                                            >= (((QData)((IData)(
                                                                 vlTOPp->idma_batch_pair_top__DOT__req
                                                                 [0U][2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [0U][1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [0U][0U])) 
                                                      >> 8U)))))) {
                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q 
                                            = (0x1fffU 
                                               & ((vlTOPp->idma_batch_pair_top__DOT__req
                                                   [0U][1U] 
                                                   << 0x18U) 
                                                  | (vlTOPp->idma_batch_pair_top__DOT__req
                                                     [0U][0U] 
                                                     >> 8U)));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [0U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [0U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U != (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) {
                            if ((0x90U == (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U)))) {
                                if (((0ULL < (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [0U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [0U][0U])) 
                                                    >> 8U)))) 
                                     & (0x100000000ULL 
                                        >= (((QData)((IData)(
                                                             vlTOPp->idma_batch_pair_top__DOT__req
                                                             [0U][2U])) 
                                             << 0x38U) 
                                            | (((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][1U])) 
                                                << 0x18U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][0U])) 
                                                  >> 8U)))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q 
                                        = (0x1ffffffffULL 
                                           & (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [0U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [0U][0U])) 
                                                    >> 8U))));
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__locked_write) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
        }
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [0U][2U] >> 0x10U))) {
            if ((0xffU != (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [0U][0U]))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
            } else {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
                    } else {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U))))) {
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [0U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [0U][0U])) 
                                                    >> 8U))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 0U;
                                }
                            } else {
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
                            }
                        } else {
                            if ((0x90U == (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U)))) {
                                if ((1U & (~ ((0ULL 
                                               < (((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [0U][2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [0U][1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [0U][0U])) 
                                                        >> 8U)))) 
                                              & (0x100000000ULL 
                                                 >= 
                                                 (((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [0U][2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                                      [0U][1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [0U][0U])) 
                                                        >> 8U)))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
                                }
                            } else {
                                if ((0x98U == (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U)))) {
                                    if ((1U & (~ ((0ULL 
                                                   < 
                                                   (((QData)((IData)(
                                                                     vlTOPp->idma_batch_pair_top__DOT__req
                                                                     [0U][2U])) 
                                                     << 0x38U) 
                                                    | (((QData)((IData)(
                                                                        vlTOPp->idma_batch_pair_top__DOT__req
                                                                        [0U][1U])) 
                                                        << 0x18U) 
                                                       | ((QData)((IData)(
                                                                          vlTOPp->idma_batch_pair_top__DOT__req
                                                                          [0U][0U])) 
                                                          >> 8U)))) 
                                                  & (0x1000ULL 
                                                     >= 
                                                     (((QData)((IData)(
                                                                       vlTOPp->idma_batch_pair_top__DOT__req
                                                                       [0U][2U])) 
                                                       << 0x38U) 
                                                      | (((QData)((IData)(
                                                                          vlTOPp->idma_batch_pair_top__DOT__req
                                                                          [0U][1U])) 
                                                          << 0x18U) 
                                                         | ((QData)((IData)(
                                                                            vlTOPp->idma_batch_pair_top__DOT__req
                                                                            [0U][0U])) 
                                                            >> 8U)))))))) {
                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
                                    }
                                } else {
                                    if ((0xd0U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][2U] 
                                                      >> 8U)))) {
                                        if ((0U != 
                                             ((vlTOPp->idma_batch_pair_top__DOT__req
                                               [0U][2U] 
                                               << 0x18U) 
                                              | (vlTOPp->idma_batch_pair_top__DOT__req
                                                 [0U][1U] 
                                                 >> 8U)))) {
                                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
                                        }
                                    } else {
                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 1U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
             & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue))) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q = 0ULL;
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q = 0ULL;
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q = 0U;
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q = 0U;
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_id_q 
                = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
        }
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_issue) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q 
                = (0x1ffffffffULL & (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q));
        }
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_done) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q 
                = (0x1ffffffffULL & (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q));
            if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q = 1U;
            }
        }
        if ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
              & (4U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) 
             & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q) 
                | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_write)))) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q = 1U;
        }
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][0U]))) {
                if (((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                         [1U][2U] >> 8U))) 
                     & (2ULL == (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U])) 
                                  << 0x38U) | (((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][1U])) 
                                                << 0x18U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][0U])) 
                                                  >> 8U)))))) {
                    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q = 1U;
                } else {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U))))) {
                                __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q = 0U;
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q = 0U;
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [1U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [1U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [1U][0U])) 
                                                    >> 8U))))) {
                                    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q = 0ULL;
                                    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q = 0ULL;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q = 0ULL;
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q = 0ULL;
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_id_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
             & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue))) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q = 0ULL;
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q = 0ULL;
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q = 0U;
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q = 0U;
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_id_q 
                = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
        }
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_issue) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q 
                = (0x1ffffffffULL & (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q));
        }
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_done) {
            __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q 
                = (0x1ffffffffULL & (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q));
            if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q = 1U;
            }
        }
        if ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
              & (4U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) 
             & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q) 
                | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_write)))) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q = 1U;
        }
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [0U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [0U][0U]))) {
                if (((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                         [0U][2U] >> 8U))) 
                     & (2ULL == (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U])) 
                                  << 0x38U) | (((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][1U])) 
                                                << 0x18U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][0U])) 
                                                  >> 8U)))))) {
                    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q = 1U;
                } else {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U))))) {
                                __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q = 0U;
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q = 0U;
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__req
                                                               [0U][2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_batch_pair_top__DOT__req
                                                                  [0U][1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__req
                                                                    [0U][0U])) 
                                                    >> 8U))))) {
                                    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q = 0ULL;
                                    __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q = 0ULL;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q = 0ULL;
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q = 0ULL;
        __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q = 0U;
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_id_q = 0ULL;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q[0U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__writes_q__v0;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q[1U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__writes_q__v1;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__writes_q__v2) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q[0U] = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q[1U] = 0U;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q[0U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__reads_q__v0;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q[1U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__reads_q__v1;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__reads_q__v2) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q[0U] = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q[1U] = 0U;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_q[0U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v0;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_q[1U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v1;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__lane_id_q__v2) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_q[0U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_q[1U] = 0ULL;
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining;
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v0) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v0;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v1) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[1U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v1;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v2) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[2U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v2;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v3) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[3U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v3;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v4) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[4U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v4;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v5) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[5U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v5;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v6) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[6U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v6;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v7) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[7U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v7;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v8) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[8U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v8;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v9) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[9U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v9;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v10) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xaU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v10;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v11) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xbU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v11;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v12) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xcU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v12;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v13) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xdU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v13;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v14) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xeU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v14;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v15) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xfU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v15;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v16) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x10U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v16;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v17) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x11U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v17;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v18) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x12U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v18;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v19) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x13U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v19;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v20) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x14U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v20;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v21) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x15U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v21;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v22) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x16U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v22;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v23) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x17U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v23;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v24) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x18U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v24;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v25) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x19U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v25;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v26) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1aU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v26;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v27) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1bU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v27;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v28) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1cU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v28;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v29) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1dU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v29;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v30) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1eU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v30;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v31) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1fU] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v31;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v32) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x20U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v32;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v33) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x21U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v33;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v34) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x22U] 
            = __Vdlyvval__idma_batch_pair_top__DOT__obs__DOT__count_q__v34;
    }
    if (__Vdlyvset__idma_batch_pair_top__DOT__obs__DOT__count_q__v35) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[1U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[2U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[3U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[4U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[5U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[6U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[7U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[8U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[9U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xaU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xbU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xcU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xdU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xeU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0xfU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x10U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x11U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x12U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x13U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x14U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x15U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x16U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x17U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x18U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x19U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1aU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1bU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1cU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1dU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1eU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x1fU] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x20U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x21U] = 0ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q[0x22U] = 0ULL;
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q 
        = __Vdly__idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_n 
        = (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_q 
           + (QData)((IData)((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining)))));
    vlTOPp->idma_batch_pair_top__DOT__complete = ((1U 
                                                   & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete)) 
                                                  | ((1U 
                                                      == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining)) 
                                                     << 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_n 
        = (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_q 
           + (QData)((IData)((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining)))));
    vlTOPp->idma_batch_pair_top__DOT__complete = ((2U 
                                                   & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete)) 
                                                  | (1U 
                                                     == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining)));
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__active 
        = (((2U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)) 
            | (3U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
           | (4U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q 
        = ((IData)(vlTOPp->rst_ni) ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d
            : 2ULL);
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [1U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [1U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [1U][0U])) 
                                                >> 8U))))) {
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q 
                                    = (1U & (vlTOPp->idma_batch_pair_top__DOT__req
                                             [1U][0U] 
                                             >> 8U));
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q = 0U;
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q 
        = ((IData)(vlTOPp->rst_ni) ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d
            : 2ULL);
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [0U][2U] >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
                           [0U][0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__req
                                                           [0U][2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__req
                                                              [0U][1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__req
                                                                [0U][0U])) 
                                                >> 8U))))) {
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q 
                                    = (1U & (vlTOPp->idma_batch_pair_top__DOT__req
                                             [0U][0U] 
                                             >> 8U));
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q = 0U;
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__active) 
           | (6U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)));
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) {
        vlTOPp->idma_batch_pair_top__DOT__done_id[1U] 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q);
        vlTOPp->idma_batch_pair_top__DOT__issue_id[1U] 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q);
    } else {
        vlTOPp->idma_batch_pair_top__DOT__done_id[1U] 
            = ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                ? 2ULL : (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q));
        vlTOPp->idma_batch_pair_top__DOT__issue_id[1U] 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n)
            : 0U);
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) {
        vlTOPp->idma_batch_pair_top__DOT__done_id[0U] 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q);
        vlTOPp->idma_batch_pair_top__DOT__issue_id[0U] 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q);
    } else {
        vlTOPp->idma_batch_pair_top__DOT__done_id[0U] 
            = ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                ? 2ULL : (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q));
        vlTOPp->idma_batch_pair_top__DOT__issue_id[0U] 
            = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n)
            : 0U);
    vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__done_id_i[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__done_id
        [0U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__done_id_i[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__done_id
        [1U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__issue_id
        [0U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__issue_id
        [1U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_cancel_i) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_n = 0U;
                }
            } else {
                if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__valid_o = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__valid_o = 1U;
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
           & (0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_cancel_i) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_n = 0U;
                }
            } else {
                if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__valid_o = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__valid_o = 1U;
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
           & (0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)));
    vlTOPp->idma_batch_pair_top__DOT__request = ((1U 
                                                  & (IData)(vlTOPp->idma_batch_pair_top__DOT__request)) 
                                                 | ((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__valid_o) 
                                                    << 1U));
    vlTOPp->running_o = ((1U & (IData)(vlTOPp->running_o)) 
                         | ((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o) 
                            << 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_done 
        = (((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o) 
            & (2U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) 
           & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
              >> 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_issue 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o) 
           & (1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)));
    vlTOPp->idma_batch_pair_top__DOT__request = ((2U 
                                                  & (IData)(vlTOPp->idma_batch_pair_top__DOT__request)) 
                                                 | (IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__valid_o));
    vlTOPp->running_o = ((2U & (IData)(vlTOPp->running_o)) 
                         | (IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_done 
        = (((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o) 
            & (2U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) 
           & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_issue 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o) 
           & (1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)));
}

VL_INLINE_OPT void Vidma_batch_pair_top::_combo__TOP__3(Vidma_batch_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_batch_pair_top::_combo__TOP__3\n"); );
    Vidma_batch_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*223:0*/ __Vtemp284[7];
    WData/*287:0*/ __Vtemp285[9];
    WData/*287:0*/ __Vtemp286[9];
    WData/*351:0*/ __Vtemp287[11];
    WData/*223:0*/ __Vtemp299[7];
    WData/*287:0*/ __Vtemp300[9];
    WData/*287:0*/ __Vtemp301[9];
    WData/*351:0*/ __Vtemp302[11];
    // Body
    vlTOPp->idma_batch_pair_top__DOT__req[0U][0U] = 
        (0xffU | (0xffffff00U & ((IData)(vlTOPp->wdata_i) 
                                 << 8U)));
    vlTOPp->idma_batch_pair_top__DOT__req[0U][1U] = 
        ((0xffU & ((IData)(vlTOPp->wdata_i) >> 0x18U)) 
         | (0xffffff00U & ((IData)((vlTOPp->wdata_i 
                                    >> 0x20U)) << 8U)));
    vlTOPp->idma_batch_pair_top__DOT__req[0U][2U] = 
        ((0xfffe0000U & (((IData)(vlTOPp->valid_i) 
                          & (~ (IData)(vlTOPp->port_i))) 
                         << 0x11U)) | ((0xffff0000U 
                                        & ((IData)(vlTOPp->write_i) 
                                           << 0x10U)) 
                                       | ((0xffffff00U 
                                           & ((IData)(vlTOPp->addr_i) 
                                              << 8U)) 
                                          | (0xffU 
                                             & ((IData)(
                                                        (vlTOPp->wdata_i 
                                                         >> 0x20U)) 
                                                >> 0x18U)))));
    vlTOPp->idma_batch_pair_top__DOT__req[1U][0U] = 
        (0xffU | (0xffffff00U & ((IData)(vlTOPp->wdata_i) 
                                 << 8U)));
    vlTOPp->idma_batch_pair_top__DOT__req[1U][1U] = 
        ((0xffU & ((IData)(vlTOPp->wdata_i) >> 0x18U)) 
         | (0xffffff00U & ((IData)((vlTOPp->wdata_i 
                                    >> 0x20U)) << 8U)));
    vlTOPp->idma_batch_pair_top__DOT__req[1U][2U] = 
        ((0xfffe0000U & (((IData)(vlTOPp->valid_i) 
                          & (IData)(vlTOPp->port_i)) 
                         << 0x11U)) | ((0xffff0000U 
                                        & ((IData)(vlTOPp->write_i) 
                                           << 0x10U)) 
                                       | ((0xffffff00U 
                                           & ((IData)(vlTOPp->addr_i) 
                                              << 8U)) 
                                          | (0xffU 
                                             & ((IData)(
                                                        (vlTOPp->wdata_i 
                                                         >> 0x20U)) 
                                                >> 0x18U)))));
    vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb[0U] 
        = (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
           [0U][0U]);
    vlTOPp->idma_batch_pair_top__DOT__cfg_wdata[0U] 
        = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                            [0U][2U])) << 0x38U) | 
           (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                             [0U][1U])) << 0x18U) | 
            ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                             [0U][0U])) >> 8U)));
    vlTOPp->idma_batch_pair_top__DOT__cfg_write = (
                                                   (2U 
                                                    & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)) 
                                                   | (1U 
                                                      & (vlTOPp->idma_batch_pair_top__DOT__req
                                                         [0U][2U] 
                                                         >> 0x10U)));
    vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb[1U] 
        = (0xffU & vlTOPp->idma_batch_pair_top__DOT__req
           [1U][0U]);
    vlTOPp->idma_batch_pair_top__DOT__cfg_wdata[1U] 
        = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                            [1U][2U])) << 0x38U) | 
           (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                             [1U][1U])) << 0x18U) | 
            ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                             [1U][0U])) >> 8U)));
    vlTOPp->idma_batch_pair_top__DOT__cfg_write = (
                                                   (1U 
                                                    & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)) 
                                                   | (2U 
                                                      & (vlTOPp->idma_batch_pair_top__DOT__req
                                                         [1U][2U] 
                                                         >> 0xfU)));
    vlTOPp->idma_batch_pair_top__DOT__cfg_addr[0U] 
        = (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__req
                    [0U][2U] >> 8U));
    vlTOPp->idma_batch_pair_top__DOT__cfg_addr[1U] 
        = (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__req
                    [1U][2U] >> 8U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access 
        = (1U & ((vlTOPp->idma_batch_pair_top__DOT__req
                  [1U][2U] >> 0x11U) & (vlTOPp->idma_batch_pair_top__DOT__req
                                        [1U][2U] >> 0xfU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access 
        = (1U & ((vlTOPp->idma_batch_pair_top__DOT__req
                  [0U][2U] >> 0x11U) & (vlTOPp->idma_batch_pair_top__DOT__req
                                        [0U][2U] >> 0xfU)));
    vlTOPp->idma_batch_pair_top__DOT__cfg_rdata = 0ULL;
    if ((0x40U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
         [0U])) {
        if ((0x20U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
             [0U])) {
            if ((0x10U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((8U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                  [0U] >> 2U)))) {
                        if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                      [0U] >> 1U)))) {
                            if ((1U & (~ vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                       [0U]))) {
                                if (VL_GTS_III(1,32,32, 0x23U, (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q))) {
                                    vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                        = ((0x22U >= 
                                            (0x3fU 
                                             & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q)))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q))]
                                            : 0ULL);
                                }
                            }
                        }
                    }
                } else {
                    if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                  [0U] >> 2U)))) {
                        if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                      [0U] >> 1U)))) {
                            if ((1U & (~ vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                       [0U]))) {
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q));
                            }
                        }
                    }
                }
            } else {
                if ((8U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                  [0U] >> 2U)))) {
                        if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                      [0U] >> 1U)))) {
                            if ((1U & (~ vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                       [0U]))) {
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q;
                            }
                        }
                    }
                } else {
                    if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                  [0U] >> 2U)))) {
                        if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                      [0U] >> 1U)))) {
                            if ((1U & (~ vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                       [0U]))) {
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = ((0xfffffffffffffffeULL 
                                        & vlTOPp->idma_batch_pair_top__DOT__cfg_rdata) 
                                       | (IData)((IData)(
                                                         (1U 
                                                          == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))));
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = ((0xfffffffffffffffdULL 
                                        & vlTOPp->idma_batch_pair_top__DOT__cfg_rdata) 
                                       | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy)) 
                                          << 1U));
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = ((0xfffffffffffffffbULL 
                                        & vlTOPp->idma_batch_pair_top__DOT__cfg_rdata) 
                                       | ((QData)((IData)(
                                                          (5U 
                                                           == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))) 
                                          << 2U));
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = ((0xffffffffULL 
                                        & vlTOPp->idma_batch_pair_top__DOT__cfg_rdata) 
                                       | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_q)) 
                                          << 0x20U));
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = ((0xffffffbfffffffffULL 
                                        & vlTOPp->idma_batch_pair_top__DOT__cfg_rdata) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_q 
                                                              >> 6U)))) 
                                          << 0x26U));
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                                    = ((0xffffff7fffffffffULL 
                                        & vlTOPp->idma_batch_pair_top__DOT__cfg_rdata) 
                                       | ((QData)((IData)(
                                                          (1U 
                                                           & ((vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_q 
                                                               >> 7U) 
                                                              | ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_q) 
                                                                 & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_error_i)))))) 
                                          << 0x27U));
                            }
                        }
                    }
                }
            }
        } else {
            if ((0x10U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((8U & vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                  [0U] >> 2U)))) {
                        if ((1U & (~ (vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                      [0U] >> 1U)))) {
                            if ((1U & (~ vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                                       [0U]))) {
                                vlTOPp->idma_batch_pair_top__DOT__cfg_rdata = 0x49444d414f425302ULL;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_write 
        = (((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) 
              & (vlTOPp->idma_batch_pair_top__DOT__req
                 [1U][2U] >> 0x10U)) & (0xffU == (0xffU 
                                                  & vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][0U]))) 
            & (0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                  [1U][2U] >> 8U)))) 
           & (2ULL == (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                                        [1U][2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                      [1U][1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [1U][0U])) 
                                                   >> 8U)))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__locked_write 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o) 
             & (vlTOPp->idma_batch_pair_top__DOT__req
                [1U][2U] >> 0x11U)) & (vlTOPp->idma_batch_pair_top__DOT__req
                                       [1U][2U] >> 0x10U)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_write 
        = (((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) 
              & (vlTOPp->idma_batch_pair_top__DOT__req
                 [0U][2U] >> 0x10U)) & (0xffU == (0xffU 
                                                  & vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][0U]))) 
            & (0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                  [0U][2U] >> 8U)))) 
           & (2ULL == (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__req
                                        [0U][2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                                   [0U][0U])) 
                                                   >> 8U)))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access 
        = (((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access)) 
            & (vlTOPp->idma_batch_pair_top__DOT__req
               [0U][2U] >> 0x11U)) & (((((0x58U == 
                                          (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U))) 
                                         | (0x60U == 
                                            (0xffU 
                                             & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))) 
                                        | (0x68U == 
                                           (0xffU & 
                                            (vlTOPp->idma_batch_pair_top__DOT__req
                                             [0U][2U] 
                                             >> 8U)))) 
                                       | (0x70U == 
                                          (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U)))) 
                                      | (0x78U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][2U] 
                                                      >> 8U)))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__req[1U][0U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__req[1U][1U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
        = vlTOPp->idma_batch_pair_top__DOT__req[1U][2U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
        = ((0x1ffffU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U]) 
           | (0x20000U & ((vlTOPp->idma_batch_pair_top__DOT__req
                           [1U][2U] & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access)) 
                                       << 0x11U)) & 
                          ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__locked_write)) 
                           << 0x11U))));
    vlTOPp->idma_batch_pair_top__DOT__cfg_valid = (
                                                   (2U 
                                                    & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid)) 
                                                   | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__locked_write 
        = (((((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o) 
              & (vlTOPp->idma_batch_pair_top__DOT__req
                 [0U][2U] >> 0x11U)) & (vlTOPp->idma_batch_pair_top__DOT__req
                                        [0U][2U] >> 0x10U)) 
            & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access))) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                  >> 0x11U) & (~ (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 0x10U))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                  >> 0x11U) & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                               >> 0x10U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                              >> 8U))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                               >> 8U))) << 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 2U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 3U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 4U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 5U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 6U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 7U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 8U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 9U));
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x23ULL > vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                         [0U])) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__index_n 
                            = (0x1ffU & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                [0U]));
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if (((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy)) 
                     & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_n 
                        = vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                        [0U];
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 0x19U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_n 
                                        = (3U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 0x10U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL == vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy) 
                                              | (1U 
                                                 == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_n = 0U;
                                }
                            } else {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 0x1dU)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 0x18U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_n 
                                        = (3U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 8U)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL == vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy) 
                                              | (1U 
                                                 == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_n = 0U;
                                }
                            } else {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__e3_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 0x1bU)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q;
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            if ((0x68U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((0x70U != vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL != vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if ((4ULL == vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy) 
                                              | (1U 
                                                 == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_n = 0U;
                                }
                            } else {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_n 
                                        = (1U & (IData)(
                                                        (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                         [0U] 
                                                         >> 0x1eU)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__req[0U][0U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__req[0U][1U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
        = vlTOPp->idma_batch_pair_top__DOT__req[0U][2U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
        = ((0x1ffffU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U]) 
           | (0x20000U & (((vlTOPp->idma_batch_pair_top__DOT__req
                            [0U][2U] & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access)) 
                                        << 0x11U)) 
                           & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access)) 
                              << 0x11U)) & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__locked_write)) 
                                            << 0x11U))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__base_req[0U]))))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                  >> 0x11U) & (~ (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 0x10U))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                  >> 0x11U) & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                               >> 0x10U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                              >> 8U))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                               >> 8U))) << 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 2U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 3U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 4U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 5U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 6U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 7U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 8U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[2U] 
                                  >> 8U))) << 9U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err));
    __Vtemp284[0U] = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp284[1U] = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                              >> 0x20U));
    __Vtemp284[2U] = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp284[3U] = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                              >> 0x20U));
    __Vtemp284[4U] = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp284[5U] = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                              >> 0x20U));
    __Vtemp284[6U] = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 6U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re)) 
                      & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp285, __Vtemp284);
    __Vtemp286[0U] = __Vtemp285[0U];
    __Vtemp286[1U] = __Vtemp285[1U];
    __Vtemp286[2U] = __Vtemp285[2U];
    __Vtemp286[3U] = __Vtemp285[3U];
    __Vtemp286[4U] = __Vtemp285[4U];
    __Vtemp286[5U] = __Vtemp285[5U];
    __Vtemp286[6U] = __Vtemp285[6U];
    __Vtemp286[7U] = __Vtemp285[7U];
    __Vtemp286[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                       >> 4U) & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                 << 1U)) 
                                     & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                        << 1U))) | 
                      __Vtemp285[8U]);
    VL_EXTEND_WW(322,258, __Vtemp287, __Vtemp286);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0U] 
        = __Vtemp287[0U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[1U] 
        = __Vtemp287[1U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[2U] 
        = __Vtemp287[2U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[3U] 
        = __Vtemp287[3U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[4U] 
        = __Vtemp287[4U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[5U] 
        = __Vtemp287[5U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[6U] 
        = __Vtemp287[6U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[7U] 
        = __Vtemp287[7U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[8U] 
        = __Vtemp287[8U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[9U] 
        = __Vtemp287[9U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp287[0xaU])))))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__base_req[0U]))))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q) 
                              | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_write))))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                         & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q)))))) {
                            if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                 & (0U != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q))) {
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_n 
                                    = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_write))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_cancel_i) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire = 1U;
                }
            } else {
                if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q)))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q)))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q)) 
                  & (0ULL != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err));
    __Vtemp299[0U] = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp299[1U] = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                              >> 0x20U));
    __Vtemp299[2U] = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp299[3U] = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                              >> 0x20U));
    __Vtemp299[4U] = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp299[5U] = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                              >> 0x20U));
    __Vtemp299[6U] = ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 6U) & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re)) 
                      & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp300, __Vtemp299);
    __Vtemp301[0U] = __Vtemp300[0U];
    __Vtemp301[1U] = __Vtemp300[1U];
    __Vtemp301[2U] = __Vtemp300[2U];
    __Vtemp301[3U] = __Vtemp300[3U];
    __Vtemp301[4U] = __Vtemp300[4U];
    __Vtemp301[5U] = __Vtemp300[5U];
    __Vtemp301[6U] = __Vtemp300[6U];
    __Vtemp301[7U] = __Vtemp300[7U];
    __Vtemp301[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                       >> 4U) & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                 << 1U)) 
                                     & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                        << 1U))) | 
                      __Vtemp300[8U]);
    VL_EXTEND_WW(322,258, __Vtemp302, __Vtemp301);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0U] 
        = __Vtemp302[0U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[1U] 
        = __Vtemp302[1U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[2U] 
        = __Vtemp302[2U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[3U] 
        = __Vtemp302[3U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[4U] 
        = __Vtemp302[4U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[5U] 
        = __Vtemp302[5U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[6U] 
        = __Vtemp302[6U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[7U] 
        = __Vtemp302[7U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[8U] 
        = __Vtemp302[8U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[9U] 
        = __Vtemp302[9U];
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp302[0xaU])))))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__id_retire) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__done = ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__done)) 
                                              | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)
                                                   ? (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_done)
                                                   : (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__trans_complete)) 
                                                 << 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_n 
                    = (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q 
                       - 1ULL);
            } else {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_n 
                                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)
                                    ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q
                                    : (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[1U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0U]))));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 0U;
            } else {
                if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_write))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_countdown_q)) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 1U;
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_cancel_i) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 0U;
                } else {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_go_i) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 1U;
                    }
                }
            } else {
                if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n 
                        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                        & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stop_q)))
                                     ? 0U : (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                              & (0U 
                                                 != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q))
                                              ? 4U : 1U)));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 2U;
            } else {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_n = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U]) 
           | (0xfffffffeU & ((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__remaining)) 
                             << 1U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U] 
                                = ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U]) 
                                   | (0xfffffffeU & 
                                      ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                       << 1U)));
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[5U] 
                                = ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                          >> 0x1fU)) 
                                   | (0xfffffffeU & 
                                      ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                >> 0x20U)) 
                                       << 1U)));
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U] 
                                = ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U]) 
                                   | (1U & ((IData)(
                                                    (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                     >> 0x20U)) 
                                            >> 0x1fU)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q) 
                              | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_write))))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                         & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q)))))) {
                            if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                 & (0U != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q))) {
                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_n 
                                    = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_write))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_cancel_i) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire = 1U;
                }
            } else {
                if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q)))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q)))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q)) 
                  & (0ULL != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__issue = ((1U 
                                                & (IData)(vlTOPp->idma_batch_pair_top__DOT__issue)) 
                                               | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)
                                                    ? (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_task_issue)
                                                    : (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__issue)) 
                                                  << 1U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__id_retire) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__done = ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__done)) 
                                              | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)
                                                  ? (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_done)
                                                  : (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__trans_complete)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_n 
                    = (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q 
                       - 1ULL);
            } else {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_n 
                                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)
                                    ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q
                                    : (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[1U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0U]))));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 0U;
            } else {
                if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_write))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_countdown_q)) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 1U;
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_cancel_i) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 0U;
                } else {
                    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_go_i) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 1U;
                    }
                }
            } else {
                if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n 
                        = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                        & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stop_q)))
                                     ? 0U : (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                              & (0U 
                                                 != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q))
                                              ? 4U : 1U)));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 2U;
            } else {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_n = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U]) 
           | (0xfffffffeU & ((0U != (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__remaining)) 
                             << 1U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U] 
                                = ((1U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U]) 
                                   | (0xfffffffeU & 
                                      ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                       << 1U)));
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[5U] 
                                = ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                          >> 0x1fU)) 
                                   | (0xfffffffeU & 
                                      ((IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                >> 0x20U)) 
                                       << 1U)));
                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U] 
                                = ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U]) 
                                   | (1U & ((IData)(
                                                    (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                     >> 0x20U)) 
                                            >> 0x1fU)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                   >> 0x20U));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__issue = ((2U 
                                                & (IData)(vlTOPp->idma_batch_pair_top__DOT__issue)) 
                                               | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)
                                                   ? (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_task_issue)
                                                   : (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__issue)));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_read 
        = (((vlTOPp->idma_batch_pair_top__DOT__req[1U][2U] 
             >> 0x11U) & (~ (vlTOPp->idma_batch_pair_top__DOT__req
                             [1U][2U] >> 0x10U))) & 
           (0x50U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                               [1U][2U] >> 8U))));
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_read) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_q);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__active_cycles_q 
                       >> 0x20U));
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__locked_write) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[1U] = 0U;
        if (((((((((0x80U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                       [1U][2U] >> 8U))) 
                   | (0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                         [1U][2U] >> 8U)))) 
                  | (0x90U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                        [1U][2U] >> 8U)))) 
                 | (0x98U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                       [1U][2U] >> 8U)))) 
                | (0xa0U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                      [1U][2U] >> 8U)))) 
               | (0xa8U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                     [1U][2U] >> 8U)))) 
              | (0xd0U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                    [1U][2U] >> 8U)))) 
             | (0xd8U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                   [1U][2U] >> 8U))))) {
            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
                = (IData)(((0x80U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                               [1U][2U] 
                                               >> 8U)))
                            ? 0x49444d4142544301ULL
                            : ((0x88U == (0xffU & (
                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                   [1U][2U] 
                                                   >> 8U)))
                                ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q)) 
                                    << 0x20U) | (QData)((IData)(
                                                                (((((0ULL 
                                                                     != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q) 
                                                                    & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q 
                                                                       == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q)) 
                                                                   & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o))) 
                                                                  << 3U) 
                                                                 | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q) 
                                                                     << 2U) 
                                                                    | (((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o) 
                                                                        << 1U) 
                                                                       | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)))))))
                                : ((0x90U == (0xffU 
                                              & (vlTOPp->idma_batch_pair_top__DOT__req
                                                 [1U][2U] 
                                                 >> 8U)))
                                    ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q
                                    : ((0x98U == (0xffU 
                                                  & (vlTOPp->idma_batch_pair_top__DOT__req
                                                     [1U][2U] 
                                                     >> 8U)))
                                        ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q))
                                        : ((0xa0U == 
                                            (0xffU 
                                             & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))
                                            ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q
                                            : ((0xa8U 
                                                == 
                                                (0xffU 
                                                 & (vlTOPp->idma_batch_pair_top__DOT__req
                                                    [1U][2U] 
                                                    >> 8U)))
                                                ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q
                                                : (
                                                   (0xd0U 
                                                    == 
                                                    (0xffU 
                                                     & (vlTOPp->idma_batch_pair_top__DOT__req
                                                        [1U][2U] 
                                                        >> 8U)))
                                                    ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q))
                                                    : 0x49444d4147415001ULL))))))));
            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
                = (IData)((((0x80U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [1U][2U] 
                                                >> 8U)))
                             ? 0x49444d4142544301ULL
                             : ((0x88U == (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [1U][2U] 
                                            >> 8U)))
                                 ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_error_q)) 
                                     << 0x20U) | (QData)((IData)(
                                                                 (((((0ULL 
                                                                      != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q) 
                                                                     & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q 
                                                                        == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q)) 
                                                                    & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o))) 
                                                                   << 3U) 
                                                                  | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_stopped_q) 
                                                                      << 2U) 
                                                                     | (((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__batch_active_o) 
                                                                         << 1U) 
                                                                        | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)))))))
                                 : ((0x90U == (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [1U][2U] 
                                                  >> 8U)))
                                     ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_target_q
                                     : ((0x98U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [1U][2U] 
                                                      >> 8U)))
                                         ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_pages_q))
                                         : ((0xa0U 
                                             == (0xffU 
                                                 & (vlTOPp->idma_batch_pair_top__DOT__req
                                                    [1U][2U] 
                                                    >> 8U)))
                                             ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q
                                             : ((0xa8U 
                                                 == 
                                                 (0xffU 
                                                  & (vlTOPp->idma_batch_pair_top__DOT__req
                                                     [1U][2U] 
                                                     >> 8U)))
                                                 ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_issued_q
                                                 : 
                                                ((0xd0U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [1U][2U] 
                                                      >> 8U)))
                                                  ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__gap_cycles_q))
                                                  : 0x49444d4147415001ULL))))))) 
                           >> 0x20U));
        } else {
            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                = (1U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        }
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q) 
                         & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_cfg_ok)))) {
                        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                            = (1U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
                    } else {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                                = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q;
    if (((1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)) 
         & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
            >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_n 
            = vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i
            [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))];
    }
    if (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__active) {
        if ((2U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
            if ((3U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
                if ((4U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
                    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                               >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_n 
                            = vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i
                            [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))];
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next);
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                   >> 0x20U));
    vlTOPp->idma_batch_pair_top__DOT__rsp[1U][0U] = 
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[0U];
    vlTOPp->idma_batch_pair_top__DOT__rsp[1U][1U] = 
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[1U];
    vlTOPp->idma_batch_pair_top__DOT__rsp[1U][2U] = 
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U];
    vlTOPp->idma_batch_pair_top__DOT__poll = ((1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__poll)) 
                                              | (0xfffeU 
                                                 & (((((vlTOPp->idma_batch_pair_top__DOT__req
                                                        [1U][2U] 
                                                        >> 0x10U) 
                                                       & ((~ 
                                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                                            [1U][2U] 
                                                            >> 0x10U)) 
                                                          << 1U)) 
                                                      & ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_access)) 
                                                         << 1U)) 
                                                     & ((0x30U 
                                                         == 
                                                         (0xffU 
                                                          & (vlTOPp->idma_batch_pair_top__DOT__req
                                                             [1U][2U] 
                                                             >> 8U))) 
                                                        << 1U)) 
                                                    & vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[2U])));
    vlTOPp->idma_batch_pair_top__DOT__poll_id[1U] = 
        ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_enable_q)
          ? (((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__state_q)) 
              & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                 == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_id_q))
              ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__1__KET____DOT__lane__DOT__batch_completed_q
              : 0ULL) : (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[1U])) 
                          << 0x20U) | (QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__1__KET____DOT__lane__dma_ctrl_rsp_o[0U]))));
    vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_read 
        = (((vlTOPp->idma_batch_pair_top__DOT__req[0U][2U] 
             >> 0x11U) & (~ (vlTOPp->idma_batch_pair_top__DOT__req
                             [0U][2U] >> 0x10U))) & 
           (0x50U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                               [0U][2U] >> 8U))));
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_read) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_q);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__active_cycles_q 
                       >> 0x20U));
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_rdata);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->idma_batch_pair_top__DOT__cfg_rdata 
                       >> 0x20U));
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__locked_write) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access) {
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U] = 0U;
        if (((((((((0x80U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                       [0U][2U] >> 8U))) 
                   | (0x88U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                         [0U][2U] >> 8U)))) 
                  | (0x90U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                        [0U][2U] >> 8U)))) 
                 | (0x98U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                       [0U][2U] >> 8U)))) 
                | (0xa0U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                      [0U][2U] >> 8U)))) 
               | (0xa8U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                     [0U][2U] >> 8U)))) 
              | (0xd0U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                    [0U][2U] >> 8U)))) 
             | (0xd8U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                   [0U][2U] >> 8U))))) {
            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U] 
                = (IData)(((0x80U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                               [0U][2U] 
                                               >> 8U)))
                            ? 0x49444d4142544301ULL
                            : ((0x88U == (0xffU & (
                                                   vlTOPp->idma_batch_pair_top__DOT__req
                                                   [0U][2U] 
                                                   >> 8U)))
                                ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q)) 
                                    << 0x20U) | (QData)((IData)(
                                                                (((((0ULL 
                                                                     != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q) 
                                                                    & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q 
                                                                       == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q)) 
                                                                   & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o))) 
                                                                  << 3U) 
                                                                 | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q) 
                                                                     << 2U) 
                                                                    | (((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o) 
                                                                        << 1U) 
                                                                       | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)))))))
                                : ((0x90U == (0xffU 
                                              & (vlTOPp->idma_batch_pair_top__DOT__req
                                                 [0U][2U] 
                                                 >> 8U)))
                                    ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q
                                    : ((0x98U == (0xffU 
                                                  & (vlTOPp->idma_batch_pair_top__DOT__req
                                                     [0U][2U] 
                                                     >> 8U)))
                                        ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q))
                                        : ((0xa0U == 
                                            (0xffU 
                                             & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))
                                            ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q
                                            : ((0xa8U 
                                                == 
                                                (0xffU 
                                                 & (vlTOPp->idma_batch_pair_top__DOT__req
                                                    [0U][2U] 
                                                    >> 8U)))
                                                ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q
                                                : (
                                                   (0xd0U 
                                                    == 
                                                    (0xffU 
                                                     & (vlTOPp->idma_batch_pair_top__DOT__req
                                                        [0U][2U] 
                                                        >> 8U)))
                                                    ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q))
                                                    : 0x49444d4147415001ULL))))))));
            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U] 
                = (IData)((((0x80U == (0xffU & (vlTOPp->idma_batch_pair_top__DOT__req
                                                [0U][2U] 
                                                >> 8U)))
                             ? 0x49444d4142544301ULL
                             : ((0x88U == (0xffU & 
                                           (vlTOPp->idma_batch_pair_top__DOT__req
                                            [0U][2U] 
                                            >> 8U)))
                                 ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_error_q)) 
                                     << 0x20U) | (QData)((IData)(
                                                                 (((((0ULL 
                                                                      != vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q) 
                                                                     & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q 
                                                                        == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q)) 
                                                                    & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o))) 
                                                                   << 3U) 
                                                                  | (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_stopped_q) 
                                                                      << 2U) 
                                                                     | (((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__batch_active_o) 
                                                                         << 1U) 
                                                                        | (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)))))))
                                 : ((0x90U == (0xffU 
                                               & (vlTOPp->idma_batch_pair_top__DOT__req
                                                  [0U][2U] 
                                                  >> 8U)))
                                     ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_target_q
                                     : ((0x98U == (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][2U] 
                                                      >> 8U)))
                                         ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_pages_q))
                                         : ((0xa0U 
                                             == (0xffU 
                                                 & (vlTOPp->idma_batch_pair_top__DOT__req
                                                    [0U][2U] 
                                                    >> 8U)))
                                             ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q
                                             : ((0xa8U 
                                                 == 
                                                 (0xffU 
                                                  & (vlTOPp->idma_batch_pair_top__DOT__req
                                                     [0U][2U] 
                                                     >> 8U)))
                                                 ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_issued_q
                                                 : 
                                                ((0xd0U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][2U] 
                                                      >> 8U)))
                                                  ? (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__gap_cycles_q))
                                                  : 0x49444d4147415001ULL))))))) 
                           >> 0x20U));
        } else {
            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                = (1U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
        }
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[8U])) {
                    if (((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q) 
                         & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_cfg_ok)))) {
                        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                            = (1U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
                    } else {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                                = (2U | vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U]);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__rsp[0U][0U] = 
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U];
    vlTOPp->idma_batch_pair_top__DOT__rsp[0U][1U] = 
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U];
    vlTOPp->idma_batch_pair_top__DOT__rsp[0U][2U] = 
        vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U];
    vlTOPp->idma_batch_pair_top__DOT__poll = ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__poll)) 
                                              | ((((((vlTOPp->idma_batch_pair_top__DOT__req
                                                      [0U][2U] 
                                                      >> 0x11U) 
                                                     & (~ 
                                                        (vlTOPp->idma_batch_pair_top__DOT__req
                                                         [0U][2U] 
                                                         >> 0x10U))) 
                                                    & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_access))) 
                                                   & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__observer_access))) 
                                                  & (0x30U 
                                                     == 
                                                     (0xffU 
                                                      & (vlTOPp->idma_batch_pair_top__DOT__req
                                                         [0U][2U] 
                                                         >> 8U)))) 
                                                 & (vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[2U] 
                                                    >> 1U)));
    vlTOPp->idma_batch_pair_top__DOT__poll_id[0U] = 
        ((IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_enable_q)
          ? (((0U == (IData)(vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__state_q)) 
              & (vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                 == vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_id_q))
              ? vlTOPp->idma_batch_pair_top__DOT__genblk1__BRA__0__KET____DOT__lane__DOT__batch_completed_q
              : 0ULL) : (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[1U])) 
                          << 0x20U) | (QData)((IData)(
                                                      vlTOPp->idma_batch_pair_top__DOT____Vcellout__genblk1__BRA__0__KET____DOT__lane__dma_ctrl_rsp_o[0U]))));
    vlTOPp->ready_o = (1U & (vlTOPp->idma_batch_pair_top__DOT__rsp
                             [vlTOPp->port_i][2U] >> 1U));
    vlTOPp->error_o = (1U & vlTOPp->idma_batch_pair_top__DOT__rsp
                       [vlTOPp->port_i][2U]);
    vlTOPp->rdata_o = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__rsp
                                        [vlTOPp->port_i][1U])) 
                        << 0x20U) | (QData)((IData)(
                                                    vlTOPp->idma_batch_pair_top__DOT__rsp
                                                    [vlTOPp->port_i][0U])));
    vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__poll_id_i[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__poll_id
        [0U];
    vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__poll_id_i[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__poll_id
        [1U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [1U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [2U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[2U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [3U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[3U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [4U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[4U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [5U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[5U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [6U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[6U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [7U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[7U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [8U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[8U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [9U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[9U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0xaU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xaU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0xbU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xbU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0xcU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xcU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0xdU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xdU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0xeU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xeU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0xfU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xfU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x10U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x10U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x11U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x11U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x12U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x12U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x13U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x13U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x14U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x14U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x15U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x15U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x16U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x16U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x17U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x17U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x18U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x18U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x19U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x19U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x1aU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1aU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x1bU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1bU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x1cU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1cU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x1dU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1dU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x1eU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1eU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x1fU];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1fU] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x20U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x20U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x21U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x21U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
        [0x22U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x22U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound2;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_n[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_q
        [0U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_n[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_q
        [1U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_q;
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
        [0U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n[0U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
        [0U];
    if ((2U == ((2U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                       << 1U)) | (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n[0U] 
            = (0xffU & ((IData)(1U) + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                        [0U]));
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
             [0U])) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                = (1U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                           << 1U)) | (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n[0U] 
                = (0xffU & (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                            [0U] - (IData)(1U)));
            if ((0U == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                 [0U])) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                    = (1U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
            }
        }
    }
    if ((2U == ((2U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                       << 1U)) | (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n[0U] 
            = (0xffU & ((IData)(1U) + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                        [0U]));
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
             [0U])) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                = (1U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                           << 1U)) | (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__complete))))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n[0U] 
                = (0xffU & (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                            [0U] - (IData)(1U)));
            if ((0U == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                 [0U])) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                    = (1U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
        [1U];
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n[1U] 
        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
        [1U];
    if ((2U == ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__issue)) 
                | (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                         >> 1U))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n[1U] 
            = (0xffU & ((IData)(1U) + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                        [1U]));
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
             [1U])) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                = (2U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__issue)) 
                    | (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                             >> 1U))))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n[1U] 
                = (0xffU & (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                            [1U] - (IData)(1U)));
            if ((0U == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                 [1U])) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                    = (2U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
            }
        }
    }
    if ((2U == ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__issue)) 
                | (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                         >> 1U))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n[1U] 
            = (0xffU & ((IData)(1U) + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                        [1U]));
        if ((0xffU == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
             [1U])) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                = (2U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
        }
    } else {
        if ((1U == ((2U & (IData)(vlTOPp->idma_batch_pair_top__DOT__issue)) 
                    | (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                             >> 1U))))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n[1U] 
                = (0xffU & (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                            [1U] - (IData)(1U)));
            if ((0U == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                 [1U])) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n 
                    = (2U | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n));
            }
        }
    }
    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__a_issue_evt = 0U;
    if (((1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)) 
         & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
            >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 2U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[7U] = 1ULL;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n = 0ULL;
        if ((0ULL == vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i
             [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
        if (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_n 
                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                   & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))));
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_n = 0U;
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_n 
                = (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                    & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n) 
                       >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))))
                    ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_n
                   [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))]
                    : 0ULL);
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_n 
                = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__a_issue_evt;
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_n 
                = (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                    & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n) 
                       >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q)))) 
                   & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__a_issue_evt)));
        }
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1eU] 
            = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
                                [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])) 
                << 0x10U) | (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
                                            [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])));
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1fU] 
            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q)
                ? (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
                                    [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])) 
                    << 0x10U) | (QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
                                                [(1U 
                                                  & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])))
                : 0ULL);
    }
    if (((1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)) 
         | (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__active))) {
        if ((1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n) 
                    >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                   | ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                      & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__track_bad_n) 
                         >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))))))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (8U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
    }
    if ((((((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__finite_q)) 
            & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_q)) 
           & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__active) 
              | ((1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)) 
                 & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                    >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))))) 
          & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q)) 
         & (~ ((IData)(vlTOPp->running_o) >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q)))))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
            = (0x20U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
    }
    if ((6U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__0__slot = 0x22U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
               [0x22U]);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__0__slot)] 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
        if ((0xffffffffffffffffULL == ((0x22U >= (0x3fU 
                                                  & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__0__slot))
                                        ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                       [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__0__slot)]
                                        : 0ULL))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
        if ((1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                    >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                   | ((IData)(vlTOPp->idma_batch_pair_top__DOT__done) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
        if ((((IData)(vlTOPp->idma_batch_pair_top__DOT__poll) 
              >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
             & (vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__poll_id_i
                [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))] 
                == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[9U] = 1ULL;
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 5U;
        }
    }
    if (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__active) {
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__1__slot = 0U;
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
               [0U]);
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__1__slot)] 
            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
        if ((0xffffffffffffffffULL == ((0x22U >= (0x3fU 
                                                  & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__1__slot))
                                        ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                       [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__1__slot)]
                                        : 0ULL))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n 
            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_q);
        if ((0xffffffffffffffffULL == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_q)) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot 
            = ((IData)(0xaU) + ((((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                                  & ((((0U != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                                        [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))]) 
                                       | (0U != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                                          [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])) 
                                      | ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                                         >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q)))) 
                                     | ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                                        >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))))) 
                                 << 1U) | (1U & (((
                                                   (0U 
                                                    != 
                                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_q
                                                    [
                                                    (1U 
                                                     & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))]) 
                                                   | (0U 
                                                      != 
                                                      vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_q
                                                      [
                                                      (1U 
                                                       & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])) 
                                                  | ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                                                     >> 
                                                     (1U 
                                                      & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))) 
                                                 | ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                                                    >> 
                                                    (1U 
                                                     & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))))));
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
            = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot))
                        ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                       [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot)]
                        : 0ULL));
        if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot)] 
                = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
        }
        if ((0xffffffffffffffffULL == ((0x22U >= (0x3fU 
                                                  & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot))
                                        ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                       [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__2__slot)]
                                        : 0ULL))) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__enable 
            = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                     & (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))));
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot = 0xeU;
        if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__enable) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot))
                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot)]
                            : 0ULL));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__enable 
            = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                     & (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))));
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot = 0xfU;
        if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__enable) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot))
                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot)]
                            : 0ULL));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__enable 
            = (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                     >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))));
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot = 0x10U;
        if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__enable) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot))
                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot)]
                            : 0ULL));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__enable 
            = (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                     >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))));
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot = 0x11U;
        if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__enable) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot))
                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot)]
                            : 0ULL));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__enable 
            = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                     & (~ (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))));
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot = 0x12U;
        if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__enable) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot))
                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot)]
                            : 0ULL));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
        }
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__enable 
            = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                     & (~ (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))));
        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot = 0x13U;
        if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__enable) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot))
                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot)]
                            : 0ULL));
            if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            }
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
        }
        if (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) {
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__enable 
                = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                          >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))) 
                         & (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q)))));
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot = 0x14U;
            if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__enable) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot))
                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot)]
                                : 0ULL));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__3__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__enable 
                = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                          >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))) 
                         & (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q)))));
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot = 0x15U;
            if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__enable) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot))
                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot)]
                                : 0ULL));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__4__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__enable 
                = (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                         >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))));
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot = 0x16U;
            if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__enable) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot))
                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot)]
                                : 0ULL));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__5__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__enable 
                = (1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__complete) 
                         >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))));
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot = 0x17U;
            if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__enable) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot))
                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot)]
                                : 0ULL));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__6__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__enable 
                = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                          >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))) 
                         & (~ (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))))));
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot = 0x18U;
            if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__enable) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot))
                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot)]
                                : 0ULL));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__7__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            }
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__enable 
                = (1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                          >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))) 
                         & (~ (3U >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))))));
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot = 0x19U;
            if (vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__enable) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot))
                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot)]
                                : 0ULL));
                if ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                }
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__8__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            }
        }
        if ((2U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
            vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__13__slot = 1U;
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                   [1U]);
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__13__slot)] 
                = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
            if ((0xffffffffffffffffULL == ((0x22U >= 
                                            (0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__13__slot))
                                            ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                           [(0x3fU 
                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__13__slot)]
                                            : 0ULL))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
            if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                       >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                    = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
            }
            if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__done) 
                       >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
                if ((vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__done_id_i
                     [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))] 
                     != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q)) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
                vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__14__slot = 8U;
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                       [8U]);
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__14__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__14__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__14__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
                if ((vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n 
                     > vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                     [4U])) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[4U] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n;
                }
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n = 0ULL;
                if (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__batch_q) {
                    if ((vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
                         [8U] == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 6U;
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x20U] 
                            = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
                                                [(1U 
                                                  & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])) 
                                << 0x10U) | (QData)((IData)(
                                                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
                                                            [
                                                            (1U 
                                                             & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])));
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x21U] 
                            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q)
                                ? (((QData)((IData)(
                                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
                                                    [
                                                    (1U 
                                                     & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])) 
                                    << 0x10U) | (QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
                                                                [
                                                                (1U 
                                                                 & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])))
                                : 0ULL);
                        if (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q) {
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_n = 1U;
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_n 
                                = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                                   & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n) 
                                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))));
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_n 
                                = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
                                [0U];
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_n 
                                = (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q) 
                                    & ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_task_n) 
                                       >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))))
                                    ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__lane_id_n
                                   [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))]
                                    : 0ULL);
                        }
                    } else {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 4U;
                    }
                } else {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 3U;
                }
            }
        } else {
            if ((3U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
                vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__15__slot = 2U;
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                    = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                       [2U]);
                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__15__slot)] 
                    = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                if ((0xffffffffffffffffULL == ((0x22U 
                                                >= 
                                                (0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__15__slot))
                                                ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                               [(0x3fU 
                                                 & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__15__slot)]
                                                : 0ULL))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
                if ((1U & (((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                            >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                           | ((IData)(vlTOPp->idma_batch_pair_top__DOT__done) 
                              >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q)))))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
                if ((((IData)(vlTOPp->idma_batch_pair_top__DOT__poll) 
                      >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))) 
                     & (vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__poll_id_i
                        [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))] 
                        == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q))) {
                    vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__16__slot = 9U;
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                        = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [9U]);
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__16__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                    if ((0xffffffffffffffffULL == (
                                                   (0x22U 
                                                    >= 
                                                    (0x3fU 
                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__16__slot))
                                                    ? 
                                                   vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                                   [
                                                   (0x3fU 
                                                    & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__16__slot)]
                                                    : 0ULL))) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                            = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                    }
                    if ((vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n 
                         > vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                         [5U])) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[5U] 
                            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n;
                    }
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n = 0ULL;
                    if ((vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n
                         [9U] == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 5U;
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x20U] 
                            = (((QData)((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
                                                [(1U 
                                                  & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])) 
                                << 0x10U) | (QData)((IData)(
                                                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
                                                            [
                                                            (1U 
                                                             & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))])));
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x21U] 
                            = ((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__has_a_q)
                                ? (((QData)((IData)(
                                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__reads_n
                                                    [
                                                    (1U 
                                                     & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])) 
                                    << 0x10U) | (QData)((IData)(
                                                                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__writes_n
                                                                [
                                                                (1U 
                                                                 & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__aggressor_q))])))
                                : 0ULL);
                    } else {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 4U;
                    }
                }
            } else {
                if ((4U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) {
                    vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__17__slot = 3U;
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                        = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                           [3U]);
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                         & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__17__slot)] 
                        = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                    if ((0xffffffffffffffffULL == (
                                                   (0x22U 
                                                    >= 
                                                    (0x3fU 
                                                     & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__17__slot))
                                                    ? 
                                                   vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                                   [
                                                   (0x3fU 
                                                    & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__17__slot)]
                                                    : 0ULL))) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                            = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                    }
                    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__done) 
                               >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                            = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                    }
                    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__issue) 
                               >> (1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))))) {
                        if (((0ULL == vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i
                              [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))]) 
                             | (vlTOPp->idma_batch_pair_top__DOT____Vcellinp__obs__issue_id_i
                                [(1U & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__victim_q))] 
                                == vlTOPp->idma_batch_pair_top__DOT__obs__DOT__expected_q))) {
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                                = (2U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                        }
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 2U;
                        vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__18__slot = 7U;
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1 
                            = (1ULL + vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                               [7U]);
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[(0x3fU 
                                                                             & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__18__slot)] 
                            = vlTOPp->idma_batch_pair_top__DOT__obs__DOT____Vlvbound1;
                        if ((0xffffffffffffffffULL 
                             == ((0x22U >= (0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__18__slot))
                                  ? vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                                 [(0x3fU & vlTOPp->__Vtask_idma_batch_pair_top__DOT__obs__DOT__bump__18__slot)]
                                  : 0ULL))) {
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                                = (4U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                        }
                        if ((vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n 
                             > vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_q
                             [6U])) {
                            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[6U] 
                                = vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n;
                        }
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n = 0ULL;
                    }
                }
            }
        }
    }
    if ((1U & ((IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_valid) 
               & (IData)(vlTOPp->idma_batch_pair_top__DOT__cfg_write)))) {
        if ((0xffU != vlTOPp->idma_batch_pair_top__DOT__cfg_wstrb
             [0U])) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                = (1U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
        } else {
            if ((0x68U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                 [0U])) {
                if ((1U & (~ ((~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy)) 
                              & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))))) {
                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                        = (1U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                }
            } else {
                if ((0x70U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                     [0U])) {
                    if ((0x23ULL <= vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                         [0U])) {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                            = (1U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                    }
                } else {
                    if ((0x60U == vlTOPp->idma_batch_pair_top__DOT__cfg_addr
                         [0U])) {
                        if ((2ULL == vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                             [0U])) {
                            if (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy) 
                                 | (1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))) {
                                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 5U;
                                vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                                    = (0x10U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                                if (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q) 
                                     & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_q)))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n 
                                        = (0x20U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n);
                                }
                            }
                        } else {
                            if ((4ULL == vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                 [0U])) {
                                if (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy) 
                                     | (1U == (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q)))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                                        = (1U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                                } else {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n = 0U;
                                }
                            } else {
                                if (((((((((((((((0ULL 
                                                  == 
                                                  (0xffffffffe0fcfcfeULL 
                                                   & vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                   [0U])) 
                                                 & (IData)(
                                                           vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U])) 
                                                & (~ (IData)(
                                                             (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                              [0U] 
                                                              >> 0x1eU)))) 
                                               & (~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1dU)))) 
                                              & ((~ (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x1cU))) 
                                                 | (IData)(
                                                           (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                            [0U] 
                                                            >> 0x1bU)))) 
                                             & (~ (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 0x1bU)))) 
                                            & (~ (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 0x1aU)))) 
                                           & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__busy))) 
                                          & (1U != (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_q))) 
                                         & (0ULL != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                        & (0x100000000ULL 
                                           >= vlTOPp->idma_batch_pair_top__DOT__obs__DOT__target_q)) 
                                       & (2U > (3U 
                                                & (IData)(
                                                          (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                           [0U] 
                                                           >> 8U))))) 
                                      & (2U > (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))) 
                                     & ((~ (IData)(
                                                   (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                    [0U] 
                                                    >> 0x18U))) 
                                        | ((3U & (IData)(
                                                         (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                          [0U] 
                                                          >> 8U))) 
                                           != (3U & (IData)(
                                                            (vlTOPp->idma_batch_pair_top__DOT__cfg_wdata
                                                             [0U] 
                                                             >> 0x10U))))))) {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__phase_n = 1U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__segment_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_carry_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_start_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_end_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_issue_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_open_id_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_close_id_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_cycles_n = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n = 0U;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[1U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[2U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[3U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[4U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[5U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[6U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[7U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[8U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[9U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xaU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xbU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xcU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xdU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xeU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0xfU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x10U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x11U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x12U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x13U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x14U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x15U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x16U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x17U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x18U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x19U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1aU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1bU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1cU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1dU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1eU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x1fU] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x20U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x21U] = 0ULL;
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__count_n[0x22U] = 0ULL;
                                } else {
                                    vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                                        = (1U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                                }
                            }
                        }
                    } else {
                        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
                            = (1U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
                    }
                }
            }
        }
    }
    if (((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q) 
         & (~ (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_q)))) {
        if ((0x10U & vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n)) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n 
                = (0x10U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n);
        }
        if ((4U & vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n)) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n 
                = (8U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n);
        }
        if ((8U & vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n)) {
            vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n 
                = (0x80U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n);
        }
    }
    if ((((IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_q) 
          & (vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
             >> 4U)) & (IData)(vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_closed_q))) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
            = (0x100U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
    }
    if ((0U != vlTOPp->idma_batch_pair_top__DOT__obs__DOT__p4m_error_n)) {
        vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n 
            = (0x100U | vlTOPp->idma_batch_pair_top__DOT__obs__DOT__error_n);
    }
}

void Vidma_batch_pair_top::_eval(Vidma_batch_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_batch_pair_top::_eval\n"); );
    Vidma_batch_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vidma_batch_pair_top::_change_request(Vidma_batch_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_batch_pair_top::_change_request\n"); );
    Vidma_batch_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vidma_batch_pair_top::_change_request_1(Vidma_batch_pair_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_batch_pair_top::_change_request_1\n"); );
    Vidma_batch_pair_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vidma_batch_pair_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_batch_pair_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((valid_i & 0xfeU))) {
        Verilated::overWidthError("valid_i");}
    if (VL_UNLIKELY((write_i & 0xfeU))) {
        Verilated::overWidthError("write_i");}
    if (VL_UNLIKELY((port_i & 0xfeU))) {
        Verilated::overWidthError("port_i");}
}
#endif  // VL_DEBUG
