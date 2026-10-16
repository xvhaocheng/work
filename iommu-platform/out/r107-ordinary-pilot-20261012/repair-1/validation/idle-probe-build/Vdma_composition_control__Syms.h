// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VDMA_COMPOSITION_CONTROL__SYMS_H_
#define _VDMA_COMPOSITION_CONTROL__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vdma_composition_control.h"

// SYMS CLASS
class Vdma_composition_control__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vdma_composition_control*      TOPp;
    
    // CREATORS
    Vdma_composition_control__Syms(Vdma_composition_control* topp, const char* namep);
    ~Vdma_composition_control__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
