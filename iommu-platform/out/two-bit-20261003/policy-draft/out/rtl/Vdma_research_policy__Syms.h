// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VDMA_RESEARCH_POLICY__SYMS_H_
#define _VDMA_RESEARCH_POLICY__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vdma_research_policy.h"

// SYMS CLASS
class Vdma_research_policy__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vdma_research_policy*          TOPp;
    
    // CREATORS
    Vdma_research_policy__Syms(Vdma_research_policy* topp, const char* namep);
    ~Vdma_research_policy__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
