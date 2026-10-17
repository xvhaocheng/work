#include "Vcertificate_top.h"
#include <verilated.h>
#include <cstdio>
#include <stdexcept>
double sc_time_stamp(){return 0;}
int main(int argc,char **argv){Verilated::commandArgs(argc,argv);try{
 Vcertificate_top d;unsigned cycles=0,checks=0,cases=0;
 auto require=[&](bool ok,const char *why){++checks;if(!ok)throw std::runtime_error(why);};
 auto tick=[&](){d.clk_i=0;d.eval();require(d.equal_o,"pre-edge donor gold mismatch");d.clk_i=1;d.eval();require(d.equal_o,"post-edge donor gold mismatch");++cycles;};
 auto reset=[&](){d.rst_ni=0;d.job_fire_i=d.job_done_i=0;d.normal_copy_i=1;d.length_i=4096;d.src_i=0x10000;d.dst_i=0x40000;tick();d.rst_ni=1;require(!d.permit_o&&!d.pending_o&&!d.fault_o,"reset");};
 auto send=[&](bool f,bool done){d.job_fire_i=f;d.job_done_i=done;tick();d.job_fire_i=d.job_done_i=0;};
 reset();++cases;send(1,0);require(d.permit_o==2&&d.pending_o==3,"real static24/4096 capacity distinction");
 d.normal_copy_i=0;send(0,0);require(!d.permit_o&&d.pending_o==3,"live normal gate");
 d.normal_copy_i=1;send(0,0);require(d.permit_o==2,"original normal gate restore");send(0,1);require(!d.permit_o&&!d.pending_o,"retire revokes");
 const uint64_t bad_lengths[]={0,64,4095,8192};
 for(unsigned invalid=0;invalid<7;invalid++){
  reset();++cases;if(invalid<4)d.length_i=bad_lengths[invalid];
  if(invalid==4)d.src_i+=8;if(invalid==5)d.dst_i+=16;if(invalid==6)d.normal_copy_i=0;
  send(1,0);require(!d.permit_o&&d.pending_o==3&&!d.fault_o,"ineligible acceptance fail closed");send(0,1);require(!d.pending_o,"ineligible retires");
 }
 reset();++cases;send(0,1);require(d.fault_o==3&&!d.pending_o,"unexpected retirement faults");send(1,0);require(d.fault_o==3&&!d.permit_o,"fault persists through future legal job");send(0,1);
 reset();++cases;send(1,0);send(1,0);require(d.fault_o==3&&!d.permit_o&&d.pending_o==3,"overlap revokes until reset");send(0,1);send(1,0);require(!d.permit_o&&d.fault_o==3,"overlap fault sticky");
 reset();++cases;send(1,0);send(1,1);require(d.permit_o==2&&d.pending_o==3&&!d.fault_o,"same-edge replaces existing sole job");d.length_i=64;send(1,1);require(!d.permit_o&&d.pending_o==3&&!d.fault_o,"same-edge ineligible replacement");send(0,1);
 reset();++cases;send(1,1);require(!d.permit_o&&!d.pending_o&&!d.fault_o,"empty immediate fire+done needs no permission");
 std::printf("{\"event\":\"certificate_unit_passed\",\"cases\":%u,\"cycles\":%u,\"checks\":%u,\"fullwrap_cases\":0}\n",cases,cycles,checks);return 0;
 }catch(const std::exception &e){std::fprintf(stderr,"FAIL %s\n",e.what());return 1;}}
