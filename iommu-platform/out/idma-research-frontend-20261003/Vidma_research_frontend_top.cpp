// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_research_frontend_top.h for the primary calling header

#include "Vidma_research_frontend_top.h"
#include "Vidma_research_frontend_top__Syms.h"

//==========

void Vidma_research_frontend_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vidma_research_frontend_top::eval\n"); );
    Vidma_research_frontend_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vidma_research_frontend_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/idma_research_frontend_top.sv", 3, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vidma_research_frontend_top::_eval_initial_loop(Vidma_research_frontend_top__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/tests/idma_research_frontend_top.sv", 3, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vidma_research_frontend_top::_sequent__TOP__2(Vidma_research_frontend_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_frontend_top::_sequent__TOP__2\n"); );
    Vidma_research_frontend_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q;
    QData/*32:0*/ __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q;
    QData/*32:0*/ __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q;
    // Body
    __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q;
    __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_issued_q;
    __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q;
    if (vlTOPp->rst_ni) {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_q 
            = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_n;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q 
            = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_q 
            = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_n;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q 
            = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_n) 
               & 1U);
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q 
            = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_n;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q 
            = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
            = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d;
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_q = 0U;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q = 0ULL;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_q = 0ULL;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q = 0U;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q = 0ULL;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q = 1ULL;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q = 2ULL;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U])) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                = (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[0U])));
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__n_trans_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__n_trans_we)
                    ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wdata_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wdata_we)
                    ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_cache_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_cache_we)
                    ? (0xfU & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U] 
                                << 0x12U) | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
                                             >> 0xeU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_ttype_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_ttype_we)
                    ? (3U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U] 
                              << 0x14U) | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
                                           >> 0xcU)))
                    : 0U);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_pv_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_pv_we) 
                   & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
                      >> 0xbU));
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_serialize_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_serialize_we) 
                   & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
                      >> 0xaU));
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_deburst_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_deburst_we) 
                   & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
                      >> 9U));
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_decouple_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_decouple_we) 
                   & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
                      >> 8U));
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__num_bytes_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__num_bytes_we)
                    ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__dst_addr_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__dst_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__src_addr_we) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__src_addr_we)
                    ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U])) 
                        << 0x38U) | (((QData)((IData)(
                                                      vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U])) 
                                      << 0x18U) | ((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U])) 
                                                   >> 8U)))
                    : 0ULL);
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U))))) {
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                    >> 8U))))) {
                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q = 0U;
                                }
                            }
                        } else {
                            if ((0x90U != (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U)))) {
                                if ((0x98U != (0xffU 
                                               & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U)))) {
                                    if ((0xd0U == (0xffU 
                                                   & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                      >> 8U)))) {
                                        if ((0U == 
                                             ((vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                               << 0x18U) 
                                              | (vlTOPp->idma_research_frontend_top__DOT__req[1U] 
                                                 >> 8U)))) {
                                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q 
                                                = (
                                                   (vlTOPp->idma_research_frontend_top__DOT__req[1U] 
                                                    << 0x18U) 
                                                   | (vlTOPp->idma_research_frontend_top__DOT__req[0U] 
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
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                        if ((0x88U != (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) {
                            if ((0x90U == (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U)))) {
                                if (((0ULL < (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                    >> 8U)))) 
                                     & (0x100000000ULL 
                                        >= (((QData)((IData)(
                                                             vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                             << 0x38U) 
                                            | (((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                << 0x18U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                  >> 8U)))))) {
                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q 
                                        = (0x1ffffffffULL 
                                           & (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                    >> 8U))));
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                        if ((0x88U != (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) {
                            if ((0x90U != (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U)))) {
                                if ((0x98U == (0xffU 
                                               & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U)))) {
                                    if (((0ULL < (((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                        >> 8U)))) 
                                         & (0x1000ULL 
                                            >= (((QData)((IData)(
                                                                 vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                    << 0x18U) 
                                                   | ((QData)((IData)(
                                                                      vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                      >> 8U)))))) {
                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q 
                                            = (0x1fffU 
                                               & ((vlTOPp->idma_research_frontend_top__DOT__req[1U] 
                                                   << 0x18U) 
                                                  | (vlTOPp->idma_research_frontend_top__DOT__req[0U] 
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
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__locked_write) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
        }
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U))) {
            if ((0xffU != (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) {
                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
            } else {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U)))))))) {
                    if ((0U != (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
                    } else {
                        if ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U))))) {
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                    >> 8U))))) {
                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 0U;
                                }
                            } else {
                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
                            }
                        } else {
                            if ((0x90U == (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U)))) {
                                if ((1U & (~ ((0ULL 
                                               < (((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                        >> 8U)))) 
                                              & (0x100000000ULL 
                                                 >= 
                                                 (((QData)((IData)(
                                                                   vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                   << 0x38U) 
                                                  | (((QData)((IData)(
                                                                      vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                      << 0x18U) 
                                                     | ((QData)((IData)(
                                                                        vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                        >> 8U)))))))) {
                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
                                }
                            } else {
                                if ((0x98U == (0xffU 
                                               & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U)))) {
                                    if ((1U & (~ ((0ULL 
                                                   < 
                                                   (((QData)((IData)(
                                                                     vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                     << 0x38U) 
                                                    | (((QData)((IData)(
                                                                        vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                        << 0x18U) 
                                                       | ((QData)((IData)(
                                                                          vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                          >> 8U)))) 
                                                  & (0x1000ULL 
                                                     >= 
                                                     (((QData)((IData)(
                                                                       vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                       << 0x38U) 
                                                      | (((QData)((IData)(
                                                                          vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                          << 0x18U) 
                                                         | ((QData)((IData)(
                                                                            vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                            >> 8U)))))))) {
                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
                                    }
                                } else {
                                    if ((0xd0U == (0xffU 
                                                   & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                      >> 8U)))) {
                                        if ((0U != 
                                             ((vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                               << 0x18U) 
                                              | (vlTOPp->idma_research_frontend_top__DOT__req[1U] 
                                                 >> 8U)))) {
                                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
                                        }
                                    } else {
                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 1U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
             & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue))) {
            __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q = 0ULL;
            __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q = 0ULL;
            __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q = 0U;
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q = 0U;
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_reverse_q 
                = vlTOPp->batch_reverse_i;
        }
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_task_issue) {
            __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q 
                = (0x1ffffffffULL & (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_issued_q));
        }
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_task_done) {
            __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q 
                = (0x1ffffffffULL & (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q));
            if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) {
                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q = 1U;
            }
        }
        if ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
              & ((4U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)) 
                 | (5U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) 
             & (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                 | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write)) 
                | (IData)(vlTOPp->policy_abort_i)))) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q = 1U;
        }
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) {
                if (((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                         >> 8U))) & 
                     (2ULL == (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                << 0x38U) | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U)))))) {
                    __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q = 1U;
                } else {
                    if ((0U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U))))) {
                                __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q = 0U;
                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q = 0U;
                                if ((0ULL == (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                               << 0x38U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                  << 0x18U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                    >> 8U))))) {
                                    __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q = 0ULL;
                                    __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q = 0ULL;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q = 0ULL;
        __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q = 0ULL;
        __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q = 0U;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q = 0U;
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_reverse_q = 0U;
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_issued_q 
        = __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_issued_q;
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q 
        = __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_completed_q;
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q 
        = __Vdly__idma_research_frontend_top__DOT__dut__DOT__batch_stop_q;
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U))) {
            if ((0xffU == (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) {
                if ((1U & (~ ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U))) 
                              & (2ULL == (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U)))))))) {
                    if ((0U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                        if ((0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) {
                            if ((1ULL >= (((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                           << 0x38U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                              << 0x18U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                >> 8U))))) {
                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q 
                                    = (1U & (vlTOPp->idma_research_frontend_top__DOT__req[0U] 
                                             >> 8U));
                            }
                        }
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q = 0U;
    }
    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) {
        vlTOPp->issue_id_o = (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_issued_q);
        vlTOPp->done_id_o = (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q);
    } else {
        vlTOPp->issue_id_o = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q;
        vlTOPp->done_id_o = ((0xffffffffffffffffULL 
                              == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                              ? 2ULL : (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q));
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n)
            : 0U);
    vlTOPp->idma_research_frontend_top__DOT__request = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                vlTOPp->idma_research_frontend_top__DOT__request = 1U;
            }
        }
    }
    vlTOPp->batch_active_o = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                              & (0U != (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)));
    vlTOPp->request_o = vlTOPp->idma_research_frontend_top__DOT__request;
}

VL_INLINE_OPT void Vidma_research_frontend_top::_combo__TOP__3(Vidma_research_frontend_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_frontend_top::_combo__TOP__3\n"); );
    Vidma_research_frontend_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*223:0*/ __Vtemp28[7];
    WData/*287:0*/ __Vtemp29[9];
    WData/*287:0*/ __Vtemp30[9];
    WData/*351:0*/ __Vtemp31[11];
    // Body
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_n 
        = (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_q 
           + (QData)((IData)((1U & (~ (IData)(vlTOPp->backend_idle_i))))));
    vlTOPp->idma_research_frontend_top__DOT__req[0U] 
        = ((0xffffff00U & ((IData)(vlTOPp->wdata_i) 
                           << 8U)) | (IData)(vlTOPp->wstrb_i));
    vlTOPp->idma_research_frontend_top__DOT__req[1U] 
        = ((0xffU & ((IData)(vlTOPp->wdata_i) >> 0x18U)) 
           | (0xffffff00U & ((IData)((vlTOPp->wdata_i 
                                      >> 0x20U)) << 8U)));
    vlTOPp->idma_research_frontend_top__DOT__req[2U] 
        = ((0xfffe0000U & ((IData)(vlTOPp->valid_i) 
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
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_n 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_task_issue 
        = (((IData)(vlTOPp->batch_active_o) & (1U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) 
           & (IData)(vlTOPp->ready_i));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_task_done 
        = (((IData)(vlTOPp->batch_active_o) & (2U == (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) 
           & (IData)(vlTOPp->complete_i));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access 
        = (1U & ((vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                  >> 0x11U) & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                               >> 0xfU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write 
        = (((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) 
              & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                 >> 0x10U)) & (0xffU == (0xffU & vlTOPp->idma_research_frontend_top__DOT__req[0U]))) 
            & (0x88U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                  >> 8U)))) & (2ULL 
                                               == (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__req[2U])) 
                                                    << 0x38U) 
                                                   | (((QData)((IData)(
                                                                       vlTOPp->idma_research_frontend_top__DOT__req[1U])) 
                                                       << 0x18U) 
                                                      | ((QData)((IData)(
                                                                         vlTOPp->idma_research_frontend_top__DOT__req[0U])) 
                                                         >> 8U)))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__observer_access 
        = (((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access)) 
            & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
               >> 0x11U)) & (((((0x58U == (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U))) 
                                | (0x60U == (0xffU 
                                             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))) 
                               | (0x68U == (0xffU & 
                                            (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                             >> 8U)))) 
                              | (0x70U == (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U)))) 
                             | (0x78U == (0xffU & (
                                                   vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                   >> 8U)))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__locked_write 
        = (((((IData)(vlTOPp->batch_active_o) & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                 >> 0x11U)) 
             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                >> 0x10U)) & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access))) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__observer_access)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U] 
        = vlTOPp->idma_research_frontend_top__DOT__req[0U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[1U] 
        = vlTOPp->idma_research_frontend_top__DOT__req[1U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
        = vlTOPp->idma_research_frontend_top__DOT__req[2U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
        = ((0x1ffffU & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U]) 
           | (0x20000U & (((vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                            & ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__observer_access)) 
                               << 0x11U)) & ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access)) 
                                             << 0x11U)) 
                          & ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__locked_write)) 
                             << 0x11U))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                  >> 0x11U) & (~ (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 0x10U))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                  >> 0x11U) & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                               >> 0x10U)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                              >> 8U))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                               >> 8U))) << 1U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 2U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 3U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 4U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 5U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 6U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 7U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 8U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 9U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_research_frontend_top__DOT__dut__DOT__base_req[0U]))))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err));
    __Vtemp28[0U] = (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp28[1U] = (IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                             >> 0x20U));
    __Vtemp28[2U] = (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp28[3U] = (IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                             >> 0x20U));
    __Vtemp28[4U] = (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp28[5U] = (IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                             >> 0x20U));
    __Vtemp28[6U] = ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 6U) & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re)) 
                     & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp29, __Vtemp28);
    __Vtemp30[0U] = __Vtemp29[0U];
    __Vtemp30[1U] = __Vtemp29[1U];
    __Vtemp30[2U] = __Vtemp29[2U];
    __Vtemp30[3U] = __Vtemp29[3U];
    __Vtemp30[4U] = __Vtemp29[4U];
    __Vtemp30[5U] = __Vtemp29[5U];
    __Vtemp30[6U] = __Vtemp29[6U];
    __Vtemp30[7U] = __Vtemp29[7U];
    __Vtemp30[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                      >> 4U) & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                << 1U)) 
                                    & ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                       << 1U))) | __Vtemp29[8U]);
    VL_EXTEND_WW(322,258, __Vtemp31, __Vtemp30);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0U] 
        = __Vtemp31[0U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[1U] 
        = __Vtemp31[1U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[2U] 
        = __Vtemp31[2U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[3U] 
        = __Vtemp31[3U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[4U] 
        = __Vtemp31[4U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[5U] 
        = __Vtemp31[5U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[6U] 
        = __Vtemp31[6U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[7U] 
        = __Vtemp31[7U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U] 
        = __Vtemp31[8U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[9U] 
        = __Vtemp31[9U];
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp31[0xaU])))))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->type_o = (3U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                             << 0x1aU) | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                          >> 6U)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_n 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if ((1U & (~ (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                               | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write)) 
                              | (IData)(vlTOPp->policy_abort_i))))) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                         & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                                            | (IData)(vlTOPp->policy_abort_i))))))) {
                            if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                 & (0U != vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q))) {
                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_n 
                                    = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                if ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_i))) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire = 1U;
                }
            } else {
                if ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_i))) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                                   | (IData)(vlTOPp->policy_abort_i))))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                                   | (IData)(vlTOPp->policy_abort_i))))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[3U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[4U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[5U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[6U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[7U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[8U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[7U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                               >> 0xcU))));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[8U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[5U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                               >> 0xcU))));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[6U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[3U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                               >> 0xcU))));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[4U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = (0xfffffffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = (0x10U | (0xf000000fU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U]));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = ((0xfffffff7U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U]) 
           | (8U & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                    >> 5U)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = ((0x7fffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]) 
           | (0xffff8000U & (((0x100U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])
                               ? 1U : 0U) << 0xfU)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = ((0xfffffff8U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[2U]) 
           | (0x7fffU & (((0x100U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])
                           ? 1U : 0U) >> 0x11U)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0x2000U | (0xffff9fffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = ((0xffffe1ffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]) 
           | (0x1e00U & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                         << 7U)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xfffffeffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xffffff1fU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xffffffe1U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0x1fffffffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xfffffffeU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0x8000000U | (0xe7ffffffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xf87fffffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (0x7800000U & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                            << 0x15U)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffbfffffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffc7ffffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xfff87fffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffff87ffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xfffffbffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xfffffdffU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (0x200U & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                        >> 2U)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xfffffe3fU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffffffc7U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xfffffffbU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (4U & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                    >> 8U)));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xfffffffdU & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (2U & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                    >> 9U)));
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[5U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q));
                vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[6U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q) 
                               >> 0x20U));
                vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[3U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q));
                vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[4U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q) 
                               >> 0x20U));
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q)) 
                  & (0ULL != vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__id_retire) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->done_o = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)
                       ? (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_task_done)
                       : (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__trans_complete));
    vlTOPp->length_o = (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[8U])) 
                         << 0x20U) | (QData)((IData)(
                                                     vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[7U])));
    vlTOPp->src_o = (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[6U])) 
                      << 0x20U) | (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[5U])));
    vlTOPp->dst_o = (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[4U])) 
                      << 0x20U) | (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__burst_req_o[3U])));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_n 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_n 
                        = (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q 
                           - 1ULL);
                }
            } else {
                if ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_n 
                                = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)
                                    ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q
                                    : (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[1U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0U]))));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                         & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_reverse_q))) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n 
                            = ((0ULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q)
                                ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q)) 
                                    - 1ULL) << 0xcU)
                                : (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q 
                                   - 0x1000ULL));
                    } else {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n 
                            = (0x1000ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_q);
                        if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                             & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n 
                                == ((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q)) 
                                    << 0xcU)))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n = 0ULL;
                        }
                    }
                }
            } else {
                if ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_offset_n 
                                = (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                    & (IData)(vlTOPp->policy_enable_i))
                                    ? (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                        & (IData)(vlTOPp->batch_reverse_i))
                                        ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q)) 
                                            - 1ULL) 
                                           << 0xcU)
                                        : 0ULL) : (
                                                   ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                                    & (IData)(vlTOPp->batch_reverse_i))
                                                    ? 
                                                   (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q)) 
                                                     - 1ULL) 
                                                    << 0xcU)
                                                    : 0ULL));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                if ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_i))) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 0U;
                } else {
                    if (((IData)(vlTOPp->backend_idle_i) 
                         & (IData)(vlTOPp->policy_allow_i))) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 1U;
                    }
                }
            } else {
                if ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                      | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_write)) 
                     | (IData)(vlTOPp->policy_abort_i))) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_countdown_q)) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n 
                            = (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->policy_enable_i))
                                ? 5U : 1U);
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n 
                        = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                        & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stop_q) 
                                           | (IData)(vlTOPp->policy_abort_i))))
                                     ? 0U : (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                              & (0U 
                                                 != vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q))
                                              ? 4U : 
                                             (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                               & (IData)(vlTOPp->policy_enable_i))
                                               ? 5U
                                               : 1U))));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 2U;
                }
            } else {
                if ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                 & (IData)(vlTOPp->policy_enable_i))) {
                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 5U;
                            } else {
                                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 1U;
                                if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                     & (~ (IData)(vlTOPp->ready_i)))) {
                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_n = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue = 1U;
                            if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                          & (IData)(vlTOPp->policy_enable_i))))) {
                                if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                     & (~ (IData)(vlTOPp->ready_i)))) {
                                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U]) 
           | (2U & ((~ (IData)(vlTOPp->backend_idle_i)) 
                    << 1U)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                  & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U] 
                                = ((1U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U]) 
                                   | (0xfffffffeU & 
                                      ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                       << 1U)));
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[5U] 
                                = ((1U & ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                          >> 0x1fU)) 
                                   | (0xfffffffeU & 
                                      ((IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                >> 0x20U)) 
                                       << 1U)));
                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U] 
                                = ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U]) 
                                   | (1U & ((IData)(
                                                    (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                     >> 0x20U)) 
                                            >> 0x1fU)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->issue_o = ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)
                        ? (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_task_issue)
                        : (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__issue));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next);
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = (IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                   >> 0x20U));
    vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_read 
        = (((vlTOPp->idma_research_frontend_top__DOT__req[2U] 
             >> 0x11U) & (~ (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                             >> 0x10U))) & (0x50U == 
                                            (0xffU 
                                             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U))));
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_research_frontend_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_read) {
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_q);
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->idma_research_frontend_top__DOT__dut__DOT__active_cycles_q 
                       >> 0x20U));
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__observer_access) {
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] = 0U;
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__locked_write) {
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_access) {
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] = 0U;
        if (((((((((0x80U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                       >> 8U))) | (0x88U 
                                                   == 
                                                   (0xffU 
                                                    & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                       >> 8U)))) 
                  | (0x90U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                        >> 8U)))) | 
                 (0x98U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                     >> 8U)))) | (0xa0U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                      >> 8U)))) 
               | (0xa8U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                     >> 8U)))) | (0xd0U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                      >> 8U)))) 
             | (0xd8U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                   >> 8U))))) {
            vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
                = (IData)(((0x80U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                               >> 8U)))
                            ? 0x49444d4142544301ULL
                            : ((0x88U == (0xffU & (
                                                   vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                   >> 8U)))
                                ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q)) 
                                    << 0x20U) | (QData)((IData)(
                                                                (((((0ULL 
                                                                     != vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q) 
                                                                    & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q 
                                                                       == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q)) 
                                                                   & (~ (IData)(vlTOPp->batch_active_o))) 
                                                                  << 3U) 
                                                                 | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q) 
                                                                     << 2U) 
                                                                    | (((IData)(vlTOPp->batch_active_o) 
                                                                        << 1U) 
                                                                       | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)))))))
                                : ((0x90U == (0xffU 
                                              & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                 >> 8U)))
                                    ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q
                                    : ((0x98U == (0xffU 
                                                  & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                     >> 8U)))
                                        ? (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q))
                                        : ((0xa0U == 
                                            (0xffU 
                                             & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))
                                            ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q
                                            : ((0xa8U 
                                                == 
                                                (0xffU 
                                                 & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                    >> 8U)))
                                                ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_issued_q
                                                : (
                                                   (0xd0U 
                                                    == 
                                                    (0xffU 
                                                     & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                        >> 8U)))
                                                    ? (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q))
                                                    : 0x49444d4147415001ULL))))))));
            vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
                = (IData)((((0x80U == (0xffU & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                >> 8U)))
                             ? 0x49444d4142544301ULL
                             : ((0x88U == (0xffU & 
                                           (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                            >> 8U)))
                                 ? (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_error_q)) 
                                     << 0x20U) | (QData)((IData)(
                                                                 (((((0ULL 
                                                                      != vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q) 
                                                                     & (vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q 
                                                                        == vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q)) 
                                                                    & (~ (IData)(vlTOPp->batch_active_o))) 
                                                                   << 3U) 
                                                                  | (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_stopped_q) 
                                                                      << 2U) 
                                                                     | (((IData)(vlTOPp->batch_active_o) 
                                                                         << 1U) 
                                                                        | (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q)))))))
                                 : ((0x90U == (0xffU 
                                               & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                  >> 8U)))
                                     ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_target_q
                                     : ((0x98U == (0xffU 
                                                   & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                      >> 8U)))
                                         ? (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_pages_q))
                                         : ((0xa0U 
                                             == (0xffU 
                                                 & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                    >> 8U)))
                                             ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_completed_q
                                             : ((0xa8U 
                                                 == 
                                                 (0xffU 
                                                  & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                     >> 8U)))
                                                 ? vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_issued_q
                                                 : 
                                                ((0xd0U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_research_frontend_top__DOT__req[2U] 
                                                      >> 8U)))
                                                  ? (QData)((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__gap_cycles_q))
                                                  : 0x49444d4147415001ULL))))))) 
                           >> 0x20U));
        } else {
            vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                = (1U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        }
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__state_q)))) {
                if ((2U & vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                    if (((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                         & (~ (IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_cfg_ok)))) {
                        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                            = (1U | vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                    } else {
                        if ((0ULL != (((QData)((IData)(
                                                       vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                       << 0x34U) | 
                                      (((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                        << 0x14U) | 
                                       ((QData)((IData)(
                                                        vlTOPp->idma_research_frontend_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                        >> 0xcU))))) {
                            vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                                = ((1U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]) 
                                   | (0xfffffffeU & 
                                      ((((IData)(vlTOPp->idma_research_frontend_top__DOT__dut__DOT__batch_enable_q) 
                                         & (IData)(vlTOPp->policy_enable_i)) 
                                        | (IData)(vlTOPp->ready_i)) 
                                       << 1U)));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->rdata_o = (((QData)((IData)(vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U])) 
                        << 0x20U) | (QData)((IData)(
                                                    vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U])));
    vlTOPp->ready_o = (1U & (vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                             >> 1U));
    vlTOPp->error_o = (1U & vlTOPp->idma_research_frontend_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
}

void Vidma_research_frontend_top::_eval(Vidma_research_frontend_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_frontend_top::_eval\n"); );
    Vidma_research_frontend_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

VL_INLINE_OPT QData Vidma_research_frontend_top::_change_request(Vidma_research_frontend_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_frontend_top::_change_request\n"); );
    Vidma_research_frontend_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vidma_research_frontend_top::_change_request_1(Vidma_research_frontend_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_frontend_top::_change_request_1\n"); );
    Vidma_research_frontend_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vidma_research_frontend_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_research_frontend_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((valid_i & 0xfeU))) {
        Verilated::overWidthError("valid_i");}
    if (VL_UNLIKELY((write_i & 0xfeU))) {
        Verilated::overWidthError("write_i");}
    if (VL_UNLIKELY((ready_i & 0xfeU))) {
        Verilated::overWidthError("ready_i");}
    if (VL_UNLIKELY((complete_i & 0xfeU))) {
        Verilated::overWidthError("complete_i");}
    if (VL_UNLIKELY((backend_idle_i & 0xfeU))) {
        Verilated::overWidthError("backend_idle_i");}
    if (VL_UNLIKELY((policy_enable_i & 0xfeU))) {
        Verilated::overWidthError("policy_enable_i");}
    if (VL_UNLIKELY((policy_allow_i & 0xfeU))) {
        Verilated::overWidthError("policy_allow_i");}
    if (VL_UNLIKELY((policy_abort_i & 0xfeU))) {
        Verilated::overWidthError("policy_abort_i");}
    if (VL_UNLIKELY((batch_reverse_i & 0xfeU))) {
        Verilated::overWidthError("batch_reverse_i");}
}
#endif  // VL_DEBUG
