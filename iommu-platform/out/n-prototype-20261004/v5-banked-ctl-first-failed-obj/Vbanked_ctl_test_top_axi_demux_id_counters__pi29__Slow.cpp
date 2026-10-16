// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vbanked_ctl_test_top.h for the primary calling header

#include "Vbanked_ctl_test_top_axi_demux_id_counters__pi29.h"
#include "Vbanked_ctl_test_top__Syms.h"

//==========

VL_CTOR_IMP(Vbanked_ctl_test_top_axi_demux_id_counters__pi29) {
    // Reset internal values
    // Reset structure values
    _ctor_var_reset();
}

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::__Vconfigure(Vbanked_ctl_test_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
}

Vbanked_ctl_test_top_axi_demux_id_counters__pi29::~Vbanked_ctl_test_top_axi_demux_id_counters__pi29() {
}

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__1(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__1\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__cnt_full = ((0xfffeU & (IData)(this->__PVT__cnt_full)) 
                             | (1U & (((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                       >> 3U) | (7U 
                                                 == 
                                                 (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))))));
    this->__PVT__cnt_full = ((0xfffdU & (IData)(this->__PVT__cnt_full)) 
                             | (2U & ((((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 1U)));
    this->__PVT__cnt_full = ((0xfffbU & (IData)(this->__PVT__cnt_full)) 
                             | (4U & ((((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 2U)));
    this->__PVT__cnt_full = ((0xfff7U & (IData)(this->__PVT__cnt_full)) 
                             | (8U & ((((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 3U)));
    this->__PVT__cnt_full = ((0xffefU & (IData)(this->__PVT__cnt_full)) 
                             | (0x10U & ((((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 4U)));
    this->__PVT__cnt_full = ((0xffdfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x20U & ((((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 5U)));
    this->__PVT__cnt_full = ((0xffbfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x40U & ((((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 6U)));
    this->__PVT__cnt_full = ((0xff7fU & (IData)(this->__PVT__cnt_full)) 
                             | (0x80U & ((((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 7U)));
    this->__PVT__cnt_full = ((0xfeffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x100U & ((((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 8U)));
    this->__PVT__cnt_full = ((0xfdffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x200U & ((((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 9U)));
    this->__PVT__cnt_full = ((0xfbffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x400U & ((((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xaU)));
    this->__PVT__cnt_full = ((0xf7ffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x800U & ((((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xbU)));
    this->__PVT__cnt_full = ((0xefffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x1000U & ((((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xcU)));
    this->__PVT__cnt_full = ((0xdfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x2000U & ((((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xdU)));
    this->__PVT__cnt_full = ((0xbfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x4000U & ((((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xeU)));
    this->__PVT__cnt_full = ((0x7fffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x8000U & ((((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xfU)));
    this->__PVT__occupied = ((0xfffeU & (IData)(this->__PVT__occupied)) 
                             | (0U != (7U & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))));
    this->__PVT__occupied = ((0xfffdU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 1U));
    this->__PVT__occupied = ((0xfffbU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 2U));
    this->__PVT__occupied = ((0xfff7U & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 3U));
    this->__PVT__occupied = ((0xffefU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 4U));
    this->__PVT__occupied = ((0xffdfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 5U));
    this->__PVT__occupied = ((0xffbfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 6U));
    this->__PVT__occupied = ((0xff7fU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 7U));
    this->__PVT__occupied = ((0xfeffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 8U));
    this->__PVT__occupied = ((0xfdffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 9U));
    this->__PVT__occupied = ((0xfbffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xaU));
    this->__PVT__occupied = ((0xf7ffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xbU));
    this->__PVT__occupied = ((0xefffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xcU));
    this->__PVT__occupied = ((0xdfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xdU));
    this->__PVT__occupied = ((0xbfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xeU));
    this->__PVT__occupied = ((0x7fffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xfU));
    this->__PVT__lookup_mst_select_o = (1U & ((IData)(this->__PVT__mst_select_q) 
                                              >> (0xfU 
                                                  & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                     >> 0x15U))));
    this->__PVT__lookup_mst_select_occupied_o = (1U 
                                                 & ((IData)(this->__PVT__occupied) 
                                                    >> 
                                                    (0xfU 
                                                     & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                        >> 0x15U))));
}

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__5(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__5\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__push_en = ((IData)(vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up)
                             ? (0xffffU & ((IData)(1U) 
                                           << (0xfU 
                                               & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                  >> 0x15U))))
                             : 0U);
    this->__PVT__pop_en = ((1U & ((vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                   >> 0x10U) & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                                                >> 4U)))
                            ? (0xffffU & ((IData)(1U) 
                                          << (0xfU 
                                              & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
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

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__3(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__3\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__occupied = ((0xfffeU & (IData)(this->__PVT__occupied)) 
                             | (0U != (7U & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))));
    this->__PVT__occupied = ((0xfffdU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 1U));
    this->__PVT__occupied = ((0xfffbU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 2U));
    this->__PVT__occupied = ((0xfff7U & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 3U));
    this->__PVT__occupied = ((0xffefU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 4U));
    this->__PVT__occupied = ((0xffdfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 5U));
    this->__PVT__occupied = ((0xffbfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 6U));
    this->__PVT__occupied = ((0xff7fU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 7U));
    this->__PVT__occupied = ((0xfeffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 8U));
    this->__PVT__occupied = ((0xfdffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 9U));
    this->__PVT__occupied = ((0xfbffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xaU));
    this->__PVT__occupied = ((0xf7ffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xbU));
    this->__PVT__occupied = ((0xefffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xcU));
    this->__PVT__occupied = ((0xdfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xdU));
    this->__PVT__occupied = ((0xbfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xeU));
    this->__PVT__occupied = ((0x7fffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xfU));
    this->__PVT__cnt_full = ((0xfffeU & (IData)(this->__PVT__cnt_full)) 
                             | (1U & (((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                       >> 3U) | (7U 
                                                 == 
                                                 (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))))));
    this->__PVT__cnt_full = ((0xfffdU & (IData)(this->__PVT__cnt_full)) 
                             | (2U & ((((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 1U)));
    this->__PVT__cnt_full = ((0xfffbU & (IData)(this->__PVT__cnt_full)) 
                             | (4U & ((((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 2U)));
    this->__PVT__cnt_full = ((0xfff7U & (IData)(this->__PVT__cnt_full)) 
                             | (8U & ((((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 3U)));
    this->__PVT__cnt_full = ((0xffefU & (IData)(this->__PVT__cnt_full)) 
                             | (0x10U & ((((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 4U)));
    this->__PVT__cnt_full = ((0xffdfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x20U & ((((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 5U)));
    this->__PVT__cnt_full = ((0xffbfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x40U & ((((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 6U)));
    this->__PVT__cnt_full = ((0xff7fU & (IData)(this->__PVT__cnt_full)) 
                             | (0x80U & ((((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 7U)));
    this->__PVT__cnt_full = ((0xfeffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x100U & ((((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 8U)));
    this->__PVT__cnt_full = ((0xfdffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x200U & ((((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 9U)));
    this->__PVT__cnt_full = ((0xfbffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x400U & ((((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xaU)));
    this->__PVT__cnt_full = ((0xf7ffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x800U & ((((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xbU)));
    this->__PVT__cnt_full = ((0xefffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x1000U & ((((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xcU)));
    this->__PVT__cnt_full = ((0xdfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x2000U & ((((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xdU)));
    this->__PVT__cnt_full = ((0xbfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x4000U & ((((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xeU)));
    this->__PVT__cnt_full = ((0x7fffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x8000U & ((((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xfU)));
    this->__PVT__lookup_mst_select_o = (1U & ((IData)(this->__PVT__mst_select_q) 
                                              >> (0xfU 
                                                  & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U])));
    this->__PVT__lookup_mst_select_occupied_o = (1U 
                                                 & ((IData)(this->__PVT__occupied) 
                                                    >> 
                                                    (0xfU 
                                                     & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U])));
}

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__6(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__6\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__inject_en = ((IData)(vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject)
                               ? (0xffffU & ((IData)(1U) 
                                             << (0xfU 
                                                 & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                    >> 0x15U))))
                               : 0U);
    this->__PVT__push_en = ((IData)(vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push)
                             ? (0xffffU & ((IData)(1U) 
                                           << (0xfU 
                                               & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U])))
                             : 0U);
    this->__PVT__pop_en = ((1U & (((vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                    >> 8U) & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U]) 
                                  & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
                                     >> 1U))) ? (0xffffU 
                                                 & ((IData)(1U) 
                                                    << 
                                                    (0xfU 
                                                     & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__0__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
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

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__2(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__2\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__cnt_full = ((0xfffeU & (IData)(this->__PVT__cnt_full)) 
                             | (1U & (((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                       >> 3U) | (7U 
                                                 == 
                                                 (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))))));
    this->__PVT__cnt_full = ((0xfffdU & (IData)(this->__PVT__cnt_full)) 
                             | (2U & ((((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 1U)));
    this->__PVT__cnt_full = ((0xfffbU & (IData)(this->__PVT__cnt_full)) 
                             | (4U & ((((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 2U)));
    this->__PVT__cnt_full = ((0xfff7U & (IData)(this->__PVT__cnt_full)) 
                             | (8U & ((((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 3U)));
    this->__PVT__cnt_full = ((0xffefU & (IData)(this->__PVT__cnt_full)) 
                             | (0x10U & ((((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 4U)));
    this->__PVT__cnt_full = ((0xffdfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x20U & ((((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 5U)));
    this->__PVT__cnt_full = ((0xffbfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x40U & ((((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 6U)));
    this->__PVT__cnt_full = ((0xff7fU & (IData)(this->__PVT__cnt_full)) 
                             | (0x80U & ((((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 7U)));
    this->__PVT__cnt_full = ((0xfeffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x100U & ((((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 8U)));
    this->__PVT__cnt_full = ((0xfdffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x200U & ((((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 9U)));
    this->__PVT__cnt_full = ((0xfbffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x400U & ((((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xaU)));
    this->__PVT__cnt_full = ((0xf7ffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x800U & ((((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xbU)));
    this->__PVT__cnt_full = ((0xefffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x1000U & ((((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xcU)));
    this->__PVT__cnt_full = ((0xdfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x2000U & ((((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xdU)));
    this->__PVT__cnt_full = ((0xbfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x4000U & ((((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xeU)));
    this->__PVT__cnt_full = ((0x7fffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x8000U & ((((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xfU)));
    this->__PVT__occupied = ((0xfffeU & (IData)(this->__PVT__occupied)) 
                             | (0U != (7U & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))));
    this->__PVT__occupied = ((0xfffdU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 1U));
    this->__PVT__occupied = ((0xfffbU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 2U));
    this->__PVT__occupied = ((0xfff7U & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 3U));
    this->__PVT__occupied = ((0xffefU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 4U));
    this->__PVT__occupied = ((0xffdfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 5U));
    this->__PVT__occupied = ((0xffbfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 6U));
    this->__PVT__occupied = ((0xff7fU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 7U));
    this->__PVT__occupied = ((0xfeffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 8U));
    this->__PVT__occupied = ((0xfdffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 9U));
    this->__PVT__occupied = ((0xfbffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xaU));
    this->__PVT__occupied = ((0xf7ffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xbU));
    this->__PVT__occupied = ((0xefffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xcU));
    this->__PVT__occupied = ((0xdfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xdU));
    this->__PVT__occupied = ((0xbfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xeU));
    this->__PVT__occupied = ((0x7fffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xfU));
    this->__PVT__lookup_mst_select_o = (1U & ((IData)(this->__PVT__mst_select_q) 
                                              >> (0xfU 
                                                  & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                     >> 0x15U))));
    this->__PVT__lookup_mst_select_occupied_o = (1U 
                                                 & ((IData)(this->__PVT__occupied) 
                                                    >> 
                                                    (0xfU 
                                                     & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                        >> 0x15U))));
}

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__7(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__7\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__push_en = ((IData)(vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up)
                             ? (0xffffU & ((IData)(1U) 
                                           << (0xfU 
                                               & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                  >> 0x15U))))
                             : 0U);
    this->__PVT__pop_en = ((1U & ((vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                   >> 0x10U) & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                                                >> 4U)))
                            ? (0xffffU & ((IData)(1U) 
                                          << (0xfU 
                                              & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
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

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__4(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__4\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__occupied = ((0xfffeU & (IData)(this->__PVT__occupied)) 
                             | (0U != (7U & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))));
    this->__PVT__occupied = ((0xfffdU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 1U));
    this->__PVT__occupied = ((0xfffbU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 2U));
    this->__PVT__occupied = ((0xfff7U & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 3U));
    this->__PVT__occupied = ((0xffefU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 4U));
    this->__PVT__occupied = ((0xffdfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 5U));
    this->__PVT__occupied = ((0xffbfU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 6U));
    this->__PVT__occupied = ((0xff7fU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 7U));
    this->__PVT__occupied = ((0xfeffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 8U));
    this->__PVT__occupied = ((0xfdffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 9U));
    this->__PVT__occupied = ((0xfbffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xaU));
    this->__PVT__occupied = ((0xf7ffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xbU));
    this->__PVT__occupied = ((0xefffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xcU));
    this->__PVT__occupied = ((0xdfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xdU));
    this->__PVT__occupied = ((0xbfffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xeU));
    this->__PVT__occupied = ((0x7fffU & (IData)(this->__PVT__occupied)) 
                             | ((IData)((0U != (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                << 0xfU));
    this->__PVT__cnt_full = ((0xfffeU & (IData)(this->__PVT__cnt_full)) 
                             | (1U & (((IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                       >> 3U) | (7U 
                                                 == 
                                                 (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q))))));
    this->__PVT__cnt_full = ((0xfffdU & (IData)(this->__PVT__cnt_full)) 
                             | (2U & ((((IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 1U)));
    this->__PVT__cnt_full = ((0xfffbU & (IData)(this->__PVT__cnt_full)) 
                             | (4U & ((((IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 2U)));
    this->__PVT__cnt_full = ((0xfff7U & (IData)(this->__PVT__cnt_full)) 
                             | (8U & ((((IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                        >> 3U) | (7U 
                                                  == 
                                                  (7U 
                                                   & (IData)(this->__PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                      << 3U)));
    this->__PVT__cnt_full = ((0xffefU & (IData)(this->__PVT__cnt_full)) 
                             | (0x10U & ((((IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 4U)));
    this->__PVT__cnt_full = ((0xffdfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x20U & ((((IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 5U)));
    this->__PVT__cnt_full = ((0xffbfU & (IData)(this->__PVT__cnt_full)) 
                             | (0x40U & ((((IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 6U)));
    this->__PVT__cnt_full = ((0xff7fU & (IData)(this->__PVT__cnt_full)) 
                             | (0x80U & ((((IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                           >> 3U) | 
                                          (7U == (7U 
                                                  & (IData)(this->__PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                         << 7U)));
    this->__PVT__cnt_full = ((0xfeffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x100U & ((((IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 8U)));
    this->__PVT__cnt_full = ((0xfdffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x200U & ((((IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 9U)));
    this->__PVT__cnt_full = ((0xfbffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x400U & ((((IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xaU)));
    this->__PVT__cnt_full = ((0xf7ffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x800U & ((((IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                            >> 3U) 
                                           | (7U == 
                                              (7U & (IData)(this->__PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                          << 0xbU)));
    this->__PVT__cnt_full = ((0xefffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x1000U & ((((IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xcU)));
    this->__PVT__cnt_full = ((0xdfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x2000U & ((((IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xdU)));
    this->__PVT__cnt_full = ((0xbfffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x4000U & ((((IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xeU)));
    this->__PVT__cnt_full = ((0x7fffU & (IData)(this->__PVT__cnt_full)) 
                             | (0x8000U & ((((IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q) 
                                             >> 3U) 
                                            | (7U == 
                                               (7U 
                                                & (IData)(this->__PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q)))) 
                                           << 0xfU)));
    this->__PVT__lookup_mst_select_o = (1U & ((IData)(this->__PVT__mst_select_q) 
                                              >> (0xfU 
                                                  & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U])));
    this->__PVT__lookup_mst_select_occupied_o = (1U 
                                                 & ((IData)(this->__PVT__occupied) 
                                                    >> 
                                                    (0xfU 
                                                     & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U])));
}

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__8(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__8\n"); );
    Vbanked_ctl_test_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__inject_en = ((IData)(vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject)
                               ? (0xffffU & ((IData)(1U) 
                                             << (0xfU 
                                                 & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                                                    >> 0x15U))))
                               : 0U);
    this->__PVT__push_en = ((IData)(vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push)
                             ? (0xffffU & ((IData)(1U) 
                                           << (0xfU 
                                               & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U])))
                             : 0U);
    this->__PVT__pop_en = ((1U & (((vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                                    >> 8U) & vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U]) 
                                  & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
                                     >> 1U))) ? (0xffffU 
                                                 & ((IData)(1U) 
                                                    << 
                                                    (0xfU 
                                                     & (vlSymsp->TOP__banked_ctl_test_top__DOT__i_dut__DOT__gen_bank__BRA__1__KET____DOT__i_bank.gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
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

void Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+                Vbanked_ctl_test_top_axi_demux_id_counters__pi29::_ctor_var_reset\n"); );
    // Body
    __PVT__clk_i = VL_RAND_RESET_I(1);
    __PVT__rst_ni = VL_RAND_RESET_I(1);
    __PVT__lookup_axi_id_i = VL_RAND_RESET_I(4);
    __PVT__lookup_mst_select_o = VL_RAND_RESET_I(1);
    __PVT__lookup_mst_select_occupied_o = VL_RAND_RESET_I(1);
    __PVT__full_o = VL_RAND_RESET_I(1);
    __PVT__push_axi_id_i = VL_RAND_RESET_I(4);
    __PVT__push_mst_select_i = VL_RAND_RESET_I(1);
    __PVT__push_i = VL_RAND_RESET_I(1);
    __PVT__inject_axi_id_i = VL_RAND_RESET_I(4);
    __PVT__inject_i = VL_RAND_RESET_I(1);
    __PVT__pop_axi_id_i = VL_RAND_RESET_I(4);
    __PVT__pop_i = VL_RAND_RESET_I(1);
    __PVT__mst_select_q = VL_RAND_RESET_I(16);
    __PVT__push_en = VL_RAND_RESET_I(16);
    __PVT__inject_en = VL_RAND_RESET_I(16);
    __PVT__pop_en = VL_RAND_RESET_I(16);
    __PVT__occupied = VL_RAND_RESET_I(16);
    __PVT__cnt_full = VL_RAND_RESET_I(16);
    __PVT__gen_counters__BRA__0__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__0__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__0__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__1__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__1__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__1__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__2__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__2__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__2__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__3__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__3__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__3__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__4__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__4__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__4__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__5__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__5__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__5__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__6__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__6__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__6__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__7__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__7__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__7__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__8__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__8__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__8__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__9__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__9__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__9__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__10__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__10__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__10__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__11__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__11__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__11__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__12__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__12__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__12__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__13__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__13__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__13__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__14__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__14__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__14__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__15__KET____DOT__cnt_en = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__15__KET____DOT__cnt_down = VL_RAND_RESET_I(1);
    __PVT__gen_counters__BRA__15__KET____DOT__cnt_delta = VL_RAND_RESET_I(3);
    __PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q = VL_RAND_RESET_I(4);
    __PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d = VL_RAND_RESET_I(4);
}
