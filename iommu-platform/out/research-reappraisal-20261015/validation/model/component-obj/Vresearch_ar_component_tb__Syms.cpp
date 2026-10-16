// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vresearch_ar_component_tb__Syms.h"
#include "Vresearch_ar_component_tb.h"



// FUNCTIONS
Vresearch_ar_component_tb__Syms::Vresearch_ar_component_tb__Syms(Vresearch_ar_component_tb* topp, const char* namep)
    // Setup locals
    : __Vm_namep(namep)
    , __Vm_didInit(false)
    // Setup submodule names
{
    // Pointer to top level
    TOPp = topp;
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOPp->__Vconfigure(this, true);
    // Setup scopes
    __Vscope_research_ar_component_tb__dut.configure(this, name(), "research_ar_component_tb.dut", "dut", -12, VerilatedScope::SCOPE_OTHER);
    __Vscope_research_ar_component_tb__dut__p_mode.configure(this, name(), "research_ar_component_tb.dut.p_mode", "p_mode", -12, VerilatedScope::SCOPE_OTHER);
    __Vscope_research_ar_component_tb__dut__p_monitor.configure(this, name(), "research_ar_component_tb.dut.p_monitor", "p_monitor", -12, VerilatedScope::SCOPE_OTHER);
}
