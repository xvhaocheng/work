#include "Vdma_e3_write_observer.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
struct B{
 Vdma_e3_write_observer d;
 void tick(){d.clk_i=0;d.eval();d.clk_i=1;d.eval();d.time_i++;}
 B(){d.rst_ni=0;tick();d.rst_ni=1;d.run_i=1;d.open_i=1;tick();d.open_i=0;d.active_i=1;}
 void event(bool aw,bool w,bool b,unsigned ai=0,unsigned bi=0){d.aw_i=aw;d.wlast_i=w;d.b_i=b;d.aw_id_i=ai;d.b_id_i=bi;tick();d.aw_i=d.wlast_i=d.b_i=0;}
 void close(){d.close_i=1;tick();d.close_i=0;d.active_i=0;assert(!d.error_o);}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);
 {B b;b.event(1,0,0);b.tick();b.event(0,1,0);b.tick();b.event(0,0,1);b.close();
  assert(b.d.data_o[2]==1 && b.d.data_o[5]==2 && b.d.data_o[7]==2 && b.d.data_o[9]==4);}
 {B b;b.event(0,1,0);b.tick();b.event(1,0,0);b.event(0,0,1);b.close();assert(b.d.data_o[4]==1 && b.d.data_o[7]==3 && b.d.data_o[9]==1);}
 {B b;for(int i=0;i<20;i++)b.event(1,1,1);b.close();assert(b.d.data_o[2]==20 && b.d.data_o[9]==0);}
 {B b;b.event(1,1,0,0);b.event(1,1,0,4);b.event(0,0,1,0,4);b.event(0,0,1,0,0);b.close();assert(b.d.data_o[2]==1 && b.d.data_o[18]==1);}
 {B b;b.d.active_i=0;b.event(1,1,0,4);b.d.open_i=1;b.tick();b.d.open_i=0;b.d.active_i=1;b.event(0,0,1,0,4);b.close();assert(b.d.data_o[29]==1 && b.d.data_o[19]==1);}
 {B b;b.event(1,0,0);b.close();assert(b.d.data_o[11]==1);}
 {B b;b.event(0,0,1);assert(b.d.error_o);}
 {B b;b.event(1,0,1);assert(b.d.error_o);}
 {B b;for(int i=0;i<5;i++)b.event(1,1,0);assert(b.d.error_o);}
 {B b;b.event(1,1,0,3);assert(b.d.error_o);}
 puts("PASS COMP writes: AW/W order, simultaneous, wrap, reordered B IDs, carry, unfinished, errors");
}
