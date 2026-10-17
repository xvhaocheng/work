// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpn_backend_top.h for the primary calling header

#include "Vpn_backend_top__pch.h"
#include "Vpn_backend_top___024root.h"

VL_ATTR_COLD void Vpn_backend_top___024root___eval_static__TOP(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___eval_static__TOP\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.nonblock_i = 0U;
}

VL_ATTR_COLD void Vpn_backend_top___024root___eval_initial(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___eval_initial\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vpn_backend_top___024root___eval_final(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___eval_final\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpn_backend_top___024root___dump_triggers__stl(Vpn_backend_top___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vpn_backend_top___024root___eval_phase__stl(Vpn_backend_top___024root* vlSelf);

VL_ATTR_COLD void Vpn_backend_top___024root___eval_settle(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___eval_settle\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vpn_backend_top___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("/home/xvhaocheng/work-e6-pn-backend/paper/ispass2027/exp/E6-pn/backend-validation/runs/run06/source/pn_backend_top.sv", 4, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vpn_backend_top___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpn_backend_top___024root___dump_triggers__stl(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___dump_triggers__stl\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
    if ((2ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 1 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((4ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 2 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
    if ((8ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 3 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((0x10ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 4 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vpn_backend_top___024root___eval_triggers__stl(Vpn_backend_top___024root* vlSelf);
VL_ATTR_COLD void Vpn_backend_top___024root___eval_stl(Vpn_backend_top___024root* vlSelf);

VL_ATTR_COLD bool Vpn_backend_top___024root___eval_phase__stl(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___eval_phase__stl\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vpn_backend_top___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vpn_backend_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpn_backend_top___024root___dump_triggers__ico(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___dump_triggers__ico\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VicoTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
    if ((2ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 1 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((4ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 2 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
    if ((8ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 3 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((0x10ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 4 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpn_backend_top___024root___dump_triggers__act(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___dump_triggers__act\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
    if ((4ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((8ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 3 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
    if ((0x10ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 4 is active: @(posedge clk_i)\n");
    }
    if ((0x20ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 5 is active: @(negedge rst_ni)\n");
    }
    if ((0x40ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 6 is active: @(negedge pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.rst_ni)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpn_backend_top___024root___dump_triggers__nba(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___dump_triggers__nba\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @([hybrid] pn_backend_top.candidate.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_b_readies)\n");
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 3 is active: @([hybrid] pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.i_demux_supported_vs_unsupported.i_demux_simple.genblk1.mst_r_readies)\n");
    }
    if ((0x10ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 4 is active: @(posedge clk_i)\n");
    }
    if ((0x20ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 5 is active: @(negedge rst_ni)\n");
    }
    if ((0x40ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 6 is active: @(negedge pn_backend_top.reference_dut.i_axi_translate.i_axi_to_axi_lite.i_axi_burst_splitter.rst_ni)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vpn_backend_top___024root___ctor_var_reset(Vpn_backend_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpn_backend_top___024root___ctor_var_reset\n"); );
    Vpn_backend_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11908517815223722933ull);
    vlSelf->rst_ni = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3161515032326629241ull);
    vlSelf->compare_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6946981713411396870ull);
    vlSelf->nonblock_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7422625687775126606ull);
    vlSelf->ctrl_aw_valid_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4767926665918393110ull);
    vlSelf->ctrl_w_valid_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1039943414536996958ull);
    vlSelf->ctrl_b_ready_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2787734109987783467ull);
    vlSelf->ctrl_ar_valid_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7652176559422903799ull);
    vlSelf->ctrl_r_ready_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12960326182196544059ull);
    vlSelf->ctrl_aw_addr_i = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 14082415255251801908ull);
    vlSelf->ctrl_w_data_i = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 2163894173981867868ull);
    vlSelf->ctrl_ar_addr_i = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8108320811150287773ull);
    vlSelf->ctrl_w_strb_i = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16863602872607454655ull);
    vlSelf->ctrl_aw_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11682546464598226378ull);
    vlSelf->ctrl_w_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17892124471120193145ull);
    vlSelf->ctrl_b_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4338027363594838912ull);
    vlSelf->ctrl_ar_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15004398771938344645ull);
    vlSelf->ctrl_r_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14609776387296902573ull);
    vlSelf->ctrl_r_data_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 497963434351843007ull);
    vlSelf->ctrl_b_resp_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16139795107594685744ull);
    vlSelf->ctrl_r_resp_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10000243589647589581ull);
    vlSelf->equal_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6159084876471489559ull);
    vlSelf->inactive_clean_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16931920923229961062ull);
    vlSelf->permit_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9998672309287941989ull);
    vlSelf->quiet_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9529518669304258198ull);
    vlSelf->certificate_pending_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 516955067011868971ull);
    vlSelf->certificate_fault_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8135269502171244045ull);
    vlSelf->frontend_idle_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6321582965938459894ull);
    vlSelf->job_fire_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10474332437683930443ull);
    vlSelf->job_done_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15136819209581519833ull);
    vlSelf->backend_req_fire_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6204146471534522544ull);
    vlSelf->job_length_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12688190026862005928ull);
    vlSelf->job_src_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15789836798153310274ull);
    vlSelf->job_dst_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15735344721778718015ull);
    vlSelf->ttype_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5808962720196427224ull);
    vlSelf->busy_o = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12830161046862190536ull);
    vlSelf->large_byte_count_o = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 17521597814732890093ull);
    vlSelf->large_in_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13689844110757813502ull);
    vlSelf->large_response_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15762555487901135926ull);
    vlSelf->obs_issue_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13191708369746502714ull);
    vlSelf->obs_done_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5581524496175817668ull);
    vlSelf->batch_active_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5287308549246915302ull);
    vlSelf->obs_issue_id_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16685485044419891681ull);
    vlSelf->obs_done_id_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8451424871679055398ull);
    vlSelf->batch_issued_o = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 13648852492337184738ull);
    vlSelf->batch_completed_o = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 42785502173918696ull);
    vlSelf->ar_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8826234275056903186ull);
    vlSelf->aw_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 257105069740668078ull);
    vlSelf->w_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11482491075140615649ull);
    vlSelf->w_last_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8405095082195231563ull);
    vlSelf->r_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2222536496470009440ull);
    vlSelf->b_ready_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7212989394254339378ull);
    vlSelf->ar_addr_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8730642641781555007ull);
    vlSelf->aw_addr_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 6020836852991476202ull);
    vlSelf->w_data_o = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15341536903361960377ull);
    vlSelf->ar_len_o = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 7352045711063897484ull);
    vlSelf->aw_len_o = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 9089831131807047832ull);
    vlSelf->w_strb_o = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13760786101240862087ull);
    vlSelf->ar_size_o = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3986191859315141819ull);
    vlSelf->aw_size_o = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13231430943609791556ull);
    vlSelf->ar_burst_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7165342887747911418ull);
    vlSelf->aw_burst_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3303062854805588805ull);
    vlSelf->ar_id_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 454730012271102517ull);
    vlSelf->aw_id_o = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3950167163552980520ull);
    vlSelf->ar_sid_o = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 13968655072114415704ull);
    vlSelf->aw_sid_o = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 8312186191675695770ull);
    vlSelf->ar_ready_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 585172666439011520ull);
    vlSelf->aw_ready_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14772586921206578554ull);
    vlSelf->w_ready_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3647745356775050924ull);
    vlSelf->r_valid_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15557017533579234054ull);
    vlSelf->r_last_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 625015004827086926ull);
    vlSelf->b_valid_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7934652909860666365ull);
    vlSelf->r_data_i = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 438151017486138275ull);
    vlSelf->r_id_i = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12921253366776852430ull);
    vlSelf->r_resp_i = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5639098315339253666ull);
    vlSelf->b_id_i = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17392199025407980469ull);
    vlSelf->b_resp_i = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3362853697915182298ull);
    VL_SCOPED_RAND_RESET_W(377, vlSelf->pn_backend_top__DOT__req, __VscopeHash, 206087579318117677ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__rsp, __VscopeHash, 16254895878096998311ull);
    VL_SCOPED_RAND_RESET_W(277, vlSelf->pn_backend_top__DOT__ctrl_req, __VscopeHash, 550376256068458375ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__gold_ctrl_rsp, __VscopeHash, 8381759078115817979ull);
    vlSelf->pn_backend_top__DOT__gold_batch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14070033840545762694ull);
    vlSelf->pn_backend_top__DOT__cfg_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6415534579186203374ull);
    vlSelf->pn_backend_top__DOT__gold_cfg_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18253860020471502803ull);
    VL_SCOPED_RAND_RESET_W(277, vlSelf->pn_backend_top__DOT____VdfgRegularize_h7273d7a5_0_1, __VscopeHash, 16845328385785165089ull);
    VL_SCOPED_RAND_RESET_W(286, vlSelf->pn_backend_top__DOT__candidate__DOT__burst_req, __VscopeHash, 15945704250412154984ull);
    VL_SCOPED_RAND_RESET_W(286, vlSelf->pn_backend_top__DOT__candidate__DOT__burst_req_d, __VscopeHash, 13619135670340033349ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__be_valid_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10101745236827246530ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__be_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15210591915227159916ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->pn_backend_top__DOT__candidate__DOT__dma_regs_req, __VscopeHash, 3582047702809417096ull);
    VL_SCOPED_RAND_RESET_W(66, vlSelf->pn_backend_top__DOT__candidate__DOT__dma_regs_rsp, __VscopeHash, 2966157731427904352ull);
    VL_SCOPED_RAND_RESET_W(286, vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_request, __VscopeHash, 5495204302247791520ull);
    VL_SCOPED_RAND_RESET_W(286, vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_request, __VscopeHash, 9584081936286805653ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_busy = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11709960002088674562ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_axi_rsp, __VscopeHash, 959588264837699309ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_axi_rsp, __VscopeHash, 18253559604898371308ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_req_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5289974287165005158ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_req_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4460691684808745213ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_rsp_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7907316485877299730ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_req_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1612456651394528128ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_req_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15711872413640352071ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_rsp_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15243359719350066479ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_ttype = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6673396924234391931ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_ttype = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6647435857157342953ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_wdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 935026594072470886ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_wdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16917413865685161217ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_small_rdata_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11748710487265380470ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__pn_large_rdata_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7191069768039571728ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT____Vcellinp__gen_pn_backend__DOT__i_large_certificate__normal_copy_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 382521040769211266ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_cnt_d = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 453909982045494781ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_cnt_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12207567168081246858ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_state_d = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3571918586716085222ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_state_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14680727163448114779ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_state_d = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9614932566052996234ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_state_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 11214223541595827057ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__id_d = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2201981440823707359ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__id_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16066977824936856298ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_beats_d = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2226824260612189212ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_beats_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14203896045409385055ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__aw_without_complete_w_downstream = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 799771770495883663ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__complete_w_without_aw_downstream = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15799086848415531771ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd_pop_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1734275769141522334ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4137476433651927969ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3569291541943680476ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13237518146716707369ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18374224121941507562ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8756675528592598342ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12600729792429234520ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17645486165370428058ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2634965358287631525ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT____VdfgRegularize_hb65dfc41_0_7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3707701090683927910ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT____VdfgRegularize_hb65dfc41_0_8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14394876045204946890ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT____VdfgRegularize_hb65dfc41_0_9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18166683548907088456ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9661109105437157426ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7069084481596215501ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5918190049215663711ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16229370846874087580ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4161021901138926849ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5378094709581161237ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__mem_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6610615637045499284ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__mem_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7544169439565192516ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5339584983333450609ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9605053543768436722ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10579850542002998548ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12332642474900376871ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4405992425810981224ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10231786867416952376ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__mem_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4347244171361654856ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__mem_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1439524272339134353ull);
    VL_SCOPED_RAND_RESET_W(136, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_fifo_out, __VscopeHash, 7683869117672245120ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_fifo_push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2315162841047488089ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_fifo_pop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6946702245624843730ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_resp_fifo_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17623583020974140961ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_resp_fifo_push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10441771038920169531ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_fifo_pop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1790887893153358620ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_resp_fifo_out, __VscopeHash, 15480595409736365820ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_resp_fifo_push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6382535265713387646ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__arb_req, __VscopeHash, 1357487080919522701ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8631089061995182305ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17317739628033796289ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT____Vcellout__i_stream_arbiter__oup_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13513235562285175257ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9177018732768836229ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17478574820091128785ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11051886577734034504ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9769273651958856490ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 11846815041317851342ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9736839438894984064ull);
    VL_SCOPED_RAND_RESET_W(272, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__mem_n, __VscopeHash, 8036302482647232699ull);
    VL_SCOPED_RAND_RESET_W(272, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__mem_q, __VscopeHash, 9055964722480913597ull);
    VL_SCOPED_RAND_RESET_W(136, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT____Vlvbound_h2a44f683__0, __VscopeHash, 15349939039989512718ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5561158651996587341ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2637132426859571501ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14590887472792942707ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5709684200268884838ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7463249488557144556ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17788368010857844904ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__mem_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14526720905048692388ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__mem_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6881211022678648498ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2332275133026973661ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18309256811365529316ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14585706380299051297ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5791447359612481161ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9211483180751022399ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4358848978946533837ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__mem_n, __VscopeHash, 6704887391230529023ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__mem_q, __VscopeHash, 12773576463629234242ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6476363582714980773ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9986238247523331878ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9453690623136949437ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17391271965431873044ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10096884290621230615ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3224294457711175333ull);
    VL_SCOPED_RAND_RESET_W(130, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__mem_n, __VscopeHash, 16153879791654705644ull);
    VL_SCOPED_RAND_RESET_W(130, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__mem_q, __VscopeHash, 4423217372246674835ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT____Vlvbound_h3649112d__0, __VscopeHash, 16773995899506497478ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__idx_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18271218289626573503ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__rr_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11848052698401897298ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_d = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16802055049392526960ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1262935701208092312ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2531944422708386442ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7262723376043658637ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__r_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4193726681585713732ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__w_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16277823010896441980ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__r_dp_req_out_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11299412205377618277ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__r_dp_req_out = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8229800323595771652ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__w_dp_req_out = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 11579271645002192458ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__r_dp_rsp_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3793352466217767065ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT____Vcellinp__i_ar_fall_through_register__data_i, __VscopeHash, 6557352565871613689ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT____VdfgRegularize_h3d8a5ec4_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3487340872077931247ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16829075370618688838ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12428601912268655374ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12493126778704792579ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14917577146337243645ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 18151954171173230883ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6300936823095470737ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4364269181192169261ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 5354886400804654760ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 10617023113829725834ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_h32d42547__0 = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 14556004082493258555ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__fifo_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13644776524838848710ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT____Vcellinp__i_fifo__push_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18365980466271081619ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15901361225875584592ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6429384720131866458ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3563452116981233116ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12650215353447221392ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2193063619428096055ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 18170481035160760425ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n, __VscopeHash, 1881196270054629796ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q, __VscopeHash, 5445341196155335404ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT____Vlvbound_h1872ff0c__0, __VscopeHash, 14370970152976300652ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_tf_d, __VscopeHash, 6515787540241728818ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_tf_q, __VscopeHash, 996027071021493740ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_tf_d, __VscopeHash, 16852463838949391705ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_tf_q, __VscopeHash, 2018197783844226149ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__opt_tf_d, __VscopeHash, 11989064946323718657ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__opt_tf_q, __VscopeHash, 14710728114243535070ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_num_bytes_possible = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 15862743101867990238ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_num_bytes_possible = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 13006113965503060863ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT____VdfgRegularize_h95b7fa56_0_4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4145265975440730061ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT____VdfgRegularize_h95b7fa56_0_6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18056409025051199216ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11229067483848075883ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4768839615993387930ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2893164447628167453ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 8559868359974006730ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 18110451288726679671ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17383892168196792833ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4974037763168631238ull);
    VL_SCOPED_RAND_RESET_W(120, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 3425046033698136838ull);
    VL_SCOPED_RAND_RESET_W(120, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 6638284831978356580ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_hed3f0410__0 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 18408553000123028521ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8684552738880352968ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2530067615242589214ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17282853362433086335ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10037338367753339291ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11119277848089734510ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 10300583580524854316ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2966278940675653679ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 1890014949858037713ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 14338805947037101117ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_heebe278f__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8954099791629935081ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__first_r_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3195532977513837260ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__first_r_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9064042426192948128ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__mask_in = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12350368275864163572ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__mask_out = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8417626671891808307ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__last_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2181959496947001759ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_happening = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4911381573414011443ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__ready_to_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3850635765758507194ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_num_beats_d = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3131322584649833757ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_num_beats_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17718906187836895555ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_cnt_valid_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9344140068525758714ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_cnt_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14944656168645556593ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__use_wdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6075682634049884173ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6908734042292419551ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 462849178736994240ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hb2271335_0_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6858774660690846416ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hb2271335_0_3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16587292344986731241ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hb2271335_0_7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10656584871662803682ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hb2271335_0_10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13281264047758064639ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hb2271335_0_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7978965942171414312ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__fifo_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4032304728619730144ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT____Vcellinp__i_fifo__push_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17394447926661663324ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7085576897367678323ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2784834297245633703ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9822100491988806712ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16982847326680448625ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17578749876493100755ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 1027524217798959782ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n, __VscopeHash, 5112771463756852997ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q, __VscopeHash, 14368385147272591067ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__candidate__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT____Vlvbound_h22431c4d__0, __VscopeHash, 13307339166655360973ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__state_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5579026198911086036ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__state_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10710080625943437318ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_counter_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 6114735345053159774ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_counter_n = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16035395745103826521ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_offset_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12139020816935015618ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_offset_n = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16150006576688867387ull);
    VL_SCOPED_RAND_RESET_W(524, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__dma_reg2hw, __VscopeHash, 4817286352666383958ull);
    VL_SCOPED_RAND_RESET_W(194, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__dma_hw2reg, __VscopeHash, 4045932442796530005ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__active_cycles_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5889502897258105963ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__active_cycles_n = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16493778534570459814ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__issue = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18155912068103371293ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_access = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14492511389184060859ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__locked_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5646434906677245573ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_enable_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1718215601114271777ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_stop_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1591954999218973951ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_stopped_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3485119800867314393ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_error_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16278893802733926083ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_target_q = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 12895915818232288036ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_issued_q = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 1796373812185360699ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_completed_q = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 2197267637307309470ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_pages_q = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 3287768286848613875ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_id_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 9827789081005594250ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_cfg_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10699267806050445294ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_task_issue = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4356562416636868063ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_task_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3139063943842240216ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__reg_error = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8369935143825870547ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5555174977127444435ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 4784034640879311599ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 2646349737428413382ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10070307261135533978ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18319418205054099911ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13955055603580030299ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16922554985171939895ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4313168631020519370ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9567687189818451538ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 4276576329507084588ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10885521547197395738ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 14808163604703299393ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__addr_hit = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 15116860314948281075ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_src_addr__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18238165576939278783ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_dst_addr__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12556726049783481725ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_num_bytes__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4167387748687532480ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_conf_cache__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14888635452836377250ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_wdata__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14481255239863219187ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_n_trans__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1988233503182161374ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5805091212203735959ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 6617479706199652542ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 11803615111400575754ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 9922337951612203549ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7462634089046373470ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5231150461585556723ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5302318520591324488ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13424187007192868966ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12093713192077797763ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8187435129692484791ull);
    VL_SCOPED_RAND_RESET_W(2288, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__mem_n, __VscopeHash, 10636462650053001588ull);
    VL_SCOPED_RAND_RESET_W(2288, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__mem_q, __VscopeHash, 2271183672709878283ull);
    VL_SCOPED_RAND_RESET_W(286, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT____Vlvbound_h87dcdb97__0, __VscopeHash, 18366676237280075179ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__r_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2922217460264669359ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__w_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17054123995095509792ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__r_dp_req_out = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 9502619352568515090ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__w_dp_req_out = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 7227592304001768707ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT____Vcellinp__i_ar_fall_through_register__data_i, __VscopeHash, 13580855648607680734ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9702201977533370706ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14012161575569748936ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 6706425081800073442ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3507473273862186870ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5909265630399875625ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2337490717821289494ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2023307548007810380ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 2734737396084964831ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 532498010006690943ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_h3e0a40d6__0 = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 14960601368957009876ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__fifo_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7608972996278098887ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT____Vcellinp__i_fifo__push_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10215507454921848019ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1598843216591857942ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1033464921973781862ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16058531359241788645ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12314842811014420962ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4905066581528233523ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7299570910350148307ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n, __VscopeHash, 2925112385976897709ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q, __VscopeHash, 2168987270696298738ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT____Vlvbound_h1872ff0c__0, __VscopeHash, 10372167116955102411ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_tf_d, __VscopeHash, 9365277006429867453ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_tf_q, __VscopeHash, 10652882342401596783ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_tf_d, __VscopeHash, 5368083545146029060ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_tf_q, __VscopeHash, 14453492545186915785ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__opt_tf_d, __VscopeHash, 8762744275336857870ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__opt_tf_q, __VscopeHash, 7745482750511239111ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_num_bytes_possible = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 9196795572405457067ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_num_bytes_possible = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 4660021583248138061ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT____VdfgRegularize_hdf94cac3_0_4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11030463030435603170ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT____VdfgRegularize_hdf94cac3_0_6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15701233060082926324ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15367084833311145904ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 8334180698782796670ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 827820116685607439ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 16907717969297084204ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11105547027767640766ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4224437274718640031ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11431506683194868520ull);
    VL_SCOPED_RAND_RESET_W(120, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 10317873484748695245ull);
    VL_SCOPED_RAND_RESET_W(120, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 6686559487516611295ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_hc45dd099__0 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 6463405580122687069ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16664459575474194585ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 5336532102564251802ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 497215630065166824ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 18260379085489660887ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 11069597676239183163ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(11, __VscopeHash, 9003782893317516832ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(11, __VscopeHash, 14493919291910333595ull);
    VL_SCOPED_RAND_RESET_W(1040, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 14852045884672504375ull);
    VL_SCOPED_RAND_RESET_W(1040, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 2484306059522156580ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_h3e7c2f51__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5334674440367614877ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__first_r_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13803717315085280409ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__first_r_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8567993759686669632ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__mask_out = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16723149442537167860ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__last_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17543271470018219097ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__buffer_out_valid = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6716864502146565274ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__ready_to_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5965632993230097650ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_num_beats_d = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2021070394570083522ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_num_beats_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8255012411352861958ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_cnt_valid_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15084027009080370110ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_cnt_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10933975232268144433ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__use_wdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 114674492465990955ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7723799027190500872ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10415740946120341812ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdbdbeed1_0_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9770323330509420641ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdbdbeed1_0_3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2215142429800395171ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdbdbeed1_0_10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16746537280858992286ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdbdbeed1_0_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17317465495100265982ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 5323780414107686963ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 13985216540591493478ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 16139565156006030009ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 72586661680270875ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 12668975647970132966ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 11101385985119889886ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 7478070249204227146ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__0__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 5666501011933252548ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 15782553100406900363ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 1930257864754299860ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 14233586646492545569ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 1667661303171938676ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 3008037799387370942ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8851827958497515440ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 5110007458778807582ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__1__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 1282791434519219858ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 7544657816343009301ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 4020561778608934987ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 4349408050712946045ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 14955653696329635014ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 12429242719812544704ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 10317109681572048583ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 12415354937136379621ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__2__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 15798379139460176149ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 12665593559073915760ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 18325231274431867589ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 6227047067449692376ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 2779124117327541000ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 6844614665088672246ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 17093190765094757953ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 11480497924530169137ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__3__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 1256448684732223325ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 8719086815219047260ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 9737126483145562281ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 17848871214450823832ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 16918360882043812184ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 3463451141541133812ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 9294266293203067793ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 2806866033961534242ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__4__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 15320591227570529688ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 8883523240984596518ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 17975108073992910491ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 473995493528710824ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 5366607767239646174ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 6833037664058413912ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 6339221532591676162ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 14991611917739431576ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__5__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 11244563913111046098ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 7926200245703193240ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 858854700443581230ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 802051485227153193ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 1406526757354435621ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 6455024047583941412ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 7136340271790225360ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 15455597522401167372ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__6__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 1694625057420410965ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 8114349742113927402ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 2460714572299520529ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 13471299669866433409ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 1083512752960765607ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 12834001440455133642ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8528765620564788717ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 10154247380728764732ull);
    VL_SCOPED_RAND_RESET_W(4096, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__i_idma_buffer__DOT__gen_fifo_buffer__BRA__7__KET____DOT__i_byte_buffer__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 7644835306343942695ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__fifo_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6313051143219517270ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT____Vcellinp__i_fifo__push_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 862558282679731137ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4538195740140108617ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8928049855583654592ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16554544478019577275ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7400065440474199051ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6002914581789450586ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2522340148297173566ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n, __VscopeHash, 7791309918303919095ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q, __VscopeHash, 11926474412978497008ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_idma_backend_fullbuffer__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT____Vlvbound_h22431c4d__0, __VscopeHash, 15852036622993689451ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_small_certificate__DOT__pending_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16286443458269506581ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_small_certificate__DOT__fault_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4489763940746919377ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_large_certificate__DOT__pending_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2662191367744496069ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_large_certificate__DOT__eligible_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13270484199672280151ull);
    vlSelf->pn_backend_top__DOT__candidate__DOT__gen_pn_backend__DOT__i_large_certificate__DOT__fault_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2164155333016707084ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__be_valid_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6825901056056246119ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__be_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11294447445523690368ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__idma_busy = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13587366821812900027ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__idma_rsp_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16191985756424012645ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->pn_backend_top__DOT__reference_dut__DOT__dma_regs_req, __VscopeHash, 13727983929648250059ull);
    VL_SCOPED_RAND_RESET_W(66, vlSelf->pn_backend_top__DOT__reference_dut__DOT__dma_regs_rsp, __VscopeHash, 1188069716563654572ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__rdata_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16506534881073604048ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_cnt_d = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10843383608313294291ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_cnt_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1995994820977571568ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_state_d = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11668301264281592952ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__w_state_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7608415343393240937ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_state_d = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 11288731568948544024ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_state_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4884559844016574550ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__id_d = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 148286186051723911ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__id_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5995170411508495737ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_beats_d = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14413868511600249162ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_beats_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6980325986711687182ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__aw_without_complete_w_downstream = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1248299100148515337ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__complete_w_without_aw_downstream = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14694734724603664692ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd_pop_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4364492765052724742ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11560574903202654095ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8813655007435734307ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8068813047161894648ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9928015756430103208ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16782880288308349065ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14429613863434342603ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8548280983619240363ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_atop_filter__DOT__r_resp_cmd__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4943120691300470347ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT____VdfgRegularize_h4fe9e189_0_7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3554151364805317250ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT____VdfgRegularize_h4fe9e189_0_8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12584381361136684493ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT____VdfgRegularize_h4fe9e189_0_9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16743749647048067428ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13152150367695484822ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9549799374791627396ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16450735956909429046ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10518402681618415360ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14894308110129520636ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 18106241895512248252ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__mem_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15642353981466601798ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_aw_id_fifo__DOT__mem_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14604269904843208164ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11197636355791395665ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 66798091974959570ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13948396532250019344ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13621887982798365241ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3389291517851457948ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5678243266136055755ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__mem_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14934152919818397319ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_to_axi_lite_id_reflect__DOT__i_ar_id_fifo__DOT__mem_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15949608184078272978ull);
    VL_SCOPED_RAND_RESET_W(136, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_fifo_out, __VscopeHash, 15244847551180892012ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_fifo_push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7932086054523149821ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_fifo_pop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5030123597196449191ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_resp_fifo_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11905341197747403685ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_resp_fifo_push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14288371031620190846ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_fifo_pop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 307300705016168635ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_resp_fifo_out, __VscopeHash, 6078893586278607663ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_resp_fifo_push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14064789578700896083ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__arb_req, __VscopeHash, 1181672542922573409ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__read_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11140385555074550352ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__write_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17808077681420953270ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT____Vcellout__i_stream_arbiter__oup_valid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10316850994443386337ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12274589350778126823ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11213445653301526211ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1935413578813256446ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15419959091585969246ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 18226437956939228897ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7864753648652963634ull);
    VL_SCOPED_RAND_RESET_W(272, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__mem_n, __VscopeHash, 5850283105964468499ull);
    VL_SCOPED_RAND_RESET_W(272, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT__mem_q, __VscopeHash, 9105312429618567882ull);
    VL_SCOPED_RAND_RESET_W(136, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_req__DOT____Vlvbound_h666404ce__0, __VscopeHash, 17897845484850047828ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10888181042132367754ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13111722604164295701ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18003052817890655575ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11023956026239729210ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17149291599962800369ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10369478771499969053ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__mem_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13561099389664051989ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_write_resp__DOT__mem_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9191744869722628620ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12925267690091463051ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13069710881955603080ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3759069238547366112ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4279645277839223154ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3755884130010584380ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12121429026950656838ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__mem_n, __VscopeHash, 9328658496145590113ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read__DOT__mem_q, __VscopeHash, 192655481746682716ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10370108716573157454ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9979757682920024200ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4248008276666525484ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8589794274199966395ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 18227142239040782064ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9032490880651062378ull);
    VL_SCOPED_RAND_RESET_W(130, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__mem_n, __VscopeHash, 16264906562596808196ull);
    VL_SCOPED_RAND_RESET_W(130, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT__mem_q, __VscopeHash, 17072014490604478567ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_fifo_read_resp__DOT____Vlvbound_h240674ce__0, __VscopeHash, 8715635809072325999ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__idx_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10023840506599229347ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__rr_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3360668445187628806ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__req_d = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7850740529267771644ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__rr_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10117709428666423237ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__lock_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 565421414119478963ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_lite_to_reg__DOT__i_stream_arbiter__DOT__i_arb__DOT__gen_rr_arb__DOT__i_arbiter__DOT__gen_arbiter__DOT__gen_int_rr__DOT__gen_lock__DOT__req_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14022658979130141754ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__r_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1634248311839763700ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__w_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8138237880957284555ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__r_dp_req_out_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8501978606009005406ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__r_dp_req_out = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 13157877542755965789ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__w_dp_req_out = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 8113490791241998040ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT____Vcellinp__i_ar_fall_through_register__data_i, __VscopeHash, 3494053551912465479ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT____VdfgRegularize_h243fe997_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3773335393035979367ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8379183223907322609ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4193698815439507165ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14402710750311545419ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 198872780971193276ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11763059869220555589ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15784819541104927126ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4309115027428109019ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 17508707280622357012ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 10925214528101065103ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_r_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_h88f07df4__0 = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 11294306815277698362ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__fifo_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13869678620150406588ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT____Vcellinp__i_fifo__push_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9981741423561788299ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2026341729406510167ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12972369022311782244ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1604486587078579356ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6409924512157205131ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9405474753110916472ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10532199171932735608ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n, __VscopeHash, 1474556938750866830ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q, __VscopeHash, 13207618565507235726ull);
    VL_SCOPED_RAND_RESET_W(146, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__i_ar_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT____Vlvbound_h02b19cb3__0, __VscopeHash, 11107528686067408874ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_tf_d, __VscopeHash, 8490589819369129373ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__r_tf_q, __VscopeHash, 15139715207513709746ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_tf_d, __VscopeHash, 2974346366676630462ull);
    VL_SCOPED_RAND_RESET_W(129, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__w_tf_q, __VscopeHash, 10598015727720955251ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__opt_tf_d, __VscopeHash, 11877161674513054998ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_hw_legalizer__DOT__i_idma_legalizer__DOT__opt_tf_q, __VscopeHash, 15345453182046432328ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17847235612678010910ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4546640136528944592ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4998586856472559137ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13793816592693412795ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2947100398688608627ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2616332507051476184ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7155178347087218680ull);
    VL_SCOPED_RAND_RESET_W(120, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n, __VscopeHash, 13019754616192913865ull);
    VL_SCOPED_RAND_RESET_W(120, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q, __VscopeHash, 12543031130414621926ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_w_dp_fifo__DOT__i_w_dp_req__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_h565ecc70__0 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 8938305127918685563ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__push = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11417251445814907881ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9216674797863235810ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9337671280891200715ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5790772528624754597ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12810672904194092766ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 17979917203017609475ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15489643577015049508ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_n = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 2516830581547735131ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT__mem_q = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 10032940626205196736ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_last_flag_fifo__DOT__i_w_last__DOT__gen_fifo__DOT__i_stream_fifo__DOT__fifo_i__DOT____Vlvbound_heebe278f__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17414773959314693506ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__first_r_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8372835800865679094ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__first_r_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2491280387787611845ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__mask_in = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12104647187367847105ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__mask_out = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 794808108491136009ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__last_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12331734772233000482ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_happening = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6244437687453498311ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__ready_to_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14781577006625823328ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_num_beats_d = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8629080780418845253ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_num_beats_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3187052365602019517ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_cnt_valid_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2747390335932856888ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__w_cnt_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8908321608622646148ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__use_wdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4883833968502414408ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2080550450309316314ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT__write_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7311783972679763631ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdd4cffa1_0_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2453931505966310588ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdd4cffa1_0_3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14456478640231824107ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdd4cffa1_0_7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18090359743497402719ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdd4cffa1_0_10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 634606312518582253ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_axi_transport_layer__DOT__i_idma_axi_transport_layer__DOT____VdfgRegularize_hdd4cffa1_0_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17414544452720452360ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__fifo_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9504103680673761431ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT____Vcellinp__i_fifo__push_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13237200676706142099ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18176718914035812076ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12803001883405336983ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1723120126901047075ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11920940808376372481ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10591381491826269729ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5011663398639785301ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_n, __VscopeHash, 17888213389634555298ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT__mem_q, __VscopeHash, 11348669779884189168ull);
    VL_SCOPED_RAND_RESET_W(152, vlSelf->pn_backend_top__DOT__reference_dut__DOT__i_idma_backend__DOT__gen_r_aw_bypass__DOT__i_aw_fall_through_register__DOT__i_fifo__DOT__i_fifo_v3__DOT____Vlvbound_hca657921__0, __VscopeHash, 1640769233232896972ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__state_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7500846590685318875ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__state_n = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2985748665311289093ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_counter_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 17245382939380860720ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_counter_n = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 13444727466253077760ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_offset_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 7568146647602120442ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__transf_offset_n = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12144302063951407186ull);
    VL_SCOPED_RAND_RESET_W(524, vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__dma_reg2hw, __VscopeHash, 16978855399282576550ull);
    VL_SCOPED_RAND_RESET_W(194, vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__dma_hw2reg, __VscopeHash, 15319715557366272848ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__active_cycles_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3387696712199077347ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__active_cycles_n = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 9343610009964764384ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__issue = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12393159207731719548ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__trans_complete = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1189666659170792285ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_access = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3007842912983928271ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__locked_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17353777044041235957ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_enable_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12946962641588265006ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_stop_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9825178219451010440ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_stopped_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10226969395825979332ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_error_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8123017883173810127ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_target_q = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 7013709035472635030ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_issued_q = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 10685642973046015536ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_completed_q = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 13919040535469741836ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_pages_q = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 5466387712010032526ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_id_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16568842994835350939ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_cfg_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4271288812277712345ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_task_issue = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6398985293762042666ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__batch_task_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12067267575992214448ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__reg_error = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4298533892665406017ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_src_addr__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 9013690430094754950ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_dst_addr__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 14097947772577071979ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_num_bytes__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 13070248985378313622ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_decouple__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1450598205319553031ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_deburst__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4644935114268105267ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_serialize__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12596265147463839189ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_pv__q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14127147682390068837ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_ttype__q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16725482540334022266ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_conf_cache__q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17574617388695907039ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_rdata__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1712555907480192699ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_wdata__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12766662956285381647ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT____Vcellout__u_n_trans__q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1821809451969644960ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__addr_hit = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 14041211810265985168ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_src_addr__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10184997810278297359ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_dst_addr__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4361272275841480650ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_num_bytes__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15941452684976493333ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_conf_cache__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16289095166363317338ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_wdata__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15159685842824963471ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_dma_conf_regs__DOT__u_n_trans__DOT__wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15782301617467981828ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__next_d = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 4521269766269639987ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__next_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16057588867371169757ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_d = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8318981259656723501ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_dma_frontend__DOT__i_idma_transfer_id_gen__DOT__completed_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3142423194602448872ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__read_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 28807260914516286ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__read_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4979970499277226219ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__write_pointer_n = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17125009258433735205ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__write_pointer_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15135387941676360768ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__status_cnt_n = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16536669963433519289ull);
    vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__status_cnt_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6598606877161622585ull);
    VL_SCOPED_RAND_RESET_W(2288, vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__mem_n, __VscopeHash, 3972295657664375082ull);
    VL_SCOPED_RAND_RESET_W(2288, vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT__mem_q, __VscopeHash, 6763445292738418738ull);
    VL_SCOPED_RAND_RESET_W(286, vlSelf->pn_backend_top__DOT__reference_dut__DOT__gen_one_d__DOT__i_stream_fifo_jobs_oned__DOT__fifo_i__DOT____Vlvbound_h40a1a9c0__0, __VscopeHash, 15435172438844417496ull);
    VL_SCOPED_RAND_RESET_W(174, vlSelf->__VdfgRegularize_h3c5bdd75_0_0, __VscopeHash, 17939933138940668268ull);
    VL_SCOPED_RAND_RESET_W(97, vlSelf->__VdfgRegularize_h3c5bdd75_0_1, __VscopeHash, 8906987226437233892ull);
    vlSelf->__VdfgRegularize_h2dd05df0_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3842117083664160541ull);
    vlSelf->__VdfgRegularize_h2dd05df0_0_1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6296288115256680902ull);
    vlSelf->__VdfgRegularize_h2dd05df0_0_12 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14897759835722064013ull);
    vlSelf->__VdfgRegularize_h2dd05df0_0_15 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13933004223437115030ull);
    vlSelf->__VdfgRegularize_h2dd05df0_0_27 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1148190084401169144ull);
    VL_SCOPED_RAND_RESET_W(155, vlSelf->__VdfgRegularize_h2dd05df0_0_31, __VscopeHash, 2933871226299315797ull);
    VL_SCOPED_RAND_RESET_W(155, vlSelf->__VdfgRegularize_h2dd05df0_0_32, __VscopeHash, 2866447726394596122ull);
    VL_SCOPED_RAND_RESET_W(155, vlSelf->__VdfgRegularize_h2dd05df0_0_35, __VscopeHash, 18392816692170733555ull);
    vlSelf->__VdfgRegularize_h495687df_0_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2023296356072706623ull);
    vlSelf->__VdfgRegularize_h495687df_0_14 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3132856617443542949ull);
    vlSelf->__VdfgRegularize_h495687df_0_15 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2068239855133095956ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->__VdfgRegularize_h495687df_0_25, __VscopeHash, 8186347004413078833ull);
    vlSelf->__VdfgRegularize_h495687df_0_39 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4379853144275189801ull);
    vlSelf->__VdfgRegularize_h495687df_0_40 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11447087963953426017ull);
    vlSelf->__VdfgRegularize_h495687df_0_41 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10608220136787683572ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->__VdfgRegularize_h495687df_0_51, __VscopeHash, 5711378586406614706ull);
    VL_SCOPED_RAND_RESET_W(174, vlSelf->__VdfgRegularize_h495687df_0_126, __VscopeHash, 7608589870039295970ull);
    vlSelf->__VdfgRegularize_h495687df_0_251 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8383308987959242458ull);
    vlSelf->__VdfgRegularize_h495687df_0_253 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3234753764292520671ull);
    vlSelf->__VdfgRegularize_h495687df_0_254 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9318793900071628034ull);
    vlSelf->__VdfgRegularize_h495687df_0_255 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14988104030211780749ull);
    vlSelf->__VdfgRegularize_h495687df_0_256 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15055814814002446475ull);
    vlSelf->__VdfgRegularize_h495687df_0_257 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6167377653259195863ull);
    vlSelf->__VdfgRegularize_h495687df_0_258 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15153884408902089362ull);
    vlSelf->__VdfgRegularize_h495687df_0_259 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15567381143335893996ull);
    vlSelf->__VdfgRegularize_h495687df_0_260 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4337508868340679249ull);
    vlSelf->__VdfgRegularize_h495687df_0_261 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1752375716971487841ull);
    vlSelf->__VdfgRegularize_h495687df_0_262 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9690243296296433841ull);
    vlSelf->__VdfgRegularize_h495687df_0_266 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13861418153362422937ull);
    vlSelf->__VdfgRegularize_h495687df_0_267 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13189775317221284169ull);
    vlSelf->__VdfgRegularize_h495687df_0_268 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3176794332429309304ull);
    vlSelf->__VdfgRegularize_h495687df_0_269 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14840354912993587557ull);
    vlSelf->__VdfgRegularize_h495687df_0_270 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3464727244694783020ull);
    vlSelf->__VdfgRegularize_h495687df_0_271 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7699958143563621738ull);
    vlSelf->__VdfgRegularize_h495687df_0_272 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9925769456700013985ull);
    vlSelf->__VdfgRegularize_h495687df_0_273 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10300244829598562712ull);
    vlSelf->__VdfgRegularize_h495687df_0_275 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2204201886897010836ull);
    vlSelf->__VdfgRegularize_h495687df_0_276 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7422461402635145271ull);
    vlSelf->__VdfgRegularize_h495687df_0_277 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3056219584759280326ull);
    vlSelf->__VdfgRegularize_h495687df_0_278 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8055124953123834971ull);
    vlSelf->__VdfgRegularize_h495687df_0_280 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16400333294969153617ull);
    vlSelf->__VdfgRegularize_h495687df_0_281 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17363900329548043044ull);
    vlSelf->__VdfgRegularize_h495687df_0_282 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16773137525443678919ull);
    vlSelf->__VdfgRegularize_h495687df_0_283 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17796143404537798365ull);
    vlSelf->__VdfgRegularize_h495687df_0_284 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7724322312867319119ull);
    vlSelf->__VdfgRegularize_h495687df_0_285 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17022564489196067137ull);
    vlSelf->__VdfgRegularize_h495687df_0_286 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16142809807049206700ull);
    vlSelf->__VdfgRegularize_h495687df_0_287 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7713570961458723701ull);
    vlSelf->__VdfgRegularize_h495687df_0_288 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 822137544400112436ull);
    vlSelf->__VdfgRegularize_h495687df_0_289 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15120708506755286685ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13758589923670249790ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14550703444623006727ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6668014587303127360ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies__0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2212838738493889967ull);
    vlSelf->__VstlDidInit = 0;
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies__1 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16852571618127421740ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies__1 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15849184848692235798ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies__1 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15730963340762775588ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies__1 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13363116386557642954ull);
    vlSelf->__VicoDidInit = 0;
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies__2 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17049179691057347530ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__candidate__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies__2 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5297627208416255553ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_b_readies__2 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9224689713995294087ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter____PVT__i_demux_supported_vs_unsupported__DOT__i_demux_simple__DOT__genblk1__DOT__mst_r_readies__2 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7541335087615660217ull);
    vlSelf->__Vtrigprevexpr___TOP__clk_i__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12668644789224949103ull);
    vlSelf->__Vtrigprevexpr___TOP__rst_ni__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2605432789578622122ull);
    vlSelf->__Vtrigprevexpr___TOP__pn_backend_top__DOT__reference_dut__DOT__i_axi_translate__DOT__i_axi_to_axi_lite__DOT__i_axi_burst_splitter__rst_ni__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15245610173713916412ull);
    vlSelf->__VactDidInit = 0;
}
