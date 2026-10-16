#include "Vcq_receipt_top.h"
#include "verilated.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <tuple>

constexpr uint64_t RING=0x10000, MARKER=0x18000;
constexpr uint32_t TOKEN=0x89abcdef;
struct Command { uint64_t lo=2, hi=0; };
struct Read { bool pending=false; unsigned beat=0; uint64_t available=0, addr=0; Command command; };
class Simulation {
public:
 Vcq_receipt_top t;
 std::array<Command,8> ring;
 Read read;
 std::string scenario;
 uint64_t cycle=0, marker=~uint64_t(TOKEN), awaddr=0, b_due=0;
 bool aw_pending=false, b_pending=false, bundle=false;
 unsigned start=0,end=5;
 bool ar_stalled=false,aw_stalled=false,w_stalled=false,r_stalled=false,b_stalled=false;
 std::tuple<uint64_t,unsigned,unsigned,unsigned,unsigned> ar_old, aw_old;
 std::tuple<uint64_t,unsigned,unsigned> w_old;
 std::tuple<uint64_t,unsigned,unsigned> r_old;
 unsigned b_old=0;
 unsigned accepted=0, aw_offers=0, w_offers=0;
 explicit Simulation(std::string s):scenario(s) {
  t.clk_i=0;t.rst_ni=0;t.enable_i=0;t.tail_i=0;
  t.ar_ready_i=t.r_valid_i=t.r_last_i=0;t.r_data_i=t.r_resp_i=0;
  t.aw_ready_i=t.w_ready_i=t.b_valid_i=t.b_resp_i=0;
  t.eval();t.clk_i=1;t.eval();t.clk_i=0;t.eval();t.rst_ni=1;t.enable_i=1;
 }
 void tick() {
  t.clk_i=0;
  t.ar_ready_i=cycle%4>=2;
  t.aw_ready_i=aw_offers>=2;
  t.w_ready_i=w_offers>=2;
  t.r_valid_i=read.pending && cycle>=read.available;
  t.r_data_i=read.beat?read.command.hi:read.command.lo;
  t.r_last_i=read.beat==1;
  t.r_resp_i=0;
  t.b_valid_i=b_pending && cycle>=b_due;
  t.b_resp_i=scenario=="b-error"?2:0;
  t.eval();
  const auto ar=std::make_tuple(uint64_t(t.ar_addr_o),unsigned(t.ar_id_o),unsigned(t.ar_len_o),unsigned(t.ar_size_o),unsigned(t.ar_burst_o));
  const auto aw=std::make_tuple(uint64_t(t.aw_addr_o),unsigned(t.aw_id_o),unsigned(t.aw_len_o),unsigned(t.aw_size_o),unsigned(t.aw_burst_o));
  const auto w=std::make_tuple(uint64_t(t.w_data_o),unsigned(t.w_strb_o),unsigned(t.w_last_o));
  const auto r=std::make_tuple(uint64_t(t.r_data_i),unsigned(t.r_last_i),unsigned(t.r_resp_i));
  if(ar_stalled) assert(t.ar_valid_o && ar==ar_old);
  if(aw_stalled) assert(t.aw_valid_o && aw==aw_old);
  if(w_stalled) assert(t.w_valid_o && w==w_old);
  if(r_stalled) assert(t.r_valid_i && r==r_old);
  if(b_stalled) assert(t.b_valid_i && unsigned(t.b_resp_i)==b_old);
  ar_stalled=t.ar_valid_o&&!t.ar_ready_i;ar_old=ar;
  aw_stalled=t.aw_valid_o&&!t.aw_ready_i;aw_old=aw;
  w_stalled=t.w_valid_o&&!t.w_ready_i;w_old=w;
  r_stalled=t.r_valid_i&&!t.r_ready_o;r_old=r;
  b_stalled=t.b_valid_i&&!t.b_ready_o;b_old=t.b_resp_i;
  bool ar_fire=t.ar_valid_o&&t.ar_ready_i, r_fire=t.r_valid_i&&t.r_ready_o;
  bool aw_fire=t.aw_valid_o&&t.aw_ready_i,w_fire=t.w_valid_o&&t.w_ready_i,b_fire=t.b_valid_i&&t.b_ready_o;
  auto head_before=t.head_o;
  const uint64_t marker_before=marker;
  // Capture every public interface value BEFORE the edge, then latch CQ CSRs.
  std::printf("{\"cycle\":%llu,\"phase\":\"%s\",\"head_before\":%u,\"tail\":%u,"
   "\"ar_valid\":%u,\"ar_ready\":%u,\"ar_addr\":%llu,\"ar_id\":%u,\"ar_len\":%u,\"ar_size\":%u,\"ar_burst\":%u,"
   "\"r_valid\":%u,\"r_ready\":%u,\"r_data\":%llu,\"r_last\":%u,\"r_resp\":%u,"
   "\"aw_valid\":%u,\"aw_ready\":%u,\"aw_addr\":%llu,\"aw_id\":%u,\"aw_len\":%u,\"aw_size\":%u,\"aw_burst\":%u,"
   "\"w_valid\":%u,\"w_ready\":%u,\"w_data\":%llu,\"w_strb\":%u,\"w_last\":%u,\"b_valid\":%u,\"b_ready\":%u,\"b_resp\":%u,"
   "\"inval_ddt\":%u,\"inval_pdt\":%u,\"dv\":%u,\"did\":%u,\"pid\":%u,"
   "\"inval_vma\":%u,\"inval_gvma\":%u,\"av\":%u,\"gv\":%u,\"pscv\":%u,\"vpn\":%u,\"pscid\":%u,\"gscid\":%u,\"marker_before\":%llu,",
   (unsigned long long)cycle,bundle?"bundle":"prelude",head_before,t.tail_i,
   t.ar_valid_o,t.ar_ready_i,(unsigned long long)t.ar_addr_o,t.ar_id_o,t.ar_len_o,t.ar_size_o,t.ar_burst_o,
   t.r_valid_i,t.r_ready_o,(unsigned long long)t.r_data_i,t.r_last_i,t.r_resp_i,
   t.aw_valid_o,t.aw_ready_i,(unsigned long long)t.aw_addr_o,t.aw_id_o,t.aw_len_o,t.aw_size_o,t.aw_burst_o,
   t.w_valid_o,t.w_ready_i,(unsigned long long)t.w_data_o,t.w_strb_o,t.w_last_o,t.b_valid_i,t.b_ready_o,t.b_resp_i,
   t.inv_ddt_o,t.inv_pdt_o,t.inv_dv_o,t.inv_did_o,t.inv_pid_o,
   t.inv_vma_o,t.inv_gvma_o,t.inv_av_o,t.inv_gv_o,t.inv_pscv_o,t.inv_vpn_o,t.inv_pscid_o,t.inv_gscid_o,
   (unsigned long long)marker_before);
  if(ar_fire) {
   assert(!read.pending);assert(t.ar_id_o==3&&t.ar_len_o==1&&t.ar_size_o==3&&t.ar_burst_o==1);
   assert(t.ar_addr_o>=RING && t.ar_addr_o<RING+128 && !(t.ar_addr_o%16));
   unsigned index=(t.ar_addr_o-RING)/16;
   read.pending=true;read.beat=0;read.addr=t.ar_addr_o;read.command=ring[index];read.available=cycle+2;
  }
  if(r_fire) {
   assert(read.pending);
   if(read.beat==1) read.pending=false;
   else {read.beat=1;read.available=cycle+(bundle && read.addr==RING+((start+4)&7)*16?7:1);}
  }
  if(t.aw_valid_o) aw_offers++;
  if(t.w_valid_o) w_offers++;
  if(aw_fire) {aw_offers=0;assert(!aw_pending&&!b_pending);assert(t.aw_addr_o==MARKER&&t.aw_id_o==0&&t.aw_len_o==0&&t.aw_size_o==3&&t.aw_burst_o==1);aw_pending=true;awaddr=t.aw_addr_o;}
  if(w_fire) {w_offers=0;assert(aw_pending&&awaddr==MARKER&&t.w_strb_o==255&&t.w_last_o);marker=t.w_data_o;aw_pending=false;b_pending=true;b_due=cycle+8;}
  if(b_fire) {assert(b_pending);b_pending=false;}
  t.clk_i=1;t.eval();
  bool receipt=bundle && marker==TOKEN && t.head_o==end && t.on_o && !t.mf_o && !t.ill_o && !t.to_o;
  if(receipt) accepted++;
  std::printf("\"head_after\":%u,\"marker_after\":%llu,\"cq_on\":%u,\"mf\":%u,\"ill\":%u,\"to\":%u,\"receipt\":%s}\n",
    t.head_o,(unsigned long long)marker,t.on_o,t.mf_o,t.ill_o,t.to_o,receipt?"true":"false");
  cycle++;
 }
 void run() {
  tick();tick();
  if(scenario=="wrap") {
   t.tail_i=6;
   while(t.head_o!=6 && cycle<200) tick();
   assert(t.head_o==6);tick();tick();assert(!read.pending);
   start=6;end=3;
  }
  bundle=true;
  const Command commands[5]={{3ULL|(1ULL<<33)|(10ULL<<40),0},{3ULL|(1ULL<<33)|(11ULL<<40),0},{1,0},
    {scenario=="head-only"?2ULL:(2ULL|(1ULL<<10)|(uint64_t(TOKEN)<<32)),scenario=="head-only"?0:MARKER>>2},{2,0}};
  for(unsigned i=0;i<5;i++) ring[(start+i)&7]=commands[i];
  t.tail_i=end;
  unsigned quiet=0;
  while(cycle<400) {
   tick();
   if((t.head_o==end&&!read.pending&&!b_pending)||(t.mf_o)) quiet++;else quiet=0;
   if(quiet>=10) break;
  }
  assert(cycle<400);
  if(scenario=="b-error") assert(t.mf_o&&t.head_o==((start+4)&7)&&!accepted);
  else if(scenario=="head-only") assert(t.head_o==end&&marker!=TOKEN&&!accepted);
  else assert(t.head_o==end&&marker==TOKEN&&accepted);
 }
};
int main(int argc,char **argv) {
 Verilated::commandArgs(argc,argv);
 if(argc!=2) return 2;
 Simulation simulation(argv[1]);simulation.run();return 0;
}
