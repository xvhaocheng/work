#include "Ve3_observer_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <initializer_list>
struct B {
 Ve3_observer_top d;
 void tick(){d.clk_i=0;d.eval();d.clk_i=1;d.eval();}
 B(){d.rst_ni=0;tick();d.rst_ni=1;tick();arm();}
 void arm(){d.active_i=0;d.arm_i=1;tick();d.arm_i=0;d.open_i=1;tick();d.open_i=0;d.active_i=1;}
 unsigned long long get(unsigned i){d.index_i=i;d.eval();return d.data_o;}
 void tr(unsigned lane){unsigned ch=lane%2;d.tr_id_i[ch]=lane<2?0:4;d.tr_fire_i=1<<ch;tick();d.tr_fire_i=0;}
 void select(unsigned lane){d.id_i=lane<2?0:4;d.write_i=lane%2;d.select_i=1;tick();d.select_i=0;}
 void req(){d.request_i=1;tick();d.request_i=0;}
 void lookup(bool hit){d.hit_i=hit;d.lookup_i=1;tick();d.lookup_i=0;}
 void complete(unsigned lane){unsigned ch=lane%2;d.response_i=1;tick();d.response_i=0;
  d.out_id_i[ch]=lane<2?0:4;d.out_valid_i=d.out_ready_i=1<<ch;tick();d.out_valid_i=d.out_ready_i=0;
  if(ch){d.wlast_i=1;tick();d.wlast_i=0;}
  d.return_id_i[ch]=lane<2?0:4;d.return_i=1<<ch;tick();d.return_i=0;}
 void close(){d.close_i=1;tick();d.close_i=0;d.active_i=0;assert(!d.error_o);}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);
 for(unsigned lane=0;lane<4;lane++){B b;for(int repeat=0;repeat<2;repeat++){
  if(repeat)b.arm();b.tr(lane);b.tick();b.select(lane);b.req();b.lookup(true);b.complete(lane);b.close();
  assert(b.get(17+lane)==1 && b.get(21+lane)==0);
  for(unsigned s=0;s<4;s++){unsigned k=96+lane*32+s*8;assert(b.get(k)==1 && b.get(k+1)==1 && !b.get(k+2));assert(b.get(k+3)==b.get(k+7));}
  auto frozen=b.get(16);b.d.lookup_i=1;b.tick();b.d.lookup_i=0;assert(b.get(16)==frozen && !b.d.error_o);
 }}
 for(bool hit:{false,true}) for(bool delay:{false,true}) {B b;b.d.delay128_i=delay;b.arm();
  b.tr(3);b.select(3);b.req();b.lookup(false);b.tr(0);b.tick();
  b.d.capture_i=1;b.tick();b.d.capture_i=0;
  for(unsigned i=0;i<(delay?128U:0U);i++)b.tick();
  b.d.available_i=b.d.consume_i=1;b.d.extra_i=delay?128:0;b.tick();b.d.available_i=b.d.consume_i=0;
  b.complete(3);b.select(0);b.req();b.lookup(hit);b.complete(0);b.close();
  assert(b.get(2)==1 && b.get(5)-b.get(4)==1+(delay?128:0) && b.get(7)==(delay?128:0));
  assert(b.get(36)==127 && b.get(68)==127 && b.get(44)==0 && b.get(76)==hit);
  assert(b.get(34)==11 && b.get(66)==10 && b.get(12)==0 && b.get(13)==3);
  assert(b.get(32)==1 && b.get(64)==1 && b.get(240)==1 && b.get(242)==1);
 }
 for(unsigned lane=0;lane<4;lane++) {B b;
  for(int i=0;i<4;i++)b.tr(lane);
  for(int i=0;i<4;i++){b.select(lane);b.req();b.lookup(true);b.complete(lane);}
  b.close();assert(b.get(17+lane)==4);
  for(unsigned s=0;s<4;s++)assert(b.get(96+lane*32+s*8+1)==4);
 }
 {B b;b.tr(3);b.select(3);b.req();b.lookup(false);b.tr(0);
  b.d.capture_i=1;b.tick();b.d.capture_i=0;b.d.available_i=b.d.consume_i=1;b.tick();b.d.available_i=b.d.consume_i=0;
  b.complete(3);b.tr(3);b.select(3);b.req();b.lookup(true);b.complete(3);
  b.select(0);b.req();b.lookup(true);b.complete(0);b.close();
  assert(b.get(12)==1 && b.get(32)==1 && b.get(36)==127 && b.get(44)==0 && b.get(68)==127);
 }
 {B b;b.d.tr_id_i[0]=3;b.d.tr_fire_i=1;b.tick();assert(b.d.error_o);}
 {B b;for(int i=0;i<5;i++)b.tr(0);assert(b.d.error_o);}
 {B b;b.select(0);assert(b.d.error_o);}
 // A read remains the physical AR head while V waits behind it.
 for(bool delay:{false,true}) {B b;b.d.delay128_i=delay;b.arm();b.d.head_a_i=1;
  b.tr(2);b.select(2);b.req();b.lookup(false);b.tr(0);
  b.d.capture_i=1;b.tick();b.d.capture_i=0;
  for(unsigned i=0;i<(delay?128U:0U);i++)b.tick();
  b.d.available_i=b.d.consume_i=1;b.d.extra_i=delay?128:0;b.tick();b.d.available_i=b.d.consume_i=0;
  b.complete(2);b.select(0);b.req();b.lookup(true);b.complete(0);b.close();
  assert(b.get(0)==0x49444d4145330002ULL && b.get(2)==1 && b.get(31)==0);
  assert(b.get(25)==1 && b.get(26)==1 && b.get(27)==1 && b.get(28)==0 && b.get(29)==1);
  assert(b.get(5)-b.get(4)==1+(delay?128:0) && b.get(45)==0 && b.get(76)==1);
  assert(b.get(94)==((1ULL<<32)|(11ULL<<8)|4) && b.get(95)==0x1000);
 }
 // First two insertions retain identities and lookup outcome; third is explicit truncation.
 {B b;b.tr(3);b.select(3);b.req();b.lookup(false);b.tr(0);
  b.d.capture_i=1;b.tick();b.d.capture_i=0;b.d.available_i=b.d.consume_i=1;b.tick();b.d.available_i=b.d.consume_i=0;b.complete(3);
  for(unsigned k=0;k<3;k++){b.tr(3);b.select(3);b.req();b.lookup(k!=1);b.complete(3);}
  b.select(0);b.req();b.lookup(false);b.complete(0);b.close();
  assert(b.get(12)==3 && b.get(62)==1 && b.get(63)==2);
  assert(b.get(46)==2 && b.get(47)==4 && b.get(48)==11 && b.get(49)==0x1000 && b.get(50)==1 && b.get(53)==1);
  assert(b.get(54)==3 && b.get(55)==4 && b.get(56)==11 && b.get(58)==1 && b.get(61)==0);
  assert(b.get(51)<=b.get(52) && b.get(59)<=b.get(60));
 }
 // Candidate gates retain no-V and non-A opportunities, including after shot use.
 {B b;b.d.ptw_r_i=1;b.tick();b.d.a_miss_i=1;b.tick();b.d.ptw_r_i=b.d.a_miss_i=0;b.close();
  assert(b.get(25)==2 && b.get(26)==1 && b.get(27)==0 && b.get(28)==1 && b.get(2)==0);}
 {B b;b.d.queue_bad_i=1;b.tick();assert(b.d.error_o);}
 {B b;b.d.arm_dirty_i=1;b.arm();assert(b.d.error_o);}
 puts("PASS E3 observer: four lanes, repeat/freeze, D0/D128 trace, retained V miss, invalid identity/queue");
}
