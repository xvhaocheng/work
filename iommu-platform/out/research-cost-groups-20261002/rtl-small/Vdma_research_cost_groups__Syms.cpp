// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vdma_research_cost_groups__Syms.h"
#include "Vdma_research_cost_groups.h"



// FUNCTIONS
Vdma_research_cost_groups__Syms::Vdma_research_cost_groups__Syms(Vdma_research_cost_groups* topp, const char* namep)
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
}
