#include "Ve3_delay_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <initializer_list>
struct B {
    Ve3_delay_top d;unsigned now=0,take=0,ready=0,done=0,take_t=0,ready_t=0,done_t=0;
    void tick(){d.clk_i=0;d.eval();if(d.capture_o){take++;take_t=now;}if(d.available_o){ready++;ready_t=now;}
        if(d.consume_o){done++;done_t=now;}d.clk_i=1;d.eval();now++;}
    B(){d.rst_ni=0;tick();d.rst_ni=1;d.enable_i=1;d.in_last_i=1;d.out_ready_i=1;tick();}
    void arm(bool delay){d.delay128_i=delay;d.arm_i=1;tick();d.arm_i=0;}
    void send(bool eligible,unsigned value=77){d.eligible_i=eligible;d.in_data_i=value;d.in_id_i=5;d.in_valid_i=1;tick();d.in_valid_i=0;}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);
    for(bool delay:{false,true}){
        B b;b.arm(delay);b.send(true);unsigned duration=delay?128:0;
        for(unsigned i=0;i<duration;i++){assert(!b.d.out_valid_o);b.tick();}
        assert(b.d.out_valid_o && b.d.out_data_o==77 && b.d.out_id_o==5 && b.d.out_last_o);
        assert(b.d.extra_o==duration);b.tick();assert(b.take==1 && b.done==1 && b.ready==1);
        assert(b.ready_t-b.take_t==duration+1 && b.done_t==b.ready_t);
        b.send(true,99);assert(b.d.out_valid_o && b.d.out_data_o==99);b.tick();assert(b.take==1);
        b.arm(delay);b.send(true);for(unsigned i=0;i<duration+1;i++)b.tick();assert(b.take==2 && b.done==2 && !b.d.error_o);
    }
    {B b;b.arm(true);b.send(false);assert(b.d.out_valid_o);b.tick();assert(!b.take && !b.done && !b.d.error_o);}
    {B b;b.arm(true);b.d.out_ready_i=0;b.send(true);
     // Disable/abort does not cancel a captured response; payload stays stable.
     b.d.enable_i=0;for(int i=0;i<140;i++)b.tick();assert(b.d.out_valid_o && b.d.out_data_o==77 && !b.done);
     for(int i=0;i<5;i++){b.d.in_data_i=100+i;b.tick();assert(b.d.out_data_o==77 && b.d.out_valid_o);}
     b.d.out_ready_i=1;b.tick();assert(b.done==1 && !b.d.error_o);}
    {B b;b.arm(true);b.d.out_ready_i=0;b.send(true);b.d.arm_i=1;b.tick();b.d.arm_i=0;
     assert(b.d.error_o);for(int i=0;i<128;i++)b.tick();assert(b.d.out_valid_o && b.d.out_data_o==77);}
    {B b;b.d.enable_i=0;b.d.in_valid_i=1;b.d.in_data_i=123;b.d.out_ready_i=0;b.d.eval();
     assert(b.d.out_valid_o && b.d.out_data_o==123 && !b.d.in_ready_o);b.d.out_ready_i=1;b.d.eval();assert(b.d.in_ready_o);}
    std::puts("PASS E3 response register: D0/D128, one-shot, repeat, backpressure, disable, busy ARM, bypass");
}
