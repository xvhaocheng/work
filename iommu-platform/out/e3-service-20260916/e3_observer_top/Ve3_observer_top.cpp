// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Ve3_observer_top.h for the primary calling header

#include "Ve3_observer_top.h"
#include "Ve3_observer_top__Syms.h"

//==========

void Ve3_observer_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Ve3_observer_top::eval\n"); );
    Ve3_observer_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/e3_observer_top.sv", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Ve3_observer_top::_eval_initial_loop(Ve3_observer_top__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/e3_observer_top.sv", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Ve3_observer_top::_sequent__TOP__1(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_sequent__TOP__1\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v16 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v16 = 0U;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__run_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__run_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__inserted_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__inserted_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__triggers_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__misses_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__hits_q__v4 = 0U;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__time_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__current_lane_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v5 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v6 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v7 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v5 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v6 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v7 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v1 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v2 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v3 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v24 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v32 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v33 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v73 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v75 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v78 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v79 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v5 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v6 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v7 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v4 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v5 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v6 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v7 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v8 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v9 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v10 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v11 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v64 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v66 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v68 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v70 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v72 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v74 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v76 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v78 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v80 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v82 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v84 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v86 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v88 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v90 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v92 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v94 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v96 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v98 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v100 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v102 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v105 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v108 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v110 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v112 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v0 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32 = 0U;
    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v64 = 0U;
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__delay_q 
                = vlTOPp->delay128_i;
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__delay_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_n;
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n;
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__write_error = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__write_error 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad;
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__write_error = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x80000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x40000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x20000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x10000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x4000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x2000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x1000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x800U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x200U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x100U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x80U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x40U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x10U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((8U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((2U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x40000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x20000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x10000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x8000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x2000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x1000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x800U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x400U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x100U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x80U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x40U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((0x20U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if ((8U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if ((2U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if ((1U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 = 1U;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->active_i;
                        vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
                        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
                            = (3U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i)))) {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [0U][0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v1 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [0U][1U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [0U][2U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v3 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [0U][3U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v4 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [1U][0U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v5 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [1U][1U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v6 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [1U][2U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v7 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
                    [1U][3U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v8 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i)))) {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v1 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [1U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [2U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v3 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [3U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v4 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [4U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v5 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [5U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v6 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [6U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v7 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
                    [7U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v8 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v3 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                    [1U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v3 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n
                    [1U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v3 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n
                    [1U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v4 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v9 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [1U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v10 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [2U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v11 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [3U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v12 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [4U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v13 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [5U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v14 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [6U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v15 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [7U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v16 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8 = 1U;
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v9 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [1U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v10 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [2U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v11 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [3U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v12 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [4U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v13 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [5U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v14 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [6U];
                vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v15 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                    [7U];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v16 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->open_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->close_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next;
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                            = (0xffffffffffffULL & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next);
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                    = (0xffffffffffffULL 
                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next);
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
                                     > vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q)) {
                                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
                                        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x40000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x20000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x10000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x8000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x2000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x1000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
}

VL_INLINE_OPT void Ve3_observer_top::_sequent__TOP__2(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_sequent__TOP__2\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*1:0*/ __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v0;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v4;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v4;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v4;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v5;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v65;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v67;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v69;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v71;
    CData/*1:0*/ __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v1;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v5;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v5;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v6;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v7;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v73;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v75;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v77;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v79;
    CData/*1:0*/ __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v2;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v6;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v6;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v8;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v9;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v81;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v83;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v85;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v87;
    CData/*1:0*/ __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v3;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v7;
    CData/*1:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v7;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v10;
    CData/*2:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v11;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v89;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v91;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v93;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v95;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v97;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v74;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v99;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v101;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v76;
    CData/*1:0*/ __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__hits_q__v4;
    CData/*1:0*/ __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__misses_q__v4;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v103;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v106;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v79;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v109;
    CData/*5:0*/ __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v111;
    WData/*127:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[4];
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v4;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v4;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v4;
    WData/*127:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[4];
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v5;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v5;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v5;
    WData/*127:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[4];
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v6;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v6;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v6;
    WData/*127:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[4];
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v7;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v7;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v7;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__hits_q__v4;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__misses_q__v4;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22;
    IData/*31:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v64;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v66;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v68;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v70;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v72;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v74;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v76;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v78;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v80;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v82;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v84;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v86;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v88;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v90;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v92;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v94;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v64;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v65;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v66;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v67;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v69;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v70;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v72;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v96;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v73;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v98;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v100;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v75;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v77;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v102;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v104;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v105;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v107;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v78;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v108;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v110;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v112;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v113;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v114;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v115;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v116;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v117;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v118;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v119;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v120;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v121;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v122;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v123;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v124;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v125;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v126;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v127;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v128;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v129;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v130;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v131;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v132;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v133;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v134;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v135;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v136;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v137;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v138;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v139;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v140;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v141;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v142;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v143;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v144;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v147;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v149;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v33;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v34;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v35;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v36;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v37;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v38;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v39;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v40;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v41;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v42;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v43;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v44;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v45;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v46;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v47;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v48;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v49;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v50;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v51;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v52;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v53;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v54;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v55;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v56;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v57;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v58;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v59;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v60;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v61;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v62;
    QData/*63:0*/ __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v63;
    // Body
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x800U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x400U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x100U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x80U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x40U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((0x20U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((8U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((2U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                }
                if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad)))) {
                    if (vlTOPp->active_i) {
                        if ((1U & (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
                                           >> 0x30U)))) {
                            vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                        }
                        if ((1U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q);
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q)) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                            }
                        }
                        if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched) {
                            if (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole) {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q);
                                if ((1U & ((0xffffffffU 
                                            == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q) 
                                           | (IData)(
                                                     (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
                                                      >> 0x30U))))) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            } else {
                                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
                                    = ((IData)(1U) 
                                       + vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q);
                                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q)) {
                                    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i)))) {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [0U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [0U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [0U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [1U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [1U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [1U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [2U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [2U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [2U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [3U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [3U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [3U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [4U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [4U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [4U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [5U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [5U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [5U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [6U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [6U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [6U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [7U][0U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [7U][1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22 = 1U;
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                    [7U][2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound2;
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23 = 1U;
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v24 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i))) {
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__run_q 
                = vlTOPp->arm_i;
            vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__time_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__current_lane_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__triggers_q = 0U;
            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__inserted_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__capture_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__available_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__consume_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__available_seen_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__consume_seen_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__extra_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__level_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__pte_q = 0ULL;
            vlTOPp->e3_observer_top__DOT__dut__DOT__state_at_capture_q = 0U;
            vlTOPp->e3_observer_top__DOT__dut__DOT__waiting_q = 0U;
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__time_q 
                    = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__time_q);
                if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__time_q)) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                }
                if (((IData)(vlTOPp->close_i) | (IData)(vlTOPp->abort_i))) {
                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__run_q = 0U;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q 
                    = ((2U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q)) 
                       | (1U & ((IData)(vlTOPp->out_valid_i) 
                                & (~ (IData)(vlTOPp->out_ready_i)))));
                if ((((((IData)(vlTOPp->tr_fire_i) 
                        & (0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                           [0U])) & (4U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                     [0U])) | (((IData)(vlTOPp->out_valid_i) 
                                                & (0U 
                                                   != 
                                                   vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                                                   [0U])) 
                                               & (4U 
                                                  != 
                                                  vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                                                  [0U]))) 
                     | (((IData)(vlTOPp->return_i) 
                         & (0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                            [0U])) & (4U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                      [0U])))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q 
                    = ((1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q)) 
                       | (2U & ((IData)(vlTOPp->out_valid_i) 
                                & ((~ ((IData)(vlTOPp->out_ready_i) 
                                       >> 1U)) << 1U))));
                if (((((((IData)(vlTOPp->tr_fire_i) 
                         >> 1U) & (0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                   [1U])) & (4U != 
                                             vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                             [1U])) 
                      | ((((IData)(vlTOPp->out_valid_i) 
                           >> 1U) & (0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                                     [1U])) & (4U != 
                                               vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                                               [1U]))) 
                     | ((((IData)(vlTOPp->return_i) 
                          >> 1U) & (0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                    [1U])) & (4U != 
                                              vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                              [1U])))) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                }
                if ((1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[0U] 
                        = (IData)(vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                  [0U]);
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[1U] 
                        = (IData)((vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                   [0U] >> 0x20U));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[2U] 
                        = (IData)((((QData)((IData)(
                                                    ((IData)(1U) 
                                                     + 
                                                     vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                     [0U]))) 
                                    << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[3U] 
                        = (IData)(((((QData)((IData)(
                                                     ((IData)(1U) 
                                                      + 
                                                      vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                      [0U]))) 
                                     << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))) 
                                   >> 0x20U));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v0 = 1U;
                    __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v0 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                        [0U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v4 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                                 [0U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v4 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                           [0U]);
                    if (((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                          [0U]) | ((4U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                    [0U]) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v4 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                 [0U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v4 = 1U;
                    if (((0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                          [0U]) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((2U == ((2U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                   << 1U)) | (1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v4 
                        = (7U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                 [0U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v4 = 1U;
                } else {
                    if ((1U == ((2U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                       << 1U)) | (1U 
                                                  & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v5 
                            = (7U & (vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                     [0U] - (IData)(1U)));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v5 = 1U;
                    }
                }
                if ((0x10U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v4 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                           [0U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v4 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                         [0U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [0U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v64 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v64 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v65 = 5U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [0U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v66 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v66 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v67 = 5U;
                    }
                }
                if (((IData)(vlTOPp->return_i) & (0U 
                                                  == 
                                                  vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                                  [0U]))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v4 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                           [0U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v4 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                         [0U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [0U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v68 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v68 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v69 = 6U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [0U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v70 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v70 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v71 = 6U;
                    }
                }
                if ((2U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[0U] 
                        = (IData)(vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                  [1U]);
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[1U] 
                        = (IData)((vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                   [1U] >> 0x20U));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[2U] 
                        = (IData)((((QData)((IData)(
                                                    ((IData)(1U) 
                                                     + 
                                                     vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                     [1U]))) 
                                    << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[3U] 
                        = (IData)(((((QData)((IData)(
                                                     ((IData)(1U) 
                                                      + 
                                                      vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                      [1U]))) 
                                     << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))) 
                                   >> 0x20U));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v1 = 1U;
                    __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v1 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                        [1U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v5 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                                 [1U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v5 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                           [1U]);
                    if (((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                          [1U]) | ((4U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                    [1U]) & (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                                >> 1U))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((2U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v5 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                 [1U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v5 = 1U;
                    if (((0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                          [1U]) & (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                      >> 1U)))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((2U == ((2U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
                            | (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                     >> 1U))))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v6 
                        = (7U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                 [1U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v6 = 1U;
                } else {
                    if ((1U == ((2U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
                                | (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                         >> 1U))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v7 
                            = (7U & (vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                     [1U] - (IData)(1U)));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v7 = 1U;
                    }
                }
                if ((0x200U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v5 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                           [1U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v5 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                         [1U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [1U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v72 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v72 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v73 = 5U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [1U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v74 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v74 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v75 = 5U;
                    }
                }
                if ((((IData)(vlTOPp->return_i) >> 1U) 
                     & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                        [1U]))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v5 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                           [1U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v5 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                         [1U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [1U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v76 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v76 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v77 = 6U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (0ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [1U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v78 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v78 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v79 = 6U;
                    }
                }
                if ((4U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[0U] 
                        = (IData)(vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                  [0U]);
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[1U] 
                        = (IData)((vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                   [0U] >> 0x20U));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[2U] 
                        = (IData)((((QData)((IData)(
                                                    ((IData)(1U) 
                                                     + 
                                                     vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                     [2U]))) 
                                    << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[3U] 
                        = (IData)(((((QData)((IData)(
                                                     ((IData)(1U) 
                                                      + 
                                                      vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                      [2U]))) 
                                     << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))) 
                                   >> 0x20U));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v2 = 1U;
                    __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v2 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                        [2U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v6 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                                 [2U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v6 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                           [2U]);
                    if (((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                          [2U]) | ((4U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                    [2U]) & (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                                >> 2U))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((4U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v6 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                 [2U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v6 = 1U;
                    if (((0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                          [2U]) & (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                      >> 2U)))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((2U == ((2U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                   >> 1U)) | (1U & 
                                              ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                               >> 2U))))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v8 
                        = (7U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                 [2U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v8 = 1U;
                } else {
                    if ((1U == ((2U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                       >> 1U)) | (1U 
                                                  & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                                     >> 2U))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v9 
                            = (7U & (vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                     [2U] - (IData)(1U)));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v9 = 1U;
                    }
                }
                if ((0x4000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v6 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                           [2U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v6 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                         [2U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [2U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v80 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v80 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v81 = 5U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [2U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v82 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v82 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v83 = 5U;
                    }
                }
                if (((IData)(vlTOPp->return_i) & (4U 
                                                  == 
                                                  vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                                  [0U]))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v6 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                           [2U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v6 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                         [2U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [2U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v84 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v84 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v85 = 6U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (0ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [2U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v86 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v86 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v87 = 6U;
                    }
                }
                if ((8U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[0U] 
                        = (IData)(vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                  [1U]);
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[1U] 
                        = (IData)((vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                                   [1U] >> 0x20U));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[2U] 
                        = (IData)((((QData)((IData)(
                                                    ((IData)(1U) 
                                                     + 
                                                     vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                     [3U]))) 
                                    << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[3U] 
                        = (IData)(((((QData)((IData)(
                                                     ((IData)(1U) 
                                                      + 
                                                      vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                                                      [3U]))) 
                                     << 0x20U) | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q))) 
                                   >> 0x20U));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v3 = 1U;
                    __Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v3 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                        [3U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v7 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q
                                 [3U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v7 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                           [3U]);
                    if (((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
                          [3U]) | ((4U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                    [3U]) & (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                                >> 3U))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((8U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v7 
                        = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                 [3U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v7 = 1U;
                    if (((0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                          [3U]) & (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                      >> 3U)))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((2U == ((2U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                   >> 2U)) | (1U & 
                                              ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                               >> 3U))))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v10 
                        = (7U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                 [3U]));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v10 = 1U;
                } else {
                    if ((1U == ((2U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                                       >> 2U)) | (1U 
                                                  & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                                                     >> 3U))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v11 
                            = (7U & (vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                     [3U] - (IData)(1U)));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v11 = 1U;
                    }
                }
                if ((0x80000U & vlTOPp->e3_observer_top__DOT__dut__DOT__events)) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v7 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                           [3U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v7 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                         [3U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [3U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v88 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v88 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v89 = 5U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q
                                                                [3U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v90 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v90 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v91 = 5U;
                    }
                }
                if ((((IData)(vlTOPp->return_i) >> 1U) 
                     & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                        [1U]))) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v7 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                           [3U]);
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v7 = 1U;
                    if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                         [3U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [3U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v92 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v92 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v93 = 6U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (4ULL == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][1U])) & (1ULL == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][0xdU])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0U] == (1ULL + (QData)((IData)(
                                                                vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q
                                                                [3U])))))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v94 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v94 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v95 = 6U;
                    }
                }
                if ((0x100U & vlTOPp->e3_observer_top__DOT__ctl[6U])) {
                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__current_lane_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane;
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v32 = 1U;
                    if (((((0U != (0xffU & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                             << 1U) 
                                            | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                               >> 0x1fU)))) 
                           & (4U != (0xffU & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                               << 1U) 
                                              | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                                 >> 0x1fU))))) 
                          | ((0xffffffU & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                            << 0x19U) 
                                           | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                              >> 7U))) 
                             != ((2U > (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))
                                  ? 0xaU : 0xbU))) 
                         | ((((QData)((IData)(vlTOPp->e3_observer_top__DOT__ctl[5U])) 
                              << 0x39U) | (((QData)((IData)(
                                                            vlTOPp->e3_observer_top__DOT__ctl[4U])) 
                                            << 0x19U) 
                                           | ((QData)((IData)(
                                                              vlTOPp->e3_observer_top__DOT__ctl[3U])) 
                                              >> 7U))) 
                            != (((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected[1U])) 
                                 << 0x20U) | (QData)((IData)(
                                                             vlTOPp->e3_observer_top__DOT__dut__DOT__selected[0U])))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v33 = 1U;
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v64 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected[3U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v65 
                        = (QData)((IData)((0xffU & 
                                           ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                             << 1U) 
                                            | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                               >> 0x1fU)))));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v66 
                        = (QData)((IData)((0xffffffU 
                                           & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                               << 0x19U) 
                                              | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                                 >> 7U)))));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v67 
                        = (((QData)((IData)(vlTOPp->e3_observer_top__DOT__ctl[5U])) 
                            << 0x39U) | (((QData)((IData)(
                                                          vlTOPp->e3_observer_top__DOT__ctl[4U])) 
                                          << 0x19U) 
                                         | ((QData)((IData)(
                                                            vlTOPp->e3_observer_top__DOT__ctl[3U])) 
                                            >> 7U)));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v69 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected[2U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v70 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v72 
                        = (QData)((IData)((1U & (vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                 >> 7U))));
                    if (((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                         & (~ (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                       [1U][4U] >> 1U))))) {
                        if (((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                             & (vlTOPp->e3_observer_top__DOT__dut__DOT__selected[3U] 
                                == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                           [1U][0U])))) {
                            __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v96 
                                = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v96 = 1U;
                            __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v97 = 1U;
                        } else {
                            vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__inserted_q 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__inserted_q);
                        }
                    }
                }
                if ((4U & vlTOPp->e3_observer_top__DOT__ctl[0U])) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v73 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v73 = 1U;
                    __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v74 = 2U;
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                              [0U] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][0U])) & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                            [1U] == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][1U])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                            [0xdU] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0xdU]))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v98 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v98 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v99 = 2U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                              [0U] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][0U])) & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                            [1U] == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][1U])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                            [0xdU] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0xdU]))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v100 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v100 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v101 = 2U;
                    }
                }
                if ((0x200U & vlTOPp->e3_observer_top__DOT__tl[3U])) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v75 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v75 = 1U;
                    __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v76 = 3U;
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v77 
                        = (QData)((IData)((1U & (vlTOPp->e3_observer_top__DOT__tl[3U] 
                                                 >> 8U))));
                    if ((1U & (((~ (vlTOPp->e3_observer_top__DOT__tl[3U] 
                                    >> 7U)) | ((0xffffffU 
                                                & ((vlTOPp->e3_observer_top__DOT__tl[3U] 
                                                    << 0x12U) 
                                                   | (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                                      >> 0xeU))) 
                                               != (IData)(
                                                          vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                                          [2U]))) 
                               | (IData)((vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                          [4U] >> 3U))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    if (vlTOPp->active_i) {
                        if ((0x100U & vlTOPp->e3_observer_top__DOT__tl[3U])) {
                            __Vdlyvval__e3_observer_top__DOT__dut__DOT__hits_q__v4 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q
                                   [vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q]);
                            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__hits_q__v4 = 1U;
                            __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__hits_q__v4 
                                = vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q;
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q
                                 [vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q])) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                            }
                        } else {
                            __Vdlyvval__e3_observer_top__DOT__dut__DOT__misses_q__v4 
                                = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q
                                   [vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q]);
                            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__misses_q__v4 = 1U;
                            __Vdlyvdim0__e3_observer_top__DOT__dut__DOT__misses_q__v4 
                                = vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q;
                            if ((0xffffffffU == vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q
                                 [vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q])) {
                                vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                            }
                        }
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                              [0U] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][0U])) & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                            [1U] == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][1U])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                            [0xdU] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0xdU]))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v102 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v102 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v103 = 3U;
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v104 
                            = (QData)((IData)((1U & 
                                               (vlTOPp->e3_observer_top__DOT__tl[3U] 
                                                >> 8U))));
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                              [0U] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][0U])) & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                            [1U] == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][1U])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                            [0xdU] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0xdU]))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v105 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v105 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v106 = 3U;
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v107 
                            = (QData)((IData)((1U & 
                                               (vlTOPp->e3_observer_top__DOT__tl[3U] 
                                                >> 8U))));
                    }
                }
                if ((2U & vlTOPp->e3_observer_top__DOT__ctl[0U])) {
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v78 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v78 = 1U;
                    if ((1U & vlTOPp->e3_observer_top__DOT__ctl[0U])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v79 = 1U;
                    __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v79 = 4U;
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                              [0U] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [0U][0U])) & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                            [1U] == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [0U][1U])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                            [0xdU] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [0U][0xdU]))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v108 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v108 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v109 = 4U;
                    }
                    if (((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                           & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                              [0U] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                              [1U][0U])) & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                            [1U] == 
                                            vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                            [1U][1U])) 
                         & (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                            [0xdU] == vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                            [1U][0xdU]))) {
                        __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v110 
                            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v110 = 1U;
                        __Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v111 = 4U;
                    }
                }
                if ((0x2000U & vlTOPp->e3_observer_top__DOT__tl[2U])) {
                    vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__triggers_q 
                        = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q);
                    vlTOPp->e3_observer_top__DOT__dut__DOT__capture_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                    vlTOPp->e3_observer_top__DOT__dut__DOT__level_q 
                        = (3U & vlTOPp->e3_observer_top__DOT__tl[2U]);
                    vlTOPp->e3_observer_top__DOT__dut__DOT__pte_q 
                        = (((QData)((IData)(vlTOPp->e3_observer_top__DOT__tl[1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->e3_observer_top__DOT__tl[0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__state_at_capture_q 
                        = (7U & ((vlTOPp->e3_observer_top__DOT__ctl[1U] 
                                  << 0x1dU) | (vlTOPp->e3_observer_top__DOT__ctl[0U] 
                                               >> 3U)));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__waiting_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                        [0U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v112 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0U];
                    vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v112 = 1U;
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v113 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [1U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v114 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [2U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v115 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [3U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v116 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [4U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v117 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [5U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v118 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [6U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v119 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [7U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v120 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [8U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v121 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [9U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v122 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0xaU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v123 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0xbU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v124 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0xcU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v125 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0xdU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v126 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0xeU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v127 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0xfU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v128 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x10U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v129 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x11U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v130 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x12U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v131 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x13U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v132 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x14U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v133 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x15U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v134 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x16U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v135 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x17U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v136 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x18U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v137 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x19U];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v138 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x1aU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v139 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x1bU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v140 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x1cU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v141 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x1dU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v142 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x1eU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v143 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                        [0x1fU];
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v144 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
                                          [0U][vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                          [0U]][3U]));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v147 
                        = (((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
                                            [0U][vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                            [0U]][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
                                                        [0U]
                                                        [
                                                        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                                        [0U]][0U])));
                    __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v149 
                        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
                                          [0U][vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                          [0U]][2U]));
                    if ((1U & (((((((((0U != vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q) 
                                      | (~ (IData)(vlTOPp->active_i))) 
                                     | (2U > (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q))) 
                                    | (0ULL != vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
                                       [0xcU])) | (~ 
                                                   (vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                                    >> 6U))) 
                                  | (0xaU != (0xffffffU 
                                              & ((vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->e3_observer_top__DOT__ctl[2U] 
                                                    >> 6U))))) 
                                 | (0U != (0xffU & 
                                           ((vlTOPp->e3_observer_top__DOT__ctl[3U] 
                                             << 2U) 
                                            | (vlTOPp->e3_observer_top__DOT__ctl[2U] 
                                               >> 0x1eU))))) 
                                | (0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
                                   [0U])) | ((((QData)((IData)(
                                                               vlTOPp->e3_observer_top__DOT__ctl[2U])) 
                                               << 0x3aU) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->e3_observer_top__DOT__ctl[1U])) 
                                                  << 0x1aU) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__ctl[0U])) 
                                                    >> 6U))) 
                                             != (((QData)((IData)(
                                                                  vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
                                                                  [0U]
                                                                  [
                                                                  vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                                                  [0U]][1U])) 
                                                  << 0x20U) 
                                                 | (QData)((IData)(
                                                                   vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
                                                                   [0U]
                                                                   [
                                                                   vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
                                                                   [0U]][0U]))))))) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 1U;
                    }
                }
                if ((0x1000U & vlTOPp->e3_observer_top__DOT__tl[2U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__available_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                    vlTOPp->e3_observer_top__DOT__dut__DOT__available_seen_q = 1U;
                    vlTOPp->e3_observer_top__DOT__dut__DOT__extra_q 
                        = (0xffU & ((vlTOPp->e3_observer_top__DOT__tl[3U] 
                                     << 0x1eU) | (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                                  >> 2U)));
                }
                if ((0x800U & vlTOPp->e3_observer_top__DOT__tl[2U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__consume_q 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
                    vlTOPp->e3_observer_top__DOT__dut__DOT__consume_seen_q = 1U;
                    vlTOPp->e3_observer_top__DOT__dut__DOT__extra_q 
                        = (0xffU & ((vlTOPp->e3_observer_top__DOT__tl[3U] 
                                     << 0x1eU) | (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                                  >> 2U)));
                }
            }
        }
    } else {
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__run_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__time_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__current_lane_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__triggers_q = 0U;
        vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__inserted_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__capture_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__available_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__consume_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__available_seen_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__consume_seen_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__extra_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__level_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__pte_q = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__state_at_capture_q = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__waiting_q = 0U;
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8 = 1U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i) {
            vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v0 = 1U;
        } else {
            if (vlTOPp->e3_observer_top__DOT__dut__DOT__run_q) {
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0U];
                vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32 = 1U;
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v33 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [1U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v34 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [2U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v35 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [3U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v36 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [4U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v37 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [5U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v38 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [6U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v39 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [7U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v40 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [8U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v41 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [9U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v42 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0xaU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v43 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0xbU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v44 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0xcU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v45 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0xdU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v46 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0xeU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v47 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0xfU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v48 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x10U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v49 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x11U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v50 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x12U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v51 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x13U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v52 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x14U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v53 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x15U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v54 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x16U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v55 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x17U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v56 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x18U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v57 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x19U];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v58 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x1aU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v59 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x1bU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v60 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x1cU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v61 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x1dU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v62 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x1eU];
                __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v63 
                    = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                    [0x1fU];
            }
        }
    } else {
        vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v64 = 1U;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps__v0;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__tail_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps[vlTOPp->__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v0;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v1;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][2U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v2;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][3U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v3;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v4;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v5;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][2U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v6;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][3U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[0U][3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q[1U][3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v0;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v1;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[2U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v2;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[3U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v3;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[4U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v4;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[5U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v5;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[6U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v6;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[7U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q[7U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[1U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v2;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v3;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q[1U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[1U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v2;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v3;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q[1U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[1U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v2;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v3;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[1U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[7U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v8;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v9;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[2U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v10;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[3U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v11;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[4U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v12;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[5U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v13;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[6U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v14;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[7U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v15;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q__v16) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q[7U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[7U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[0U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v8;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[1U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v9;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[2U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v10;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[3U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v11;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[4U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v12;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[5U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v13;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[6U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v14;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[7U] 
            = vlTOPp->__Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v15;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q__v16) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q[7U] = 0U;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[0U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v0;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[0U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v1;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[0U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v2;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[1U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v3;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[1U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[1U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[2U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[2U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[2U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v8;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[3U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v9;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[3U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v10;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[3U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v11;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[4U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v12;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[4U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v13;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[4U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v14;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[5U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v15;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[5U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v16;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[5U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v17;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[6U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v18;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[6U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v19;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[6U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v20;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[7U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v21;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[7U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v22;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[7U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v23;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q__v24) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[0U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[0U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[0U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[1U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[1U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[1U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[2U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[2U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[2U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[3U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[3U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[3U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[4U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[4U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[4U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[5U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[5U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[5U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[6U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[6U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[6U][2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[7U][0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[7U][1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q[7U][2U] = 0U;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__triggers_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__inserted_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__inserted_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[0U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v0][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[0U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[0U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v0][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[1U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[0U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v0][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[2U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[0U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v0][3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v0[3U];
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v1) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[1U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v1][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[0U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[1U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v1][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[1U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[1U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v1][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[2U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[1U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v1][3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v1[3U];
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[2U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v2][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[0U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[2U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v2][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[1U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[2U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v2][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[2U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[2U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v2][3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v2[3U];
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v3) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[3U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v3][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[0U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[3U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v3][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[1U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[3U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v3][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[2U];
        vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem[3U][__Vdlyvdim1__e3_observer_top__DOT__dut__DOT__queue_mem__v3][3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__queue_mem__v3[3U];
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v5) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v6) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__returned_q__v7) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__returned_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__returned_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v5) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v6) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__comp_q__v7) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__comp_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__comp_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v1) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v3) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__tail_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__tail_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1fU] = 0ULL;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v32) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0U] = 0ULL;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v33) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1fU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v64;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v65;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v66;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v67;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] = 3ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[5U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v69;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[6U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v70;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xcU] = 2ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xdU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v72;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v73) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[7U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v73;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
               [4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v74)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v75) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[8U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v75;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
               [4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v76)));
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xcU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v77;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v78) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[9U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__current_q__v78;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__current_q__v79) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__current_q
               [4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__current_q__v79)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__current_q[0x1fU] = 0ULL;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v1) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v2) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__queue_mem__v3) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__seq_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v5) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v6) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v7) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__head_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__head_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v5) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v5;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v6) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v6;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v7) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v7;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v8;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v9) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v9;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v10) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v10;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__count_q__v11) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__count_q__v11;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__count_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__misses_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__misses_q__v4] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__misses_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__hits_q__v4) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[__Vdlyvdim0__e3_observer_top__DOT__dut__DOT__hits_q__v4] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__hits_q__v4;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q[3U] = 0U;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1fU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1fU] = 0ULL;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v64) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v64;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v65)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v66) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v66;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v67)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v68) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v68;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v69)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v70) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v70;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v71)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v72) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v72;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v73)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v74) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v74;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v75)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v76) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v76;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v77)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v78) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v78;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v79)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v80) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v80;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v81)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v82) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v82;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v83)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v84) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v84;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v85)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v86) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v86;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v87)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v88) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v88;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v89)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v90) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v90;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v91)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v92) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v92;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v93)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v94) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v94;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v95)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v96) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][6U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v96;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v97)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v98) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][7U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v98;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v99)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v100) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][7U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v100;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v101)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v102) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][8U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v102;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v103)));
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xcU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v104;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v105) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][8U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v105;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v106)));
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xcU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v107;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v108) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][9U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v108;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [0U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v109)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v110) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][9U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v110;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
               [1U][4U] | ((QData)((IData)(1U)) << (IData)(__Vdlyvlsb__e3_observer_top__DOT__dut__DOT__shot_q__v111)));
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__shot_q__v112) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v112;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v113;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v114;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v115;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v116;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][5U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v117;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][6U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v118;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][7U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v119;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][8U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v120;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][9U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v121;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v122;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v123;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xcU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v124;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xdU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v125;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xeU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v126;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xfU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v127;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x10U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v128;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x11U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v129;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x12U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v130;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x13U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v131;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x14U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v132;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x15U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v133;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x16U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v134;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x17U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v135;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x18U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v136;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x19U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v137;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1aU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v138;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1bU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v139;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1cU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v140;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1dU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v141;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1eU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v142;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1fU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v143;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v144;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][2U] = 0xaULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v147;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] = 1ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][5U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__shot_q__v149;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xcU] = 2ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xdU] = 0ULL;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__head_q__v8) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[0U][0x1fU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q[1U][0x1fU] = 0ULL;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__time_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q 
        = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__current_lane_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__run_q = vlTOPp->__Vdly__e3_observer_top__DOT__dut__DOT__run_q;
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v0) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1fU] = 0ULL;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v32;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[1U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v33;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[2U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v34;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[3U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v35;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[4U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v36;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[5U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v37;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[6U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v38;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[7U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v39;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[8U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v40;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[9U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v41;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xaU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v42;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xbU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v43;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xcU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v44;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xdU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v45;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xeU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v46;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xfU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v47;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x10U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v48;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x11U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v49;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x12U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v50;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x13U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v51;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x14U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v52;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x15U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v53;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x16U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v54;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x17U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v55;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x18U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v56;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x19U] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v57;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1aU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v58;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1bU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v59;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1cU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v60;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1dU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v61;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1eU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v62;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1fU] 
            = __Vdlyvval__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v63;
    }
    if (vlTOPp->__Vdlyvset__e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q__v64) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[1U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[2U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[3U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[4U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[5U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[6U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[7U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[8U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[9U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xaU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xbU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xcU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xdU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xeU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0xfU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x10U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x11U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x12U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x13U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x14U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x15U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x16U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x17U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x18U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x19U] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1aU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1bU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1cU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1dU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1eU] = 0ULL;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q[0x1fU] = 0ULL;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0x7fffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 0xfU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xbfffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 0xeU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xdfffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 0xdU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xefffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o) 
              << 0xcU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xf7ffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 0xbU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfbffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 0xaU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfdffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 9U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfeffU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o) 
              << 8U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xff7fU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 7U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xffbfU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 6U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xffdfU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 5U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xffefU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o) 
              << 4U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfff7U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__error_o) 
              << 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfffbU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__error_o) 
              << 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfffdU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__error_o) 
              << 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error 
        = ((0xfffeU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error)) 
           | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__error_o));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[0U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__entered_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[1U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__full_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[2U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__carry_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[4U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__max_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[5U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__opening_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[6U] 
        = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__closing_q));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__area_q;
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? 0U : (vlTOPp->e3_observer_top__DOT__dut__DOT__time_q 
                    - vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__stamps
                    [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_next 
        = (0x1ffffffffffffULL & (vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__sum_q 
                                 + (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__age))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[8U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [8U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[9U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [9U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xaU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xaU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xbU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xbU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xcU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xcU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xdU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xdU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xeU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xeU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0xfU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xfU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x10U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x10U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x11U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x11U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x12U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x12U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x13U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x13U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x14U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x14U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x15U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x15U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x16U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x16U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x17U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x17U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x18U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x18U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x19U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x19U];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1aU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1aU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1bU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1bU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1cU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1cU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1dU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1dU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1eU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1eU];
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o[0x1fU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1fU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[3U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[2U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[1U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][3U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][2U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][1U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data[0U][0U][7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[8U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [8U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[9U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [9U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xaU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xaU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xbU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xbU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xcU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xcU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xdU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xdU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xeU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xeU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0xfU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0xfU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x10U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x10U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x11U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x11U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x12U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x12U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x13U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x13U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x14U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x14U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x15U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x15U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x16U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x16U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x17U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x17U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x18U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x18U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x19U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x19U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1aU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1aU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1bU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1bU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1cU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1cU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1dU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1dU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1eU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1eU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__write_data[0x1fU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellout__i_writes__data_o
        [0x1fU];
}

VL_INLINE_OPT void Ve3_observer_top::_combo__TOP__3(Ve3_observer_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Ve3_observer_top::_combo__TOP__3\n"); );
    Ve3_observer_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i 
        = (1U & (((IData)(vlTOPp->out_valid_i) & (IData)(vlTOPp->out_ready_i)) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__tl[0U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[2U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[3U] = 0U;
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x1ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xfffffe00U 
                                               & ((IData)(vlTOPp->lookup_i) 
                                                  << 9U)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x2ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0xffffff00U 
                                               & ((IData)(vlTOPp->hit_i) 
                                                  << 8U)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = (0x80U | 
                                            vlTOPp->e3_observer_top__DOT__tl[3U]);
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0x3fffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffffc000U 
                                               & (((0U 
                                                    == (IData)(vlTOPp->id_i))
                                                    ? 0xaU
                                                    : 0xbU) 
                                                  << 0xeU)));
    vlTOPp->e3_observer_top__DOT__tl[3U] = ((0x3c0U 
                                             & vlTOPp->e3_observer_top__DOT__tl[3U]) 
                                            | (0x3fffU 
                                               & (((0U 
                                                    == (IData)(vlTOPp->id_i))
                                                    ? 0xaU
                                                    : 0xbU) 
                                                  >> 0x12U)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffffdfffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xffffe000U 
                                               & ((IData)(vlTOPp->capture_i) 
                                                  << 0xdU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xffffefffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xfffff000U 
                                               & ((IData)(vlTOPp->available_i) 
                                                  << 0xcU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xfffff7ffU 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xfffff800U 
                                               & ((IData)(vlTOPp->consume_i) 
                                                  << 0xbU)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = ((0xfffffc03U 
                                             & vlTOPp->e3_observer_top__DOT__tl[2U]) 
                                            | (0xfffffffcU 
                                               & ((IData)(vlTOPp->extra_i) 
                                                  << 2U)));
    vlTOPp->e3_observer_top__DOT__tl[2U] = (1U | (0xfffffffcU 
                                                  & vlTOPp->e3_observer_top__DOT__tl[2U]));
    vlTOPp->e3_observer_top__DOT__tl[0U] = 0x80001000U;
    vlTOPp->e3_observer_top__DOT__tl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i[0U] 
        = vlTOPp->return_id_i[0U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i[1U] 
        = vlTOPp->return_id_i[1U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i[0U] 
        = vlTOPp->tr_id_i[0U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i[1U] 
        = vlTOPp->tr_id_i[1U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i[0U] 
        = vlTOPp->out_id_i[0U];
    vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i[1U] 
        = vlTOPp->out_id_i[1U];
    vlTOPp->e3_observer_top__DOT__ctl[0U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[2U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[3U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[4U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[5U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[6U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0xffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0xffffff00U 
                                                & ((IData)(vlTOPp->select_i) 
                                                   << 8U)));
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0x17fU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0xffffff80U 
                                                & ((IData)(vlTOPp->write_i) 
                                                   << 7U)));
    vlTOPp->e3_observer_top__DOT__ctl[5U] = ((0x7fffffffU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[5U]) 
                                             | (0x80000000U 
                                                & ((IData)(vlTOPp->id_i) 
                                                   << 0x1fU)));
    vlTOPp->e3_observer_top__DOT__ctl[6U] = ((0x180U 
                                              & vlTOPp->e3_observer_top__DOT__ctl[6U]) 
                                             | (0x7fffffffU 
                                                & ((IData)(vlTOPp->id_i) 
                                                   >> 1U)));
    vlTOPp->e3_observer_top__DOT__ctl[5U] = ((0x8000007fU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[5U]) 
                                             | (0xffffff80U 
                                                & (((0U 
                                                     == (IData)(vlTOPp->id_i))
                                                     ? 0xaU
                                                     : 0xbU) 
                                                   << 7U)));
    vlTOPp->e3_observer_top__DOT__ctl[3U] = (0x80000U 
                                             | (0x7fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[3U]));
    vlTOPp->e3_observer_top__DOT__ctl[4U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[5U] = (0xffffff80U 
                                             & vlTOPp->e3_observer_top__DOT__ctl[5U]);
    vlTOPp->e3_observer_top__DOT__ctl[3U] = (0x40U 
                                             | vlTOPp->e3_observer_top__DOT__ctl[3U]);
    vlTOPp->e3_observer_top__DOT__ctl[2U] = (0x280U 
                                             | (0xc000003fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[2U]));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = (0x40000U 
                                             | (0x3fU 
                                                & vlTOPp->e3_observer_top__DOT__ctl[0U]));
    vlTOPp->e3_observer_top__DOT__ctl[1U] = 0U;
    vlTOPp->e3_observer_top__DOT__ctl[2U] = (0xffffffc0U 
                                             & vlTOPp->e3_observer_top__DOT__ctl[2U]);
    vlTOPp->e3_observer_top__DOT__ctl[0U] = (0x10U 
                                             | (0xffffffc7U 
                                                & vlTOPp->e3_observer_top__DOT__ctl[0U]));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = ((0xfffffffbU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[0U]) 
                                             | (0xfffffffcU 
                                                & ((IData)(vlTOPp->request_i) 
                                                   << 2U)));
    vlTOPp->e3_observer_top__DOT__ctl[0U] = ((0xfffffffdU 
                                              & vlTOPp->e3_observer_top__DOT__ctl[0U]) 
                                             | (0xfffffffeU 
                                                & ((IData)(vlTOPp->response_i) 
                                                   << 1U)));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__clear_i 
        = ((IData)(vlTOPp->clear_i) | (IData)(vlTOPp->arm_i));
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q;
    if (vlTOPp->wlast_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__whole 
        = ((0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))
            ? (IData)(vlTOPp->active_i) : vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__born_in_window
           [vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__head_q]);
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_n 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q;
    if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q)));
    }
    vlTOPp->error_o = (1U & ((((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q) 
                               | (0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error))) 
                              | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__write_error)) 
                             | (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                >> 0xaU)));
    vlTOPp->data_o = 0ULL;
    if (((0x60U <= (IData)(vlTOPp->index_i)) & (0xe0U 
                                                > (IData)(vlTOPp->index_i)))) {
        vlTOPp->data_o = vlTOPp->e3_observer_top__DOT__dut__DOT__stage_data
            [(3U & (((IData)(vlTOPp->index_i) - (IData)(0x60U)) 
                    >> 5U))][(3U & (((IData)(vlTOPp->index_i) 
                                     - (IData)(0x60U)) 
                                    >> 3U))][(7U & (IData)(vlTOPp->index_i))];
    } else {
        if ((0xe0U <= (IData)(vlTOPp->index_i))) {
            vlTOPp->data_o = vlTOPp->e3_observer_top__DOT__dut__DOT__write_data
                [(0x1fU & (IData)(vlTOPp->index_i))];
        } else {
            if ((0x20U <= (IData)(vlTOPp->index_i))) {
                vlTOPp->data_o = vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                    [(1U & (((IData)(vlTOPp->index_i) 
                             - (IData)(0x20U)) >> 5U))]
                    [(0x1fU & (IData)(vlTOPp->index_i))];
            } else {
                if (((((((((0U == (IData)(vlTOPp->index_i)) 
                           | (1U == (IData)(vlTOPp->index_i))) 
                          | (2U == (IData)(vlTOPp->index_i))) 
                         | (3U == (IData)(vlTOPp->index_i))) 
                        | (4U == (IData)(vlTOPp->index_i))) 
                       | (5U == (IData)(vlTOPp->index_i))) 
                      | (6U == (IData)(vlTOPp->index_i))) 
                     | (7U == (IData)(vlTOPp->index_i)))) {
                    vlTOPp->data_o = ((0U == (IData)(vlTOPp->index_i))
                                       ? 0x49444d4145330001ULL
                                       : ((1U == (IData)(vlTOPp->index_i))
                                           ? (((QData)((IData)(
                                                               (1U 
                                                                & (vlTOPp->e3_observer_top__DOT__tl[2U] 
                                                                   >> 0xaU)))) 
                                               << 3U) 
                                              | (QData)((IData)(
                                                                (((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__write_error) 
                                                                  << 2U) 
                                                                 | (((IData)(
                                                                             (0U 
                                                                              != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__stage_error))) 
                                                                     << 1U) 
                                                                    | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__bad_q))))))
                                           : ((2U == (IData)(vlTOPp->index_i))
                                               ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__triggers_q))
                                               : ((3U 
                                                   == (IData)(vlTOPp->index_i))
                                                   ? 
                                                  ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__delay_q)
                                                    ? 0x80ULL
                                                    : 0ULL)
                                                   : 
                                                  ((4U 
                                                    == (IData)(vlTOPp->index_i))
                                                    ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__capture_q))
                                                    : 
                                                   ((5U 
                                                     == (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__available_q))
                                                     : 
                                                    ((6U 
                                                      == (IData)(vlTOPp->index_i))
                                                      ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__consume_q))
                                                      : (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__extra_q)))))))));
                } else {
                    if (((((((((8U == (IData)(vlTOPp->index_i)) 
                               | (9U == (IData)(vlTOPp->index_i))) 
                              | (0xaU == (IData)(vlTOPp->index_i))) 
                             | (0xbU == (IData)(vlTOPp->index_i))) 
                            | (0xcU == (IData)(vlTOPp->index_i))) 
                           | (0xdU == (IData)(vlTOPp->index_i))) 
                          | (0xeU == (IData)(vlTOPp->index_i))) 
                         | (0xfU == (IData)(vlTOPp->index_i)))) {
                        vlTOPp->data_o = ((8U == (IData)(vlTOPp->index_i))
                                           ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__level_q))
                                           : ((9U == (IData)(vlTOPp->index_i))
                                               ? vlTOPp->e3_observer_top__DOT__dut__DOT__pte_q
                                               : ((0xaU 
                                                   == (IData)(vlTOPp->index_i))
                                                   ? 
                                                  vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                                  [1U]
                                                  [0U]
                                                   : 
                                                  ((0xbU 
                                                    == (IData)(vlTOPp->index_i))
                                                    ? 
                                                   vlTOPp->e3_observer_top__DOT__dut__DOT__shot_q
                                                   [0U]
                                                   [0U]
                                                    : 
                                                   ((0xcU 
                                                     == (IData)(vlTOPp->index_i))
                                                     ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__inserted_q))
                                                     : 
                                                    ((0xdU 
                                                      == (IData)(vlTOPp->index_i))
                                                      ? 
                                                     (((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__consume_seen_q)) 
                                                       << 1U) 
                                                      | (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__available_seen_q)))
                                                      : 
                                                     ((0xeU 
                                                       == (IData)(vlTOPp->index_i))
                                                       ? (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__state_at_capture_q))
                                                       : (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__waiting_q)))))))));
                    } else {
                        if ((0x10U == (IData)(vlTOPp->index_i))) {
                            vlTOPp->data_o = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__time_q));
                        } else {
                            if (((((0x11U == (IData)(vlTOPp->index_i)) 
                                   | (0x12U == (IData)(vlTOPp->index_i))) 
                                  | (0x13U == (IData)(vlTOPp->index_i))) 
                                 | (0x14U == (IData)(vlTOPp->index_i)))) {
                                vlTOPp->data_o = (QData)((IData)(
                                                                 vlTOPp->e3_observer_top__DOT__dut__DOT__hits_q
                                                                 [
                                                                 (3U 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     - (IData)(1U)))]));
                            } else {
                                if (((((0x15U == (IData)(vlTOPp->index_i)) 
                                       | (0x16U == (IData)(vlTOPp->index_i))) 
                                      | (0x17U == (IData)(vlTOPp->index_i))) 
                                     | (0x18U == (IData)(vlTOPp->index_i)))) {
                                    vlTOPp->data_o 
                                        = (QData)((IData)(
                                                          vlTOPp->e3_observer_top__DOT__dut__DOT__misses_q
                                                          [
                                                          (3U 
                                                           & ((IData)(vlTOPp->index_i) 
                                                              - (IData)(1U)))]));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__write_error;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base = 0U;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w = 0ULL;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b = 0ULL;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b = 0ULL;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [0U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [0U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [0U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [1U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [1U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [1U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [2U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[2U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [2U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[2U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [2U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[2U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [3U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[3U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [3U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[3U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [3U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[3U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [4U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[4U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [4U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[4U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [4U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[4U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [5U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[5U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [5U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[5U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [5U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[5U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [6U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[6U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [6U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[6U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [6U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[6U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [7U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[7U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [7U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[7U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_q
        [7U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[7U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT____Vlvbound1;
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[0U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [0U][3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[1U][3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_q
        [1U][3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [3U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[4U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [4U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[5U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [5U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[6U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [6U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[7U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [7U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[8U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [8U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[9U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [9U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xaU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xaU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xbU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xbU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xcU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xdU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xdU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xeU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xeU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xfU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0xfU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x10U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x10U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x11U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x11U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x12U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x12U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x13U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x13U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x14U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x14U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x15U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x15U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x16U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x16U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x17U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x17U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x18U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x18U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x19U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x19U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1aU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1aU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1bU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1bU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1cU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1cU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1dU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1dU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1eU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1eU];
    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1fU] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
        [0x1fU];
    if (vlTOPp->e3_observer_top__DOT__dut__DOT____Vcellinp__i_writes__aw_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role 
            = (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
               [1U]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q;
        if ((1U & ((((0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                      [1U]) & (4U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                               [1U])) | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
                   | (4U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_q
                      [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[(7U 
                                                                          & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (1U | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
               [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = ((6U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
               | (IData)(vlTOPp->active_i));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)][0U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n[(7U 
                                                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n[(1U 
                                                                        & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)][vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q[(1U 
                                                                                & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_tail_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_n[(1U 
                                                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__tail_q
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[(1U 
                                                                        & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (7U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]));
        if (vlTOPp->active_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                           & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role))] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
                   [(0x1fU & VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role))]);
        }
    }
    if (vlTOPp->wlast_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_tail_q;
        if ((2U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
             [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)])) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[(7U 
                                                                          & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (2U | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
               [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = ((5U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 1U));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)][1U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
    }
    if ((2U & (IData)(vlTOPp->return_i))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role 
            = (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
               [1U]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__order_n
            [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]
            [vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
            [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]];
        if ((1U & ((((((0U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                        [1U]) & (4U != vlTOPp->e3_observer_top__DOT____Vcellinp__dut__return_id_i
                                 [1U])) | (0U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                                           [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)])) 
                     | (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                        [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)])) 
                    | (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                          [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
                          >> 1U))) | (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                      [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
                                      >> 2U)))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[(7U 
                                                                          & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = (4U | vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
               [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)] 
            = ((3U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                [(7U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)]) 
               | ((IData)(vlTOPp->active_i) << 2U));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n[(7U 
                                                                         & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__slot)][2U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_n[(1U 
                                                                       & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (3U & ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__head_q
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n[(1U 
                                                                        & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
            = (7U & (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                     [(1U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)] 
                     - (IData)(1U)));
        if (vlTOPp->active_i) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                           & ((IData)(1U) 
                                                                              + 
                                                                              VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)))] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
                   [(0x1fU & ((IData)(1U) + VL_MULS_III(5,32,32, (IData)(0x10U), vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role)))]);
        }
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [0U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [0U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [0U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [0U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [0U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [0U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [0U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [0U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [0U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [0U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [0U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[0U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[0U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [1U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [1U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [1U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [1U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [1U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [1U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [1U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [1U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [1U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [1U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [1U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[1U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[1U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [2U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [2U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [2U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [2U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [2U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [2U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [2U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [2U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [2U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [2U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [2U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[2U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[2U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [3U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [3U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [3U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [3U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [3U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [3U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [3U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [3U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [3U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [3U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [3U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[3U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[3U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [4U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [4U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [4U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [4U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [4U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [4U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [4U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [4U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [4U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [4U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [4U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[4U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[4U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [5U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [5U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [5U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [5U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [5U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [5U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [5U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [5U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [5U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [5U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [5U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[5U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[5U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [6U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [6U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [6U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [6U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [6U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [6U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [6U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [6U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [6U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [6U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [6U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[6U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[6U] = 0U;
    }
    if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
         [7U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base 
            = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__role_n
               [7U] ? 0x10U : 0U);
        if ((4U & vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
             [7U])) {
            if ((7U == vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n
                 [7U])) {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(2U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(2U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [7U][1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                     [7U][0U])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(4U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(4U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
                } else {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                        = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                           [7U][1U])) 
                           - (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                             [7U][0U])));
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(5U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                           [(0x1fU & ((IData)(5U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                           + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w);
                    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w 
                         > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                         [(0x1fU & ((IData)(6U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(6U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                            = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_w;
                    }
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [7U]
                                                                    [1U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                    = ((QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                       [7U][2U])) - (QData)((IData)(
                                                                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__stamps_n
                                                                    [7U]
                                                                    [0U])));
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(7U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(7U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(8U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(8U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__w_b;
                }
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(9U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(9U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                       + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b);
                if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b 
                     > vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                     [(0x1fU & ((IData)(0xaU) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))])) {
                    vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                                & ((IData)(0xaU) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                        = vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__aw_b;
                }
            } else {
                vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[(0x1fU 
                                                                               & ((IData)(3U) 
                                                                                + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))] 
                    = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                       [(0x1fU & ((IData)(3U) + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__base))]);
            }
        }
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n[7U] = 0U;
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__inside_n[7U] = 0U;
    }
    if (vlTOPp->open_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xdU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [0U]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1dU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [1U]));
    }
    if (vlTOPp->close_i) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xbU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [0U]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0x1bU] 
            = (QData)((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__count_n
                              [1U]));
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] = 0ULL;
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [0U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [0U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [1U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [1U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [2U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [2U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [3U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [3U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [4U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [4U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [5U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [5U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [6U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [6U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
        if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                    [7U] >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__present_n
                                   [7U])))) {
            vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n[0xcU] 
                = (1ULL + vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
                   [0xcU]);
        }
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [1U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [1U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [2U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [2U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [3U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [3U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [4U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [4U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [5U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [5U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [6U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [6U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [7U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [7U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [8U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [8U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [9U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [9U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0xaU] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0xaU])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x10U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x10U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x11U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x11U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x12U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x12U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x13U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x13U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x14U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x14U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x15U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x15U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x16U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x16U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x17U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x17U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x18U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x18U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x19U] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x19U])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    if ((vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_n
         [0x1aU] < vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__data_q
         [0x1aU])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__i_writes__DOT__bad = 1U;
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane 
        = (((4U == (0xffU & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                              << 1U) | (vlTOPp->e3_observer_top__DOT__ctl[5U] 
                                        >> 0x1fU)))) 
            << 1U) | (1U & (vlTOPp->e3_observer_top__DOT__ctl[6U] 
                            >> 7U)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[0U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][0U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[1U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][1U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[2U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][2U];
    vlTOPp->e3_observer_top__DOT__dut__DOT__selected[3U] 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__queue_mem
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]
        [vlTOPp->e3_observer_top__DOT__dut__DOT__head_q
        [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]][3U];
    if ((0U == vlTOPp->e3_observer_top__DOT__dut__DOT__count_q
         [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane])) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[3U] 
            = ((IData)(1U) + vlTOPp->e3_observer_top__DOT__dut__DOT__seq_q
               [vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[2U] 
            = vlTOPp->e3_observer_top__DOT__dut__DOT__time_q;
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[0U] 
            = (IData)(vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                      [(1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))]);
        vlTOPp->e3_observer_top__DOT__dut__DOT__selected[1U] 
            = (IData)((vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_address_i
                       [(1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))] 
                       >> 0x20U));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xeU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | ((IData)(vlTOPp->tr_fire_i) & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                          [0U])));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xeU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                       >> 8U) 
                                                      & (0U 
                                                         == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffffeU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffffdU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffffbU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (0U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffff7U & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = (((IData)(vlTOPp->out_valid_i) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q))) 
           & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
              [0U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfffefU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 4U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xdU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | (0xfffffffeU & ((IData)(vlTOPp->tr_fire_i) 
                           & ((0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                               [1U]) << 1U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xdU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | (0x1fffffeU 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                          >> 7U) 
                                                         & ((1U 
                                                             == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                                                            << 1U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfffdfU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1) 
              << 5U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                 >> 1U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfffbfU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 6U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (1U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfff7fU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 7U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (1U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffeffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 8U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = ((((IData)(vlTOPp->out_valid_i) >> 1U) & 
            (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q) 
                >> 1U))) & (0U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                            [1U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffdffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 9U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((0xbU & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | (((IData)(vlTOPp->tr_fire_i) & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                           [0U])) << 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (0xbU 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | (0x3fffffcU 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                          >> 6U) 
                                                         & ((2U 
                                                             == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                                                            << 2U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                 >> 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xffbffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1) 
              << 0xaU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                 >> 2U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xff7ffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 0xbU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (2U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfefffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 0xcU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (2U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfdfffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 0xdU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = (((IData)(vlTOPp->out_valid_i) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q))) 
           & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
              [0U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xfbfffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 0xeU));
    vlTOPp->e3_observer_top__DOT__dut__DOT__push = 
        ((7U & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push)) 
         | (0xfffffff8U & (((IData)(vlTOPp->tr_fire_i) 
                            << 2U) & ((4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__tr_id_i
                                       [1U]) << 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__pop = (
                                                   (7U 
                                                    & (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop)) 
                                                   | (0x7fffff8U 
                                                      & ((vlTOPp->e3_observer_top__DOT__ctl[6U] 
                                                          >> 5U) 
                                                         & ((3U 
                                                             == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__selected_lane)) 
                                                            << 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__push) 
                 >> 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xf7fffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound1) 
              << 0xfU));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2 
        = (1U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__pop) 
                 >> 3U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xeffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound2) 
              << 0x10U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 2U) 
           & (3U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xdffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound3) 
              << 0x11U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4 
        = ((vlTOPp->e3_observer_top__DOT__ctl[0U] >> 1U) 
           & (3U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__current_lane_q)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0xbffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound4) 
              << 0x12U));
    vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5 
        = ((((IData)(vlTOPp->out_valid_i) >> 1U) & 
            (~ ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__out_hold_q) 
                >> 1U))) & (4U == vlTOPp->e3_observer_top__DOT____Vcellinp__dut__out_id_i
                            [1U]));
    vlTOPp->e3_observer_top__DOT__dut__DOT__events 
        = ((0x7ffffU & vlTOPp->e3_observer_top__DOT__dut__DOT__events) 
           | ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT____Vlvbound5) 
              << 0x13U));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
               & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                     >> 1U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 1U) & (~ vlTOPp->e3_observer_top__DOT__dut__DOT__events)))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 1U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 2U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 2U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 1U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 2U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 3U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 3U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 2U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 3U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 4U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 4U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 3U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 5U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 6U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 6U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 5U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 6U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 7U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 7U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 6U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 7U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 8U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 8U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 7U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 8U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 9U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 9U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                             >> 8U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xaU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xbU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xbU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xaU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xbU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xcU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xcU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xbU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xcU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xdU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xdU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xcU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xdU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xeU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xeU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0xdU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0xfU) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 0x10U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x10U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0xfU))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x10U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x11U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x11U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x10U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x11U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x12U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x12U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x11U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
        = vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q;
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x12U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x13U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)));
    }
    if ((1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                >> 0x13U) & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                >> 0x12U))))) {
        vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_next 
            = (7U & ((IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q) 
                     - (IData)(1U)));
    }
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 1U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                            | vlTOPp->e3_observer_top__DOT__dut__DOT__events)));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 1U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                     & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 1U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 2U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 1U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 2U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 1U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 2U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 3U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 2U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 3U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 2U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 3U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 4U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 3U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 4U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 3U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__0__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 4U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 6U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 5U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 6U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 5U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 6U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 7U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 6U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 7U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 6U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 7U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 8U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 7U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 8U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 7U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 8U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 9U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                            | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                               >> 8U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 9U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 8U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__1__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 9U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xbU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xaU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xbU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xaU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xbU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xcU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xbU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xcU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xbU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xcU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xdU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xcU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xdU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xcU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xdU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0xeU) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                              | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                 >> 0xdU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0xeU) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xdU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__2__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0xeU)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x10U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0xfU))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x10U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0xfU) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__0__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x10U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x11U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0x10U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x11U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0x10U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__1__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x11U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x12U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0x11U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x12U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0x11U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__2__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x12U)))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched 
        = (1U & ((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                  >> 0x13U) & ((0U != (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q)) 
                               | (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                                  >> 0x12U))));
    vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__bad 
        = (1U & (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                   >> 0x13U) & (~ (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__matched))) 
                 | (((vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                      >> 0x12U) & (4U == (IData)(vlTOPp->e3_observer_top__DOT__dut__DOT__gen_lane__BRA__3__KET____DOT__gen_stage__BRA__3__KET____DOT__i_stage__DOT__count_q))) 
                    & (~ (vlTOPp->e3_observer_top__DOT__dut__DOT__events 
                          >> 0x13U)))));
}
