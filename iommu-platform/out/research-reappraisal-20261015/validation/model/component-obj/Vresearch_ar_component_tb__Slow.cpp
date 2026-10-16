// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresearch_ar_component_tb.h for the primary calling header

#include "Vresearch_ar_component_tb.h"
#include "Vresearch_ar_component_tb__Syms.h"

//==========

VL_CTOR_IMP(Vresearch_ar_component_tb) {
    Vresearch_ar_component_tb__Syms* __restrict vlSymsp = __VlSymsp = new Vresearch_ar_component_tb__Syms(this, name());
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vresearch_ar_component_tb::__Vconfigure(Vresearch_ar_component_tb__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vresearch_ar_component_tb::~Vresearch_ar_component_tb() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vresearch_ar_component_tb::_final_TOP(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_final_TOP\n"); );
    // Variables
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    VL_WRITEF("AR-OWNER-SUMMARY-JSON {\"cycle\":%0#,\"mode\":%0d,\"first_offer_count\":%0#,\"accept_count\":%0#,\"held\":%0#,\"dual_head_first_offer_count\":%0#,\"first_dual_head_cycle\":%0#,\"held_nonds_ds_seen_count\":%0#,\"held_nonds_late_ds_count\":%0#,\"source_accepts\":[%0#,%0#,%0#,%0#]}\n",
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q,
              32,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode,
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q,
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q,
              1,(IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q),
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q,
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_dual_head_cycle_q,
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_ds_seen_count_q,
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_late_ds_count_q,
              64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q
              [0U],64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q
              [1U],64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q
              [2U],64,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q
              [3U]);
}

void Vresearch_ar_component_tb::_settle__TOP__1(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_settle__TOP__1\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut 
        = (0xfcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut 
        = (4U | (0xf3U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut 
        = (0x20U | (0xcfU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut 
        = (0xc0U | (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut 
        = (0xfcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut 
        = (4U | (0xf3U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut 
        = (0x20U | (0xcfU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut 
        = (0xc0U | (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = (0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask));
    vlTOPp->research_ar_component_tb__DOT__data_i[0U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[1U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
        = (0xfffc0000U & vlTOPp->research_ar_component_tb__DOT__data_i[2U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
        = (0xfffc0fffU & vlTOPp->research_ar_component_tb__DOT__data_i[2U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[0U] 
        = (0x1000000U | (0xfffU & vlTOPp->research_ar_component_tb__DOT__data_i[0U]));
    vlTOPp->research_ar_component_tb__DOT__data_i[1U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
        = (0xfffff000U & vlTOPp->research_ar_component_tb__DOT__data_i[2U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[0U] 
        = (0xfffff00fU & vlTOPp->research_ar_component_tb__DOT__data_i[0U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
        = (0x3ffffU & vlTOPp->research_ar_component_tb__DOT__data_i[2U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[3U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[4U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
        = (0xfffffff0U & vlTOPp->research_ar_component_tb__DOT__data_i[5U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[4U] 
        = (0x3fffffffU & vlTOPp->research_ar_component_tb__DOT__data_i[4U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
        = (4U | (0xfffffff0U & vlTOPp->research_ar_component_tb__DOT__data_i[5U]));
    vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
        = (0x3fffffffU & vlTOPp->research_ar_component_tb__DOT__data_i[2U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[3U] = 0x800U;
    vlTOPp->research_ar_component_tb__DOT__data_i[4U] 
        = (0xc0000000U & vlTOPp->research_ar_component_tb__DOT__data_i[4U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
        = (0xc03fffffU & vlTOPp->research_ar_component_tb__DOT__data_i[2U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
        = (0xfU & vlTOPp->research_ar_component_tb__DOT__data_i[5U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[6U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
        = (0xffc00000U & vlTOPp->research_ar_component_tb__DOT__data_i[7U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
        = (0x200000U | (0xffc0ffffU & vlTOPp->research_ar_component_tb__DOT__data_i[7U]));
    vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
        = (0x30000000U | (0xffffU & vlTOPp->research_ar_component_tb__DOT__data_i[5U]));
    vlTOPp->research_ar_component_tb__DOT__data_i[6U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
        = (0xffff0000U & vlTOPp->research_ar_component_tb__DOT__data_i[7U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
        = (0xffff00ffU & vlTOPp->research_ar_component_tb__DOT__data_i[5U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
        = (0x3fffffU & vlTOPp->research_ar_component_tb__DOT__data_i[7U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[8U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[9U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[0xaU] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[0xaU] 
        = (0xc0U | (3U & vlTOPp->research_ar_component_tb__DOT__data_i[0xaU]));
    vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
        = (0x10000U | (3U & vlTOPp->research_ar_component_tb__DOT__data_i[8U]));
    vlTOPp->research_ar_component_tb__DOT__data_i[9U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__data_i[0xaU] 
        = (0xfcU & vlTOPp->research_ar_component_tb__DOT__data_i[0xaU]);
    vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
        = (0x3ffffffU & vlTOPp->research_ar_component_tb__DOT__data_i[7U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
        = (0xfffffffcU & vlTOPp->research_ar_component_tb__DOT__data_i[8U]);
    vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
        = ((0xffffff0fU & vlTOPp->research_ar_component_tb__DOT__data_i[5U]) 
           | (0xfffffff0U & (((IData)(vlTOPp->tamper_i)
                               ? 8U : 0U) << 4U)));
}

void Vresearch_ar_component_tb::_initial__TOP__3(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_initial__TOP__3\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*127:0*/ __Vtemp24[4];
    // Body
    __Vtemp24[0U] = 0x743d2564U;
    __Vtemp24[1U] = 0x66697273U;
    __Vtemp24[2U] = 0x2d64732dU;
    __Vtemp24[3U] = 0x6172U;
    if ((! VL_VALUEPLUSARGS_INI(32,VL_CVT_PACK_STR_NW(4, __Vtemp24),
                                vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode))) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode = 0U;
    }
    if (VL_UNLIKELY(((0U != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode) 
                     & (1U != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)))) {
        VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:22: Assertion failed in %Nresearch_ar_component_tb.dut.p_mode: ar-ds-first must be 0 (original RR) or 1 (DS-first/F4)\n",
                  64,VL_TIME_UNITED_Q(1),vlSymsp->name());
        VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 22, "");
    }
}

void Vresearch_ar_component_tb::_settle__TOP__5(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_settle__TOP__5\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp83[3];
    // Body
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_i 
        = ((1U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)
            ? (IData)(vlTOPp->req) : 0U);
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i 
        = ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode) 
           & (IData)(vlTOPp->ready));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_req_i 
        = ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)
            ? (IData)(vlTOPp->req) : 0U);
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_i 
        = ((1U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode) 
           & (IData)(vlTOPp->ready));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes 
        = ((6U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes)) 
           | (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d 
        = ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q)
            ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q)
            : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_req_i));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_pending 
        = (0U != (7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_i)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__fresh_valid 
        = (0U != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_i));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__fresh_idx = 0U;
    if ((1U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_i) 
                >> 3U) & (~ ((4U == (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_streak_q)) 
                             & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_pending)))))) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__fresh_idx = 3U;
    } else {
        if (vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_pending) {
            vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__cursor 
                = vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_rr_q;
            vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active 
                = (7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_i));
            vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__Vfuncout 
                = ((1U == (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__cursor))
                    ? ((2U & (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active))
                        ? 1U : ((4U & (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active))
                                 ? 2U : 0U)) : ((2U 
                                                 == (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__cursor))
                                                 ? 
                                                ((4U 
                                                  & (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active))
                                                  ? 2U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active))
                                                   ? 0U
                                                   : 1U))
                                                 : 
                                                ((1U 
                                                  & (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active))
                                                  ? 0U
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active))
                                                   ? 1U
                                                   : 2U))));
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__fresh_idx 
                = vlTOPp->__Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__Vfuncout;
        }
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o 
        = ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_q)
            ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_idx_q)
            : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__fresh_idx));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_o 
        = ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_q) 
           | (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__fresh_valid));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[0U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[1U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[2U] = 0U;
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_o = 0U;
    if (vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_o) {
        if ((0x147U >= (0x1ffU & ((IData)(0x52U) * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))))) {
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[0U] 
                = (((0U == (0x1fU & ((IData)(0x52U) 
                                     * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))))
                     ? 0U : (vlTOPp->research_ar_component_tb__DOT__data_i[
                             ((IData)(1U) + (0xfU & 
                                             (((IData)(0x52U) 
                                               * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)) 
                                              >> 5U)))] 
                             << ((IData)(0x20U) - (0x1fU 
                                                   & ((IData)(0x52U) 
                                                      * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)))))) 
                   | (vlTOPp->research_ar_component_tb__DOT__data_i[
                      (0xfU & (((IData)(0x52U) * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)) 
                               >> 5U))] >> (0x1fU & 
                                            ((IData)(0x52U) 
                                             * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)))));
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[1U] 
                = (((0U == (0x1fU & ((IData)(0x52U) 
                                     * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))))
                     ? 0U : (vlTOPp->research_ar_component_tb__DOT__data_i[
                             ((IData)(2U) + (0xfU & 
                                             (((IData)(0x52U) 
                                               * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)) 
                                              >> 5U)))] 
                             << ((IData)(0x20U) - (0x1fU 
                                                   & ((IData)(0x52U) 
                                                      * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)))))) 
                   | (vlTOPp->research_ar_component_tb__DOT__data_i[
                      ((IData)(1U) + (0xfU & (((IData)(0x52U) 
                                               * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)) 
                                              >> 5U)))] 
                      >> (0x1fU & ((IData)(0x52U) * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)))));
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[2U] 
                = (0x3ffffU & (((0U == (0x1fU & ((IData)(0x52U) 
                                                 * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))))
                                 ? 0U : (vlTOPp->research_ar_component_tb__DOT__data_i[
                                         ((IData)(3U) 
                                          + (0xfU & 
                                             (((IData)(0x52U) 
                                               * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)) 
                                              >> 5U)))] 
                                         << ((IData)(0x20U) 
                                             - (0x1fU 
                                                & ((IData)(0x52U) 
                                                   * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)))))) 
                               | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                  ((IData)(2U) + (0xfU 
                                                  & (((IData)(0x52U) 
                                                      * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)) 
                                                     >> 5U)))] 
                                  >> (0x1fU & ((IData)(0x52U) 
                                               * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))))));
        } else {
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[0U] = 0U;
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[1U] = 0U;
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[2U] = 0U;
        }
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_o 
            = (((~ ((IData)(1U) << (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))) 
                & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_o)) 
               | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_i) 
                  << (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)));
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_fire 
        = ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_o) 
           & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_i));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes 
        = ((5U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes)) 
           | (2U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d) 
                     << 1U) | (0xfffffffeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes 
        = ((3U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes)) 
           | (4U & ((0xfffffffcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d)) 
                    | (0x7ffffffcU & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d) 
                                      >> 1U)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel 
        = (1U & ((~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d)) 
                 | (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d) 
                     >> 1U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel 
        = (1U & ((~ ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d) 
                     >> 2U)) | (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d) 
                                 >> 3U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (1U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffffeU & (((1U <= (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffffcU & (((2U <= (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)) 
                              << 2U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask 
        = ((7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)) 
           | (0xfffffff8U & (((3U <= (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)) 
                              << 3U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffffeU & (((1U > (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)) 
                              << 1U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffffcU & (((2U > (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)) 
                              << 2U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask 
        = ((7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)) 
           | (0xfffffff8U & (((3U > (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)) 
                              << 3U) & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes 
        = ((6U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes)) 
           | (1U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes) 
                     >> 1U) | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes) 
                               >> 2U))));
    vlTOPp->valid = (1U & ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)
                            ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes)
                            : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_o)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel 
        = (1U & ((~ ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes) 
                     >> 1U)) | (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes) 
                                 >> 2U) & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q) 
                                           >> 1U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o 
        = ((0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o)) 
           | (1U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes) 
                     >> 1U) & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o)) 
           | (0xfffffffeU & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes) 
                             & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel) 
                                << 1U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes 
        = ((0x33U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes)) 
           | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel) 
              << 2U));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U] 
        = ((0x3ffffU & vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U]) 
           | (0xfffc0000U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
                               ? ((vlTOPp->research_ar_component_tb__DOT__data_i[3U] 
                                   << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
                                               >> 0x12U))
                               : vlTOPp->research_ar_component_tb__DOT__data_i[0U]) 
                             << 0x12U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U] 
        = ((0x3ffffU & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
                          ? ((vlTOPp->research_ar_component_tb__DOT__data_i[3U] 
                              << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__data_i[2U] 
                                          >> 0x12U))
                          : vlTOPp->research_ar_component_tb__DOT__data_i[0U]) 
                        >> 0xeU)) | (0xfffc0000U & 
                                     (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
                                        ? ((vlTOPp->research_ar_component_tb__DOT__data_i[4U] 
                                            << 0xeU) 
                                           | (vlTOPp->research_ar_component_tb__DOT__data_i[3U] 
                                              >> 0x12U))
                                        : vlTOPp->research_ar_component_tb__DOT__data_i[1U]) 
                                      << 0x12U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U] 
        = ((0x3ffffU & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
                          ? ((vlTOPp->research_ar_component_tb__DOT__data_i[4U] 
                              << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__data_i[3U] 
                                          >> 0x12U))
                          : vlTOPp->research_ar_component_tb__DOT__data_i[1U]) 
                        >> 0xeU)) | (0xfffc0000U & 
                                     (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
                                        ? ((vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
                                            << 0xeU) 
                                           | (vlTOPp->research_ar_component_tb__DOT__data_i[4U] 
                                              >> 0x12U))
                                        : vlTOPp->research_ar_component_tb__DOT__data_i[2U]) 
                                      << 0x12U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
        = ((0xfffffff0U & vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U]) 
           | (0xfU & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
                        ? ((vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
                            << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__data_i[4U] 
                                        >> 0x12U)) : 
                       vlTOPp->research_ar_component_tb__DOT__data_i[2U]) 
                      >> 0xeU)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o)) 
           | (4U & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes) 
                    & ((~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel)) 
                       << 2U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o 
        = ((7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o)) 
           | (0xfffffff8U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes) 
                              << 1U) & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel) 
                                        << 3U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes 
        = ((0xfU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes)) 
           | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel) 
              << 4U));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
        = ((0xfU & vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U]) 
           | (0xfffffff0U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel)
                               ? ((vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
                                   << 0xaU) | (vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
                                               >> 0x16U))
                               : ((vlTOPp->research_ar_component_tb__DOT__data_i[6U] 
                                   << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
                                                >> 4U))) 
                             << 4U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U] 
        = ((0xfU & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel)
                      ? ((vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
                          << 0xaU) | (vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
                                      >> 0x16U)) : 
                     ((vlTOPp->research_ar_component_tb__DOT__data_i[6U] 
                       << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__data_i[5U] 
                                    >> 4U))) >> 0x1cU)) 
           | (0xfffffff0U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel)
                               ? ((vlTOPp->research_ar_component_tb__DOT__data_i[9U] 
                                   << 0xaU) | (vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
                                               >> 0x16U))
                               : ((vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
                                   << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__data_i[6U] 
                                                >> 4U))) 
                             << 4U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U] 
        = ((0xfU & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel)
                      ? ((vlTOPp->research_ar_component_tb__DOT__data_i[9U] 
                          << 0xaU) | (vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
                                      >> 0x16U)) : 
                     ((vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
                       << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__data_i[6U] 
                                    >> 4U))) >> 0x1cU)) 
           | (0x3ffff0U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel)
                             ? ((vlTOPp->research_ar_component_tb__DOT__data_i[0xaU] 
                                 << 0xaU) | (vlTOPp->research_ar_component_tb__DOT__data_i[9U] 
                                             >> 0x16U))
                             : ((vlTOPp->research_ar_component_tb__DOT__data_i[8U] 
                                 << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__data_i[7U] 
                                              >> 4U))) 
                           << 4U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (4U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp 
        = ((7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
           | (8U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (1U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (2U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (4U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp 
        = ((7U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
           | (8U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes 
        = ((5U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes)) 
           | (2U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes) 
                     & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel))) 
                    << 1U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes 
        = ((3U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes)) 
           | (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes) 
               & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)) 
              << 2U));
    vlTOPp->grant = ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)
                      ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_o)
                      : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_o));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes 
        = ((0x3cU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes)) 
           | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel)
               ? (2U | (1U & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes) 
                              >> 4U))) : (1U & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes) 
                                                >> 2U))));
    vlTOPp->index_o = (3U & ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)
                              ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes)
                              : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)));
    if (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel) {
        __Vtemp83[1U] = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U] 
                          << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U] 
                                       >> 4U));
        __Vtemp83[2U] = (0x3ffffU & (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U] 
                                     >> 4U));
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U] 
            = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U] 
                << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
                             >> 4U));
    } else {
        __Vtemp83[1U] = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U] 
                          << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U] 
                                      >> 0x12U));
        __Vtemp83[2U] = (0x3ffffU & ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
                                      << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U] 
                                                  >> 0x12U)));
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U] 
            = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U] 
                << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U] 
                            >> 0x12U));
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1U] 
        = __Vtemp83[1U];
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U] 
        = ((0xfffc0000U & vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U]) 
           | __Vtemp83[2U]);
    if ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)) {
        vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U] 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U];
        vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U] 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1U];
        vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
            = (0x3ffffU & vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U]);
    } else {
        vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U] 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[0U];
        vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U] 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[1U];
        vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
            = (0x3ffffU & vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_data_o[2U]);
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes)) 
           | (2U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp) 
                     << 1U) | (0xfffffffeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes)) 
           | (4U & ((0xfffffffcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp)) 
                    | (0x7ffffffcU & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp) 
                                      >> 1U)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((0xf3U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (0xcU & (((1U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp))
                        ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut)
                        : ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                           >> 2U)) << 2U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((0xcfU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (0x30U & (((4U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp))
                         ? ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                            >> 4U) : ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut) 
                                      >> 6U)) << 4U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((0xf3U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (0xcU & (((1U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp))
                        ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut)
                        : ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                           >> 2U)) << 2U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((0xcfU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (0x30U & (((4U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp))
                         ? ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                            >> 4U) : ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut) 
                                      >> 6U)) << 4U)));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((0xdU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (2U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                     << 1U) | (0xfffffffeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((0xbU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (4U & ((0xfffffffcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp)) 
                    | (0x7ffffffcU & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp) 
                                      >> 1U)))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes 
        = ((0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes)) 
           | (1U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes) 
                     >> 1U) | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes) 
                               >> 2U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = ((0xfcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes)) 
           | (3U & ((2U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes))
                     ? ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes) 
                        >> 2U) : ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes) 
                                  >> 4U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = ((0xeU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)) 
           | (1U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes) 
                     >> 1U) | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes) 
                               >> 2U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = ((0xfcU & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)) 
           | (3U & ((2U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))
                     ? ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes) 
                        >> 2U) : ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes) 
                                  >> 4U))));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d 
        = (3U & (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i) 
                  & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes))
                  ? ((1U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))
                      ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)
                      : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes))
                  : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q)));
}

void Vresearch_ar_component_tb::_eval_initial(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_eval_initial\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
    vlTOPp->_initial__TOP__3(vlSymsp);
}

void Vresearch_ar_component_tb::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::final\n"); );
    // Variables
    Vresearch_ar_component_tb__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_final_TOP(vlSymsp);
}

void Vresearch_ar_component_tb::_eval_settle(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_eval_settle\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__1(vlSymsp);
    vlTOPp->_settle__TOP__5(vlSymsp);
}

void Vresearch_ar_component_tb::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    req = VL_RAND_RESET_I(4);
    ready = VL_RAND_RESET_I(1);
    tamper_i = VL_RAND_RESET_I(1);
    valid = VL_RAND_RESET_I(1);
    grant = VL_RAND_RESET_I(4);
    index_o = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(328, research_ar_component_tb__DOT__data_i);
    VL_RAND_RESET_W(82, research_ar_component_tb__DOT____Vcellout__dut__data_o);
    research_ar_component_tb__DOT__dut__DOT__mode = VL_RAND_RESET_I(32);
    research_ar_component_tb__DOT__dut__DOT__rr_req_i = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__rr_gnt_o = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__ds_req_i = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__ds_gnt_o = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__rr_gnt_i = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__ds_gnt_i = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__ds_req_o = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__ds_idx_o = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(82, research_ar_component_tb__DOT__dut__DOT__ds_data_o);
    research_ar_component_tb__DOT__dut__DOT__nd_rr_q = VL_RAND_RESET_I(2);
    research_ar_component_tb__DOT__dut__DOT__held_idx_q = VL_RAND_RESET_I(2);
    research_ar_component_tb__DOT__dut__DOT__ds_streak_q = VL_RAND_RESET_I(3);
    research_ar_component_tb__DOT__dut__DOT__held_q = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__nd_pending = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__fresh_valid = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__ds_fire = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__fresh_idx = VL_RAND_RESET_I(2);
    research_ar_component_tb__DOT__dut__DOT__mon_cycle_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_sequence_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_held_sequence_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_first_dual_head_cycle_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_ds_seen_count_q = 0;
    research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_late_ds_count_q = 0;
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[__Vi0] = 0;
    }}
    research_ar_component_tb__DOT__dut__DOT__mon_held_q = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__mon_held_saw_ds_q = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__mon_ds_present_at_offer_q = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(82, research_ar_component_tb__DOT__dut__DOT__mon_held_data_q);
    research_ar_component_tb__DOT__dut__DOT__mon_mode_q = VL_RAND_RESET_I(32);
    research_ar_component_tb__DOT__dut__DOT__mon_streak_q = VL_RAND_RESET_I(32);
    research_ar_component_tb__DOT__dut__DOT__mon_nd_cursor_q = VL_RAND_RESET_I(32);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes = VL_RAND_RESET_I(6);
    VL_RAND_RESET_W(246, research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes = VL_RAND_RESET_I(3);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes = VL_RAND_RESET_I(3);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q = VL_RAND_RESET_I(2);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d = VL_RAND_RESET_I(2);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__upper_mask = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__lower_mask = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__0__KET____DOT__gen_level__BRA__0__KET____DOT__sel = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__0__KET____DOT__sel = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_levels__BRA__1__KET____DOT__gen_level__BRA__1__KET____DOT__sel = VL_RAND_RESET_I(1);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_lut = VL_RAND_RESET_I(8);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes = VL_RAND_RESET_I(8);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__in_tmp = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_lut = VL_RAND_RESET_I(8);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes = VL_RAND_RESET_I(4);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes = VL_RAND_RESET_I(8);
    research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__in_tmp = VL_RAND_RESET_I(4);
    __Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__Vfuncout = VL_RAND_RESET_I(2);
    __Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__active = VL_RAND_RESET_I(3);
    __Vfunc_research_ar_component_tb__DOT__dut__DOT__nd_pick__0__cursor = VL_RAND_RESET_I(2);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes = VL_RAND_RESET_I(6);
    VL_RAND_RESET_W(246, __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes = VL_RAND_RESET_I(3);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes = VL_RAND_RESET_I(3);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes = VL_RAND_RESET_I(4);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes = VL_RAND_RESET_I(8);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes = VL_RAND_RESET_I(4);
    __Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes = VL_RAND_RESET_I(8);
}
