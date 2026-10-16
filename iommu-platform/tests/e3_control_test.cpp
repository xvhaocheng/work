#include "Vdma_observer.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <initializer_list>
struct B{
 Vdma_observer d;
 void tick(){d.clk_i=0;d.eval();d.clk_i=1;d.eval();}
 B(){d.rst_ni=0;tick();d.rst_ni=1;tick();}
 void write(unsigned a,unsigned long long v){d.cfg_valid_i=d.cfg_write_i=1;d.cfg_wstrb_i=255;d.cfg_addr_i=a;d.cfg_wdata_i=v;tick();d.cfg_valid_i=d.cfg_write_i=0;}
 unsigned long long read(unsigned a){d.cfg_addr_i=a;d.eval();return d.cfg_rdata_o;}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);unsigned cmd=1|(1<<25)|(1<<26)|(1<<27)|(1<<24)|(1<<16);
 {B b;assert(b.read(0x58)==0x49444d414f425305ULL);b.write(0x68,1);b.write(0x60,cmd|(1<<28));assert(b.read(0x60)==1 && (b.d.e3_control_o&129)==129);
  b.d.e3_data_i=123;for(unsigned i=192;i<448;i++){b.write(0x70,i);assert(b.d.e3_index_o==i-192 && b.read(0x78)==123);}
  b.d.batch_active_i=2;b.d.issue_i=1;b.d.issue_id_i[0]=1;b.tick();b.d.issue_i=0;assert(b.d.e3_control_o&8);
  b.d.done_i=1;b.d.done_id_i[0]=1;b.tick();b.d.done_i=0;assert(!(b.d.e3_control_o&8));
  b.d.poll_i=1;b.d.poll_id_i[0]=1;b.tick();b.d.poll_i=0;assert(b.read(0x60)==4);
  b.d.e3_error_i=1;assert(b.read(0x60)&(1ULL<<39));b.d.e3_error_i=0;b.write(0x60,4);assert(b.read(0x60)==0 && !(b.d.e3_control_o&1));}
 for(unsigned invalid:{cmd|(1<<8),cmd^(1<<26),cmd^(1<<27)^(1<<28),cmd^(3<<16)}){
  B b;b.write(0x68,1);b.write(0x60,invalid);assert(b.read(0x60)>>32);}
 puts("PASS v5 MMIO: identity, bank range, mode, window, sticky error, clear, illegal controls");
}
