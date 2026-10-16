#include "Vidma_frontend_observer_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
static void check(bool c,const char*m){if(!c)throw std::runtime_error(m);}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    Vidma_frontend_observer_top d;
    auto eval=[&](){d.eval();check(d.unchanged_o,"normal behavior differs from pre-observation frontend");};
    auto tick=[&](){d.clk_i=0;eval();d.clk_i=1;eval();d.clk_i=0;eval();};
    d.valid_i=d.write_i=d.complete_i=0;d.ready_i=1;d.rst_ni=0;tick();d.rst_ni=1;tick();
    auto access=[&](unsigned addr,bool wr,uint64_t value){d.addr_i=addr;d.write_i=wr;d.wdata_i=value;d.valid_i=1;eval();check(d.ready_o,"register ready");auto x=d.rdata_o;tick();d.valid_i=0;eval();return x;};
    access(0,1,0x1000);access(8,1,0x2000);access(16,1,4096);access(24,1,0);
    for(unsigned job=0;job<12;job++){
        uint64_t id=job+2;
        check(access(0x58,0,0)==0x49444d414f425301ULL,"observer window");
        d.addr_i=0x28;d.write_i=0;d.valid_i=1;eval();check(d.issue_o&&d.issue_id_o==id&&d.rdata_o==id,"issue identity");tick();d.valid_i=0;
        d.ready_i=0;for(int i=0;i<3;i++){eval();check(d.request_o,"request held until ready");check(d.src_o==0x1000&&d.dst_o==0x2000,"config not aliased");tick();}
        d.ready_i=1;tick();
        check(access(0x30,0,0)==id-1,"old completion while active");
        // Register-window reads/writes while WAIT cannot affect functional state.
        access(0x60,1,4);access(0x68,1,1);access(0x70,1,3);access(0x78,0,0);
        d.complete_i=1;eval();check(d.done_o&&d.done_id_o==id,"retire ID predicted from functional generator");tick();d.complete_i=0;
        d.addr_i=0x30;d.write_i=0;d.valid_i=1;eval();check(d.poll_o&&d.poll_id_o==id,"accepted DONE identity");tick();d.valid_i=0;
        check(access(0x30,0,0)==id,"completed result");
    }
    std::cout<<"PASS 12 production frontend transfers vs original, backpressure and observer register isolation\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
