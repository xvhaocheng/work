// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vbanked_ctl_test_top.h for the primary calling header

#include "Vbanked_ctl_test_top_axi_demux_id_counters__pi22.h"
#include "Vbanked_ctl_test_top__Syms.h"

//==========

VL_INLINE_OPT void Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__3(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__3\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__push_en = ((IData)(vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up)
                             ? (0xffffU & ((IData)(1U) 
                                           << (0xfU 
                                               & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[0xbU] 
                                                  >> 0x13U))))
                             : 0U);
    this->__PVT__pop_en = ((1U & ((vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                   >> 0x10U) & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[4U] 
                                                >> 0x13U)))
                            ? (0xffffU & ((IData)(1U) 
                                          << (0xfU 
                                              & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                                 >> 0xcU))))
                            : 0U);
    this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((1U & (IData)(this->__PVT__push_en))
                ? (~ (IData)(this->__PVT__pop_en)) : (IData)(this->__PVT__pop_en)))) {
        this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ (IData)(this->__PVT__push_en)) 
                              & (IData)(this->__PVT__pop_en)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((1U & (IData)(this->__PVT__push_en))
                               ? ((1U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((1U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((1U & (IData)(this->__PVT__push_en))
                               ? ((1U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((1U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((2U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 1U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 1U)))) {
        this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 1U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 1U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((2U & (IData)(this->__PVT__push_en))
                               ? ((2U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((2U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((2U & (IData)(this->__PVT__push_en))
                               ? ((2U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((2U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((4U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 2U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 2U)))) {
        this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 2U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 2U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((4U & (IData)(this->__PVT__push_en))
                               ? ((4U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((4U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((4U & (IData)(this->__PVT__push_en))
                               ? ((4U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((4U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((8U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 3U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 3U)))) {
        this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 3U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 3U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((8U & (IData)(this->__PVT__push_en))
                               ? ((8U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((8U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((8U & (IData)(this->__PVT__push_en))
                               ? ((8U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((8U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x10U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 4U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 4U)))) {
        this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 4U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 4U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x10U & (IData)(this->__PVT__push_en))
                               ? ((0x10U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x10U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x10U & (IData)(this->__PVT__push_en))
                               ? ((0x10U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x10U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x20U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 5U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 5U)))) {
        this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 5U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 5U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x20U & (IData)(this->__PVT__push_en))
                               ? ((0x20U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x20U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x20U & (IData)(this->__PVT__push_en))
                               ? ((0x20U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x20U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x40U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 6U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 6U)))) {
        this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 6U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 6U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x40U & (IData)(this->__PVT__push_en))
                               ? ((0x40U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x40U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x40U & (IData)(this->__PVT__push_en))
                               ? ((0x40U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x40U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x80U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 7U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 7U)))) {
        this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 7U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 7U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x80U & (IData)(this->__PVT__push_en))
                               ? ((0x80U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x80U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x80U & (IData)(this->__PVT__push_en))
                               ? ((0x80U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x80U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x100U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 8U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 8U)))) {
        this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 8U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 8U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x100U & (IData)(this->__PVT__push_en))
                               ? ((0x100U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x100U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x100U & (IData)(this->__PVT__push_en))
                               ? ((0x100U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x100U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x200U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 9U)) : ((IData)(this->__PVT__pop_en) 
                                 >> 9U)))) {
        this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 9U)) & ((IData)(this->__PVT__pop_en) 
                                             >> 9U)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x200U & (IData)(this->__PVT__push_en))
                               ? ((0x200U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x200U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x200U & (IData)(this->__PVT__push_en))
                               ? ((0x200U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x200U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x400U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 0xaU)) : ((IData)(this->__PVT__pop_en) 
                                   >> 0xaU)))) {
        this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 0xaU)) & ((IData)(this->__PVT__pop_en) 
                                               >> 0xaU)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x400U & (IData)(this->__PVT__push_en))
                               ? ((0x400U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x400U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x400U & (IData)(this->__PVT__push_en))
                               ? ((0x400U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x400U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x800U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 0xbU)) : ((IData)(this->__PVT__pop_en) 
                                   >> 0xbU)))) {
        this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 0xbU)) & ((IData)(this->__PVT__pop_en) 
                                               >> 0xbU)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x800U & (IData)(this->__PVT__push_en))
                               ? ((0x800U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x800U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x800U & (IData)(this->__PVT__push_en))
                               ? ((0x800U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x800U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x1000U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 0xcU)) : ((IData)(this->__PVT__pop_en) 
                                   >> 0xcU)))) {
        this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 0xcU)) & ((IData)(this->__PVT__pop_en) 
                                               >> 0xcU)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x1000U & (IData)(this->__PVT__push_en))
                               ? ((0x1000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x1000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x1000U & (IData)(this->__PVT__push_en))
                               ? ((0x1000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x1000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x2000U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 0xdU)) : ((IData)(this->__PVT__pop_en) 
                                   >> 0xdU)))) {
        this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 0xdU)) & ((IData)(this->__PVT__pop_en) 
                                               >> 0xdU)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x2000U & (IData)(this->__PVT__push_en))
                               ? ((0x2000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x2000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x2000U & (IData)(this->__PVT__push_en))
                               ? ((0x2000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x2000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x4000U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 0xeU)) : ((IData)(this->__PVT__pop_en) 
                                   >> 0xeU)))) {
        this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 0xeU)) & ((IData)(this->__PVT__pop_en) 
                                               >> 0xeU)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x4000U & (IData)(this->__PVT__push_en))
                               ? ((0x4000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x4000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x4000U & (IData)(this->__PVT__push_en))
                               ? ((0x4000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x4000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
    this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if ((1U & ((0x8000U & (IData)(this->__PVT__push_en))
                ? (~ ((IData)(this->__PVT__pop_en) 
                      >> 0xfU)) : ((IData)(this->__PVT__pop_en) 
                                   >> 0xfU)))) {
        this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((1U & ((~ ((IData)(this->__PVT__push_en) 
                                  >> 0xfU)) & ((IData)(this->__PVT__pop_en) 
                                               >> 0xfU)))
                        ? ((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - ((0x8000U & (IData)(this->__PVT__push_en))
                               ? ((0x8000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x8000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))
                        : ((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + ((0x8000U & (IData)(this->__PVT__push_en))
                               ? ((0x8000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U) : ((0x8000U 
                                                  & (IData)(this->__PVT__pop_en))
                                                  ? 1U
                                                  : 0U)))));
    }
}

VL_INLINE_OPT void Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_sequent__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__6(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_sequent__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__6\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_ni) {
        this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d;
    } else {
        this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfffeU & (IData)(this->__PVT__mst_select_q)) 
                                         | (1U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                                  >> 0x15U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfffeU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((2U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfffdU & (IData)(this->__PVT__mst_select_q)) 
                                         | (2U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                                  >> 0x14U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfffdU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((4U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfffbU & (IData)(this->__PVT__mst_select_q)) 
                                         | (4U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                                  >> 0x13U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfffbU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((8U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfff7U & (IData)(this->__PVT__mst_select_q)) 
                                         | (8U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                                  >> 0x12U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfff7U & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x10U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xffefU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x10U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                             >> 0x11U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xffefU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x20U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xffdfU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x20U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                             >> 0x10U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xffdfU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x40U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xffbfU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x40U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                             >> 0xfU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xffbfU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x80U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xff7fU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x80U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                             >> 0xeU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xff7fU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x100U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfeffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x100U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 0xdU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfeffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x200U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfdffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x200U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 0xcU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfdffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x400U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfbffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x400U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 0xbU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfbffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x800U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xf7ffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x800U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 0xaU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xf7ffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x1000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xefffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x1000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 9U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xefffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x2000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xdfffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x2000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 8U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xdfffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x4000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xbfffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x4000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 7U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xbfffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x8000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0x7fffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x8000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[0xbU] 
                                               >> 6U)));
        }
    } else {
        this->__PVT__mst_select_q = (0x7fffU & (IData)(this->__PVT__mst_select_q));
    }
    this->__PVT__cnt_full = ((0x7fffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x8000U & ((((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xfU)));
    this->__PVT__occupied = ((0x7fffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xfU));
    this->__PVT__cnt_full = ((0xbfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x4000U & ((((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xeU)));
    this->__PVT__occupied = ((0xbfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xeU));
    this->__PVT__cnt_full = ((0xdfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x2000U & ((((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xdU)));
    this->__PVT__occupied = ((0xdfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xdU));
    this->__PVT__cnt_full = ((0xefffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x1000U & ((((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xcU)));
    this->__PVT__occupied = ((0xefffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xcU));
    this->__PVT__cnt_full = ((0xf7ffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x800U & ((((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xbU)));
    this->__PVT__occupied = ((0xf7ffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xbU));
    this->__PVT__cnt_full = ((0xfbffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x400U & ((((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xaU)));
    this->__PVT__occupied = ((0xfbffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xaU));
    this->__PVT__cnt_full = ((0xfdffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x200U & ((((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 9U)));
    this->__PVT__occupied = ((0xfdffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 9U));
    this->__PVT__cnt_full = ((0xfeffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x100U & ((((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 8U)));
    this->__PVT__occupied = ((0xfeffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 8U));
    this->__PVT__cnt_full = ((0xff7fU & (IData)(this->__PVT__cnt_full)) 
                             | (0x80U & ((((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 7U)));
    this->__PVT__occupied = ((0xff7fU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 7U));
    this->__PVT__cnt_full = ((0xffbfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x40U & ((((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 6U)));
    this->__PVT__occupied = ((0xffbfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 6U));
    this->__PVT__cnt_full = ((0xffdfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x20U & ((((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 5U)));
    this->__PVT__occupied = ((0xffdfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 5U));
    this->__PVT__cnt_full = ((0xffefU & (IData)(this->__PVT__cnt_full)) 
                             | (0x10U & ((((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 4U)));
    this->__PVT__occupied = ((0xffefU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 4U));
    this->__PVT__cnt_full = ((0xfff7U & (IData)(this->__PVT__cnt_full)) 
                             | (8U & ((((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 3U)));
    this->__PVT__occupied = ((0xfff7U & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 3U));
    this->__PVT__cnt_full = ((0xfffbU & (IData)(this->__PVT__cnt_full)) 
                             | (4U & ((((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 2U)));
    this->__PVT__occupied = ((0xfffbU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 2U));
    this->__PVT__cnt_full = ((0xfffdU & (IData)(this->__PVT__cnt_full)) 
                             | (2U & ((((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 1U)));
    this->__PVT__occupied = ((0xfffdU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 1U));
    this->__PVT__cnt_full = ((0xfffeU & (IData)(this->__PVT__cnt_full)) 
                             | (1U & (((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                       >> 3U) | (7U 
                                                 == 
                                                 (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))))));
    this->__PVT__occupied = ((0xfffeU & (IData)(this->__PVT__occupied)) 
                             | (0U != (7U & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))));
}

VL_INLINE_OPT void Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__8(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__8\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->lookup_mst_select_o = (1U & ((IData)(this->__PVT__mst_select_q) 
                                       >> (0xfU & (
                                                   vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[0xbU] 
                                                   >> 0x13U))));
    this->lookup_mst_select_occupied_o = (1U & ((IData)(this->__PVT__occupied) 
                                                >> 
                                                (0xfU 
                                                 & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[0xbU] 
                                                    >> 0x13U))));
}

VL_INLINE_OPT void Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_sequent__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__5(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_sequent__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__5\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_ni) {
        this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q 
            = this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d;
    } else {
        this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
        this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfffeU & (IData)(this->__PVT__mst_select_q)) 
                                         | (1U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                                  >> 0x11U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfffeU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((2U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfffdU & (IData)(this->__PVT__mst_select_q)) 
                                         | (2U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                                  >> 0x10U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfffdU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((4U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfffbU & (IData)(this->__PVT__mst_select_q)) 
                                         | (4U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                                  >> 0xfU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfffbU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((8U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfff7U & (IData)(this->__PVT__mst_select_q)) 
                                         | (8U & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                                  >> 0xeU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfff7U & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x10U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xffefU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x10U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                             >> 0xdU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xffefU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x20U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xffdfU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x20U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                             >> 0xcU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xffdfU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x40U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xffbfU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x40U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                             >> 0xbU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xffbfU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x80U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xff7fU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x80U & 
                                            (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                             >> 0xaU)));
        }
    } else {
        this->__PVT__mst_select_q = (0xff7fU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x100U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfeffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x100U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 9U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfeffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x200U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfdffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x200U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 8U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfdffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x400U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xfbffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x400U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 7U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xfbffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x800U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xf7ffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x800U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 6U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xf7ffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x1000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xefffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x1000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 5U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xefffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x2000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xdfffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x2000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 4U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xdfffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x4000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0xbfffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x4000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 3U)));
        }
    } else {
        this->__PVT__mst_select_q = (0xbfffU & (IData)(this->__PVT__mst_select_q));
    }
    if (vlTOPp->rst_ni) {
        if ((0x8000U & (IData)(this->__PVT__push_en))) {
            this->__PVT__mst_select_q = ((0x7fffU & (IData)(this->__PVT__mst_select_q)) 
                                         | (0x8000U 
                                            & (vlTOPp->banked_ctl_test_top__DOT__tr_req[4U] 
                                               >> 2U)));
        }
    } else {
        this->__PVT__mst_select_q = (0x7fffU & (IData)(this->__PVT__mst_select_q));
    }
    this->__PVT__occupied = ((0x7fffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xfU));
    this->__PVT__cnt_full = ((0x7fffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x8000U & ((((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xfU)));
    this->__PVT__occupied = ((0xbfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xeU));
    this->__PVT__cnt_full = ((0xbfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x4000U & ((((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xeU)));
    this->__PVT__occupied = ((0xdfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xdU));
    this->__PVT__cnt_full = ((0xdfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x2000U & ((((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xdU)));
    this->__PVT__occupied = ((0xefffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xcU));
    this->__PVT__cnt_full = ((0xefffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x1000U & ((((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xcU)));
    this->__PVT__occupied = ((0xf7ffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xbU));
    this->__PVT__cnt_full = ((0xf7ffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x800U & ((((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xbU)));
    this->__PVT__occupied = ((0xfbffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xaU));
    this->__PVT__cnt_full = ((0xfbffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x400U & ((((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xaU)));
    this->__PVT__occupied = ((0xfdffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 9U));
    this->__PVT__cnt_full = ((0xfdffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x200U & ((((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 9U)));
    this->__PVT__occupied = ((0xfeffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 8U));
    this->__PVT__cnt_full = ((0xfeffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x100U & ((((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 8U)));
    this->__PVT__occupied = ((0xff7fU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 7U));
    this->__PVT__cnt_full = ((0xff7fU & (IData)(this->__PVT__cnt_full)) 
                             | (0x80U & ((((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 7U)));
    this->__PVT__occupied = ((0xffbfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 6U));
    this->__PVT__cnt_full = ((0xffbfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x40U & ((((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 6U)));
    this->__PVT__occupied = ((0xffdfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 5U));
    this->__PVT__cnt_full = ((0xffdfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x20U & ((((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 5U)));
    this->__PVT__occupied = ((0xffefU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 4U));
    this->__PVT__cnt_full = ((0xffefU & (IData)(this->__PVT__cnt_full)) 
                             | (0x10U & ((((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 4U)));
    this->__PVT__occupied = ((0xfff7U & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 3U));
    this->__PVT__cnt_full = ((0xfff7U & (IData)(this->__PVT__cnt_full)) 
                             | (8U & ((((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 3U)));
    this->__PVT__occupied = ((0xfffbU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 2U));
    this->__PVT__cnt_full = ((0xfffbU & (IData)(this->__PVT__cnt_full)) 
                             | (4U & ((((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 2U)));
    this->__PVT__occupied = ((0xfffdU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 1U));
    this->__PVT__cnt_full = ((0xfffdU & (IData)(this->__PVT__cnt_full)) 
                             | (2U & ((((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 1U)));
    this->__PVT__occupied = ((0xfffeU & (IData)(this->__PVT__occupied)) 
                             | (0U != (7U & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))));
    this->__PVT__cnt_full = ((0xfffeU & (IData)(this->__PVT__cnt_full)) 
                             | (1U & (((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                       >> 3U) | (7U 
                                                 == 
                                                 (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))))));
}

VL_INLINE_OPT void Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__7(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__7\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->lookup_mst_select_o = (1U & ((IData)(this->__PVT__mst_select_q) 
                                       >> (0xfU & (
                                                   (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[5U] 
                                                    << 0x11U) 
                                                   | (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[4U] 
                                                      >> 0xfU)))));
    this->lookup_mst_select_occupied_o = (1U & ((IData)(this->__PVT__occupied) 
                                                >> 
                                                (0xfU 
                                                 & ((vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[5U] 
                                                     << 0x11U) 
                                                    | (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[4U] 
                                                       >> 0xfU)))));
}

VL_INLINE_OPT void Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__10(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi22::_combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__10\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__push_en = ((IData)(vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push)
                             ? (0xffffU & ((IData)(1U) 
                                           << (0xfU 
                                               & ((vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[5U] 
                                                   << 0x11U) 
                                                  | (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[4U] 
                                                     >> 0xfU)))))
                             : 0U);
    this->__PVT__inject_en = ((IData)(vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject)
                               ? (0xffffU & ((IData)(1U) 
                                             << (0xfU 
                                                 & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[0xbU] 
                                                    >> 0x13U))))
                               : 0U);
    this->__PVT__pop_en = ((1U & (((vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                    >> 8U) & vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__slv_req_cut[0U]) 
                                  & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
                                     >> 1U))) ? (0xffffU 
                                                 & ((IData)(1U) 
                                                    << 
                                                    (0xfU 
                                                     & (vlTOPp->banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                                        >> 4U))))
                            : 0U);
    this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_en 
        = (1U & ((1U & (IData)(this->__PVT__push_en))
                  ? ((IData)(this->__PVT__inject_en) 
                     | (~ (IData)(this->__PVT__pop_en)))
                  : ((1U & (IData)(this->__PVT__inject_en))
                      ? (~ (IData)(this->__PVT__pop_en))
                      : (IData)(this->__PVT__pop_en))));
    this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_down 
        = (1U & ((~ (IData)(this->__PVT__push_en)) 
                 & ((~ (IData)(this->__PVT__inject_en)) 
                    & (IData)(this->__PVT__pop_en))));
    this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_delta 
        = ((1U & (IData)(this->__PVT__push_en)) ? (
                                                   (1U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 1U
                                                     : 2U)
                                                    : 
                                                   ((1U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 0U
                                                     : 1U))
            : ((1U & (IData)(this->__PVT__inject_en))
                ? ((1U & (IData)(this->__PVT__pop_en))
                    ? 0U : 1U) : ((1U & (IData)(this->__PVT__pop_en))
                                   ? 1U : 0U)));
    this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_en 
        = (1U & ((2U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 1U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 1U))) : ((2U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 1U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 1U))));
    this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 1U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 1U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 1U))));
    this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_delta 
        = ((2U & (IData)(this->__PVT__push_en)) ? (
                                                   (2U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 1U
                                                     : 2U)
                                                    : 
                                                   ((2U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 0U
                                                     : 1U))
            : ((2U & (IData)(this->__PVT__inject_en))
                ? ((2U & (IData)(this->__PVT__pop_en))
                    ? 0U : 1U) : ((2U & (IData)(this->__PVT__pop_en))
                                   ? 1U : 0U)));
    this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_en 
        = (1U & ((4U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 2U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 2U))) : ((4U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 2U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 2U))));
    this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 2U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 2U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 2U))));
    this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_delta 
        = ((4U & (IData)(this->__PVT__push_en)) ? (
                                                   (4U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 1U
                                                     : 2U)
                                                    : 
                                                   ((4U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 0U
                                                     : 1U))
            : ((4U & (IData)(this->__PVT__inject_en))
                ? ((4U & (IData)(this->__PVT__pop_en))
                    ? 0U : 1U) : ((4U & (IData)(this->__PVT__pop_en))
                                   ? 1U : 0U)));
    this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_en 
        = (1U & ((8U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 3U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 3U))) : ((8U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 3U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 3U))));
    this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 3U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 3U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 3U))));
    this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_delta 
        = ((8U & (IData)(this->__PVT__push_en)) ? (
                                                   (8U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   ((8U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 1U
                                                     : 2U)
                                                    : 
                                                   ((8U 
                                                     & (IData)(this->__PVT__pop_en))
                                                     ? 0U
                                                     : 1U))
            : ((8U & (IData)(this->__PVT__inject_en))
                ? ((8U & (IData)(this->__PVT__pop_en))
                    ? 0U : 1U) : ((8U & (IData)(this->__PVT__pop_en))
                                   ? 1U : 0U)));
    this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_en 
        = (1U & ((0x10U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 4U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 4U))) : ((0x10U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 4U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 4U))));
    this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 4U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 4U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 4U))));
    this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_delta 
        = ((0x10U & (IData)(this->__PVT__push_en)) ? 
           ((0x10U & (IData)(this->__PVT__inject_en))
             ? ((0x10U & (IData)(this->__PVT__pop_en))
                 ? 1U : 2U) : ((0x10U & (IData)(this->__PVT__pop_en))
                                ? 0U : 1U)) : ((0x10U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (
                                                   (0x10U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                : (
                                                   (0x10U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_en 
        = (1U & ((0x20U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 5U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 5U))) : ((0x20U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 5U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 5U))));
    this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 5U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 5U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 5U))));
    this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_delta 
        = ((0x20U & (IData)(this->__PVT__push_en)) ? 
           ((0x20U & (IData)(this->__PVT__inject_en))
             ? ((0x20U & (IData)(this->__PVT__pop_en))
                 ? 1U : 2U) : ((0x20U & (IData)(this->__PVT__pop_en))
                                ? 0U : 1U)) : ((0x20U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (
                                                   (0x20U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                : (
                                                   (0x20U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_en 
        = (1U & ((0x40U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 6U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 6U))) : ((0x40U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 6U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 6U))));
    this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 6U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 6U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 6U))));
    this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_delta 
        = ((0x40U & (IData)(this->__PVT__push_en)) ? 
           ((0x40U & (IData)(this->__PVT__inject_en))
             ? ((0x40U & (IData)(this->__PVT__pop_en))
                 ? 1U : 2U) : ((0x40U & (IData)(this->__PVT__pop_en))
                                ? 0U : 1U)) : ((0x40U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (
                                                   (0x40U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                : (
                                                   (0x40U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_en 
        = (1U & ((0x80U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 7U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 7U))) : ((0x80U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 7U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 7U))));
    this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 7U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 7U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 7U))));
    this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_delta 
        = ((0x80U & (IData)(this->__PVT__push_en)) ? 
           ((0x80U & (IData)(this->__PVT__inject_en))
             ? ((0x80U & (IData)(this->__PVT__pop_en))
                 ? 1U : 2U) : ((0x80U & (IData)(this->__PVT__pop_en))
                                ? 0U : 1U)) : ((0x80U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (
                                                   (0x80U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                : (
                                                   (0x80U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_en 
        = (1U & ((0x100U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 8U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 8U))) : ((0x100U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 8U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 8U))));
    this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 8U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 8U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 8U))));
    this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_delta 
        = ((0x100U & (IData)(this->__PVT__push_en))
            ? ((0x100U & (IData)(this->__PVT__inject_en))
                ? ((0x100U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x100U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x100U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x100U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x100U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_en 
        = (1U & ((0x200U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 9U) | (~ ((IData)(this->__PVT__pop_en) 
                                   >> 9U))) : ((0x200U 
                                                & (IData)(this->__PVT__inject_en))
                                                ? (~ 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 9U))
                                                : ((IData)(this->__PVT__pop_en) 
                                                   >> 9U))));
    this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 9U)) & ((~ ((IData)(this->__PVT__inject_en) 
                                    >> 9U)) & ((IData)(this->__PVT__pop_en) 
                                               >> 9U))));
    this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_delta 
        = ((0x200U & (IData)(this->__PVT__push_en))
            ? ((0x200U & (IData)(this->__PVT__inject_en))
                ? ((0x200U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x200U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x200U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x200U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x200U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_en 
        = (1U & ((0x400U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 0xaU) | (~ ((IData)(this->__PVT__pop_en) 
                                     >> 0xaU))) : (
                                                   (0x400U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   (~ 
                                                    ((IData)(this->__PVT__pop_en) 
                                                     >> 0xaU))
                                                    : 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 0xaU))));
    this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 0xaU)) & ((~ ((IData)(this->__PVT__inject_en) 
                                      >> 0xaU)) & ((IData)(this->__PVT__pop_en) 
                                                   >> 0xaU))));
    this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_delta 
        = ((0x400U & (IData)(this->__PVT__push_en))
            ? ((0x400U & (IData)(this->__PVT__inject_en))
                ? ((0x400U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x400U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x400U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x400U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x400U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_en 
        = (1U & ((0x800U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 0xbU) | (~ ((IData)(this->__PVT__pop_en) 
                                     >> 0xbU))) : (
                                                   (0x800U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   (~ 
                                                    ((IData)(this->__PVT__pop_en) 
                                                     >> 0xbU))
                                                    : 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 0xbU))));
    this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 0xbU)) & ((~ ((IData)(this->__PVT__inject_en) 
                                      >> 0xbU)) & ((IData)(this->__PVT__pop_en) 
                                                   >> 0xbU))));
    this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_delta 
        = ((0x800U & (IData)(this->__PVT__push_en))
            ? ((0x800U & (IData)(this->__PVT__inject_en))
                ? ((0x800U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x800U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x800U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x800U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x800U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_en 
        = (1U & ((0x1000U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 0xcU) | (~ ((IData)(this->__PVT__pop_en) 
                                     >> 0xcU))) : (
                                                   (0x1000U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   (~ 
                                                    ((IData)(this->__PVT__pop_en) 
                                                     >> 0xcU))
                                                    : 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 0xcU))));
    this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 0xcU)) & ((~ ((IData)(this->__PVT__inject_en) 
                                      >> 0xcU)) & ((IData)(this->__PVT__pop_en) 
                                                   >> 0xcU))));
    this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_delta 
        = ((0x1000U & (IData)(this->__PVT__push_en))
            ? ((0x1000U & (IData)(this->__PVT__inject_en))
                ? ((0x1000U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x1000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x1000U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x1000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x1000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_en 
        = (1U & ((0x2000U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 0xdU) | (~ ((IData)(this->__PVT__pop_en) 
                                     >> 0xdU))) : (
                                                   (0x2000U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   (~ 
                                                    ((IData)(this->__PVT__pop_en) 
                                                     >> 0xdU))
                                                    : 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 0xdU))));
    this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 0xdU)) & ((~ ((IData)(this->__PVT__inject_en) 
                                      >> 0xdU)) & ((IData)(this->__PVT__pop_en) 
                                                   >> 0xdU))));
    this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_delta 
        = ((0x2000U & (IData)(this->__PVT__push_en))
            ? ((0x2000U & (IData)(this->__PVT__inject_en))
                ? ((0x2000U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x2000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x2000U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x2000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x2000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_en 
        = (1U & ((0x4000U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 0xeU) | (~ ((IData)(this->__PVT__pop_en) 
                                     >> 0xeU))) : (
                                                   (0x4000U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   (~ 
                                                    ((IData)(this->__PVT__pop_en) 
                                                     >> 0xeU))
                                                    : 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 0xeU))));
    this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 0xeU)) & ((~ ((IData)(this->__PVT__inject_en) 
                                      >> 0xeU)) & ((IData)(this->__PVT__pop_en) 
                                                   >> 0xeU))));
    this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_delta 
        = ((0x4000U & (IData)(this->__PVT__push_en))
            ? ((0x4000U & (IData)(this->__PVT__inject_en))
                ? ((0x4000U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x4000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x4000U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x4000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x4000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_en 
        = (1U & ((0x8000U & (IData)(this->__PVT__push_en))
                  ? (((IData)(this->__PVT__inject_en) 
                      >> 0xfU) | (~ ((IData)(this->__PVT__pop_en) 
                                     >> 0xfU))) : (
                                                   (0x8000U 
                                                    & (IData)(this->__PVT__inject_en))
                                                    ? 
                                                   (~ 
                                                    ((IData)(this->__PVT__pop_en) 
                                                     >> 0xfU))
                                                    : 
                                                   ((IData)(this->__PVT__pop_en) 
                                                    >> 0xfU))));
    this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_down 
        = (1U & ((~ ((IData)(this->__PVT__push_en) 
                     >> 0xfU)) & ((~ ((IData)(this->__PVT__inject_en) 
                                      >> 0xfU)) & ((IData)(this->__PVT__pop_en) 
                                                   >> 0xfU))));
    this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_delta 
        = ((0x8000U & (IData)(this->__PVT__push_en))
            ? ((0x8000U & (IData)(this->__PVT__inject_en))
                ? ((0x8000U & (IData)(this->__PVT__pop_en))
                    ? 1U : 2U) : ((0x8000U & (IData)(this->__PVT__pop_en))
                                   ? 0U : 1U)) : ((0x8000U 
                                                   & (IData)(this->__PVT__inject_en))
                                                   ? 
                                                  ((0x8000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 0U
                                                    : 1U)
                                                   : 
                                                  ((0x8000U 
                                                    & (IData)(this->__PVT__pop_en))
                                                    ? 1U
                                                    : 0U)));
    this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__cnt_delta))));
    }
    this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d 
        = this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    if (this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_en) {
        this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_down)
                        ? ((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           - (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_delta))
                        : ((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                           + (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__cnt_delta))));
    }
}
