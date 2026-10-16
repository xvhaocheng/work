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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/next-experiment-20261002/revised/cva6/corev_apu/fpga/src/dma_research_observer.sv", 21, "",
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
            VL_FATAL_MT("/home/xvhaocheng/work/iommu-platform/out/next-experiment-20261002/revised/cva6/corev_apu/fpga/src/dma_research_observer.sv", 21, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vdma_research_observer::_combo__TOP__1(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_combo__TOP__1\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_research_observer__DOT__sel_known = 
        ((0xaU == vlTOPp->select_did_i) | (0xbU == vlTOPp->select_did_i));
    vlTOPp->dma_research_observer__DOT__main_cyc = 
        ((1U == (IData)(vlTOPp->ctl_state_i)) | (2U 
                                                 == (IData)(vlTOPp->ctl_state_i)));
    vlTOPp->dma_research_observer__DOT__main_known 
        = ((0xaU == vlTOPp->ctl_did_i) | (0xbU == vlTOPp->ctl_did_i));
    vlTOPp->dma_research_observer__DOT__lu_known = 
        ((0xaU == vlTOPp->lookup_did_i) | (0xbU == vlTOPp->lookup_did_i));
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

VL_INLINE_OPT void Vdma_research_observer::_sequent__TOP__3(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_sequent__TOP__3\n"); );
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
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__snap_freeze_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_miss_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v5;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v6;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_main_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_main_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__lane_id_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__lane_id_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_main_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_main_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_main_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_main_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_comp_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_sel_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_hit_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_issue_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_issue_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_done_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_done_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__cost_done_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__cost_done_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_comp_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_comp_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_comp_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_comp_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v3;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v4;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__event_ram_q__v5;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_sel_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_sel_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_sel_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_sel_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_hit_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_hit_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_hit_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_hit_q__v2;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_miss_q__v0;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_miss_q__v0;
    CData/*7:0*/ __Vdlyvval__dma_research_observer__DOT__win_miss_q__v1;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__win_miss_q__v2;
    CData/*0:0*/ __Vdlyvset__dma_research_observer__DOT__snap_warm_q__v0;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v0;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_oth_q__v1;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v0;
    SData/*15:0*/ __Vdlyvval__dma_research_observer__DOT__fp_live_self_q__v1;
    SData/*9:0*/ __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0;
    SData/*9:0*/ __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v1;
    SData/*9:0*/ __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v2;
    SData/*9:0*/ __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v3;
    SData/*10:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0;
    SData/*10:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v1;
    SData/*10:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v2;
    SData/*10:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v3;
    SData/*10:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v4;
    SData/*10:0*/ __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v5;
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
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__fp_time_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__fp_time_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v6;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v7;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v8;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v9;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v10;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v11;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v12;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v13;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v14;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v15;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v16;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v17;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v18;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v19;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v20;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v21;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v22;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v23;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v24;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v25;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v26;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v27;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v28;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v29;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v30;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v31;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v6;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v7;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v8;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v9;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v10;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v11;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v12;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v13;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v14;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v15;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v16;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v17;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v18;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v19;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v20;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v21;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v22;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v23;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v24;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v25;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v26;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v27;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v28;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v29;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v30;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v31;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__lane_id_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__lane_id_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__event_ram_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v0;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v1;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v2;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v3;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v4;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v5;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v6;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v7;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v8;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v9;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v10;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v11;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v12;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v13;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v14;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v15;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v16;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v17;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v18;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v19;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v20;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v21;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v22;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v23;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v24;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v25;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v26;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v27;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v28;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v29;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v30;
    QData/*63:0*/ __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v31;
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
    __Vdlyvset__dma_research_observer__DOT__snap_freeze_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__snap_warm_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0 = 0U;
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
    __Vdlyvset__dma_research_observer__DOT__lane_id_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__lane_id_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_ctx_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_key_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__target_key_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v3 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v4 = 0U;
    __Vdlyvset__dma_research_observer__DOT__event_ram_q__v5 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v0 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v1 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v2 = 0U;
    __Vdlyvset__dma_research_observer__DOT__plru_snap_q__v3 = 0U;
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
        vlTOPp->dma_research_observer__DOT__carry_in_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__carry_in_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__cur_a_flags_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_flags_n;
        vlTOPp->dma_research_observer__DOT__carry_out_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__carry_out_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__cost_cycles_q 
            = vlTOPp->dma_research_observer__DOT__cost_cycles_n;
        vlTOPp->dma_research_observer__DOT__cost_a_done_all_q 
            = vlTOPp->dma_research_observer__DOT__cost_a_done_all_n;
        vlTOPp->dma_research_observer__DOT__win_cycles_q 
            = vlTOPp->dma_research_observer__DOT__win_cycles_n;
        vlTOPp->dma_research_observer__DOT__cur_a_dur_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_dur_n;
        vlTOPp->dma_research_observer__DOT__cur_v_main_q 
            = vlTOPp->dma_research_observer__DOT__cur_v_main_n;
        vlTOPp->dma_research_observer__DOT__cur_v_comp_q 
            = vlTOPp->dma_research_observer__DOT__cur_v_comp_n;
        vlTOPp->dma_research_observer__DOT__cur_a_main_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
        vlTOPp->dma_research_observer__DOT__cur_a_req_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_req_n;
        vlTOPp->dma_research_observer__DOT__cur_a_comp_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
        vlTOPp->dma_research_observer__DOT__cur_a_miss_q 
            = vlTOPp->dma_research_observer__DOT__cur_a_miss_n;
        vlTOPp->dma_research_observer__DOT__cost_owner_err_q 
            = vlTOPp->dma_research_observer__DOT__cost_owner_err_n;
        vlTOPp->dma_research_observer__DOT__e2_ovf_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__e2_ovf_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__sample_ovf_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__sample_ovf_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__cost_err_q 
            = vlTOPp->dma_research_observer__DOT__cost_err_n;
        vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q 
            = vlTOPp->dma_research_observer__DOT__e2a_done_cnt_n;
        vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q 
            = vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_n;
        vlTOPp->dma_research_observer__DOT__dst_iova_q 
            = vlTOPp->dma_research_observer__DOT__dst_iova_n;
        vlTOPp->dma_research_observer__DOT__src_iova_q 
            = vlTOPp->dma_research_observer__DOT__src_iova_n;
        vlTOPp->dma_research_observer__DOT__e2_err_q 
            = vlTOPp->dma_research_observer__DOT__e2_err_n;
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
        vlTOPp->dma_research_observer__DOT__carry_in_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_flags_q = 0U;
        vlTOPp->dma_research_observer__DOT__carry_out_q = 0U;
        vlTOPp->dma_research_observer__DOT__cost_cycles_q = 0U;
        vlTOPp->dma_research_observer__DOT__cost_a_done_all_q = 0U;
        vlTOPp->dma_research_observer__DOT__win_cycles_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_dur_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_v_main_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_v_comp_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_main_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_req_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_comp_q = 0U;
        vlTOPp->dma_research_observer__DOT__cur_a_miss_q = 0U;
        vlTOPp->dma_research_observer__DOT__cost_owner_err_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2_ovf_q = 0U;
        vlTOPp->dma_research_observer__DOT__sample_ovf_q = 0U;
        vlTOPp->dma_research_observer__DOT__cost_err_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q = 0U;
        vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q = 0U;
        vlTOPp->dma_research_observer__DOT__dst_iova_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__src_iova_q = 0ULL;
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
    vlTOPp->dma_research_observer__DOT__window_closed_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__window_closed_n));
    vlTOPp->dma_research_observer__DOT__window_open_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__window_open_n));
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__fp_done_q 
            = vlTOPp->dma_research_observer__DOT__fp_done_n;
        vlTOPp->dma_research_observer__DOT__dst_set_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__dst_set_n) 
               & 1U);
        vlTOPp->dma_research_observer__DOT__src_set_q 
            = ((IData)(vlTOPp->dma_research_observer__DOT__src_set_n) 
               & 1U);
    } else {
        vlTOPp->dma_research_observer__DOT__fp_done_q = 0U;
        vlTOPp->dma_research_observer__DOT__dst_set_q = 0U;
        vlTOPp->dma_research_observer__DOT__src_set_q = 0U;
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_observer__DOT__acc_freeze) {
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v0 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0U][0U])));
            __Vdlyvset__dma_research_observer__DOT__snap_freeze_q__v0 = 1U;
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v1 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v2 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [1U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [1U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v3 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [1U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [1U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v4 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [2U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [2U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v5 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [2U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [2U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v6 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [3U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [3U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v7 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [3U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [3U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v8 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [4U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [4U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v9 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [4U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [4U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v10 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [5U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [5U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v11 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [5U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [5U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v12 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [6U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [6U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v13 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [6U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [6U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v14 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [7U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [7U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v15 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [7U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [7U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v16 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [8U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [8U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v17 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [8U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [8U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v18 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [9U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [9U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v19 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [9U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [9U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v20 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xaU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xaU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v21 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xaU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xaU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v22 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xbU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xbU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v23 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xbU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xbU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v24 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xcU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xcU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v25 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xcU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xcU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v26 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xdU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xdU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v27 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xdU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xdU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v28 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xeU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xeU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v29 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xeU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xeU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v30 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xfU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xfU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v31 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xfU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xfU][2U])));
        }
    }
    if (vlTOPp->rst_ni) {
        if (vlTOPp->dma_research_observer__DOT__acc_a) {
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v0 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0U][0U])));
            __Vdlyvset__dma_research_observer__DOT__snap_warm_q__v0 = 1U;
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v1 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v2 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [1U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [1U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v3 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [1U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [1U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v4 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [2U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [2U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v5 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [2U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [2U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v6 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [3U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [3U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v7 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [3U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [3U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v8 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [4U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [4U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v9 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [4U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [4U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v10 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [5U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [5U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v11 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [5U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [5U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v12 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [6U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [6U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v13 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [6U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [6U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v14 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [7U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [7U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v15 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [7U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [7U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v16 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [8U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [8U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v17 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [8U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [8U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v18 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [9U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [9U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v19 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [9U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [9U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v20 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xaU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xaU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v21 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xaU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xaU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v22 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xbU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xbU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v23 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xbU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xbU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v24 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xcU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xcU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v25 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xcU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xcU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v26 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xdU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xdU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v27 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xdU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xdU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v28 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xeU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xeU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v29 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xeU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xeU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v30 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xfU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xfU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v31 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xfU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xfU][2U])));
        }
    }
    if (vlTOPp->rst_ni) {
        if ((((((IData)(vlTOPp->dma_research_observer__DOT__win_act) 
                & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok)) 
               & ((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                  >> 1U)) & (0xfU == (0xfU & (IData)(vlTOPp->dma_research_observer__DOT__eligible_q)))) 
             & (0x100U > (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q)))) {
            __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0 
                = vlTOPp->done_id_i[1U];
            __Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0 = 1U;
            __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0 
                = (0x3ffU & ((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q) 
                             << 2U));
            __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v1 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_main_n)) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_comp_n)));
            __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v1 
                = (0x3ffU & ((IData)(1U) + (0xffffU 
                                            & ((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q) 
                                               << 2U))));
            __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v2 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_req_n)) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_miss_n)));
            __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v2 
                = (0x3ffU & ((IData)(2U) + (0xffffU 
                                            & ((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q) 
                                               << 2U))));
            __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v3 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_dur_n)) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_flags_n)));
            __Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v3 
                = (0x3ffU & ((IData)(3U) + (0xffffU 
                                            & ((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q) 
                                               << 2U))));
        }
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
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__target_ctx_valid_q 
            = vlTOPp->dma_research_observer__DOT__target_ctx_valid_n;
        vlTOPp->dma_research_observer__DOT__lane_active_q 
            = vlTOPp->dma_research_observer__DOT__lane_active_n;
        vlTOPp->dma_research_observer__DOT__target_valid_q 
            = vlTOPp->dma_research_observer__DOT__target_valid_n;
    } else {
        vlTOPp->dma_research_observer__DOT__target_ctx_valid_q = 0U;
        vlTOPp->dma_research_observer__DOT__lane_active_q = 0U;
        vlTOPp->dma_research_observer__DOT__target_valid_q = 0U;
    }
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
    vlTOPp->dma_research_observer__DOT__cost_active_q 
        = ((IData)(vlTOPp->rst_ni) & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_n));
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
    vlTOPp->dma_research_observer__DOT__mode_q = ((IData)(vlTOPp->rst_ni)
                                                   ? (IData)(vlTOPp->dma_research_observer__DOT__mode_n)
                                                   : 0U);
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
    if (vlTOPp->rst_ni) {
        if (((((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                 | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
               & (IData)(vlTOPp->update_i)) & (IData)(vlTOPp->dma_research_observer__DOT__slot_oh_valid)) 
             & (0x100U > (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))) {
            vlTOPp->dma_research_observer__DOT____Vlvbound3 
                = vlTOPp->dma_research_observer__DOT__epoch_q;
            if ((0x5ffU >= (0x7ffU & ((IData)(6U) * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))) {
                __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0 
                    = vlTOPp->dma_research_observer__DOT____Vlvbound3;
                __Vdlyvset__dma_research_observer__DOT__event_ram_q__v0 = 1U;
                __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0 
                    = (0x7ffU & ((IData)(6U) * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)));
            }
            vlTOPp->dma_research_observer__DOT____Vlvbound4 
                = vlTOPp->dma_research_observer__DOT__evt_meta;
            if ((0x5ffU >= (0x7ffU & ((IData)(1U) + 
                                      (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))))) {
                __Vdlyvval__dma_research_observer__DOT__event_ram_q__v1 
                    = vlTOPp->dma_research_observer__DOT____Vlvbound4;
                __Vdlyvset__dma_research_observer__DOT__event_ram_q__v1 = 1U;
                __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v1 
                    = (0x7ffU & ((IData)(1U) + (0xffffU 
                                                & ((IData)(6U) 
                                                   * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))));
            }
            vlTOPp->dma_research_observer__DOT____Vlvbound5 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [vlTOPp->dma_research_observer__DOT__slot_idx][1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->entries_i
                                                [vlTOPp->dma_research_observer__DOT__slot_idx][0U])));
            if ((0x5ffU >= (0x7ffU & ((IData)(2U) + 
                                      (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))))) {
                __Vdlyvval__dma_research_observer__DOT__event_ram_q__v2 
                    = vlTOPp->dma_research_observer__DOT____Vlvbound5;
                __Vdlyvset__dma_research_observer__DOT__event_ram_q__v2 = 1U;
                __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v2 
                    = (0x7ffU & ((IData)(2U) + (0xffffU 
                                                & ((IData)(6U) 
                                                   * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))));
            }
            vlTOPp->dma_research_observer__DOT____Vlvbound6 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [vlTOPp->dma_research_observer__DOT__slot_idx][3U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->entries_i
                                                [vlTOPp->dma_research_observer__DOT__slot_idx][2U])));
            if ((0x5ffU >= (0x7ffU & ((IData)(3U) + 
                                      (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))))) {
                __Vdlyvval__dma_research_observer__DOT__event_ram_q__v3 
                    = vlTOPp->dma_research_observer__DOT____Vlvbound6;
                __Vdlyvset__dma_research_observer__DOT__event_ram_q__v3 = 1U;
                __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v3 
                    = (0x7ffU & ((IData)(3U) + (0xffffU 
                                                & ((IData)(6U) 
                                                   * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))));
            }
            vlTOPp->dma_research_observer__DOT____Vlvbound7 
                = (((QData)((IData)(vlTOPp->entries_next_i
                                    [vlTOPp->dma_research_observer__DOT__slot_idx][1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->entries_next_i
                                                [vlTOPp->dma_research_observer__DOT__slot_idx][0U])));
            if ((0x5ffU >= (0x7ffU & ((IData)(4U) + 
                                      (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))))) {
                __Vdlyvval__dma_research_observer__DOT__event_ram_q__v4 
                    = vlTOPp->dma_research_observer__DOT____Vlvbound7;
                __Vdlyvset__dma_research_observer__DOT__event_ram_q__v4 = 1U;
                __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v4 
                    = (0x7ffU & ((IData)(4U) + (0xffffU 
                                                & ((IData)(6U) 
                                                   * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))));
            }
            vlTOPp->dma_research_observer__DOT____Vlvbound8 
                = (((QData)((IData)(vlTOPp->entries_next_i
                                    [vlTOPp->dma_research_observer__DOT__slot_idx][3U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->entries_next_i
                                                [vlTOPp->dma_research_observer__DOT__slot_idx][2U])));
            if ((0x5ffU >= (0x7ffU & ((IData)(5U) + 
                                      (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))))) {
                __Vdlyvval__dma_research_observer__DOT__event_ram_q__v5 
                    = vlTOPp->dma_research_observer__DOT____Vlvbound8;
                __Vdlyvset__dma_research_observer__DOT__event_ram_q__v5 = 1U;
                __Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v5 
                    = (0x7ffU & ((IData)(5U) + (0xffffU 
                                                & ((IData)(6U) 
                                                   * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))));
            }
        }
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
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v0 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0U][0U])));
            __Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v0 = 1U;
            __Vdlyvval__dma_research_observer__DOT__plru_snap_q__v4 
                = vlTOPp->plru_i;
            vlTOPp->dma_research_observer__DOT__snapshot_valid_q 
                = (2U | (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v1 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0U][2U])));
            __Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v1 = 1U;
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v2 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [1U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [1U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v3 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [1U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [1U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v4 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [2U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [2U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v5 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [2U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [2U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v6 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [3U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [3U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v7 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [3U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [3U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v8 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [4U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [4U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v9 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [4U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [4U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v10 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [5U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [5U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v11 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [5U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [5U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v12 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [6U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [6U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v13 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [6U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [6U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v14 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [7U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [7U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v15 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [7U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [7U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v16 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [8U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [8U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v17 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [8U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [8U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v18 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [9U][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [9U][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v19 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [9U][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [9U][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v20 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xaU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xaU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v21 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xaU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xaU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v22 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xbU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xbU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v23 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xbU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xbU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v24 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xcU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xcU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v25 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xcU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xcU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v26 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xdU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xdU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v27 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xdU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xdU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v28 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xeU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xeU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v29 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xeU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xeU][2U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v30 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xfU][1U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xfU][0U])));
            __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v31 
                = (((QData)((IData)(vlTOPp->entries_i
                                    [0xfU][3U])) << 0x20U) 
                   | (QData)((IData)(vlTOPp->entries_i
                                     [0xfU][2U])));
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
        vlTOPp->dma_research_observer__DOT__cost_issue_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_issue_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_sel_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_sel_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_sel_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_sel_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_sel_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_sel_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_sel_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_comp_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_comp_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_main_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_main_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_main_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_sel_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_sel_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_sel_q__v0;
        vlTOPp->dma_research_observer__DOT__win_sel_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_sel_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_sel_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_sel_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__win_sel_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_comp_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_comp_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_comp_q__v0;
        vlTOPp->dma_research_observer__DOT__win_comp_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_comp_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_comp_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_comp_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__win_comp_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_hit_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_hit_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_hit_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_hit_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_hit_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_hit_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_hit_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_main_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_main_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_main_q__v0;
        vlTOPp->dma_research_observer__DOT__win_main_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_main_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_main_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_main_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__win_main_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_miss_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_miss_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_miss_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_miss_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_miss_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_miss_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_miss_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_miss_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_miss_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_miss_q__v0;
        vlTOPp->dma_research_observer__DOT__win_miss_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_miss_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_miss_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_miss_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__win_miss_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_hit_q__v0) {
        vlTOPp->dma_research_observer__DOT__win_hit_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__win_hit_q__v0;
        vlTOPp->dma_research_observer__DOT__win_hit_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__win_hit_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__win_hit_q__v2) {
        vlTOPp->dma_research_observer__DOT__win_hit_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__win_hit_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_done_q__v0) {
        vlTOPp->dma_research_observer__DOT__cost_done_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_done_q__v0;
        vlTOPp->dma_research_observer__DOT__cost_done_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__cost_done_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__cost_done_q__v2) {
        vlTOPp->dma_research_observer__DOT__cost_done_q[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__cost_done_q[1U] = 0U;
    }
    if (__Vdlyvset__dma_research_observer__DOT__snap_freeze_q__v0) {
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v0;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v1;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v2;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v3;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v4;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v5;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[6U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v6;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[7U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v7;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[8U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v8;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[9U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v9;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xaU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v10;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xbU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v11;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xcU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v12;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xdU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v13;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xeU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v14;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0xfU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v15;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x10U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v16;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x11U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v17;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x12U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v18;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x13U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v19;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x14U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v20;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x15U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v21;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x16U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v22;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x17U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v23;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x18U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v24;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x19U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v25;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1aU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v26;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1bU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v27;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1cU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v28;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1dU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v29;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1eU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v30;
        vlTOPp->dma_research_observer__DOT__snap_freeze_q[0x1fU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_freeze_q__v31;
    }
    if (__Vdlyvset__dma_research_observer__DOT__snap_warm_q__v0) {
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v0;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v1;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v2;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v3;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v4;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v5;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[6U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v6;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[7U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v7;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[8U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v8;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[9U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v9;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0xaU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v10;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0xbU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v11;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0xcU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v12;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0xdU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v13;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0xeU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v14;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0xfU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v15;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x10U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v16;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x11U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v17;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x12U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v18;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x13U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v19;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x14U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v20;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x15U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v21;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x16U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v22;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x17U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v23;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x18U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v24;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x19U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v25;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1aU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v26;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1bU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v27;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1cU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v28;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1dU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v29;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1eU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v30;
        vlTOPp->dma_research_observer__DOT__snap_warm_q[0x1fU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_warm_q__v31;
    }
    if (__Vdlyvset__dma_research_observer__DOT__sample_ram_q__v0) {
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v0] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v0;
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v1] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v1;
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v2] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v2;
        vlTOPp->dma_research_observer__DOT__sample_ram_q[__Vdlyvdim0__dma_research_observer__DOT__sample_ram_q__v3] 
            = __Vdlyvval__dma_research_observer__DOT__sample_ram_q__v3;
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
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v0) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v0] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v1) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v1] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v1;
    }
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v2) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v2] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v2;
    }
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v3) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v3] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v3;
    }
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v4) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v4] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v4;
    }
    if (__Vdlyvset__dma_research_observer__DOT__event_ram_q__v5) {
        vlTOPp->dma_research_observer__DOT__event_ram_q[__Vdlyvdim0__dma_research_observer__DOT__event_ram_q__v5] 
            = __Vdlyvval__dma_research_observer__DOT__event_ram_q__v5;
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
    if (__Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v0) {
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
    if (__Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v0) {
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v0;
    }
    if (__Vdlyvset__dma_research_observer__DOT__snap_deadline_q__v1) {
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[1U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v1;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[2U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v2;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[3U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v3;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[4U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v4;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[5U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v5;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[6U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v6;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[7U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v7;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[8U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v8;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[9U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v9;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xaU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v10;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xbU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v11;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xcU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v12;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xdU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v13;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xeU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v14;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0xfU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v15;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x10U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v16;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x11U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v17;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x12U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v18;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x13U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v19;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x14U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v20;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x15U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v21;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x16U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v22;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x17U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v23;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x18U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v24;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x19U] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v25;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1aU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v26;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1bU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v27;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1cU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v28;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1dU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v29;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1eU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v30;
        vlTOPp->dma_research_observer__DOT__snap_deadline_q[0x1fU] 
            = __Vdlyvval__dma_research_observer__DOT__snap_deadline_q__v31;
    }
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__lane_in_window_q 
            = vlTOPp->dma_research_observer__DOT__lane_in_window_n;
        vlTOPp->dma_research_observer__DOT__sample_count_q 
            = vlTOPp->dma_research_observer__DOT__sample_count_n;
        vlTOPp->dma_research_observer__DOT__eligible_q 
            = vlTOPp->dma_research_observer__DOT__eligible_n;
    } else {
        vlTOPp->dma_research_observer__DOT__lane_in_window_q = 0U;
        vlTOPp->dma_research_observer__DOT__sample_count_q = 0U;
        vlTOPp->dma_research_observer__DOT__eligible_q = 0U;
    }
    vlTOPp->dma_research_observer__DOT__win_act = (
                                                   ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
                                                   & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q)));
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
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__event_count_q 
            = vlTOPp->dma_research_observer__DOT__event_count_n;
        vlTOPp->dma_research_observer__DOT__epoch_q 
            = vlTOPp->dma_research_observer__DOT__epoch_n;
    } else {
        vlTOPp->dma_research_observer__DOT__event_count_q = 0U;
        vlTOPp->dma_research_observer__DOT__epoch_q = 0ULL;
    }
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
    if (vlTOPp->rst_ni) {
        vlTOPp->dma_research_observer__DOT__a_elapsed_q 
            = vlTOPp->dma_research_observer__DOT__a_elapsed_n;
        vlTOPp->dma_research_observer__DOT__e2_phase_q 
            = vlTOPp->dma_research_observer__DOT__e2_phase_n;
    } else {
        vlTOPp->dma_research_observer__DOT__a_elapsed_q = 0ULL;
        vlTOPp->dma_research_observer__DOT__e2_phase_q = 0U;
    }
    vlTOPp->dma_research_observer__DOT__e2_active_phase 
        = ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
             | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
            | (6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
           | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)));
    vlTOPp->dma_research_observer__DOT__lanes_en = 
        ((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))
          ? (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase)
          : ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
             & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)));
}

VL_INLINE_OPT void Vdma_research_observer::_combo__TOP__4(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_combo__TOP__4\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
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
    vlTOPp->dma_research_observer__DOT__cmd_n = vlTOPp->dma_research_observer__DOT__cmd_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U != (IData)(vlTOPp->index_i))) {
            if ((5U == (IData)(vlTOPp->index_i))) {
                vlTOPp->dma_research_observer__DOT__cmd_n 
                    = (0xfU & (IData)(vlTOPp->wdata_i));
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_act_issue 
        = ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & ((IData)(vlTOPp->dma_research_observer__DOT__window_open_q) 
                | (IData)(vlTOPp->p4m_open_i))) & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q))) 
           & (~ (IData)(vlTOPp->p4m_close_i)));
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
    vlTOPp->dma_research_observer__DOT__cost_cycles_n 
        = vlTOPp->dma_research_observer__DOT__cost_cycles_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        vlTOPp->dma_research_observer__DOT__cost_cycles_n 
            = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__cost_cycles_q)));
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_cycles_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_cycles_n 
        = vlTOPp->dma_research_observer__DOT__win_cycles_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->dma_research_observer__DOT__win_act) {
            vlTOPp->dma_research_observer__DOT__win_cycles_n 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__win_cycles_q)));
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_cycles_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__window_open_n 
        = vlTOPp->dma_research_observer__DOT__window_open_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->p4m_open_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)))) {
                vlTOPp->dma_research_observer__DOT__window_open_n = 1U;
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__window_open_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__mode_n = vlTOPp->dma_research_observer__DOT__mode_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U == (IData)(vlTOPp->index_i))) {
            if ((1U & (~ ((((0ULL != (0x3fffffffffffffffULL 
                                      & (vlTOPp->wdata_i 
                                         >> 2U))) | 
                            (3U == (3U & (IData)(vlTOPp->wdata_i)))) 
                           | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                          | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))))) {
                vlTOPp->dma_research_observer__DOT__mode_n 
                    = (3U & (IData)(vlTOPp->wdata_i));
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_sel_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_sel_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_sel_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_sel_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->select_fire_i) {
            if (vlTOPp->dma_research_observer__DOT__sel_known) {
                vlTOPp->dma_research_observer__DOT__cost_sel_n[(
                                                                (0xbU 
                                                                 == vlTOPp->select_did_i)
                                                                 ? 1U
                                                                 : 0U)] 
                    = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_sel_q
                                [((0xbU == vlTOPp->select_did_i)
                                   ? 1U : 0U)]));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_sel_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_sel_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_comp_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_comp_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_comp_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((4U == (IData)(vlTOPp->ctl_state_i))) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                vlTOPp->dma_research_observer__DOT__cost_comp_n[(
                                                                 (0xbU 
                                                                  == vlTOPp->ctl_did_i)
                                                                  ? 1U
                                                                  : 0U)] 
                    = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_comp_q
                                [((0xbU == vlTOPp->ctl_did_i)
                                   ? 1U : 0U)]));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_comp_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_comp_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_main_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_main_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_main_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->dma_research_observer__DOT__main_cyc) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                vlTOPp->dma_research_observer__DOT__cost_main_n[(
                                                                 (0xbU 
                                                                  == vlTOPp->ctl_did_i)
                                                                  ? 1U
                                                                  : 0U)] 
                    = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_main_q
                                [((0xbU == vlTOPp->ctl_did_i)
                                   ? 1U : 0U)]));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_main_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_main_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_v_comp_n 
        = vlTOPp->dma_research_observer__DOT__cur_v_comp_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if (((4U == (IData)(vlTOPp->ctl_state_i)) 
                 & (0xaU == vlTOPp->ctl_did_i))) {
                vlTOPp->dma_research_observer__DOT__cur_v_comp_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_v_comp_q);
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_v_comp_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_v_main_n 
        = vlTOPp->dma_research_observer__DOT__cur_v_main_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if (((IData)(vlTOPp->dma_research_observer__DOT__main_cyc) 
                 & (0xaU == vlTOPp->ctl_did_i))) {
                vlTOPp->dma_research_observer__DOT__cur_v_main_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_v_main_q);
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_v_main_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__window_closed_n 
        = vlTOPp->dma_research_observer__DOT__window_closed_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->p4m_open_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)))) {
                vlTOPp->dma_research_observer__DOT__window_closed_n = 0U;
            }
        }
        if (vlTOPp->p4m_close_i) {
            if ((1U & (~ ((~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
                          | (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q))))) {
                vlTOPp->dma_research_observer__DOT__window_closed_n = 1U;
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__window_closed_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__carry_in_n 
        = vlTOPp->dma_research_observer__DOT__carry_in_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->p4m_open_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)))) {
                vlTOPp->dma_research_observer__DOT__carry_in_n 
                    = (1U & ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q) 
                             >> 1U));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__carry_in_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__src_iova_n 
        = vlTOPp->dma_research_observer__DOT__src_iova_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U != (IData)(vlTOPp->index_i))) {
            if ((5U != (IData)(vlTOPp->index_i))) {
                if ((0x30U == (IData)(vlTOPp->index_i))) {
                    if ((1U & (~ ((((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                                    | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                                   | (0U != (0xfffU 
                                             & (IData)(vlTOPp->wdata_i)))) 
                                  | ((IData)(vlTOPp->dma_research_observer__DOT__dst_set_q) 
                                     & ((0xfffffffffffffULL 
                                         & (vlTOPp->wdata_i 
                                            >> 0xcU)) 
                                        == (0xfffffffffffffULL 
                                            & (vlTOPp->dma_research_observer__DOT__dst_iova_q 
                                               >> 0xcU)))))))) {
                        vlTOPp->dma_research_observer__DOT__src_iova_n 
                            = vlTOPp->wdata_i;
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__dst_iova_n 
        = vlTOPp->dma_research_observer__DOT__dst_iova_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U != (IData)(vlTOPp->index_i))) {
            if ((5U != (IData)(vlTOPp->index_i))) {
                if ((0x30U != (IData)(vlTOPp->index_i))) {
                    if ((0x31U == (IData)(vlTOPp->index_i))) {
                        if ((1U & (~ ((((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                                        | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                                       | (0U != (0xfffU 
                                                 & (IData)(vlTOPp->wdata_i)))) 
                                      | ((IData)(vlTOPp->dma_research_observer__DOT__src_set_q) 
                                         & ((0xfffffffffffffULL 
                                             & (vlTOPp->wdata_i 
                                                >> 0xcU)) 
                                            == (0xfffffffffffffULL 
                                                & (vlTOPp->dma_research_observer__DOT__src_iova_q 
                                                   >> 0xcU)))))))) {
                            vlTOPp->dma_research_observer__DOT__dst_iova_n 
                                = vlTOPp->wdata_i;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_hit_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_hit_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_hit_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_hit_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->lookup_i) {
            if (vlTOPp->dma_research_observer__DOT__lu_known) {
                if (vlTOPp->lookup_hit_i) {
                    vlTOPp->dma_research_observer__DOT__cost_hit_n[(
                                                                    (0xbU 
                                                                     == vlTOPp->lookup_did_i)
                                                                     ? 1U
                                                                     : 0U)] 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_hit_q
                                    [((0xbU == vlTOPp->lookup_did_i)
                                       ? 1U : 0U)]));
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_hit_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_hit_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_main_n[0U] 
        = vlTOPp->dma_research_observer__DOT__win_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT__win_main_n[1U] 
        = vlTOPp->dma_research_observer__DOT__win_main_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->dma_research_observer__DOT__main_cyc) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    vlTOPp->dma_research_observer__DOT__win_main_n[(
                                                                    (0xbU 
                                                                     == vlTOPp->ctl_did_i)
                                                                     ? 1U
                                                                     : 0U)] 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__win_main_q
                                    [((0xbU == vlTOPp->ctl_did_i)
                                       ? 1U : 0U)]));
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_main_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__win_main_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_miss_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_miss_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_miss_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_miss_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->lookup_i) {
            if (vlTOPp->dma_research_observer__DOT__lu_known) {
                if ((1U & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                    vlTOPp->dma_research_observer__DOT__cost_miss_n[(
                                                                     (0xbU 
                                                                      == vlTOPp->lookup_did_i)
                                                                      ? 1U
                                                                      : 0U)] 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_miss_q
                                    [((0xbU == vlTOPp->lookup_did_i)
                                       ? 1U : 0U)]));
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_miss_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_miss_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_comp_n[0U] 
        = vlTOPp->dma_research_observer__DOT__win_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT__win_comp_n[1U] 
        = vlTOPp->dma_research_observer__DOT__win_comp_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((4U == (IData)(vlTOPp->ctl_state_i))) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    vlTOPp->dma_research_observer__DOT__win_comp_n[(
                                                                    (0xbU 
                                                                     == vlTOPp->ctl_did_i)
                                                                     ? 1U
                                                                     : 0U)] 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__win_comp_q
                                    [((0xbU == vlTOPp->ctl_did_i)
                                       ? 1U : 0U)]));
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_comp_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__win_comp_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_sel_n[0U] 
        = vlTOPp->dma_research_observer__DOT__win_sel_q
        [0U];
    vlTOPp->dma_research_observer__DOT__win_sel_n[1U] 
        = vlTOPp->dma_research_observer__DOT__win_sel_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->select_fire_i) {
            if (vlTOPp->dma_research_observer__DOT__sel_known) {
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    vlTOPp->dma_research_observer__DOT__win_sel_n[(
                                                                   (0xbU 
                                                                    == vlTOPp->select_did_i)
                                                                    ? 1U
                                                                    : 0U)] 
                        = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__win_sel_q
                                    [((0xbU == vlTOPp->select_did_i)
                                       ? 1U : 0U)]));
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_sel_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__win_sel_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_hit_n[0U] 
        = vlTOPp->dma_research_observer__DOT__win_hit_q
        [0U];
    vlTOPp->dma_research_observer__DOT__win_hit_n[1U] 
        = vlTOPp->dma_research_observer__DOT__win_hit_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->lookup_i) {
            if (vlTOPp->dma_research_observer__DOT__lu_known) {
                if (vlTOPp->lookup_hit_i) {
                    if (vlTOPp->dma_research_observer__DOT__win_act) {
                        vlTOPp->dma_research_observer__DOT__win_hit_n[(
                                                                       (0xbU 
                                                                        == vlTOPp->lookup_did_i)
                                                                        ? 1U
                                                                        : 0U)] 
                            = (0xffU & ((IData)(1U) 
                                        + vlTOPp->dma_research_observer__DOT__win_hit_q
                                        [((0xbU == vlTOPp->lookup_did_i)
                                           ? 1U : 0U)]));
                    }
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_hit_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__win_hit_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__win_miss_n[0U] 
        = vlTOPp->dma_research_observer__DOT__win_miss_q
        [0U];
    vlTOPp->dma_research_observer__DOT__win_miss_n[1U] 
        = vlTOPp->dma_research_observer__DOT__win_miss_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->lookup_i) {
            if (vlTOPp->dma_research_observer__DOT__lu_known) {
                if ((1U & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                    if (vlTOPp->dma_research_observer__DOT__win_act) {
                        vlTOPp->dma_research_observer__DOT__win_miss_n[(
                                                                        (0xbU 
                                                                         == vlTOPp->lookup_did_i)
                                                                         ? 1U
                                                                         : 0U)] 
                            = (0xffU & ((IData)(1U) 
                                        + vlTOPp->dma_research_observer__DOT__win_miss_q
                                        [((0xbU == vlTOPp->lookup_did_i)
                                           ? 1U : 0U)]));
                    }
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_miss_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__win_miss_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
        = vlTOPp->dma_research_observer__DOT__cost_owner_err_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->dma_research_observer__DOT__main_cyc) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__main_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q)));
            }
        }
        if ((4U == (IData)(vlTOPp->ctl_state_i))) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__main_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q)));
            }
        }
        if (vlTOPp->select_fire_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__sel_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q)));
            }
        }
        if (vlTOPp->lookup_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__lu_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q)));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_owner_err_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__status_q = 0ULL;
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffffffeULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | (IData)((IData)((0U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffffffdULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) 
            << 1U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffffffbULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) 
            << 2U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffffff7ULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) 
            << 3U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffffffefULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((4U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) 
            << 4U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffffffdfULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((5U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) 
            << 5U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffffffbfULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) 
            << 6U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffff8ffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
            << 8U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffff7ffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)))) 
            << 0xbU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffffefffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)))) 
            << 0xcU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffffdfffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
            << 0xdU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffffbfffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
            << 0xeU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffff7fffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__window_closed_q)) 
            << 0xfU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffeffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U & (IData)(vlTOPp->dma_research_observer__DOT__target_valid_q)))) 
            << 0x10U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffdffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U & ((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
                                   >> 1U)))) << 0x11U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffffbffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U & (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q)))) 
            << 0x12U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffff7ffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U & ((IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q) 
                                   >> 1U)))) << 0x13U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffefffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((1U & ((IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q) 
                                   >> 2U)))) << 0x14U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffdfffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->quiet_i)) << 0x15U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffffbfffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->supported_i)) << 0x16U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffff7fffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_err_q)))) 
            << 0x17U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffeffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((0U != (IData)(vlTOPp->dma_research_observer__DOT__cost_err_q)))) 
            << 0x18U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffdffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((3U == (IData)(vlTOPp->dma_research_observer__DOT__fp_done_q)))) 
            << 0x19U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffffbffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__sample_ovf_q)) 
            << 0x1aU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffffff7ffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_ovf_q)) 
            << 0x1bU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffffefffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_err_q)) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__cost_err_q))))) 
            << 0x1cU));
    vlTOPp->dma_research_observer__DOT__status_q = 
        (0xffffffff1fffffffULL & vlTOPp->dma_research_observer__DOT__status_q);
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffff8ffffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)(vlTOPp->ctl_state_i)) << 0x20U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xfffffff7ffffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((0xaU == vlTOPp->ctl_did_i))) 
            << 0x23U));
    vlTOPp->dma_research_observer__DOT__status_q = 
        ((0xffffffefffffffffULL & vlTOPp->dma_research_observer__DOT__status_q) 
         | ((QData)((IData)((0xbU == vlTOPp->ctl_did_i))) 
            << 0x24U));
    vlTOPp->dma_research_observer__DOT__acc_probe = 
        (((((((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
              & (0ULL == (0xfffffffffffffffULL & (vlTOPp->wdata_i 
                                                  >> 4U)))) 
             & (1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
            & (3U == (0xfU & (IData)(vlTOPp->wdata_i)))) 
           & (6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
          & (IData)(vlTOPp->quiet_i)) & (IData)(vlTOPp->supported_i));
    vlTOPp->dma_research_observer__DOT__acc_freeze 
        = ((((((((((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
                   & (0ULL == (0xfffffffffffffffULL 
                               & (vlTOPp->wdata_i >> 4U)))) 
                  & (1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
                 & (4U == (0xfU & (IData)(vlTOPp->wdata_i)))) 
                & (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
               & (IData)(vlTOPp->quiet_i)) & (IData)(vlTOPp->supported_i)) 
             & (3U == (IData)(vlTOPp->dma_research_observer__DOT__fp_done_q))) 
            & (~ (IData)(vlTOPp->update_i))) & (~ (IData)(vlTOPp->invalidate_i)));
    vlTOPp->dma_research_observer__DOT__acc_clear = 
        ((((((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
             & (0ULL == (0xfffffffffffffffULL & (vlTOPp->wdata_i 
                                                 >> 4U)))) 
            & (6U == (0xfU & (IData)(vlTOPp->wdata_i)))) 
           & (IData)(vlTOPp->quiet_i)) & (~ (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))) 
         & (((0U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
             | (2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
            | ((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
               & (((0U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                   | (4U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                  | (5U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))));
    vlTOPp->dma_research_observer__DOT__acc_warm = 
        ((((((((((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
                 & (0ULL == (0xfffffffffffffffULL & 
                             (vlTOPp->wdata_i >> 4U)))) 
                & (1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
               & (1U == (0xfU & (IData)(vlTOPp->wdata_i)))) 
              & (0U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
             & (IData)(vlTOPp->quiet_i)) & (IData)(vlTOPp->supported_i)) 
           & ((IData)(vlTOPp->dma_research_observer__DOT__src_set_q) 
              & (IData)(vlTOPp->dma_research_observer__DOT__dst_set_q))) 
          & (~ (IData)(vlTOPp->update_i))) & (~ (IData)(vlTOPp->invalidate_i)));
    vlTOPp->dma_research_observer__DOT__cost_active_n 
        = vlTOPp->dma_research_observer__DOT__cost_active_q;
    if (vlTOPp->dma_research_observer__DOT__abort_cmd) {
        if ((1U & (~ ((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))))) {
            if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                 & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))) {
                vlTOPp->dma_research_observer__DOT__cost_active_n = 0U;
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->p4m_close_i) {
            if ((1U & (~ ((~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
                          | (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q))))) {
                vlTOPp->dma_research_observer__DOT__cost_active_n = 0U;
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_active_n = 1U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__lane_id_n[0U] 
        = vlTOPp->dma_research_observer__DOT__lane_id_q
        [0U];
    vlTOPp->dma_research_observer__DOT__lane_id_n[1U] 
        = vlTOPp->dma_research_observer__DOT__lane_id_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__lanes_en) {
        if ((1U & (((IData)(vlTOPp->issue_i) >> 1U) 
                   & (~ ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q) 
                         >> 1U))))) {
            vlTOPp->dma_research_observer__DOT__lane_id_n[1U] 
                = vlTOPp->issue_id_i[1U];
        }
        if ((1U & ((IData)(vlTOPp->issue_i) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))))) {
            vlTOPp->dma_research_observer__DOT__lane_id_n[0U] 
                = vlTOPp->issue_id_i[0U];
        }
    }
    vlTOPp->dma_research_observer__DOT__lane_proto_err 
        = ((IData)(vlTOPp->dma_research_observer__DOT__lanes_en) 
           & ((((((((IData)(vlTOPp->issue_i) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
                   >> 1U) | (((IData)(vlTOPp->done_i) 
                              >> 1U) & (~ ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q) 
                                           >> 1U)))) 
                 | ((((IData)(vlTOPp->done_i) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
                     >> 1U) & (vlTOPp->done_id_i[1U] 
                               != vlTOPp->dma_research_observer__DOT__lane_id_q
                               [1U]))) | ((IData)(vlTOPp->issue_i) 
                                          & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) 
               | ((IData)(vlTOPp->done_i) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)))) 
              | (((IData)(vlTOPp->done_i) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
                 & (vlTOPp->done_id_i[0U] != vlTOPp->dma_research_observer__DOT__lane_id_q
                    [0U]))));
    vlTOPp->dma_research_observer__DOT__a_done_ok = 
        ((((IData)(vlTOPp->dma_research_observer__DOT__lanes_en) 
           & ((IData)(vlTOPp->done_i) >> 1U)) & ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q) 
                                                 >> 1U)) 
         & (vlTOPp->done_id_i[1U] == vlTOPp->dma_research_observer__DOT__lane_id_q
            [1U]));
    vlTOPp->dma_research_observer__DOT__a_issue_ok 
        = (((IData)(vlTOPp->dma_research_observer__DOT__lanes_en) 
            & ((IData)(vlTOPp->issue_i) >> 1U)) & (~ 
                                                   ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q) 
                                                    >> 1U)));
    vlTOPp->dma_research_observer__DOT__evt_meta = 0ULL;
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffffffffffff0000ULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | (IData)((IData)(vlTOPp->replace_mask_i)));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffffffffffe0ffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__changed_index)) 
            << 0x10U));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffffffffff1fffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
            << 0x15U));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffff000000ffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)(vlTOPp->update_did_i)) 
            << 0x18U));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        (0x1000000000000ULL | vlTOPp->dma_research_observer__DOT__evt_meta);
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xfffdffffffffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)((0U == (IData)(vlTOPp->dma_research_observer__DOT__changed_count)))) 
            << 0x31U));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xfffbffffffffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)((1U < (IData)(vlTOPp->dma_research_observer__DOT__changed_count)))) 
            << 0x32U));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xfff7ffffffffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)(((1U == (IData)(vlTOPp->dma_research_observer__DOT__changed_count)) 
                             & ((IData)(vlTOPp->dma_research_observer__DOT__changed_mask) 
                                != (IData)(vlTOPp->replace_mask_i))))) 
            << 0x33U));
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[0U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[1U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[2U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[3U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__k[1U] 
                                          >> 0x12U))));
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[0U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[1U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[2U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[3U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__k[1U] 
                                          >> 0x12U))));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffefffffffffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)(((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
                               & (vlTOPp->entries_i
                                  [vlTOPp->dma_research_observer__DOT__slot_idx][1U] 
                                  >> 0x13U)) & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__8__Vfuncout)) 
                             & ((~ (vlTOPp->entries_next_i
                                    [vlTOPp->dma_research_observer__DOT__slot_idx][1U] 
                                    >> 0x13U)) | (~ (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__9__Vfuncout)))))) 
            << 0x34U));
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[0U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[1U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[2U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[3U] 
        = vlTOPp->entries_next_i[vlTOPp->dma_research_observer__DOT__slot_idx][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__k[1U] 
                                          >> 0x12U))));
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[0U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][0U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[1U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][1U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[2U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][2U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[3U] 
        = vlTOPp->entries_i[vlTOPp->dma_research_observer__DOT__slot_idx][3U];
    vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__Vfuncout 
        = ((((((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[2U]) 
               == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[2U])) 
              & ((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[1U] 
                               << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[0U] 
                                         >> 0x1dU))) 
                 == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[1U] 
                                  << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[0U] 
                                            >> 0x1dU))))) 
             & ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[0U]) 
                == (0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[0U]))) 
            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[1U] 
                      >> 0x11U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[1U] 
                                           >> 0x11U)))) 
           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__e[1U] 
                     >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__k[1U] 
                                          >> 0x12U))));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffdfffffffffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)((((((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
                                >> 1U) & (vlTOPp->entries_i
                                          [vlTOPp->dma_research_observer__DOT__slot_idx][1U] 
                                          >> 0x13U)) 
                              & (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__10__Vfuncout)) 
                             & ((~ (vlTOPp->entries_next_i
                                    [vlTOPp->dma_research_observer__DOT__slot_idx][1U] 
                                    >> 0x13U)) | (~ (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__id_match__11__Vfuncout)))))) 
            << 0x35U));
    vlTOPp->dma_research_observer__DOT__evt_meta = 
        ((0xffbfffffffffffffULL & vlTOPp->dma_research_observer__DOT__evt_meta) 
         | ((QData)((IData)((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__slot_oh_valid))))) 
            << 0x36U));
    vlTOPp->dma_research_observer__DOT__acc_a = (((
                                                   ((((((((((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
                                                            & (0ULL 
                                                               == 
                                                               (0xfffffffffffffffULL 
                                                                & (vlTOPp->wdata_i 
                                                                   >> 4U)))) 
                                                           & (1U 
                                                              == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
                                                          & (2U 
                                                             == 
                                                             (0xfU 
                                                              & (IData)(vlTOPp->wdata_i)))) 
                                                         & (1U 
                                                            == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                                                        & (IData)(vlTOPp->quiet_i)) 
                                                       & (IData)(vlTOPp->supported_i)) 
                                                      & (3U 
                                                         == (IData)(vlTOPp->dma_research_observer__DOT__target_valid_q))) 
                                                     & (IData)(vlTOPp->dma_research_observer__DOT__tgt_present0)) 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__tgt_present1)) 
                                                   & (~ (IData)(vlTOPp->update_i))) 
                                                  & (~ (IData)(vlTOPp->invalidate_i))) 
                                                 & (IData)(vlTOPp->dma_research_observer__DOT__ctx_ok_both));
    vlTOPp->rdata_o = 0ULL;
    if ((0x100U > (IData)(vlTOPp->index_i))) {
        vlTOPp->rdata_o = ((0x1000U & (IData)(vlTOPp->index_i))
                            ? 0ULL : ((0x800U & (IData)(vlTOPp->index_i))
                                       ? 0ULL : ((0x400U 
                                                  & (IData)(vlTOPp->index_i))
                                                  ? 0ULL
                                                  : 
                                                 ((0x200U 
                                                   & (IData)(vlTOPp->index_i))
                                                   ? 0ULL
                                                   : 
                                                  ((0x100U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 0ULL
                                                    : 
                                                   ((0x80U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? 0ULL
                                                     : 
                                                    ((0x40U 
                                                      & (IData)(vlTOPp->index_i))
                                                      ? 0ULL
                                                      : 
                                                     ((0x20U 
                                                       & (IData)(vlTOPp->index_i))
                                                       ? 
                                                      ((0x10U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((8U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 0ULL
                                                         : 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 0ULL
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 0ULL
                                                            : 
                                                           (((QData)((IData)(vlTOPp->dma_research_observer__DOT__dst_set_q)) 
                                                             << 1U) 
                                                            | (QData)((IData)(vlTOPp->dma_research_observer__DOT__src_set_q))))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? vlTOPp->dma_research_observer__DOT__dst_iova_q
                                                            : vlTOPp->dma_research_observer__DOT__src_iova_q))))
                                                        : 
                                                       ((8U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 0ULL
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 0ULL
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 0ULL
                                                            : vlTOPp->dma_research_observer__DOT__e2a_last_q)))
                                                         : 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? vlTOPp->dma_research_observer__DOT__e2a_first_q
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q)))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q))
                                                            : (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__plru_snap_q
                                                                              [2U]))))
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__plru_snap_q
                                                                              [1U]))
                                                            : (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__plru_snap_q
                                                                              [0U])))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q))
                                                            : vlTOPp->dma_research_observer__DOT__freeze_ts_q)))))
                                                       : 
                                                      ((0x10U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((8U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? vlTOPp->dma_research_observer__DOT__probestart_q
                                                            : vlTOPp->dma_research_observer__DOT__deadline_q)
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__fp_done_q))
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q))))
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                             [1U][3U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                              [1U][2U])))
                                                            : 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                             [1U][1U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                              [1U][0U]))))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__fp_hit_q
                                                                             [1U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              ((vlTOPp->dma_research_observer__DOT__fp_live_oth_q
                                                                                [1U] 
                                                                                << 0x10U) 
                                                                               | vlTOPp->dma_research_observer__DOT__fp_live_self_q
                                                                               [1U]))))
                                                            : 
                                                           vlTOPp->dma_research_observer__DOT__fp_time_q
                                                           [1U])))
                                                         : 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                             [0U][3U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                              [0U][2U])))
                                                            : 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                             [0U][1U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__fp_key_q
                                                                              [0U][0U]))))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__fp_hit_q
                                                                             [0U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              ((vlTOPp->dma_research_observer__DOT__fp_live_oth_q
                                                                                [0U] 
                                                                                << 0x10U) 
                                                                               | vlTOPp->dma_research_observer__DOT__fp_live_self_q
                                                                               [0U]))))
                                                            : 
                                                           vlTOPp->dma_research_observer__DOT__fp_time_q
                                                           [0U]))
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__target_key_q
                                                                             [1U][3U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__target_key_q
                                                                              [1U][2U])))
                                                            : 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__target_key_q
                                                                             [1U][1U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__target_key_q
                                                                              [1U][0U]))))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__target_key_q
                                                                             [0U][3U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__target_key_q
                                                                              [0U][2U])))
                                                            : 
                                                           (((QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__target_key_q
                                                                             [0U][1U])) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              vlTOPp->dma_research_observer__DOT__target_key_q
                                                                              [0U][0U])))))))
                                                        : 
                                                       ((8U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__quiet_timeout_q))
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__update_count_q)))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_err_q))
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_ovf_q))))
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__inval_count_q))
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__event_count_q)))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? vlTOPp->dma_research_observer__DOT__epoch_q
                                                            : vlTOPp->dma_research_observer__DOT__a_elapsed_q)))
                                                         : 
                                                        ((4U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? vlTOPp->dma_research_observer__DOT__astart_q
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cmd_q))
                                                            : (QData)((IData)(vlTOPp->dma_research_observer__DOT__mode_q))))
                                                          : 
                                                         ((2U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 
                                                           (((QData)((IData)(vlTOPp->dma_research_observer__DOT__cost_err_q)) 
                                                             << 0x10U) 
                                                            | (QData)((IData)(vlTOPp->dma_research_observer__DOT__e2_err_q)))
                                                            : vlTOPp->dma_research_observer__DOT__status_q)
                                                           : 
                                                          ((1U 
                                                            & (IData)(vlTOPp->index_i))
                                                            ? 0x49444d414f425308ULL
                                                            : 0x49444d4152455331ULL)))))))))))));
    } else {
        if ((0x160U > (IData)(vlTOPp->index_i))) {
            vlTOPp->rdata_o = ((0x120U > (IData)(vlTOPp->index_i))
                                ? ((1U & (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q))
                                    ? vlTOPp->dma_research_observer__DOT__snap_warm_q
                                   [(0x1fU & (IData)(vlTOPp->index_i))]
                                    : 0ULL) : ((0x140U 
                                                > (IData)(vlTOPp->index_i))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q))
                                                    ? 
                                                   vlTOPp->dma_research_observer__DOT__snap_deadline_q
                                                   [
                                                   (0x1fU 
                                                    & (IData)(vlTOPp->index_i))]
                                                    : 0ULL)
                                                : (
                                                   (4U 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q))
                                                    ? 
                                                   vlTOPp->dma_research_observer__DOT__snap_freeze_q
                                                   [
                                                   (0x1fU 
                                                    & (IData)(vlTOPp->index_i))]
                                                    : 0ULL)));
        } else {
            if (((0x200U <= (IData)(vlTOPp->index_i)) 
                 & (0x800U > (IData)(vlTOPp->index_i)))) {
                vlTOPp->rdata_o = (((0xffffU & ((IData)(vlTOPp->index_i) 
                                                - (IData)(0x200U))) 
                                    < (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))
                                    ? ((0x5ffU >= (0x7ffU 
                                                   & ((IData)(vlTOPp->index_i) 
                                                      - (IData)(0x200U))))
                                        ? vlTOPp->dma_research_observer__DOT__event_ram_q
                                       [(0x7ffU & ((IData)(vlTOPp->index_i) 
                                                   - (IData)(0x200U)))]
                                        : 0ULL) : 0ULL);
            } else {
                if (((0x800U <= (IData)(vlTOPp->index_i)) 
                     & (0xc00U > (IData)(vlTOPp->index_i)))) {
                    vlTOPp->rdata_o = (((0xffffU & 
                                         ((IData)(vlTOPp->index_i) 
                                          - (IData)(0x800U))) 
                                        < (0xffffU 
                                           & ((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q) 
                                              << 2U)))
                                        ? vlTOPp->dma_research_observer__DOT__sample_ram_q
                                       [(0x3ffU & (IData)(vlTOPp->index_i))]
                                        : 0ULL);
                } else {
                    if (((0xc00U <= (IData)(vlTOPp->index_i)) 
                         & (0xc40U > (IData)(vlTOPp->index_i)))) {
                        vlTOPp->rdata_o = ((0x1000U 
                                            & (IData)(vlTOPp->index_i))
                                            ? 0ULL : 
                                           ((0x800U 
                                             & (IData)(vlTOPp->index_i))
                                             ? ((0x400U 
                                                 & (IData)(vlTOPp->index_i))
                                                 ? 
                                                ((0x200U 
                                                  & (IData)(vlTOPp->index_i))
                                                  ? 0ULL
                                                  : 
                                                 ((0x100U 
                                                   & (IData)(vlTOPp->index_i))
                                                   ? 0ULL
                                                   : 
                                                  ((0x80U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 0ULL
                                                    : 
                                                   ((0x40U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? 0ULL
                                                     : 
                                                    ((0x20U 
                                                      & (IData)(vlTOPp->index_i))
                                                      ? 
                                                     ((0x10U 
                                                       & (IData)(vlTOPp->index_i))
                                                       ? 0ULL
                                                       : 
                                                      ((8U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 0ULL
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 0xb00000aULL
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_v_comp_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_v_main_q)))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_flags_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_dur_q)))))
                                                        : 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_comp_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_miss_q)))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_req_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_main_q))))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_miss_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_hit_q
                                                                             [1U])))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_miss_q
                                                                             [0U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_hit_q
                                                                             [0U])))))))
                                                      : 
                                                     ((0x10U 
                                                       & (IData)(vlTOPp->index_i))
                                                       ? 
                                                      ((8U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_sel_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_sel_q
                                                                             [0U])))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_comp_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_comp_q
                                                                             [0U]))))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_main_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__win_main_q
                                                                             [0U])))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__win_cycles_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__carry_out_q)))))
                                                        : 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__carry_in_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__sample_ovf_q)))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__eligible_q))))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cost_a_done_all_q))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_done_q
                                                                             [0U])))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_issue_q
                                                                             [0U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_done_q
                                                                             [1U]))))))
                                                       : 
                                                      ((8U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_issue_q
                                                                             [1U]))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q)))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_miss_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_hit_q
                                                                             [1U]))))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_miss_q
                                                                             [0U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_hit_q
                                                                             [0U])))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_sel_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_sel_q
                                                                             [0U])))))
                                                        : 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_comp_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_comp_q
                                                                             [0U])))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_main_q
                                                                             [1U]))
                                                           : (QData)((IData)(
                                                                             vlTOPp->dma_research_observer__DOT__cost_main_q
                                                                             [0U]))))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__cost_cycles_q))
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__cost_err_q)))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          (((QData)((IData)(vlTOPp->dma_research_observer__DOT__window_closed_q)) 
                                                            << 2U) 
                                                           | (QData)((IData)(
                                                                             (((IData)(vlTOPp->dma_research_observer__DOT__window_open_q) 
                                                                               << 1U) 
                                                                              | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)))))
                                                           : 0x49444d4152454331ULL))))))))))
                                                 : 0ULL)
                                             : 0ULL));
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__dst_set_n = vlTOPp->dma_research_observer__DOT__dst_set_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U != (IData)(vlTOPp->index_i))) {
            if ((5U != (IData)(vlTOPp->index_i))) {
                if ((0x30U != (IData)(vlTOPp->index_i))) {
                    if ((0x31U == (IData)(vlTOPp->index_i))) {
                        if ((1U & (~ ((((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                                        | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                                       | (0U != (0xfffU 
                                                 & (IData)(vlTOPp->wdata_i)))) 
                                      | ((IData)(vlTOPp->dma_research_observer__DOT__src_set_q) 
                                         & ((0xfffffffffffffULL 
                                             & (vlTOPp->wdata_i 
                                                >> 0xcU)) 
                                            == (0xfffffffffffffULL 
                                                & (vlTOPp->dma_research_observer__DOT__src_iova_q 
                                                   >> 0xcU)))))))) {
                            vlTOPp->dma_research_observer__DOT__dst_set_n = 1U;
                        }
                    }
                }
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__dst_set_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__src_set_n = vlTOPp->dma_research_observer__DOT__src_set_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U != (IData)(vlTOPp->index_i))) {
            if ((5U != (IData)(vlTOPp->index_i))) {
                if ((0x30U == (IData)(vlTOPp->index_i))) {
                    if ((1U & (~ ((((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                                    | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                                   | (0U != (0xfffU 
                                             & (IData)(vlTOPp->wdata_i)))) 
                                  | ((IData)(vlTOPp->dma_research_observer__DOT__dst_set_q) 
                                     & ((0xfffffffffffffULL 
                                         & (vlTOPp->wdata_i 
                                            >> 0xcU)) 
                                        == (0xfffffffffffffULL 
                                            & (vlTOPp->dma_research_observer__DOT__dst_iova_q 
                                               >> 0xcU)))))))) {
                        vlTOPp->dma_research_observer__DOT__src_set_n = 1U;
                    }
                }
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__src_set_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__snapshot_valid_n 
        = vlTOPp->dma_research_observer__DOT__snapshot_valid_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__snapshot_valid_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__epoch_n = vlTOPp->dma_research_observer__DOT__epoch_q;
    if (((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
           | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
          | (6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
         | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
        vlTOPp->dma_research_observer__DOT__epoch_n 
            = (1ULL + vlTOPp->dma_research_observer__DOT__epoch_q);
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__epoch_n = 0ULL;
    }
    vlTOPp->dma_research_observer__DOT__update_count_n 
        = vlTOPp->dma_research_observer__DOT__update_count_q;
    if ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
          | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
         | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
        if (vlTOPp->update_i) {
            vlTOPp->dma_research_observer__DOT__update_count_n 
                = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__update_count_q);
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__update_count_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__inval_count_n 
        = vlTOPp->dma_research_observer__DOT__inval_count_q;
    if ((6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
        if (vlTOPp->invalidate_i) {
            vlTOPp->dma_research_observer__DOT__inval_count_n 
                = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__inval_count_q);
        }
    } else {
        if ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
              | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
             | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
            if (vlTOPp->invalidate_i) {
                vlTOPp->dma_research_observer__DOT__inval_count_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__inval_count_q);
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__inval_count_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__freeze_ts_n 
        = vlTOPp->dma_research_observer__DOT__freeze_ts_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__freeze_ts_n = 0ULL;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_freeze) {
        vlTOPp->dma_research_observer__DOT__freeze_ts_n 
            = vlTOPp->dma_research_observer__DOT__epoch_q;
    }
    vlTOPp->dma_research_observer__DOT__probestart_n 
        = vlTOPp->dma_research_observer__DOT__probestart_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__probestart_n = 0ULL;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_probe) {
        vlTOPp->dma_research_observer__DOT__probestart_n 
            = vlTOPp->dma_research_observer__DOT__epoch_q;
    }
    vlTOPp->dma_research_observer__DOT__event_count_n 
        = vlTOPp->dma_research_observer__DOT__event_count_q;
    if ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
          | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
         | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
        if (vlTOPp->update_i) {
            if (vlTOPp->dma_research_observer__DOT__slot_oh_valid) {
                if ((0x100U > (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))) {
                    vlTOPp->dma_research_observer__DOT__event_count_n 
                        = (0xffffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__event_count_q)));
                }
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__event_count_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__quiet_timeout_n 
        = vlTOPp->dma_research_observer__DOT__quiet_timeout_q;
    if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
         & (0x40ULL <= (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q)))) {
        if ((1U & (~ (IData)(vlTOPp->quiet_i)))) {
            vlTOPp->dma_research_observer__DOT__quiet_timeout_n 
                = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__quiet_timeout_q);
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__quiet_timeout_n = 0U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__quiet_timeout_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__e2_ovf_n = vlTOPp->dma_research_observer__DOT__e2_ovf_q;
    if ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
          | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
         | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
        if (vlTOPp->update_i) {
            if (vlTOPp->dma_research_observer__DOT__slot_oh_valid) {
                if ((0x100U <= (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))) {
                    vlTOPp->dma_research_observer__DOT__e2_ovf_n = 1U;
                }
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2_ovf_n = 0U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__e2_ovf_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__carry_out_n 
        = vlTOPp->dma_research_observer__DOT__carry_out_q;
    vlTOPp->dma_research_observer__DOT__lane_active_n 
        = ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n)) 
           | (1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)));
    vlTOPp->dma_research_observer__DOT__lane_active_n 
        = ((1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n)) 
           | (2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)));
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__lane_active_n = 0U;
    }
    if (vlTOPp->dma_research_observer__DOT__lanes_en) {
        if ((1U & (((IData)(vlTOPp->issue_i) >> 1U) 
                   & (~ ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_q) 
                         >> 1U))))) {
            vlTOPp->dma_research_observer__DOT__lane_active_n 
                = (2U | (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n));
        }
        if (((((IData)(vlTOPp->done_i) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
              >> 1U) & (vlTOPp->done_id_i[1U] == vlTOPp->dma_research_observer__DOT__lane_id_q
                        [1U]))) {
            vlTOPp->dma_research_observer__DOT__lane_active_n 
                = (1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n));
        }
        if ((1U & ((IData)(vlTOPp->issue_i) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))))) {
            vlTOPp->dma_research_observer__DOT__lane_active_n 
                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n));
        }
        if ((((IData)(vlTOPp->done_i) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
             & (vlTOPp->done_id_i[0U] == vlTOPp->dma_research_observer__DOT__lane_id_q
                [0U]))) {
            vlTOPp->dma_research_observer__DOT__lane_active_n 
                = (2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_n));
        }
    }
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->p4m_close_i) {
            if ((1U & (~ ((~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
                          | (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q))))) {
                vlTOPp->dma_research_observer__DOT__carry_out_n 
                    = (1U & ((IData)(vlTOPp->dma_research_observer__DOT__lane_active_n) 
                             >> 1U));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__carry_out_n = 0U;
                    vlTOPp->dma_research_observer__DOT__lane_active_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__e2a_done_cnt_n 
        = vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2a_done_cnt_n = 0U;
    }
    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
         & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
        if (vlTOPp->dma_research_observer__DOT__a_done_ok) {
            vlTOPp->dma_research_observer__DOT__e2a_done_cnt_n 
                = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q);
        }
    }
    vlTOPp->dma_research_observer__DOT__e2a_last_n 
        = vlTOPp->dma_research_observer__DOT__e2a_last_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2a_last_n = 0ULL;
    }
    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
         & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
        if (vlTOPp->dma_research_observer__DOT__a_done_ok) {
            vlTOPp->dma_research_observer__DOT__e2a_last_n 
                = vlTOPp->dma_research_observer__DOT__epoch_q;
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_a_done_all_n 
        = vlTOPp->dma_research_observer__DOT__cost_a_done_all_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            vlTOPp->dma_research_observer__DOT__cost_a_done_all_n 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__cost_a_done_all_q)));
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_a_done_all_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__eligible_n 
        = vlTOPp->dma_research_observer__DOT__eligible_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            if ((((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                  >> 1U) & (IData)(vlTOPp->dma_research_observer__DOT__win_act))) {
                vlTOPp->dma_research_observer__DOT__eligible_n 
                    = (0xffU & ((IData)(1U) + (IData)(vlTOPp->dma_research_observer__DOT__eligible_q)));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__eligible_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__sample_count_n 
        = vlTOPp->dma_research_observer__DOT__sample_count_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            if ((((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                  >> 1U) & (IData)(vlTOPp->dma_research_observer__DOT__win_act))) {
                if ((0xfU == (0xfU & (IData)(vlTOPp->dma_research_observer__DOT__eligible_q)))) {
                    if ((0x100U > (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q))) {
                        vlTOPp->dma_research_observer__DOT__sample_count_n 
                            = (0xffffU & ((IData)(1U) 
                                          + (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q)));
                    }
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__sample_count_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_done_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_done_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_done_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_done_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            vlTOPp->dma_research_observer__DOT__cost_done_n[1U] 
                = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_done_q
                            [1U]));
        }
        if (((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
               & (IData)(vlTOPp->done_i)) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
             & (vlTOPp->done_id_i[0U] == vlTOPp->dma_research_observer__DOT__lane_id_q
                [0U]))) {
            vlTOPp->dma_research_observer__DOT__cost_done_n[0U] 
                = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_done_q
                            [0U]));
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_done_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_done_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__sample_ovf_n 
        = vlTOPp->dma_research_observer__DOT__sample_ovf_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            if ((((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                  >> 1U) & (IData)(vlTOPp->dma_research_observer__DOT__win_act))) {
                if ((0xfU == (0xfU & (IData)(vlTOPp->dma_research_observer__DOT__eligible_q)))) {
                    if ((0x100U <= (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q))) {
                        vlTOPp->dma_research_observer__DOT__sample_ovf_n = 1U;
                    }
                }
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__sample_ovf_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_n 
        = vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_n = 0U;
    }
    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
         & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
        if (vlTOPp->dma_research_observer__DOT__a_issue_ok) {
            vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_n 
                = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q);
        }
    }
    vlTOPp->dma_research_observer__DOT__e2a_first_seen_n 
        = vlTOPp->dma_research_observer__DOT__e2a_first_seen_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2a_first_seen_n = 0U;
    }
    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
         & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
        if (vlTOPp->dma_research_observer__DOT__a_issue_ok) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__e2a_first_seen_q)))) {
                vlTOPp->dma_research_observer__DOT__e2a_first_seen_n = 1U;
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__e2a_first_n 
        = vlTOPp->dma_research_observer__DOT__e2a_first_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2a_first_n = 0ULL;
    }
    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
         & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
        if (vlTOPp->dma_research_observer__DOT__a_issue_ok) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__e2a_first_seen_q)))) {
                vlTOPp->dma_research_observer__DOT__e2a_first_n 
                    = vlTOPp->dma_research_observer__DOT__epoch_q;
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_a_dur_n 
        = vlTOPp->dma_research_observer__DOT__cur_a_dur_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            vlTOPp->dma_research_observer__DOT__cur_a_dur_n 
                = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_a_dur_q);
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_dur_n = 0U;
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_a_dur_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_issue_n[0U] 
        = vlTOPp->dma_research_observer__DOT__cost_issue_q
        [0U];
    vlTOPp->dma_research_observer__DOT__cost_issue_n[1U] 
        = vlTOPp->dma_research_observer__DOT__cost_issue_q
        [1U];
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cost_issue_n[1U] 
                = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_issue_q
                            [1U]));
        }
        if ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
              & (IData)(vlTOPp->issue_i)) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)))) {
            vlTOPp->dma_research_observer__DOT__cost_issue_n[0U] 
                = (0xffU & ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cost_issue_q
                            [0U]));
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_issue_n[0U] = 0U;
                    vlTOPp->dma_research_observer__DOT__cost_issue_n[1U] = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__lane_in_window_n 
        = ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_n)) 
           | (1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q)));
    vlTOPp->dma_research_observer__DOT__lane_in_window_n 
        = ((1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_n)) 
           | (2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q)));
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__lane_in_window_n = 0U;
    }
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__lane_in_window_n 
                = ((1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_n)) 
                   | ((IData)(vlTOPp->dma_research_observer__DOT__win_act_issue) 
                      << 1U));
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            vlTOPp->dma_research_observer__DOT__lane_in_window_n 
                = (1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_n));
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__lane_in_window_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_a_comp_n 
        = vlTOPp->dma_research_observer__DOT__cur_a_comp_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if (((4U == (IData)(vlTOPp->ctl_state_i)) 
                 & (0xbU == vlTOPp->ctl_did_i))) {
                vlTOPp->dma_research_observer__DOT__cur_a_comp_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_a_comp_q);
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_comp_n = 0U;
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_a_comp_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_a_req_n 
        = vlTOPp->dma_research_observer__DOT__cur_a_req_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if (((IData)(vlTOPp->select_fire_i) & (0xbU 
                                                   == vlTOPp->select_did_i))) {
                vlTOPp->dma_research_observer__DOT__cur_a_req_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_a_req_q);
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_req_n = 0U;
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_a_req_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_a_main_n 
        = vlTOPp->dma_research_observer__DOT__cur_a_main_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if (((IData)(vlTOPp->dma_research_observer__DOT__main_cyc) 
                 & (0xbU == vlTOPp->ctl_did_i))) {
                vlTOPp->dma_research_observer__DOT__cur_a_main_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_a_main_q);
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_main_n = 0U;
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_a_main_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_a_miss_n 
        = vlTOPp->dma_research_observer__DOT__cur_a_miss_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if ((((IData)(vlTOPp->lookup_i) & (0xbU 
                                               == vlTOPp->lookup_did_i)) 
                 & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                vlTOPp->dma_research_observer__DOT__cur_a_miss_n 
                    = ((IData)(1U) + vlTOPp->dma_research_observer__DOT__cur_a_miss_q);
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_miss_n = 0U;
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_a_miss_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cur_a_flags_n 
        = vlTOPp->dma_research_observer__DOT__cur_a_flags_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_flags_n = 0U;
            vlTOPp->dma_research_observer__DOT__cur_a_flags_n 
                = ((0xfffffffeU & vlTOPp->dma_research_observer__DOT__cur_a_flags_n) 
                   | (1U & (~ ((IData)(vlTOPp->dma_research_observer__DOT__window_open_q) 
                               | (IData)(vlTOPp->p4m_open_i)))));
            vlTOPp->dma_research_observer__DOT__cur_a_flags_n 
                = ((0xfffffffdU & vlTOPp->dma_research_observer__DOT__cur_a_flags_n) 
                   | ((IData)(vlTOPp->dma_research_observer__DOT__window_closed_q) 
                      << 1U));
            vlTOPp->dma_research_observer__DOT__cur_a_flags_n 
                = ((0xfffffffbU & vlTOPp->dma_research_observer__DOT__cur_a_flags_n) 
                   | ((IData)(vlTOPp->dma_research_observer__DOT__win_act_issue) 
                      << 2U));
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            vlTOPp->dma_research_observer__DOT__cur_a_flags_n 
                = ((0xfffffff7U & vlTOPp->dma_research_observer__DOT__cur_a_flags_n) 
                   | ((IData)(vlTOPp->p4m_close_i) 
                      << 3U));
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cur_a_flags_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__cost_err_n 
        = vlTOPp->dma_research_observer__DOT__cost_err_q;
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U == (IData)(vlTOPp->index_i))) {
            if (((((0ULL != (0x3fffffffffffffffULL 
                             & (vlTOPp->wdata_i >> 2U))) 
                   | (3U == (3U & (IData)(vlTOPp->wdata_i)))) 
                  | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                 | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))) {
                if ((3U == (3U & (IData)(vlTOPp->wdata_i)))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (0x40U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__abort_cmd) {
        if ((1U & (~ ((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))))) {
            if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                 & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (0x100U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__cost_err_n = 0U;
    }
    if (vlTOPp->dma_research_observer__DOT__lane_proto_err) {
        if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
            vlTOPp->dma_research_observer__DOT__cost_err_n 
                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
        }
    }
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if ((1U & (~ (IData)(vlTOPp->supported_i)))) {
            vlTOPp->dma_research_observer__DOT__cost_err_n 
                = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
        }
        if (vlTOPp->response_error_i) {
            vlTOPp->dma_research_observer__DOT__cost_err_n 
                = (0x80U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
        }
        if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__cost_cycles_q))) {
            vlTOPp->dma_research_observer__DOT__cost_err_n 
                = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
        }
        if (vlTOPp->dma_research_observer__DOT__win_act) {
            if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__win_cycles_q))) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if (vlTOPp->dma_research_observer__DOT__main_cyc) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_main_q
                     [((0xbU == vlTOPp->ctl_did_i) ? 1U
                        : 0U)])) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    if ((0xffU == vlTOPp->dma_research_observer__DOT__win_main_q
                         [((0xbU == vlTOPp->ctl_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if ((4U == (IData)(vlTOPp->ctl_state_i))) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_comp_q
                     [((0xbU == vlTOPp->ctl_did_i) ? 1U
                        : 0U)])) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    if ((0xffU == vlTOPp->dma_research_observer__DOT__win_comp_q
                         [((0xbU == vlTOPp->ctl_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if (vlTOPp->select_fire_i) {
            if (vlTOPp->dma_research_observer__DOT__sel_known) {
                if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_sel_q
                     [((0xbU == vlTOPp->select_did_i)
                        ? 1U : 0U)])) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    if ((0xffU == vlTOPp->dma_research_observer__DOT__win_sel_q
                         [((0xbU == vlTOPp->select_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if (vlTOPp->lookup_i) {
            if (vlTOPp->dma_research_observer__DOT__lu_known) {
                if (vlTOPp->lookup_hit_i) {
                    if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_hit_q
                         [((0xbU == vlTOPp->lookup_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                    if (vlTOPp->dma_research_observer__DOT__win_act) {
                        if ((0xffU == vlTOPp->dma_research_observer__DOT__win_hit_q
                             [((0xbU == vlTOPp->lookup_did_i)
                                ? 1U : 0U)])) {
                            vlTOPp->dma_research_observer__DOT__cost_err_n 
                                = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                        }
                    }
                } else {
                    if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_miss_q
                         [((0xbU == vlTOPp->lookup_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                    if (vlTOPp->dma_research_observer__DOT__win_act) {
                        if ((0xffU == vlTOPp->dma_research_observer__DOT__win_miss_q
                             [((0xbU == vlTOPp->lookup_did_i)
                                ? 1U : 0U)])) {
                            vlTOPp->dma_research_observer__DOT__cost_err_n 
                                = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                        }
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__cost_owner_err_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_a_dur_q)) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
            if (((IData)(vlTOPp->dma_research_observer__DOT__main_cyc) 
                 & (0xbU == vlTOPp->ctl_did_i))) {
                if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_a_main_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
            if (((IData)(vlTOPp->select_fire_i) & (0xbU 
                                                   == vlTOPp->select_did_i))) {
                if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_a_req_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
            if ((((IData)(vlTOPp->lookup_i) & (0xbU 
                                               == vlTOPp->lookup_did_i)) 
                 & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_a_miss_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
            if (((4U == (IData)(vlTOPp->ctl_state_i)) 
                 & (0xbU == vlTOPp->ctl_did_i))) {
                if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_a_comp_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if ((1U & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q))) {
            if (((IData)(vlTOPp->dma_research_observer__DOT__main_cyc) 
                 & (0xaU == vlTOPp->ctl_did_i))) {
                if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_v_main_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
            if (((4U == (IData)(vlTOPp->ctl_state_i)) 
                 & (0xaU == vlTOPp->ctl_did_i))) {
                if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__cur_v_comp_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_issue_ok))) {
            if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_issue_q
                 [1U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
              & (IData)(vlTOPp->issue_i)) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)))) {
            if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_issue_q
                 [0U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_done_q
                 [1U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
            if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__cost_a_done_all_q))) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
            if ((((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                  >> 1U) & (IData)(vlTOPp->dma_research_observer__DOT__win_act))) {
                if ((0xffU == (IData)(vlTOPp->dma_research_observer__DOT__eligible_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if ((0xfU == (0xfU & (IData)(vlTOPp->dma_research_observer__DOT__eligible_q)))) {
                    if ((0x100U <= (IData)(vlTOPp->dma_research_observer__DOT__sample_count_q))) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (8U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            }
        }
        if (((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
               & (IData)(vlTOPp->done_i)) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
             & (vlTOPp->done_id_i[0U] == vlTOPp->dma_research_observer__DOT__lane_id_q
                [0U]))) {
            if ((0xffU == vlTOPp->dma_research_observer__DOT__cost_done_q
                 [0U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if (vlTOPp->p4m_open_i) {
            if (vlTOPp->dma_research_observer__DOT__window_open_q) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (0x10U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if (vlTOPp->p4m_close_i) {
            if ((1U & ((~ (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
                       | (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q)))) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (0x10U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
             & (~ (IData)(vlTOPp->quiet_i)))) {
            vlTOPp->dma_research_observer__DOT__cost_err_n 
                = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
        } else {
            if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                 & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                    | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (0x40U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            } else {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n = 0U;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__astart_n = vlTOPp->dma_research_observer__DOT__astart_q;
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__astart_n = 0ULL;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_a) {
        vlTOPp->dma_research_observer__DOT__astart_n 
            = vlTOPp->dma_research_observer__DOT__epoch_q;
    }
    vlTOPp->dma_research_observer__DOT__a_elapsed_n 
        = vlTOPp->dma_research_observer__DOT__a_elapsed_q;
    if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
        vlTOPp->dma_research_observer__DOT__a_elapsed_n 
            = (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q);
    }
    if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
         & (0x40ULL <= (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q)))) {
        vlTOPp->dma_research_observer__DOT__a_elapsed_n = 0x40ULL;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__a_elapsed_n = 0ULL;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_a) {
        vlTOPp->dma_research_observer__DOT__a_elapsed_n = 0ULL;
    }
    vlTOPp->dma_research_observer__DOT__deadline_n 
        = vlTOPp->dma_research_observer__DOT__deadline_q;
    if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
         & (0x40ULL <= (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q)))) {
        vlTOPp->dma_research_observer__DOT__deadline_n 
            = vlTOPp->dma_research_observer__DOT__epoch_q;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__deadline_n = 0ULL;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_a) {
        vlTOPp->dma_research_observer__DOT__deadline_n 
            = (0x40ULL + vlTOPp->dma_research_observer__DOT__epoch_q);
    }
    vlTOPp->dma_research_observer__DOT__e2_phase_n 
        = vlTOPp->dma_research_observer__DOT__e2_phase_q;
    if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
         & (0x40ULL <= (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q)))) {
        vlTOPp->dma_research_observer__DOT__e2_phase_n = 6U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__e2_phase_n = 1U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_a) {
        vlTOPp->dma_research_observer__DOT__e2_phase_n = 2U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_probe) {
        vlTOPp->dma_research_observer__DOT__e2_phase_n = 3U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_freeze) {
        vlTOPp->dma_research_observer__DOT__e2_phase_n = 4U;
    }
    if (vlTOPp->dma_research_observer__DOT__abort_cmd) {
        if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
             & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
            vlTOPp->dma_research_observer__DOT__e2_phase_n = 5U;
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__e2_phase_n = 0U;
    }
    vlTOPp->dma_research_observer__DOT__e2_err_n = vlTOPp->dma_research_observer__DOT__e2_err_q;
    vlTOPp->dma_research_observer__DOT__target_valid_n 
        = vlTOPp->dma_research_observer__DOT__target_valid_q;
    vlTOPp->dma_research_observer__DOT__target_ctx_valid_n 
        = vlTOPp->dma_research_observer__DOT__target_ctx_valid_q;
    vlTOPp->dma_research_observer__DOT__fp_done_n = vlTOPp->dma_research_observer__DOT__fp_done_q;
    vlTOPp->dma_research_observer__DOT__target_key_n[0U][0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][0U];
    vlTOPp->dma_research_observer__DOT__target_key_n[0U][1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][1U];
    vlTOPp->dma_research_observer__DOT__target_key_n[0U][2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][2U];
    vlTOPp->dma_research_observer__DOT__target_key_n[0U][3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [0U][3U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][0U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][0U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][1U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][1U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][2U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][2U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][3U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [0U][3U];
    vlTOPp->dma_research_observer__DOT__fp_time_n[0U] 
        = vlTOPp->dma_research_observer__DOT__fp_time_q
        [0U];
    vlTOPp->dma_research_observer__DOT__fp_hit_n[0U] 
        = vlTOPp->dma_research_observer__DOT__fp_hit_q
        [0U];
    vlTOPp->dma_research_observer__DOT__fp_live_self_n[0U] 
        = vlTOPp->dma_research_observer__DOT__fp_live_self_q
        [0U];
    vlTOPp->dma_research_observer__DOT__fp_live_oth_n[0U] 
        = vlTOPp->dma_research_observer__DOT__fp_live_oth_q
        [0U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][0U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [0U][0U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][1U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [0U][1U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][2U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [0U][2U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][3U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [0U][3U];
    vlTOPp->dma_research_observer__DOT__target_key_n[1U][0U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][0U];
    vlTOPp->dma_research_observer__DOT__target_key_n[1U][1U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][1U];
    vlTOPp->dma_research_observer__DOT__target_key_n[1U][2U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][2U];
    vlTOPp->dma_research_observer__DOT__target_key_n[1U][3U] 
        = vlTOPp->dma_research_observer__DOT__target_key_q
        [1U][3U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][0U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][0U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][1U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][1U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][2U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][2U];
    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][3U] 
        = vlTOPp->dma_research_observer__DOT__target_ctx_q
        [1U][3U];
    vlTOPp->dma_research_observer__DOT__fp_time_n[1U] 
        = vlTOPp->dma_research_observer__DOT__fp_time_q
        [1U];
    vlTOPp->dma_research_observer__DOT__fp_hit_n[1U] 
        = vlTOPp->dma_research_observer__DOT__fp_hit_q
        [1U];
    vlTOPp->dma_research_observer__DOT__fp_live_self_n[1U] 
        = vlTOPp->dma_research_observer__DOT__fp_live_self_q
        [1U];
    vlTOPp->dma_research_observer__DOT__fp_live_oth_n[1U] 
        = vlTOPp->dma_research_observer__DOT__fp_live_oth_q
        [1U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[1U][0U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [1U][0U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[1U][1U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [1U][1U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[1U][2U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [1U][2U];
    vlTOPp->dma_research_observer__DOT__fp_key_n[1U][3U] 
        = vlTOPp->dma_research_observer__DOT__fp_key_q
        [1U][3U];
    if (((IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase) 
         & (~ (IData)(vlTOPp->supported_i)))) {
        vlTOPp->dma_research_observer__DOT__e2_err_n 
            = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
    }
    if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
         & (0x40ULL <= (1ULL + vlTOPp->dma_research_observer__DOT__a_elapsed_q)))) {
        if ((1U & (~ (IData)(vlTOPp->quiet_i)))) {
            vlTOPp->dma_research_observer__DOT__e2_err_n 
                = (0x10U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
        }
        if (((IData)(vlTOPp->update_i) | (IData)(vlTOPp->invalidate_i))) {
            vlTOPp->dma_research_observer__DOT__e2_err_n 
                = (0x80U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
        }
        if ((1U & (~ (IData)(vlTOPp->supported_i)))) {
            vlTOPp->dma_research_observer__DOT__e2_err_n 
                = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
        }
    }
    if ((6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
        if ((((((((IData)(vlTOPp->lookup_i) | (IData)(vlTOPp->update_i)) 
                 | (IData)(vlTOPp->invalidate_i)) | 
                (0U != (IData)(vlTOPp->issue_i))) | 
               (0U != (IData)(vlTOPp->done_i))) | (IData)(vlTOPp->select_fire_i)) 
             | (IData)(vlTOPp->response_error_i))) {
            vlTOPp->dma_research_observer__DOT__e2_err_n 
                = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
        }
    } else {
        if ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
              | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
             | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
            if (vlTOPp->lookup_i) {
                if ((1U & ((~ (IData)(vlTOPp->lookup_stage1_i)) 
                           | (IData)(vlTOPp->lookup_stage2_i)))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                    if ((0xbU != vlTOPp->lookup_did_i)) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                } else {
                    if ((0xaU != vlTOPp->lookup_did_i)) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__13__b 
                        = vlTOPp->dma_research_observer__DOT__dst_iova_q;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__13__a 
                        = vlTOPp->lookup_iova_i;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__13__Vfuncout 
                        = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__13__a 
                                                  >> 0xcU)) 
                           == (0xfffffffffffffULL & 
                               (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__13__b 
                                >> 0xcU)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__12__b 
                        = vlTOPp->dma_research_observer__DOT__src_iova_q;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__12__a 
                        = vlTOPp->lookup_iova_i;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__12__Vfuncout 
                        = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__12__a 
                                                  >> 0xcU)) 
                           == (0xfffffffffffffULL & 
                               (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__12__b 
                                >> 0xcU)));
                    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                         & (~ ((IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__12__Vfuncout) 
                               | (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__13__Vfuncout))))) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__15__b 
                        = vlTOPp->dma_research_observer__DOT__dst_iova_q;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__15__a 
                        = vlTOPp->lookup_iova_i;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__15__Vfuncout 
                        = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__15__a 
                                                  >> 0xcU)) 
                           == (0xfffffffffffffULL & 
                               (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__15__b 
                                >> 0xcU)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__14__b 
                        = vlTOPp->dma_research_observer__DOT__src_iova_q;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__14__a 
                        = vlTOPp->lookup_iova_i;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__14__Vfuncout 
                        = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__14__a 
                                                  >> 0xcU)) 
                           == (0xfffffffffffffULL & 
                               (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__14__b 
                                >> 0xcU)));
                    if (((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                         & (~ ((IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__14__Vfuncout) 
                               | (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__15__Vfuncout))))) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                }
            }
            if (vlTOPp->update_i) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                    if ((0xbU != vlTOPp->update_did_i)) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                } else {
                    if ((0xaU != vlTOPp->update_did_i)) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                }
            }
            if (vlTOPp->invalidate_i) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if ((((IData)(vlTOPp->issue_i) >> 1U) & 
                 (2U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (((IData)(vlTOPp->issue_i) & (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if ((((IData)(vlTOPp->done_i) >> 1U) & 
                 (2U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (((IData)(vlTOPp->done_i) & (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (vlTOPp->select_fire_i) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                    if ((0xbU != vlTOPp->select_did_i)) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                } else {
                    if ((0xaU != vlTOPp->select_did_i)) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                }
            }
            if (vlTOPp->response_error_i) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x2000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        }
    }
    if ((((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
            & (IData)(vlTOPp->lookup_i)) & (IData)(vlTOPp->lookup_stage1_i)) 
          & (~ (IData)(vlTOPp->lookup_stage2_i))) & 
         (0xaU == vlTOPp->lookup_did_i))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__16__b 
            = vlTOPp->dma_research_observer__DOT__src_iova_q;
        vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__16__a 
            = vlTOPp->lookup_iova_i;
        vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__16__Vfuncout 
            = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__16__a 
                                      >> 0xcU)) == 
               (0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__16__b 
                                      >> 0xcU)));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__16__Vfuncout) {
            if ((1U & (IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q))) {
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[0U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [0U][0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[1U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [0U][1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[2U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [0U][2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[3U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [0U][3U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[0U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[2U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[3U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[0U] 
                    = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[0U]) 
                       | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                                 >> 0xcU))));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[0U] 
                    = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[0U]) 
                       | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                         << 0x1dU)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U] 
                    = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U]) 
                       | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                         >> 3U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U] 
                    = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U]) 
                       | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                         << 0x11U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U] 
                    = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U]) 
                       | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                         << 0x12U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U] 
                    = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U]);
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[2U] 
                    = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[2U]) 
                       | (IData)(vlTOPp->lookup_gscid_i));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__k[3U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__18__Vfuncout[3U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__Vfuncout 
                    = (((((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[1U] 
                                        << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[0U] 
                                                  >> 0x1dU))) 
                          == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[1U] 
                                           << 3U) | 
                                          (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[0U] 
                                           >> 0x1dU)))) 
                         & ((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[2U]) 
                            == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[2U]))) 
                        & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[1U] 
                                  >> 0x11U)) == (1U 
                                                 & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[1U] 
                                                    >> 0x11U)))) 
                       & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__a[1U] 
                                 >> 0x12U)) == (1U 
                                                & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__b[1U] 
                                                   >> 0x12U))));
                if ((1U & (~ (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__17__Vfuncout)))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
            } else {
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[0U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[2U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[3U] = 0U;
                vlTOPp->dma_research_observer__DOT__target_ctx_valid_n 
                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_n));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[0U] 
                    = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[0U]) 
                       | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                                 >> 0xcU))));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[0U] 
                    = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[0U]) 
                       | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                         << 0x1dU)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U] 
                    = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U]) 
                       | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                         >> 3U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U] 
                    = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U]) 
                       | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                         << 0x11U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U] 
                    = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U]) 
                       | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                         << 0x12U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U] 
                    = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U]);
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[2U] 
                    = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[2U]) 
                       | (IData)(vlTOPp->lookup_gscid_i));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__k[3U];
                vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[0U];
                vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[1U];
                vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[2U];
                vlTOPp->dma_research_observer__DOT__target_ctx_n[0U][3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__19__Vfuncout[3U];
            }
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__target_valid_q)))) {
                if (vlTOPp->dma_research_observer__DOT__cap_found) {
                    vlTOPp->dma_research_observer__DOT__target_key_n[0U][0U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[0U];
                    vlTOPp->dma_research_observer__DOT__target_key_n[0U][1U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[1U];
                    vlTOPp->dma_research_observer__DOT__target_key_n[0U][2U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[2U];
                    vlTOPp->dma_research_observer__DOT__target_key_n[0U][3U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[3U];
                    vlTOPp->dma_research_observer__DOT__target_valid_n 
                        = (1U | (IData)(vlTOPp->dma_research_observer__DOT__target_valid_n));
                } else {
                    if (vlTOPp->dma_research_observer__DOT__cap_bad_geom) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                }
            }
        } else {
            vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__20__b 
                = vlTOPp->dma_research_observer__DOT__dst_iova_q;
            vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__20__a 
                = vlTOPp->lookup_iova_i;
            vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__20__Vfuncout 
                = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__20__a 
                                          >> 0xcU)) 
                   == (0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__20__b 
                                             >> 0xcU)));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__20__Vfuncout) {
                if ((2U & (IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q))) {
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[0U] 
                        = vlTOPp->dma_research_observer__DOT__target_ctx_q
                        [1U][0U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[1U] 
                        = vlTOPp->dma_research_observer__DOT__target_ctx_q
                        [1U][1U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[2U] 
                        = vlTOPp->dma_research_observer__DOT__target_ctx_q
                        [1U][2U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[3U] 
                        = vlTOPp->dma_research_observer__DOT__target_ctx_q
                        [1U][3U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[0U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[2U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[3U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[0U] 
                        = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[0U]) 
                           | (0x1fffffffU & (IData)(
                                                    (vlTOPp->lookup_iova_i 
                                                     >> 0xcU))));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[0U] 
                        = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[0U]) 
                           | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                             << 0x1dU)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U] 
                        = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U]) 
                           | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                             >> 3U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U] 
                        = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U]) 
                           | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                             << 0x11U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U] 
                        = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U]) 
                           | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                             << 0x12U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U] 
                        = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U]);
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[2U] 
                        = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[2U]) 
                           | (IData)(vlTOPp->lookup_gscid_i));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[0U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[0U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[1U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[1U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[2U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[2U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[3U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__k[3U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[0U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[0U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[1U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[1U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[2U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[2U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[3U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__22__Vfuncout[3U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__Vfuncout 
                        = (((((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[1U] 
                                            << 3U) 
                                           | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[0U] 
                                              >> 0x1dU))) 
                              == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[1U] 
                                               << 3U) 
                                              | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[0U] 
                                                 >> 0x1dU)))) 
                             & ((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[2U]) 
                                == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[2U]))) 
                            & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[1U] 
                                      >> 0x11U)) == 
                               (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[1U] 
                                      >> 0x11U)))) 
                           & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__a[1U] 
                                     >> 0x12U)) == 
                              (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__b[1U] 
                                     >> 0x12U))));
                    if ((1U & (~ (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__21__Vfuncout)))) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                } else {
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[0U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[2U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[3U] = 0U;
                    vlTOPp->dma_research_observer__DOT__target_ctx_valid_n 
                        = (2U | (IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_n));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[0U] 
                        = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[0U]) 
                           | (0x1fffffffU & (IData)(
                                                    (vlTOPp->lookup_iova_i 
                                                     >> 0xcU))));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[0U] 
                        = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[0U]) 
                           | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                             << 0x1dU)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U] 
                        = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U]) 
                           | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                             >> 3U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U] 
                        = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U]) 
                           | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                             << 0x11U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U] 
                        = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U]) 
                           | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                             << 0x12U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U] 
                        = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U]);
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[2U] 
                        = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[2U]) 
                           | (IData)(vlTOPp->lookup_gscid_i));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[0U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[0U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[1U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[1U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[2U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[2U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[3U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__k[3U];
                    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][0U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[0U];
                    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][1U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[1U];
                    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][2U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[2U];
                    vlTOPp->dma_research_observer__DOT__target_ctx_n[1U][3U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__23__Vfuncout[3U];
                }
                if ((1U & (~ ((IData)(vlTOPp->dma_research_observer__DOT__target_valid_q) 
                              >> 1U)))) {
                    if (vlTOPp->dma_research_observer__DOT__cap_found) {
                        vlTOPp->dma_research_observer__DOT__target_key_n[1U][0U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[0U];
                        vlTOPp->dma_research_observer__DOT__target_key_n[1U][1U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[1U];
                        vlTOPp->dma_research_observer__DOT__target_key_n[1U][2U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[2U];
                        vlTOPp->dma_research_observer__DOT__target_key_n[1U][3U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[3U];
                        vlTOPp->dma_research_observer__DOT__target_valid_n 
                            = (2U | (IData)(vlTOPp->dma_research_observer__DOT__target_valid_n));
                    } else {
                        if (vlTOPp->dma_research_observer__DOT__cap_bad_geom) {
                            vlTOPp->dma_research_observer__DOT__e2_err_n 
                                = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                        }
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        }
    }
    if ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
          | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
         | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)))) {
        if (vlTOPp->update_i) {
            if (vlTOPp->invalidate_i) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x100U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__slot_oh_valid)))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x100U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if ((1U < (IData)(vlTOPp->dma_research_observer__DOT__changed_count))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__changed_count)) 
                 & ((IData)(vlTOPp->dma_research_observer__DOT__changed_mask) 
                    != (IData)(vlTOPp->replace_mask_i)))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x100U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (vlTOPp->dma_research_observer__DOT__slot_oh_valid) {
                if ((0x100U <= (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (8U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
            }
        }
    }
    if (((IData)(vlTOPp->cfg_valid_i) & (IData)(vlTOPp->cfg_write_i))) {
        if ((4U == (IData)(vlTOPp->index_i))) {
            if (((((0ULL != (0x3fffffffffffffffULL 
                             & (vlTOPp->wdata_i >> 2U))) 
                   | (3U == (3U & (IData)(vlTOPp->wdata_i)))) 
                  | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                 | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        } else {
            if ((5U == (IData)(vlTOPp->index_i))) {
                if ((((0ULL != (0xfffffffffffffffULL 
                                & (vlTOPp->wdata_i 
                                   >> 4U))) | (0U == 
                                               (0xfU 
                                                & (IData)(vlTOPp->wdata_i)))) 
                     | (6U < (0xfU & (IData)(vlTOPp->wdata_i))))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                } else {
                    if ((1U == (0xfU & (IData)(vlTOPp->wdata_i)))) {
                        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__acc_warm)))) {
                            vlTOPp->dma_research_observer__DOT__e2_err_n 
                                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                            if (((IData)(vlTOPp->update_i) 
                                 | (IData)(vlTOPp->invalidate_i))) {
                                vlTOPp->dma_research_observer__DOT__e2_err_n 
                                    = (0x80U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                            }
                        }
                    } else {
                        if ((2U == (0xfU & (IData)(vlTOPp->wdata_i)))) {
                            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__acc_a)))) {
                                vlTOPp->dma_research_observer__DOT__e2_err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                if ((((3U == (IData)(vlTOPp->dma_research_observer__DOT__target_valid_q)) 
                                      & (IData)(vlTOPp->dma_research_observer__DOT__tgt_present0)) 
                                     & (IData)(vlTOPp->dma_research_observer__DOT__tgt_present1))) {
                                    if (((IData)(vlTOPp->update_i) 
                                         | (IData)(vlTOPp->invalidate_i))) {
                                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                                            = (0x80U 
                                               | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                    } else {
                                        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__ctx_ok_both)))) {
                                            vlTOPp->dma_research_observer__DOT__e2_err_n 
                                                = (0x1000U 
                                                   | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                        }
                                    }
                                } else {
                                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                                        = (0x40U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                }
                            }
                        } else {
                            if ((3U == (0xfU & (IData)(vlTOPp->wdata_i)))) {
                                if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__acc_probe)))) {
                                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                                        = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                }
                            } else {
                                if ((4U == (0xfU & (IData)(vlTOPp->wdata_i)))) {
                                    if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__acc_freeze)))) {
                                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                                            = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                        if ((3U != (IData)(vlTOPp->dma_research_observer__DOT__fp_done_q))) {
                                            vlTOPp->dma_research_observer__DOT__e2_err_n 
                                                = (1U 
                                                   | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                        } else {
                                            if (((IData)(vlTOPp->update_i) 
                                                 | (IData)(vlTOPp->invalidate_i))) {
                                                vlTOPp->dma_research_observer__DOT__e2_err_n 
                                                    = 
                                                    (0x80U 
                                                     | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                            }
                                        }
                                    }
                                } else {
                                    if ((5U != (0xfU 
                                                & (IData)(vlTOPp->wdata_i)))) {
                                        if ((6U == 
                                             (0xfU 
                                              & (IData)(vlTOPp->wdata_i)))) {
                                            if ((1U 
                                                 & (~ (IData)(vlTOPp->dma_research_observer__DOT__acc_clear)))) {
                                                vlTOPp->dma_research_observer__DOT__e2_err_n 
                                                    = 
                                                    (1U 
                                                     | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if ((1U & (~ (IData)(vlTOPp->supported_i)))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
            } else {
                if ((0x30U == (IData)(vlTOPp->index_i))) {
                    if (((((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                           | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                          | (0U != (0xfffU & (IData)(vlTOPp->wdata_i)))) 
                         | ((IData)(vlTOPp->dma_research_observer__DOT__dst_set_q) 
                            & ((0xfffffffffffffULL 
                                & (vlTOPp->wdata_i 
                                   >> 0xcU)) == (0xfffffffffffffULL 
                                                 & (vlTOPp->dma_research_observer__DOT__dst_iova_q 
                                                    >> 0xcU)))))) {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (0x800U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                } else {
                    if ((0x31U == (IData)(vlTOPp->index_i))) {
                        if (((((0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
                               | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                              | (0U != (0xfffU & (IData)(vlTOPp->wdata_i)))) 
                             | ((IData)(vlTOPp->dma_research_observer__DOT__src_set_q) 
                                & ((0xfffffffffffffULL 
                                    & (vlTOPp->wdata_i 
                                       >> 0xcU)) == 
                                   (0xfffffffffffffULL 
                                    & (vlTOPp->dma_research_observer__DOT__src_iova_q 
                                       >> 0xcU)))))) {
                            vlTOPp->dma_research_observer__DOT__e2_err_n 
                                = (0x800U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                        }
                    } else {
                        vlTOPp->dma_research_observer__DOT__e2_err_n 
                            = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                    }
                }
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__acc_warm) {
        vlTOPp->dma_research_observer__DOT__target_valid_n = 0U;
        vlTOPp->dma_research_observer__DOT__target_ctx_valid_n = 0U;
        vlTOPp->dma_research_observer__DOT__fp_done_n = 0U;
    }
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__e2_err_n = 0U;
    }
    if ((((((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
            & (IData)(vlTOPp->lookup_i)) & (IData)(vlTOPp->lookup_stage1_i)) 
          & (~ (IData)(vlTOPp->lookup_stage2_i))) & 
         (0xaU == vlTOPp->lookup_did_i))) {
        vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__24__b 
            = vlTOPp->dma_research_observer__DOT__src_iova_q;
        vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__24__a 
            = vlTOPp->lookup_iova_i;
        vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__24__Vfuncout 
            = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__24__a 
                                      >> 0xcU)) == 
               (0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__24__b 
                                      >> 0xcU)));
        if (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__24__Vfuncout) {
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[0U] 
                = vlTOPp->dma_research_observer__DOT__target_ctx_q
                [0U][0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[1U] 
                = vlTOPp->dma_research_observer__DOT__target_ctx_q
                [0U][1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[2U] 
                = vlTOPp->dma_research_observer__DOT__target_ctx_q
                [0U][2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[3U] 
                = vlTOPp->dma_research_observer__DOT__target_ctx_q
                [0U][3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[0U] = 0U;
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U] = 0U;
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[2U] = 0U;
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[3U] = 0U;
            if ((1U & ((IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q) 
                       & (~ (IData)(vlTOPp->dma_research_observer__DOT__fp_done_q))))) {
                vlTOPp->dma_research_observer__DOT__fp_time_n[0U] 
                    = vlTOPp->dma_research_observer__DOT__epoch_q;
                vlTOPp->dma_research_observer__DOT__fp_hit_n[0U] 
                    = vlTOPp->lookup_hit_i;
                vlTOPp->dma_research_observer__DOT__fp_live_self_n[0U] 
                    = vlTOPp->dma_research_observer__DOT__tgt_live0;
                vlTOPp->dma_research_observer__DOT__fp_live_oth_n[0U] 
                    = vlTOPp->dma_research_observer__DOT__tgt_live1;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[0U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[2U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[3U] = 0U;
                vlTOPp->dma_research_observer__DOT__fp_done_n 
                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__fp_done_n));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[0U] 
                    = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[0U]) 
                       | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                                 >> 0xcU))));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[0U] 
                    = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[0U]) 
                       | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                         << 0x1dU)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U] 
                    = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U]) 
                       | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                         >> 3U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U] 
                    = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U]) 
                       | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                         << 0x11U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U] 
                    = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U]) 
                       | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                         << 0x12U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U] 
                    = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U]);
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[2U] 
                    = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[2U]) 
                       | (IData)(vlTOPp->lookup_gscid_i));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__k[3U];
                if (vlTOPp->dma_research_observer__DOT__cap_found) {
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][0U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[0U];
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][1U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[1U];
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][2U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[2U];
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][3U] 
                        = vlTOPp->dma_research_observer__DOT__cap_key[3U];
                } else {
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][0U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[0U];
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][1U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[1U];
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][2U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[2U];
                    vlTOPp->dma_research_observer__DOT__fp_key_n[0U][3U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__27__Vfuncout[3U];
                }
            }
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[0U] 
                = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[0U]) 
                   | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                             >> 0xcU))));
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[0U] 
                = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[0U]) 
                   | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                     << 0x1dU)));
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U] 
                = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U]) 
                   | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                     >> 3U)));
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U] 
                = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U]) 
                   | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                     << 0x11U)));
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U] 
                = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U]) 
                   | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                     << 0x12U)));
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U] 
                = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U]);
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[2U] 
                = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[2U]) 
                   | (IData)(vlTOPp->lookup_gscid_i));
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[0U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[1U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[2U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[3U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__k[3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[0U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[0U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[1U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[1U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[2U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[2U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[3U] 
                = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__26__Vfuncout[3U];
            vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__Vfuncout 
                = (((((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[1U] 
                                    << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[0U] 
                                              >> 0x1dU))) 
                      == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[1U] 
                                       << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[0U] 
                                                 >> 0x1dU)))) 
                     & ((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[2U]) 
                        == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[2U]))) 
                    & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[1U] 
                              >> 0x11U)) == (1U & (
                                                   vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[1U] 
                                                   >> 0x11U)))) 
                   & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__a[1U] 
                             >> 0x12U)) == (1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__b[1U] 
                                                  >> 0x12U))));
            if ((1U & ((~ (IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q)) 
                       | (~ (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__25__Vfuncout))))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (((IData)(vlTOPp->lookup_hit_i) != (IData)(vlTOPp->dma_research_observer__DOT__cap_found))) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
            if (vlTOPp->dma_research_observer__DOT__cap_bad_geom) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        } else {
            vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__28__b 
                = vlTOPp->dma_research_observer__DOT__dst_iova_q;
            vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__28__a 
                = vlTOPp->lookup_iova_i;
            vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__28__Vfuncout 
                = ((0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__28__a 
                                          >> 0xcU)) 
                   == (0xfffffffffffffULL & (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__28__b 
                                             >> 0xcU)));
            if (vlTOPp->__Vfunc_dma_research_observer__DOT__page_match__28__Vfuncout) {
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[0U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [1U][0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[1U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [1U][1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[2U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [1U][2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[3U] 
                    = vlTOPp->dma_research_observer__DOT__target_ctx_q
                    [1U][3U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[0U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[2U] = 0U;
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[3U] = 0U;
                if ((1U & (((IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q) 
                            >> 1U) & (~ ((IData)(vlTOPp->dma_research_observer__DOT__fp_done_q) 
                                         >> 1U))))) {
                    vlTOPp->dma_research_observer__DOT__fp_time_n[1U] 
                        = vlTOPp->dma_research_observer__DOT__epoch_q;
                    vlTOPp->dma_research_observer__DOT__fp_hit_n[1U] 
                        = vlTOPp->lookup_hit_i;
                    vlTOPp->dma_research_observer__DOT__fp_live_self_n[1U] 
                        = vlTOPp->dma_research_observer__DOT__tgt_live1;
                    vlTOPp->dma_research_observer__DOT__fp_live_oth_n[1U] 
                        = vlTOPp->dma_research_observer__DOT__tgt_live0;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[0U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[2U] = 0U;
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[3U] = 0U;
                    vlTOPp->dma_research_observer__DOT__fp_done_n 
                        = (2U | (IData)(vlTOPp->dma_research_observer__DOT__fp_done_n));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[0U] 
                        = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[0U]) 
                           | (0x1fffffffU & (IData)(
                                                    (vlTOPp->lookup_iova_i 
                                                     >> 0xcU))));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[0U] 
                        = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[0U]) 
                           | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                             << 0x1dU)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U] 
                        = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U]) 
                           | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                             >> 3U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U] 
                        = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U]) 
                           | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                             << 0x11U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U] 
                        = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U]) 
                           | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                             << 0x12U)));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U] 
                        = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U]);
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[2U] 
                        = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[2U]) 
                           | (IData)(vlTOPp->lookup_gscid_i));
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[0U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[0U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[1U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[1U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[2U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[2U];
                    vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[3U] 
                        = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__k[3U];
                    if (vlTOPp->dma_research_observer__DOT__cap_found) {
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][0U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[0U];
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][1U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[1U];
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][2U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[2U];
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][3U] 
                            = vlTOPp->dma_research_observer__DOT__cap_key[3U];
                    } else {
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][0U] 
                            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[0U];
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][1U] 
                            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[1U];
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][2U] 
                            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[2U];
                        vlTOPp->dma_research_observer__DOT__fp_key_n[1U][3U] 
                            = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__31__Vfuncout[3U];
                    }
                }
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[0U] 
                    = ((0xe0000000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[0U]) 
                       | (0x1fffffffU & (IData)((vlTOPp->lookup_iova_i 
                                                 >> 0xcU))));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[0U] 
                    = ((0x1fffffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[0U]) 
                       | (0xe0000000U & (vlTOPp->lookup_pscid_i 
                                         << 0x1dU)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U] 
                    = ((0xfffe0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U]) 
                       | (0x1fffffffU & (vlTOPp->lookup_pscid_i 
                                         >> 3U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U] 
                    = ((0xfffdffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U]) 
                       | (0xfffe0000U & ((IData)(vlTOPp->lookup_stage1_i) 
                                         << 0x11U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U] 
                    = ((0xfffbffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U]) 
                       | (0xfffc0000U & ((IData)(vlTOPp->lookup_stage2_i) 
                                         << 0x12U)));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U] 
                    = (0x80000U | vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U]);
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[2U] 
                    = ((0xffff0000U & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[2U]) 
                       | (IData)(vlTOPp->lookup_gscid_i));
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__k[3U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[0U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[0U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[1U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[1U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[2U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[2U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[3U] 
                    = vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_key_128__30__Vfuncout[3U];
                vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__Vfuncout 
                    = (((((0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[1U] 
                                        << 3U) | (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[0U] 
                                                  >> 0x1dU))) 
                          == (0xfffffU & ((vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[1U] 
                                           << 3U) | 
                                          (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[0U] 
                                           >> 0x1dU)))) 
                         & ((0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[2U]) 
                            == (0xffffU & vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[2U]))) 
                        & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[1U] 
                                  >> 0x11U)) == (1U 
                                                 & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[1U] 
                                                    >> 0x11U)))) 
                       & ((1U & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__a[1U] 
                                 >> 0x12U)) == (1U 
                                                & (vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__b[1U] 
                                                   >> 0x12U))));
                if ((1U & ((~ ((IData)(vlTOPp->dma_research_observer__DOT__target_ctx_valid_q) 
                               >> 1U)) | (~ (IData)(vlTOPp->__Vfunc_dma_research_observer__DOT__ctx_match__29__Vfuncout))))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (0x1000U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
                if (((IData)(vlTOPp->lookup_hit_i) 
                     != (IData)(vlTOPp->dma_research_observer__DOT__cap_found))) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__cap_bad_geom) {
                    vlTOPp->dma_research_observer__DOT__e2_err_n 
                        = (0x200U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                }
            } else {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        }
    }
    if (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
         & (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase))) {
        if (vlTOPp->dma_research_observer__DOT__a_issue_ok) {
            if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__e2a_issue_cnt_q)) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x400U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        }
        if (vlTOPp->dma_research_observer__DOT__a_done_ok) {
            if ((0xffffffffU == vlTOPp->dma_research_observer__DOT__e2a_done_cnt_q)) {
                vlTOPp->dma_research_observer__DOT__e2_err_n 
                    = (0x400U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
            }
        }
    }
    if (vlTOPp->dma_research_observer__DOT__lane_proto_err) {
        if ((2U != (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
            vlTOPp->dma_research_observer__DOT__e2_err_n 
                = (0x400U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
        }
    }
}
