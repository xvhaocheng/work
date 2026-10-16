// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VIDMA_RESEARCH_FRONTEND_TOP__SYMS_H_
#define _VIDMA_RESEARCH_FRONTEND_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vidma_research_frontend_top.h"

// SYMS CLASS
class Vidma_research_frontend_top__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vidma_research_frontend_top*   TOPp;
    
    // CREATORS
    Vidma_research_frontend_top__Syms(Vidma_research_frontend_top* topp, const char* namep);
    ~Vidma_research_frontend_top__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
