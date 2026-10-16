#include "Vidma_batch_pair_top.h"
#include "verilated.h"
#include <array>
#include <stdexcept>
#include <iostream>
static void ck(bool b,const char*s){if(!b)throw std::runtime_error(s);}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
 for(unsigned v:{0U,1U})for(unsigned n:{8U,64U}){
  Vidma_batch_pair_top d;auto tick=[&](){d.clk_i=0;d.eval();d.clk_i=1;d.eval();d.clk_i=0;d.eval();};
  d.valid_i=d.write_i=0;d.rst_ni=0;tick();d.rst_ni=1;tick();
  auto access=[&](unsigned port,unsigned addr,bool wr=false,uint64_t val=0){d.port_i=port;d.addr_i=addr;d.write_i=wr;d.wdata_i=val;d.valid_i=1;d.eval();ck(d.ready_o&&!d.error_o,"register access");auto r=d.rdata_o;tick();d.valid_i=0;d.eval();return r;};
  for(unsigned p=0;p<2;p++){access(p,0,1,0x10000);access(p,8,1,0x100000);access(p,16,1,4096);access(p,24,1,0);access(p,0x90,1,p==v?n:1000);access(p,0x98,1,p==v?1:8);access(p,0x88,1,1);}
  access(0,0x68,1,n);access(0,0x60,1,1|(1ULL<<25)|(1ULL<<24)|(v<<8)|((1-v)<<16));
  access(1-v,0x28);auto id=access(v,0x28);
  unsigned tries=0;while(access(v,0x30)!=id&&tries++<2000){}ck(tries<2000,"batch completes");
  access(1-v,0x88,1,2);for(unsigned i=0;i<30&&d.running_o;i++)tick();ck(d.running_o==0,"both drained");
  ck(access(0,0x60)==4,"integrated observer valid frozen");
  std::array<uint64_t,35> r{};for(unsigned i=0;i<35;i++){access(0,0x70,1,i);r[i]=access(0,0x78);}
  auto h=n*(v==0?6:9);ck(r[1]==h&&r[3]==n-1&&r[0]==h+n-1,"independently expected H and hardware gap");
  ck(r[7]==n&&r[8]==n&&r[9]==1&&r[2]==0,"task versus batch observation counts");
  ck(r[13]>0&&r[0]==r[10]+r[11]+r[12]+r[13],"true simultaneous synthetic transactions observed");
  for(unsigned lane=0;lane<2;lane++){ck((r[30+lane]>>16)+r[14+lane*6]==(r[32+lane]>>16)+r[16+lane*6],"read conservation across opening edge");ck((r[30+lane]&65535)+r[15+lane*6]==(r[32+lane]&65535)+r[17+lane*6],"write conservation");}
 }
 std::cout<<"PASS 4 integrated production frontend/observer pairs, two roles and two lengths\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
