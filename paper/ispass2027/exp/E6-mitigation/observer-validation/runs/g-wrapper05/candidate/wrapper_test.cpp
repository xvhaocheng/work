#include "Vwrapper_top.h"
#include <verilated.h>
#include <iostream>
#include <stdexcept>
static unsigned checks=0,cycles=0;
static void ck(bool v,const char*m){checks++;if(!v)throw std::runtime_error(m);}
struct Bench{
 Vwrapper_top d;
 void tick(){d.clk_i=0;d.eval();d.clk_i=1;d.eval();cycles++;}
 Bench(){d.rst_ni=0;d.wstrb_i=255;tick();d.rst_ni=1;tick();}
 uint64_t access(unsigned lane,unsigned addr,bool write=false,uint64_t value=0){
  d.lane_i=lane;d.addr_i=addr;d.cfg_valid_i=1;d.cfg_write_i=write;d.wdata_i=value;
  bool done=false;uint64_t result=0;
  for(int i=0;i<64;i++){d.clk_i=0;d.eval();if(d.ready_o){ck(!d.error_o,"real AXI configuration transaction failed");result=d.rdata_o;done=true;tick();break;}tick();}
  ck(done,"bounded real AXI configuration completion");d.cfg_valid_i=0;tick();return result;
 }
 uint64_t get(unsigned index){access(0,0x70,true,index);return access(0,0x78);}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
 Bench b;
 for(unsigned lane=0;lane<2;lane++){b.access(lane,0x88,true,0);b.access(lane,0x90,true,4096);b.access(lane,0x98,true,lane?8:1);b.access(lane,0x88,true,1);}
 b.access(1,0xe0,true,1000);
 ck(b.d.target0_o==4096&&b.d.target1_o==4096&&b.d.gap0_o==0&&b.d.gap1_o==1000,"actual frontend to wrapper readonly taps");
 ck(b.access(1,0xe8)==0x49444d4147415001ULL,"real wrapper gap cap");
 b.access(0,0x68,true,4096);b.access(0,0x60,true,1|(1<<25)|(1<<26)|(1<<24)|(1<<16));
 ck(b.access(0,0x60)==1,"real frontend observer forwarding accepts ARM");
 ck(b.get(768)==0x49444d4150344d02ULL&&b.get(769)==0,"real wrapper G bank cap/reserved0");
 ck(b.get(772)==4096&&b.get(773)==4096&&b.get(774)==0&&b.get(775)==1000,"observer snapshots actual hardware targets/gaps");
 b.access(1,0xe0,true,2000);
 ck(b.d.gap1_o==2000&&b.get(775)==1000&&(b.get(771)&512),"actual live config change invalidates G while snapshot remains");
 ck(b.access(0,0x60)&(1ULL<<38),"actual G error gates old status valid");
 std::cout<<"PASS real wrapper/frontend/observer configuration chain checks="<<checks<<" cycles="<<cycles<<"; no DMA jobs or board\n";return 0;
 }catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
