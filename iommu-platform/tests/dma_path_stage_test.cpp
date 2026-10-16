#include "Vpath_stage_test_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
int main(int argc,char**argv) {
    Verilated::commandArgs(argc,argv);Vpath_stage_test_top d;
    auto tick=[&](){d.clk_i=0;d.eval();d.clk_i=1;d.eval();d.time_i=(d.time_i+1)&255;};
    auto clear=[&](){d.clear_i=1;tick();d.clear_i=0;d.enter_i=d.leave_i=0;};
    d.rst_ni=0;tick();d.rst_ni=1;d.run_i=d.active_i=1;
    clear();d.enter_i=d.leave_i=1;for(int i=0;i<8;i++)tick();
    assert(d.error_o); // 3-bit event counter overflow
    clear();d.enter_i=1;for(int i=0;i<4;i++)tick();d.enter_i=0;
    for(int i=0;i<130;i++)tick();
    assert(d.error_o); // 9-bit occupancy-area overflow
    clear();
    for(int j=0;j<3;j++) {
        d.enter_i=1;tick();d.enter_i=0;
        for(int i=0;i<190;i++)tick();
        d.leave_i=1;tick();d.leave_i=0;
    }
    assert(d.error_o && d.data_o[3] < 512); // 9-bit time-sum overflow
    clear();assert(!d.error_o);
    std::puts("PASS path stage: count/area/sum overflow and clear");
}
