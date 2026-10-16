// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_research_observer.h for the primary calling header

#include "Vdma_research_observer.h"
#include "Vdma_research_observer__Syms.h"

//==========

VL_CTOR_IMP(Vdma_research_observer) {
    Vdma_research_observer__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_research_observer__Syms(this, name());
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_research_observer::__Vconfigure(Vdma_research_observer__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_research_observer::~Vdma_research_observer() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_research_observer::_settle__TOP__2(Vdma_research_observer__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_research_observer::_settle__TOP__2\n"); );
    Vdma_research_observer* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*383:0*/ __Vtemp375[12];
    WData/*255:0*/ __Vtemp376[8];
    WData/*95:0*/ __Vtemp390[3];
    WData/*95:0*/ __Vtemp391[3];
    WData/*95:0*/ __Vtemp392[3];
    WData/*95:0*/ __Vtemp395[3];
    WData/*95:0*/ __Vtemp396[3];
    WData/*95:0*/ __Vtemp397[3];
    WData/*95:0*/ __Vtemp400[3];
    WData/*95:0*/ __Vtemp401[3];
    WData/*95:0*/ __Vtemp402[3];
    WData/*95:0*/ __Vtemp405[3];
    WData/*95:0*/ __Vtemp406[3];
    WData/*95:0*/ __Vtemp407[3];
    WData/*95:0*/ __Vtemp410[3];
    WData/*95:0*/ __Vtemp411[3];
    WData/*95:0*/ __Vtemp412[3];
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
    vlTOPp->reverse_order_o = ((IData)(vlTOPp->dma_research_observer__DOT__reverse_order_q) 
                               & (((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                                   & (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                                  | ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                                     & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))));
    vlTOPp->dma_research_observer__DOT__cost_cycles_n 
        = vlTOPp->dma_research_observer__DOT__cost_cycles_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        vlTOPp->dma_research_observer__DOT__cost_cycles_n 
            = (1ULL + vlTOPp->dma_research_observer__DOT__cost_cycles_q);
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_cycles_n = 0ULL;
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
    vlTOPp->dma_research_observer__DOT__win_act_issue 
        = ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & ((IData)(vlTOPp->dma_research_observer__DOT__window_open_q) 
                | (IData)(vlTOPp->p4m_open_i))) & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q))) 
           & (~ (IData)(vlTOPp->p4m_close_i)));
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
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen 
        = ((4U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
           | (5U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)));
    vlTOPp->dma_research_observer__DOT__win_act = (
                                                   ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__window_open_q)) 
                                                   & (~ (IData)(vlTOPp->dma_research_observer__DOT__window_closed_q)));
    vlTOPp->dma_research_observer__DOT__e2_active_phase 
        = ((((1U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)) 
             | (2U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
            | (6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
           | (3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q)));
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
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_sel_q
                       [((0xbU == vlTOPp->select_did_i)
                          ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__cost_sel_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_sel_n[1U] = 0ULL;
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
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_comp_q
                       [((0xbU == vlTOPp->ctl_did_i)
                          ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__cost_comp_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_comp_n[1U] = 0ULL;
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
                        = (1ULL + vlTOPp->dma_research_observer__DOT__cost_miss_q
                           [((0xbU == vlTOPp->lookup_did_i)
                              ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__cost_miss_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_miss_n[1U] = 0ULL;
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
                        = (1ULL + vlTOPp->dma_research_observer__DOT__cost_hit_q
                           [((0xbU == vlTOPp->lookup_did_i)
                              ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__cost_hit_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_hit_n[1U] = 0ULL;
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
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_main_q
                       [((0xbU == vlTOPp->ctl_did_i)
                          ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__cost_main_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_main_n[1U] = 0ULL;
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
    vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
        = vlTOPp->dma_research_observer__DOT__cost_owner_err_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->dma_research_observer__DOT__main_cyc) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__main_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_owner_err_q);
            }
        }
        if ((4U == (IData)(vlTOPp->ctl_state_i))) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__main_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_owner_err_q);
            }
        }
        if (vlTOPp->select_fire_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__sel_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_owner_err_q);
            }
        }
        if (vlTOPp->lookup_i) {
            if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__lu_known)))) {
                vlTOPp->dma_research_observer__DOT__cost_owner_err_n 
                    = (1ULL + vlTOPp->dma_research_observer__DOT__cost_owner_err_q);
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
                    vlTOPp->dma_research_observer__DOT__cost_owner_err_n = 0ULL;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__abort_cmd = 
        (((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
          & (0ULL == (0xfffffffffffffffULL & (vlTOPp->wdata_i 
                                              >> 4U)))) 
         & (5U == (0xfU & (IData)(vlTOPp->wdata_i))));
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
    vlTOPp->dma_research_observer__DOT__acc_probe = 
        (((((((IData)(vlTOPp->dma_research_observer__DOT__cmd_wr) 
              & (0ULL == (0xfffffffffffffffULL & (vlTOPp->wdata_i 
                                                  >> 4U)))) 
             & (1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
            & (3U == (0xfU & (IData)(vlTOPp->wdata_i)))) 
           & (6U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
          & (IData)(vlTOPp->quiet_i)) & (IData)(vlTOPp->supported_i));
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
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open 
        = ((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_edge) 
           | (2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q)));
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now 
        = ((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_edge)
            ? vlTOPp->dma_research_observer__DOT__epoch_q
            : vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_q);
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
    vlTOPp->dma_research_observer__DOT__win_cycles_n 
        = vlTOPp->dma_research_observer__DOT__win_cycles_q;
    if (vlTOPp->dma_research_observer__DOT__cost_active_q) {
        if (vlTOPp->dma_research_observer__DOT__win_act) {
            vlTOPp->dma_research_observer__DOT__win_cycles_n 
                = (1ULL + vlTOPp->dma_research_observer__DOT__win_cycles_q);
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__win_cycles_n = 0ULL;
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
                        = (1ULL + vlTOPp->dma_research_observer__DOT__win_main_q
                           [((0xbU == vlTOPp->ctl_did_i)
                              ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__win_main_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__win_main_n[1U] = 0ULL;
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
                        = (1ULL + vlTOPp->dma_research_observer__DOT__win_comp_q
                           [((0xbU == vlTOPp->ctl_did_i)
                              ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__win_comp_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__win_comp_n[1U] = 0ULL;
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
                        = (1ULL + vlTOPp->dma_research_observer__DOT__win_sel_q
                           [((0xbU == vlTOPp->select_did_i)
                              ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__win_sel_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__win_sel_n[1U] = 0ULL;
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
                            = (1ULL + vlTOPp->dma_research_observer__DOT__win_miss_q
                               [((0xbU == vlTOPp->lookup_did_i)
                                  ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__win_miss_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__win_miss_n[1U] = 0ULL;
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
                            = (1ULL + vlTOPp->dma_research_observer__DOT__win_hit_q
                               [((0xbU == vlTOPp->lookup_did_i)
                                  ? 1U : 0U)]);
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
                    vlTOPp->dma_research_observer__DOT__win_hit_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__win_hit_n[1U] = 0ULL;
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__lanes_en = 
        ((1U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))
          ? (IData)(vlTOPp->dma_research_observer__DOT__e2_active_phase)
          : ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
             & (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)));
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
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((1U & (IData)(vlTOPp->issue_i))) {
                    if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_n 
                            = vlTOPp->dma_research_observer__DOT__epoch_q;
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                    if (((0xaU == vlTOPp->ctl_did_i) 
                         & ((1U == (IData)(vlTOPp->ctl_state_i)) 
                            | (2U == (IData)(vlTOPp->ctl_state_i))))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_n 
                            = (1ULL + vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q);
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                    if (((0xaU == vlTOPp->ctl_did_i) 
                         & (4U == (IData)(vlTOPp->ctl_state_i)))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_n 
                            = (1ULL + vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q);
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
    vlTOPp->dma_research_observer__DOT__agg_clear = 
        ((IData)(vlTOPp->dma_research_observer__DOT__acc_clear) 
         | (((((IData)(vlTOPp->p4m_arm_i) & (2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) 
              & (IData)(vlTOPp->quiet_i)) & (~ (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q))) 
            & (0U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))));
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_n 
                            = (1ULL + vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q);
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__lookup_v_i) {
                        if (vlTOPp->lookup_hit_i) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_n 
                                = (1ULL + vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__lookup_v_i) {
                        if ((1U & (~ (IData)(vlTOPp->lookup_hit_i)))) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_n 
                                = (1ULL + vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if (vlTOPp->dma_research_observer__DOT__acc_probe) {
                if ((0U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n = 1U;
                }
            }
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((1U & (IData)(vlTOPp->issue_i))) {
                    if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n = 2U;
                    }
                }
                if ((1U & (IData)(vlTOPp->done_i))) {
                    if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n = 3U;
                    } else {
                        if ((3U != (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                            if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_n = 3U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i) {
                        if ((1U & (~ (IData)((vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_n 
                                = vlTOPp->dma_research_observer__DOT__epoch_q;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((1U & (IData)(vlTOPp->done_i))) {
                    if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_n 
                            = vlTOPp->dma_research_observer__DOT__epoch_q;
                    } else {
                        if ((3U != (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                            if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_n 
                                    = vlTOPp->dma_research_observer__DOT__epoch_q;
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((1U & (IData)(vlTOPp->issue_i))) {
                    if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n 
                            = (1ULL | vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n);
                    }
                }
                if ((1U & (IData)(vlTOPp->done_i))) {
                    if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n 
                            = (4ULL | vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n);
                    } else {
                        if ((3U != (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                            if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n 
                                    = (4ULL | vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n);
                            }
                        }
                    }
                }
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i) {
                        if ((1U & (~ (IData)((vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n 
                                = (2ULL | vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_n);
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if ((1U & (IData)(vlTOPp->done_i))) {
                    if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_n 
                            = ((vlTOPp->dma_research_observer__DOT__epoch_q 
                                < vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now)
                                ? 0ULL : (vlTOPp->dma_research_observer__DOT__epoch_q 
                                          - vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now));
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_n = 0ULL;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i) {
                        if ((1U & (~ (IData)((vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_n 
                                = ((vlTOPp->dma_research_observer__DOT__epoch_q 
                                    < vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now)
                                    ? 0ULL : (vlTOPp->dma_research_observer__DOT__epoch_q 
                                              - vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now));
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_q;
    if (vlTOPp->dma_research_observer__DOT__acc_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n = 0U;
    } else {
        if ((1U & (~ (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__phase_frozen)))) {
            if (vlTOPp->dma_research_observer__DOT__acc_probe) {
                if ((0U != (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                        = (0x80U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                }
            }
            if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) {
                if (((~ (IData)(vlTOPp->supported_i)) 
                     & ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q)) 
                        | (2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                        = (0x20U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                }
                if (((IData)(vlTOPp->response_error_i) 
                     & (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                        = (0x10U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                }
                if ((1U & (IData)(vlTOPp->issue_i))) {
                    if ((1U != (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                        if (((2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q)) 
                             | (3U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q)))) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                = (2U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                        }
                    }
                }
                if ((1U & (IData)(vlTOPp->done_i))) {
                    if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                        if ((vlTOPp->dma_research_observer__DOT__epoch_q 
                             < vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now)) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                = (0x40U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                        }
                    } else {
                        if ((3U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                = (8U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                        } else {
                            if ((1U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                            }
                        }
                    }
                }
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__state_q))) {
                    if (((0xaU == vlTOPp->ctl_did_i) 
                         & ((1U == (IData)(vlTOPp->ctl_state_i)) 
                            | (2U == (IData)(vlTOPp->ctl_state_i))))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q)) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                        }
                    }
                    if (((0xaU == vlTOPp->ctl_did_i) 
                         & (4U == (IData)(vlTOPp->ctl_state_i)))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q)) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                        }
                    }
                }
                if (vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__interval_open) {
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__select_v_i) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q)) {
                            vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                        }
                        if ((1U & (~ (IData)((vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q 
                                              >> 1U))))) {
                            if ((vlTOPp->dma_research_observer__DOT__epoch_q 
                                 < vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_now)) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                    = (0x40U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                            }
                        }
                    }
                    if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_e2_timing__lookup_v_i) {
                        if (vlTOPp->lookup_hit_i) {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q)) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                            }
                        } else {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q)) {
                                vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_n));
                            }
                        }
                    }
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
                                    ? (((0U == (0x1fU 
                                                & (((IData)(vlTOPp->index_i) 
                                                    - (IData)(0x100U)) 
                                                   << 6U)))
                                         ? 0ULL : ((QData)((IData)(
                                                                   vlTOPp->dma_research_observer__DOT__snap_warm_q[
                                                                   ((IData)(2U) 
                                                                    + 
                                                                    (0x3eU 
                                                                     & (((IData)(vlTOPp->index_i) 
                                                                         - (IData)(0x100U)) 
                                                                        << 1U)))])) 
                                                   << 
                                                   ((IData)(0x40U) 
                                                    - 
                                                    (0x1fU 
                                                     & (((IData)(vlTOPp->index_i) 
                                                         - (IData)(0x100U)) 
                                                        << 6U))))) 
                                       | (((QData)((IData)(
                                                           vlTOPp->dma_research_observer__DOT__snap_warm_q[
                                                           ((IData)(1U) 
                                                            + 
                                                            (0x3eU 
                                                             & (((IData)(vlTOPp->index_i) 
                                                                 - (IData)(0x100U)) 
                                                                << 1U)))])) 
                                           << ((0U 
                                                == 
                                                (0x1fU 
                                                 & (((IData)(vlTOPp->index_i) 
                                                     - (IData)(0x100U)) 
                                                    << 6U)))
                                                ? 0x20U
                                                : ((IData)(0x20U) 
                                                   - 
                                                   (0x1fU 
                                                    & (((IData)(vlTOPp->index_i) 
                                                        - (IData)(0x100U)) 
                                                       << 6U))))) 
                                          | ((QData)((IData)(
                                                             vlTOPp->dma_research_observer__DOT__snap_warm_q[
                                                             (0x3eU 
                                                              & (((IData)(vlTOPp->index_i) 
                                                                  - (IData)(0x100U)) 
                                                                 << 1U))])) 
                                             >> (0x1fU 
                                                 & (((IData)(vlTOPp->index_i) 
                                                     - (IData)(0x100U)) 
                                                    << 6U)))))
                                    : 0ULL) : ((0x140U 
                                                > (IData)(vlTOPp->index_i))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q))
                                                    ? 
                                                   (((0U 
                                                      == 
                                                      (0x1fU 
                                                       & (((IData)(vlTOPp->index_i) 
                                                           - (IData)(0x120U)) 
                                                          << 6U)))
                                                      ? 0ULL
                                                      : 
                                                     ((QData)((IData)(
                                                                      vlTOPp->dma_research_observer__DOT__snap_deadline_q[
                                                                      ((IData)(2U) 
                                                                       + 
                                                                       (0x3eU 
                                                                        & (((IData)(vlTOPp->index_i) 
                                                                            - (IData)(0x120U)) 
                                                                           << 1U)))])) 
                                                      << 
                                                      ((IData)(0x40U) 
                                                       - 
                                                       (0x1fU 
                                                        & (((IData)(vlTOPp->index_i) 
                                                            - (IData)(0x120U)) 
                                                           << 6U))))) 
                                                    | (((QData)((IData)(
                                                                        vlTOPp->dma_research_observer__DOT__snap_deadline_q[
                                                                        ((IData)(1U) 
                                                                         + 
                                                                         (0x3eU 
                                                                          & (((IData)(vlTOPp->index_i) 
                                                                              - (IData)(0x120U)) 
                                                                             << 1U)))])) 
                                                        << 
                                                        ((0U 
                                                          == 
                                                          (0x1fU 
                                                           & (((IData)(vlTOPp->index_i) 
                                                               - (IData)(0x120U)) 
                                                              << 6U)))
                                                          ? 0x20U
                                                          : 
                                                         ((IData)(0x20U) 
                                                          - 
                                                          (0x1fU 
                                                           & (((IData)(vlTOPp->index_i) 
                                                               - (IData)(0x120U)) 
                                                              << 6U))))) 
                                                       | ((QData)((IData)(
                                                                          vlTOPp->dma_research_observer__DOT__snap_deadline_q[
                                                                          (0x3eU 
                                                                           & (((IData)(vlTOPp->index_i) 
                                                                               - (IData)(0x120U)) 
                                                                              << 1U))])) 
                                                          >> 
                                                          (0x1fU 
                                                           & (((IData)(vlTOPp->index_i) 
                                                               - (IData)(0x120U)) 
                                                              << 6U)))))
                                                    : 0ULL)
                                                : (
                                                   (4U 
                                                    & (IData)(vlTOPp->dma_research_observer__DOT__snapshot_valid_q))
                                                    ? 
                                                   (((0U 
                                                      == 
                                                      (0x1fU 
                                                       & (((IData)(vlTOPp->index_i) 
                                                           - (IData)(0x140U)) 
                                                          << 6U)))
                                                      ? 0ULL
                                                      : 
                                                     ((QData)((IData)(
                                                                      vlTOPp->dma_research_observer__DOT__snap_freeze_q[
                                                                      ((IData)(2U) 
                                                                       + 
                                                                       (0x3eU 
                                                                        & (((IData)(vlTOPp->index_i) 
                                                                            - (IData)(0x140U)) 
                                                                           << 1U)))])) 
                                                      << 
                                                      ((IData)(0x40U) 
                                                       - 
                                                       (0x1fU 
                                                        & (((IData)(vlTOPp->index_i) 
                                                            - (IData)(0x140U)) 
                                                           << 6U))))) 
                                                    | (((QData)((IData)(
                                                                        vlTOPp->dma_research_observer__DOT__snap_freeze_q[
                                                                        ((IData)(1U) 
                                                                         + 
                                                                         (0x3eU 
                                                                          & (((IData)(vlTOPp->index_i) 
                                                                              - (IData)(0x140U)) 
                                                                             << 1U)))])) 
                                                        << 
                                                        ((0U 
                                                          == 
                                                          (0x1fU 
                                                           & (((IData)(vlTOPp->index_i) 
                                                               - (IData)(0x140U)) 
                                                              << 6U)))
                                                          ? 0x20U
                                                          : 
                                                         ((IData)(0x20U) 
                                                          - 
                                                          (0x1fU 
                                                           & (((IData)(vlTOPp->index_i) 
                                                               - (IData)(0x140U)) 
                                                              << 6U))))) 
                                                       | ((QData)((IData)(
                                                                          vlTOPp->dma_research_observer__DOT__snap_freeze_q[
                                                                          (0x3eU 
                                                                           & (((IData)(vlTOPp->index_i) 
                                                                               - (IData)(0x140U)) 
                                                                              << 1U))])) 
                                                          >> 
                                                          (0x1fU 
                                                           & (((IData)(vlTOPp->index_i) 
                                                               - (IData)(0x140U)) 
                                                              << 6U)))))
                                                    : 0ULL)));
        } else {
            if (((0x200U <= (IData)(vlTOPp->index_i)) 
                 & (0x800U > (IData)(vlTOPp->index_i)))) {
                __Vtemp375[0U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][0U];
                __Vtemp375[1U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][1U];
                __Vtemp375[2U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][2U];
                __Vtemp375[3U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][3U];
                __Vtemp375[4U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][4U];
                __Vtemp375[5U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][5U];
                __Vtemp375[6U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][6U];
                __Vtemp375[7U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][7U];
                __Vtemp375[8U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][8U];
                __Vtemp375[9U] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][9U];
                __Vtemp375[0xaU] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][0xaU];
                __Vtemp375[0xbU] = vlTOPp->dma_research_observer__DOT__event_ram_q
                    [(0xffU & VL_DIV_III(32, ((IData)(vlTOPp->index_i) 
                                              - (IData)(0x200U)), (IData)(6U)))][0xbU];
                vlTOPp->rdata_o = (((0xffffU & ((IData)(vlTOPp->index_i) 
                                                - (IData)(0x200U))) 
                                    < (0xffffU & ((IData)(6U) 
                                                  * (IData)(vlTOPp->dma_research_observer__DOT__event_count_q))))
                                    ? ((0x17fU >= (0x1ffU 
                                                   & (VL_MODDIV_III(32, 
                                                                    ((IData)(vlTOPp->index_i) 
                                                                     - (IData)(0x200U)), (IData)(6U)) 
                                                      << 6U)))
                                        ? (((0U == 
                                             (0x1fU 
                                              & (VL_MODDIV_III(32, 
                                                               ((IData)(vlTOPp->index_i) 
                                                                - (IData)(0x200U)), (IData)(6U)) 
                                                 << 6U)))
                                             ? 0ULL
                                             : ((QData)((IData)(
                                                                __Vtemp375[
                                                                ((IData)(2U) 
                                                                 + 
                                                                 (0xeU 
                                                                  & (VL_MODDIV_III(32, 
                                                                                ((IData)(vlTOPp->index_i) 
                                                                                - (IData)(0x200U)), (IData)(6U)) 
                                                                     << 1U)))])) 
                                                << 
                                                ((IData)(0x40U) 
                                                 - 
                                                 (0x1fU 
                                                  & (VL_MODDIV_III(32, 
                                                                   ((IData)(vlTOPp->index_i) 
                                                                    - (IData)(0x200U)), (IData)(6U)) 
                                                     << 6U))))) 
                                           | (((QData)((IData)(
                                                               __Vtemp375[
                                                               ((IData)(1U) 
                                                                + 
                                                                (0xeU 
                                                                 & (VL_MODDIV_III(32, 
                                                                                ((IData)(vlTOPp->index_i) 
                                                                                - (IData)(0x200U)), (IData)(6U)) 
                                                                    << 1U)))])) 
                                               << (
                                                   (0U 
                                                    == 
                                                    (0x1fU 
                                                     & (VL_MODDIV_III(32, 
                                                                      ((IData)(vlTOPp->index_i) 
                                                                       - (IData)(0x200U)), (IData)(6U)) 
                                                        << 6U)))
                                                    ? 0x20U
                                                    : 
                                                   ((IData)(0x20U) 
                                                    - 
                                                    (0x1fU 
                                                     & (VL_MODDIV_III(32, 
                                                                      ((IData)(vlTOPp->index_i) 
                                                                       - (IData)(0x200U)), (IData)(6U)) 
                                                        << 6U))))) 
                                              | ((QData)((IData)(
                                                                 __Vtemp375[
                                                                 (0xeU 
                                                                  & (VL_MODDIV_III(32, 
                                                                                ((IData)(vlTOPp->index_i) 
                                                                                - (IData)(0x200U)), (IData)(6U)) 
                                                                     << 1U))])) 
                                                 >> 
                                                 (0x1fU 
                                                  & (VL_MODDIV_III(32, 
                                                                   ((IData)(vlTOPp->index_i) 
                                                                    - (IData)(0x200U)), (IData)(6U)) 
                                                     << 6U)))))
                                        : 0ULL) : 0ULL);
            } else {
                if (((0x800U <= (IData)(vlTOPp->index_i)) 
                     & (0xc00U > (IData)(vlTOPp->index_i)))) {
                    __Vtemp376[0U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][0U];
                    __Vtemp376[1U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][1U];
                    __Vtemp376[2U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][2U];
                    __Vtemp376[3U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][3U];
                    __Vtemp376[4U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][4U];
                    __Vtemp376[5U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][5U];
                    __Vtemp376[6U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][6U];
                    __Vtemp376[7U] = vlTOPp->dma_research_observer__DOT__sample_ram_q
                        [(0xffU & (((IData)(vlTOPp->index_i) 
                                    - (IData)(0x800U)) 
                                   >> 2U))][7U];
                    vlTOPp->rdata_o = (((0xffffU & 
                                         ((IData)(vlTOPp->index_i) 
                                          - (IData)(0x800U))) 
                                        < (0xffffU 
                                           & ((IData)(vlTOPp->dma_research_observer__DOT__sample_count_q) 
                                              << 2U)))
                                        ? (((QData)((IData)(
                                                            __Vtemp376[
                                                            ((IData)(1U) 
                                                             + 
                                                             (6U 
                                                              & (((IData)(vlTOPp->index_i) 
                                                                  - (IData)(0x800U)) 
                                                                 << 1U)))])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             __Vtemp376[
                                                             (6U 
                                                              & (((IData)(vlTOPp->index_i) 
                                                                  - (IData)(0x800U)) 
                                                                 << 1U))])))
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
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__win_miss_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__win_hit_q
                                                          [1U])
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__win_miss_q
                                                          [0U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__win_hit_q
                                                          [0U])))))
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
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__win_sel_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__win_sel_q
                                                          [0U])
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__win_comp_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__win_comp_q
                                                          [0U]))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__win_main_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__win_main_q
                                                          [0U])
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__win_cycles_q
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
                                                           : vlTOPp->dma_research_observer__DOT__eligible_q))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__cost_a_done_all_q
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_done_q
                                                          [0U])
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__cost_issue_q
                                                          [0U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_done_q
                                                          [1U]))))
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
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__cost_issue_q
                                                          [1U]
                                                           : vlTOPp->dma_research_observer__DOT__cost_owner_err_q)
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__cost_miss_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_hit_q
                                                          [1U]))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__cost_miss_q
                                                          [0U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_hit_q
                                                          [0U])
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__cost_sel_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_sel_q
                                                          [0U])))
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
                                                          vlTOPp->dma_research_observer__DOT__cost_comp_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_comp_q
                                                          [0U])
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 
                                                          vlTOPp->dma_research_observer__DOT__cost_main_q
                                                          [1U]
                                                           : 
                                                          vlTOPp->dma_research_observer__DOT__cost_main_q
                                                          [0U]))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__cost_cycles_q
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
                    } else {
                        if (((0xd00U <= (IData)(vlTOPp->index_i)) 
                             & (0xd70U > (IData)(vlTOPp->index_i)))) {
                            vlTOPp->rdata_o = ((0xd10U 
                                                > (IData)(vlTOPp->index_i))
                                                ? (
                                                   (0xd00U 
                                                    == (IData)(vlTOPp->index_i))
                                                    ? 0x49444d4141474731ULL
                                                    : 
                                                   ((0xd01U 
                                                     == (IData)(vlTOPp->index_i))
                                                     ? 1ULL
                                                     : 
                                                    ((0xd02U 
                                                      == (IData)(vlTOPp->index_i))
                                                      ? 6ULL
                                                      : 
                                                     ((0xd03U 
                                                       == (IData)(vlTOPp->index_i))
                                                       ? 0x10ULL
                                                       : 
                                                      ((0xd04U 
                                                        == (IData)(vlTOPp->index_i))
                                                        ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_q))
                                                        : 
                                                       ((0xd05U 
                                                         == (IData)(vlTOPp->index_i))
                                                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q
                                                         : 0ULL))))))
                                                : (
                                                   ((1U 
                                                     <= 
                                                     (0xfU 
                                                      & ((IData)(vlTOPp->index_i) 
                                                         >> 4U))) 
                                                    & (6U 
                                                       >= 
                                                       (0xfU 
                                                        & ((IData)(vlTOPp->index_i) 
                                                           >> 4U))))
                                                    ? 
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
                                                        : 
                                                       (((QData)((IData)(
                                                                         ((5U 
                                                                           >= 
                                                                           (7U 
                                                                            & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                               - (IData)(1U))))
                                                                           ? 
                                                                          vlTOPp->dma_research_observer__DOT__agg_max_dur_o
                                                                          [
                                                                          (7U 
                                                                           & (((IData)(vlTOPp->index_i) 
                                                                               >> 4U) 
                                                                              - (IData)(1U)))]
                                                                           : 0U))) 
                                                         << 0x20U) 
                                                        | (QData)((IData)(
                                                                          ((5U 
                                                                            >= 
                                                                            (7U 
                                                                             & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                                - (IData)(1U))))
                                                                            ? 
                                                                           vlTOPp->dma_research_observer__DOT__agg_min_dur_o
                                                                           [
                                                                           (7U 
                                                                            & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                               - (IData)(1U)))]
                                                                            : 0U)))))))
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
                                                                         ((5U 
                                                                           >= 
                                                                           (7U 
                                                                            & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                               - (IData)(1U))))
                                                                           ? 
                                                                          vlTOPp->dma_research_observer__DOT__agg_max_comp_o
                                                                          [
                                                                          (7U 
                                                                           & (((IData)(vlTOPp->index_i) 
                                                                               >> 4U) 
                                                                              - (IData)(1U)))]
                                                                           : 0U))) 
                                                         << 0x20U) 
                                                        | (QData)((IData)(
                                                                          ((5U 
                                                                            >= 
                                                                            (7U 
                                                                             & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                                - (IData)(1U))))
                                                                            ? 
                                                                           vlTOPp->dma_research_observer__DOT__agg_min_comp_o
                                                                           [
                                                                           (7U 
                                                                            & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                               - (IData)(1U)))]
                                                                            : 0U))))
                                                        : 
                                                       (((QData)((IData)(
                                                                         ((5U 
                                                                           >= 
                                                                           (7U 
                                                                            & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                               - (IData)(1U))))
                                                                           ? 
                                                                          vlTOPp->dma_research_observer__DOT__agg_max_main_o
                                                                          [
                                                                          (7U 
                                                                           & (((IData)(vlTOPp->index_i) 
                                                                               >> 4U) 
                                                                              - (IData)(1U)))]
                                                                           : 0U))) 
                                                         << 0x20U) 
                                                        | (QData)((IData)(
                                                                          ((5U 
                                                                            >= 
                                                                            (7U 
                                                                             & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                                - (IData)(1U))))
                                                                            ? 
                                                                           vlTOPp->dma_research_observer__DOT__agg_min_main_o
                                                                           [
                                                                           (7U 
                                                                            & (((IData)(vlTOPp->index_i) 
                                                                                >> 4U) 
                                                                               - (IData)(1U)))]
                                                                            : 0U)))))
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((5U 
                                                         >= 
                                                         (7U 
                                                          & (((IData)(vlTOPp->index_i) 
                                                              >> 4U) 
                                                             - (IData)(1U))))
                                                         ? 
                                                        vlTOPp->dma_research_observer__DOT__agg_sum_miss_o
                                                        [
                                                        (7U 
                                                         & (((IData)(vlTOPp->index_i) 
                                                             >> 4U) 
                                                            - (IData)(1U)))]
                                                         : 0ULL)
                                                        : 
                                                       ((5U 
                                                         >= 
                                                         (7U 
                                                          & (((IData)(vlTOPp->index_i) 
                                                              >> 4U) 
                                                             - (IData)(1U))))
                                                         ? 
                                                        vlTOPp->dma_research_observer__DOT__agg_sum_req_o
                                                        [
                                                        (7U 
                                                         & (((IData)(vlTOPp->index_i) 
                                                             >> 4U) 
                                                            - (IData)(1U)))]
                                                         : 0ULL)))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->index_i))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((5U 
                                                         >= 
                                                         (7U 
                                                          & (((IData)(vlTOPp->index_i) 
                                                              >> 4U) 
                                                             - (IData)(1U))))
                                                         ? 
                                                        vlTOPp->dma_research_observer__DOT__agg_sum_dur_o
                                                        [
                                                        (7U 
                                                         & (((IData)(vlTOPp->index_i) 
                                                             >> 4U) 
                                                            - (IData)(1U)))]
                                                         : 0ULL)
                                                        : 
                                                       ((5U 
                                                         >= 
                                                         (7U 
                                                          & (((IData)(vlTOPp->index_i) 
                                                              >> 4U) 
                                                             - (IData)(1U))))
                                                         ? 
                                                        vlTOPp->dma_research_observer__DOT__agg_sum_comp_o
                                                        [
                                                        (7U 
                                                         & (((IData)(vlTOPp->index_i) 
                                                             >> 4U) 
                                                            - (IData)(1U)))]
                                                         : 0ULL))
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->index_i))
                                                        ? 
                                                       ((5U 
                                                         >= 
                                                         (7U 
                                                          & (((IData)(vlTOPp->index_i) 
                                                              >> 4U) 
                                                             - (IData)(1U))))
                                                         ? 
                                                        vlTOPp->dma_research_observer__DOT__agg_sum_main_o
                                                        [
                                                        (7U 
                                                         & (((IData)(vlTOPp->index_i) 
                                                             >> 4U) 
                                                            - (IData)(1U)))]
                                                         : 0ULL)
                                                        : 
                                                       ((5U 
                                                         >= 
                                                         (7U 
                                                          & (((IData)(vlTOPp->index_i) 
                                                              >> 4U) 
                                                             - (IData)(1U))))
                                                         ? 
                                                        vlTOPp->dma_research_observer__DOT__agg_count_o
                                                        [
                                                        (7U 
                                                         & (((IData)(vlTOPp->index_i) 
                                                             >> 4U) 
                                                            - (IData)(1U)))]
                                                         : 0ULL)))))
                                                    : 0ULL));
                        } else {
                            if (((0xf00U <= (IData)(vlTOPp->index_i)) 
                                 & (0xf10U >= (IData)(vlTOPp->index_i)))) {
                                vlTOPp->rdata_o = (
                                                   (0xf00U 
                                                    == (IData)(vlTOPp->index_i))
                                                    ? 0x49444d4132434150ULL
                                                    : 
                                                   ((0xf01U 
                                                     == (IData)(vlTOPp->index_i))
                                                     ? 1ULL
                                                     : 
                                                    ((0xf02U 
                                                      == (IData)(vlTOPp->index_i))
                                                      ? 1ULL
                                                      : 
                                                     ((0xf03U 
                                                       == (IData)(vlTOPp->index_i))
                                                       ? 7ULL
                                                       : 
                                                      ((0xf10U 
                                                        == (IData)(vlTOPp->index_i))
                                                        ? (QData)((IData)(vlTOPp->dma_research_observer__DOT__reverse_order_q))
                                                        : 0ULL)))));
                            } else {
                                if (((0xd80U <= (IData)(vlTOPp->index_i)) 
                                     & (0xd90U > (IData)(vlTOPp->index_i)))) {
                                    vlTOPp->rdata_o 
                                        = ((0x1000U 
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
                                                   ? 
                                                  ((0x80U 
                                                    & (IData)(vlTOPp->index_i))
                                                    ? 
                                                   ((0x40U 
                                                     & (IData)(vlTOPp->index_i))
                                                     ? 0ULL
                                                     : 
                                                    ((0x20U 
                                                      & (IData)(vlTOPp->index_i))
                                                      ? 0ULL
                                                      : 
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
                                                           ? vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_miss_q
                                                           : vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_hit_q))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_sel_q
                                                           : vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_comp_q)
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__v_main_q
                                                           : vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_issue_q)))
                                                        : 
                                                       ((4U 
                                                         & (IData)(vlTOPp->index_i))
                                                         ? 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_issue_q
                                                           : vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__done_epoch_q)
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__first_select_epoch_q
                                                           : vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__issue_epoch_q))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlTOPp->index_i))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__flags_q
                                                           : (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_e2_timing__DOT__err_q)))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlTOPp->index_i))
                                                           ? 1ULL
                                                           : 0x49444d4145325431ULL)))))))
                                                    : 0ULL)
                                                   : 0ULL))
                                                 : 0ULL)
                                             : 0ULL));
                                }
                            }
                        }
                    }
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
                = (1ULL + vlTOPp->dma_research_observer__DOT__cost_a_done_all_q);
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_a_done_all_n = 0ULL;
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
                    = (1ULL + vlTOPp->dma_research_observer__DOT__eligible_q);
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
                    vlTOPp->dma_research_observer__DOT__eligible_n = 0ULL;
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
                = (1ULL + vlTOPp->dma_research_observer__DOT__cost_done_q
                   [1U]);
        }
        if (((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
               & (IData)(vlTOPp->done_i)) & (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)) 
             & (vlTOPp->done_id_i[0U] == vlTOPp->dma_research_observer__DOT__lane_id_q
                [0U]))) {
            vlTOPp->dma_research_observer__DOT__cost_done_n[0U] 
                = (1ULL + vlTOPp->dma_research_observer__DOT__cost_done_q
                   [0U]);
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_done_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_done_n[1U] = 0ULL;
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
    vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_cost_groups__commit_i 
        = (((IData)(vlTOPp->dma_research_observer__DOT__win_act) 
            & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok)) 
           & ((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
              >> 1U));
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
                = (1ULL + vlTOPp->dma_research_observer__DOT__cost_issue_q
                   [1U]);
        }
        if ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
              & (IData)(vlTOPp->issue_i)) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)))) {
            vlTOPp->dma_research_observer__DOT__cost_issue_n[0U] 
                = (1ULL + vlTOPp->dma_research_observer__DOT__cost_issue_q
                   [0U]);
        }
    }
    if (vlTOPp->p4m_arm_i) {
        if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                      & (~ (IData)(vlTOPp->quiet_i)))))) {
            if ((1U & (~ ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q)) 
                          & ((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
                             | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))))))) {
                if ((2U == (IData)(vlTOPp->dma_research_observer__DOT__mode_q))) {
                    vlTOPp->dma_research_observer__DOT__cost_issue_n[0U] = 0ULL;
                    vlTOPp->dma_research_observer__DOT__cost_issue_n[1U] = 0ULL;
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
        if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_cycles_q)) {
            vlTOPp->dma_research_observer__DOT__cost_err_n 
                = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
        }
        if (vlTOPp->dma_research_observer__DOT__win_act) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__win_cycles_q)) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if (vlTOPp->dma_research_observer__DOT__main_cyc) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_main_q
                     [((0xbU == vlTOPp->ctl_did_i) ? 1U
                        : 0U)])) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__win_main_q
                         [((0xbU == vlTOPp->ctl_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_owner_err_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if ((4U == (IData)(vlTOPp->ctl_state_i))) {
            if (vlTOPp->dma_research_observer__DOT__main_known) {
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_comp_q
                     [((0xbU == vlTOPp->ctl_did_i) ? 1U
                        : 0U)])) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__win_comp_q
                         [((0xbU == vlTOPp->ctl_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_owner_err_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if (vlTOPp->select_fire_i) {
            if (vlTOPp->dma_research_observer__DOT__sel_known) {
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_sel_q
                     [((0xbU == vlTOPp->select_did_i)
                        ? 1U : 0U)])) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
                if (vlTOPp->dma_research_observer__DOT__win_act) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__win_sel_q
                         [((0xbU == vlTOPp->select_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_owner_err_q)) {
                    vlTOPp->dma_research_observer__DOT__cost_err_n 
                        = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                }
            }
        }
        if (vlTOPp->lookup_i) {
            if (vlTOPp->dma_research_observer__DOT__lu_known) {
                if (vlTOPp->lookup_hit_i) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_hit_q
                         [((0xbU == vlTOPp->lookup_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                    if (vlTOPp->dma_research_observer__DOT__win_act) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_observer__DOT__win_hit_q
                             [((0xbU == vlTOPp->lookup_did_i)
                                ? 1U : 0U)])) {
                            vlTOPp->dma_research_observer__DOT__cost_err_n 
                                = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                        }
                    }
                } else {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_miss_q
                         [((0xbU == vlTOPp->lookup_did_i)
                            ? 1U : 0U)])) {
                        vlTOPp->dma_research_observer__DOT__cost_err_n 
                            = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
                    }
                    if (vlTOPp->dma_research_observer__DOT__win_act) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_research_observer__DOT__win_miss_q
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
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_owner_err_q)) {
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
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_issue_q
                 [1U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if ((((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
              & (IData)(vlTOPp->issue_i)) & (~ (IData)(vlTOPp->dma_research_observer__DOT__lane_active_q)))) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_issue_q
                 [0U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
        }
        if (((IData)(vlTOPp->dma_research_observer__DOT__cost_active_q) 
             & (IData)(vlTOPp->dma_research_observer__DOT__a_done_ok))) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_done_q
                 [1U])) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_a_done_all_q)) {
                vlTOPp->dma_research_observer__DOT__cost_err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__cost_err_n));
            }
            if ((((IData)(vlTOPp->dma_research_observer__DOT__lane_in_window_q) 
                  >> 1U) & (IData)(vlTOPp->dma_research_observer__DOT__win_act))) {
                if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__eligible_q)) {
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
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__cost_done_q
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
                        if ((0xf10U == (IData)(vlTOPp->index_i))) {
                            if ((1U & ((((~ (IData)(vlTOPp->quiet_i)) 
                                         | (0U != (IData)(vlTOPp->dma_research_observer__DOT__e2_phase_q))) 
                                        | (IData)(vlTOPp->dma_research_observer__DOT__cost_active_q)) 
                                       | (0ULL != (0x7fffffffffffffffULL 
                                                   & (vlTOPp->wdata_i 
                                                      >> 1U)))))) {
                                vlTOPp->dma_research_observer__DOT__e2_err_n 
                                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                            }
                        } else {
                            vlTOPp->dma_research_observer__DOT__e2_err_n 
                                = (1U | (IData)(vlTOPp->dma_research_observer__DOT__e2_err_n));
                        }
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
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q;
    if (vlTOPp->dma_research_observer__DOT__agg_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_n = 0ULL;
    } else {
        if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_cost_groups__commit_i) {
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_n 
                = (1ULL + vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q);
        }
    }
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [0U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[0U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [1U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[1U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [2U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[2U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [3U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[3U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [4U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[4U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound1;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound2;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound3;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound4;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound5;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound6;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound7;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound8;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound9;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound10;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound11;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
        [5U];
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[5U] 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound12;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_q;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_q;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp = 5U;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U] = 0U;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U] = 0U;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U] = 0U;
    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__main_plus_comp = 0ULL;
    if (((4U == vlTOPp->dma_research_observer__DOT__cur_a_req_n) 
         & (4U >= vlTOPp->dma_research_observer__DOT__cur_a_miss_n))) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp 
            = (7U & vlTOPp->dma_research_observer__DOT__cur_a_miss_n);
    }
    if (vlTOPp->dma_research_observer__DOT__agg_clear) {
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[0U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[0U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_n = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[1U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[1U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[2U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[2U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[3U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[3U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[4U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[4U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[5U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[5U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[5U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[5U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[5U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[5U] = 0ULL;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[5U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[5U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[5U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[5U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[5U] = 0U;
        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[5U] = 0U;
    } else {
        if (vlTOPp->dma_research_observer__DOT____Vcellinp__u_dma_research_cost_groups__commit_i) {
            if ((vlTOPp->dma_research_observer__DOT__cur_a_miss_n 
                 > vlTOPp->dma_research_observer__DOT__cur_a_req_n)) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (4U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__main_plus_comp 
                = (0x1ffffffffULL & ((QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_main_n)) 
                                     + (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_comp_n))));
            if ((vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__main_plus_comp 
                 > (QData)((IData)(vlTOPp->dma_research_observer__DOT__cur_a_dur_n)))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (8U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            if ((0xffffffffffffffffULL == vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__total_q)) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            if ((0xffffffffffffffffULL == ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                                            ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
                                           [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                                            : 0ULL))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (1U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound25 
                = (1ULL + ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                            ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_q
                           [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                            : 0ULL));
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__count_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound25;
            }
            VL_EXTEND_WQ(65,64, __Vtemp390, ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                                              ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_q
                                             [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                                              : 0ULL));
            VL_EXTEND_WI(65,32, __Vtemp391, vlTOPp->dma_research_observer__DOT__cur_a_main_n);
            VL_ADD_W(3, __Vtemp392, __Vtemp390, __Vtemp391);
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp392[0U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp392[1U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp392[2U]);
            if ((1U & vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U])) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound26 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U])));
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_main_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound26;
            }
            VL_EXTEND_WQ(65,64, __Vtemp395, ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                                              ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_q
                                             [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                                              : 0ULL));
            VL_EXTEND_WI(65,32, __Vtemp396, vlTOPp->dma_research_observer__DOT__cur_a_comp_n);
            VL_ADD_W(3, __Vtemp397, __Vtemp395, __Vtemp396);
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp397[0U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp397[1U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp397[2U]);
            if ((1U & vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U])) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound27 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U])));
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_comp_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound27;
            }
            VL_EXTEND_WQ(65,64, __Vtemp400, ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                                              ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_q
                                             [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                                              : 0ULL));
            VL_EXTEND_WI(65,32, __Vtemp401, vlTOPp->dma_research_observer__DOT__cur_a_dur_n);
            VL_ADD_W(3, __Vtemp402, __Vtemp400, __Vtemp401);
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp402[0U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp402[1U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp402[2U]);
            if ((1U & vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U])) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound28 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U])));
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_dur_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound28;
            }
            VL_EXTEND_WQ(65,64, __Vtemp405, ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                                              ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_q
                                             [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                                              : 0ULL));
            VL_EXTEND_WI(65,32, __Vtemp406, vlTOPp->dma_research_observer__DOT__cur_a_req_n);
            VL_ADD_W(3, __Vtemp407, __Vtemp405, __Vtemp406);
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp407[0U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp407[1U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp407[2U]);
            if ((1U & vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U])) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound29 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U])));
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_req_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound29;
            }
            VL_EXTEND_WQ(65,64, __Vtemp410, ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                                              ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_q
                                             [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                                              : 0ULL));
            VL_EXTEND_WI(65,32, __Vtemp411, vlTOPp->dma_research_observer__DOT__cur_a_miss_n);
            VL_ADD_W(3, __Vtemp412, __Vtemp410, __Vtemp411);
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U] 
                = __Vtemp412[0U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U] 
                = __Vtemp412[1U];
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U] 
                = (1U & __Vtemp412[2U]);
            if ((1U & vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[2U])) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n 
                    = (2U | (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__err_n));
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound30 
                = (((QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[1U])) 
                    << 0x20U) | (QData)((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__ext[0U])));
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__sum_miss_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                    = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound30;
            }
            if (((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp)) 
                 & ((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_q) 
                    >> (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp)))) {
                if ((vlTOPp->dma_research_observer__DOT__cur_a_main_n 
                     < ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_q
                        [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                         : 0U))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound31 
                        = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
                    if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound31;
                    }
                }
                if ((vlTOPp->dma_research_observer__DOT__cur_a_main_n 
                     > ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_q
                        [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                         : 0U))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound32 
                        = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
                    if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound32;
                    }
                }
                if ((vlTOPp->dma_research_observer__DOT__cur_a_comp_n 
                     < ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_q
                        [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                         : 0U))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound33 
                        = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
                    if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound33;
                    }
                }
                if ((vlTOPp->dma_research_observer__DOT__cur_a_comp_n 
                     > ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_q
                        [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                         : 0U))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound34 
                        = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
                    if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound34;
                    }
                }
                if ((vlTOPp->dma_research_observer__DOT__cur_a_dur_n 
                     < ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_q
                        [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                         : 0U))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound35 
                        = vlTOPp->dma_research_observer__DOT__cur_a_dur_n;
                    if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound35;
                    }
                }
                if ((vlTOPp->dma_research_observer__DOT__cur_a_dur_n 
                     > ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))
                         ? vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_q
                        [vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp]
                         : 0U))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound36 
                        = vlTOPp->dma_research_observer__DOT__cur_a_dur_n;
                    if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                        vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                            = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound36;
                    }
                }
            } else {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound37 
                    = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
                if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_main_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound37;
                }
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound38 
                    = vlTOPp->dma_research_observer__DOT__cur_a_main_n;
                if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_main_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound38;
                }
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound39 
                    = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
                if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_comp_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound39;
                }
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound40 
                    = vlTOPp->dma_research_observer__DOT__cur_a_comp_n;
                if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_comp_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound40;
                }
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound41 
                    = vlTOPp->dma_research_observer__DOT__cur_a_dur_n;
                if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__min_dur_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound41;
                }
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound42 
                    = vlTOPp->dma_research_observer__DOT__cur_a_dur_n;
                if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                    vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__max_dur_n[vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp] 
                        = vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound42;
                }
            }
            vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound43 = 1U;
            if ((5U >= (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) {
                vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_n 
                    = (((~ ((IData)(1U) << (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp))) 
                        & (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__seen_n)) 
                       | ((IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT____Vlvbound43) 
                          << (IData)(vlTOPp->dma_research_observer__DOT__u_dma_research_cost_groups__DOT__grp)));
            }
        }
    }
}
