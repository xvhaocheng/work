// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresearch_ar_component_tb.h for the primary calling header

#include "Vresearch_ar_component_tb.h"
#include "Vresearch_ar_component_tb__Syms.h"

//==========

void Vresearch_ar_component_tb::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vresearch_ar_component_tb::eval\n"); );
    Vresearch_ar_component_tb__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/component_tb.sv", 2, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vresearch_ar_component_tb::_eval_initial_loop(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/component_tb.sv", 2, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vresearch_ar_component_tb::_sequent__TOP__2(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_sequent__TOP__2\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*1:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__Vfuncout;
    CData/*2:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__active;
    CData/*0:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found;
    CData/*2:0*/ __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q;
    CData/*1:0*/ __Vdlyvdim0__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0;
    CData/*0:0*/ __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0;
    CData/*0:0*/ __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v1;
    CData/*0:0*/ __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v2;
    CData/*0:0*/ __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v3;
    IData/*31:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__cursor;
    IData/*31:0*/ __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate;
    IData/*31:0*/ __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q;
    QData/*63:0*/ __Vtask_research_ar_component_tb__DOT__dut__DOT__emit_owner__2__owner_sequence;
    QData/*63:0*/ __Vtask_research_ar_component_tb__DOT__dut__DOT__emit_owner__3__owner_sequence;
    QData/*63:0*/ __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_cycle_q;
    QData/*63:0*/ __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_sequence_q;
    QData/*63:0*/ __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q;
    QData/*63:0*/ __Vdlyvval__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0;
    // Body
    __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_streak_q;
    __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q;
    __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_sequence_q 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_sequence_q;
    __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_streak_q;
    __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_cycle_q 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q;
    __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0 = 0U;
    __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v1 = 0U;
    __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v2 = 0U;
    __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v3 = 0U;
    if (vlTOPp->rst_ni) {
        if (vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_pending) {
            if (vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_fire) {
                if ((3U != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))) {
                    __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q = 0U;
                } else {
                    if ((4U != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_streak_q))) {
                        __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q 
                            = (7U & ((IData)(1U) + (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_streak_q)));
                    }
                }
            }
        } else {
            __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q = 0U;
        }
    } else {
        __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if ((((~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_q)) 
              & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_o)) 
             & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_i)))) {
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_idx_q 
                = vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o;
        }
    } else {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_idx_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q 
            = (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes) 
                & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i))) 
               & 1U);
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d;
    } else {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__rr_q = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_cycle_q 
            = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q);
        if (VL_UNLIKELY(((vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode 
                          != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_mode_q) 
                         | ((0U != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode) 
                            & (1U != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode))))) {
            VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:198: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner policy changed after reset\n",
                      64,VL_TIME_UNITED_Q(1),vlSymsp->name());
            VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 198, "");
        }
        if (VL_UNLIKELY((1U & (~ VL_ONEHOT0_I((IData)(vlTOPp->grant)))))) {
            VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:202: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner grants more than one input\n",
                      64,VL_TIME_UNITED_Q(1),vlSymsp->name());
            VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 202, "");
        }
        if (VL_UNLIKELY(((0U != (IData)(vlTOPp->grant)) 
                         & (~ (IData)(vlTOPp->ready))))) {
            VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:204: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner grants an input while downstream is not ready\n",
                      64,VL_TIME_UNITED_Q(1),vlSymsp->name());
            VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 204, "");
        }
        if (VL_UNLIKELY(((IData)(vlTOPp->valid) != 
                         (0U != (IData)(vlTOPp->req))))) {
            VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:206: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner valid is not supported by a current valid input\n",
                      64,VL_TIME_UNITED_Q(1),vlSymsp->name());
            VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 206, "");
        }
        if (VL_UNLIKELY((vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q 
                         != (vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q 
                             + ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q)
                                 ? 1ULL : 0ULL))))) {
            VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:209: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner first-offer/accept/held conservation failed\n",
                      64,VL_TIME_UNITED_Q(1),vlSymsp->name());
            VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 209, "");
        }
        if (vlTOPp->valid) {
            if (VL_UNLIKELY((1U & (~ ((IData)(vlTOPp->req) 
                                      >> (IData)(vlTOPp->index_o)))))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:213: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner selected an invalid input\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 213, "");
            }
            if (VL_UNLIKELY((0U != ((IData)(vlTOPp->grant) 
                                    & (~ (IData)(vlTOPp->req)))))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:217: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner grants an invalid input during a valid offer\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 217, "");
            }
            if (VL_UNLIKELY((0U != (((vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U] 
                                      ^ ((0x147U >= 
                                          (0x1ffU & 
                                           ((IData)(0x52U) 
                                            * (IData)(vlTOPp->index_o))))
                                          ? (((0U == 
                                               (0x1fU 
                                                & ((IData)(0x52U) 
                                                   * (IData)(vlTOPp->index_o))))
                                               ? 0U
                                               : (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                  ((IData)(1U) 
                                                   + 
                                                   (0xfU 
                                                    & (((IData)(0x52U) 
                                                        * (IData)(vlTOPp->index_o)) 
                                                       >> 5U)))] 
                                                  << 
                                                  ((IData)(0x20U) 
                                                   - 
                                                   (0x1fU 
                                                    & ((IData)(0x52U) 
                                                       * (IData)(vlTOPp->index_o)))))) 
                                             | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                (0xfU 
                                                 & (((IData)(0x52U) 
                                                     * (IData)(vlTOPp->index_o)) 
                                                    >> 5U))] 
                                                >> 
                                                (0x1fU 
                                                 & ((IData)(0x52U) 
                                                    * (IData)(vlTOPp->index_o)))))
                                          : 0U)) | 
                                     (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U] 
                                      ^ ((0x147U >= 
                                          (0x1ffU & 
                                           ((IData)(0x52U) 
                                            * (IData)(vlTOPp->index_o))))
                                          ? (((0U == 
                                               (0x1fU 
                                                & ((IData)(0x52U) 
                                                   * (IData)(vlTOPp->index_o))))
                                               ? 0U
                                               : (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                  ((IData)(2U) 
                                                   + 
                                                   (0xfU 
                                                    & (((IData)(0x52U) 
                                                        * (IData)(vlTOPp->index_o)) 
                                                       >> 5U)))] 
                                                  << 
                                                  ((IData)(0x20U) 
                                                   - 
                                                   (0x1fU 
                                                    & ((IData)(0x52U) 
                                                       * (IData)(vlTOPp->index_o)))))) 
                                             | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                ((IData)(1U) 
                                                 + 
                                                 (0xfU 
                                                  & (((IData)(0x52U) 
                                                      * (IData)(vlTOPp->index_o)) 
                                                     >> 5U)))] 
                                                >> 
                                                (0x1fU 
                                                 & ((IData)(0x52U) 
                                                    * (IData)(vlTOPp->index_o)))))
                                          : 0U))) | 
                                    (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
                                     ^ ((0x147U >= 
                                         (0x1ffU & 
                                          ((IData)(0x52U) 
                                           * (IData)(vlTOPp->index_o))))
                                         ? (0x3ffffU 
                                            & (((0U 
                                                 == 
                                                 (0x1fU 
                                                  & ((IData)(0x52U) 
                                                     * (IData)(vlTOPp->index_o))))
                                                 ? 0U
                                                 : 
                                                (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                 ((IData)(3U) 
                                                  + 
                                                  (0xfU 
                                                   & (((IData)(0x52U) 
                                                       * (IData)(vlTOPp->index_o)) 
                                                      >> 5U)))] 
                                                 << 
                                                 ((IData)(0x20U) 
                                                  - 
                                                  (0x1fU 
                                                   & ((IData)(0x52U) 
                                                      * (IData)(vlTOPp->index_o)))))) 
                                               | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                  ((IData)(2U) 
                                                   + 
                                                   (0xfU 
                                                    & (((IData)(0x52U) 
                                                        * (IData)(vlTOPp->index_o)) 
                                                       >> 5U)))] 
                                                  >> 
                                                  (0x1fU 
                                                   & ((IData)(0x52U) 
                                                      * (IData)(vlTOPp->index_o))))))
                                         : 0U)))))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:219: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner data does not belong to its selected input\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 219, "");
            }
            if (VL_UNLIKELY(((3U & (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
                                    >> 0x10U)) != (IData)(vlTOPp->index_o)))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:221: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner source index disagrees with prepended fullID\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 221, "");
            }
            if (VL_UNLIKELY(((IData)(vlTOPp->ready) 
                             & ((IData)(vlTOPp->grant) 
                                != (0xfU & ((IData)(1U) 
                                            << (IData)(vlTOPp->index_o))))))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:223: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner accepted output without its unique input grant\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 223, "");
            }
        }
        if (vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q) {
            if (VL_UNLIKELY((1U & (((~ (IData)(vlTOPp->valid)) 
                                    | ((IData)(vlTOPp->index_o) 
                                       != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))) 
                                   | (0U != (((vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U] 
                                               ^ vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[0U]) 
                                              | (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U] 
                                                 ^ 
                                                 vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[1U])) 
                                             | (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
                                                ^ vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[2U]))))))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:229: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner changed a previously offered held full packet\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 229, "");
            }
            if ((((3U != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                  & ((IData)(vlTOPp->req) >> 3U)) & 
                 (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_saw_ds_q)))) {
                vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_ds_seen_count_q 
                    = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_ds_seen_count_q);
                if ((1U & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_ds_present_at_offer_q)))) {
                    vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_late_ds_count_q 
                        = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_late_ds_count_q);
                }
                vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_saw_ds_q = 1U;
            }
            if (VL_UNLIKELY((1U & ((~ ((IData)(vlTOPp->req) 
                                       >> (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))) 
                                   | (0U != (((((0x147U 
                                                 >= 
                                                 (0x1ffU 
                                                  & ((IData)(0x52U) 
                                                     * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))
                                                 ? 
                                                (((0U 
                                                   == 
                                                   (0x1fU 
                                                    & ((IData)(0x52U) 
                                                       * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))
                                                   ? 0U
                                                   : 
                                                  (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                   ((IData)(1U) 
                                                    + 
                                                    (0xfU 
                                                     & (((IData)(0x52U) 
                                                         * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                                                        >> 5U)))] 
                                                   << 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & ((IData)(0x52U) 
                                                        * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)))))) 
                                                 | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                    (0xfU 
                                                     & (((IData)(0x52U) 
                                                         * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                                                        >> 5U))] 
                                                    >> 
                                                    (0x1fU 
                                                     & ((IData)(0x52U) 
                                                        * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)))))
                                                 : 0U) 
                                               ^ vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[0U]) 
                                              | (((0x147U 
                                                   >= 
                                                   (0x1ffU 
                                                    & ((IData)(0x52U) 
                                                       * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))
                                                   ? 
                                                  (((0U 
                                                     == 
                                                     (0x1fU 
                                                      & ((IData)(0x52U) 
                                                         * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))
                                                     ? 0U
                                                     : 
                                                    (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                     ((IData)(2U) 
                                                      + 
                                                      (0xfU 
                                                       & (((IData)(0x52U) 
                                                           * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                                                          >> 5U)))] 
                                                     << 
                                                     ((IData)(0x20U) 
                                                      - 
                                                      (0x1fU 
                                                       & ((IData)(0x52U) 
                                                          * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)))))) 
                                                   | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                      ((IData)(1U) 
                                                       + 
                                                       (0xfU 
                                                        & (((IData)(0x52U) 
                                                            * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                                                           >> 5U)))] 
                                                      >> 
                                                      (0x1fU 
                                                       & ((IData)(0x52U) 
                                                          * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)))))
                                                   : 0U) 
                                                 ^ 
                                                 vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[1U])) 
                                             | (((0x147U 
                                                  >= 
                                                  (0x1ffU 
                                                   & ((IData)(0x52U) 
                                                      * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))
                                                  ? 
                                                 (0x3ffffU 
                                                  & (((0U 
                                                       == 
                                                       (0x1fU 
                                                        & ((IData)(0x52U) 
                                                           * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))
                                                       ? 0U
                                                       : 
                                                      (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                       ((IData)(3U) 
                                                        + 
                                                        (0xfU 
                                                         & (((IData)(0x52U) 
                                                             * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                                                            >> 5U)))] 
                                                       << 
                                                       ((IData)(0x20U) 
                                                        - 
                                                        (0x1fU 
                                                         & ((IData)(0x52U) 
                                                            * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)))))) 
                                                     | (vlTOPp->research_ar_component_tb__DOT__data_i[
                                                        ((IData)(2U) 
                                                         + 
                                                         (0xfU 
                                                          & (((IData)(0x52U) 
                                                              * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q)) 
                                                             >> 5U)))] 
                                                        >> 
                                                        (0x1fU 
                                                         & ((IData)(0x52U) 
                                                            * (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q))))))
                                                  : 0U) 
                                                ^ vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[2U]))))))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:232: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: AR owner upstream withdrew or changed a held full packet\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 232, "");
            }
        }
        if ((1U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)) {
            if (VL_UNLIKELY(((((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_q) 
                               != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q)) 
                              | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_streak_q) 
                                 != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_streak_q)) 
                             | ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_rr_q) 
                                != vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_nd_cursor_q)))) {
                VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:245: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: DS-first control state disagrees with accepted-history reference\n",
                          64,VL_TIME_UNITED_Q(1),vlSymsp->name());
                VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 245, "");
            }
            if (((IData)(vlTOPp->valid) & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q)))) {
                if ((1U & (((IData)(vlTOPp->req) >> 3U) 
                           & (~ ((4U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_streak_q) 
                                 & (0U != (7U & (IData)(vlTOPp->req)))))))) {
                    if (VL_UNLIKELY((3U != (IData)(vlTOPp->index_o)))) {
                        VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:249: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: DS-first new selection failed to select current DS\n",
                                  64,VL_TIME_UNITED_Q(1),
                                  vlSymsp->name());
                        VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 249, "");
                    }
                } else {
                    if ((0U != (7U & (IData)(vlTOPp->req)))) {
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__cursor 
                            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_nd_cursor_q;
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__active 
                            = (7U & (IData)(vlTOPp->req));
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__Vfuncout = 0U;
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found = 0U;
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                            = __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__cursor;
                        if ((3U <= __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)) {
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                                = (__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                                   - (IData)(3U));
                        }
                        if (((2U >= (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)) 
                             & ((IData)(__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__active) 
                                >> (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)))) {
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__Vfuncout 
                                = (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate);
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found = 1U;
                        }
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                            = ((IData)(1U) + __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__cursor);
                        if ((3U <= __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)) {
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                                = (__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                                   - (IData)(3U));
                        }
                        if (((~ (IData)(__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found)) 
                             & ((2U >= (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)) 
                                & ((IData)(__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__active) 
                                   >> (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate))))) {
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__Vfuncout 
                                = (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate);
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found = 1U;
                        }
                        __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                            = ((IData)(2U) + __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__cursor);
                        if ((3U <= __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)) {
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                                = (__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate 
                                   - (IData)(3U));
                        }
                        if (((~ (IData)(__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found)) 
                             & ((2U >= (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate)) 
                                & ((IData)(__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__active) 
                                   >> (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate))))) {
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__Vfuncout 
                                = (3U & __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__candidate);
                            __Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__found = 1U;
                        }
                        if (VL_UNLIKELY(((IData)(vlTOPp->index_o) 
                                         != (IData)(__Vfunc_research_ar_component_tb__DOT__dut__DOT__monitor_nd_pick__1__Vfuncout)))) {
                            VL_WRITEF("[%0t] %%Error: research_ar_selector.sv:252: Assertion failed in %Nresearch_ar_component_tb.dut.p_monitor: DS-first F4 escape or non-DS cyclic selection failed\n",
                                      64,VL_TIME_UNITED_Q(1),
                                      vlSymsp->name());
                            VL_STOP_MT("/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/validation/model/research_ar_selector.sv", 252, "");
                        }
                    }
                }
            }
            if ((0U != (7U & (IData)(vlTOPp->req)))) {
                if (((IData)(vlTOPp->valid) & (IData)(vlTOPp->ready))) {
                    if ((3U != (IData)(vlTOPp->index_o))) {
                        __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q = 0U;
                    } else {
                        if ((4U > vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_streak_q)) {
                            __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q 
                                = ((IData)(1U) + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_streak_q);
                        }
                    }
                }
            } else {
                __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q = 0U;
            }
            if ((((IData)(vlTOPp->valid) & (IData)(vlTOPp->ready)) 
                 & (3U != (IData)(vlTOPp->index_o)))) {
                vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_nd_cursor_q 
                    = ((2U == (IData)(vlTOPp->index_o))
                        ? 0U : ((IData)(1U) + (IData)(vlTOPp->index_o)));
            }
        }
        if (VL_UNLIKELY(((IData)(vlTOPp->valid) & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q))))) {
            __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_sequence_q 
                = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_sequence_q);
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q 
                = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q);
            __Vtask_research_ar_component_tb__DOT__dut__DOT__emit_owner__2__owner_sequence 
                = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_sequence_q);
            VL_WRITEF("AR-OWNER-JSON {\"cycle\":%0#,\"mode\":%0d,\"phase\":\"firstoffer\",\"sequence\":%0#,\"source\":%0#,\"mask\":%0#,\"ready\":%0#,\"held\":%0#,\"addr\":\"0x%0x\",\"id\":%0#,\"len\":%0#,\"packet\":\"0x%0x\"}\n",
                      64,(1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q),
                      32,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode,
                      64,__Vtask_research_ar_component_tb__DOT__dut__DOT__emit_owner__2__owner_sequence,
                      2,(IData)(vlTOPp->index_o),4,
                      vlTOPp->req,1,(IData)(vlTOPp->ready),
                      1,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q,
                      64,(((QData)((IData)(vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U])) 
                           << 0x34U) | (((QData)((IData)(
                                                         vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U])) 
                                         << 0x14U) 
                                        | ((QData)((IData)(
                                                           vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U])) 
                                           >> 0xcU))),
                      6,(0x3fU & (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
                                  >> 0xcU)),8,(0xffU 
                                               & ((vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U] 
                                                   << 0x1cU) 
                                                  | (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U] 
                                                     >> 4U))),
                      82,vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o);
            if ((((IData)(vlTOPp->req) >> 3U) & (0U 
                                                 != 
                                                 (7U 
                                                  & (IData)(vlTOPp->req))))) {
                __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q 
                    = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q);
                if ((0ULL == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q)) {
                    vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_dual_head_cycle_q 
                        = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q);
                }
            }
        }
        if (VL_UNLIKELY(((IData)(vlTOPp->valid) & (IData)(vlTOPp->ready)))) {
            __Vtask_research_ar_component_tb__DOT__dut__DOT__emit_owner__3__owner_sequence 
                = ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q)
                    ? vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_sequence_q
                    : (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_sequence_q));
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q 
                = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q);
            __Vdlyvval__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0 
                = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q
                   [vlTOPp->index_o]);
            __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0 = 1U;
            __Vdlyvdim0__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0 
                = vlTOPp->index_o;
            VL_WRITEF("AR-OWNER-JSON {\"cycle\":%0#,\"mode\":%0d,\"phase\":\"accept\",\"sequence\":%0#,\"source\":%0#,\"mask\":%0#,\"ready\":%0#,\"held\":%0#,\"addr\":\"0x%0x\",\"id\":%0#,\"len\":%0#,\"packet\":\"0x%0x\"}\n",
                      64,(1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q),
                      32,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode,
                      64,__Vtask_research_ar_component_tb__DOT__dut__DOT__emit_owner__3__owner_sequence,
                      2,(IData)(vlTOPp->index_o),4,
                      vlTOPp->req,1,(IData)(vlTOPp->ready),
                      1,vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q,
                      64,(((QData)((IData)(vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U])) 
                           << 0x34U) | (((QData)((IData)(
                                                         vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U])) 
                                         << 0x14U) 
                                        | ((QData)((IData)(
                                                           vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U])) 
                                           >> 0xcU))),
                      6,(0x3fU & (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U] 
                                  >> 0xcU)),8,(0xffU 
                                               & ((vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U] 
                                                   << 0x1cU) 
                                                  | (vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U] 
                                                     >> 4U))),
                      82,vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o);
        }
        if ((((~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q)) 
              & (IData)(vlTOPp->valid)) & (~ (IData)(vlTOPp->ready)))) {
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_saw_ds_q = 0U;
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q 
                = vlTOPp->index_o;
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[0U] 
                = vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[0U];
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[1U] 
                = vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[1U];
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[2U] 
                = vlTOPp->research_ar_component_tb__DOT____Vcellout__dut__data_o[2U];
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_sequence_q 
                = (1ULL + vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_sequence_q);
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_ds_present_at_offer_q 
                = (1U & ((IData)(vlTOPp->req) >> 3U));
        }
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q 
            = ((IData)(vlTOPp->valid) & (~ (IData)(vlTOPp->ready)));
    } else {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_offer_count_q = 0ULL;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_accept_count_q = 0ULL;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_ds_seen_count_q = 0ULL;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_nonds_late_ds_count_q = 0ULL;
        __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v1 = 1U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_saw_ds_q = 0U;
        __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_cycle_q = 0ULL;
        __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_sequence_q = 0ULL;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_sequence_q = 0ULL;
        __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q = 0ULL;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_first_dual_head_cycle_q = 0ULL;
        __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v2 = 1U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_q = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_ds_present_at_offer_q = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_idx_q = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[0U] = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[1U] = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_held_data_q[2U] = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_mode_q 
            = vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode;
        __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_nd_cursor_q = 0U;
        __Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v3 = 1U;
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_cycle_q 
        = __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_cycle_q;
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_streak_q 
        = __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_streak_q;
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_sequence_q 
        = __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_sequence_q;
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q 
        = __Vdly__research_ar_component_tb__DOT__dut__DOT__mon_dual_head_first_offer_count_q;
    if (__Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[__Vdlyvdim0__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0] 
            = __Vdlyvval__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v0;
    }
    if (__Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v1) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[0U] = 0ULL;
    }
    if (__Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v2) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[1U] = 0ULL;
    }
    if (__Vdlyvset__research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q__v3) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[2U] = 0ULL;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__mon_source_accepts_q[3U] = 0ULL;
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_streak_q 
        = __Vdly__research_ar_component_tb__DOT__dut__DOT__ds_streak_q;
    if (vlTOPp->rst_ni) {
        if (((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_fire) 
             & (3U != (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o)))) {
            vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_rr_q 
                = ((2U == (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))
                    ? 0U : (3U & ((IData)(1U) + (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_idx_o))));
        }
    } else {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__nd_rr_q = 0U;
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__held_q 
        = ((IData)(vlTOPp->rst_ni) & ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_o) 
                                      & (~ (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_i))));
}

VL_INLINE_OPT void Vresearch_ar_component_tb::_combo__TOP__4(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_combo__TOP__4\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp49[3];
    // Body
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_req_i 
        = ((1U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)
            ? (IData)(vlTOPp->req) : 0U);
    if ((0U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode)) {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_req_i 
            = vlTOPp->req;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i 
            = ((IData)(vlTOPp->ready) & 1U);
    } else {
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_req_i = 0U;
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i = 0U;
    }
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
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__ds_gnt_i 
        = ((1U == vlTOPp->research_ar_component_tb__DOT__dut__DOT__mode) 
           & (IData)(vlTOPp->ready));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_d 
        = ((IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q)
            ? (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q)
            : (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_req_i));
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes 
        = ((6U & (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes)) 
           | (IData)(vlTOPp->research_ar_component_tb__DOT__dut__DOT__rr_gnt_i));
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
        __Vtemp49[1U] = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U] 
                          << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U] 
                                       >> 4U));
        __Vtemp49[2U] = (0x3ffffU & (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U] 
                                     >> 4U));
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U] 
            = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U] 
                << 0x1cU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
                             >> 4U));
    } else {
        __Vtemp49[1U] = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U] 
                          << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U] 
                                      >> 0x12U));
        __Vtemp49[2U] = (0x3ffffU & ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
                                      << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U] 
                                                  >> 0x12U)));
        vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U] 
            = ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U] 
                << 0xeU) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U] 
                            >> 0x12U));
    }
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1U] 
        = __Vtemp49[1U];
    vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U] 
        = ((0xfffc0000U & vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U]) 
           | __Vtemp49[2U]);
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

void Vresearch_ar_component_tb::_eval(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_eval\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk_i))) 
         | ((~ (IData)(vlTOPp->rst_ni)) & (IData)(vlTOPp->__Vclklast__TOP__rst_ni)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
    vlTOPp->__Vclklast__TOP__rst_ni = vlTOPp->rst_ni;
}

VL_INLINE_OPT QData Vresearch_ar_component_tb::_change_request(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_change_request\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vresearch_ar_component_tb::_change_request_1(Vresearch_ar_component_tb__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_change_request_1\n"); );
    Vresearch_ar_component_tb* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    __req |= ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes)
         | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7])
         | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes)
        || (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes)
         | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes)
         | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes)
         | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes)
         | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes));
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv:125: research_ar_component_tb.dut.i_original_rr.gen_arbiter.index_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6]) | (vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7] ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7]))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv:126: research_ar_component_tb.dut.i_original_rr.gen_arbiter.data_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv:127: research_ar_component_tb.dut.i_original_rr.gen_arbiter.gnt_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv:128: research_ar_component_tb.dut.i_original_rr.gen_arbiter.req_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/lzc.sv:49: research_ar_component_tb.dut.i_original_rr.gen_arbiter.gen_int_rr.gen_fair_arb.i_lzc_upper.gen_lzc.sel_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/lzc.sv:50: research_ar_component_tb.dut.i_original_rr.gen_arbiter.gen_int_rr.gen_fair_arb.i_lzc_upper.gen_lzc.index_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/lzc.sv:49: research_ar_component_tb.dut.i_original_rr.gen_arbiter.gen_int_rr.gen_fair_arb.i_lzc_lower.gen_lzc.sel_nodes\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes ^ vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes))) VL_DBG_MSGF("        CHANGE: /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/lzc.sv:50: research_ar_component_tb.dut.i_original_rr.gen_arbiter.gen_int_rr.gen_fair_arb.i_lzc_lower.gen_lzc.index_nodes\n"); );
    // Final
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__index_nodes;
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[0U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[1U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[2U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[3U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[4U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[5U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[6U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U] 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__data_nodes[7U];
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gnt_nodes;
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__req_nodes;
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__sel_nodes;
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_upper__DOT__gen_lzc__DOT__index_nodes;
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__sel_nodes;
    vlTOPp->__Vchglast__TOP__research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes 
        = vlTOPp->research_ar_component_tb__DOT__dut__DOT__i_original_rr__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_fair_arb__DOT__i_lzc_lower__DOT__gen_lzc__DOT__index_nodes;
    return __req;
}

#ifdef VL_DEBUG
void Vresearch_ar_component_tb::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresearch_ar_component_tb::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk_i & 0xfeU))) {
        Verilated::overWidthError("clk_i");}
    if (VL_UNLIKELY((rst_ni & 0xfeU))) {
        Verilated::overWidthError("rst_ni");}
    if (VL_UNLIKELY((req & 0xf0U))) {
        Verilated::overWidthError("req");}
    if (VL_UNLIKELY((ready & 0xfeU))) {
        Verilated::overWidthError("ready");}
    if (VL_UNLIKELY((tamper_i & 0xfeU))) {
        Verilated::overWidthError("tamper_i");}
}
#endif  // VL_DEBUG
