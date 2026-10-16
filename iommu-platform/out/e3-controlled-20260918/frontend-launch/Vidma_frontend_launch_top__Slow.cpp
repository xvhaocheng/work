// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vidma_frontend_launch_top.h for the primary calling header

#include "Vidma_frontend_launch_top.h"
#include "Vidma_frontend_launch_top__Syms.h"

//==========

VL_CTOR_IMP(Vidma_frontend_launch_top) {
    Vidma_frontend_launch_top__Syms* __restrict vlSymsp = __VlSymsp = new Vidma_frontend_launch_top__Syms(this, name());
    Vidma_frontend_launch_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vidma_frontend_launch_top::__Vconfigure(Vidma_frontend_launch_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vidma_frontend_launch_top::~Vidma_frontend_launch_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vidma_frontend_launch_top::_settle__TOP__1(Vidma_frontend_launch_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_frontend_launch_top::_settle__TOP__1\n"); );
    Vidma_frontend_launch_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*223:0*/ __Vtemp9[7];
    WData/*287:0*/ __Vtemp10[9];
    WData/*287:0*/ __Vtemp11[9];
    WData/*351:0*/ __Vtemp12[11];
    // Body
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = (2U | vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__active_cycles_n 
        = (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__active_cycles_q 
           + (QData)((IData)((1U & (~ (IData)(vlTOPp->idle_i))))));
    vlTOPp->idma_frontend_launch_top__DOT__req[0U] 
        = (0xffU | (0xffffff00U & ((IData)(vlTOPp->wdata_i) 
                                   << 8U)));
    vlTOPp->idma_frontend_launch_top__DOT__req[1U] 
        = ((0xffU & ((IData)(vlTOPp->wdata_i) >> 0x18U)) 
           | (0xffffff00U & ((IData)((vlTOPp->wdata_i 
                                      >> 0x20U)) << 8U)));
    vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
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
    vlTOPp->launch_held_o = (3U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q));
    vlTOPp->launch_active_o = (((((3U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)) 
                                  | (1U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) 
                                 | (2U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) 
                                | (4U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) 
                               | (5U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)));
    vlTOPp->launch_cancelled_o = ((3U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)) 
                                  & (IData)(vlTOPp->cancel_i));
    vlTOPp->issue_id_o = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)
                           ? (1ULL + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_issued_q)
                           : (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                               | (IData)(vlTOPp->hold_i))
                               ? ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q)
                                   ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_q
                                   : vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q)
                               : vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q));
    vlTOPp->idma_frontend_launch_top__DOT__request = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                vlTOPp->idma_frontend_launch_top__DOT__request = 1U;
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o 
        = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
           & (0U != (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)));
    vlTOPp->cfg_write_o = (1U & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                 >> 0x10U));
    vlTOPp->cfg_addr_o = (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                   >> 8U));
    vlTOPp->cfg_wdata_o = (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__req[2U])) 
                            << 0x38U) | (((QData)((IData)(
                                                          vlTOPp->idma_frontend_launch_top__DOT__req[1U])) 
                                          << 0x18U) 
                                         | ((QData)((IData)(
                                                            vlTOPp->idma_frontend_launch_top__DOT__req[0U])) 
                                            >> 8U)));
    vlTOPp->cfg_wstrb_o = (0xffU & vlTOPp->idma_frontend_launch_top__DOT__req[0U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read 
        = (((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
             >> 0x11U) & (~ (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                             >> 0x10U))) & (0x28U == 
                                            (0xffU 
                                             & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                >> 8U))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access 
        = (((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
             >> 0x11U) & (0xb0U <= (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                             >> 8U)))) 
           & (~ ((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                  >> 0x11U) & ((0xd0U == (0xffU & (
                                                   vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                   >> 8U))) 
                               | (0xd8U == (0xffU & 
                                            (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                             >> 8U)))))));
    vlTOPp->request_o = vlTOPp->idma_frontend_launch_top__DOT__request;
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_task_done 
        = (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o) 
            & (2U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) 
           & (IData)(vlTOPp->complete_i));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_task_issue 
        = (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o) 
            & (1U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) 
           & (IData)(vlTOPp->ready_i));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access 
        = (1U & (((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                   >> 0x11U) & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                >> 0xfU)) & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write 
        = ((((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access) 
               & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                  >> 0x10U)) & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q))) 
             & (0xffU == (0xffU & vlTOPp->idma_frontend_launch_top__DOT__req[0U]))) 
            & (0x88U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                  >> 8U)))) & (2ULL 
                                               == (
                                                   ((QData)((IData)(
                                                                    vlTOPp->idma_frontend_launch_top__DOT__req[2U])) 
                                                    << 0x38U) 
                                                   | (((QData)((IData)(
                                                                       vlTOPp->idma_frontend_launch_top__DOT__req[1U])) 
                                                       << 0x18U) 
                                                      | ((QData)((IData)(
                                                                         vlTOPp->idma_frontend_launch_top__DOT__req[0U])) 
                                                         >> 8U)))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access 
        = ((((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access)) 
             & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access))) 
            & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
               >> 0x11U)) & (((((0x58U == (0xffU & 
                                           (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                            >> 8U))) 
                                | (0x60U == (0xffU 
                                             & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                >> 8U)))) 
                               | (0x68U == (0xffU & 
                                            (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                             >> 8U)))) 
                              | (0x70U == (0xffU & 
                                           (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                            >> 8U)))) 
                             | (0x78U == (0xffU & (
                                                   vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                   >> 8U)))));
    vlTOPp->cfg_valid_o = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access) 
                           | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__locked_write 
        = ((((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o) 
               & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                  >> 0x11U)) & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                >> 0x10U)) & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access))) 
            & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access))) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_locked_write 
        = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
           & (((((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                  >> 0x11U) & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                               >> 0x10U)) & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access))) 
               & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access))) 
              & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U] 
        = vlTOPp->idma_frontend_launch_top__DOT__req[0U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[1U] 
        = vlTOPp->idma_frontend_launch_top__DOT__req[1U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
        = vlTOPp->idma_frontend_launch_top__DOT__req[2U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
        = ((0x1ffffU & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U]) 
           | (0x20000U & ((((((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                               & ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access)) 
                                  << 0x11U)) & ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access)) 
                                                << 0x11U)) 
                             & ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access)) 
                                << 0x11U)) & ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__locked_write)) 
                                              << 0x11U)) 
                           & ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_locked_write)) 
                              << 0x11U)) & ((~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                                                & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) 
                                            << 0x11U))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re 
        = (1U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                  >> 0x11U) & (~ (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 0x10U))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we 
        = (1U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                  >> 0x11U) & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                               >> 0x10U)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit = 0U;
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3feU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | (0U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                              >> 8U))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fdU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((8U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                               >> 8U))) << 1U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3fbU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x10U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 2U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3f7U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x18U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 3U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3efU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x20U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 4U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3dfU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x28U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 5U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x3bfU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x30U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 6U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x37fU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x38U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 7U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x2ffU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x40U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 8U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit 
        = ((0x1ffU & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit)) 
           | ((0x48U == (0x7fU & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[2U] 
                                  >> 8U))) << 9U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err 
        = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we) 
           & (((((((((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       & (0U != (0xfU & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U])))) 
                      | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                          >> 1U) & (0U != (0xfU & (~ 
                                                   vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
                     | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                         >> 2U) & (0U != (0xfU & (~ 
                                                  vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
                    | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 3U) & (0U != (1U & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
                   | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                       >> 4U) & (0U != (1U & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
                  | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 5U) & (0U != (0xfU & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
                 | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 6U) & (0U != (0xfU & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
                | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                    >> 7U) & (0U != (0xfU & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
               | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                   >> 8U) & (0U != (0xfU & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))) 
              | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 9U) & (0U != (0xfU & (~ vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__base_req[0U]))))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__src_addr_we 
        = (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
            & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__dst_addr_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 1U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__num_bytes_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 2U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_decouple_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_deburst_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_serialize_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_pv_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_ttype_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_cache_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 3U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wdata_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 8U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__n_trans_we 
        = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
             >> 9U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we)) 
           & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U] 
        = ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U]) 
           | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err));
    __Vtemp9[0U] = (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q);
    __Vtemp9[1U] = (IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q 
                            >> 0x20U));
    __Vtemp9[2U] = (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q);
    __Vtemp9[3U] = (IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q 
                            >> 0x20U));
    __Vtemp9[4U] = (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q);
    __Vtemp9[5U] = (IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q 
                            >> 0x20U));
    __Vtemp9[6U] = ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                      >> 6U) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re)) 
                    & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)));
    VL_EXTEND_WW(257,193, __Vtemp10, __Vtemp9);
    __Vtemp11[0U] = __Vtemp10[0U];
    __Vtemp11[1U] = __Vtemp10[1U];
    __Vtemp11[2U] = __Vtemp10[2U];
    __Vtemp11[3U] = __Vtemp10[3U];
    __Vtemp11[4U] = __Vtemp10[4U];
    __Vtemp11[5U] = __Vtemp10[5U];
    __Vtemp11[6U] = __Vtemp10[6U];
    __Vtemp11[7U] = __Vtemp10[7U];
    __Vtemp11[8U] = ((0xffffffeU & ((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                      >> 4U) & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re) 
                                                << 1U)) 
                                    & ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err)) 
                                       << 1U))) | __Vtemp10[8U]);
    VL_EXTEND_WW(322,258, __Vtemp12, __Vtemp11);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0U] 
        = __Vtemp12[0U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[1U] 
        = __Vtemp12[1U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[2U] 
        = __Vtemp12[2U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[3U] 
        = __Vtemp12[3U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[4U] 
        = __Vtemp12[4U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[5U] 
        = __Vtemp12[5U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[6U] 
        = __Vtemp12[6U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[7U] 
        = __Vtemp12[7U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U] 
        = __Vtemp12[8U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[9U] 
        = __Vtemp12[9U];
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
        = ((0xfffff000U & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                           << 0xcU)) | ((0xfffff800U 
                                         & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q) 
                                            << 0xbU)) 
                                        | ((0xfffffc00U 
                                            & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q) 
                                               << 0xaU)) 
                                           | ((0xfffffe00U 
                                               & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q) 
                                                  << 9U)) 
                                              | ((0xffffff00U 
                                                  & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q) 
                                                     << 8U)) 
                                                 | ((0xffffffc0U 
                                                     & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q) 
                                                        << 6U)) 
                                                    | ((0xfffffffcU 
                                                        & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q) 
                                                           << 2U)) 
                                                       | __Vtemp12[0xaU])))))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU] 
        = ((0xfffU & ((IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q 
                               >> 0x20U)) >> 0x14U)) 
           | (0xfffff000U & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                             << 0xcU)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU] 
        = ((0xfffU & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q) 
                      >> 0x14U)) | (0xfffff000U & ((IData)(
                                                           (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                                                            >> 0x20U)) 
                                                   << 0xcU)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0x10U] 
        = (0xfffU & ((IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q 
                              >> 0x20U)) >> 0x14U));
    vlTOPp->type_o = (3U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                             << 0x1aU) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                          >> 6U)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_n 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_q;
    if ((4U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q) 
                              | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write))))) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_n 
                        = ((1U >= vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_q)
                            ? 0U : (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_q 
                                    - (IData)(1U)));
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q)))) {
                        if ((1U & (~ (((0ULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q) 
                                       | ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)) 
                                          & (0U == 
                                             (3U & 
                                              ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                << 0x1aU) 
                                               | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                  >> 6U)))))) 
                                      | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                         & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q)))))) {
                            if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                 & (0U != vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_cycles_q))) {
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_n 
                                    = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_cycles_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire = 0U;
    if ((4U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write))) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire = 1U;
                }
            } else {
                if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write))) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire = 1U;
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->cancel_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire = 1U;
                }
            } else {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q)))) {
                            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[3U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[4U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[5U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[6U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[7U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[8U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[7U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                               >> 0xcU))));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[8U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[5U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                               >> 0xcU))));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[6U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[3U] 
        = (IData)((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                               >> 0xcU))));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[4U] 
        = (IData)(((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                     << 0x34U) | (((QData)((IData)(
                                                   vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                   << 0x14U) | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                                >> 0xcU))) 
                   >> 0x20U));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = (0xfffffffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = (0x10U | (0xf000000fU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U]));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = ((0xfffffff7U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U]) 
           | (8U & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                    >> 5U)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = ((0x7fffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]) 
           | (0xffff8000U & (((0x100U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])
                               ? 1U : 0U) << 0xfU)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U] 
        = ((0xfffffff8U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[2U]) 
           | (0x7fffU & (((0x100U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])
                           ? 1U : 0U) >> 0x11U)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0x2000U | (0xffff9fffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = ((0xffffe1ffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]) 
           | (0x1e00U & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                         << 7U)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xfffffeffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xffffff1fU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xffffffe1U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0x1fffffffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U] 
        = (0xfffffffeU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[1U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0x8000000U | (0xe7ffffffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xf87fffffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (0x7800000U & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                            << 0x15U)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffbfffffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffc7ffffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xfff87fffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffff87ffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xfffffbffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xfffffdffU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (0x200U & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                        >> 2U)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xfffffe3fU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = (0xffffffc7U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]);
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xfffffffbU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (4U & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                    >> 8U)));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U] 
        = ((0xfffffffdU & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[0U]) 
           | (2U & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                    >> 9U)));
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[5U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q));
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[6U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0x10U])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q) 
                               >> 0x20U));
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[3U] 
                    = (IData)(((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                 << 0x34U) | (((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                               << 0x14U) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                                 >> 0xcU))) 
                               + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q));
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[4U] 
                    = (IData)((((((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU])) 
                                  << 0x34U) | (((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU])) 
                                                << 0x14U) 
                                               | ((QData)((IData)(
                                                                  vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                                  >> 0xcU))) 
                                + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q) 
                               >> 0x20U));
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__trans_complete = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__trans_complete = 1U;
                    } else {
                        if ((((0ULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q) 
                              | ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)) 
                                 & (0U == (3U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                  << 0x1aU) 
                                                 | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                    >> 6U)))))) 
                             | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q)))) {
                            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__trans_complete = 1U;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok 
        = (((((((((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_error_q)) 
                  & (0ULL != vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q)) 
                 & (0x100000000ULL >= vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q)) 
                & (0U != (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q))) 
               & (0x1000U >= (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q))) 
              & (0x1000ULL == (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                << 0x34U) | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) 
             & (0U == (3U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                              << 0x1aU) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                           >> 6U))))) 
            & (0U == (0xfffU & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU] 
                                 << 0x14U) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU] 
                                              >> 0xcU))))) 
           & (0U == (0xfffU & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU] 
                                << 0x14U) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU] 
                                             >> 0xcU)))));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok 
        = (((((((((((0U == (3U & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                   << 0x1aU) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                >> 6U)))) 
                    & (~ (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                          >> 8U))) & (0U == (0xfU & 
                                             ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                               << 0x1eU) 
                                              | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                 >> 2U))))) 
                  & (~ (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                        >> 0xbU))) & (~ (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                         >> 0xaU))) 
                & (~ (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                      >> 9U))) & (0x40ULL == (((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                               << 0x34U) 
                                              | (((QData)((IData)(
                                                                  vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                                  << 0x14U) 
                                                 | ((QData)((IData)(
                                                                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                    >> 0xcU))))) 
              & (0U == (0x3fU & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xfU] 
                                  << 0x14U) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xeU] 
                                               >> 0xcU))))) 
             & (0U == (0x3fU & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xdU] 
                                 << 0x14U) | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU] 
                                              >> 0xcU))))) 
            & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q))) 
           & (IData)(vlTOPp->idle_i));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q)) {
        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d 
            = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__id_retire) {
            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d 
                = (1ULL + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q);
        }
    }
    vlTOPp->length_o = (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[8U])) 
                         << 0x20U) | (QData)((IData)(
                                                     vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[7U])));
    vlTOPp->src_o = (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[6U])) 
                      << 0x20U) | (QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[5U])));
    vlTOPp->dst_o = (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[4U])) 
                      << 0x20U) | (QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o[3U])));
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) {
        vlTOPp->done_o = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_task_done;
        vlTOPp->done_id_o = (1ULL + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q);
    } else {
        vlTOPp->done_o = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__trans_complete;
        vlTOPp->done_id_o = (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                              & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__trans_complete))
                              ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_q
                              : ((0xffffffffffffffffULL 
                                  == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q)
                                  ? 2ULL : (1ULL + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q)));
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_n 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->cancel_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_n = 0U;
                }
            } else {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_n = 0U;
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_n = 1U;
                    }
                }
            }
        }
    }
    vlTOPp->launch_error_o = (((((((0U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)) 
                                   & (IData)(vlTOPp->hold_i)) 
                                  & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read)) 
                                 & ((IData)(vlTOPp->cancel_i) 
                                    | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))) 
                                | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                                   & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) 
                               | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_locked_write)) 
                              | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                                  & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access)) 
                                 & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                    >> 0x10U)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_n 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->cancel_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_n = 0ULL;
                }
            } else {
                if (vlTOPp->complete_i) {
                    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_n = 0ULL;
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_id_n 
                            = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q;
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_n 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_n 
                        = (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q 
                           - 1ULL);
                }
            } else {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_n 
                            = (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[1U])) 
                                << 0x20U) | (QData)((IData)(
                                                            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0U])));
                    }
                } else {
                    if ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                      & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_n 
                                    = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)
                                        ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q
                                        : (((QData)((IData)(
                                                            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[1U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0U]))));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                         & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_reverse_q))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n 
                            = ((0ULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q)
                                ? (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q)) 
                                    - 1ULL) << 0xcU)
                                : (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q 
                                   - 0x1000ULL));
                    } else {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n 
                            = (0x1000ULL + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q);
                        if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                             & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n 
                                == ((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q)) 
                                    << 0xcU)))) {
                            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n = 0ULL;
                        }
                    }
                }
            } else {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n = 0ULL;
                    }
                } else {
                    if ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                      & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n = 0ULL;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q;
    if ((4U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write))) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 0U;
                }
            } else {
                if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q) 
                     | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write))) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 0U;
                } else {
                    if ((1U >= vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_q)) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 1U;
                    }
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->cancel_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 0U;
                } else {
                    if (vlTOPp->go_i) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 1U;
                    }
                }
            } else {
                if (vlTOPp->complete_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n 
                        = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q)
                            ? 0U : ((((0ULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q) 
                                      | ((~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)) 
                                         & (0U == (3U 
                                                   & ((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU] 
                                                       << 0x1aU) 
                                                      | (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU] 
                                                         >> 6U)))))) 
                                     | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                        & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q)))
                                     ? 0U : (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                              & (0U 
                                                 != vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_cycles_q))
                                              ? 4U : 1U)));
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) {
                if (vlTOPp->ready_i) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 2U;
                }
            } else {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 3U;
                    }
                } else {
                    if ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                      & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 1U;
                                if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                     & (~ (IData)(vlTOPp->ready_i)))) {
                                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_n = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue = 1U;
                    }
                } else {
                    if ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                      & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue = 1U;
                                if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                     & (~ (IData)(vlTOPp->ready_i)))) {
                                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U] 
        = (1U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[5U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U] 
        = (2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U] 
        = (1U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[3U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U] 
        = (0xfffffffeU & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U] 
        = ((1U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U]) 
           | (2U & ((~ (IData)(vlTOPp->idle_i)) << 1U)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U] 
        = ((1U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U]) 
           | (0xfffffffeU & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                             << 1U)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[3U] 
        = ((1U & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q) 
                  >> 0x1fU)) | (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                                        >> 0x20U)) 
                                               << 1U)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U] 
        = ((0xfffffffeU & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U]) 
           | (1U & ((IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                             >> 0x20U)) >> 0x1fU)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U] 
        = (0xfffffffeU & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U]);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[0U] = 0U;
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[1U] = 0U;
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & (~ ((IData)(vlTOPp->cancel_i) 
                                  | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok)))))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U] 
                            = ((1U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U]) 
                               | (0xfffffffeU & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                                 << 1U)));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[5U] 
                            = ((1U & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                      >> 0x1fU)) | 
                               (0xfffffffeU & ((IData)(
                                                       (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                        >> 0x20U)) 
                                               << 1U)));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U] 
                            = ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U]) 
                               | (1U & ((IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                 >> 0x20U)) 
                                        >> 0x1fU)));
                    }
                } else {
                    if ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                                      & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok)))))) {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U] 
                                    = ((1U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U]) 
                                       | (0xfffffffeU 
                                          & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                             << 1U)));
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[5U] 
                                    = ((1U & ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q) 
                                              >> 0x1fU)) 
                                       | (0xfffffffeU 
                                          & ((IData)(
                                                     (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                      >> 0x20U)) 
                                             << 1U)));
                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U] 
                                    = ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U]) 
                                       | (1U & ((IData)(
                                                        (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q 
                                                         >> 0x20U)) 
                                                >> 0x1fU)));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q;
    if ((0xffffffffffffffffULL == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q)) {
        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d 
            = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue)
                ? 2ULL : 1ULL);
    } else {
        if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue) {
            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d 
                = (1ULL + vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q);
        }
    }
    vlTOPp->issue_o = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)
                        ? (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_task_issue)
                        : (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                            | (IData)(vlTOPp->hold_i))
                            ? (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
                                & (1U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q))) 
                               & (IData)(vlTOPp->ready_i))
                            : (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__issue)));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next = 0ULL;
    if ((1U & ((((((((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                        >> 1U)) | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                   >> 2U)) | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                              >> 3U)) 
                  | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                     >> 4U)) | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                >> 5U)) | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                                           >> 6U)) 
               | ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit) 
                  >> 7U)))) {
        if ((1U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q;
        } else {
            if ((2U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                    = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q;
            } else {
                if ((4U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q;
                } else {
                    if ((8U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))) {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffeULL 
                                & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | (IData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q)));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffdULL 
                                & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q)) 
                                  << 1U));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffffbULL 
                                & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q)) 
                                  << 2U));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffff7ULL 
                                & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q)) 
                                  << 3U));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xffffffffffffffcfULL 
                                & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q)) 
                                  << 4U));
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0xfffffffffffffc3fULL 
                                & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                               | ((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q)) 
                                  << 6U));
                    } else {
                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                            = ((0x10U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                                ? ((0xfffffffffffffffeULL 
                                    & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next) 
                                   | (IData)((IData)(
                                                     (1U 
                                                      & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U] 
                                                         >> 1U)))))
                                : ((0x20U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                                    ? (((QData)((IData)(
                                                        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[6U])) 
                                        << 0x3fU) | 
                                       (((QData)((IData)(
                                                         vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[5U])) 
                                         << 0x1fU) 
                                        | ((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U])) 
                                           >> 1U)))
                                    : ((0x40U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                                        ? (((QData)((IData)(
                                                            vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[4U])) 
                                            << 0x3fU) 
                                           | (((QData)((IData)(
                                                               vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[3U])) 
                                               << 0x1fU) 
                                              | ((QData)((IData)(
                                                                 vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg[2U])) 
                                                 >> 1U)))
                                        : vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q)));
                    }
                }
            }
        }
    } else {
        vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
            = ((0x100U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q
                : ((0x200U & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit))
                    ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q
                    : 0xffffffffffffffffULL));
    }
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U] 
        = (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next);
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U] 
        = (IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next 
                   >> 0x20U));
    vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__active_cycles_read 
        = (((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
             >> 0x11U) & (~ (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                             >> 0x10U))) & (0x50U == 
                                            (0xffU 
                                             & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                >> 8U))));
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[0U];
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[1U];
    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
        = vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp[2U];
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__active_cycles_read) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__active_cycles_q);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__active_cycles_q 
                       >> 0x20U));
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->obs_cfg_rdata_i);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->obs_cfg_rdata_i >> 0x20U));
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
            = (IData)(vlTOPp->obs_cfg_rdata_i);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
            = (IData)((vlTOPp->obs_cfg_rdata_i >> 0x20U));
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__locked_write) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    }
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_locked_write) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] = 0U;
    }
    if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
         & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] = 0U;
    }
    if (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access) {
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
            = (2U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] = 0U;
        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] = 0U;
        if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q) 
             & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                >> 0x10U))) {
            vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
        } else {
            if (((((((((0x80U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                           >> 8U))) 
                       | (0x88U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                             >> 8U)))) 
                      | (0x90U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                            >> 8U)))) 
                     | (0x98U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                           >> 8U)))) 
                    | (0xa0U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                          >> 8U)))) 
                   | (0xa8U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                         >> 8U)))) 
                  | (0xd0U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                        >> 8U)))) | 
                 (0xd8U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                     >> 8U))))) {
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U] 
                    = (IData)(((0x80U == (0xffU & (
                                                   vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                   >> 8U)))
                                ? 0x49444d4142544301ULL
                                : ((0x88U == (0xffU 
                                              & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                 >> 8U)))
                                    ? (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_error_q)) 
                                        << 0x20U) | (QData)((IData)(
                                                                    (((((0ULL 
                                                                         != vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q) 
                                                                        & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q 
                                                                           == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q)) 
                                                                       & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o))) 
                                                                      << 3U) 
                                                                     | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stopped_q) 
                                                                         << 2U) 
                                                                        | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o) 
                                                                            << 1U) 
                                                                           | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)))))))
                                    : ((0x90U == (0xffU 
                                                  & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                     >> 8U)))
                                        ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q
                                        : ((0x98U == 
                                            (0xffU 
                                             & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                >> 8U)))
                                            ? (QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q))
                                            : ((0xa0U 
                                                == 
                                                (0xffU 
                                                 & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                    >> 8U)))
                                                ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q
                                                : (
                                                   (0xa8U 
                                                    == 
                                                    (0xffU 
                                                     & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                        >> 8U)))
                                                    ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_issued_q
                                                    : 
                                                   ((0xd0U 
                                                     == 
                                                     (0xffU 
                                                      & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                         >> 8U)))
                                                     ? (QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_cycles_q))
                                                     : 0x49444d4147415001ULL))))))));
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U] 
                    = (IData)((((0x80U == (0xffU & 
                                           (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                            >> 8U)))
                                 ? 0x49444d4142544301ULL
                                 : ((0x88U == (0xffU 
                                               & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                  >> 8U)))
                                     ? (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_error_q)) 
                                         << 0x20U) 
                                        | (QData)((IData)(
                                                          (((((0ULL 
                                                               != vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q) 
                                                              & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q 
                                                                 == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q)) 
                                                             & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o))) 
                                                            << 3U) 
                                                           | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_stopped_q) 
                                                               << 2U) 
                                                              | (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_active_o) 
                                                                  << 1U) 
                                                                 | (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)))))))
                                     : ((0x90U == (0xffU 
                                                   & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                      >> 8U)))
                                         ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_target_q
                                         : ((0x98U 
                                             == (0xffU 
                                                 & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                    >> 8U)))
                                             ? (QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q))
                                             : ((0xa0U 
                                                 == 
                                                 (0xffU 
                                                  & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                     >> 8U)))
                                                 ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q
                                                 : 
                                                ((0xa8U 
                                                  == 
                                                  (0xffU 
                                                   & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                      >> 8U)))
                                                  ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_issued_q
                                                  : 
                                                 ((0xd0U 
                                                   == 
                                                   (0xffU 
                                                    & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                                       >> 8U)))
                                                   ? (QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__gap_cycles_q))
                                                   : 0x49444d4147415001ULL))))))) 
                               >> 0x20U));
            } else {
                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                    = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
            }
        }
    }
    if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                  >> 2U)))) {
        if ((1U & (~ ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)))) {
                if (((IData)(vlTOPp->hold_i) & (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read))) {
                    if ((1U & ((IData)(vlTOPp->cancel_i) 
                               | (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok))))) {
                        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                            = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                    } else {
                        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                            = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                        vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                            = (2U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                    }
                } else {
                    if ((2U & vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[8U])) {
                        if (((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q) 
                             & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok)))) {
                            vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                                = (2U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                            vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                                = (1U | vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
                        } else {
                            if ((0ULL != (((QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xcU])) 
                                           << 0x34U) 
                                          | (((QData)((IData)(
                                                              vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xbU])) 
                                              << 0x14U) 
                                             | ((QData)((IData)(
                                                                vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw[0xaU])) 
                                                >> 0xcU))))) {
                                vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                                    = ((1U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]) 
                                       | (0xfffffffeU 
                                          & ((IData)(vlTOPp->ready_i) 
                                             << 1U)));
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->rdata_o = (((QData)((IData)(vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U])) 
                        << 0x20U) | (QData)((IData)(
                                                    vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U])));
    vlTOPp->ready_o = (1U & (vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                             >> 1U));
    vlTOPp->error_o = (1U & vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U]);
    vlTOPp->poll_o = (((((((vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                            >> 0x11U) & (~ (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                            >> 0x10U))) 
                          & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_access))) 
                         & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__observer_access))) 
                        & (~ (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__launcher_access))) 
                       & (0x30U == (0xffU & (vlTOPp->idma_frontend_launch_top__DOT__req[2U] 
                                             >> 8U)))) 
                      & (vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[2U] 
                         >> 1U));
    vlTOPp->poll_id_o = ((IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q)
                          ? (((0U == (IData)(vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__state_q)) 
                              & (vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q 
                                 == vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_id_q))
                              ? vlTOPp->idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q
                              : 0ULL) : (((QData)((IData)(
                                                          vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[1U])) 
                                          << 0x20U) 
                                         | (QData)((IData)(
                                                           vlTOPp->idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o[0U]))));
}

void Vidma_frontend_launch_top::_eval_initial(Vidma_frontend_launch_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_frontend_launch_top::_eval_initial\n"); );
    Vidma_frontend_launch_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

void Vidma_frontend_launch_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_frontend_launch_top::final\n"); );
    // Variables
    Vidma_frontend_launch_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vidma_frontend_launch_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vidma_frontend_launch_top::_eval_settle(Vidma_frontend_launch_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_frontend_launch_top::_eval_settle\n"); );
    Vidma_frontend_launch_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__1(vlSymsp);
}

void Vidma_frontend_launch_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vidma_frontend_launch_top::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    valid_i = VL_RAND_RESET_I(1);
    write_i = VL_RAND_RESET_I(1);
    addr_i = VL_RAND_RESET_I(8);
    wdata_i = VL_RAND_RESET_Q(64);
    ready_i = VL_RAND_RESET_I(1);
    complete_i = VL_RAND_RESET_I(1);
    idle_i = VL_RAND_RESET_I(1);
    hold_i = VL_RAND_RESET_I(1);
    go_i = VL_RAND_RESET_I(1);
    cancel_i = VL_RAND_RESET_I(1);
    obs_cfg_rdata_i = VL_RAND_RESET_Q(64);
    rdata_o = VL_RAND_RESET_Q(64);
    ready_o = VL_RAND_RESET_I(1);
    error_o = VL_RAND_RESET_I(1);
    issue_o = VL_RAND_RESET_I(1);
    done_o = VL_RAND_RESET_I(1);
    poll_o = VL_RAND_RESET_I(1);
    issue_id_o = VL_RAND_RESET_Q(64);
    done_id_o = VL_RAND_RESET_Q(64);
    poll_id_o = VL_RAND_RESET_Q(64);
    launch_held_o = VL_RAND_RESET_I(1);
    launch_active_o = VL_RAND_RESET_I(1);
    launch_cancelled_o = VL_RAND_RESET_I(1);
    launch_error_o = VL_RAND_RESET_I(1);
    cfg_valid_o = VL_RAND_RESET_I(1);
    cfg_write_o = VL_RAND_RESET_I(1);
    cfg_addr_o = VL_RAND_RESET_I(8);
    cfg_wdata_o = VL_RAND_RESET_Q(64);
    cfg_wstrb_o = VL_RAND_RESET_I(8);
    request_o = VL_RAND_RESET_I(1);
    length_o = VL_RAND_RESET_Q(64);
    type_o = VL_RAND_RESET_I(2);
    src_o = VL_RAND_RESET_Q(64);
    dst_o = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(82, idma_frontend_launch_top__DOT__req);
    idma_frontend_launch_top__DOT__request = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(288, idma_frontend_launch_top__DOT____Vcellout__dut__burst_req_o);
    VL_RAND_RESET_W(66, idma_frontend_launch_top__DOT____Vcellout__dut__dma_ctrl_rsp_o);
    idma_frontend_launch_top__DOT__dut__DOT__batch_active_o = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__state_q = VL_RAND_RESET_I(3);
    idma_frontend_launch_top__DOT__dut__DOT__state_n = VL_RAND_RESET_I(3);
    idma_frontend_launch_top__DOT__dut__DOT__transf_counter_q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__transf_counter_n = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__transf_offset_q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__transf_offset_n = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(524, idma_frontend_launch_top__DOT__dut__DOT__dma_reg2hw);
    VL_RAND_RESET_W(194, idma_frontend_launch_top__DOT__dut__DOT__dma_hw2reg);
    idma_frontend_launch_top__DOT__dut__DOT__active_cycles_q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__active_cycles_n = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__issue = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__trans_complete = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__id_retire = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__active_cycles_read = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(82, idma_frontend_launch_top__DOT__dut__DOT__base_req);
    idma_frontend_launch_top__DOT__dut__DOT__observer_access = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_access = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__locked_write = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_enable_q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_stop_q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_stopped_q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_error_q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_target_q = VL_RAND_RESET_Q(33);
    idma_frontend_launch_top__DOT__dut__DOT__batch_issued_q = VL_RAND_RESET_Q(33);
    idma_frontend_launch_top__DOT__dut__DOT__batch_completed_q = VL_RAND_RESET_Q(33);
    idma_frontend_launch_top__DOT__dut__DOT__batch_pages_q = VL_RAND_RESET_I(13);
    idma_frontend_launch_top__DOT__dut__DOT__batch_reverse_q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_id_q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__batch_cfg_ok = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_task_issue = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__batch_task_done = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__gap_cycles_q = VL_RAND_RESET_I(32);
    idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_q = VL_RAND_RESET_I(32);
    idma_frontend_launch_top__DOT__dut__DOT__gap_countdown_n = VL_RAND_RESET_I(32);
    idma_frontend_launch_top__DOT__dut__DOT__batch_stop_write = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launcher_access = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launch_cfg_ok = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launch_next_id_read = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launch_locked_write = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launch_ctrl_n = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__launch_id_q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__launch_id_n = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_re = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wr_err = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_rdata_next = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(66, idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__reg_intf_rsp);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__src_addr_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__dst_addr_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__num_bytes_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_decouple_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_deburst_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_serialize_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_pv_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_cache_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__conf_ttype_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__wdata_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__n_trans_we = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = VL_RAND_RESET_I(1);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = VL_RAND_RESET_I(2);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = VL_RAND_RESET_I(4);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_dma_conf_regs__DOT__addr_hit = VL_RAND_RESET_I(10);
    idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_d = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__next_q = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_d = VL_RAND_RESET_Q(64);
    idma_frontend_launch_top__DOT__dut__DOT__i_idma_transfer_id_gen__DOT__completed_q = VL_RAND_RESET_Q(64);
}
