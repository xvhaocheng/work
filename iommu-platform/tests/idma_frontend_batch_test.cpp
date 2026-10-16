#include "Vidma_frontend_observer_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
static void ck(bool b,const char*s){if(!b)throw std::runtime_error(s);}
struct Bench {
 Vidma_frontend_observer_top d;
 uint64_t cycle=0,due=0;unsigned pages=1;bool stall=false;
 std::vector<uint64_t> src;unsigned completions=0;
 void eval(){d.clk_i=0;d.ready_i=!stall;d.complete_i=due&&cycle==due;d.eval();}
 void tick(){
  eval();
  if(d.rst_ni&&d.request_o&&d.ready_i){
   ck(!due,"one task outstanding");
   ck(d.length_o==4096&&d.type_o==0,"DEFAULT 4K copies");
   ck(d.src_o==0x10000+(src.size()%pages)*4096,"source ring sequence");
   ck(d.dst_o==0x100000+(src.size()%pages)*4096,"destination ring sequence");
   ck(d.issue_o&&d.issue_id_o==src.size()+1,"per-task issue event");
   src.push_back(d.src_o);due=cycle+7;
  }
  if(d.complete_i){ck(d.done_o&&d.done_id_o==completions+1,"per-task retirement");completions++;due=0;}
  d.clk_i=1;d.eval();cycle++;eval();
 }
 Bench(){d.rst_ni=0;d.valid_i=d.write_i=0;tick();d.rst_ni=1;tick();}
 uint64_t access(unsigned addr,bool wr=false,uint64_t val=0){
  d.addr_i=addr;d.write_i=wr;d.wdata_i=val;d.valid_i=1;eval();
  ck(d.ready_o,"MMIO ready");auto r=d.rdata_o;tick();d.valid_i=0;eval();return r;
 }
 void configure(unsigned n,unsigned w){pages=w;access(0,1,0x10000);access(8,1,0x100000);access(16,1,4096);access(24,1,0);access(0x88,1,0);access(0x90,1,n);access(0x98,1,w);access(0x88,1,1);}
 void drain(){for(unsigned i=0;i<10000&&d.batch_active_o;i++)tick();ck(!d.batch_active_o,"batch eventually idle");}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{unsigned cases=0;
 for(unsigned n:{1U,8U,64U})for(unsigned w:{1U,8U}){
  Bench b;ck(b.access(0x80)==0x49444d4142544301ULL,"batch bank magic");b.configure(n,w);
  auto id=b.access(0x28);ck(id==2,"one functional ID per batch");
  // Stall an offered task without letting STOP/configuration change its payload.
  b.stall=true;for(int i=0;i<5;i++){b.eval();ck(b.d.request_o,"valid persists under backpressure");b.tick();}b.stall=false;
  b.drain();ck(b.src.size()==n&&b.completions==n,"exact finite count");
  ck(b.access(0xa0)==n&&b.access(0xa8)==n,"hardware progress counters");
  ck(b.access(0x88)==9&&b.access(0x30)==id,"batch completion status and ID");cases++;
 }
 {Bench b;b.configure(64,8);b.access(0x28);while(b.src.size()<3)b.tick();b.access(0x88,1,2);b.drain();
  ck(b.src.size()>=3&&b.src.size()<=4&&b.src.size()==b.completions,"STOP drains offered task, no unlimited refill");
  ck((b.access(0x88)&7)==5,"stopped status");cases++;}
 {Bench b;b.configure(8,1);b.access(0x28);b.access(0,1,0x90000);b.drain();
  ck(b.access(0x88)>>32,"active configuration rejected");ck(b.access(0)==0x10000,"locked source unchanged");cases++;}
 {Bench b;b.configure(8,1);b.access(0x98,1,0);b.access(0x88,1,1);
  ck(b.access(0x88)>>32,"invalid configuration remains sticky");
  b.access(0x28);ck(!b.d.batch_active_o&&b.src.empty(),"invalid batch cannot start");cases++;}
 {Bench b;b.configure(64,8);b.access(0x28);b.stall=true;b.access(0x88,1,2);
  for(int i=0;i<8;i++){b.eval();ck(b.d.request_o&&b.d.src_o==0x10000,"STOP cannot withdraw stalled valid");b.tick();}
  b.stall=false;b.drain();ck(b.src.size()==1&&b.completions==1,"stalled STOP safe completion");cases++;}
 {Bench b;for(int batch=0;batch<2;batch++){
  b.src.clear();b.completions=0;b.configure(8,1);ck(b.access(0x88)==1,"clear previous count before re-arm");
  auto id=b.access(0x28);b.drain();ck(b.access(0xa0)==8&&b.access(0x30)==id,"consecutive same-size batches");
 }cases++;}
 std::cout<<"PASS "<<cases<<" production frontend batch scenarios\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
