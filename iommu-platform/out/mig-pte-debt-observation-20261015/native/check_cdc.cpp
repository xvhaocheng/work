#include "Vmig_capture_cdc.h"
#include "verilated.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
static Vmig_capture_cdc d;
static unsigned tick=0, reads=0, checks=0;
static bool memory_pending=false;
static unsigned memory_index=0;
static uint64_t pattern(unsigned i) { return 0xcafe000012340000ULL | i; }
static void check(bool v,const char *s) { ++checks; if(!v){std::cerr<<"FAIL "<<s<<"\n";std::exit(1);} }
static void step(unsigned n=1) {
  for(unsigned k=0;k<n;k++,tick++) {
    bool src=(tick%8)==4, dst=(tick%2)==1;
    d.src_clk_i=src; d.dst_clk_i=dst;
    bool req=dst&&d.rd_req_o; unsigned idx=d.rd_index_o;
    d.eval();
    if(dst) {
      d.rd_valid_i=d.dst_rst_ni&&memory_pending;
      if(d.rd_valid_i)d.rd_data_i=pattern(memory_index);
      memory_pending=d.dst_rst_ni&&req;
      if(req){memory_index=idx;++reads;}
      d.eval();
    }
  }
}
static void srcpulse(bool arm,bool open,bool close,bool inv) {
  d.arm_i=arm;d.open_i=open;d.close_i=close;d.invalidate_i=inv;step(8);
  d.arm_i=0;d.open_i=0;d.close_i=0;d.invalidate_i=0;step(8);
}
static void query(unsigned i) {
  d.index_i=0xf00;srcpulse(false,false,false,true);
  d.index_i=0x1000+i;step(8);d.index_i=0xf00;
  for(unsigned k=0;k<200&&(d.status_o&3)!=1;k++)step(1);
  check((d.status_o&3)==1,"mailbox acknowledged without data race");
  check(((d.status_o>>16)&0xfff)==i,"acknowledged index matches request");
  d.index_i=0x1000+i;step(16);
  check(d.data_o==pattern(i),"stable bundled result returned intact");
}
int main(int argc,char**argv) {
  Verilated::commandArgs(argc,argv);
  d.src_rst_ni=0;d.dst_rst_ni=0;d.index_i=0;d.rd_valid_i=0;step(20);
  d.src_rst_ni=1;d.dst_rst_ni=1;step(20);
  check((d.status_o>>32)==0x4d434301,"cache identity");
  query(0);unsigned before=reads;step(80);check(reads==before,"cached index does not issue duplicate reads");
  query(1);query(0xdff);query(0x9ff);query(0x200);query(0x201);query(0);
  srcpulse(true,false,false,false);step(10);
  srcpulse(false,true,false,false);step(10);check(d.window_o,"window level crosses into UI clock");
  srcpulse(false,false,true,false);step(10);check(!d.window_o,"close crosses without stale open");
  d.index_i=0;d.src_rst_ni=0;d.dst_rst_ni=0;step(20);d.src_rst_ni=1;d.dst_rst_ni=1;step(20);
  check((d.status_o&3)==0,"common reset invalidates cached data");query(5);
  std::cout<<"PASS "<<checks<<" asynchronous mailbox checks; "<<reads<<" native requests\n";
}
