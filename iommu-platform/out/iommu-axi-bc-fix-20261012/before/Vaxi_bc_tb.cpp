// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vaxi_bc_tb.h for the primary calling header

#include "Vaxi_bc_tb.h"
#include "Vaxi_bc_tb__Syms.h"

//==========

void Vaxi_bc_tb::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vaxi_bc_tb::eval\n"); );
    Vaxi_bc_tb__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("sim/axi_bc/axi_bc_tb.sv", 12, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vaxi_bc_tb::_eval_initial_loop(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("sim/axi_bc/axi_bc_tb.sv", 12, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vaxi_bc_tb::_combo__TOP__1(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_combo__TOP__1\n"); );
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__log2_len 
        = ((1U == (IData)(vlTOPp->burst_length_i)) ? 1U
            : ((3U == (IData)(vlTOPp->burst_length_i))
                ? 2U : ((7U == (IData)(vlTOPp->burst_length_i))
                         ? 3U : ((0xfU == (IData)(vlTOPp->burst_length_i))
                                  ? 4U : 7U))));
    vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__wrap_boundary 
        = ((vlTOPp->addr_i >> (7U & ((IData)(vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__log2_len) 
                                     + (IData)(vlTOPp->n_bytes_i)))) 
           << (7U & ((IData)(vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__log2_len) 
                     + (IData)(vlTOPp->n_bytes_i))));
    vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__wrap_faddr 
        = ((0xfffULL & vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__wrap_boundary) 
           + ((0x3fU >= ((IData)(vlTOPp->n_bytes_i) 
                         - (IData)(1U))) ? ((1ULL + (QData)((IData)(vlTOPp->burst_length_i))) 
                                            << ((IData)(vlTOPp->n_bytes_i) 
                                                - (IData)(1U)))
               : 0ULL));
    vlTOPp->allow_request_o = 0U;
    if ((0U == (IData)(vlTOPp->burst_type_i))) {
        if ((0ULL == (0xfffffffffffff000ULL & ((0xfffULL 
                                                & vlTOPp->addr_i) 
                                               + (1ULL 
                                                  << (IData)(vlTOPp->n_bytes_i)))))) {
            vlTOPp->allow_request_o = vlTOPp->check_i;
        }
    } else {
        if ((2U == (IData)(vlTOPp->burst_type_i))) {
            if (((~ (IData)((7U == (IData)(vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__log2_len)))) 
                 & (0ULL == (0xfffffffffffff000ULL 
                             & vlTOPp->axi_bc_tb__DOT__i_axi_bc__DOT__wrap_faddr)))) {
                vlTOPp->allow_request_o = vlTOPp->check_i;
            }
        } else {
            if ((1U == (IData)(vlTOPp->burst_type_i))) {
                if ((0ULL == (0xfffffffffffff000ULL 
                              & ((0xfffULL & vlTOPp->addr_i) 
                                 + ((0x3fU >= ((IData)(vlTOPp->n_bytes_i) 
                                               - (IData)(1U)))
                                     ? ((1ULL + (QData)((IData)(vlTOPp->burst_length_i))) 
                                        << ((IData)(vlTOPp->n_bytes_i) 
                                            - (IData)(1U)))
                                     : 0ULL))))) {
                    vlTOPp->allow_request_o = vlTOPp->check_i;
                }
            }
        }
    }
}

void Vaxi_bc_tb::_eval(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_eval\n"); );
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
}

VL_INLINE_OPT QData Vaxi_bc_tb::_change_request(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_change_request\n"); );
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vaxi_bc_tb::_change_request_1(Vaxi_bc_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_change_request_1\n"); );
    Vaxi_bc_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vaxi_bc_tb::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaxi_bc_tb::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((check_i & 0xfeU))) {
        Verilated::overWidthError("check_i");}
    if (VL_UNLIKELY((burst_type_i & 0xfcU))) {
        Verilated::overWidthError("burst_type_i");}
    if (VL_UNLIKELY((n_bytes_i & 0xf8U))) {
        Verilated::overWidthError("n_bytes_i");}
}
#endif  // VL_DEBUG
