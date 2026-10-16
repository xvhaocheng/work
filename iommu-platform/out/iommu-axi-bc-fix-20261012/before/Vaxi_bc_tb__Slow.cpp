// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vaxi_bc_tb.h for the primary calling header

#include "Vaxi_bc_tb.h"
#include "Vaxi_bc_tb__Syms.h"

//==========

VL_CTOR_IMP(Vaxi_bc_tb) {
    Vaxi_bc_tb__Syms* __restrict vlSymsp = __VlSymsp = new Vaxi_bc_tb__Syms(this, name());
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vaxi_bc_tb::__Vconfigure(Vaxi_bc_tb__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vaxi_bc_tb::~Vaxi_bc_tb() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vaxi_bc_tb::_eval_initial(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_eval_initial\n"); );
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vaxi_bc_tb::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::final\n"); );
    // Variables
    Vaxi_bc_tb__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vaxi_bc_tb::_eval_settle(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_eval_settle\n"); );
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
}

void Vaxi_bc_tb::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_ctor_var_reset\n"); );
    // Body
    check_i = VL_RAND_RESET_I(1);
    addr_i = VL_RAND_RESET_Q(64);
    burst_type_i = VL_RAND_RESET_I(2);
    burst_length_i = VL_RAND_RESET_I(8);
    n_bytes_i = VL_RAND_RESET_I(3);
    allow_request_o = VL_RAND_RESET_I(1);
    axi_bc_tb__DOT__i_axi_bc__DOT__log2_len = VL_RAND_RESET_I(3);
    axi_bc_tb__DOT__i_axi_bc__DOT__wrap_boundary = VL_RAND_RESET_Q(64);
    axi_bc_tb__DOT__i_axi_bc__DOT__wrap_faddr = VL_RAND_RESET_Q(64);
}
