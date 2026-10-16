// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_observer.h for the primary calling header

#include "Vdma_research_observer.h"
#include "Vdma_research_observer__Syms.h"

//==========

void Vdma_research_observer::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdma_research_observer::eval\n"); );
    Vdma_research_observer__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_observer.sv", 21, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdma_research_observer::_eval_initial_loop(Vdma_research_observer__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_observer.sv", 21, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_research_observer::_combo__TOP__2(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_combo__TOP__2\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__lookup_v_i 
        = ((IData)(vlTOPp->lookup_i) & (0xaU == vlTOPp->lookup_did_i));
    vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i 
        = ((IData)(vlTOPp->select_fire_i) & (0xaU == vlTOPp->select_did_i));
    vlTOPp->dma_research_observer__DOT__sel_known = 
        ((0xaU == vlTOPp->select_did_i) | (0xbU == vlTOPp->select_did_i));
    vlTOPp->dma_research_observer__DOT__main_known 
        = ((0xaU == vlTOPp->ctl_did_i) | (0xbU == vlTOPp->ctl_did_i));
    vlTOPp->dma_research_observer__DOT__lu_known = 
        ((0xaU == vlTOPp->lookup_did_i) | (0xbU == vlTOPp->lookup_did_i));
    vlTOPp->dma_research_observer__DOT__main_cyc = 
        ((1U == (IData)(vlTOPp->ctl_state_i)) | (2U 
                                                 == (IData)(vlTOPp->ctl_state_i)));
    vlTOPp->dma_research_observer__DOT__changed_mask = 0U;
    if ((0U != ((((vlTOPp->entries_i[0U][0U] ^ vlTOPp->entries_next_i
                   [0U][0U]) | (vlTOPp->entries_i[0U][1U] 
                                ^ vlTOPp->entries_next_i
                                [0U][1U])) | (vlTOPp->entries_i
                                              [0U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [0U][2U])) 
                | (vlTOPp->entries_i[0U][3U] ^ vlTOPp->entries_next_i
                   [0U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (1U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[1U][0U] ^ vlTOPp->entries_next_i
                   [1U][0U]) | (vlTOPp->entries_i[1U][1U] 
                                ^ vlTOPp->entries_next_i
                                [1U][1U])) | (vlTOPp->entries_i
                                              [1U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [1U][2U])) 
                | (vlTOPp->entries_i[1U][3U] ^ vlTOPp->entries_next_i
                   [1U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (2U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[2U][0U] ^ vlTOPp->entries_next_i
                   [2U][0U]) | (vlTOPp->entries_i[2U][1U] 
                                ^ vlTOPp->entries_next_i
                                [2U][1U])) | (vlTOPp->entries_i
                                              [2U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [2U][2U])) 
                | (vlTOPp->entries_i[2U][3U] ^ vlTOPp->entries_next_i
                   [2U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[3U][0U] ^ vlTOPp->entries_next_i
                   [3U][0U]) | (vlTOPp->entries_i[3U][1U] 
                                ^ vlTOPp->entries_next_i
                                [3U][1U])) | (vlTOPp->entries_i
                                              [3U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [3U][2U])) 
                | (vlTOPp->entries_i[3U][3U] ^ vlTOPp->entries_next_i
                   [3U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (8U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[4U][0U] ^ vlTOPp->entries_next_i
                   [4U][0U]) | (vlTOPp->entries_i[4U][1U] 
                                ^ vlTOPp->entries_next_i
                                [4U][1U])) | (vlTOPp->entries_i
                                              [4U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [4U][2U])) 
                | (vlTOPp->entries_i[4U][3U] ^ vlTOPp->entries_next_i
                   [4U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x10U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[5U][0U] ^ vlTOPp->entries_next_i
                   [5U][0U]) | (vlTOPp->entries_i[5U][1U] 
                                ^ vlTOPp->entries_next_i
                                [5U][1U])) | (vlTOPp->entries_i
                                              [5U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [5U][2U])) 
                | (vlTOPp->entries_i[5U][3U] ^ vlTOPp->entries_next_i
                   [5U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[6U][0U] ^ vlTOPp->entries_next_i
                   [6U][0U]) | (vlTOPp->entries_i[6U][1U] 
                                ^ vlTOPp->entries_next_i
                                [6U][1U])) | (vlTOPp->entries_i
                                              [6U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [6U][2U])) 
                | (vlTOPp->entries_i[6U][3U] ^ vlTOPp->entries_next_i
                   [6U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x40U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[7U][0U] ^ vlTOPp->entries_next_i
                   [7U][0U]) | (vlTOPp->entries_i[7U][1U] 
                                ^ vlTOPp->entries_next_i
                                [7U][1U])) | (vlTOPp->entries_i
                                              [7U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [7U][2U])) 
                | (vlTOPp->entries_i[7U][3U] ^ vlTOPp->entries_next_i
                   [7U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x80U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[8U][0U] ^ vlTOPp->entries_next_i
                   [8U][0U]) | (vlTOPp->entries_i[8U][1U] 
                                ^ vlTOPp->entries_next_i
                                [8U][1U])) | (vlTOPp->entries_i
                                              [8U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [8U][2U])) 
                | (vlTOPp->entries_i[8U][3U] ^ vlTOPp->entries_next_i
                   [8U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x100U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[9U][0U] ^ vlTOPp->entries_next_i
                   [9U][0U]) | (vlTOPp->entries_i[9U][1U] 
                                ^ vlTOPp->entries_next_i
                                [9U][1U])) | (vlTOPp->entries_i
                                              [9U][2U] 
                                              ^ vlTOPp->entries_next_i
                                              [9U][2U])) 
                | (vlTOPp->entries_i[9U][3U] ^ vlTOPp->entries_next_i
                   [9U][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[0xaU][0U] ^ vlTOPp->entries_next_i
                   [0xaU][0U]) | (vlTOPp->entries_i
                                  [0xaU][1U] ^ vlTOPp->entries_next_i
                                  [0xaU][1U])) | (vlTOPp->entries_i
                                                  [0xaU][2U] 
                                                  ^ 
                                                  vlTOPp->entries_next_i
                                                  [0xaU][2U])) 
                | (vlTOPp->entries_i[0xaU][3U] ^ vlTOPp->entries_next_i
                   [0xaU][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x400U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[0xbU][0U] ^ vlTOPp->entries_next_i
                   [0xbU][0U]) | (vlTOPp->entries_i
                                  [0xbU][1U] ^ vlTOPp->entries_next_i
                                  [0xbU][1U])) | (vlTOPp->entries_i
                                                  [0xbU][2U] 
                                                  ^ 
                                                  vlTOPp->entries_next_i
                                                  [0xbU][2U])) 
                | (vlTOPp->entries_i[0xbU][3U] ^ vlTOPp->entries_next_i
                   [0xbU][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x800U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[0xcU][0U] ^ vlTOPp->entries_next_i
                   [0xcU][0U]) | (vlTOPp->entries_i
                                  [0xcU][1U] ^ vlTOPp->entries_next_i
                                  [0xcU][1U])) | (vlTOPp->entries_i
                                                  [0xcU][2U] 
                                                  ^ 
                                                  vlTOPp->entries_next_i
                                                  [0xcU][2U])) 
                | (vlTOPp->entries_i[0xcU][3U] ^ vlTOPp->entries_next_i
                   [0xcU][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[0xdU][0U] ^ vlTOPp->entries_next_i
                   [0xdU][0U]) | (vlTOPp->entries_i
                                  [0xdU][1U] ^ vlTOPp->entries_next_i
                                  [0xdU][1U])) | (vlTOPp->entries_i
                                                  [0xdU][2U] 
                                                  ^ 
                                                  vlTOPp->entries_next_i
                                                  [0xdU][2U])) 
                | (vlTOPp->entries_i[0xdU][3U] ^ vlTOPp->entries_next_i
                   [0xdU][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x2000U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[0xeU][0U] ^ vlTOPp->entries_next_i
                   [0xeU][0U]) | (vlTOPp->entries_i
                                  [0xeU][1U] ^ vlTOPp->entries_next_i
                                  [0xeU][1U])) | (vlTOPp->entries_i
                                                  [0xeU][2U] 
                                                  ^ 
                                                  vlTOPp->entries_next_i
                                                  [0xeU][2U])) 
                | (vlTOPp->entries_i[0xeU][3U] ^ vlTOPp->entries_next_i
                   [0xeU][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x4000U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    if ((0U != ((((vlTOPp->entries_i[0xfU][0U] ^ vlTOPp->entries_next_i
                   [0xfU][0U]) | (vlTOPp->entries_i
                                  [0xfU][1U] ^ vlTOPp->entries_next_i
                                  [0xfU][1U])) | (vlTOPp->entries_i
                                                  [0xfU][2U] 
                                                  ^ 
                                                  vlTOPp->entries_next_i
                                                  [0xfU][2U])) 
                | (vlTOPp->entries_i[0xfU][3U] ^ vlTOPp->entries_next_i
                   [0xfU][3U])))) {
        vlTOPp->dma_research_observer__DOT__changed_mask 
            = (0x8000U | (IData)(vlTOPp->dma_research_observer__DOT__changed_mask));
    }
    vlTOPp->dma_research_observer__DOT__changed_count = 0U;
    vlTOPp->dma_research_observer__DOT__changed_index = 0U;
    if ((1U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0U;
    }
    if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 1U;
    }
    if ((4U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 2U;
    }
    if ((8U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 3U;
    }
    if ((0x10U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 4U;
    }
    if ((0x20U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 5U;
    }
    if ((0x40U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 6U;
    }
    if ((0x80U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 7U;
    }
    if ((0x100U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 8U;
    }
    if ((0x200U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 9U;
    }
    if ((0x400U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0xaU;
    }
    if ((0x800U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0xbU;
    }
    if ((0x1000U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0xcU;
    }
    if ((0x2000U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0xdU;
    }
    if ((0x4000U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0xeU;
    }
    if ((0x8000U & (IData)(vlTOPp->dma_research_observer__DOT__changed_mask))) {
        vlTOPp->dma_research_observer__DOT__changed_count 
            = (0x1fU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__changed_count)));
        vlTOPp->dma_research_observer__DOT__changed_index = 0xfU;
    }
    vlTOPp->dma_research_observer__DOT__cap_found = 0U;
    vlTOPp->dma_research_observer__DOT__cap_bad_geom = 0U;
    vlTOPp->dma_research_observer__DOT__cap_key[0U] = 0U;
    vlTOPp->dma_research_observer__DOT__cap_key[1U] = 0U;
    vlTOPp->dma_research_observer__DOT__cap_key[2U] = 0U;
    vlTOPp->dma_research_observer__DOT__cap_key[3U] = 0U;
    if (vlTOPp->lookup_i) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[1U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[1U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[1U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[1U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[1U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[1U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[1U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[1U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[1U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[1U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[1U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[1U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[1U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[2U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[2U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[2U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[2U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[2U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[2U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[2U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[2U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[2U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[2U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[2U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[2U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[2U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[3U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[3U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[3U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[3U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[3U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[3U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[3U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[3U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[3U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[3U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[3U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[3U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[3U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[4U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[4U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[4U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[4U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[4U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[4U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[4U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[4U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[4U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[4U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[4U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[4U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[4U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[5U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[5U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[5U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[5U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[5U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[5U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[5U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[5U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[5U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[5U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[5U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[5U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[5U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[6U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[6U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[6U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[6U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[6U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[6U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[6U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[6U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[6U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[6U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[6U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[6U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[6U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[7U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[7U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[7U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[7U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[7U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[7U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[7U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[7U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[7U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[7U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[7U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[7U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[7U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[8U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[8U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[8U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[8U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[8U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[8U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[8U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[8U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[8U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[8U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[8U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[8U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[8U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[9U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[9U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[9U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[9U][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[9U][1U] >> 0x13U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[9U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[9U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[9U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[9U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[9U][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[9U][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[9U][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[9U][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0xaU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0xaU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0xaU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0xaU][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0xaU][1U] >> 0x13U) 
             & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0xaU][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0xaU][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0xaU][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0xaU][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0xaU][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0xaU][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0xaU][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0xaU][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0xbU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0xbU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0xbU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0xbU][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0xbU][1U] >> 0x13U) 
             & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0xbU][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0xbU][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0xbU][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0xbU][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0xbU][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0xbU][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0xbU][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0xbU][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0xcU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0xcU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0xcU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0xcU][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0xcU][1U] >> 0x13U) 
             & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0xcU][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0xcU][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0xcU][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0xcU][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0xcU][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0xcU][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0xcU][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0xcU][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0xdU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0xdU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0xdU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0xdU][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0xdU][1U] >> 0x13U) 
             & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0xdU][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0xdU][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0xdU][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0xdU][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0xdU][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0xdU][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0xdU][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0xdU][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0xeU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0xeU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0xeU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0xeU][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0xeU][1U] >> 0x13U) 
             & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0xeU][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0xeU][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0xeU][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0xeU][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0xeU][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0xeU][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0xeU][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0xeU][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U] = 0U;
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                         >> 0xcU))));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U] 
            = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U]) 
               | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                 << 0x1dU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                 >> 3U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                 << 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]) 
               | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                 << 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U] 
            = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U]);
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U] 
            = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U]) 
               | (IData)(vlTOPp->lookup_gscid_i));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__k[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[3U] 
            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__7__Vfuncout[3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
            = vlTOPp->entries_i[0xfU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
            = vlTOPp->entries_i[0xfU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U] 
            = vlTOPp->entries_i[0xfU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[3U] 
            = vlTOPp->entries_i[0xfU][3U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout 
            = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[2U]) 
                   == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[2U])) 
                  & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                                   << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U] 
                                             >> 0x1dU))) 
                     == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                      << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U] 
                                                >> 0x1dU))))) 
                 & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[0U]) 
                    == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[0U]))) 
                & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                          >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                               >> 0x11U)))) 
               & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__e[1U] 
                         >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__k[1U] 
                                              >> 0x12U))));
        if (((vlTOPp->entries_i[0xfU][1U] >> 0x13U) 
             & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__5__Vfuncout))) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[0U] 
                = vlTOPp->entries_i[0xfU][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                = vlTOPp->entries_i[0xfU][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[2U] 
                = vlTOPp->entries_i[0xfU][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[3U] 
                = vlTOPp->entries_i[0xfU][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout 
                = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                             >> 0x11U)) 
                              & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                    >> 0x12U))) & (~ 
                                                   (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                    >> 0x14U))) 
                            & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                  >> 0x15U))) & (~ 
                                                 (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x16U))) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__e[1U] 
                                                  >> 0x18U))));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__6__Vfuncout) {
                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__cap_found)))) {
                    vlTOPp->dma_research_observer__DOT__cap_found = 1U;
                    vlTOPp->dma_research_observer__DOT__cap_key[0U] 
                        = vlTOPp->entries_i[0xfU][0U];
                    vlTOPp->dma_research_observer__DOT__cap_key[1U] 
                        = vlTOPp->entries_i[0xfU][1U];
                    vlTOPp->dma_research_observer__DOT__cap_key[2U] 
                        = vlTOPp->entries_i[0xfU][2U];
                    vlTOPp->dma_research_observer__DOT__cap_key[3U] 
                        = vlTOPp->entries_i[0xfU][3U];
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cap_bad_geom = 1U;
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cmd_wr = (((IData)(vlTOPp->cfg_valid_i) 
                                                   & (IData)(vlTOPp->cfg_write_i)) 
                                                  & (5U 
                                                     == (IData)(vlTOPp->index_i)));
    vlTOPp->dma_research_observer__DOT__abort_cmd = 
        (((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
          & (0ULL == (0xfffffffffffffffULL & (vlTOPp->wdata_i 
                                              >> 4U)))) 
         & (5U == (0xfU & (IData)(vlTOPp->wdata_i))));
}

VL_INLINE_OPT void Vdma_research_observer::_sequent__TOP__4(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_sequent__TOP__4\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_key_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_key_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v2;
    CData/*0:0*/ __Vdlyvval__dma_research_observer__DOT__fp_hit_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_hit_q__v0;
    CData/*0:0*/ __Vdlyvval__dma_research_observer__DOT__fp_hit_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_hit_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_time_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__fp_time_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__target_key_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__target_key_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__lane_id_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__lane_id_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_done_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_done_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_sel_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_sel_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_hit_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_hit_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_miss_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_miss_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v6;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v6;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v0;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v1;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v0;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v1;
    WData/*127:0*/ __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[4];
    WData/*127:0*/ __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[4];
    WData/*127:0*/ __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[4];
    WData/*127:0*/ __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[4];
    WData/*127:0*/ __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[4];
    WData/*127:0*/ __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[4];
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4;
    IData/*31:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__fp_time_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__fp_time_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_main_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_main_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__lane_id_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__lane_id_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_main_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_main_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_done_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__cost_done_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_comp_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_comp_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_sel_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_sel_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_hit_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_hit_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_miss_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__win_miss_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5;
    // Body
    __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_main_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_main_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_sel_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_sel_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_main_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_main_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_miss_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_miss_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_hit_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__win_hit_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_done_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__cost_done_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_time_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_time_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_hit_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_hit_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_key_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__fp_key_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__lane_id_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__lane_id_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_key_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_key_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v6 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v6 = 0U;
    vlTOPp->dma_research_observer__DOT__e2a_first_seen_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__e2a_first_seen_n));
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__astart_q 
            = vlTOPp->dma_research_observer__DOT__astart_n;
        vlTOPp->dma_research_observer__DOT__probestart_q 
            = vlTOPp->dma_research_observer__DOT__probestart_n;
        vlTOPp->dma_research_observer__DOT__freeze_ts_q 
            = vlTOPp->dma_research_observer__DOT__freeze_ts_n;
        vlTOPp->dma_research_observer__DOT__inval_count_q 
            = vlTOPp->dma_research_observer__DOT__inval_count_n;
        vlTOPp->dma_research_observer__DOT__update_count_q 
            = vlTOPp->dma_research_observer__DOT__update_count_n;
        vlTOPp->dma_research_observer__DOT__cmd_q = vlTOPp->dma_research_observer__DOT__cmd_n;
        vlTOPp->dma_research_observer__DOT__e2a_last_q 
            = vlTOPp->dma_research_observer__DOT__e2a_last_n;
        vlTOPp->dma_research_observer__DOT__quiet_timeout_q 
            = vlTOPp->dma_research_observer__DOT__quiet_timeout_n;
        vlTOPp->dma_research_observer__DOT__deadline_q 
            = vlTOPp->dma_research_observer__DOT__deadline_n;
        vlTOPp->dma_research_observer__DOT__e2a_first_q 
            = vlTOPp->dma_research_observer__DOT__e2a_first_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_n;
        vlTOPp->dma_research_observer__DOT__carry_in_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__carry_in_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__cur_a_flags_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_flags_n;
        vlTOPp->dma_research_observer__DOT__carry_out_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__carry_out_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n;
        vlTOPp->dma_research_observer__DOT__cost_cycles_q 
            = vlTOPp->dma_research_observer__DOT__cost_cycles_n;
        vlTOPp->dma_research_observer__DOT__cost_a_done_all_q 
            = vlTOPp->dma_research_observer__DOT__cost_a_done_all_n;
        vlTOPp->dma_research_observer__DOT__win_cycles_q 
            = vlTOPp->dma_research_observer__DOT__win_cycles_n;
        vlTOPp->dma_research_observer__DOT__cur_v_main_q 
            = vlTOPp->dma_research_observer__DOT__cur_v_main_n;
        vlTOPp->dma_research_observer__DOT__cur_v_comp_q 
            = vlTOPp->dma_research_observer__DOT__cur_v_comp_n;
        vlTOPp->dma_research_observer__DOT__cost_owner_err_q 
            = vlTOPp->dma_research_observer__DOT__cost_owner_err_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_n;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_n;
    } else {
        vlTOPp->dma_research_observer__DOT__astart_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__probestart_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__freeze_ts_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__inval_count_q = 0U;
        vlTOPp->dma_research_observer__DOT__update_count_q = 0U;
        vlTOPp->dma_research_observer__DOT__cmd_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2a_last_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__quiet_timeout_q = 0U;
        vlTOPp->dma_research_observer__DOT__deadline_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__e2a_first_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__carry_in_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_flags_q = 0U;
        vlTOPp->dma_research_observer__DOT__carry_out_q = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_q = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_cycles_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_a_done_all_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__win_cycles_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__cur_v_main_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_v_comp_q = 0U;
        vlTOPp->dma_research_observer__DOT__cost_owner_err_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_q = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q = 0ULL;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_observer__DOT__acc_freeze) {
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[1U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[2U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[3U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[4U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [1U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [1U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[5U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [1U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [1U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[6U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [1U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [1U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[7U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [1U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [1U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[8U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [2U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [2U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[9U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [2U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [2U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xaU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [2U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [2U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xbU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [2U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [2U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xcU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [3U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [3U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xdU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [3U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [3U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xeU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [3U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [3U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xfU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [3U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [3U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x10U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [4U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [4U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x11U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [4U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [4U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x12U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [4U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [4U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x13U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [4U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [4U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x14U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [5U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [5U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x15U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [5U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [5U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x16U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [5U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [5U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x17U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [5U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [5U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x18U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [6U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [6U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x19U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [6U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [6U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [6U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [6U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [6U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [6U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [7U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [7U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [7U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [7U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [7U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [7U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [7U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [7U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x20U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [8U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [8U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x21U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [8U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [8U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x22U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [8U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [8U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x23U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [8U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [8U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x24U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [9U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [9U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x25U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [9U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [9U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x26U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [9U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [9U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x27U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [9U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [9U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x28U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xaU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xaU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x29U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xaU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xaU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x2aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xaU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xaU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x2bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xaU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xaU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x2cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xbU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xbU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x2dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xbU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xbU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x2eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xbU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xbU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x2fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xbU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xbU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x30U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xcU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xcU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x31U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xcU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xcU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x32U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xcU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xcU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x33U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xcU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xcU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x34U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xdU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xdU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x35U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xdU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xdU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x36U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xdU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xdU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x37U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xdU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xdU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x38U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xeU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xeU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x39U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xeU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xeU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x3aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xeU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xeU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x3bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xeU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xeU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x3cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xfU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xfU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x3dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xfU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xfU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x3eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xfU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xfU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x3fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xfU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xfU][2U]))) 
                           >> 0x20U));
        }
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_observer__DOT__acc_a) {
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[1U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[2U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[3U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[4U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [1U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [1U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[5U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [1U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [1U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[6U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [1U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [1U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[7U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [1U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [1U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[8U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [2U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [2U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[9U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [2U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [2U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0xaU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [2U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [2U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0xbU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [2U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [2U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0xcU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [3U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [3U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0xdU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [3U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [3U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0xeU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [3U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [3U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0xfU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [3U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [3U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x10U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [4U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [4U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x11U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [4U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [4U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x12U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [4U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [4U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x13U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [4U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [4U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x14U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [5U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [5U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x15U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [5U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [5U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x16U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [5U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [5U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x17U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [5U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [5U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x18U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [6U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [6U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x19U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [6U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [6U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [6U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [6U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [6U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [6U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [7U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [7U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [7U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [7U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [7U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [7U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [7U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [7U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x20U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [8U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [8U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x21U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [8U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [8U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x22U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [8U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [8U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x23U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [8U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [8U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x24U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [9U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [9U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x25U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [9U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [9U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x26U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [9U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [9U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x27U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [9U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [9U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x28U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xaU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xaU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x29U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xaU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xaU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x2aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xaU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xaU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x2bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xaU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xaU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x2cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xbU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xbU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x2dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xbU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xbU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x2eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xbU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xbU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x2fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xbU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xbU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x30U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xcU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xcU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x31U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xcU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xcU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x32U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xcU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xcU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x33U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xcU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xcU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x34U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xdU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xdU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x35U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xdU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xdU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x36U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xdU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xdU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x37U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xdU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xdU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x38U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xeU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xeU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x39U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xeU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xeU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x3aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xeU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xeU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x3bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xeU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xeU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x3cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xfU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xfU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x3dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xfU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xfU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x3eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xfU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xfU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_warm_q[0x3fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xfU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xfU][2U]))) 
                           >> 0x20U));
        }
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n;
        vlTOPp->dma_research_observer__DOT__e2_ovf_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__e2_ovf_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__sample_ovf_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__sample_ovf_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q 
            = vlTOPp->dma_research_observer__DOT__e2a_done_cnt_n;
        vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q 
            = vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_n;
        vlTOPp->dma_research_observer__DOT__dst_iova_q 
            = vlTOPp->dma_research_observer__DOT__dst_iova_n;
        vlTOPp->dma_research_observer__DOT__src_iova_q 
            = vlTOPp->dma_research_observer__DOT__src_iova_n;
        vlTOPp->dma_research_observer__DOT__cost_err_q 
            = vlTOPp->dma_research_observer__DOT__cost_err_n;
        vlTOPp->dma_research_observer__DOT__cur_a_dur_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_dur_n;
        vlTOPp->dma_research_observer__DOT__cur_a_main_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
        vlTOPp->dma_research_observer__DOT__cur_a_req_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_req_n;
        vlTOPp->dma_research_observer__DOT__cur_a_comp_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
        vlTOPp->dma_research_observer__DOT__cur_a_miss_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_miss_n;
        vlTOPp->dma_research_observer__DOT__e2_err_q 
            = vlTOPp->dma_research_observer__DOT__e2_err_n;
    } else {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2_ovf_q = 0U;
        vlTOPp->dma_research_observer__DOT__sample_ovf_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q = 0U;
        vlTOPp->dma_research_observer__DOT__dst_iova_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__src_iova_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_err_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_dur_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_main_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_req_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_comp_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_miss_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2_err_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_issue_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_issue_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_sel_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_sel_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_comp_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_comp_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_main_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_main_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_main_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_main_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_main_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_main_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__win_sel_q__v0 
            = vlTOPp->dma_research_observer__DOT__win_sel_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__win_sel_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__win_sel_q__v1 
            = vlTOPp->dma_research_observer__DOT__win_sel_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__win_sel_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__win_comp_q__v0 
            = vlTOPp->dma_research_observer__DOT__win_comp_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__win_comp_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__win_comp_q__v1 
            = vlTOPp->dma_research_observer__DOT__win_comp_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__win_comp_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_hit_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_hit_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__win_main_q__v0 
            = vlTOPp->dma_research_observer__DOT__win_main_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__win_main_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__win_main_q__v1 
            = vlTOPp->dma_research_observer__DOT__win_main_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__win_main_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_miss_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_miss_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__win_miss_q__v0 
            = vlTOPp->dma_research_observer__DOT__win_miss_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__win_miss_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__win_miss_q__v1 
            = vlTOPp->dma_research_observer__DOT__win_miss_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__win_miss_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__win_hit_q__v0 
            = vlTOPp->dma_research_observer__DOT__win_hit_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__win_hit_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__win_hit_q__v1 
            = vlTOPp->dma_research_observer__DOT__win_hit_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__win_hit_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__cost_done_q__v0 
            = vlTOPp->dma_research_observer__DOT__cost_done_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__cost_done_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__cost_done_q__v1 
            = vlTOPp->dma_research_observer__DOT__cost_done_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__cost_done_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n;
        vlTOPp->dma_research_observer__DOT__fp_done_q 
            = vlTOPp->dma_research_observer__DOT__fp_done_n;
        vlTOPp->dma_research_observer__DOT__dst_set_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__dst_set_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__src_set_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__src_set_n) 
               & 1U);
    } else {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q = 0U;
        vlTOPp->dma_research_observer__DOT__fp_done_q = 0U;
        vlTOPp->dma_research_observer__DOT__dst_set_q = 0U;
        vlTOPp->dma_research_observer__DOT__src_set_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__fp_time_q__v0 
            = vlTOPp->dma_research_observer__DOT__fp_time_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__fp_time_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__fp_time_q__v1 
            = vlTOPp->dma_research_observer__DOT__fp_time_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__fp_time_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__fp_hit_q__v0 
            = vlTOPp->dma_research_observer__DOT__fp_hit_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__fp_hit_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__fp_hit_q__v1 
            = vlTOPp->dma_research_observer__DOT__fp_hit_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__fp_hit_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v0 
            = vlTOPp->dma_research_observer__DOT__fp_live_self_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v1 
            = vlTOPp->dma_research_observer__DOT__fp_live_self_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v0 
            = vlTOPp->dma_research_observer__DOT__fp_live_oth_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v1 
            = vlTOPp->dma_research_observer__DOT__fp_live_oth_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[0U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [0U][0U];
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[1U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [0U][1U];
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[2U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [0U][2U];
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[3U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [0U][3U];
        __Vdlyvset__dma_research_observer__DOT__fp_key_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[0U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [1U][0U];
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[1U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [1U][1U];
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[2U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [1U][2U];
        __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[3U] 
            = vlTOPp->dma_research_observer__DOT__fp_key_n
            [1U][3U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__fp_key_q__v2 = 1U;
    }
    vlTOPp->dma_research_observer__DOT__window_closed_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__window_closed_n));
    vlTOPp->dma_research_observer__DOT__window_open_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__window_open_n));
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__target_ctx_valid_q 
            = vlTOPp->dma_research_observer__DOT__target_ctx_valid_n;
        vlTOPp->dma_research_observer__DOT__target_valid_q 
            = vlTOPp->dma_research_observer__DOT__target_valid_n;
    } else {
        vlTOPp->dma_research_observer__DOT__target_ctx_valid_q = 0U;
        vlTOPp->dma_research_observer__DOT__target_valid_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[0U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [0U][0U];
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[1U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [0U][1U];
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[2U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [0U][2U];
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[3U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [0U][3U];
        __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[0U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [1U][0U];
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[1U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [1U][1U];
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[2U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [1U][2U];
        __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[3U] 
            = vlTOPp->dma_research_observer__DOT__target_ctx_n
            [1U][3U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v2 = 1U;
    }
    vlTOPp->dma_research_observer__DOT__lane_active_q 
        = ((IData)(vlTOPp->rst_ni) ? (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n)
            : 0U);
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__lane_id_q__v0 
            = vlTOPp->dma_research_observer__DOT__lane_id_n
            [0U];
        __Vdlyvset__dma_research_observer__DOT__lane_id_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__lane_id_q__v1 
            = vlTOPp->dma_research_observer__DOT__lane_id_n
            [1U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__lane_id_q__v2 = 1U;
    }
    if (vlTOPp->rst_ni) {
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[0U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [0U][0U];
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[1U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [0U][1U];
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[2U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [0U][2U];
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[3U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [0U][3U];
        __Vdlyvset__dma_research_observer__DOT__target_key_q__v0 = 1U;
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[0U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [1U][0U];
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[1U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [1U][1U];
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[2U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [1U][2U];
        __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[3U] 
            = vlTOPp->dma_research_observer__DOT__target_key_n
            [1U][3U];
    } else {
        __Vdlyvset__dma_research_observer__DOT__target_key_q__v2 = 1U;
    }
    vlTOPp->dma_research_observer__DOT__cost_active_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_n));
    vlTOPp->dma_research_observer__DOT__mode_q = ((IData)(vlTOPp->rst_ni)
                                                   ? (IData)(vlTOPp->dma_research_observer__DOT__mode_n)
                                                   : 0U);
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound48;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound44;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound45;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound46;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound47;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound49;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound50;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound51;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound52;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound53;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound54;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n
            [3U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n
            [4U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4 = 1U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n
            [5U];
        __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5 
            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound55;
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5 = 1U;
    } else {
        __Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v6 = 1U;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__snapshot_valid_q 
            = vlTOPp->dma_research_observer__DOT__snapshot_valid_n;
        vlTOPp->dma_research_observer__DOT____Vlvbound2 
            = vlTOPp->dma_research_observer__DOT__plru_snap_n
            [0U];
        __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v0 
            = vlTOPp->dma_research_observer__DOT____Vlvbound2;
        __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v0 = 1U;
        vlTOPp->dma_research_observer__DOT____Vlvbound2 
            = vlTOPp->dma_research_observer__DOT__plru_snap_n
            [1U];
        __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v1 
            = vlTOPp->dma_research_observer__DOT____Vlvbound2;
        __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v1 = 1U;
        vlTOPp->dma_research_observer__DOT____Vlvbound2 
            = vlTOPp->dma_research_observer__DOT__plru_snap_n
            [2U];
        __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v2 
            = vlTOPp->dma_research_observer__DOT____Vlvbound2;
        __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v2 = 1U;
        if (vlTOPp->dma_research_observer__DOT__acc_a) {
            __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v3 
                = vlTOPp->plru_i;
            __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v3 = 1U;
            vlTOPp->dma_research_observer__DOT__snapshot_valid_q 
                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q));
        }
        if (((((((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                 & (0x40ULL <= (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q))) 
                & (IData)(vlTOPp->quiet_i)) & (IData)(vlTOPp->supported_i)) 
              & (~ (IData)(vlTOPp->update_i))) & (~ (IData)(vlTOPp->invalidate_i)))) {
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[1U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0U][0U]))) 
                           >> 0x20U));
            __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v4 
                = vlTOPp->plru_i;
            __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v4 = 1U;
            vlTOPp->dma_research_observer__DOT__snapshot_valid_q 
                = (2U | (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[2U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[3U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[4U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [1U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [1U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[5U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [1U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [1U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[6U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [1U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [1U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[7U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [1U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [1U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[8U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [2U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [2U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[9U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [2U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [2U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xaU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [2U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [2U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xbU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [2U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [2U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xcU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [3U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [3U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xdU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [3U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [3U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xeU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [3U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [3U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xfU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [3U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [3U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x10U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [4U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [4U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x11U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [4U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [4U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x12U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [4U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [4U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x13U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [4U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [4U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x14U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [5U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [5U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x15U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [5U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [5U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x16U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [5U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [5U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x17U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [5U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [5U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x18U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [6U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [6U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x19U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [6U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [6U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [6U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [6U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [6U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [6U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [7U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [7U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [7U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [7U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [7U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [7U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [7U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [7U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x20U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [8U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [8U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x21U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [8U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [8U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x22U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [8U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [8U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x23U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [8U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [8U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x24U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [9U][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [9U][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x25U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [9U][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [9U][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x26U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [9U][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [9U][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x27U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [9U][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [9U][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x28U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xaU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xaU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x29U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xaU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xaU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x2aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xaU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xaU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x2bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xaU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xaU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x2cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xbU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xbU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x2dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xbU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xbU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x2eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xbU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xbU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x2fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xbU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xbU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x30U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xcU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xcU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x31U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xcU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xcU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x32U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xcU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xcU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x33U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xcU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xcU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x34U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xdU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xdU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x35U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xdU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xdU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x36U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xdU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xdU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x37U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xdU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xdU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x38U] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xeU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xeU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x39U] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xeU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xeU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x3aU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xeU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xeU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x3bU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xeU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xeU][2U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x3cU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xfU][1U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xfU][0U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x3dU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xfU][1U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xfU][0U]))) 
                           >> 0x20U));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x3eU] 
                = (IData)((((QData)((IData)(vlTOPp->entries_i
                                            [0xfU][3U])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->entries_i
                                                        [0xfU][2U]))));
            vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x3fU] 
                = (IData)(((((QData)((IData)(vlTOPp->entries_i
                                             [0xfU][3U])) 
                             << 0x20U) | (QData)((IData)(
                                                         vlTOPp->entries_i
                                                         [0xfU][2U]))) 
                           >> 0x20U));
        }
        if (vlTOPp->dma_research_observer__DOT__acc_freeze) {
            __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v5 
                = vlTOPp->plru_i;
            __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v5 = 1U;
            vlTOPp->dma_research_observer__DOT__snapshot_valid_q 
                = (4U | (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q));
        }
    } else {
        vlTOPp->dma_research_observer__DOT__snapshot_valid_q = 0U;
        __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v6 = 1U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_issue_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_issue_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_issue_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_issue_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_issue_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_issue_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_sel_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_sel_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_sel_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_sel_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_sel_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_sel_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_comp_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_comp_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_main_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_main_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_main_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_sel_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_sel_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_sel_q__v0;
        vlTOPp->dma_research_observer__DOT__win_sel_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_sel_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_sel_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_sel_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__win_sel_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_comp_q__v0;
        vlTOPp->dma_research_observer__DOT__win_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_comp_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__win_comp_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_hit_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_hit_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_hit_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_hit_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_hit_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_hit_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_main_q__v0;
        vlTOPp->dma_research_observer__DOT__win_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_main_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__win_main_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_miss_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_miss_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_miss_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_miss_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_miss_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_miss_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_miss_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_miss_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_miss_q__v0;
        vlTOPp->dma_research_observer__DOT__win_miss_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_miss_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_miss_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_miss_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__win_miss_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_hit_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_hit_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_hit_q__v0;
        vlTOPp->dma_research_observer__DOT__win_hit_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_hit_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_hit_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_hit_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__win_hit_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_done_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_done_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_done_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_done_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_done_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_done_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_done_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__cost_done_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_time_q__v0) {
        vlTOPp->dma_research_observer__DOT__fp_time_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_time_q__v0;
        vlTOPp->dma_research_observer__DOT__fp_time_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_time_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_time_q__v2) {
        vlTOPp->dma_research_observer__DOT__fp_time_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__fp_time_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_hit_q__v0) {
        vlTOPp->dma_research_observer__DOT__fp_hit_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_hit_q__v0;
        vlTOPp->dma_research_observer__DOT__fp_hit_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_hit_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_hit_q__v2) {
        vlTOPp->dma_research_observer__DOT__fp_hit_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_hit_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v0) {
        vlTOPp->dma_research_observer__DOT__fp_live_self_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v0;
        vlTOPp->dma_research_observer__DOT__fp_live_self_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_live_self_q__v2) {
        vlTOPp->dma_research_observer__DOT__fp_live_self_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_live_self_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v0) {
        vlTOPp->dma_research_observer__DOT__fp_live_oth_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v0;
        vlTOPp->dma_research_observer__DOT__fp_live_oth_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_live_oth_q__v2) {
        vlTOPp->dma_research_observer__DOT__fp_live_oth_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_live_oth_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_key_q__v0) {
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][0U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[0U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][1U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[1U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][2U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[2U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][3U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v0[3U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][0U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[0U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][1U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[1U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][2U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[2U];
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][3U] 
            = __Vdlyvval__dma_research_observer__DOT__fp_key_q__v1[3U];
    }
    if (__Vdlyvset__dma_research_observer__DOT__fp_key_q__v2) {
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][0U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][1U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][2U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[0U][3U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][0U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][1U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][2U] = 0U;
        vlTOPp->dma_research_observer__DOT__fp_key_q[1U][3U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__target_ctx_q__v0) {
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][0U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[0U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][1U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[1U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][2U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[2U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][3U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v0[3U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][0U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[0U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][1U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[1U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][2U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[2U];
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][3U] 
            = __Vdlyvval__dma_research_observer__DOT__target_ctx_q__v1[3U];
    }
    if (__Vdlyvset__dma_research_observer__DOT__target_ctx_q__v2) {
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][0U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][1U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][2U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[0U][3U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][0U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][1U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][2U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_q[1U][3U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__lane_id_q__v0) {
        vlTOPp->dma_research_observer__DOT__lane_id_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__lane_id_q__v0;
        vlTOPp->dma_research_observer__DOT__lane_id_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__lane_id_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__lane_id_q__v2) {
        vlTOPp->dma_research_observer__DOT__lane_id_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__lane_id_q[1U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__target_key_q__v0) {
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][0U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[0U];
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][1U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[1U];
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][2U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[2U];
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][3U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v0[3U];
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][0U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[0U];
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][1U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[1U];
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][2U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[2U];
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][3U] 
            = __Vdlyvval__dma_research_observer__DOT__target_key_q__v1[3U];
    }
    if (__Vdlyvset__dma_research_observer__DOT__target_key_q__v2) {
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][0U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][1U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][2U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[0U][3U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][0U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][1U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][2U] = 0U;
        vlTOPp->dma_research_observer__DOT__target_key_q[1U][3U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q[5U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q[5U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q[5U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q[5U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q[5U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q[5U] = 0ULL;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q__v6) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q[5U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v0) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v1) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v2) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v3) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v4) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v5) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v5;
    }
    if (__Vdlyvset__dma_research_observer__DOT__plru_snap_q__v6) {
        vlTOPp->dma_research_observer__DOT__plru_snap_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__plru_snap_q[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__plru_snap_q[2U] = 0U;
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[0U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[1U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[2U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[3U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[0U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[1U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[2U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[3U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__Vfuncout 
        = (((((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[1U] 
                            << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[0U] 
                                      >> 0x1dU))) == 
              (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[1U] 
                            << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[0U] 
                                      >> 0x1dU)))) 
             & ((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[2U]) 
                == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[2U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__a[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__b[1U] 
                                          >> 0x12U))));
    vlTOPp->dma_research_observer__DOT__ctx_ok_both 
        = ((3U == (IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q)) 
           & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__0__Vfuncout));
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound72;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound68;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound69;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound70;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound71;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound73;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound74;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound75;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound76;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound77;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound78;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [0U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [1U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [2U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [3U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [4U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [5U];
    vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound79;
    vlTOPp->dma_research_observer__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__plru_snap_q
        [0U];
    vlTOPp->dma_research_observer__DOT__plru_snap_n[0U] 
        = vlTOPp->dma_research_observer__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__plru_snap_q
        [1U];
    vlTOPp->dma_research_observer__DOT__plru_snap_n[1U] 
        = vlTOPp->dma_research_observer__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__plru_snap_q
        [2U];
    vlTOPp->dma_research_observer__DOT__plru_snap_n[2U] 
        = vlTOPp->dma_research_observer__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__a_elapsed_q 
        = ((IData)(vlTOPp->rst_ni) ? vlTOPp->dma_research_observer__DOT__a_elapsed_n
            : 0ULL);
    vlTOPp->dma_research_observer__DOT__agg_sum_req_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_sum_req_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_sum_req_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_sum_req_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_sum_req_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_sum_req_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_req_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_count_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_count_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_count_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_count_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_count_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_count_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__count_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_sum_main_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_sum_main_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_sum_main_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_sum_main_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_sum_main_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_sum_main_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_main_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_sum_comp_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_sum_comp_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_sum_comp_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_sum_comp_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_sum_comp_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_sum_comp_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_comp_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_sum_dur_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_sum_dur_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_sum_dur_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_sum_dur_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_sum_dur_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_sum_dur_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_dur_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_sum_miss_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_sum_miss_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_sum_miss_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_sum_miss_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_sum_miss_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_sum_miss_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__sum_miss_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_min_main_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_min_main_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_min_main_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_min_main_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_min_main_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_min_main_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_main_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_max_main_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_max_main_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_max_main_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_max_main_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_max_main_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_max_main_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_main_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_min_comp_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_min_comp_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_min_comp_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_min_comp_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_min_comp_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_min_comp_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_comp_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_max_comp_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_max_comp_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_max_comp_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_max_comp_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_max_comp_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_max_comp_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_comp_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_min_dur_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_min_dur_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_min_dur_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_min_dur_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_min_dur_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_min_dur_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__min_dur_o
        [5U];
    vlTOPp->dma_research_observer__DOT__agg_max_dur_o[0U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o
        [0U];
    vlTOPp->dma_research_observer__DOT__agg_max_dur_o[1U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o
        [1U];
    vlTOPp->dma_research_observer__DOT__agg_max_dur_o[2U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o
        [2U];
    vlTOPp->dma_research_observer__DOT__agg_max_dur_o[3U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o
        [3U];
    vlTOPp->dma_research_observer__DOT__agg_max_dur_o[4U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o
        [4U];
    vlTOPp->dma_research_observer__DOT__agg_max_dur_o[5U] 
        = vlTOPp->dma_research_observer__DOT____Vcellout__u_dma_research_cost_groups__max_dur_o
        [5U];
}

VL_INLINE_OPT void Vdma_research_observer::_sequent__TOP__5(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_sequent__TOP__5\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*7:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v0;
    CData/*7:0*/ __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0;
    WData/*383:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[12];
    WData/*255:0*/ __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[8];
    // Body
    __Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v0 = 0U;
    if (((IData)(vlTOPp->rst_ni) & (((((IData)(vlTOPp->dma_research_observer__DOT__win_act) 
                                       & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok)) 
                                      & ((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                                         >> 1U)) & 
                                     (0xfU == (0xfU 
                                               & (IData)(vlTOPp->dma_research_observer__DOT__eligible_q)))) 
                                    & (0x100U > (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q))))) {
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[0U] 
            = (IData)(vlTOPp->done_id_i[1U]);
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[1U] 
            = (IData)((vlTOPp->done_id_i[1U] >> 0x20U));
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[2U] 
            = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[3U] 
            = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[4U] 
            = vlTOPp->dma_research_observer__DOT__cur_a_miss_n;
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[5U] 
            = vlTOPp->dma_research_observer__DOT__cur_a_req_n;
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[6U] 
            = (IData)((((QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_dur_n)) 
                        << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_flags_n))));
        __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[7U] 
            = (IData)(((((QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_dur_n)) 
                         << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_flags_n))) 
                       >> 0x20U));
        __Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0 = 1U;
        __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0 
            = (0xffU & (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q));
    }
    if (((IData)(vlTOPp->rst_ni) & ((((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                                        | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                                       | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                                      & (IData)(vlTOPp->update_i)) 
                                     & (IData)(vlTOPp->dma_research_observer__DOT__slot_oh_valid)) 
                                    & (0x100U > (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))) {
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[0U] 
            = (IData)(vlTOPp->dma_research_observer__DOT__epoch_q);
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[1U] 
            = (IData)((vlTOPp->dma_research_observer__DOT__epoch_q 
                       >> 0x20U));
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[2U] 
            = (IData)(vlTOPp->dma_research_observer__DOT__evt_meta);
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[3U] 
            = (IData)((vlTOPp->dma_research_observer__DOT__evt_meta 
                       >> 0x20U));
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[4U] 
            = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][0U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[5U] 
            = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][1U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[6U] 
            = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][2U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[7U] 
            = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][3U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[8U] 
            = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][0U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[9U] 
            = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][1U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[0xaU] 
            = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][2U];
        __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[0xbU] 
            = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][3U];
        __Vdlyvset__dma_research_observer__DOT__event_ram_q__v0 = 1U;
        __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0 
            = (0xffU & (IData)(vlTOPp->dma_research_observer__DOT__event_count_q));
    }
    if (__Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0) {
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][0U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[0U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][1U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[1U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][2U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[2U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][3U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[3U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][4U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[4U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][5U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[5U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][6U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[6U];
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0][7U] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0[7U];
    }
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v0) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][0U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[0U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][1U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[1U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][2U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[2U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][3U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[3U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][4U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[4U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][5U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[5U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][6U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[6U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][7U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[7U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][8U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[8U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][9U] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[9U];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][0xaU] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[0xaU];
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0][0xbU] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0[0xbU];
    }
}

VL_INLINE_OPT void Vdma_research_observer::_combo__TOP__6(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_combo__TOP__6\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_research_observer__DOT__cmd_n = vlTOPp->dma_research_observer__DOT__cmd_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U != (IData)(vlTOPp->index_i))) {
            if ((5U == (IData)(vlTOPp->index_i))) {
                vlTOPp->dma_research_observer__DOT__cmd_n 
                    = (0xfU & (IData)(vlTOPp->wdata_i));
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_edge 
        = ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q)) 
           & (IData)(vlTOPp->issue_i));
    vlTOPp->dma_research_observer__DOT__tgt_live0 = 0U;
    vlTOPp->dma_research_observer__DOT__tgt_present0 = 0U;
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfffeU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (1U & (vlTOPp->entries_i[0U][1U] >> 0x13U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[1U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[1U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[1U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[1U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfffdU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (2U & (vlTOPp->entries_i[1U][1U] >> 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[2U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[2U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[2U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[2U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[2U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[2U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[2U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[2U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfffbU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (4U & (vlTOPp->entries_i[2U][1U] >> 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[3U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[3U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[3U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[3U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[3U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[3U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[3U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[3U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfff7U & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (8U & (vlTOPp->entries_i[3U][1U] >> 0x10U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[4U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[4U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[4U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[4U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[4U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[4U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[4U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[4U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xffefU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x10U & (vlTOPp->entries_i[4U][1U] 
                           >> 0xfU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[5U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[5U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[5U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[5U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[5U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[5U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[5U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[5U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xffdfU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x20U & (vlTOPp->entries_i[5U][1U] 
                           >> 0xeU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[6U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[6U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[6U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[6U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[6U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[6U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[6U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[6U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xffbfU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x40U & (vlTOPp->entries_i[6U][1U] 
                           >> 0xdU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[7U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[7U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[7U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[7U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[7U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[7U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[7U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[7U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xff7fU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x80U & (vlTOPp->entries_i[7U][1U] 
                           >> 0xcU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[8U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[8U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[8U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[8U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[8U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[8U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[8U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[8U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfeffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x100U & (vlTOPp->entries_i[8U][1U] 
                            >> 0xbU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[9U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[9U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[9U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[9U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[9U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[9U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[9U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[9U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfdffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x200U & (vlTOPp->entries_i[9U][1U] 
                            >> 0xaU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0xaU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0xaU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0xaU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0xaU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0xaU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0xaU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0xaU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0xaU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xfbffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x400U & (vlTOPp->entries_i[0xaU][1U] 
                            >> 9U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0xbU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0xbU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0xbU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0xbU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0xbU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0xbU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0xbU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0xbU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xf7ffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x800U & (vlTOPp->entries_i[0xbU][1U] 
                            >> 8U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0xcU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0xcU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0xcU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0xcU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0xcU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0xcU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0xcU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0xcU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xefffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x1000U & (vlTOPp->entries_i[0xcU][1U] 
                             >> 7U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0xdU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0xdU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0xdU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0xdU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0xdU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0xdU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0xdU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0xdU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xdfffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x2000U & (vlTOPp->entries_i[0xdU][1U] 
                             >> 6U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0xeU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0xeU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0xeU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0xeU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0xeU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0xeU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0xeU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0xeU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0xbfffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x4000U & (vlTOPp->entries_i[0xeU][1U] 
                             >> 5U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
        = vlTOPp->entries_i[0xfU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
        = vlTOPp->entries_i[0xfU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U] 
        = vlTOPp->entries_i[0xfU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[3U] 
        = vlTOPp->entries_i[0xfU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__k[1U] 
                                          >> 0x12U))));
    if (((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
         & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__1__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[0U] 
            = vlTOPp->entries_i[0xfU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
            = vlTOPp->entries_i[0xfU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[2U] 
            = vlTOPp->entries_i[0xfU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[3U] 
            = vlTOPp->entries_i[0xfU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live0 
            = ((0x7fffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live0)) 
               | (0x8000U & (vlTOPp->entries_i[0xfU][1U] 
                             >> 4U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__2__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present0 = 1U;
        }
    }
    vlTOPp->dma_research_observer__DOT__tgt_live1 = 0U;
    vlTOPp->dma_research_observer__DOT__tgt_present1 = 0U;
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfffeU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (1U & (vlTOPp->entries_i[0U][1U] >> 0x13U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[1U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[1U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[1U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[1U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfffdU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (2U & (vlTOPp->entries_i[1U][1U] >> 0x12U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[2U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[2U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[2U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[2U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[2U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[2U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[2U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[2U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfffbU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (4U & (vlTOPp->entries_i[2U][1U] >> 0x11U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[3U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[3U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[3U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[3U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[3U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[3U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[3U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[3U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfff7U & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (8U & (vlTOPp->entries_i[3U][1U] >> 0x10U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[4U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[4U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[4U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[4U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[4U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[4U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[4U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[4U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xffefU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x10U & (vlTOPp->entries_i[4U][1U] 
                           >> 0xfU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[5U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[5U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[5U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[5U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[5U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[5U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[5U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[5U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xffdfU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x20U & (vlTOPp->entries_i[5U][1U] 
                           >> 0xeU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[6U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[6U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[6U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[6U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[6U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[6U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[6U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[6U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xffbfU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x40U & (vlTOPp->entries_i[6U][1U] 
                           >> 0xdU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[7U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[7U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[7U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[7U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[7U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[7U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[7U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[7U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xff7fU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x80U & (vlTOPp->entries_i[7U][1U] 
                           >> 0xcU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[8U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[8U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[8U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[8U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[8U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[8U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[8U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[8U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfeffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x100U & (vlTOPp->entries_i[8U][1U] 
                            >> 0xbU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[9U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[9U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[9U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[9U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[9U][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[9U][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[9U][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[9U][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfdffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x200U & (vlTOPp->entries_i[9U][1U] 
                            >> 0xaU)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0xaU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0xaU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0xaU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0xaU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0xaU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0xaU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0xaU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0xaU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xfbffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x400U & (vlTOPp->entries_i[0xaU][1U] 
                            >> 9U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0xbU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0xbU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0xbU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0xbU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0xbU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0xbU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0xbU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0xbU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xf7ffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x800U & (vlTOPp->entries_i[0xbU][1U] 
                            >> 8U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0xcU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0xcU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0xcU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0xcU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0xcU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0xcU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0xcU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0xcU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xefffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x1000U & (vlTOPp->entries_i[0xcU][1U] 
                             >> 7U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0xdU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0xdU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0xdU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0xdU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0xdU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0xdU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0xdU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0xdU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xdfffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x2000U & (vlTOPp->entries_i[0xdU][1U] 
                             >> 6U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0xeU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0xeU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0xeU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0xeU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0xeU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0xeU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0xeU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0xeU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0xbfffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x4000U & (vlTOPp->entries_i[0xeU][1U] 
                             >> 5U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
        = vlTOPp->entries_i[0xfU][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
        = vlTOPp->entries_i[0xfU][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U] 
        = vlTOPp->entries_i[0xfU][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[3U] 
        = vlTOPp->entries_i[0xfU][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__k[1U] 
                                          >> 0x12U))));
    if ((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
          >> 1U) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__3__Vfuncout))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[0U] 
            = vlTOPp->entries_i[0xfU][0U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
            = vlTOPp->entries_i[0xfU][1U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[2U] 
            = vlTOPp->entries_i[0xfU][2U];
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[3U] 
            = vlTOPp->entries_i[0xfU][3U];
        vlTOPp->dma_research_observer__DOT__tgt_live1 
            = ((0x7fffU & (IData)(vlTOPp->dma_research_observer__DOT__tgt_live1)) 
               | (0x8000U & (vlTOPp->entries_i[0xfU][1U] 
                             >> 4U)));
        vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout 
            = (1U & ((((((((vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x13U) & (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                         >> 0x11U)) 
                          & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                >> 0x12U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                  >> 0x14U))) 
                        & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                              >> 0x15U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                                >> 0x16U))) 
                      & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                            >> 0x17U))) & (~ (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__e[1U] 
                                              >> 0x18U))));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__ordinary4k__4__Vfuncout) {
            vlTOPp->dma_research_observer__DOT__tgt_present1 = 1U;
        }
    }
    vlTOPp->dma_research_observer__DOT__win_act_issue 
        = ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & ((IData)(vlTOPp->dma_research_observer__DOT__window_open_q) 
                | (IData)(vlTOPp->p4m_open_i))) & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q))) 
           & (~ (IData)(vlTOPp->p4m_close_i)));
    vlTOPp->dma_research_observer__DOT__slot_idx = 0U;
    if ((1U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0U;
    }
    if ((2U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 1U;
    }
    if ((4U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 2U;
    }
    if ((8U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 3U;
    }
    if ((0x10U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 4U;
    }
    if ((0x20U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 5U;
    }
    if ((0x40U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 6U;
    }
    if ((0x80U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 7U;
    }
    if ((0x100U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 8U;
    }
    if ((0x200U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 9U;
    }
    if ((0x400U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0xaU;
    }
    if ((0x800U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0xbU;
    }
    if ((0x1000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0xcU;
    }
    if ((0x2000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0xdU;
    }
    if ((0x4000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0xeU;
    }
    if ((0x8000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_research_observer__DOT__slot_idx = 0xfU;
    }
    vlTOPp->dma_research_observer__DOT__slot_oh_valid 
        = ((0U != (IData)(vlTOPp->replace_mask_i)) 
           & (0U == ((IData)(vlTOPp->replace_mask_i) 
                     & ((IData)(vlTOPp->replace_mask_i) 
                        - (IData)(1U)))));
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open 
        = ((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_edge) 
           | (2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q)));
}
