// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VAXI_BC_TB__SYMS_H_
#define _VAXI_BC_TB__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vaxi_bc_tb.h"

// SYMS CLASS
class Vaxi_bc_tb__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vaxi_bc_tb*                    TOPp;
    
    // CREATORS
    Vaxi_bc_tb__Syms(Vaxi_bc_tb* topp, const char* namep);
    ~Vaxi_bc_tb__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
