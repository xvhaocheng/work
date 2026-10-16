// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VPATH_STAGE_TEST_TOP__SYMS_H_
#define _VPATH_STAGE_TEST_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vpath_stage_test_top.h"

// SYMS CLASS
class Vpath_stage_test_top__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vpath_stage_test_top*          TOPp;
    
    // CREATORS
    Vpath_stage_test_top__Syms(Vpath_stage_test_top* topp, const char* namep);
    ~Vpath_stage_test_top__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
