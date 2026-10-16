// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VRESEARCH_AR_COMPONENT_TB__SYMS_H_
#define _VRESEARCH_AR_COMPONENT_TB__SYMS_H_  // guard

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "Vresearch_ar_component_tb.h"

// SYMS CLASS
class Vresearch_ar_component_tb__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vresearch_ar_component_tb*     TOPp;
    
    // SCOPE NAMES
    VerilatedScope __Vscope_research_ar_component_tb__dut;
    VerilatedScope __Vscope_research_ar_component_tb__dut__p_mode;
    VerilatedScope __Vscope_research_ar_component_tb__dut__p_monitor;
    
    // CREATORS
    Vresearch_ar_component_tb__Syms(Vresearch_ar_component_tb* topp, const char* namep);
    ~Vresearch_ar_component_tb__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
