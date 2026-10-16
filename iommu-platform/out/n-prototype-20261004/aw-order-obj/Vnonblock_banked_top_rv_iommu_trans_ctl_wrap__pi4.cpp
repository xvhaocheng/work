// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vnonblock_banked_top.h for the primary calling header

#include "Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4.h"
#include "Vnonblock_banked_top__Syms.h"

//==========

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_combo__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__1(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_combo__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__1\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_inp 
        = ((0xf00U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                      << 8U)) | (0xffU & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] 
                                           << 8U) | 
                                          (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
                                           >> 0x18U))));
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_settle__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__7(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_settle__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__7\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_aw_stream_fifo__DOT__push = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] 
                                                 >> 0x1fU) 
                                                & (2U 
                                                   != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_ar_stream_fifo__DOT__push = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
                                                 >> 1U) 
                                                & (2U 
                                                   != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_w_stream_fifo__DOT__push = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
                                                >> 0x14U) 
                                               & (4U 
                                                  != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__axi_comp_req[8U] = ((0x1fffffU & this->__PVT__axi_comp_req[8U]) 
                                     | (0x1e00000U 
                                        & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
                                           << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0x1fffffU & this->__PVT__axi_comp_req[6U]) 
                                     | (0xffe00000U 
                                        & ((IData)(
                                                   (0xffffffffffffffULL 
                                                    & (((QData)((IData)(
                                                                        this->__PVT__trans_resp_data_q[5U])) 
                                                        << 0x22U) 
                                                       | (((QData)((IData)(
                                                                           this->__PVT__trans_resp_data_q[4U])) 
                                                           << 2U) 
                                                          | ((QData)((IData)(
                                                                             this->__PVT__trans_resp_data_q[3U])) 
                                                             >> 0x1eU))))) 
                                           << 0x15U)));
    this->__PVT__axi_comp_req[7U] = ((0x1fffffU & ((IData)(
                                                           (0xffffffffffffffULL 
                                                            & (((QData)((IData)(
                                                                                this->__PVT__trans_resp_data_q[5U])) 
                                                                << 0x22U) 
                                                               | (((QData)((IData)(
                                                                                this->__PVT__trans_resp_data_q[4U])) 
                                                                   << 2U) 
                                                                  | ((QData)((IData)(
                                                                                this->__PVT__trans_resp_data_q[3U])) 
                                                                     >> 0x1eU))))) 
                                                   >> 0xbU)) 
                                     | (0xffe00000U 
                                        & ((IData)(
                                                   ((0xffffffffffffffULL 
                                                     & (((QData)((IData)(
                                                                         this->__PVT__trans_resp_data_q[5U])) 
                                                         << 0x22U) 
                                                        | (((QData)((IData)(
                                                                            this->__PVT__trans_resp_data_q[4U])) 
                                                            << 2U) 
                                                           | ((QData)((IData)(
                                                                              this->__PVT__trans_resp_data_q[3U])) 
                                                              >> 0x1eU)))) 
                                                    >> 0x20U)) 
                                           << 0x15U)));
    this->__PVT__axi_comp_req[8U] = ((0x1e00000U & 
                                      this->__PVT__axi_comp_req[8U]) 
                                     | (0x1fffffU & 
                                        ((IData)(((0xffffffffffffffULL 
                                                   & (((QData)((IData)(
                                                                       this->__PVT__trans_resp_data_q[5U])) 
                                                       << 0x22U) 
                                                      | (((QData)((IData)(
                                                                          this->__PVT__trans_resp_data_q[4U])) 
                                                          << 2U) 
                                                         | ((QData)((IData)(
                                                                            this->__PVT__trans_resp_data_q[3U])) 
                                                            >> 0x1eU)))) 
                                                  >> 0x20U)) 
                                         >> 0xbU)));
    this->__PVT__axi_comp_req[6U] = ((0xffe01fffU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x1fe000U & 
                                        (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                         << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xffffe3ffU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x1c00U & (
                                                   this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                   << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xfffffcffU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x300U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                  << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xffffff7fU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x80U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                 << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xffffff87U & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x78U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                 << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xfffffff8U & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (7U & ((this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                               << 2U) 
                                              | (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                 >> 0x1eU))));
    this->__PVT__axi_comp_req[5U] = ((0xfffffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xf0000000U 
                                        & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                           << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xf0ffffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xf000000U 
                                        & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                           << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xff03ffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xfc0000U & 
                                        (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                         << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xfffdffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0x20000U & 
                                        (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                         << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xfffeffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xffff0000U 
                                        & ((IData)(this->__PVT__complete_write) 
                                           << 0x10U)));
    this->__PVT__axi_comp_req[3U] = ((0xfffffff0U & 
                                      this->__PVT__axi_comp_req[3U]) 
                                     | (0xfU & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
                                                >> 0xdU)));
    this->__PVT__axi_comp_req[1U] = (IData)((0xffffffffffffffULL 
                                             & (((QData)((IData)(
                                                                 this->__PVT__trans_resp_data_q[5U])) 
                                                 << 0x22U) 
                                                | (((QData)((IData)(
                                                                    this->__PVT__trans_resp_data_q[4U])) 
                                                    << 2U) 
                                                   | ((QData)((IData)(
                                                                      this->__PVT__trans_resp_data_q[3U])) 
                                                      >> 0x1eU)))));
    this->__PVT__axi_comp_req[2U] = (IData)(((0xffffffffffffffULL 
                                              & (((QData)((IData)(
                                                                  this->__PVT__trans_resp_data_q[5U])) 
                                                  << 0x22U) 
                                                 | (((QData)((IData)(
                                                                     this->__PVT__trans_resp_data_q[4U])) 
                                                     << 2U) 
                                                    | ((QData)((IData)(
                                                                       this->__PVT__trans_resp_data_q[3U])) 
                                                       >> 0x1eU)))) 
                                             >> 0x20U));
    this->__PVT__axi_comp_req[0U] = ((0xffffffU & this->__PVT__axi_comp_req[0U]) 
                                     | (0xff000000U 
                                        & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                           << 0x13U)));
    this->__PVT__axi_comp_req[0U] = ((0xff1fffffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0xe00000U & 
                                        (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                         << 0x13U)));
    this->__PVT__axi_comp_req[0U] = ((0xffe7ffffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x180000U & 
                                        (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                         << 0x13U)));
    this->__PVT__axi_comp_req[0U] = ((0xfffbffffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x40000U & 
                                        (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                         >> 0xdU)));
    this->__PVT__axi_comp_req[0U] = ((0xfffc3fffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x3c000U & 
                                        ((this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                          << 0x13U) 
                                         | (0x7c000U 
                                            & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                               >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xffffc7ffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x3800U & (
                                                   (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                    << 0x13U) 
                                                   | (0x7f800U 
                                                      & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                         >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xfffff87fU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x780U & ((
                                                   this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                   << 0x13U) 
                                                  | (0x7ff80U 
                                                     & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                        >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xffffff87U & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x78U & ((this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                  << 0x13U) 
                                                 | (0x7fff8U 
                                                    & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                       >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xfffffffbU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (4U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                              >> 0xdU)));
    this->__PVT__axi_comp_req[0U] = ((0xfffffffdU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0xfffffffeU 
                                        & ((IData)(this->__PVT__complete_read) 
                                           << 1U)));
    this->__PVT__axi_comp_req[3U] = ((0x3fU & this->__PVT__axi_comp_req[3U]) 
                                     | (0xffffffc0U 
                                        & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                           << 6U)));
    this->__PVT__axi_comp_req[4U] = ((0x3fU & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                               >> 0x1aU)) 
                                     | (0xffffffc0U 
                                        & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                           << 6U)));
    this->__PVT__axi_comp_req[5U] = ((0xffff0000U & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | ((0x3fU & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                  >> 0x1aU)) 
                                        | (0xffffffc0U 
                                           & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                              << 6U))));
    this->__PVT__axi_comp_req[3U] = ((0xffffffdfU & 
                                      this->__PVT__axi_comp_req[3U]) 
                                     | (0xffffffe0U 
                                        & (((0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                            & (0U != 
                                               (3U 
                                                & (IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q)))) 
                                           << 5U)));
    this->__PVT__axi_comp_req[0U] = ((0xfffffffeU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (1U & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]));
    this->__PVT__axi_comp_req[3U] = ((0xffffffefU & 
                                      this->__PVT__axi_comp_req[3U]) 
                                     | (0x10U & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
                                                 >> 0xfU)));
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__gate_clock = 0U;
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q)));
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
           & (IData)(this->__PVT__i_aw_stream_fifo__DOT__pop)) 
          & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[0U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[0U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[1U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[1U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[2U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[2U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[3U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[3U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[4U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[4U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[5U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[5U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[6U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[6U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[7U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[7U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[8U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[8U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[9U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[9U];
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[0U] 
            = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[7U];
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[1U] 
            = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U];
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[2U] 
            = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U];
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[3U] 
            = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xaU];
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[4U] 
            = (0x7fffffU & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]);
        if ((0x12dU >= (0x1ffU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))))) {
            VL_ASSIGNSEL_WIIW(151,(0x1ffU & ((IData)(0x97U) 
                                             * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))), this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n, this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1);
        }
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__gate_clock = 0U;
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q)));
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
           & (IData)(this->__PVT__i_ar_stream_fifo__DOT__pop)) 
          & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[0U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[0U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[1U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[1U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[2U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[2U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[3U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[3U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[4U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[4U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[5U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[5U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[6U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[6U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[7U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[7U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[8U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[8U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[9U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[9U];
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[0U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] 
                << 0x1eU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
                             >> 2U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[1U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[2U] 
                << 0x1eU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] 
                             >> 2U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[2U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                << 0x1eU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[2U] 
                             >> 2U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[3U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
                << 0x1eU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                             >> 2U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[4U] 
            = (0x1ffffU & ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
                            << 0x1eU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
                                         >> 2U)));
        if ((0x121U >= (0x1ffU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))))) {
            VL_ASSIGNSEL_WIIW(145,(0x1ffU & ((IData)(0x91U) 
                                             * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))), this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n, this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1);
        }
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__gate_clock = 0U;
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q)));
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (7U & ((IData)(1U) + (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (7U & ((IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
           & (IData)(this->__PVT__i_w_stream_fifo__DOT__pop)) 
          & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[0U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[0U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[1U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[1U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[2U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[2U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[3U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[3U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[4U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[4U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[5U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[5U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[6U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[6U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[7U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[7U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[8U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[8U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[9U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[9U];
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[0U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
                << 0xbU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
                            >> 0x15U));
        this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[1U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] 
                << 0xbU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
                            >> 0x15U));
        this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[2U] 
            = (0x3ffU & ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[7U] 
                          << 0xbU) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] 
                                      >> 0x15U)));
        if ((0x127U >= (0x1ffU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))))) {
            VL_ASSIGNSEL_WIIW(74,(0x1ffU & ((IData)(0x4aU) 
                                            * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))), this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n, this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1);
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[5U] >> 0x10U) 
           & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_chan));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[5U] >> 0x10U) 
           & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_sel));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[3U] >> 5U) & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_w_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[0U] >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_chan));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[0U] >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_sel));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->__PVT__axi_comp_req[3U] >> 4U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->__PVT__axi_comp_req[3U] >> 4U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ this->__PVT__axi_comp_req[0U]));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & this->__PVT__axi_comp_req[0U]);
    this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_d 
        = this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q;
    if ((1U & (((this->__PVT__axi_comp_req[5U] >> 0x10U) 
                & (this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                   >> 0x13U)) ^ (IData)(this->__PVT__decr_w_cnt)))) {
        this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_d 
            = (7U & ((IData)(this->__PVT__decr_w_cnt)
                      ? ((IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q) 
                         - (IData)(1U)) : ((IData)(1U) 
                                           + (IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q))));
    }
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_sequent__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__9(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_sequent__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__9\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_n));
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n));
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_n));
    if (vlTOPp->rst_ni) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q 
            = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_n;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q 
            = ((IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_n) 
               & 1U);
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q 
            = ((IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_n) 
               & 1U);
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n) 
               & 1U);
    } else {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = 0U;
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__gate_clock)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q 
            = this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_d;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_n) 
               & 1U);
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_n) 
               & 1U);
    } else {
        this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__gate_clock)))) {
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[0U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[1U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[2U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[3U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[4U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[5U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[6U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[7U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[8U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[9U];
        }
    } else {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__gate_clock)))) {
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[0U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[1U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[2U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[3U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[4U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[5U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[6U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[7U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[8U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[9U];
        }
    } else {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] = 0U;
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__gate_clock)))) {
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[0U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[1U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[2U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[3U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[4U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[5U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[6U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[7U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[8U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[9U];
        }
    } else {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] = 0U;
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d));
    if (vlTOPp->rst_ni) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d;
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__trans_req_data_q[0U] = this->__PVT__trans_req_data_n[0U];
        this->__PVT__trans_req_data_q[1U] = this->__PVT__trans_req_data_n[1U];
        this->__PVT__trans_req_data_q[2U] = this->__PVT__trans_req_data_n[2U];
        this->__PVT__trans_req_data_q[3U] = this->__PVT__trans_req_data_n[3U];
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d;
    } else {
        this->__PVT__trans_req_data_q[0U] = 0U;
        this->__PVT__trans_req_data_q[1U] = 0U;
        this->__PVT__trans_req_data_q[2U] = 0U;
        this->__PVT__trans_req_data_q[3U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_q 
                = this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_n));
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__gate_clock)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d)
            : 0U);
    if (vlTOPp->rst_ni) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__state_q = this->__PVT__state_n;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q 
            = (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes) 
                & (~ this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U])) 
               & 1U);
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q 
            = (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes) 
                & (~ (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                      >> 4U))) & 1U);
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n) 
               & 1U);
    } else {
        this->__PVT__state_q = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = 0U;
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__gate_clock)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q 
            = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q 
            = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q 
            = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n;
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if ((0x121U >= (0x1ffU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))) {
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(1U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  (0xfU & (((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                           >> 5U))] >> (0x1fU & ((IData)(0x91U) 
                                                 * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(2U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(1U) + (0xfU & (((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(3U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(2U) + (0xfU & (((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(4U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(3U) + (0xfU & (((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
            = (0x1ffffU & (((0U == (0x1fU & ((IData)(0x91U) 
                                             * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                             ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                                     ((IData)(5U) + 
                                      (0xfU & (((IData)(0x91U) 
                                                * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                               >> 5U)))] 
                                     << ((IData)(0x20U) 
                                         - (0x1fU & 
                                            ((IData)(0x91U) 
                                             * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
                           | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                              ((IData)(4U) + (0xfU 
                                              & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                              >> (0x1fU & ((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))));
    } else {
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] = 0U;
    }
    if ((0x12dU >= (0x1ffU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))) {
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(1U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  (0xfU & (((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                           >> 5U))] >> (0x1fU & ((IData)(0x97U) 
                                                 * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(2U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(1U) + (0xfU & (((IData)(0x97U) 
                                           * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(3U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(2U) + (0xfU & (((IData)(0x97U) 
                                           * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(4U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(3U) + (0xfU & (((IData)(0x97U) 
                                           * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
            = (0x7fffffU & (((0U == (0x1fU & ((IData)(0x97U) 
                                              * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                              ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                                      ((IData)(5U) 
                                       + (0xfU & (((IData)(0x97U) 
                                                   * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                  >> 5U)))] 
                                      << ((IData)(0x20U) 
                                          - (0x1fU 
                                             & ((IData)(0x97U) 
                                                * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
                            | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                               ((IData)(4U) + (0xfU 
                                               & (((IData)(0x97U) 
                                                   * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                  >> 5U)))] 
                               >> (0x1fU & ((IData)(0x97U) 
                                            * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))));
    } else {
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] = 0U;
    }
    if ((0x127U >= (0x1ffU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))) {
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
            = (((0U == (0x1fU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(1U) + (0xfU & (((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  (0xfU & (((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                           >> 5U))] >> (0x1fU & ((IData)(0x4aU) 
                                                 * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
            = (((0U == (0x1fU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(2U) + (0xfU & (((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(1U) + (0xfU & (((IData)(0x4aU) 
                                           * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
            = (0x3ffU & (((0U == (0x1fU & ((IData)(0x4aU) 
                                           * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                           ? 0U : (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                                   ((IData)(3U) + (0xfU 
                                                   & (((IData)(0x4aU) 
                                                       * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                      >> 5U)))] 
                                   << ((IData)(0x20U) 
                                       - (0x1fU & ((IData)(0x4aU) 
                                                   * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
                         | (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                            ((IData)(2U) + (0xfU & 
                                            (((IData)(0x4aU) 
                                              * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                             >> 5U)))] 
                            >> (0x1fU & ((IData)(0x4aU) 
                                         * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))));
    } else {
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] = 0U;
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] = 0U;
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] = 0U;
    }
    this->__PVT__is_read = ((((1U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                << 8U) 
                                               | (this->__PVT__trans_req_data_q[0U] 
                                                  >> 0x18U)))) 
                              | (2U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                  << 8U) 
                                                 | (this->__PVT__trans_req_data_q[0U] 
                                                    >> 0x18U))))) 
                             | (6U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                 << 8U) 
                                                | (this->__PVT__trans_req_data_q[0U] 
                                                   >> 0x18U))))) 
                            | (6U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                << 8U) 
                                               | (this->__PVT__trans_req_data_q[0U] 
                                                  >> 0x18U)))));
    this->__PVT__is_write = ((3U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                               << 8U) 
                                              | (this->__PVT__trans_req_data_q[0U] 
                                                 >> 0x18U)))) 
                             | (7U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                 << 8U) 
                                                | (this->__PVT__trans_req_data_q[0U] 
                                                   >> 0x18U)))));
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q 
                = (1U & (~ (this->__PVT__trans_resp_data_q[5U] 
                            >> 0x17U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_load = 0U;
    if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q)))) {
        if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_load = 1U;
        }
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q 
                = (1U & (~ (this->__PVT__trans_resp_data_q[5U] 
                            >> 0x17U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q = 0U;
    }
    this->trans_resp_ready_o = 0U;
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((2U & (IData)(this->__PVT__state_q))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                this->trans_resp_ready_o = 1U;
            }
        }
    }
    this->trans_req_valid_o = 0U;
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(this->__PVT__state_q))) {
                this->trans_req_valid_o = 1U;
            }
        }
    }
    this->i_iommu_axi_err_slv__DOT____Vcellout__i_r_fifo__data_o 
        = ((0xbU >= (0xfU & ((IData)(0xcU) * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q))))
            ? (0xfffU & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q) 
                         >> (0xfU & ((IData)(0xcU) 
                                     * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q)))))
            : 0U);
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xf01ffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = ((0xf0fffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]) 
           | (0xf000U & (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q) 
                          >> (7U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q) 
                                    << 2U))) << 0xcU)));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0x800U | (0xff3ffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xeffffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
            = (0x10000U | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = ((this->__PVT__axi_comp_req[1U] << 0x1eU) 
                   | (this->__PVT__axi_comp_req[0U] 
                      >> 2U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = ((this->__PVT__axi_comp_req[2U] << 0x1eU) 
                   | (this->__PVT__axi_comp_req[1U] 
                      >> 2U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = ((this->__PVT__axi_comp_req[3U] << 0x1eU) 
                   | (this->__PVT__axi_comp_req[2U] 
                      >> 2U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] 
                = (3U & ((this->__PVT__axi_comp_req[4U] 
                          << 0x1eU) | (this->__PVT__axi_comp_req[3U] 
                                       >> 2U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = ((this->__PVT__axi_comp_req[4U] << 0x1aU) 
                   | (this->__PVT__axi_comp_req[3U] 
                      >> 6U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = ((this->__PVT__axi_comp_req[5U] << 0x1aU) 
                   | (this->__PVT__axi_comp_req[4U] 
                      >> 6U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = (0x3ffU & ((this->__PVT__axi_comp_req[6U] 
                              << 0x1aU) | (this->__PVT__axi_comp_req[5U] 
                                           >> 6U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = ((this->__PVT__axi_comp_req[6U] << 0xfU) 
                   | (this->__PVT__axi_comp_req[5U] 
                      >> 0x11U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = ((this->__PVT__axi_comp_req[7U] << 0xfU) 
                   | (this->__PVT__axi_comp_req[6U] 
                      >> 0x11U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = ((this->__PVT__axi_comp_req[8U] << 0xfU) 
                   | (this->__PVT__axi_comp_req[7U] 
                      >> 0x11U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] 
                = (0xffU & (this->__PVT__axi_comp_req[8U] 
                            >> 0x11U));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = ((0x3ffffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]) 
           | (0xfffc0000U & (((1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
                              << 0x13U) | ((1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)) 
                                           << 0x12U))));
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = (0xffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q 
                = (0x7fU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                            >> 9U));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q = 0U;
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_chan 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_w_spill_reg__ready_o 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_chan 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_r_spill_reg__ready_o 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xffeffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xffffff00U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                              | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                             << 8U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_b_spill_reg__ready_o 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xeffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xffff0000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                              | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                             << 0x10U)));
    this->__PVT__trans_req_data_n[0U] = this->__PVT__trans_req_data_q[0U];
    this->__PVT__trans_req_data_n[1U] = this->__PVT__trans_req_data_q[1U];
    this->__PVT__trans_req_data_n[2U] = this->__PVT__trans_req_data_q[2U];
    this->__PVT__trans_req_data_n[3U] = this->__PVT__trans_req_data_q[3U];
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if (((0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                     & (~ ((IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q) 
                           >> 2U)))) {
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0x3fffffffU & this->__PVT__trans_req_data_n[0U]) 
                           | (0xc0000000U & ((IData)(
                                                     (((QData)((IData)(
                                                                       this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                       << 0x2dU) 
                                                      | (((QData)((IData)(
                                                                          this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                          << 0xdU) 
                                                         | ((QData)((IData)(
                                                                            this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                            >> 0x13U)))) 
                                             << 0x1eU)));
                    this->__PVT__trans_req_data_n[1U] 
                        = ((0x3fffffffU & ((IData)(
                                                   (((QData)((IData)(
                                                                     this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                     << 0x2dU) 
                                                    | (((QData)((IData)(
                                                                        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                        << 0xdU) 
                                                       | ((QData)((IData)(
                                                                          this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                          >> 0x13U)))) 
                                           >> 2U)) 
                           | (0xc0000000U & ((IData)(
                                                     ((((QData)((IData)(
                                                                        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                        << 0x2dU) 
                                                       | (((QData)((IData)(
                                                                           this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                           << 0xdU) 
                                                          | ((QData)((IData)(
                                                                             this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                             >> 0x13U))) 
                                                      >> 0x20U)) 
                                             << 0x1eU)));
                    this->__PVT__trans_req_data_n[2U] 
                        = ((0xc0000000U & this->__PVT__trans_req_data_n[2U]) 
                           | (0x3fffffffU & ((IData)(
                                                     ((((QData)((IData)(
                                                                        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                        << 0x2dU) 
                                                       | (((QData)((IData)(
                                                                           this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                           << 0xdU) 
                                                          | ((QData)((IData)(
                                                                             this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                             >> 0x13U))) 
                                                      >> 0x20U)) 
                                             >> 2U)));
                    this->__PVT__trans_req_data_n[2U] 
                        = ((0x3fffffffU & this->__PVT__trans_req_data_n[2U]) 
                           | (0xc0000000U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                             << 7U)));
                    this->__PVT__trans_req_data_n[3U] 
                        = (0x3fffffU & ((0x3fffff80U 
                                         & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                            << 7U)) 
                                        | (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                           >> 0x19U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xfffffff7U & this->__PVT__trans_req_data_n[0U]) 
                           | (8U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                    >> 0x13U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xff00000fU & this->__PVT__trans_req_data_n[0U]) 
                           | (0xfffff0U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                           << 2U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xc0ffffffU & this->__PVT__trans_req_data_n[0U]) 
                           | (0xff000000U & (((1U & 
                                               ((this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                                 >> 1U) 
                                                & (~ 
                                                   this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U])))
                                               ? 7U
                                               : 3U) 
                                             << 0x18U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xfffffffbU & this->__PVT__trans_req_data_n[0U]) 
                           | (4U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                    >> 0x1cU)));
                    this->__PVT__trans_req_data_n[0U] 
                        = (0xfffffffdU & this->__PVT__trans_req_data_n[0U]);
                } else {
                    if ((0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) {
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0x3fffffffU & this->__PVT__trans_req_data_n[0U]) 
                               | (0xc0000000U & ((IData)(
                                                         (((QData)((IData)(
                                                                           this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                           << 0x33U) 
                                                          | (((QData)((IData)(
                                                                              this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                              << 0x13U) 
                                                             | ((QData)((IData)(
                                                                                this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                                >> 0xdU)))) 
                                                 << 0x1eU)));
                        this->__PVT__trans_req_data_n[1U] 
                            = ((0x3fffffffU & ((IData)(
                                                       (((QData)((IData)(
                                                                         this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                         << 0x33U) 
                                                        | (((QData)((IData)(
                                                                            this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                            << 0x13U) 
                                                           | ((QData)((IData)(
                                                                              this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                              >> 0xdU)))) 
                                               >> 2U)) 
                               | (0xc0000000U & ((IData)(
                                                         ((((QData)((IData)(
                                                                            this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                            << 0x33U) 
                                                           | (((QData)((IData)(
                                                                               this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                               << 0x13U) 
                                                              | ((QData)((IData)(
                                                                                this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                                 >> 0xdU))) 
                                                          >> 0x20U)) 
                                                 << 0x1eU)));
                        this->__PVT__trans_req_data_n[2U] 
                            = ((0xc0000000U & this->__PVT__trans_req_data_n[2U]) 
                               | (0x3fffffffU & ((IData)(
                                                         ((((QData)((IData)(
                                                                            this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                            << 0x33U) 
                                                           | (((QData)((IData)(
                                                                               this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                               << 0x13U) 
                                                              | ((QData)((IData)(
                                                                                this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                                 >> 0xdU))) 
                                                          >> 0x20U)) 
                                                 >> 2U)));
                        this->__PVT__trans_req_data_n[2U] 
                            = ((0x3fffffffU & this->__PVT__trans_req_data_n[2U]) 
                               | (0xc0000000U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                                 << 7U)));
                        this->__PVT__trans_req_data_n[3U] 
                            = (0x3fffffU & ((0x3fffff80U 
                                             & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                << 7U)) 
                                            | (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                               >> 0x19U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xfffffff7U & this->__PVT__trans_req_data_n[0U]) 
                               | (8U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                        >> 0x13U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xff00000fU & this->__PVT__trans_req_data_n[0U]) 
                               | (0xfffff0U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                               << 2U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xc0ffffffU & this->__PVT__trans_req_data_n[0U]) 
                               | (0xff000000U & (((1U 
                                                   & ((this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                                       >> 1U) 
                                                      & (~ 
                                                         this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U])))
                                                   ? 
                                                  ((0x4000000U 
                                                    & this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U])
                                                    ? 5U
                                                    : 6U)
                                                   : 
                                                  ((0x4000000U 
                                                    & this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U])
                                                    ? 1U
                                                    : 2U)) 
                                                 << 0x18U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xfffffffbU & this->__PVT__trans_req_data_n[0U]) 
                               | (4U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                        >> 0x16U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = (0xfffffffdU & this->__PVT__trans_req_data_n[0U]);
                    }
                }
            }
        }
    }
    this->__PVT__complete_read = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                this->__PVT__complete_read = this->__PVT__is_read;
            }
        }
    }
    this->__PVT__complete_write = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                this->__PVT__complete_write = this->__PVT__is_write;
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] = 0U;
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[1U] = 0U;
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xfff00U & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = ((0xfff0fU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]) 
           | (0xf0U & ((IData)(this->i_iommu_axi_err_slv__DOT____Vcellout__i_r_fifo__data_o) 
                       >> 4U)));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] 
        = (0xadcab1e0U | (0xfU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[1U] = 0xa11ab1ebU;
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xcU | (0xffff0U & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] 
        = (8U | (0xfffffff3U & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xffeffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
            = (0x100U | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] 
            = ((0xfffffffdU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U]) 
               | (0xfffffffeU & ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q))) 
                                 << 1U)));
    }
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xdffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfffe0000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_w_spill_reg__ready_o) 
                             << 0x11U)));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xbffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfffc0000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_chan) 
                              & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_sel)) 
                             << 0x12U)));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0x7ffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfff80000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_chan) 
                              & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_sel)) 
                             << 0x13U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
        = ((0xfffffffeU & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U]) 
           | (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_r_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
        = ((0xffffffefU & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U]) 
           | (0xfffffff0U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_b_spill_reg__ready_o) 
                             << 4U)));
    if (vlTOPp->rst_ni) {
        this->__PVT__trans_resp_data_q[0U] = this->__PVT__trans_resp_data_n[0U];
        this->__PVT__trans_resp_data_q[1U] = this->__PVT__trans_resp_data_n[1U];
        this->__PVT__trans_resp_data_q[2U] = this->__PVT__trans_resp_data_n[2U];
        this->__PVT__trans_resp_data_q[3U] = this->__PVT__trans_resp_data_n[3U];
        this->__PVT__trans_resp_data_q[4U] = this->__PVT__trans_resp_data_n[4U];
        this->__PVT__trans_resp_data_q[5U] = this->__PVT__trans_resp_data_n[5U];
    } else {
        this->__PVT__trans_resp_data_q[0U] = 0U;
        this->__PVT__trans_resp_data_q[1U] = 0U;
        this->__PVT__trans_resp_data_q[2U] = 0U;
        this->__PVT__trans_resp_data_q[3U] = 0U;
        this->__PVT__trans_resp_data_q[4U] = 0U;
        this->__PVT__trans_resp_data_q[5U] = 0U;
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
        = ((1U & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U]) 
           | (0xfffffffeU & ((0xfffffffcU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                             << 2U)) 
                             | ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                  | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                 & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                    | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                << 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
        = ((1U & ((1U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                           ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                           : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                         >> 0x1eU)) | ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                         | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                        & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                           | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                       >> 0x1fU))) 
           | (0xfffffffeU & ((2U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                      ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                      : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                    >> 0x1eU)) | (0xfffffffcU 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                     << 2U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
        = ((1U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                    ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                    : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                  >> 0x1eU)) | (0xfffffffeU & ((2U 
                                                & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                     ? 
                                                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                     : 
                                                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                   >> 0x1eU)) 
                                               | (0xfffffffcU 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                     << 2U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
        = ((0xfffffff0U & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U]) 
           | ((1U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                     >> 0x1eU)) | (0xfffffffeU & ((2U 
                                                   & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                        ? 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                        : 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                      >> 0x1eU)) 
                                                  | (0xfffffffcU 
                                                     & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                          ? 
                                                         this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U]
                                                          : 
                                                         this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U]) 
                                                        << 2U))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
        = ((0x1fU & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U]) 
           | (0xffffffe0U & ((0xffffffc0U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                             << 6U)) 
                             | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                 | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                << 5U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
        = ((0x1fU & ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                 ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                 : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                               >> 0x1aU)) | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                              | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                             >> 0x1bU))) 
           | (0xffffffe0U & ((0x20U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                         ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                         : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                       >> 0x1aU)) | 
                             (0xffffffc0U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                             << 6U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
        = ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                     >> 0x1aU)) | (0xffffffe0U & ((0xfffe0000U 
                                                   & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                        ? 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                                        : 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                                      << 0x11U)) 
                                                  | ((0xffff0000U 
                                                      & ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                                           | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                                          & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                                             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                                         << 0x10U)) 
                                                     | ((0x20U 
                                                         & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                              ? 
                                                             this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                              : 
                                                             this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                            >> 0x1aU)) 
                                                        | (0xffffffc0U 
                                                           & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                                ? 
                                                               this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                                : 
                                                               this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                              << 6U)))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
        = ((0x1fU & ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                        ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                        : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                      >> 0xfU) | (((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                     | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                    & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                       | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                   >> 0x10U) | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                  ? 
                                                 this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                  : 
                                                 this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                >> 0x1aU)))) 
           | (0xffffffe0U & ((0x1ffe0U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                            ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                            : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                          >> 0xfU)) 
                             | (0xfffe0000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                 ? 
                                                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                 : 
                                                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                               << 0x11U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
        = ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                     >> 0xfU)) | (0xffffffe0U & ((0x1ffe0U 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                     >> 0xfU)) 
                                                 | (0xfffe0000U 
                                                    & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                         ? 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                         : 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                       << 0x11U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
        = ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                     >> 0xfU)) | (0xffffffe0U & ((0x1ffe0U 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                     >> 0xfU)) 
                                                 | (0xfffe0000U 
                                                    & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                         ? 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U]
                                                         : 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U]) 
                                                       << 0x11U)))));
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) {
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[0U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U];
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[1U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U];
    } else {
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[0U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[1U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
    }
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xfff00U & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xf01ffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfffffe00U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                               ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q)
                               : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q)) 
                             << 9U)));
    this->__PVT__ar_ready = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                       >> 0x12U) & (IData)(this->__PVT__is_read)) 
                     | ((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                         >> 0x13U) & (IData)(this->__PVT__is_write)))) {
                    this->__PVT__ar_ready = this->__PVT__is_read;
                }
            }
        }
    }
    this->__PVT__aw_ready = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                       >> 0x12U) & (IData)(this->__PVT__is_read)) 
                     | ((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                         >> 0x13U) & (IData)(this->__PVT__is_write)))) {
                    this->__PVT__aw_ready = this->__PVT__is_write;
                }
            }
        }
    }
    this->__PVT__w_ready = ((0U != (3U & (IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q))) 
                            & (this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                               >> 0x11U));
    this->tr_axi_resp_o[0U] = this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[0U];
    this->tr_axi_resp_o[1U] = this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[1U];
    this->tr_axi_resp_o[2U] = this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U];
    this->tr_axi_resp_o[2U] = ((0x7ffffU & this->tr_axi_resp_o[2U]) 
                               | (0xfff80000U & ((2U 
                                                  != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                                 << 0x13U)));
    this->tr_axi_resp_o[2U] = ((0xbffffU & this->tr_axi_resp_o[2U]) 
                               | (0xfffc0000U & ((2U 
                                                  != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                                 << 0x12U)));
    this->tr_axi_resp_o[2U] = ((0xdffffU & this->tr_axi_resp_o[2U]) 
                               | (0xfffe0000U & ((4U 
                                                  != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                                 << 0x11U)));
    this->__PVT__trans_resp_data_n[0U] = this->__PVT__trans_resp_data_q[0U];
    this->__PVT__trans_resp_data_n[1U] = this->__PVT__trans_resp_data_q[1U];
    this->__PVT__trans_resp_data_n[2U] = this->__PVT__trans_resp_data_q[2U];
    this->__PVT__trans_resp_data_n[3U] = this->__PVT__trans_resp_data_q[3U];
    this->__PVT__trans_resp_data_n[4U] = this->__PVT__trans_resp_data_q[4U];
    this->__PVT__trans_resp_data_n[5U] = this->__PVT__trans_resp_data_q[5U];
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((2U & (IData)(this->__PVT__state_q))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((1U & (IData)(vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_valid))) {
                    this->__PVT__trans_resp_data_n[0U] 
                        = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[0U];
                    this->__PVT__trans_resp_data_n[1U] 
                        = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[1U];
                    this->__PVT__trans_resp_data_n[2U] 
                        = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[2U];
                    this->__PVT__trans_resp_data_n[3U] 
                        = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[3U];
                    this->__PVT__trans_resp_data_n[4U] 
                        = vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[4U];
                    this->__PVT__trans_resp_data_n[5U] 
                        = (0x1ffffffU & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[5U]);
                }
            }
        }
    }
    this->__PVT__i_ar_stream_fifo__DOT__pop = ((IData)(this->__PVT__ar_ready) 
                                               & (0U 
                                                  != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_aw_stream_fifo__DOT__pop = ((IData)(this->__PVT__aw_ready) 
                                               & (0U 
                                                  != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__decr_w_cnt = (((0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                & (IData)(this->__PVT__w_ready)) 
                               & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                  >> 1U));
    this->__PVT__i_w_stream_fifo__DOT__pop = ((IData)(this->__PVT__w_ready) 
                                              & (0U 
                                                 != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)));
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)));
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)));
    }
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_sequent__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__11(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_sequent__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__11\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q)
            : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q)
            : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select 
        = ((0U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_q)
            : (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid = 1U;
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid = 1U;
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up = 0U;
    if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q)))) {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up = 1U;
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid = 1U;
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid = 1U;
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up) 
           | (0U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))));
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_combo__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__15(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_combo__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__15\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_en = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_en = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_clear = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_clear = 1U;
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop = 1U;
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d 
        = this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d = 0U;
            }
        }
    } else {
        if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load = 1U;
            }
        }
    } else {
        if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
            >> 1U) & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)));
    this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop = 0U;
    if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop 
            = (1U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                     >> 4U));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
            >> 0x10U) & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)));
    this->__PVT__state_n = this->__PVT__state_q;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((2U & (IData)(this->__PVT__state_q))) {
            this->__PVT__state_n = 0U;
        } else {
            if ((1U & (IData)(this->__PVT__state_q))) {
                this->__PVT__state_n = 0U;
            } else {
                if ((((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                       >> 0x12U) & (IData)(this->__PVT__is_read)) 
                     | ((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                         >> 0x13U) & (IData)(this->__PVT__is_write)))) {
                    this->__PVT__state_n = 0U;
                }
            }
        }
    } else {
        if ((2U & (IData)(this->__PVT__state_q))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((1U & (IData)(vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_valid))) {
                    this->__PVT__state_n = ((1U & (
                                                   (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[3U] 
                                                    >> 0x1dU) 
                                                   & (~ 
                                                      (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[5U] 
                                                       >> 0x17U))))
                                             ? 3U : 4U);
                }
            }
        } else {
            if ((1U & (IData)(this->__PVT__state_q))) {
                if ((1U & (IData)(vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_req_ready))) {
                    this->__PVT__state_n = 2U;
                }
            } else {
                if (((0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                     & (~ ((IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q) 
                           >> 2U)))) {
                    this->__PVT__state_n = 1U;
                } else {
                    if ((0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) {
                        this->__PVT__state_n = 1U;
                    }
                }
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_clear) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d = 0U;
    } else {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_load) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d 
                = (0xffU & (IData)(this->i_iommu_axi_err_slv__DOT____Vcellout__i_r_fifo__data_o));
        } else {
            if (this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_en) {
                this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d 
                    = (0x1ffU & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q) 
                                 - (IData)(1U)));
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__gate_clock = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
           & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop)) 
          & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->i_iommu_axi_err_slv__DOT__i_r_fifo__DOT____Vlvbound1 
            = this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_inp;
        if ((0xbU >= (0xfU & ((IData)(0xcU) * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q))))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n 
                = (((~ ((IData)(0xfffU) << (0xfU & 
                                            ((IData)(0xcU) 
                                             * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q))))) 
                    & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n)) 
                   | ((IData)(this->i_iommu_axi_err_slv__DOT__i_r_fifo__DOT____Vlvbound1) 
                      << (0xfU & ((IData)(0xcU) * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q)))));
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__gate_clock = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n 
            = (((~ ((IData)(0xfU) << (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q) 
                                            << 2U)))) 
                & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n)) 
               | ((0xfU & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] 
                            << 0xbU) | (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
                                        >> 0x15U))) 
                  << (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q) 
                            << 2U))));
    }
    this->i_iommu_axi_err_slv__DOT____Vcellout__i_w_fifo__data_o 
        = (0xfU & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q) 
                   >> (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q) 
                             << 2U))));
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        this->i_iommu_axi_err_slv__DOT____Vcellout__i_w_fifo__data_o 
            = (0xfU & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] 
                        << 0xbU) | (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
                                    >> 0x15U)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty 
        = ((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
           & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push)));
    this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop = 0U;
    if (((~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        if ((1U & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                    >> 5U) & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                              >> 7U)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push = 0U;
    if (((~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        if ((1U & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                    >> 5U) & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                              >> 7U)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xdffffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    if (((~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
            = (0x20000U | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q)));
    }
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) 
         & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
           & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop)) 
          & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q))) 
         & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q;
    }
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) 
         & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q)));
    }
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__gate_clock = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
           & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop)) 
          & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n 
            = (((~ ((IData)(0xfU) << (7U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q) 
                                            << 2U)))) 
                & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n)) 
               | ((IData)(this->i_iommu_axi_err_slv__DOT____Vcellout__i_w_fifo__data_o) 
                  << (7U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q) 
                            << 2U))));
    }
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[0U] 
        = this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U];
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[1U] 
        = this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[1U];
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
        = ((0xfff00000U & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[0U] 
                           << 0x14U)) | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[3U] 
        = ((0xfffffU & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[0U] 
                        >> 0xcU)) | (0xfff00000U & 
                                     (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[1U] 
                                      << 0x14U)));
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[4U] 
        = ((0xfffffU & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[1U] 
                        >> 0xcU)) | (0xfff00000U & 
                                     (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[2U] 
                                      << 0x14U)));
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U] 
        = (0xffU & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[2U] 
                    >> 0xcU));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down = 0U;
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select)))) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down 
            = (1U & (((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                       >> 5U) & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
                                 >> 0x11U)) & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                                               >> 7U)));
    }
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select))) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down 
            = (1U & (((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                       & this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U]) 
                      >> 5U) & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                                >> 7U)));
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = (0xdffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xdffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0x20000U & this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U]));
    }
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xdffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0x20000U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U] 
                              << 0xcU)));
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid) 
           & ((0xa7U >= (0xffU & ((IData)(0x52U) + 
                                  ((IData)(0x54U) * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))))) 
              & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                 (7U & (((IData)(0x52U) + ((IData)(0x54U) 
                                           * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) 
                        >> 5U))] >> (0x1fU & ((IData)(0x52U) 
                                              + ((IData)(0x54U) 
                                                 * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid) 
           & ((0xa7U >= (0xffU & ((IData)(0x53U) + 
                                  ((IData)(0x54U) * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))))) 
              & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                 (7U & (((IData)(0x53U) + ((IData)(0x54U) 
                                           * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) 
                        >> 5U))] >> (0x1fU & ((IData)(0x53U) 
                                              + ((IData)(0x54U) 
                                                 * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids)) 
           | (1U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
                    >> 0x10U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids)) 
           | (1U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
                    >> 8U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids)) 
           | (2U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U] 
                    >> 3U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids)) 
           | (2U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[4U] 
                    >> 0x1bU)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d 
        = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q;
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up) 
         ^ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down))) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down)
                        ? ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q) 
                           - (IData)(1U)) : ((IData)(1U) 
                                             + (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))));
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d 
        = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d = 0U;
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock = 1U;
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push = 1U;
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push = 1U;
                }
            }
        }
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = (0xbffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                = (0x40000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
                    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                        = (0x40000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d 
        = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d = 0U;
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock = 1U;
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject 
                = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                         >> 0x17U));
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject 
                        = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                                 >> 0x17U));
                }
            }
        }
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = (0x7ffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                = (0x80000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
                    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                        = (0x80000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q)
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q)
            : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q)
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q)
            : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffffeU & (((1U <= (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffffeU & (((1U > (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d)) 
                 | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d) 
                     >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d) 
                 | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d) 
                    >> 1U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffffeU & (((1U <= (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffffeU & (((1U > (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d)) 
                 | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d) 
                     >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d) 
                 | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d) 
                    >> 1U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies)) 
           | (1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                     >> 4U) & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies)) 
           | (0x1ffffffeU & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                              >> 3U) & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel) 
                                        << 1U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = ((0xeffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
           | (0xffff0000U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes) 
                             << 0x10U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies)) 
           | (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                    & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies)) 
           | ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
               & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)) 
              << 1U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = ((0xffeffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
           | (0xffffff00U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes) 
                             << 8U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                    | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                       >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                    | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                       >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[2U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[7U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xaU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xcU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xdU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xfU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x11U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
        = (0xffU & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                    >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
        = ((0x1ffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]) 
           | (0xfffe0000U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                             << 0x11U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] 
        = ((0x1ffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                        >> 0xfU)) | (0xfffe0000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                                      << 0x11U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[7U] 
        = ((0x1ffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                        >> 0xfU)) | (0xfffe0000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                                      << 0x11U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = ((0xfe000000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]) 
           | ((0x1ffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                           >> 0xfU)) | (0xfffe0000U 
                                        & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
                                           << 0x11U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
        = (0xfffeffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid) 
         & (~ (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
            = (0x10000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
        = (0x3ffU & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                      << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                                   >> 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = ((0x3fU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
           | (0xffffffc0U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                             << 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
        = ((0x3fU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                     >> 0x1aU)) | (0xffffffc0U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                                                  << 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
        = ((0xffff0000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]) 
           | ((0x3fU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                        >> 0x1aU)) | (0xffffffc0U & 
                                      (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
                                       << 6U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = (0xffffffdfU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6 
            = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                     >> 5U));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
            = ((0xffffffdfU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
               | (0xffffffe0U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6) 
                                 << 5U)));
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7 
        = (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = ((0xffffffefU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
           | (0xfffffff0U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7) 
                             << 4U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
        = (3U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                  << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                               >> 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
        = ((3U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]) 
           | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                             << 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] 
        = ((3U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                  >> 0x1eU)) | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                                               << 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[2U] 
        = ((3U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                  >> 0x1eU)) | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                                               << 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = ((0xfffffff0U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
           | ((3U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                     >> 0x1eU)) | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
                                                  << 2U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
        = (0xfffffffdU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid) 
         & (~ (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
            = (2U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11 
        = (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
        = ((0xfffffffeU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]) 
           | (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
        = (0xffU & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                    >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
        = ((0x3ffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]) 
           | (0xfffffc00U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                             << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xfU] 
        = ((0x3ffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                      >> 0x16U)) | (0xfffffc00U & (
                                                   this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                                                   << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] 
        = ((0x3ffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                      >> 0x16U)) | (0xfffffc00U & (
                                                   this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                                                   << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x11U] 
        = ((0x3ffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                      >> 0x16U)) | (0xfffffc00U & (
                                                   this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
                                                   << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
        = (0xfffffdffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid) 
         & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
            = (0x200U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
        = (0x3ffU & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                      << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                                   >> 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = ((0x7fffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
           | (0x80000000U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                             << 0x1fU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xcU] 
        = ((0x7fffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                           >> 1U)) | (0x80000000U & 
                                      (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                                       << 0x1fU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xdU] 
        = ((0x7fffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                           >> 1U)) | (0x80000000U & 
                                      (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
                                       << 0x1fU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
        = ((0xfffffe00U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]) 
           | (0x7fffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
                             >> 1U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = (0xbfffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6 
            = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                     >> 5U));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
            = ((0xbfffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
               | (0xc0000000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6) 
                                 << 0x1eU)));
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies) 
                 >> 1U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = ((0xdfffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
           | (0xe0000000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7) 
                             << 0x1dU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
        = (3U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                  << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                               >> 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = ((0x7ffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]) 
           | (0xf8000000U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                             << 0x1bU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] 
        = ((0x7ffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                          >> 5U)) | (0xf8000000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                                      << 0x1bU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xaU] 
        = ((0x7ffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                          >> 5U)) | (0xf8000000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                                      << 0x1bU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = ((0xe0000000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
           | ((0x7ffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                             >> 5U)) | (0xf8000000U 
                                        & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
                                           << 0x1bU))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = (0xfbffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid) 
         & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
            = (0x4000000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies) 
                 >> 1U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = ((0xfdffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]) 
           | (0xfe000000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11) 
                             << 0x19U)));
    if ((0x10000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U])) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xf0fffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffff000U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x4cU) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (0xfU & (((0U 
                                                == 
                                                (0x1fU 
                                                 & ((IData)(0x4cU) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                ? 0U
                                                : (
                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                   ((IData)(1U) 
                                                    + 
                                                    (7U 
                                                     & (((IData)(0x4cU) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                        >> 5U)))] 
                                                   << 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(0x4cU) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                              | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                 (7U 
                                                  & (((IData)(0x4cU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U))] 
                                                 >> 
                                                 (0x1fU 
                                                  & ((IData)(0x4cU) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 0xcU)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xff3ffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffffc00U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x4aU) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (3U & (((0U == 
                                              (0x1fU 
                                               & ((IData)(0x4aU) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                              ? 0U : 
                                             (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                              ((IData)(1U) 
                                               + (7U 
                                                  & (((IData)(0x4aU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U)))] 
                                              << ((IData)(0x20U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(0x4aU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                            | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                               (7U 
                                                & (((IData)(0x4aU) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                   >> 5U))] 
                                               >> (0x1fU 
                                                   & ((IData)(0x4aU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 0xaU)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xffdffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffffe00U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x49U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))) 
                                  & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                     (7U & (((IData)(0x49U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                            >> 5U))] 
                                     >> (0x1fU & ((IData)(0x49U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))) 
                                 << 9U)));
    } else {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = (0xf01ffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    }
    if ((0x100U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U])) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xfff0fU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffffff0U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x44U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (0xfU & (((0U 
                                                == 
                                                (0x1fU 
                                                 & ((IData)(0x44U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                ? 0U
                                                : (
                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                   ((IData)(1U) 
                                                    + 
                                                    (7U 
                                                     & (((IData)(0x44U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                        >> 5U)))] 
                                                   << 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(0x44U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                              | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                 (7U 
                                                  & (((IData)(0x44U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U))] 
                                                 >> 
                                                 (0x1fU 
                                                  & ((IData)(0x44U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 4U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | (0xfffffff0U & ((IData)(((0xa7U >= 
                                           (0xffU & 
                                            ((IData)(4U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                           ? (((0U 
                                                == 
                                                (0x1fU 
                                                 & ((IData)(4U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                ? 0ULL
                                                : ((QData)((IData)(
                                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                   ((IData)(2U) 
                                                                    + 
                                                                    (7U 
                                                                     & (((IData)(4U) 
                                                                         + 
                                                                         ((IData)(0x54U) 
                                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                        >> 5U)))])) 
                                                   << 
                                                   ((IData)(0x40U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                              | (((QData)((IData)(
                                                                  this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                  ((IData)(1U) 
                                                                   + 
                                                                   (7U 
                                                                    & (((IData)(4U) 
                                                                        + 
                                                                        ((IData)(0x54U) 
                                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                       >> 5U)))])) 
                                                  << 
                                                  ((0U 
                                                    == 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                    ? 0x20U
                                                    : 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                                 | ((QData)((IData)(
                                                                    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                    (7U 
                                                                     & (((IData)(4U) 
                                                                         + 
                                                                         ((IData)(0x54U) 
                                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                        >> 5U))])) 
                                                    >> 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                           : 0ULL)) 
                                 << 4U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[1U] 
            = ((0xfU & ((IData)(((0xa7U >= (0xffU & 
                                            ((IData)(4U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                  ? (((0U == (0x1fU 
                                              & ((IData)(4U) 
                                                 + 
                                                 ((IData)(0x54U) 
                                                  * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                       ? 0ULL : ((QData)((IData)(
                                                                 this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                 ((IData)(2U) 
                                                                  + 
                                                                  (7U 
                                                                   & (((IData)(4U) 
                                                                       + 
                                                                       ((IData)(0x54U) 
                                                                        * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                      >> 5U)))])) 
                                                 << 
                                                 ((IData)(0x40U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(4U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                     | (((QData)((IData)(
                                                         this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                         ((IData)(1U) 
                                                          + 
                                                          (7U 
                                                           & (((IData)(4U) 
                                                               + 
                                                               ((IData)(0x54U) 
                                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                              >> 5U)))])) 
                                         << ((0U == 
                                              (0x1fU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                              ? 0x20U
                                              : ((IData)(0x20U) 
                                                 - 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                        | ((QData)((IData)(
                                                           this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                           (7U 
                                                            & (((IData)(4U) 
                                                                + 
                                                                ((IData)(0x54U) 
                                                                 * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                               >> 5U))])) 
                                           >> (0x1fU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                  : 0ULL)) >> 0x1cU)) 
               | (0xfffffff0U & ((IData)((((0xa7U >= 
                                            (0xffU 
                                             & ((IData)(4U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                            ? (((0U 
                                                 == 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                 ? 0ULL
                                                 : 
                                                ((QData)((IData)(
                                                                 this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                 ((IData)(2U) 
                                                                  + 
                                                                  (7U 
                                                                   & (((IData)(4U) 
                                                                       + 
                                                                       ((IData)(0x54U) 
                                                                        * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                      >> 5U)))])) 
                                                 << 
                                                 ((IData)(0x40U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(4U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                               | (((QData)((IData)(
                                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                   ((IData)(1U) 
                                                                    + 
                                                                    (7U 
                                                                     & (((IData)(4U) 
                                                                         + 
                                                                         ((IData)(0x54U) 
                                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                        >> 5U)))])) 
                                                   << 
                                                   ((0U 
                                                     == 
                                                     (0x1fU 
                                                      & ((IData)(4U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                     ? 0x20U
                                                     : 
                                                    ((IData)(0x20U) 
                                                     - 
                                                     (0x1fU 
                                                      & ((IData)(4U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                                  | ((QData)((IData)(
                                                                     this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                     (7U 
                                                                      & (((IData)(4U) 
                                                                          + 
                                                                          ((IData)(0x54U) 
                                                                           * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                         >> 5U))])) 
                                                     >> 
                                                     (0x1fU 
                                                      & ((IData)(4U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                            : 0ULL) 
                                          >> 0x20U)) 
                                 << 4U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xffff0U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfU & ((IData)((((0xa7U >= (0xffU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                     ? (((0U == (0x1fU 
                                                 & ((IData)(4U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                          ? 0ULL : 
                                         ((QData)((IData)(
                                                          this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                          ((IData)(2U) 
                                                           + 
                                                           (7U 
                                                            & (((IData)(4U) 
                                                                + 
                                                                ((IData)(0x54U) 
                                                                 * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                               >> 5U)))])) 
                                          << ((IData)(0x40U) 
                                              - (0x1fU 
                                                 & ((IData)(4U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                        | (((QData)((IData)(
                                                            this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                            ((IData)(1U) 
                                                             + 
                                                             (7U 
                                                              & (((IData)(4U) 
                                                                  + 
                                                                  ((IData)(0x54U) 
                                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                 >> 5U)))])) 
                                            << ((0U 
                                                 == 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                 ? 0x20U
                                                 : 
                                                ((IData)(0x20U) 
                                                 - 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                           | ((QData)((IData)(
                                                              this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                              (7U 
                                                               & (((IData)(4U) 
                                                                   + 
                                                                   ((IData)(0x54U) 
                                                                    * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                  >> 5U))])) 
                                              >> (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                     : 0ULL) >> 0x20U)) 
                          >> 0x1cU)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfffffff3U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | (0xfffffffcU & (((0xa7U >= (0xffU 
                                             & ((IData)(2U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (3U & (((0U == 
                                              (0x1fU 
                                               & ((IData)(2U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                              ? 0U : 
                                             (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                              ((IData)(1U) 
                                               + (7U 
                                                  & (((IData)(2U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U)))] 
                                              << ((IData)(0x20U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(2U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                            | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                               (7U 
                                                & (((IData)(2U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                   >> 5U))] 
                                               >> (0x1fU 
                                                   & ((IData)(2U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 2U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfffffffdU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | (0xfffffffeU & (((0xa7U >= (0xffU 
                                             & ((IData)(1U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))) 
                                  & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                     (7U & (((IData)(1U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                            >> 5U))] 
                                     >> (0x1fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))) 
                                 << 1U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfffffffeU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | ((0xa7U >= (0xffU & ((IData)(0x54U) 
                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))) 
                  & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                     (7U & (((IData)(0x54U) * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)) 
                            >> 5U))] >> (0x1fU & ((IData)(0x54U) 
                                                  * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))));
    } else {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] = 0U;
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[1U] = 0U;
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = (0xfff00U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x13U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x13U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x13U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x13U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x11U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x11U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x12U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x12U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x12U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x12U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            >> 0x10U) & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_b_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            >> 8U) & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_r_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d 
        = (1U & (((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                   >> 4U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes))
                  ? ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))
                      ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)
                      : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes))
                  : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d 
        = (1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                  & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes))
                  ? ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))
                      ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)
                      : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes))
                  : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q)));
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_settle__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__8(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_settle__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__8\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_aw_stream_fifo__DOT__push = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x12U] 
                                                 >> 0x16U) 
                                                & (2U 
                                                   != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_ar_stream_fifo__DOT__push = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
                                                 >> 0x18U) 
                                                & (2U 
                                                   != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_w_stream_fifo__DOT__push = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] 
                                                >> 0xbU) 
                                               & (4U 
                                                  != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__axi_comp_req[8U] = ((0x1fffffU & this->__PVT__axi_comp_req[8U]) 
                                     | (0x1e00000U 
                                        & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
                                           << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0x1fffffU & this->__PVT__axi_comp_req[6U]) 
                                     | (0xffe00000U 
                                        & ((IData)(
                                                   (0xffffffffffffffULL 
                                                    & (((QData)((IData)(
                                                                        this->__PVT__trans_resp_data_q[5U])) 
                                                        << 0x22U) 
                                                       | (((QData)((IData)(
                                                                           this->__PVT__trans_resp_data_q[4U])) 
                                                           << 2U) 
                                                          | ((QData)((IData)(
                                                                             this->__PVT__trans_resp_data_q[3U])) 
                                                             >> 0x1eU))))) 
                                           << 0x15U)));
    this->__PVT__axi_comp_req[7U] = ((0x1fffffU & ((IData)(
                                                           (0xffffffffffffffULL 
                                                            & (((QData)((IData)(
                                                                                this->__PVT__trans_resp_data_q[5U])) 
                                                                << 0x22U) 
                                                               | (((QData)((IData)(
                                                                                this->__PVT__trans_resp_data_q[4U])) 
                                                                   << 2U) 
                                                                  | ((QData)((IData)(
                                                                                this->__PVT__trans_resp_data_q[3U])) 
                                                                     >> 0x1eU))))) 
                                                   >> 0xbU)) 
                                     | (0xffe00000U 
                                        & ((IData)(
                                                   ((0xffffffffffffffULL 
                                                     & (((QData)((IData)(
                                                                         this->__PVT__trans_resp_data_q[5U])) 
                                                         << 0x22U) 
                                                        | (((QData)((IData)(
                                                                            this->__PVT__trans_resp_data_q[4U])) 
                                                            << 2U) 
                                                           | ((QData)((IData)(
                                                                              this->__PVT__trans_resp_data_q[3U])) 
                                                              >> 0x1eU)))) 
                                                    >> 0x20U)) 
                                           << 0x15U)));
    this->__PVT__axi_comp_req[8U] = ((0x1e00000U & 
                                      this->__PVT__axi_comp_req[8U]) 
                                     | (0x1fffffU & 
                                        ((IData)(((0xffffffffffffffULL 
                                                   & (((QData)((IData)(
                                                                       this->__PVT__trans_resp_data_q[5U])) 
                                                       << 0x22U) 
                                                      | (((QData)((IData)(
                                                                          this->__PVT__trans_resp_data_q[4U])) 
                                                          << 2U) 
                                                         | ((QData)((IData)(
                                                                            this->__PVT__trans_resp_data_q[3U])) 
                                                            >> 0x1eU)))) 
                                                  >> 0x20U)) 
                                         >> 0xbU)));
    this->__PVT__axi_comp_req[6U] = ((0xffe01fffU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x1fe000U & 
                                        (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                         << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xffffe3ffU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x1c00U & (
                                                   this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                   << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xfffffcffU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x300U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                  << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xffffff7fU & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x80U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                 << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xffffff87U & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (0x78U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                 << 2U)));
    this->__PVT__axi_comp_req[6U] = ((0xfffffff8U & 
                                      this->__PVT__axi_comp_req[6U]) 
                                     | (7U & ((this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                               << 2U) 
                                              | (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                 >> 0x1eU))));
    this->__PVT__axi_comp_req[5U] = ((0xfffffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xf0000000U 
                                        & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                           << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xf0ffffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xf000000U 
                                        & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                           << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xff03ffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xfc0000U & 
                                        (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                         << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xfffdffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0x20000U & 
                                        (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                         << 2U)));
    this->__PVT__axi_comp_req[5U] = ((0xfffeffffU & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | (0xffff0000U 
                                        & ((IData)(this->__PVT__complete_write) 
                                           << 0x10U)));
    this->__PVT__axi_comp_req[3U] = ((0xfffffff0U & 
                                      this->__PVT__axi_comp_req[3U]) 
                                     | (0xfU & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
                                                >> 0xdU)));
    this->__PVT__axi_comp_req[1U] = (IData)((0xffffffffffffffULL 
                                             & (((QData)((IData)(
                                                                 this->__PVT__trans_resp_data_q[5U])) 
                                                 << 0x22U) 
                                                | (((QData)((IData)(
                                                                    this->__PVT__trans_resp_data_q[4U])) 
                                                    << 2U) 
                                                   | ((QData)((IData)(
                                                                      this->__PVT__trans_resp_data_q[3U])) 
                                                      >> 0x1eU)))));
    this->__PVT__axi_comp_req[2U] = (IData)(((0xffffffffffffffULL 
                                              & (((QData)((IData)(
                                                                  this->__PVT__trans_resp_data_q[5U])) 
                                                  << 0x22U) 
                                                 | (((QData)((IData)(
                                                                     this->__PVT__trans_resp_data_q[4U])) 
                                                     << 2U) 
                                                    | ((QData)((IData)(
                                                                       this->__PVT__trans_resp_data_q[3U])) 
                                                       >> 0x1eU)))) 
                                             >> 0x20U));
    this->__PVT__axi_comp_req[0U] = ((0xffffffU & this->__PVT__axi_comp_req[0U]) 
                                     | (0xff000000U 
                                        & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                           << 0x13U)));
    this->__PVT__axi_comp_req[0U] = ((0xff1fffffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0xe00000U & 
                                        (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                         << 0x13U)));
    this->__PVT__axi_comp_req[0U] = ((0xffe7ffffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x180000U & 
                                        (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                         << 0x13U)));
    this->__PVT__axi_comp_req[0U] = ((0xfffbffffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x40000U & 
                                        (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                         >> 0xdU)));
    this->__PVT__axi_comp_req[0U] = ((0xfffc3fffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x3c000U & 
                                        ((this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                          << 0x13U) 
                                         | (0x7c000U 
                                            & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                               >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xffffc7ffU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x3800U & (
                                                   (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                    << 0x13U) 
                                                   | (0x7f800U 
                                                      & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                         >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xfffff87fU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x780U & ((
                                                   this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                   << 0x13U) 
                                                  | (0x7ff80U 
                                                     & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                        >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xffffff87U & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0x78U & ((this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                                  << 0x13U) 
                                                 | (0x7fff8U 
                                                    & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                       >> 0xdU)))));
    this->__PVT__axi_comp_req[0U] = ((0xfffffffbU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (4U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                              >> 0xdU)));
    this->__PVT__axi_comp_req[0U] = ((0xfffffffdU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (0xfffffffeU 
                                        & ((IData)(this->__PVT__complete_read) 
                                           << 1U)));
    this->__PVT__axi_comp_req[3U] = ((0x3fU & this->__PVT__axi_comp_req[3U]) 
                                     | (0xffffffc0U 
                                        & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                           << 6U)));
    this->__PVT__axi_comp_req[4U] = ((0x3fU & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                               >> 0x1aU)) 
                                     | (0xffffffc0U 
                                        & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                           << 6U)));
    this->__PVT__axi_comp_req[5U] = ((0xffff0000U & 
                                      this->__PVT__axi_comp_req[5U]) 
                                     | ((0x3fU & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                  >> 0x1aU)) 
                                        | (0xffffffc0U 
                                           & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
                                              << 6U))));
    this->__PVT__axi_comp_req[3U] = ((0xffffffdfU & 
                                      this->__PVT__axi_comp_req[3U]) 
                                     | (0xffffffe0U 
                                        & (((0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                            & (0U != 
                                               (3U 
                                                & (IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q)))) 
                                           << 5U)));
    this->__PVT__axi_comp_req[0U] = ((0xfffffffeU & 
                                      this->__PVT__axi_comp_req[0U]) 
                                     | (1U & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
                                              >> 0x17U)));
    this->__PVT__axi_comp_req[3U] = ((0xffffffefU & 
                                      this->__PVT__axi_comp_req[3U]) 
                                     | (0x10U & (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] 
                                                 >> 6U)));
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__gate_clock = 0U;
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q)));
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
           & (IData)(this->__PVT__i_aw_stream_fifo__DOT__pop)) 
          & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[0U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[0U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[1U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[1U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[2U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[2U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[3U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[3U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[4U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[4U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[5U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[5U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[6U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[6U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[7U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[7U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[8U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[8U];
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[9U] 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[9U];
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[0U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x13U] 
                << 9U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x12U] 
                          >> 0x17U));
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[1U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x14U] 
                << 9U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x13U] 
                          >> 0x17U));
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[2U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x15U] 
                << 9U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x14U] 
                          >> 0x17U));
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[3U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x16U] 
                << 9U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x15U] 
                          >> 0x17U));
        this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[4U] 
            = (0x7fffffU & ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x17U] 
                             << 9U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x16U] 
                                       >> 0x17U)));
        if ((0x12dU >= (0x1ffU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))))) {
            VL_ASSIGNSEL_WIIW(151,(0x1ffU & ((IData)(0x97U) 
                                             * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))), this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n, this->i_aw_stream_fifo__DOT__fifo_i__DOT____Vlvbound1);
        }
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__gate_clock = 0U;
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q)));
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
           & (IData)(this->__PVT__i_ar_stream_fifo__DOT__pop)) 
          & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    }
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[0U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[0U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[1U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[1U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[2U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[2U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[3U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[3U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[4U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[4U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[5U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[5U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[6U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[6U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[7U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[7U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[8U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[8U];
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[9U] 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[9U];
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__push) 
         & (2U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[0U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xcU] 
                << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
                          >> 0x19U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[1U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xdU] 
                << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xcU] 
                          >> 0x19U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[2U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
                << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xdU] 
                          >> 0x19U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[3U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xfU] 
                << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
                          >> 0x19U));
        this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[4U] 
            = (0x1ffffU & ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] 
                            << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xfU] 
                                      >> 0x19U)));
        if ((0x121U >= (0x1ffU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))))) {
            VL_ASSIGNSEL_WIIW(145,(0x1ffU & ((IData)(0x91U) 
                                             * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))), this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n, this->i_ar_stream_fifo__DOT__fifo_i__DOT____Vlvbound1);
        }
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__gate_clock = 0U;
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q)));
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (7U & ((IData)(1U) + (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = (7U & ((IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
           & (IData)(this->__PVT__i_w_stream_fifo__DOT__pop)) 
          & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n 
            = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q;
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[0U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[0U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[1U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[1U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[2U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[2U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[3U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[3U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[4U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[4U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[5U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[5U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[6U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[6U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[7U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[7U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[8U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[8U];
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[9U] 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[9U];
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__push) 
         & (4U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[0U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x11U] 
                << 0x14U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] 
                             >> 0xcU));
        this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[1U] 
            = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x12U] 
                << 0x14U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x11U] 
                             >> 0xcU));
        this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1[2U] 
            = (0x3ffU & ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x13U] 
                          << 0x14U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__i_in_axi_demux__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x12U] 
                                       >> 0xcU)));
        if ((0x127U >= (0x1ffU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))))) {
            VL_ASSIGNSEL_WIIW(74,(0x1ffU & ((IData)(0x4aU) 
                                            * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q))), this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n, this->i_w_stream_fifo__DOT__fifo_i__DOT____Vlvbound1);
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[5U] >> 0x10U) 
           & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_chan));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[5U] >> 0x10U) 
           & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_sel));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[3U] >> 5U) & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_w_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[0U] >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_chan));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->__PVT__axi_comp_req[0U] >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_sel));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->__PVT__axi_comp_req[3U] >> 4U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->__PVT__axi_comp_req[3U] >> 4U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ this->__PVT__axi_comp_req[0U]));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & this->__PVT__axi_comp_req[0U]);
    this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_d 
        = this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q;
    if ((1U & (((this->__PVT__axi_comp_req[5U] >> 0x10U) 
                & (this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                   >> 0x13U)) ^ (IData)(this->__PVT__decr_w_cnt)))) {
        this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_d 
            = (7U & ((IData)(this->__PVT__decr_w_cnt)
                      ? ((IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q) 
                         - (IData)(1U)) : ((IData)(1U) 
                                           + (IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q))));
    }
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_sequent__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__10(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_sequent__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__10\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_n));
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n));
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_n));
    if (vlTOPp->rst_ni) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q 
            = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_n;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q 
            = ((IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_n) 
               & 1U);
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q 
            = ((IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_n) 
               & 1U);
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n) 
               & 1U);
    } else {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = 0U;
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__gate_clock)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q 
            = this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_d;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_n) 
               & 1U);
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_n) 
               & 1U);
    } else {
        this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__gate_clock)))) {
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[0U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[1U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[2U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[3U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[4U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[5U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[6U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[7U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[8U];
            this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] 
                = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_n[9U];
        }
    } else {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__gate_clock)))) {
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[0U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[1U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[2U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[3U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[4U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[5U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[6U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[7U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[8U];
            this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] 
                = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_n[9U];
        }
    } else {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] = 0U;
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__gate_clock)))) {
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[0U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[1U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[2U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[3U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[4U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[5U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[6U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[7U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[8U];
            this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] 
                = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_n[9U];
        }
    } else {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[0U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[1U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[2U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[3U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[4U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[5U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[6U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[7U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[8U] = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[9U] = 0U;
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d));
    if (vlTOPp->rst_ni) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d;
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__trans_req_data_q[0U] = this->__PVT__trans_req_data_n[0U];
        this->__PVT__trans_req_data_q[1U] = this->__PVT__trans_req_data_n[1U];
        this->__PVT__trans_req_data_q[2U] = this->__PVT__trans_req_data_n[2U];
        this->__PVT__trans_req_data_q[3U] = this->__PVT__trans_req_data_n[3U];
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d;
    } else {
        this->__PVT__trans_req_data_q[0U] = 0U;
        this->__PVT__trans_req_data_q[1U] = 0U;
        this->__PVT__trans_req_data_q[2U] = 0U;
        this->__PVT__trans_req_data_q[3U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_q 
                = this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_n));
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__gate_clock)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d)
            : 0U);
    if (vlTOPp->rst_ni) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__state_q = this->__PVT__state_n;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q 
            = (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes) 
                & (~ this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U])) 
               & 1U);
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q 
            = (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes) 
                & (~ (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                      >> 4U))) & 1U);
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n) 
               & 1U);
    } else {
        this->__PVT__state_q = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = 0U;
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__gate_clock)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n;
        }
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q 
            = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_n;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q 
            = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_n;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q 
            = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_n;
    } else {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q = 0U;
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = 0U;
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = 0U;
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U] = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U];
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill) 
             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain))) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q 
                = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill;
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q = 0U;
    }
    if ((0x121U >= (0x1ffU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))) {
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(1U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  (0xfU & (((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                           >> 5U))] >> (0x1fU & ((IData)(0x91U) 
                                                 * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(2U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(1U) + (0xfU & (((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(3U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(2U) + (0xfU & (((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] 
            = (((0U == (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(4U) + (0xfU & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(3U) + (0xfU & (((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x91U) * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
            = (0x1ffffU & (((0U == (0x1fU & ((IData)(0x91U) 
                                             * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                             ? 0U : (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                                     ((IData)(5U) + 
                                      (0xfU & (((IData)(0x91U) 
                                                * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                               >> 5U)))] 
                                     << ((IData)(0x20U) 
                                         - (0x1fU & 
                                            ((IData)(0x91U) 
                                             * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
                           | (this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__mem_q[
                              ((IData)(4U) + (0xfU 
                                              & (((IData)(0x91U) 
                                                  * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                              >> (0x1fU & ((IData)(0x91U) 
                                           * (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))));
    } else {
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] = 0U;
        this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] = 0U;
    }
    if ((0x12dU >= (0x1ffU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))) {
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(1U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  (0xfU & (((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                           >> 5U))] >> (0x1fU & ((IData)(0x97U) 
                                                 * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(2U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(1U) + (0xfU & (((IData)(0x97U) 
                                           * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(3U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(2U) + (0xfU & (((IData)(0x97U) 
                                           * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] 
            = (((0U == (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(4U) + (0xfU & (((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x97U) 
                                                  * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(3U) + (0xfU & (((IData)(0x97U) 
                                           * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x97U) * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] 
            = (0x7fffffU & (((0U == (0x1fU & ((IData)(0x97U) 
                                              * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                              ? 0U : (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                                      ((IData)(5U) 
                                       + (0xfU & (((IData)(0x97U) 
                                                   * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                  >> 5U)))] 
                                      << ((IData)(0x20U) 
                                          - (0x1fU 
                                             & ((IData)(0x97U) 
                                                * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
                            | (this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__mem_q[
                               ((IData)(4U) + (0xfU 
                                               & (((IData)(0x97U) 
                                                   * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                  >> 5U)))] 
                               >> (0x1fU & ((IData)(0x97U) 
                                            * (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))));
    } else {
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U] = 0U;
        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U] = 0U;
    }
    if ((0x127U >= (0x1ffU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))) {
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
            = (((0U == (0x1fU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(1U) + (0xfU & (((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  (0xfU & (((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                           >> 5U))] >> (0x1fU & ((IData)(0x4aU) 
                                                 * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
            = (((0U == (0x1fU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                 ? 0U : (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                         ((IData)(2U) + (0xfU & (((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                 >> 5U)))] 
                         << ((IData)(0x20U) - (0x1fU 
                                               & ((IData)(0x4aU) 
                                                  * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
               | (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                  ((IData)(1U) + (0xfU & (((IData)(0x4aU) 
                                           * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                          >> 5U)))] 
                  >> (0x1fU & ((IData)(0x4aU) * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))));
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] 
            = (0x3ffU & (((0U == (0x1fU & ((IData)(0x4aU) 
                                           * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))
                           ? 0U : (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                                   ((IData)(3U) + (0xfU 
                                                   & (((IData)(0x4aU) 
                                                       * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                                      >> 5U)))] 
                                   << ((IData)(0x20U) 
                                       - (0x1fU & ((IData)(0x4aU) 
                                                   * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)))))) 
                         | (this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__mem_q[
                            ((IData)(2U) + (0xfU & 
                                            (((IData)(0x4aU) 
                                              * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)) 
                                             >> 5U)))] 
                            >> (0x1fU & ((IData)(0x4aU) 
                                         * (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q))))));
    } else {
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] = 0U;
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] = 0U;
        this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U] = 0U;
    }
    this->__PVT__is_read = ((((1U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                << 8U) 
                                               | (this->__PVT__trans_req_data_q[0U] 
                                                  >> 0x18U)))) 
                              | (2U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                  << 8U) 
                                                 | (this->__PVT__trans_req_data_q[0U] 
                                                    >> 0x18U))))) 
                             | (6U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                 << 8U) 
                                                | (this->__PVT__trans_req_data_q[0U] 
                                                   >> 0x18U))))) 
                            | (6U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                << 8U) 
                                               | (this->__PVT__trans_req_data_q[0U] 
                                                  >> 0x18U)))));
    this->__PVT__is_write = ((3U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                               << 8U) 
                                              | (this->__PVT__trans_req_data_q[0U] 
                                                 >> 0x18U)))) 
                             | (7U == (0x3fU & ((this->__PVT__trans_req_data_q[1U] 
                                                 << 8U) 
                                                | (this->__PVT__trans_req_data_q[0U] 
                                                   >> 0x18U)))));
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q 
                = (1U & (~ (this->__PVT__trans_resp_data_q[5U] 
                            >> 0x17U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_load = 0U;
    if ((1U & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q)))) {
        if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_load = 1U;
        }
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q 
                = (1U & (~ (this->__PVT__trans_resp_data_q[5U] 
                            >> 0x17U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q = 0U;
    }
    this->trans_resp_ready_o = 0U;
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((2U & (IData)(this->__PVT__state_q))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                this->trans_resp_ready_o = 1U;
            }
        }
    }
    this->trans_req_valid_o = 0U;
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(this->__PVT__state_q))) {
                this->trans_req_valid_o = 1U;
            }
        }
    }
    this->i_iommu_axi_err_slv__DOT____Vcellout__i_r_fifo__data_o 
        = ((0xbU >= (0xfU & ((IData)(0xcU) * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q))))
            ? (0xfffU & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q) 
                         >> (0xfU & ((IData)(0xcU) 
                                     * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q)))))
            : 0U);
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xf01ffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = ((0xf0fffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]) 
           | (0xf000U & (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q) 
                          >> (7U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q) 
                                    << 2U))) << 0xcU)));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0x800U | (0xff3ffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xeffffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
            = (0x10000U | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = ((this->__PVT__axi_comp_req[1U] << 0x1eU) 
                   | (this->__PVT__axi_comp_req[0U] 
                      >> 2U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = ((this->__PVT__axi_comp_req[2U] << 0x1eU) 
                   | (this->__PVT__axi_comp_req[1U] 
                      >> 2U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = ((this->__PVT__axi_comp_req[3U] << 0x1eU) 
                   | (this->__PVT__axi_comp_req[2U] 
                      >> 2U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] 
                = (3U & ((this->__PVT__axi_comp_req[4U] 
                          << 0x1eU) | (this->__PVT__axi_comp_req[3U] 
                                       >> 2U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = ((this->__PVT__axi_comp_req[4U] << 0x1aU) 
                   | (this->__PVT__axi_comp_req[3U] 
                      >> 6U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = ((this->__PVT__axi_comp_req[5U] << 0x1aU) 
                   | (this->__PVT__axi_comp_req[4U] 
                      >> 6U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = (0x3ffU & ((this->__PVT__axi_comp_req[6U] 
                              << 0x1aU) | (this->__PVT__axi_comp_req[5U] 
                                           >> 6U)));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = ((this->__PVT__axi_comp_req[6U] << 0xfU) 
                   | (this->__PVT__axi_comp_req[5U] 
                      >> 0x11U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = ((this->__PVT__axi_comp_req[7U] << 0xfU) 
                   | (this->__PVT__axi_comp_req[6U] 
                      >> 0x11U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = ((this->__PVT__axi_comp_req[8U] << 0xfU) 
                   | (this->__PVT__axi_comp_req[7U] 
                      >> 0x11U));
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] 
                = (0xffU & (this->__PVT__axi_comp_req[8U] 
                            >> 0x11U));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U] = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = ((0x3ffffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]) 
           | (0xfffc0000U & (((1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
                              << 0x13U) | ((1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)) 
                                           << 0x12U))));
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] 
                = this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] 
                = this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[1U];
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] 
                = (0xffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U] = 0U;
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U] = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q 
                = (0x7fU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                            >> 9U));
        }
    } else {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q = 0U;
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_chan 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_w_spill_reg__ready_o 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_chan 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_r_spill_reg__ready_o 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xffeffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xffffff00U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                              | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                             << 8U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
           & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_b_spill_reg__ready_o 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q)) 
                 | (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xeffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xffff0000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                              | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                             << 0x10U)));
    this->__PVT__trans_req_data_n[0U] = this->__PVT__trans_req_data_q[0U];
    this->__PVT__trans_req_data_n[1U] = this->__PVT__trans_req_data_q[1U];
    this->__PVT__trans_req_data_n[2U] = this->__PVT__trans_req_data_q[2U];
    this->__PVT__trans_req_data_n[3U] = this->__PVT__trans_req_data_q[3U];
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if (((0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                     & (~ ((IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q) 
                           >> 2U)))) {
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0x3fffffffU & this->__PVT__trans_req_data_n[0U]) 
                           | (0xc0000000U & ((IData)(
                                                     (((QData)((IData)(
                                                                       this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                       << 0x2dU) 
                                                      | (((QData)((IData)(
                                                                          this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                          << 0xdU) 
                                                         | ((QData)((IData)(
                                                                            this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                            >> 0x13U)))) 
                                             << 0x1eU)));
                    this->__PVT__trans_req_data_n[1U] 
                        = ((0x3fffffffU & ((IData)(
                                                   (((QData)((IData)(
                                                                     this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                     << 0x2dU) 
                                                    | (((QData)((IData)(
                                                                        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                        << 0xdU) 
                                                       | ((QData)((IData)(
                                                                          this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                          >> 0x13U)))) 
                                           >> 2U)) 
                           | (0xc0000000U & ((IData)(
                                                     ((((QData)((IData)(
                                                                        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                        << 0x2dU) 
                                                       | (((QData)((IData)(
                                                                           this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                           << 0xdU) 
                                                          | ((QData)((IData)(
                                                                             this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                             >> 0x13U))) 
                                                      >> 0x20U)) 
                                             << 0x1eU)));
                    this->__PVT__trans_req_data_n[2U] 
                        = ((0xc0000000U & this->__PVT__trans_req_data_n[2U]) 
                           | (0x3fffffffU & ((IData)(
                                                     ((((QData)((IData)(
                                                                        this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                        << 0x2dU) 
                                                       | (((QData)((IData)(
                                                                           this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                           << 0xdU) 
                                                          | ((QData)((IData)(
                                                                             this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                             >> 0x13U))) 
                                                      >> 0x20U)) 
                                             >> 2U)));
                    this->__PVT__trans_req_data_n[2U] 
                        = ((0x3fffffffU & this->__PVT__trans_req_data_n[2U]) 
                           | (0xc0000000U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                             << 7U)));
                    this->__PVT__trans_req_data_n[3U] 
                        = (0x3fffffU & ((0x3fffff80U 
                                         & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                            << 7U)) 
                                        | (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                           >> 0x19U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xfffffff7U & this->__PVT__trans_req_data_n[0U]) 
                           | (8U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                    >> 0x13U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xff00000fU & this->__PVT__trans_req_data_n[0U]) 
                           | (0xfffff0U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                           << 2U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xc0ffffffU & this->__PVT__trans_req_data_n[0U]) 
                           | (0xff000000U & (((1U & 
                                               ((this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                                 >> 1U) 
                                                & (~ 
                                                   this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U])))
                                               ? 7U
                                               : 3U) 
                                             << 0x18U)));
                    this->__PVT__trans_req_data_n[0U] 
                        = ((0xfffffffbU & this->__PVT__trans_req_data_n[0U]) 
                           | (4U & (this->i_aw_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                    >> 0x1cU)));
                    this->__PVT__trans_req_data_n[0U] 
                        = (0xfffffffdU & this->__PVT__trans_req_data_n[0U]);
                } else {
                    if ((0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) {
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0x3fffffffU & this->__PVT__trans_req_data_n[0U]) 
                               | (0xc0000000U & ((IData)(
                                                         (((QData)((IData)(
                                                                           this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                           << 0x33U) 
                                                          | (((QData)((IData)(
                                                                              this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                              << 0x13U) 
                                                             | ((QData)((IData)(
                                                                                this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                                >> 0xdU)))) 
                                                 << 0x1eU)));
                        this->__PVT__trans_req_data_n[1U] 
                            = ((0x3fffffffU & ((IData)(
                                                       (((QData)((IData)(
                                                                         this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                         << 0x33U) 
                                                        | (((QData)((IData)(
                                                                            this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                            << 0x13U) 
                                                           | ((QData)((IData)(
                                                                              this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                              >> 0xdU)))) 
                                               >> 2U)) 
                               | (0xc0000000U & ((IData)(
                                                         ((((QData)((IData)(
                                                                            this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                            << 0x33U) 
                                                           | (((QData)((IData)(
                                                                               this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                               << 0x13U) 
                                                              | ((QData)((IData)(
                                                                                this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                                 >> 0xdU))) 
                                                          >> 0x20U)) 
                                                 << 0x1eU)));
                        this->__PVT__trans_req_data_n[2U] 
                            = ((0xc0000000U & this->__PVT__trans_req_data_n[2U]) 
                               | (0x3fffffffU & ((IData)(
                                                         ((((QData)((IData)(
                                                                            this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[4U])) 
                                                            << 0x33U) 
                                                           | (((QData)((IData)(
                                                                               this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[3U])) 
                                                               << 0x13U) 
                                                              | ((QData)((IData)(
                                                                                this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[2U])) 
                                                                 >> 0xdU))) 
                                                          >> 0x20U)) 
                                                 >> 2U)));
                        this->__PVT__trans_req_data_n[2U] 
                            = ((0x3fffffffU & this->__PVT__trans_req_data_n[2U]) 
                               | (0xc0000000U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                                 << 7U)));
                        this->__PVT__trans_req_data_n[3U] 
                            = (0x3fffffU & ((0x3fffff80U 
                                             & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                                << 7U)) 
                                            | (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                               >> 0x19U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xfffffff7U & this->__PVT__trans_req_data_n[0U]) 
                               | (8U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                        >> 0x13U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xff00000fU & this->__PVT__trans_req_data_n[0U]) 
                               | (0xfffff0U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                               << 2U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xc0ffffffU & this->__PVT__trans_req_data_n[0U]) 
                               | (0xff000000U & (((1U 
                                                   & ((this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                                       >> 1U) 
                                                      & (~ 
                                                         this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U])))
                                                   ? 
                                                  ((0x4000000U 
                                                    & this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U])
                                                    ? 5U
                                                    : 6U)
                                                   : 
                                                  ((0x4000000U 
                                                    & this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U])
                                                    ? 1U
                                                    : 2U)) 
                                                 << 0x18U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = ((0xfffffffbU & this->__PVT__trans_req_data_n[0U]) 
                               | (4U & (this->i_ar_stream_fifo__DOT____Vcellout__fifo_i__data_o[1U] 
                                        >> 0x16U)));
                        this->__PVT__trans_req_data_n[0U] 
                            = (0xfffffffdU & this->__PVT__trans_req_data_n[0U]);
                    }
                }
            }
        }
    }
    this->__PVT__complete_read = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                this->__PVT__complete_read = this->__PVT__is_read;
            }
        }
    }
    this->__PVT__complete_write = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                this->__PVT__complete_write = this->__PVT__is_write;
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] = 0U;
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[1U] = 0U;
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xfff00U & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = ((0xfff0fU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]) 
           | (0xf0U & ((IData)(this->i_iommu_axi_err_slv__DOT____Vcellout__i_r_fifo__data_o) 
                       >> 4U)));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] 
        = (0xadcab1e0U | (0xfU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[1U] = 0xa11ab1ebU;
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xcU | (0xffff0U & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] 
        = (8U | (0xfffffff3U & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U]));
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xffeffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
            = (0x100U | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U] 
            = ((0xfffffffdU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U]) 
               | (0xfffffffeU & ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q))) 
                                 << 1U)));
    }
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xdffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfffe0000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_w_spill_reg__ready_o) 
                             << 0x11U)));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xbffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfffc0000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_chan) 
                              & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_ar_ready_sel)) 
                             << 0x12U)));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0x7ffffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfff80000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_chan) 
                              & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_aw_ready_sel)) 
                             << 0x13U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
        = ((0xfffffffeU & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U]) 
           | (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_r_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
        = ((0xffffffefU & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U]) 
           | (0xfffffff0U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_b_spill_reg__ready_o) 
                             << 4U)));
    if (vlTOPp->rst_ni) {
        this->__PVT__trans_resp_data_q[0U] = this->__PVT__trans_resp_data_n[0U];
        this->__PVT__trans_resp_data_q[1U] = this->__PVT__trans_resp_data_n[1U];
        this->__PVT__trans_resp_data_q[2U] = this->__PVT__trans_resp_data_n[2U];
        this->__PVT__trans_resp_data_q[3U] = this->__PVT__trans_resp_data_n[3U];
        this->__PVT__trans_resp_data_q[4U] = this->__PVT__trans_resp_data_n[4U];
        this->__PVT__trans_resp_data_q[5U] = this->__PVT__trans_resp_data_n[5U];
    } else {
        this->__PVT__trans_resp_data_q[0U] = 0U;
        this->__PVT__trans_resp_data_q[1U] = 0U;
        this->__PVT__trans_resp_data_q[2U] = 0U;
        this->__PVT__trans_resp_data_q[3U] = 0U;
        this->__PVT__trans_resp_data_q[4U] = 0U;
        this->__PVT__trans_resp_data_q[5U] = 0U;
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
        = ((1U & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U]) 
           | (0xfffffffeU & ((0xfffffffcU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                             << 2U)) 
                             | ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                  | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                 & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                    | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                << 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
        = ((1U & ((1U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                           ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                           : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                         >> 0x1eU)) | ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                         | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                        & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                           | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                       >> 0x1fU))) 
           | (0xfffffffeU & ((2U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                      ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                      : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                    >> 0x1eU)) | (0xfffffffcU 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                     << 2U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
        = ((1U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                    ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                    : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                  >> 0x1eU)) | (0xfffffffeU & ((2U 
                                                & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                     ? 
                                                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                     : 
                                                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                   >> 0x1eU)) 
                                               | (0xfffffffcU 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                     << 2U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
        = ((0xfffffff0U & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U]) 
           | ((1U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                     >> 0x1eU)) | (0xfffffffeU & ((2U 
                                                   & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                        ? 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                        : 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                      >> 0x1eU)) 
                                                  | (0xfffffffcU 
                                                     & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                          ? 
                                                         this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U]
                                                          : 
                                                         this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U]) 
                                                        << 2U))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
        = ((0x1fU & this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U]) 
           | (0xffffffe0U & ((0xffffffc0U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                             << 6U)) 
                             | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                 | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                << 5U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
        = ((0x1fU & ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                 ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                 : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                               >> 0x1aU)) | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                              | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                             >> 0x1bU))) 
           | (0xffffffe0U & ((0x20U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                         ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                         : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                       >> 0x1aU)) | 
                             (0xffffffc0U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                             << 6U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
        = ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                     >> 0x1aU)) | (0xffffffe0U & ((0xfffe0000U 
                                                   & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                        ? 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                                        : 
                                                       this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                                      << 0x11U)) 
                                                  | ((0xffff0000U 
                                                      & ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                                           | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                                          & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                                             | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                                         << 0x10U)) 
                                                     | ((0x20U 
                                                         & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                              ? 
                                                             this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                              : 
                                                             this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                            >> 0x1aU)) 
                                                        | (0xffffffc0U 
                                                           & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                                ? 
                                                               this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                                : 
                                                               this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                              << 6U)))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
        = ((0x1fU & ((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                        ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                        : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                      >> 0xfU) | (((((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                     | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)) 
                                    & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_full_q) 
                                       | (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q))) 
                                   >> 0x10U) | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                  ? 
                                                 this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                  : 
                                                 this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                >> 0x1aU)))) 
           | (0xffffffe0U & ((0x1ffe0U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                            ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U]
                                            : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U]) 
                                          >> 0xfU)) 
                             | (0xfffe0000U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                 ? 
                                                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                 : 
                                                this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                               << 0x11U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
        = ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                     >> 0xfU)) | (0xffffffe0U & ((0x1ffe0U 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U]) 
                                                     >> 0xfU)) 
                                                 | (0xfffe0000U 
                                                    & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                         ? 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                         : 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                       << 0x11U)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
        = ((0x1fU & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                       ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                       : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                     >> 0xfU)) | (0xffffffe0U & ((0x1ffe0U 
                                                  & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                       ? 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
                                                       : 
                                                      this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]) 
                                                     >> 0xfU)) 
                                                 | (0xfffe0000U 
                                                    & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                                                         ? 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[3U]
                                                         : 
                                                        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[3U]) 
                                                       << 0x11U)))));
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) {
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[0U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[0U];
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[1U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[1U];
    } else {
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[0U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[0U];
        this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[1U] 
            = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[1U];
    }
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xfff00U & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
               ? this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q[2U]
               : this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q[2U]));
    this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
        = ((0xf01ffU & this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U]) 
           | (0xfffffe00U & (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q)
                               ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_data_q)
                               : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_data_q)) 
                             << 9U)));
    this->__PVT__ar_ready = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                       >> 0x12U) & (IData)(this->__PVT__is_read)) 
                     | ((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                         >> 0x13U) & (IData)(this->__PVT__is_write)))) {
                    this->__PVT__ar_ready = this->__PVT__is_read;
                }
            }
        }
    }
    this->__PVT__aw_ready = 0U;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((1U & (~ ((IData)(this->__PVT__state_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                       >> 0x12U) & (IData)(this->__PVT__is_read)) 
                     | ((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                         >> 0x13U) & (IData)(this->__PVT__is_write)))) {
                    this->__PVT__aw_ready = this->__PVT__is_write;
                }
            }
        }
    }
    this->__PVT__w_ready = ((0U != (3U & (IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q))) 
                            & (this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                               >> 0x11U));
    this->tr_axi_resp_o[0U] = this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[0U];
    this->tr_axi_resp_o[1U] = this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[1U];
    this->tr_axi_resp_o[2U] = this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U];
    this->tr_axi_resp_o[2U] = ((0x7ffffU & this->tr_axi_resp_o[2U]) 
                               | (0xfff80000U & ((2U 
                                                  != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                                 << 0x13U)));
    this->tr_axi_resp_o[2U] = ((0xbffffU & this->tr_axi_resp_o[2U]) 
                               | (0xfffc0000U & ((2U 
                                                  != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                                 << 0x12U)));
    this->tr_axi_resp_o[2U] = ((0xdffffU & this->tr_axi_resp_o[2U]) 
                               | (0xfffe0000U & ((4U 
                                                  != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                                 << 0x11U)));
    this->__PVT__trans_resp_data_n[0U] = this->__PVT__trans_resp_data_q[0U];
    this->__PVT__trans_resp_data_n[1U] = this->__PVT__trans_resp_data_q[1U];
    this->__PVT__trans_resp_data_n[2U] = this->__PVT__trans_resp_data_q[2U];
    this->__PVT__trans_resp_data_n[3U] = this->__PVT__trans_resp_data_q[3U];
    this->__PVT__trans_resp_data_n[4U] = this->__PVT__trans_resp_data_q[4U];
    this->__PVT__trans_resp_data_n[5U] = this->__PVT__trans_resp_data_q[5U];
    if ((1U & (~ ((IData)(this->__PVT__state_q) >> 2U)))) {
        if ((2U & (IData)(this->__PVT__state_q))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((2U & (IData)(vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_valid))) {
                    this->__PVT__trans_resp_data_n[0U] 
                        = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[6U] 
                            << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[5U] 
                                      >> 0x19U));
                    this->__PVT__trans_resp_data_n[1U] 
                        = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[7U] 
                            << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[6U] 
                                      >> 0x19U));
                    this->__PVT__trans_resp_data_n[2U] 
                        = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[8U] 
                            << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[7U] 
                                      >> 0x19U));
                    this->__PVT__trans_resp_data_n[3U] 
                        = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[9U] 
                            << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[8U] 
                                      >> 0x19U));
                    this->__PVT__trans_resp_data_n[4U] 
                        = ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[0xaU] 
                            << 7U) | (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[9U] 
                                      >> 0x19U));
                    this->__PVT__trans_resp_data_n[5U] 
                        = (0x1ffffffU & ((vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[0xbU] 
                                          << 7U) | 
                                         (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[0xaU] 
                                          >> 0x19U)));
                }
            }
        }
    }
    this->__PVT__i_ar_stream_fifo__DOT__pop = ((IData)(this->__PVT__ar_ready) 
                                               & (0U 
                                                  != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_aw_stream_fifo__DOT__pop = ((IData)(this->__PVT__aw_ready) 
                                               & (0U 
                                                  != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__decr_w_cnt = (((0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                                & (IData)(this->__PVT__w_ready)) 
                               & (this->i_w_stream_fifo__DOT____Vcellout__fifo_i__data_o[0U] 
                                  >> 1U));
    this->__PVT__i_w_stream_fifo__DOT__pop = ((IData)(this->__PVT__w_ready) 
                                              & (0U 
                                                 != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)));
    this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
        = this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_ar_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)));
    }
    this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
        = this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_aw_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)));
    }
    this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
        = this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_w_stream_fifo__DOT__pop) 
         & (0U != (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)))) {
        this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_w_stream_fifo__DOT__fifo_i__DOT__read_pointer_q)));
    }
}

VL_INLINE_OPT void Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_combo__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__16(Vnonblock_banked_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vnonblock_banked_top_rv_iommu_trans_ctl_wrap__pi4::_combo__TOP__nonblock_banked_top__DOT__i_banked__DOT__gen_bank__BRA__1__KET____DOT__i_bank__16\n"); );
    Vnonblock_banked_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_en = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_en = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_clear = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_clear = 1U;
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop = 1U;
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d 
        = this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d = 0U;
            }
        }
    } else {
        if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_d = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load = 0U;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_q) {
        if ((1U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U])) {
            if ((0U == (0xffU & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q)))) {
                this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load = 1U;
            }
        }
    } else {
        if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__r_busy_load = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
            >> 1U) & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)));
    this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop = 0U;
    if ((0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop 
            = (1U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                     >> 4U));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
            >> 0x10U) & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)));
    this->__PVT__state_n = this->__PVT__state_q;
    if ((4U & (IData)(this->__PVT__state_q))) {
        if ((2U & (IData)(this->__PVT__state_q))) {
            this->__PVT__state_n = 0U;
        } else {
            if ((1U & (IData)(this->__PVT__state_q))) {
                this->__PVT__state_n = 0U;
            } else {
                if ((((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                       >> 0x12U) & (IData)(this->__PVT__is_read)) 
                     | ((this->__Vcellout__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__slv_resp_o[2U] 
                         >> 0x13U) & (IData)(this->__PVT__is_write)))) {
                    this->__PVT__state_n = 0U;
                }
            }
        }
    } else {
        if ((2U & (IData)(this->__PVT__state_q))) {
            if ((1U & (~ (IData)(this->__PVT__state_q)))) {
                if ((2U & (IData)(vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_valid))) {
                    this->__PVT__state_n = ((1U & (
                                                   (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[9U] 
                                                    >> 0x16U) 
                                                   & (~ 
                                                      (vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_resp_data[0xbU] 
                                                       >> 0x10U))))
                                             ? 3U : 4U);
                }
            }
        } else {
            if ((1U & (IData)(this->__PVT__state_q))) {
                if ((2U & (IData)(vlTOPp->nonblock_banked_top__DOT__i_banked__DOT__bank_trans_req_ready))) {
                    this->__PVT__state_n = 2U;
                }
            } else {
                if (((0U != (IData)(this->__PVT__i_aw_stream_fifo__DOT__fifo_i__DOT__status_cnt_q)) 
                     & (~ ((IData)(this->__PVT__i_w_counter__DOT__i_counter__DOT__counter_q) 
                           >> 2U)))) {
                    this->__PVT__state_n = 1U;
                } else {
                    if ((0U != (IData)(this->__PVT__i_ar_stream_fifo__DOT__fifo_i__DOT__status_cnt_q))) {
                        this->__PVT__state_n = 1U;
                    }
                }
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q;
    if (this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_clear) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d = 0U;
    } else {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_load) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d 
                = (0xffU & (IData)(this->i_iommu_axi_err_slv__DOT____Vcellout__i_r_fifo__data_o));
        } else {
            if (this->__PVT__i_iommu_axi_err_slv__DOT__r_cnt_en) {
                this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_d 
                    = (0x1ffU & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_counter__DOT__i_counter__DOT__counter_q) 
                                 - (IData)(1U)));
            }
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_n) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__read_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__gate_clock = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
           & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_pop)) 
          & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_n 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__status_cnt_q)))) {
        this->i_iommu_axi_err_slv__DOT__i_r_fifo__DOT____Vlvbound1 
            = this->__PVT__i_iommu_axi_err_slv__DOT__r_fifo_inp;
        if ((0xbU >= (0xfU & ((IData)(0xcU) * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q))))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n 
                = (((~ ((IData)(0xfffU) << (0xfU & 
                                            ((IData)(0xcU) 
                                             * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q))))) 
                    & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__mem_n)) 
                   | ((IData)(this->i_iommu_axi_err_slv__DOT__i_r_fifo__DOT____Vlvbound1) 
                      << (0xfU & ((IData)(0xcU) * (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_r_fifo__DOT__write_pointer_q)))));
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__read_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__gate_clock = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n 
            = (((~ ((IData)(0xfU) << (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q) 
                                            << 2U)))) 
                & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_n)) 
               | ((0xfU & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] 
                            << 0xbU) | (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
                                        >> 0x15U))) 
                  << (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q) 
                            << 2U))));
    }
    this->i_iommu_axi_err_slv__DOT____Vcellout__i_w_fifo__data_o 
        = (0xfU & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__mem_q) 
                   >> (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q) 
                             << 2U))));
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        this->i_iommu_axi_err_slv__DOT____Vcellout__i_w_fifo__data_o 
            = (0xfU & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] 
                        << 0xbU) | (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
                                    >> 0x15U)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty 
        = ((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
           & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push)));
    this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop = 0U;
    if (((~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        if ((1U & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                    >> 5U) & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                              >> 7U)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push = 0U;
    if (((~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        if ((1U & ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                    >> 5U) & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
                              >> 7U)))) {
            this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push = 1U;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
        = (0xdffffU & this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    if (((~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U] 
            = (0x20000U | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q)));
    }
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_n 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__write_pointer_q;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
         & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) 
         & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push) 
           & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop)) 
          & (1U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q))) 
         & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q;
    }
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_n 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) 
         & (~ (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_empty)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n 
            = ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n) 
               & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q)));
    }
    if (((0U == (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__status_cnt_q)) 
         & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_push))) {
        if (this->__PVT__i_iommu_axi_err_slv__DOT__w_fifo_pop) {
            this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_n 
                = this->__PVT__i_iommu_axi_err_slv__DOT__i_w_fifo__DOT__read_pointer_q;
        }
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__gate_clock = 1U;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__gate_clock = 0U;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_n 
            = (1U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q)));
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(1U) + (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)));
    }
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
            = (3U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q) 
                     - (IData)(1U)));
    }
    if (((((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
           & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_pop)) 
          & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q))) 
         & (0U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_n 
            = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q;
    }
    this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n 
        = this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_q;
    if (((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__b_fifo_push) 
         & (2U != (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__status_cnt_q)))) {
        this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n 
            = (((~ ((IData)(0xfU) << (7U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q) 
                                            << 2U)))) 
                & (IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__mem_n)) 
               | ((IData)(this->i_iommu_axi_err_slv__DOT____Vcellout__i_w_fifo__data_o) 
                  << (7U & ((IData)(this->__PVT__i_iommu_axi_err_slv__DOT__i_b_fifo__DOT__write_pointer_q) 
                            << 2U))));
    }
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[0U] 
        = this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[0U];
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[1U] 
        = this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[1U];
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
        = ((0xfff00000U & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[2U]) 
           | this->__PVT__i_iommu_axi_err_slv__DOT__err_resp[2U]);
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[3U] 
        = ((0xfffffU & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[3U]) 
           | (0xfff00000U & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[3U]));
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[4U] 
        = ((0xfffffU & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[4U]) 
           | (0xfff00000U & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[4U]));
    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U] 
        = (0xffU & vlTOPp->nonblock_banked_top__DOT__i_banked__DOT____Vcellout__i_out_axi_mux__slv_resps_o[5U]);
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down = 0U;
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select)))) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down 
            = (1U & (((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                       >> 5U) & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
                                 >> 0x11U)) & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                                               >> 7U)));
    }
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select))) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down 
            = (1U & (((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                       & this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U]) 
                      >> 5U) & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                                >> 7U)));
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = (0xdffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xdffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0x20000U & this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U]));
    }
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xdffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0x20000U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U] 
                              << 0xcU)));
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid) 
           & ((0xa7U >= (0xffU & ((IData)(0x52U) + 
                                  ((IData)(0x54U) * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))))) 
              & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                 (7U & (((IData)(0x52U) + ((IData)(0x54U) 
                                           * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) 
                        >> 5U))] >> (0x1fU & ((IData)(0x52U) 
                                              + ((IData)(0x54U) 
                                                 * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid) 
           & ((0xa7U >= (0xffU & ((IData)(0x53U) + 
                                  ((IData)(0x54U) * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))))) 
              & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                 (7U & (((IData)(0x53U) + ((IData)(0x54U) 
                                           * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) 
                        >> 5U))] >> (0x1fU & ((IData)(0x53U) 
                                              + ((IData)(0x54U) 
                                                 * (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids)) 
           | (1U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
                    >> 0x10U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids)) 
           | (1U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[2U] 
                    >> 8U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids)) 
           | (2U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[5U] 
                    >> 3U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids)) 
           | (2U & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[4U] 
                    >> 0x1bU)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d 
        = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q;
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_up) 
         ^ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down))) {
        this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_d 
            = (0xfU & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_cnt_down)
                        ? ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q) 
                           - (IData)(1U)) : ((IData)(1U) 
                                             + (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))));
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d 
        = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d = 0U;
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_d = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock = 1U;
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_ar_lock = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push = 1U;
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_push = 1U;
                }
            }
        }
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = (0xbffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_ar_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
            this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                = (0x40000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
        }
    } else {
        if ((1U & (~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)))))) {
            if ((1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                        >> 1U) & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                                  | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                                     == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__lookup_mst_select_o)))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_ready) {
                    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                        = (0x40000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d 
        = this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d = 0U;
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_d = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock = 1U;
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if ((1U & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready)))) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__load_aw_lock = 1U;
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject = 0U;
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject 
                = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                         >> 0x17U));
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
                    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__atop_inject 
                        = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                                 >> 0x17U));
                }
            }
        }
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = (0x7ffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__lock_aw_valid_q) {
        if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
            this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                = (0x80000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
        }
    } else {
        if ((((~ (IData)((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__cnt_full)))) 
              & (7U != (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q)))) 
             & (~ ((0U != (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter->__PVT__cnt_full)) 
                   & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                      >> 0x17U))))) {
            if ((((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                   >> 0x10U) & ((0U == (7U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_counter_open_w__DOT__i_counter__DOT__counter_q))) 
                                | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select) 
                                   == (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) 
                 & ((~ (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_occupied_o)) 
                    | ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o) 
                       == (IData)(__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter->__PVT__lookup_mst_select_o))))) {
                if (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_ready) {
                    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                        = (0x80000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
                }
            }
        }
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q)
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q)
            : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_valids));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q)
            ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q)
            : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_valids));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffffeU & (((1U <= (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffffeU & (((1U > (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d)) 
                 | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d) 
                     >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d) 
                 | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_d) 
                    >> 1U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffffeU & (((1U <= (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffffeU & (((1U > (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel 
        = (1U & ((~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d)) 
                 | (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d) 
                     >> 1U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d) 
                 | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_d) 
                    >> 1U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies)) 
           | (1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                     >> 4U) & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies)) 
           | (0x1ffffffeU & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                              >> 3U) & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel) 
                                        << 1U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = ((0xeffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
           | (0xffff0000U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes) 
                             << 0x10U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies)) 
           | (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                    & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies 
        = ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies)) 
           | ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
               & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)) 
              << 1U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
        = ((0xffeffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
           | (0xffffff00U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes) 
                             << 8U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                    | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                       >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                    | ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                       >> 1U))));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((2U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (1U & ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp))
                     ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)
                     : ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                        >> 1U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[2U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[7U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xaU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xcU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xdU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xfU] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x11U] = 0U;
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
        = (0xffU & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                    >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
        = ((0x1ffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]) 
           | (0xfffe0000U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                             << 0x11U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[6U] 
        = ((0x1ffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                        >> 0xfU)) | (0xfffe0000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                                      << 0x11U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[7U] 
        = ((0x1ffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                        >> 0xfU)) | (0xfffe0000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                                      << 0x11U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = ((0xfe000000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]) 
           | ((0x1ffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                           >> 0xfU)) | (0xfffe0000U 
                                        & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
                                           << 0x11U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
        = (0xfffeffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid) 
         & (~ (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
            = (0x10000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
        = (0x3ffU & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                      << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                                   >> 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = ((0x3fU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
           | (0xffffffc0U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                             << 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[4U] 
        = ((0x3fU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                     >> 0x1aU)) | (0xffffffc0U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                                                  << 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U] 
        = ((0xffff0000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[5U]) 
           | ((0x3fU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                        >> 0x1aU)) | (0xffffffc0U & 
                                      (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
                                       << 6U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = (0xffffffdfU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (~ (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6 
            = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                     >> 5U));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
            = ((0xffffffdfU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
               | (0xffffffe0U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6) 
                                 << 5U)));
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7 
        = (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = ((0xffffffefU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
           | (0xfffffff0U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7) 
                             << 4U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
        = (3U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                  << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                               >> 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
        = ((3U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]) 
           | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                             << 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[1U] 
        = ((3U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                  >> 0x1eU)) | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                                               << 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[2U] 
        = ((3U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                  >> 0x1eU)) | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                                               << 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U] 
        = ((0xfffffff0U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[3U]) 
           | ((3U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                     >> 0x1eU)) | (0xfffffffcU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
                                                  << 2U))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
        = (0xfffffffdU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid) 
         & (~ (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o)))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
            = (2U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11 
        = (1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U] 
        = ((0xfffffffeU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0U]) 
           | (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
            << 0xfU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[7U] 
                        >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
        = (0xffU & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[8U] 
                    >> 0x11U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
        = ((0x3ffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]) 
           | (0xfffffc00U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                             << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xfU] 
        = ((0x3ffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[0U] 
                      >> 0x16U)) | (0xfffffc00U & (
                                                   this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                                                   << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x10U] 
        = ((0x3ffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[1U] 
                      >> 0x16U)) | (0xfffffc00U & (
                                                   this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                                                   << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0x11U] 
        = ((0x3ffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[2U] 
                      >> 0x16U)) | (0xfffffc00U & (
                                                   this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound1[3U] 
                                                   << 0xaU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
        = (0xfffffdffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__aw_valid) 
         & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
            = (0x200U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
            << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                         >> 6U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
        = (0x3ffU & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[6U] 
                      << 0x1aU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[5U] 
                                   >> 6U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = ((0x7fffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
           | (0x80000000U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                             << 0x1fU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xcU] 
        = ((0x7fffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[0U] 
                           >> 1U)) | (0x80000000U & 
                                      (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                                       << 0x1fU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xdU] 
        = ((0x7fffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[1U] 
                           >> 1U)) | (0x80000000U & 
                                      (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
                                       << 0x1fU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU] 
        = ((0xfffffe00U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xeU]) 
           | (0x7fffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound4[2U] 
                             >> 1U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = (0xbfffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select_valid) 
         & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__w_select))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6 
            = (1U & (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                     >> 5U));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
            = ((0xbfffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
               | (0xc0000000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound6) 
                                 << 0x1eU)));
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies) 
                 >> 1U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = ((0xdfffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
           | (0xe0000000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound7) 
                             << 0x1dU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[1U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
        = ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
            << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[2U] 
                         >> 2U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
        = (3U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[4U] 
                  << 0x1eU) | (this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                               >> 2U)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = ((0x7ffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]) 
           | (0xf8000000U & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                             << 0x1bU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[9U] 
        = ((0x7ffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[0U] 
                          >> 5U)) | (0xf8000000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                                      << 0x1bU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xaU] 
        = ((0x7ffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[1U] 
                          >> 5U)) | (0xf8000000U & 
                                     (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                                      << 0x1bU)));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU] 
        = ((0xe0000000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[0xbU]) 
           | ((0x7ffffffU & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[2U] 
                             >> 5U)) | (0xf8000000U 
                                        & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound8[3U] 
                                           << 0x1bU))));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = (0xfbffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]);
    if (((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__ar_valid) 
         & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT____Vcellout__spill_register_flushable_i__data_o))) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
            = (0x4000000U | this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]);
    }
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11 
        = (1U & ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies) 
                 >> 1U));
    this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U] 
        = ((0xfdffffffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__mst_reqs_o[8U]) 
           | (0xfe000000U & ((IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT____Vlvbound11) 
                             << 0x19U)));
    if ((0x10000U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U])) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xf0fffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffff000U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x4cU) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (0xfU & (((0U 
                                                == 
                                                (0x1fU 
                                                 & ((IData)(0x4cU) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                ? 0U
                                                : (
                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                   ((IData)(1U) 
                                                    + 
                                                    (7U 
                                                     & (((IData)(0x4cU) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                        >> 5U)))] 
                                                   << 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(0x4cU) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                              | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                 (7U 
                                                  & (((IData)(0x4cU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U))] 
                                                 >> 
                                                 (0x1fU 
                                                  & ((IData)(0x4cU) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 0xcU)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xff3ffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffffc00U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x4aU) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (3U & (((0U == 
                                              (0x1fU 
                                               & ((IData)(0x4aU) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                              ? 0U : 
                                             (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                              ((IData)(1U) 
                                               + (7U 
                                                  & (((IData)(0x4aU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U)))] 
                                              << ((IData)(0x20U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(0x4aU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                            | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                               (7U 
                                                & (((IData)(0x4aU) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                   >> 5U))] 
                                               >> (0x1fU 
                                                   & ((IData)(0x4aU) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 0xaU)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xffdffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffffe00U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x49U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))) 
                                  & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                     (7U & (((IData)(0x49U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                            >> 5U))] 
                                     >> (0x1fU & ((IData)(0x49U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))) 
                                 << 9U)));
    } else {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = (0xf01ffU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    }
    if ((0x100U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U])) {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xfff0fU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfffffff0U & (((0xa7U >= (0xffU 
                                             & ((IData)(0x44U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (0xfU & (((0U 
                                                == 
                                                (0x1fU 
                                                 & ((IData)(0x44U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                ? 0U
                                                : (
                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                   ((IData)(1U) 
                                                    + 
                                                    (7U 
                                                     & (((IData)(0x44U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                        >> 5U)))] 
                                                   << 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(0x44U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                              | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                 (7U 
                                                  & (((IData)(0x44U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U))] 
                                                 >> 
                                                 (0x1fU 
                                                  & ((IData)(0x44U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 4U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | (0xfffffff0U & ((IData)(((0xa7U >= 
                                           (0xffU & 
                                            ((IData)(4U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                           ? (((0U 
                                                == 
                                                (0x1fU 
                                                 & ((IData)(4U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                ? 0ULL
                                                : ((QData)((IData)(
                                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                   ((IData)(2U) 
                                                                    + 
                                                                    (7U 
                                                                     & (((IData)(4U) 
                                                                         + 
                                                                         ((IData)(0x54U) 
                                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                        >> 5U)))])) 
                                                   << 
                                                   ((IData)(0x40U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                              | (((QData)((IData)(
                                                                  this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                  ((IData)(1U) 
                                                                   + 
                                                                   (7U 
                                                                    & (((IData)(4U) 
                                                                        + 
                                                                        ((IData)(0x54U) 
                                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                       >> 5U)))])) 
                                                  << 
                                                  ((0U 
                                                    == 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                    ? 0x20U
                                                    : 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                                 | ((QData)((IData)(
                                                                    this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                    (7U 
                                                                     & (((IData)(4U) 
                                                                         + 
                                                                         ((IData)(0x54U) 
                                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                        >> 5U))])) 
                                                    >> 
                                                    (0x1fU 
                                                     & ((IData)(4U) 
                                                        + 
                                                        ((IData)(0x54U) 
                                                         * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                           : 0ULL)) 
                                 << 4U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[1U] 
            = ((0xfU & ((IData)(((0xa7U >= (0xffU & 
                                            ((IData)(4U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                  ? (((0U == (0x1fU 
                                              & ((IData)(4U) 
                                                 + 
                                                 ((IData)(0x54U) 
                                                  * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                       ? 0ULL : ((QData)((IData)(
                                                                 this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                 ((IData)(2U) 
                                                                  + 
                                                                  (7U 
                                                                   & (((IData)(4U) 
                                                                       + 
                                                                       ((IData)(0x54U) 
                                                                        * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                      >> 5U)))])) 
                                                 << 
                                                 ((IData)(0x40U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(4U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                     | (((QData)((IData)(
                                                         this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                         ((IData)(1U) 
                                                          + 
                                                          (7U 
                                                           & (((IData)(4U) 
                                                               + 
                                                               ((IData)(0x54U) 
                                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                              >> 5U)))])) 
                                         << ((0U == 
                                              (0x1fU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                              ? 0x20U
                                              : ((IData)(0x20U) 
                                                 - 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                        | ((QData)((IData)(
                                                           this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                           (7U 
                                                            & (((IData)(4U) 
                                                                + 
                                                                ((IData)(0x54U) 
                                                                 * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                               >> 5U))])) 
                                           >> (0x1fU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                  : 0ULL)) >> 0x1cU)) 
               | (0xfffffff0U & ((IData)((((0xa7U >= 
                                            (0xffU 
                                             & ((IData)(4U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                            ? (((0U 
                                                 == 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                 ? 0ULL
                                                 : 
                                                ((QData)((IData)(
                                                                 this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                 ((IData)(2U) 
                                                                  + 
                                                                  (7U 
                                                                   & (((IData)(4U) 
                                                                       + 
                                                                       ((IData)(0x54U) 
                                                                        * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                      >> 5U)))])) 
                                                 << 
                                                 ((IData)(0x40U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(4U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                               | (((QData)((IData)(
                                                                   this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                   ((IData)(1U) 
                                                                    + 
                                                                    (7U 
                                                                     & (((IData)(4U) 
                                                                         + 
                                                                         ((IData)(0x54U) 
                                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                        >> 5U)))])) 
                                                   << 
                                                   ((0U 
                                                     == 
                                                     (0x1fU 
                                                      & ((IData)(4U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                     ? 0x20U
                                                     : 
                                                    ((IData)(0x20U) 
                                                     - 
                                                     (0x1fU 
                                                      & ((IData)(4U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                                  | ((QData)((IData)(
                                                                     this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                                     (7U 
                                                                      & (((IData)(4U) 
                                                                          + 
                                                                          ((IData)(0x54U) 
                                                                           * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                         >> 5U))])) 
                                                     >> 
                                                     (0x1fU 
                                                      & ((IData)(4U) 
                                                         + 
                                                         ((IData)(0x54U) 
                                                          * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                            : 0ULL) 
                                          >> 0x20U)) 
                                 << 4U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = ((0xffff0U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]) 
               | (0xfU & ((IData)((((0xa7U >= (0xffU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                     ? (((0U == (0x1fU 
                                                 & ((IData)(4U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                          ? 0ULL : 
                                         ((QData)((IData)(
                                                          this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                          ((IData)(2U) 
                                                           + 
                                                           (7U 
                                                            & (((IData)(4U) 
                                                                + 
                                                                ((IData)(0x54U) 
                                                                 * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                               >> 5U)))])) 
                                          << ((IData)(0x40U) 
                                              - (0x1fU 
                                                 & ((IData)(4U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                        | (((QData)((IData)(
                                                            this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                            ((IData)(1U) 
                                                             + 
                                                             (7U 
                                                              & (((IData)(4U) 
                                                                  + 
                                                                  ((IData)(0x54U) 
                                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                 >> 5U)))])) 
                                            << ((0U 
                                                 == 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                                 ? 0x20U
                                                 : 
                                                ((IData)(0x20U) 
                                                 - 
                                                 (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                           | ((QData)((IData)(
                                                              this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                                              (7U 
                                                               & (((IData)(4U) 
                                                                   + 
                                                                   ((IData)(0x54U) 
                                                                    * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                                  >> 5U))])) 
                                              >> (0x1fU 
                                                  & ((IData)(4U) 
                                                     + 
                                                     ((IData)(0x54U) 
                                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                     : 0ULL) >> 0x20U)) 
                          >> 0x1cU)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfffffff3U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | (0xfffffffcU & (((0xa7U >= (0xffU 
                                             & ((IData)(2U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                   ? (3U & (((0U == 
                                              (0x1fU 
                                               & ((IData)(2U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))
                                              ? 0U : 
                                             (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                              ((IData)(1U) 
                                               + (7U 
                                                  & (((IData)(2U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                     >> 5U)))] 
                                              << ((IData)(0x20U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(2U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))))) 
                                            | (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                               (7U 
                                                & (((IData)(2U) 
                                                    + 
                                                    ((IData)(0x54U) 
                                                     * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                                   >> 5U))] 
                                               >> (0x1fU 
                                                   & ((IData)(2U) 
                                                      + 
                                                      ((IData)(0x54U) 
                                                       * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))))
                                   : 0U) << 2U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfffffffdU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | (0xfffffffeU & (((0xa7U >= (0xffU 
                                             & ((IData)(1U) 
                                                + ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))) 
                                  & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                                     (7U & (((IData)(1U) 
                                             + ((IData)(0x54U) 
                                                * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                                            >> 5U))] 
                                     >> (0x1fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(0x54U) 
                                                   * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))))) 
                                 << 1U)));
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] 
            = ((0xfffffffeU & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U]) 
               | ((0xa7U >= (0xffU & ((IData)(0x54U) 
                                      * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))) 
                  & (this->__Vcellinp__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__mst_resps_i[
                     (7U & (((IData)(0x54U) * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)) 
                            >> 5U))] >> (0x1fU & ((IData)(0x54U) 
                                                  * (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))))));
    } else {
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[0U] = 0U;
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[1U] = 0U;
        this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            = (0xfff00U & this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U]);
    }
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x13U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x13U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x13U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_aw_select_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x13U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x11U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_w_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x11U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x12U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x12U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_fill 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_drain) 
           & (~ (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
                 >> 0x12U)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_drain 
        = ((IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_ar_sel_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__b_full_q) 
           & (this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
              >> 0x12U));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_b_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            >> 0x10U) & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_b_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_r_spill_reg__DOT__spill_register_flushable_i__DOT__gen_spill_reg__DOT__a_fill 
        = ((this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_demux_simple__slv_resp_o[2U] 
            >> 8U) & (IData)(this->gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT____Vcellout__i_r_spill_reg__ready_o));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d 
        = (1U & (((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[3U] 
                   >> 4U) & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__req_nodes))
                  ? ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))
                      ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)
                      : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes))
                  : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_b_mux__DOT__gen_arbiter__DOT__rr_q)));
    this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d 
        = (1U & ((this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__slv_req_cut[0U] 
                  & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__req_nodes))
                  ? ((1U & (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))
                      ? (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)
                      : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes))
                  : (IData)(this->__PVT__gen_mrif_support_disabled__DOT__i_iommu_axi_demux_no_mrif__DOT__i_demux_simple__DOT__genblk1__DOT__i_r_mux__DOT__gen_arbiter__DOT__rr_q)));
}
