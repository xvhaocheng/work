// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VDMA_E3_WRITE_OBSERVER__SYMS_H_
#define _VDMA_E3_WRITE_OBSERVER__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vdma_e3_write_observer.h"

// SYMS CLASS
class Vdma_e3_write_observer__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vdma_e3_write_observer*        TOPp;
    
    // CREATORS
    Vdma_e3_write_observer__Syms(Vdma_e3_write_observer* topp, const char* namep);
    ~Vdma_e3_write_observer__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
