// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VHPM_TEST_TOP__SYMS_H_
#define _VHPM_TEST_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vhpm_test_top.h"

// SYMS CLASS
class Vhpm_test_top__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vhpm_test_top*                 TOPp;
    
    // CREATORS
    Vhpm_test_top__Syms(Vhpm_test_top* topp, const char* namep);
    ~Vhpm_test_top__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
