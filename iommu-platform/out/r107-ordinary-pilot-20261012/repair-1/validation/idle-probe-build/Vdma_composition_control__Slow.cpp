// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdma_composition_control.h for the primary calling header

#include "Vdma_composition_control.h"
#include "Vdma_composition_control__Syms.h"

//==========

VL_CTOR_IMP(Vdma_composition_control) {
    Vdma_composition_control__Syms* __restrict vlSymsp = __VlSymsp = new Vdma_composition_control__Syms(this, name());
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdma_composition_control::__Vconfigure(Vdma_composition_control__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vdma_composition_control::~Vdma_composition_control() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vdma_composition_control::_settle__TOP__2(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_settle__TOP__2\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->dma_composition_control__DOT__repl_idx = 0U;
    if ((1U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0U;
    }
    if ((2U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 1U;
    }
    if ((4U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 2U;
    }
    if ((8U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 3U;
    }
    if ((0x10U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 4U;
    }
    if ((0x20U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 5U;
    }
    if ((0x40U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 6U;
    }
    if ((0x80U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 7U;
    }
    if ((0x100U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 8U;
    }
    if ((0x200U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 9U;
    }
    if ((0x400U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xaU;
    }
    if ((0x800U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xbU;
    }
    if ((0x1000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xcU;
    }
    if ((0x2000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xdU;
    }
    if ((0x4000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xeU;
    }
    if ((0x8000U & (IData)(vlTOPp->replace_mask_i))) {
        vlTOPp->dma_composition_control__DOT__repl_idx = 0xfU;
    }
    vlTOPp->dma_composition_control__DOT__lookup_ok 
        = (((IData)(vlTOPp->normal_i) & (IData)(vlTOPp->stage1_i)) 
           & (~ (IData)(vlTOPp->stage2_i)));
    vlTOPp->dma_composition_control__DOT__cfg_data_legal 
        = (((0U == (IData)((vlTOPp->wdata_i >> 0x20U))) 
            & (0U == (0x3fU & (IData)((vlTOPp->wdata_i 
                                       >> 2U))))) & 
           ((0xaU == (0xffffffU & (IData)((vlTOPp->wdata_i 
                                           >> 8U)))) 
            | (0xbU == (0xffffffU & (IData)((vlTOPp->wdata_i 
                                             >> 8U))))));
    vlTOPp->mode_o = vlTOPp->dma_composition_control__DOT__mode_q;
    vlTOPp->protected_did_o = vlTOPp->dma_composition_control__DOT__protected_did_q;
    vlTOPp->dma_composition_control__DOT__state_d = vlTOPp->dma_composition_control__DOT__state_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__state_d = 1U;
            }
        }
    }
    if (vlTOPp->open_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__state_d = 2U;
            }
        }
    }
    if (vlTOPp->close_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__state_d = 3U;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__seq_d = vlTOPp->dma_composition_control__DOT__seq_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                if ((0xffffffffffffffffULL != vlTOPp->dma_composition_control__DOT__seq_q)) {
                    vlTOPp->dma_composition_control__DOT__seq_d 
                        = (1ULL + vlTOPp->dma_composition_control__DOT__seq_q);
                }
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__plru_close_d 
        = vlTOPp->dma_composition_control__DOT__plru_close_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_close_d = 0U;
            }
        }
    }
    if (vlTOPp->close_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_close_d 
                    = vlTOPp->plru_i;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__win_gen_d 
        = vlTOPp->dma_composition_control__DOT__win_gen_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__win_gen_d 
                    = vlTOPp->dma_composition_control__DOT__generation_q;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[1U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [1U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[2U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [2U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[3U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [3U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[4U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [4U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[5U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [5U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[6U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [6U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[7U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [7U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[8U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [8U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[9U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [9U][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xaU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xbU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xcU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xdU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xeU][3U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][0U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][1U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][2U];
    vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_open_q
        [0xfU][3U];
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][3U] = 0U;
            }
        }
    }
    if (vlTOPp->open_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][0U] 
                    = vlTOPp->entries_i[0U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][1U] 
                    = vlTOPp->entries_i[0U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][2U] 
                    = vlTOPp->entries_i[0U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0U][3U] 
                    = vlTOPp->entries_i[0U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][0U] 
                    = vlTOPp->entries_i[1U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][1U] 
                    = vlTOPp->entries_i[1U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][2U] 
                    = vlTOPp->entries_i[1U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[1U][3U] 
                    = vlTOPp->entries_i[1U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][0U] 
                    = vlTOPp->entries_i[2U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][1U] 
                    = vlTOPp->entries_i[2U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][2U] 
                    = vlTOPp->entries_i[2U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[2U][3U] 
                    = vlTOPp->entries_i[2U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][0U] 
                    = vlTOPp->entries_i[3U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][1U] 
                    = vlTOPp->entries_i[3U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][2U] 
                    = vlTOPp->entries_i[3U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[3U][3U] 
                    = vlTOPp->entries_i[3U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][0U] 
                    = vlTOPp->entries_i[4U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][1U] 
                    = vlTOPp->entries_i[4U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][2U] 
                    = vlTOPp->entries_i[4U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[4U][3U] 
                    = vlTOPp->entries_i[4U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][0U] 
                    = vlTOPp->entries_i[5U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][1U] 
                    = vlTOPp->entries_i[5U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][2U] 
                    = vlTOPp->entries_i[5U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[5U][3U] 
                    = vlTOPp->entries_i[5U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][0U] 
                    = vlTOPp->entries_i[6U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][1U] 
                    = vlTOPp->entries_i[6U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][2U] 
                    = vlTOPp->entries_i[6U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[6U][3U] 
                    = vlTOPp->entries_i[6U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][0U] 
                    = vlTOPp->entries_i[7U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][1U] 
                    = vlTOPp->entries_i[7U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][2U] 
                    = vlTOPp->entries_i[7U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[7U][3U] 
                    = vlTOPp->entries_i[7U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][0U] 
                    = vlTOPp->entries_i[8U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][1U] 
                    = vlTOPp->entries_i[8U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][2U] 
                    = vlTOPp->entries_i[8U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[8U][3U] 
                    = vlTOPp->entries_i[8U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][0U] 
                    = vlTOPp->entries_i[9U][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][1U] 
                    = vlTOPp->entries_i[9U][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][2U] 
                    = vlTOPp->entries_i[9U][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[9U][3U] 
                    = vlTOPp->entries_i[9U][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][0U] 
                    = vlTOPp->entries_i[0xaU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][1U] 
                    = vlTOPp->entries_i[0xaU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][2U] 
                    = vlTOPp->entries_i[0xaU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xaU][3U] 
                    = vlTOPp->entries_i[0xaU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][0U] 
                    = vlTOPp->entries_i[0xbU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][1U] 
                    = vlTOPp->entries_i[0xbU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][2U] 
                    = vlTOPp->entries_i[0xbU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xbU][3U] 
                    = vlTOPp->entries_i[0xbU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][0U] 
                    = vlTOPp->entries_i[0xcU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][1U] 
                    = vlTOPp->entries_i[0xcU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][2U] 
                    = vlTOPp->entries_i[0xcU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xcU][3U] 
                    = vlTOPp->entries_i[0xcU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][0U] 
                    = vlTOPp->entries_i[0xdU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][1U] 
                    = vlTOPp->entries_i[0xdU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][2U] 
                    = vlTOPp->entries_i[0xdU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xdU][3U] 
                    = vlTOPp->entries_i[0xdU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][0U] 
                    = vlTOPp->entries_i[0xeU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][1U] 
                    = vlTOPp->entries_i[0xeU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][2U] 
                    = vlTOPp->entries_i[0xeU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xeU][3U] 
                    = vlTOPp->entries_i[0xeU][3U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][0U] 
                    = vlTOPp->entries_i[0xfU][0U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][1U] 
                    = vlTOPp->entries_i[0xfU][1U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][2U] 
                    = vlTOPp->entries_i[0xfU][2U];
                vlTOPp->dma_composition_control__DOT__entries_open_d[0xfU][3U] 
                    = vlTOPp->entries_i[0xfU][3U];
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[1U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [1U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[2U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [2U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[3U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [3U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[4U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [4U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[5U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [5U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[6U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [6U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[7U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [7U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[8U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [8U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[9U][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [9U][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xaU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xbU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xcU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xdU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xeU][3U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][0U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][0U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][1U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][1U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][2U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][2U];
    vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][3U] 
        = vlTOPp->dma_composition_control__DOT__entries_close_q
        [0xfU][3U];
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][3U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][0U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][1U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][2U] = 0U;
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][3U] = 0U;
            }
        }
    }
    if (vlTOPp->close_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][0U] 
                    = vlTOPp->entries_next_i[0U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][1U] 
                    = vlTOPp->entries_next_i[0U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][2U] 
                    = vlTOPp->entries_next_i[0U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0U][3U] 
                    = vlTOPp->entries_next_i[0U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][0U] 
                    = vlTOPp->entries_next_i[1U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][1U] 
                    = vlTOPp->entries_next_i[1U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][2U] 
                    = vlTOPp->entries_next_i[1U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[1U][3U] 
                    = vlTOPp->entries_next_i[1U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][0U] 
                    = vlTOPp->entries_next_i[2U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][1U] 
                    = vlTOPp->entries_next_i[2U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][2U] 
                    = vlTOPp->entries_next_i[2U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[2U][3U] 
                    = vlTOPp->entries_next_i[2U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][0U] 
                    = vlTOPp->entries_next_i[3U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][1U] 
                    = vlTOPp->entries_next_i[3U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][2U] 
                    = vlTOPp->entries_next_i[3U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[3U][3U] 
                    = vlTOPp->entries_next_i[3U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][0U] 
                    = vlTOPp->entries_next_i[4U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][1U] 
                    = vlTOPp->entries_next_i[4U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][2U] 
                    = vlTOPp->entries_next_i[4U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[4U][3U] 
                    = vlTOPp->entries_next_i[4U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][0U] 
                    = vlTOPp->entries_next_i[5U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][1U] 
                    = vlTOPp->entries_next_i[5U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][2U] 
                    = vlTOPp->entries_next_i[5U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[5U][3U] 
                    = vlTOPp->entries_next_i[5U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][0U] 
                    = vlTOPp->entries_next_i[6U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][1U] 
                    = vlTOPp->entries_next_i[6U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][2U] 
                    = vlTOPp->entries_next_i[6U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[6U][3U] 
                    = vlTOPp->entries_next_i[6U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][0U] 
                    = vlTOPp->entries_next_i[7U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][1U] 
                    = vlTOPp->entries_next_i[7U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][2U] 
                    = vlTOPp->entries_next_i[7U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[7U][3U] 
                    = vlTOPp->entries_next_i[7U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][0U] 
                    = vlTOPp->entries_next_i[8U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][1U] 
                    = vlTOPp->entries_next_i[8U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][2U] 
                    = vlTOPp->entries_next_i[8U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[8U][3U] 
                    = vlTOPp->entries_next_i[8U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][0U] 
                    = vlTOPp->entries_next_i[9U][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][1U] 
                    = vlTOPp->entries_next_i[9U][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][2U] 
                    = vlTOPp->entries_next_i[9U][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[9U][3U] 
                    = vlTOPp->entries_next_i[9U][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][0U] 
                    = vlTOPp->entries_next_i[0xaU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][1U] 
                    = vlTOPp->entries_next_i[0xaU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][2U] 
                    = vlTOPp->entries_next_i[0xaU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xaU][3U] 
                    = vlTOPp->entries_next_i[0xaU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][0U] 
                    = vlTOPp->entries_next_i[0xbU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][1U] 
                    = vlTOPp->entries_next_i[0xbU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][2U] 
                    = vlTOPp->entries_next_i[0xbU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xbU][3U] 
                    = vlTOPp->entries_next_i[0xbU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][0U] 
                    = vlTOPp->entries_next_i[0xcU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][1U] 
                    = vlTOPp->entries_next_i[0xcU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][2U] 
                    = vlTOPp->entries_next_i[0xcU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xcU][3U] 
                    = vlTOPp->entries_next_i[0xcU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][0U] 
                    = vlTOPp->entries_next_i[0xdU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][1U] 
                    = vlTOPp->entries_next_i[0xdU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][2U] 
                    = vlTOPp->entries_next_i[0xdU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xdU][3U] 
                    = vlTOPp->entries_next_i[0xdU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][0U] 
                    = vlTOPp->entries_next_i[0xeU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][1U] 
                    = vlTOPp->entries_next_i[0xeU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][2U] 
                    = vlTOPp->entries_next_i[0xeU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xeU][3U] 
                    = vlTOPp->entries_next_i[0xeU][3U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][0U] 
                    = vlTOPp->entries_next_i[0xfU][0U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][1U] 
                    = vlTOPp->entries_next_i[0xfU][1U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][2U] 
                    = vlTOPp->entries_next_i[0xfU][2U];
                vlTOPp->dma_composition_control__DOT__entries_close_d[0xfU][3U] 
                    = vlTOPp->entries_next_i[0xfU][3U];
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__plru_open_d 
        = vlTOPp->dma_composition_control__DOT__plru_open_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_open_d = 0U;
            }
        }
    }
    if (vlTOPp->open_i) {
        if ((1U & (~ (IData)(vlTOPp->arm_i)))) {
            if ((1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__plru_open_d 
                    = vlTOPp->plru_i;
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__win_cfg_d 
        = vlTOPp->dma_composition_control__DOT__win_cfg_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__win_cfg_d 
                    = (((QData)((IData)(vlTOPp->dma_composition_control__DOT__protected_did_q)) 
                        << 8U) | (QData)((IData)(vlTOPp->dma_composition_control__DOT__mode_q)));
            }
        }
    }
    vlTOPp->rdata_o = 0ULL;
    if ((0x40U & (IData)(vlTOPp->index_i))) {
        if (((0x40U <= (IData)(vlTOPp->index_i)) & 
             (0x5fU >= (IData)(vlTOPp->index_i)))) {
            vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                ? (((QData)((IData)(
                                                    vlTOPp->dma_composition_control__DOT__entries_open_q
                                                    [
                                                    (0xfU 
                                                     & ((IData)(vlTOPp->index_i) 
                                                        >> 1U))][3U])) 
                                    << 0x20U) | (QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][2U])))
                                : (((QData)((IData)(
                                                    vlTOPp->dma_composition_control__DOT__entries_open_q
                                                    [
                                                    (0xfU 
                                                     & ((IData)(vlTOPp->index_i) 
                                                        >> 1U))][1U])) 
                                    << 0x20U) | (QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][0U]))));
        } else {
            if ((0x60U <= (IData)(vlTOPp->index_i))) {
                vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                    ? (((QData)((IData)(
                                                        vlTOPp->dma_composition_control__DOT__entries_close_q
                                                        [
                                                        (0xfU 
                                                         & ((IData)(vlTOPp->index_i) 
                                                            >> 1U))][3U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][2U])))
                                    : (((QData)((IData)(
                                                        vlTOPp->dma_composition_control__DOT__entries_close_q
                                                        [
                                                        (0xfU 
                                                         & ((IData)(vlTOPp->index_i) 
                                                            >> 1U))][1U])) 
                                        << 0x20U) | (QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][0U]))));
            }
        }
    } else {
        if ((0x20U & (IData)(vlTOPp->index_i))) {
            if ((0x10U & (IData)(vlTOPp->index_i))) {
                if (((0x40U <= (IData)(vlTOPp->index_i)) 
                     & (0x5fU >= (IData)(vlTOPp->index_i)))) {
                    vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                        ? (((QData)((IData)(
                                                            vlTOPp->dma_composition_control__DOT__entries_open_q
                                                            [
                                                            (0xfU 
                                                             & ((IData)(vlTOPp->index_i) 
                                                                >> 1U))][3U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->dma_composition_control__DOT__entries_open_q
                                                             [
                                                             (0xfU 
                                                              & ((IData)(vlTOPp->index_i) 
                                                                 >> 1U))][2U])))
                                        : (((QData)((IData)(
                                                            vlTOPp->dma_composition_control__DOT__entries_open_q
                                                            [
                                                            (0xfU 
                                                             & ((IData)(vlTOPp->index_i) 
                                                                >> 1U))][1U])) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             vlTOPp->dma_composition_control__DOT__entries_open_q
                                                             [
                                                             (0xfU 
                                                              & ((IData)(vlTOPp->index_i) 
                                                                 >> 1U))][0U]))));
                } else {
                    if ((0x60U <= (IData)(vlTOPp->index_i))) {
                        vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                            ? (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][3U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][2U])))
                                            : (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][1U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][0U]))));
                    }
                }
            } else {
                if ((8U & (IData)(vlTOPp->index_i))) {
                    if (((0x40U <= (IData)(vlTOPp->index_i)) 
                         & (0x5fU >= (IData)(vlTOPp->index_i)))) {
                        vlTOPp->rdata_o = ((1U & (IData)(vlTOPp->index_i))
                                            ? (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][3U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][2U])))
                                            : (((QData)((IData)(
                                                                vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                [
                                                                (0xfU 
                                                                 & ((IData)(vlTOPp->index_i) 
                                                                    >> 1U))][1U])) 
                                                << 0x20U) 
                                               | (QData)((IData)(
                                                                 vlTOPp->dma_composition_control__DOT__entries_open_q
                                                                 [
                                                                 (0xfU 
                                                                  & ((IData)(vlTOPp->index_i) 
                                                                     >> 1U))][0U]))));
                    } else {
                        if ((0x60U <= (IData)(vlTOPp->index_i))) {
                            vlTOPp->rdata_o = ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (
                                                   ((QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][3U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                     [
                                                                     (0xfU 
                                                                      & ((IData)(vlTOPp->index_i) 
                                                                         >> 1U))][2U])))
                                                : (
                                                   ((QData)((IData)(
                                                                    vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                    [
                                                                    (0xfU 
                                                                     & ((IData)(vlTOPp->index_i) 
                                                                        >> 1U))][1U])) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(
                                                                     vlTOPp->dma_composition_control__DOT__entries_close_q
                                                                     [
                                                                     (0xfU 
                                                                      & ((IData)(vlTOPp->index_i) 
                                                                         >> 1U))][0U]))));
                        }
                    }
                } else {
                    vlTOPp->rdata_o = ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(vlTOPp->dma_composition_control__DOT__pscid_a_q))
                                                : (QData)((IData)(vlTOPp->dma_composition_control__DOT__pscid_v_q)))
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(vlTOPp->dma_composition_control__DOT__plru_close_q))
                                                : (QData)((IData)(vlTOPp->dma_composition_control__DOT__plru_open_q))))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? 0ULL
                                                : 3ULL)
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x21U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x20U])));
                }
            }
        } else {
            vlTOPp->rdata_o = ((0x10U & (IData)(vlTOPp->index_i))
                                ? ((8U & (IData)(vlTOPp->index_i))
                                    ? ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1fU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1eU])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1dU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1cU]))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1bU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x1aU])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x19U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x18U])))
                                    : ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x17U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x16U])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x15U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x14U]))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x13U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x12U])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x11U]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0x10U]))))
                                : ((8U & (IData)(vlTOPp->index_i))
                                    ? ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xfU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xeU])
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xdU]
                                                : vlTOPp->dma_composition_control__DOT__cnt_q
                                               [0xcU]))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__win_gen_q
                                                : vlTOPp->dma_composition_control__DOT__win_cfg_q)
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(vlTOPp->dma_composition_control__DOT__err_q))
                                                : vlTOPp->dma_composition_control__DOT__cycles_q)))
                                    : ((4U & (IData)(vlTOPp->index_i))
                                        ? ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__seq_q
                                                : (
                                                   ((QData)((IData)(
                                                                    (3U 
                                                                     == (IData)(vlTOPp->dma_composition_control__DOT__state_q)))) 
                                                    << 2U) 
                                                   | (QData)((IData)(
                                                                     (((2U 
                                                                        == (IData)(vlTOPp->dma_composition_control__DOT__state_q)) 
                                                                       << 1U) 
                                                                      | (1U 
                                                                         == (IData)(vlTOPp->dma_composition_control__DOT__state_q)))))))
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? (QData)((IData)(
                                                                  (((((IData)(vlTOPp->quiet_i) 
                                                                      & (~ 
                                                                         ((1U 
                                                                           == (IData)(vlTOPp->dma_composition_control__DOT__state_q)) 
                                                                          | (2U 
                                                                             == (IData)(vlTOPp->dma_composition_control__DOT__state_q))))) 
                                                                     & (~ (IData)(vlTOPp->arm_i))) 
                                                                    & (~ (IData)(vlTOPp->open_i))) 
                                                                   & (~ (IData)(vlTOPp->close_i)))))
                                                : (QData)((IData)(vlTOPp->dma_composition_control__DOT__last_status_q))))
                                        : ((2U & (IData)(vlTOPp->index_i))
                                            ? ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? vlTOPp->dma_composition_control__DOT__generation_q
                                                : (
                                                   ((QData)((IData)(vlTOPp->dma_composition_control__DOT__protected_did_q)) 
                                                    << 8U) 
                                                   | (QData)((IData)(vlTOPp->dma_composition_control__DOT__mode_q))))
                                            : ((1U 
                                                & (IData)(vlTOPp->index_i))
                                                ? 1ULL
                                                : 0x49444d4143504e01ULL)))));
        }
    }
    vlTOPp->dma_composition_control__DOT__cfg_busy 
        = (1U & ((((((~ (IData)(vlTOPp->quiet_i)) | 
                     (1U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) 
                    | (2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) 
                   | (IData)(vlTOPp->arm_i)) | (IData)(vlTOPp->open_i)) 
                 | (IData)(vlTOPp->close_i)));
    vlTOPp->dma_composition_control__DOT__pscid_v_d 
        = vlTOPp->dma_composition_control__DOT__pscid_v_q;
    vlTOPp->dma_composition_control__DOT__seen_lookup_v_d 
        = vlTOPp->dma_composition_control__DOT__seen_lookup_v_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__seen_lookup_v_d = 0U;
                vlTOPp->dma_composition_control__DOT__pscid_v_d = 0U;
            }
        }
    }
    if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
        if (vlTOPp->lookup_i) {
            if ((0xaU == vlTOPp->lookup_did_i)) {
                if (vlTOPp->dma_composition_control__DOT__lookup_ok) {
                    if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__seen_lookup_v_d)))) {
                        vlTOPp->dma_composition_control__DOT__seen_lookup_v_d = 1U;
                        vlTOPp->dma_composition_control__DOT__pscid_v_d 
                            = vlTOPp->lookup_pscid_i;
                    }
                }
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__pscid_a_d 
        = vlTOPp->dma_composition_control__DOT__pscid_a_q;
    vlTOPp->dma_composition_control__DOT__seen_lookup_a_d 
        = vlTOPp->dma_composition_control__DOT__seen_lookup_a_q;
    if (vlTOPp->arm_i) {
        if ((1U & (~ ((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))))) {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__seen_lookup_a_d = 0U;
                vlTOPp->dma_composition_control__DOT__pscid_a_d = 0U;
            }
        }
    }
    if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
        if (vlTOPp->lookup_i) {
            if ((0xaU != vlTOPp->lookup_did_i)) {
                if ((0xbU == vlTOPp->lookup_did_i)) {
                    if (vlTOPp->dma_composition_control__DOT__lookup_ok) {
                        if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__seen_lookup_a_d)))) {
                            vlTOPp->dma_composition_control__DOT__seen_lookup_a_d = 1U;
                            vlTOPp->dma_composition_control__DOT__pscid_a_d 
                                = vlTOPp->lookup_pscid_i;
                        }
                    }
                }
            }
        }
    }
    vlTOPp->dma_composition_control__DOT__cycles_d 
        = vlTOPp->dma_composition_control__DOT__cycles_q;
    vlTOPp->dma_composition_control__DOT__err_d = vlTOPp->dma_composition_control__DOT__err_q;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [1U];
    vlTOPp->dma_composition_control__DOT__cnt_d[1U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [2U];
    vlTOPp->dma_composition_control__DOT__cnt_d[2U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [3U];
    vlTOPp->dma_composition_control__DOT__cnt_d[3U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [4U];
    vlTOPp->dma_composition_control__DOT__cnt_d[4U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [5U];
    vlTOPp->dma_composition_control__DOT__cnt_d[5U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [6U];
    vlTOPp->dma_composition_control__DOT__cnt_d[6U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [7U];
    vlTOPp->dma_composition_control__DOT__cnt_d[7U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [8U];
    vlTOPp->dma_composition_control__DOT__cnt_d[8U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [9U];
    vlTOPp->dma_composition_control__DOT__cnt_d[9U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xaU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xaU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xbU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xbU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xcU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xcU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xdU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xdU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xeU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xeU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0xfU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0xfU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x10U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x10U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x11U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x11U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x12U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x12U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x13U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x13U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x14U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x14U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x15U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x15U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x16U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x16U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x17U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x17U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x18U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x18U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x19U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x19U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1aU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1aU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1bU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1bU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1cU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1cU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1dU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1dU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1eU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x1fU];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x1fU] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x20U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x21U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x21U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x22U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x22U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x23U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x23U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x24U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x24U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x25U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x25U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x26U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x26U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    vlTOPp->dma_composition_control__DOT____Vlvbound1 
        = vlTOPp->dma_composition_control__DOT__cnt_q
        [0x27U];
    vlTOPp->dma_composition_control__DOT__cnt_d[0x27U] 
        = vlTOPp->dma_composition_control__DOT____Vlvbound1;
    if (vlTOPp->arm_i) {
        if (((IData)(vlTOPp->open_i) | (IData)(vlTOPp->close_i))) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__err_d = 0U;
                vlTOPp->dma_composition_control__DOT__cycles_d = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[1U] = 0ULL;
                if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__seq_q)) {
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                }
                vlTOPp->dma_composition_control__DOT__cnt_d[2U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[3U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[4U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[5U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[6U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[7U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[8U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[9U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xaU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xbU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xcU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xdU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xeU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0xfU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x10U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x11U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x12U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x13U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x14U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x15U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x16U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x17U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x18U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x19U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1aU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1bU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1cU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1dU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1fU] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x21U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x22U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x23U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x24U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x25U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x26U] = 0ULL;
                vlTOPp->dma_composition_control__DOT__cnt_d[0x27U] = 0ULL;
            }
        }
    }
    if (vlTOPp->open_i) {
        if (vlTOPp->arm_i) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            if ((1U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
    }
    if (vlTOPp->close_i) {
        if (vlTOPp->arm_i) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            if ((2U != (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
    }
    if ((2U == (IData)(vlTOPp->dma_composition_control__DOT__state_q))) {
        if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cycles_d)) {
            vlTOPp->dma_composition_control__DOT__err_d 
                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
        } else {
            vlTOPp->dma_composition_control__DOT__cycles_d 
                = (1ULL + vlTOPp->dma_composition_control__DOT__cycles_d);
        }
        if ((1U & (IData)(vlTOPp->gap_active_i))) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                 [0x18U])) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__cnt_d[0x18U] 
                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                       [0x18U]);
            }
        }
        if ((2U & (IData)(vlTOPp->gap_active_i))) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                 [0x19U])) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__cnt_d[0x19U] 
                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                       [0x19U]);
            }
        }
        if (vlTOPp->lookup_i) {
            if ((0xaU == vlTOPp->lookup_did_i)) {
                if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                     [0xcU])) {
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                } else {
                    vlTOPp->dma_composition_control__DOT__cnt_d[0xcU] 
                        = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                           [0xcU]);
                }
                if (vlTOPp->lookup_hit_i) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0xdU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0xdU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0xdU]);
                    }
                } else {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0xeU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0xeU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0xeU]);
                    }
                }
                if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__lookup_ok)))) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x20U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x20U]);
                    }
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                }
            } else {
                if ((0xbU == vlTOPp->lookup_did_i)) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0xfU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0xfU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0xfU]);
                    }
                    if (vlTOPp->lookup_hit_i) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x10U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x10U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x10U]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x11U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x11U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x11U]);
                        }
                    }
                    if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__lookup_ok)))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x20U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x20U]);
                        }
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    }
                } else {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x12U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x12U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x12U]);
                    }
                    if (vlTOPp->lookup_hit_i) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x13U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x13U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x13U]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x14U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x14U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x14U]);
                        }
                    }
                    if ((1U & (~ (IData)(vlTOPp->dma_composition_control__DOT__lookup_ok)))) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x20U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x20U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x20U]);
                        }
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    }
                }
            }
        }
        if (vlTOPp->fast_i) {
            if (vlTOPp->accept_i) {
                if ((0xaU == vlTOPp->fast_did_i)) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x15U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x15U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x15U]);
                    }
                } else {
                    if ((0xbU == vlTOPp->fast_did_i)) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x16U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x16U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x16U]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x17U])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x17U] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x17U]);
                        }
                    }
                }
                if ((1U & (~ ((1U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)) 
                              | (3U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)))))) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x21U])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x21U] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x21U]);
                    }
                    vlTOPp->dma_composition_control__DOT__err_d 
                        = (4U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                }
            } else {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (1U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
        if (vlTOPp->update_i) {
            if (((0U != (IData)(vlTOPp->replace_mask_i)) 
                 & (0U == ((IData)(vlTOPp->replace_mask_i) 
                           & ((IData)(vlTOPp->replace_mask_i) 
                              - (IData)(1U)))))) {
                if ((0xaU == vlTOPp->update_did_i)) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x1aU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x1aU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x1aU]);
                    }
                } else {
                    if ((0xbU == vlTOPp->update_did_i)) {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x1bU])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x1bU] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x1bU]);
                        }
                    } else {
                        if ((0xffffffffffffffffULL 
                             == vlTOPp->dma_composition_control__DOT__cnt_d
                             [0x1cU])) {
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        } else {
                            vlTOPp->dma_composition_control__DOT__cnt_d[0x1cU] 
                                = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                   [0x1cU]);
                        }
                    }
                }
                if (((2U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)) 
                     | (3U == (IData)(vlTOPp->dma_composition_control__DOT__mode_q)))) {
                    if ((vlTOPp->update_did_i == vlTOPp->dma_composition_control__DOT__protected_did_q)) {
                        if ((2U <= (IData)(vlTOPp->dma_composition_control__DOT__repl_idx))) {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_composition_control__DOT__cnt_d
                                 [0x1eU])) {
                                vlTOPp->dma_composition_control__DOT__err_d 
                                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                            } else {
                                vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] 
                                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                       [0x1eU]);
                            }
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (0x10U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        }
                    } else {
                        if ((2U > (IData)(vlTOPp->dma_composition_control__DOT__repl_idx))) {
                            if ((0xffffffffffffffffULL 
                                 == vlTOPp->dma_composition_control__DOT__cnt_d
                                 [0x1eU])) {
                                vlTOPp->dma_composition_control__DOT__err_d 
                                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                            } else {
                                vlTOPp->dma_composition_control__DOT__cnt_d[0x1eU] 
                                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                                       [0x1eU]);
                            }
                            vlTOPp->dma_composition_control__DOT__err_d 
                                = (0x10U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                        }
                    }
                }
                if (((vlTOPp->entries_i[vlTOPp->dma_composition_control__DOT__repl_idx][1U] 
                      >> 0x13U) & ((0xfffffU & ((vlTOPp->entries_i
                                                 [vlTOPp->dma_composition_control__DOT__repl_idx][1U] 
                                                 << 3U) 
                                                | (vlTOPp->entries_i
                                                   [vlTOPp->dma_composition_control__DOT__repl_idx][0U] 
                                                   >> 0x1dU))) 
                                   != (0xfffffU & (
                                                   (vlTOPp->entries_next_i
                                                    [vlTOPp->dma_composition_control__DOT__repl_idx][1U] 
                                                    << 3U) 
                                                   | (vlTOPp->entries_next_i
                                                      [vlTOPp->dma_composition_control__DOT__repl_idx][0U] 
                                                      >> 0x1dU)))))) {
                    if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                         [0x1dU])) {
                        vlTOPp->dma_composition_control__DOT__err_d 
                            = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
                    } else {
                        vlTOPp->dma_composition_control__DOT__cnt_d[0x1dU] 
                            = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                               [0x1dU]);
                    }
                }
            } else {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (8U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            }
        }
        if (vlTOPp->invalidate_i) {
            if ((0xffffffffffffffffULL == vlTOPp->dma_composition_control__DOT__cnt_d
                 [0x1fU])) {
                vlTOPp->dma_composition_control__DOT__err_d 
                    = (2U | (IData)(vlTOPp->dma_composition_control__DOT__err_d));
            } else {
                vlTOPp->dma_composition_control__DOT__cnt_d[0x1fU] 
                    = (1ULL + vlTOPp->dma_composition_control__DOT__cnt_d
                       [0x1fU]);
            }
        }
    }
    vlTOPp->clear_o = (((((((IData)(vlTOPp->rst_ni) 
                            & (IData)(vlTOPp->cfg_write_i)) 
                           & (2U == (IData)(vlTOPp->index_i))) 
                          & (~ (IData)(vlTOPp->dma_composition_control__DOT__cfg_busy))) 
                         & (IData)(vlTOPp->supported_i)) 
                        & (IData)(vlTOPp->dma_composition_control__DOT__cfg_data_legal)) 
                       & (0xffffffffffffffffULL != vlTOPp->dma_composition_control__DOT__generation_q));
}

void Vdma_composition_control::_eval_initial(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_eval_initial\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk_i = vlTOPp->clk_i;
}

void Vdma_composition_control::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::final\n"); );
    // Variables
    Vdma_composition_control__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdma_composition_control::_eval_settle(Vdma_composition_control__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_eval_settle\n"); );
    Vdma_composition_control* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

void Vdma_composition_control::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdma_composition_control::_ctor_var_reset\n"); );
    // Body
    clk_i = VL_RAND_RESET_I(1);
    rst_ni = VL_RAND_RESET_I(1);
    cfg_write_i = VL_RAND_RESET_I(1);
    index_i = VL_RAND_RESET_I(7);
    wdata_i = VL_RAND_RESET_Q(64);
    rdata_o = VL_RAND_RESET_Q(64);
    quiet_i = VL_RAND_RESET_I(1);
    supported_i = VL_RAND_RESET_I(1);
    arm_i = VL_RAND_RESET_I(1);
    open_i = VL_RAND_RESET_I(1);
    close_i = VL_RAND_RESET_I(1);
    mode_o = VL_RAND_RESET_I(2);
    protected_did_o = VL_RAND_RESET_I(24);
    clear_o = VL_RAND_RESET_I(1);
    lookup_i = VL_RAND_RESET_I(1);
    lookup_hit_i = VL_RAND_RESET_I(1);
    normal_i = VL_RAND_RESET_I(1);
    stage1_i = VL_RAND_RESET_I(1);
    stage2_i = VL_RAND_RESET_I(1);
    lookup_did_i = VL_RAND_RESET_I(24);
    lookup_pscid_i = VL_RAND_RESET_I(20);
    accept_i = VL_RAND_RESET_I(1);
    fast_i = VL_RAND_RESET_I(1);
    fast_did_i = VL_RAND_RESET_I(24);
    gap_active_i = VL_RAND_RESET_I(2);
    update_i = VL_RAND_RESET_I(1);
    invalidate_i = VL_RAND_RESET_I(1);
    update_did_i = VL_RAND_RESET_I(24);
    replace_mask_i = VL_RAND_RESET_I(16);
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, entries_i[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, entries_next_i[__Vi0]);
    }}
    plru_i = VL_RAND_RESET_I(32);
    dma_composition_control__DOT__state_q = VL_RAND_RESET_I(2);
    dma_composition_control__DOT__state_d = VL_RAND_RESET_I(2);
    dma_composition_control__DOT__seq_q = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__seq_d = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__cycles_q = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__cycles_d = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__err_q = VL_RAND_RESET_I(5);
    dma_composition_control__DOT__err_d = VL_RAND_RESET_I(5);
    { int __Vi0=0; for (; __Vi0<40; ++__Vi0) {
            dma_composition_control__DOT__cnt_q[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<40; ++__Vi0) {
            dma_composition_control__DOT__cnt_d[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    dma_composition_control__DOT__win_cfg_q = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__win_cfg_d = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__win_gen_q = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__win_gen_d = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__plru_open_q = VL_RAND_RESET_I(32);
    dma_composition_control__DOT__plru_open_d = VL_RAND_RESET_I(32);
    dma_composition_control__DOT__plru_close_q = VL_RAND_RESET_I(32);
    dma_composition_control__DOT__plru_close_d = VL_RAND_RESET_I(32);
    dma_composition_control__DOT__pscid_v_q = VL_RAND_RESET_I(20);
    dma_composition_control__DOT__pscid_v_d = VL_RAND_RESET_I(20);
    dma_composition_control__DOT__pscid_a_q = VL_RAND_RESET_I(20);
    dma_composition_control__DOT__pscid_a_d = VL_RAND_RESET_I(20);
    dma_composition_control__DOT__seen_lookup_v_q = VL_RAND_RESET_I(1);
    dma_composition_control__DOT__seen_lookup_v_d = VL_RAND_RESET_I(1);
    dma_composition_control__DOT__seen_lookup_a_q = VL_RAND_RESET_I(1);
    dma_composition_control__DOT__seen_lookup_a_d = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_composition_control__DOT__entries_open_q[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_composition_control__DOT__entries_open_d[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_composition_control__DOT__entries_close_q[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            VL_RAND_RESET_W(128, dma_composition_control__DOT__entries_close_d[__Vi0]);
    }}
    dma_composition_control__DOT__mode_q = VL_RAND_RESET_I(2);
    dma_composition_control__DOT__protected_did_q = VL_RAND_RESET_I(24);
    dma_composition_control__DOT__generation_q = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT__last_status_q = VL_RAND_RESET_I(3);
    dma_composition_control__DOT__cfg_busy = VL_RAND_RESET_I(1);
    dma_composition_control__DOT__cfg_data_legal = VL_RAND_RESET_I(1);
    dma_composition_control__DOT__repl_idx = VL_RAND_RESET_I(4);
    dma_composition_control__DOT__lookup_ok = VL_RAND_RESET_I(1);
    dma_composition_control__DOT____Vlvbound1 = VL_RAND_RESET_Q(64);
    dma_composition_control__DOT____Vlvbound3 = VL_RAND_RESET_Q(64);
}
