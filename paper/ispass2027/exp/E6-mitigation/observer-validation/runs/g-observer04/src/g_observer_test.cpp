#include "Vdma_observer.h"
#include <verilated.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>

static unsigned cases=0;static uint64_t checks=0,clocks=0;
static void ck(bool v,const char*m){checks++;if(!v)throw std::runtime_error(m);}
struct Bench {
 Vdma_observer d; uint64_t cycle=0,first[2]={},last[2]={};unsigned issued[2]={},done[2]={};
 Bench(){d.rst_ni=0;d.cfg_wstrb_i=255;d.batch_target_i[0]=d.batch_target_i[1]=4096;
  d.batch_gap_cycles_i[1]=1000;tick();d.rst_ni=1;tick();}
 void tick(){d.clk_i=0;d.eval();for(int r=0;r<2;r++){
   if(d.issue_i&(1<<r)){if(!issued[r])first[r]=cycle;issued[r]++;}
   if(d.done_i&(1<<r)){done[r]++;last[r]=cycle;}}
  d.clk_i=1;d.eval();cycle++;clocks++;}
 void write(unsigned a,uint64_t value){d.cfg_valid_i=d.cfg_write_i=1;d.cfg_addr_i=a;d.cfg_wdata_i=value;tick();d.cfg_valid_i=d.cfg_write_i=0;}
 uint64_t reg(unsigned a){d.cfg_addr_i=a;d.eval();return d.cfg_rdata_o;}
 uint64_t get(unsigned i){write(0x70,768+i);return reg(0x78);}
 void arm(bool pair=true){write(0x60,4);write(0x68,4096);write(0x60,1|(1<<25)|(1<<26)|(1<<16)|(pair?1<<24:0));
  ck(reg(0x60)==1,"real G ARM accepted");d.batch_active_i=pair?3:1;}
 void issue(int r,unsigned id){d.issue_id_i[r]=id;d.issue_i=1<<r;tick();d.issue_i=0;}
 void finish(int r,unsigned id){d.done_id_i[r]=id;d.done_i=1<<r;tick();d.done_i=0;}
 void task(int r,unsigned id){issue(r,id);finish(r,id);}
 void many(int r,unsigned lo=1,unsigned hi=4096){for(unsigned id=lo;id<=hi;id++)task(r,id);}
 void poll(){d.poll_i=1;d.poll_id_i[0]=4096;tick();d.poll_i=0;}
 void drop(int r){d.batch_active_i &= ~(1<<r);}
 void axi_path(unsigned ch){
  if(ch)d.aw_valid_i=d.aw_ready_i=1;else d.ar_valid_i=d.ar_ready_i=1;tick();d.aw_valid_i=d.aw_ready_i=d.ar_valid_i=d.ar_ready_i=0;
  d.tr_id_i[ch]=0;if(ch)d.shared_aw_valid_i=d.shared_aw_ready_i=1;else d.shared_ar_valid_i=d.shared_ar_ready_i=1;
  tick();d.shared_aw_valid_i=d.shared_aw_ready_i=d.shared_ar_valid_i=d.shared_ar_ready_i=0;
  d.out_id_i[ch]=0;d.out_valid_i=1<<ch;tick();d.out_ready_i=1<<ch;tick();d.out_valid_i=d.out_ready_i=0;
  d.return_id_i[ch]=0;d.return_i=1<<ch;if(ch)d.b_fire_i=1;else d.rlast_fire_i=1;tick();d.return_i=d.b_fire_i=d.rlast_fire_i=0;
 }
 void valid(bool pair=true){ck(reg(0x60)==4,"final old OBS4 status exact valid");ck(get(0)==0x49444d4150344d02ULL,"G independent capability");
  ck(get(1)==0&&get(31)==0,"reserved0/absence of hardware seq");ck(get(3)==0,"G errors0");
  ck(get(4)==4096&&get(8)==4096&&get(9)==4096,"V exact full budget");ck(get(10)==last[0]-first[0],"V own elapsed exact");
  ck(get(16)==get(10),"V elapsed equals actual V window");
  if(pair){ck(get(5)==4096&&get(11)==4096&&get(12)==4096,"A exact natural full budget");
   ck(get(13)==last[1]-first[1],"A own elapsed exact independent of V");}
  else for(unsigned i:{5U,7U,11U,12U,13U,15U,17U,18U,19U,20U,21U,22U,23U,24U,25U,26U,29U,30U})ck(get(i)==0,"Solo A fields zero");
  ck(get(19)+get(17)==get(18)+get(20),"actual window conservation");
  ck(get(27)+get(10)==get(28),"V timestamps/elapsed");
  if(pair)ck(get(29)+get(13)==get(30),"A timestamps/elapsed");
  for(unsigned lane=0;lane<4;lane++)for(unsigned stage=0;stage<4;stage++){
   write(0x70,64+lane*32+stage*8+1);auto full=reg(0x78);uint64_t sum=0;
   for(unsigned bin=0;bin<32;bin++){write(0x70,256+lane*128+stage*32+bin);sum+=reg(0x78);}ck(sum==full,"actual 512 histogram/bin full conservation");}
 }
 void report(const char*n){std::cout<<"CASE "<<n<<" clocks="<<cycle<<" V="<<done[0]<<" A="<<done[1]<<" Gerror="<<get(3)<<"\n";cases++;}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
 {Bench b;b.arm(false);b.issue(0,1);b.axi_path(0);b.axi_path(1);b.finish(0,1);b.many(0,2);b.drop(0);b.poll();b.valid(false);
  for(unsigned l=0;l<2;l++)for(unsigned s=0;s<4;s++){b.write(0x70,64+l*32+s*8+1);ck(b.reg(0x78)==1,"G-on real PATH full samples");}b.report("Solo-nonzero-PATH-hist");}
 {Bench b; // Actual warmup IDs need not start at 1; reset only measurement bookkeeping.
  b.issue(0,321);b.finish(0,321);b.issue(1,987);b.finish(1,987);
  b.issued[0]=b.issued[1]=b.done[0]=b.done[1]=0;
  b.arm();b.many(1);b.drop(1);b.many(0);b.drop(0);b.poll();b.valid();
  ck(b.get(17)==0&&b.get(18)==0,"A done before V has zero window work");b.report("A-before-V-arbitrary-warmup-IDs");}
 {Bench b;b.arm();b.issue(0,1);b.task(1,1);
  b.d.batch_gap_active_i=2;b.tick();b.tick();b.d.batch_gap_active_i=0;
  b.many(1,2);b.drop(1);b.finish(0,1);b.many(0,2);b.drop(0);b.poll();b.valid();
  ck(b.get(14)==0&&b.get(15)==2,"actual GAP active tap counts per-lane whole occupancy");
  ck(b.get(18)==4096,"A naturally finishes during V");b.report("A-during-V");}
 {Bench b;b.arm();b.many(0);b.drop(0);b.poll();
  ck(b.reg(0x60)&(1ULL<<38),"premature G finish invalid while A live");
  b.many(1);b.drop(1);b.valid();ck(b.get(18)==0,"A after V does not extend V window");b.report("A-after-V-normal-poll-not-abort");}
 {Bench b;b.arm();b.many(0);b.drop(0);b.poll();auto old=b.get(8),elapsed=b.get(10);
  b.write(0x60,4);ck(b.get(8)==old&&b.get(10)==elapsed,"whole busy clear cannot erase V frozen data");
  b.write(0x60,1|(1<<25)|(1<<26)|(1<<24)|(1<<16));
  ck(b.get(8)==4096&&(b.get(3)&256),"whole busy reARM rejected without phase/data change");
  b.many(1);b.drop(1);ck(b.get(12)==4096,"rejected controls never stop natural A work");
  ck(b.reg(0x60)&(1ULL<<38),"local G error gates final OBS4 valid");b.report("V-closed-A-live-controls-rejected");}
 {Bench b;b.arm();b.issue(1,1);b.issue(0,1);b.finish(1,1);b.many(1,2);b.drop(1);
  b.finish(0,1);b.many(0,2);b.drop(0);b.poll();b.valid();
  ck(b.get(19)==1&&b.get(21)==1&&b.get(22)==0&&b.get(23)==1,"strict carried-in cohort ID");b.report("strict-carry-in");}
 {Bench b;b.arm();b.issue(1,1);
  b.d.done_i=2;b.d.done_id_i[1]=1;b.d.issue_i=3;b.d.issue_id_i[0]=1;b.d.issue_id_i[1]=2;b.tick();b.d.done_i=b.d.issue_i=0;
  b.finish(1,2);b.many(1,3);b.drop(1);b.finish(0,1);b.many(0,2);b.drop(0);b.poll();b.valid();
  ck(b.get(19)==1&&b.get(21)==0&&b.get(22)==1&&b.get(23)==2,"open-edge replacement separate from strict carry");
  ck(b.get(17)==4094&&b.get(18)==4095,"opening edge issue/done excluded exactly");b.report("open-done-issue-handoff");}
 {Bench b;b.arm();b.issue(0,1);b.many(1,1,4095);b.finish(0,1);b.many(0,2,4095);b.issue(0,4096);
  b.d.done_i=1;b.d.done_id_i[0]=4096;b.d.issue_i=2;b.d.issue_id_i[1]=4096;b.tick();b.d.done_i=b.d.issue_i=0;b.drop(0);b.poll();
  ck(b.get(20)==1&&b.get(24)==4096,"closing-edge A offer is true carry-out");
  auto inside=b.get(18);b.finish(1,4096);b.drop(1);b.valid();ck(b.get(18)==inside,"post-close A completion excluded from window");b.report("close-edge-issue-carry-out");}
 {Bench b;b.arm();b.issue(0,1);b.issue(1,1);
  // Real observer AXI inventory still consumes request/return across abort.
  b.d.ar_valid_i=b.d.ar_ready_i=2;b.tick();b.d.ar_valid_i=b.d.ar_ready_i=0;
  auto v=b.get(8),a=b.get(11);b.d.done_i=2;b.d.done_id_i[1]=1;b.d.issue_i=2;b.d.issue_id_i[1]=2;
  b.write(0x60,2);b.d.done_i=b.d.issue_i=0;
  ck(b.get(8)==v&&b.get(11)==a&&(b.get(3)&128),"abort excludes its edge and freezes invalid measurement");
  b.finish(1,2);b.finish(0,1);b.d.rlast_fire_i=2;b.tick();b.d.rlast_fire_i=0;b.d.batch_active_i=0;
  b.write(0x60,4);ck(b.get(3)==0&&b.get(8)==0,"post-abort live pairing and AXI drain permit real clear");
  b.arm();ck(b.reg(0x60)==1,"reARM after true drain allowed");b.report("abort-edge-handoff-late-return-drain");}
 {Bench b;b.d.batch_target_i[1]=17;b.write(0x68,4096);b.write(0x60,1|(1<<25)|(1<<26)|(1<<24)|(1<<16));
  ck(b.get(3)&1,"actual target mismatch rejected at ARM");ck(b.get(8)==0&&!(b.reg(0x60)&3),"invalid ARM no phase or counts mutation");b.report("actual-target-preflight");}
 {Bench b;b.arm();ck(b.get(7)==1000,"actual gap tap latched");b.d.batch_gap_cycles_i[1]=2000;b.tick();
  ck((b.get(3)&512)&&b.get(7)==1000,"live config mismatch error preserves ARM snapshot");b.report("actual-gap-config-change");}
 {Bench b;b.arm();b.issue(0,2);ck(b.get(3)&2,"measurement ID must start at1");ck(b.reg(0x60)&(1ULL<<38),"local ID error participates valid");b.report("measured-ID-order");}
 {Bench b;b.arm();b.finish(1,1);ck(b.get(3)&4,"done without matching pending rejected");b.report("done-order");}
 {Bench b;b.arm(false);b.issue(0,1);b.drop(0);b.tick();ck(b.get(3)&8,"started unfinished batch inactive is invalid");b.report("finite-budget-not-natural-done");}
 // Verification-only register corruption exercises real 64-bit boundary guards;
 // it is not legal traffic or a claim of running 2^64 cycles. Production RTL has no test hook.
 {Bench b;b.arm(false);b.issue(0,1);
  b.d.dma_observer__DOT__gen_gap__DOT__i_gap__DOT__tick_q=~0ULL;b.tick();
  ck((b.get(3)&16)&&b.d.dma_observer__DOT__gen_gap__DOT__i_gap__DOT__tick_q==~0ULL,"64bit actual timer saturates and sticks error");
  ck(b.reg(0x60)&(1ULL<<38),"64bit overflow gates final valid");b.report("injected-timer-saturation");}
 {Bench b;b.arm(false);b.d.dma_observer__DOT__gen_gap__DOT__i_gap__DOT__words_q[8]=~0ULL;b.issue(0,1);
  ck((b.get(3)&16)&&b.get(8)==~0ULL,"64bit actual task counter saturates without wrap");b.report("injected-counter-saturation");}
 {Bench b;b.arm(false);b.many(0,1,4095);b.issue(0,4096);
  b.d.dma_observer__DOT__gen_gap__DOT__i_gap__DOT__words_q[16]=0;b.finish(0,4096);b.drop(0);b.poll();
  ck(b.get(3)&32,"window cycles binding error detected");b.report("injected-window-binding");}
 {Bench b;b.arm(false);b.many(0,1,4095);b.issue(0,4096);
  b.d.dma_observer__DOT__gen_gap__DOT__i_gap__DOT__words_q[19]=2;b.finish(0,4096);b.drop(0);b.poll();
  ck(b.get(3)&64,"window conservation impossible stock detected");b.report("injected-window-balance");}
 std::cout<<"PASS G observer cases="<<cases<<" checks="<<checks<<" clocks="<<clocks<<"; actual observer, synthetic task/AXI event stimulus; no board\n";return 0;
 }catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
