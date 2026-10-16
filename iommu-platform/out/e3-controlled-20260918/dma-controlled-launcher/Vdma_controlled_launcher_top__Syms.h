// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VDMA_CONTROLLED_LAUNCHER_TOP__SYMS_H_
#define _VDMA_CONTROLLED_LAUNCHER_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vdma_controlled_launcher_top.h"

// SYMS CLASS
class Vdma_controlled_launcher_top__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vdma_controlled_launcher_top*  TOPp;
    
    // CREATORS
    Vdma_controlled_launcher_top__Syms(Vdma_controlled_launcher_top* topp, const char* namep);
    ~Vdma_controlled_launcher_top__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
