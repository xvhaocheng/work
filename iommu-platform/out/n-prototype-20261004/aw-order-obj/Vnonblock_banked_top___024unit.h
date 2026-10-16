// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnonblock_banked_top.h for the primary calling header

#ifndef _VNONBLOCK_BANKED_TOP___024UNIT_H_
#define _VNONBLOCK_BANKED_TOP___024UNIT_H_  // guard

#include "verilated_heavy.h"

//==========

class Vnonblock_banked_top__Syms;

//----------

VL_MODULE(Vnonblock_banked_top___024unit) {
  public:
    
    // INTERNAL VARIABLES
  private:
    Vnonblock_banked_top__Syms* __VlSymsp;  // Symbol table
  public:
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vnonblock_banked_top___024unit);  ///< Copying not allowed
  public:
    Vnonblock_banked_top___024unit(const char* name = "TOP");
    ~Vnonblock_banked_top___024unit();
    
    // INTERNAL METHODS
    void __Vconfigure(Vnonblock_banked_top__Syms* symsp, bool first);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
