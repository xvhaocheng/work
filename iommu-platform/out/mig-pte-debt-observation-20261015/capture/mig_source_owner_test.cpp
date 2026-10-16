#include "Vmig_source_owner.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
struct B {
    Vmig_source_owner d;
    void tick(unsigned n=1){while(n--){d.clk_i=0;d.eval();d.clk_i=1;d.eval();d.clk_i=0;d.eval();}}
    void check(bool b,const char*s){if(!b)throw std::runtime_error(s);}
    void read(unsigned i,uint64_t v){d.rd_req_i=1;d.rd_index_i=i;tick();check(d.rd_valid_o&&d.rd_data_o==v,"record mismatch");d.rd_req_i=0;tick();}
    B(){d.arm_i=d.open_i=d.close_i=d.selected_toggle_async_i=0;d.issue_i=d.done_i=d.lane_did_fire_i=0;
        d.ptw_fire_i=d.ptw_error_i=d.comp_fire_i=d.comp_id_i=0;d.comp_addr_i=0x80001000;d.comp_len_i=255;
        d.comp_size_i=3;d.comp_burst_i=1;d.rd_req_i=0;d.rd_index_i=0;
        for(unsigned i=0;i<4;i++){d.issue_id_i[i]=d.done_id_i[i]=1;d.lane_did_i[i]=10+i;}
        for(unsigned i=0;i<8;i++)d.ptw_i[i]=0;
        d.rst_ni=0;tick(2);d.rst_ni=1;tick();}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    B b;auto&d=b.d;d.issue_i=d.lane_did_fire_i=1;b.tick();d.issue_i=d.lane_did_fire_i=0;
    d.arm_i=1;b.tick();d.arm_i=0;d.open_i=1;b.tick();d.open_i=0;
    d.ptw_i[0]=9;d.ptw_i[1]=0x8200000a;d.ptw_i[2]=0x80002000;d.ptw_i[4]=0x1000;d.ptw_i[6]=1;d.ptw_i[7]=0x05000000;
    d.ptw_fire_i=d.comp_fire_i=1;b.tick();d.ptw_fire_i=d.comp_fire_i=0;
    d.issue_i=d.done_i=1;b.tick();d.issue_i=d.done_i=0;d.ptw_fire_i=1;b.tick();d.ptw_fire_i=0;
    b.read(33,1);b.read(34,0x8200000a00000009ULL);b.read(38,1);b.read(39,0x1c00000001ULL);b.read(47,0x1c00000002ULL);
    d.comp_fire_i=1;for(unsigned j=0;j<109;j++){d.comp_addr_i=j*8;b.tick();}d.comp_fire_i=0;
    b.read(544+13*5+1,110);
    d.selected_toggle_async_i=1;b.tick(4);b.check(d.frozen_o,"freeze crossing");
    d.comp_addr_i=0xdead;d.comp_fire_i=d.ptw_fire_i=1;b.tick();d.comp_fire_i=d.ptw_fire_i=0;
    b.read(544+13*5+1,110);b.read(8,111);d.close_i=1;b.tick();d.close_i=0;
    b.read(9,3);b.read(10,3);b.read(0,0x4d49474f574e0001ULL);b.check(d.error_o==0,"unexpected errors");
    d.done_i=2;b.tick();d.done_i=0;d.comp_id_i=1;d.comp_fire_i=1;b.tick();d.comp_fire_i=0;
    b.check((d.error_o&0x201)==0x201,"missing sticky error");
    d.issue_i=d.done_i=d.comp_fire_i=d.ptw_fire_i=0;d.selected_toggle_async_i=0;
    d.rst_ni=0;b.tick(2);d.rst_ni=1;b.tick();
    b.read(11,0);b.check(d.error_o==0&&!d.frozen_o,"reset did not clear occupancy/error");
    d.issue_i=d.lane_did_fire_i=1;d.issue_id_i[0]=1;b.tick();d.issue_i=d.lane_did_fire_i=0;
    d.ptw_i[0]=55;d.ptw_fire_i=1;b.tick();d.ptw_fire_i=0;
    b.read(33,1);b.read(34,0x8200000a00000037ULL);b.read(39,0x1c00000001ULL);
    b.check(d.error_o==0,"new reset epoch reused stale memory validity");
    std::cout<<"PASS source owner: descriptor reuse/generation, context, ring wrap/freeze, continuous count, sticky errors\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
