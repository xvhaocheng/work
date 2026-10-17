// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vds_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <deque>
#include <vector>
static Vds_top t;
static FILE *trace,*cases;
static unsigned cycles=0,evals=0,checks=0,ncases=0;
static const char* phase="reset";
struct Debt {unsigned r=0,w=0;bool bad=false;};
static Debt unit,ds;
static unsigned arfires=0,awfires=0,wfires=0,rlasts=0,bfires=0,fills=0;
static void ck(bool ok,const char*s){checks++;if(!ok){fprintf(stderr,"FAIL cycle=%u eval=%u phase=%s %s\n",cycles,evals,phase,s);exit(1);}}
static void update_one(unsigned& n,bool inc,bool dec,bool&bad){if(inc==dec)return;if(inc){if(n==255)bad=true;else n++;}else{if(n==0)bad=true;else n--;}}
static void update(Debt&d,bool ri,bool rl,bool wi,bool wd){update_one(d.r,ri,rl,d.bad);update_one(d.w,wi,wd,d.bad);}
static void ev(){
 t.eval();evals++;
 if(!t.rst_ni){unit={};ds={};}
 ck(t.equal_on_o,"enabled DS differs byte-for-byte from original5b DS");
 ck(t.equal_off_o,"default-off DS differs byte-for-byte from original5b DS");
 ck(t.off_quiet_o==0,"default-off optional output not conservative0");
 ck(t.read_debt_o==ds.r&&t.write_debt_o==ds.w&&t.bad_o==ds.bad,"DS debt differs independent handshake scoreboard");
 ck(t.unit_read_o==unit.r&&t.unit_write_o==unit.w&&t.unit_bad_o==unit.bad,"standalone debt differs independent scoreboard");
 ck(bool(t.unit_quiet_o)==(!unit.bad&&!unit.r&&!unit.w),"standalone quiet differs debt predicate");
 ck(bool(t.transport_quiet_o)==(bool(t.buffers_quiet_o)&&!ds.bad&&!ds.r&&!ds.w),"DS transport quiet differs buffers+scoreboard");
 fprintf(trace,"%u,%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
 cycles,evals,phase,t.clk_i,t.rst_ni,t.equal_on_o,t.equal_off_o,t.buffers_quiet_o,t.transport_quiet_o,
 t.read_debt_o,t.write_debt_o,t.bad_o,t.owner_empty_o,t.ar_spill_o,t.aw_spill_o,t.w_spill_o,t.r_spill_o,t.b_spill_o,
 t.ds_ar_valid_o,t.ds_ar_ready_i,t.ds_aw_valid_o,t.ds_aw_ready_i,t.ds_w_valid_o,t.ds_w_ready_i,t.ds_w_last_o,
 t.ds_r_valid_i,t.ds_r_ready_o,t.ds_r_last_i,t.ds_b_valid_i,t.ds_b_ready_o,t.unit_read_o,t.unit_write_o,t.unit_bad_o,
 t.msi_mode_i,t.msi_idle_o,t.msi_fill_o,t.msi_error_o);
}
static void tick(){
 t.clk_i=0;ev();
 if(t.rst_ni){
  bool ar=t.ds_ar_valid_o&&t.ds_ar_ready_i,aw=t.ds_aw_valid_o&&t.ds_aw_ready_i;
  bool rl=t.ds_r_valid_i&&t.ds_r_ready_o&&t.ds_r_last_i,br=t.ds_b_valid_i&&t.ds_b_ready_o;
  update(ds,ar,rl,aw,br);update(unit,t.unit_read_issue_i,t.unit_read_last_i,t.unit_write_issue_i,t.unit_write_done_i);
  arfires+=ar;awfires+=aw;wfires+=t.ds_w_valid_o&&t.ds_w_ready_i;rlasts+=rl;bfires+=br;fills+=t.msi_fill_o;
 }
 t.clk_i=1;ev();t.clk_i=0;ev();cycles++;
}
static void mark(const char*s){phase=s;ncases++;fprintf(cases,"%u,%s,%u\n",ncases,s,cycles);}
static void clear(){
 t.msi_mode_i=0;t.msi_init_i=0;t.ar_valid_i=0;t.aw_valid_i=0;t.w_valid_i=0;t.w_last_i=0x1ff;
 t.r_ready_i=t.b_ready_i=0x1ff;t.ds_ar_ready_i=t.ds_aw_ready_i=t.ds_w_ready_i=1;
 t.ds_r_valid_i=t.ds_b_valid_i=0;t.ds_r_id_i=t.ds_b_id_i=0;t.ds_r_data_i=0;t.ds_r_last_i=1;
 t.ds_r_resp_i=t.ds_b_resp_i=0;
 t.unit_read_issue_i=t.unit_read_last_i=t.unit_write_issue_i=t.unit_write_done_i=0;
 for(unsigned i=0;i<18;i++)t.ar_addr_i[i]=t.aw_addr_i[i]=t.w_data_i[i]=0;
 for(unsigned i=0;i<3;i++)t.ar_len_i[i]=t.aw_len_i[i]=0,t.w_strb_i[i]=0xffffffff;
}
static void reset(){clear();t.rst_ni=0;tick();tick();t.rst_ni=1;tick();ck(t.transport_quiet_o,"reset DS quiet missing");}
static void idle(unsigned n=4){for(unsigned i=0;i<n;i++)tick();}
static void set64(VlWide<18>&a,unsigned p,uint64_t v){a[2*p]=v;a[2*p+1]=v>>32;}
static void setbyte(VlWide<3>&a,unsigned p,unsigned v){unsigned shift=(p%4)*8;a[p/4]=(a[p/4]&~(255u<<shift))|((v&255)<<shift);}
static void offer_ar(unsigned p,unsigned len=0){
 t.ar_valid_i|=1u<<p;set64(t.ar_addr_i,p,0x10000000ull+(uint64_t(p)<<12)+arfires*8);setbyte(t.ar_len_i,p,len);
 for(unsigned n=0;;n++){ev();ck(n<40,"AR producer not accepted");bool fire=t.ar_ready_o&(1<<p);tick();if(fire)break;}
 t.ar_valid_i&=~(1<<p);idle(3);
}
static void offer_aw(unsigned p,unsigned len=0){
 t.aw_valid_i|=1u<<p;set64(t.aw_addr_i,p,0x20000000ull+(uint64_t(p)<<12)+awfires*8);setbyte(t.aw_len_i,p,len);
 for(unsigned n=0;;n++){ev();ck(n<40,"AW producer not accepted");bool fire=t.aw_ready_o&(1<<p);tick();if(fire)break;}
 t.aw_valid_i&=~(1<<p);idle(3);
}
static void offer_w(unsigned p,bool last,uint64_t data){
 t.w_valid_i|=1u<<p;if(last)t.w_last_i|=1u<<p;else t.w_last_i&=~(1u<<p);set64(t.w_data_i,p,data);
 for(unsigned n=0;;n++){ev();ck(n<40,"W producer not accepted");bool fire=t.w_ready_o&(1<<p);tick();if(fire)break;}
 t.w_valid_i&=~(1<<p);idle(2);
}
static void send_r(unsigned id,bool last,uint64_t data){
 t.ds_r_valid_i=1;t.ds_r_id_i=id;t.ds_r_last_i=last;t.ds_r_data_i=data;
 for(unsigned n=0;;n++){ev();ck(n<40,"external R not accepted");bool fire=t.ds_r_ready_o;tick();if(fire)break;}
 t.ds_r_valid_i=0;idle(3);
}
static void send_b(unsigned id){
 t.ds_b_valid_i=1;t.ds_b_id_i=id;
 for(unsigned n=0;;n++){ev();ck(n<40,"external B not accepted");bool fire=t.ds_b_ready_o;tick();if(fire)break;}
 t.ds_b_valid_i=0;idle(3);
}
static uint32_t random_state=0x20261018;
static unsigned rnd(){random_state^=random_state<<13;random_state^=random_state>>17;random_state^=random_state<<5;return random_state;}
struct Rjob{unsigned id,left;};struct Wjob{unsigned id,left;};
static void random_legal(){
 // Producer request queues never withdraw a held offer. W has deliberately
 // independent timing, including W-before-AW. The bus model holds responses.
 const unsigned rp[6]={0,1,2,3,4,5},wp[5]={4,5,6,7,8};
 const unsigned awid[9]={0,0,0,0,2,0,1,4,3};
 std::deque<Rjob> rq;std::deque<Wjob>wq;std::deque<unsigned>bq;std::deque<bool> wbq;
 unsigned arremaining[9]={},awremaining[9]={},wremaining[9]={};
 bool rhold=false,bhold=false;
 for(unsigned it=0;it<3500;it++){
  bool create=it<2600;
  for(auto p:rp)if(create&&rq.size()<8&&!arremaining[p]&&!(t.ar_valid_i&(1u<<p))&&(rnd()%12==0)){
   arremaining[p]=1;t.ar_valid_i|=1u<<p;set64(t.ar_addr_i,p,0x30000000ull+(uint64_t(p)<<20)+it*8);setbyte(t.ar_len_i,p,rnd()%4);
  }
  for(auto p:wp)if(create&&(wq.size()+bq.size()+wbq.size())<8&&!awremaining[p]&&!wremaining[p]&&!(t.aw_valid_i&(1u<<p))&&!(t.w_valid_i&(1u<<p))&&(rnd()%17==0)){
   unsigned beats=1+rnd()%4;awremaining[p]=1;wremaining[p]=beats;t.aw_valid_i|=1u<<p;setbyte(t.aw_len_i,p,beats-1);set64(t.aw_addr_i,p,0x40000000ull+(uint64_t(p)<<20)+it*8);
  }
  for(auto p:wp)if(wremaining[p]&&!(t.w_valid_i&(1u<<p))&&(rnd()%3)){
   t.w_valid_i|=1u<<p;set64(t.w_data_i,p,0xabc0000000000000ull+(uint64_t(p)<<32)+wremaining[p]);
   if(wremaining[p]==1)t.w_last_i|=1u<<p;else t.w_last_i&=~(1u<<p);
  }
  t.ds_ar_ready_i=(rnd()%5)!=0;t.ds_aw_ready_i=(rnd()%4)!=0;t.ds_w_ready_i=(rnd()%3)!=0;
  t.r_ready_i=rnd()&0x1ff;t.b_ready_i=rnd()&0x1ff;
  if(!create)t.r_ready_i=t.b_ready_i=0x1ff;
  if(!rhold&&!rq.empty()&&(rnd()%3)){rhold=true;t.ds_r_valid_i=1;t.ds_r_id_i=rq.front().id;t.ds_r_last_i=rq.front().left==1;t.ds_r_data_i=0xadd00000ull+it;}
  if(!bhold&&!bq.empty()&&(rnd()%3)){bhold=true;t.ds_b_valid_i=1;t.ds_b_id_i=bq.front();}
  ev();
  unsigned araccept=t.ar_valid_i&t.ar_ready_o,awaccept=t.aw_valid_i&t.aw_ready_o,waccept=t.w_valid_i&t.w_ready_o;
  bool ar=t.ds_ar_valid_o&&t.ds_ar_ready_i,aw=t.ds_aw_valid_o&&t.ds_aw_ready_i,w=t.ds_w_valid_o&&t.ds_w_ready_i;
  bool r=t.ds_r_valid_i&&t.ds_r_ready_o,b=t.ds_b_valid_i&&t.ds_b_ready_o;
  if(ar)rq.push_back({unsigned(t.ds_ar_id_o),unsigned(t.ds_ar_len_o)+1});
  if(aw)wq.push_back({unsigned(t.ds_aw_id_o),unsigned(t.ds_aw_len_o)+1});
  if(w)wbq.push_back(t.ds_w_last_o);
  while(!wbq.empty()&&!wq.empty()){ck(wbq.front()==(wq.front().left==1),"WLAST/accepted AW length mismatch");wbq.pop_front();if(!--wq.front().left){bq.push_back(wq.front().id);wq.pop_front();}}
  tick();
  for(auto p:rp)if(araccept&(1u<<p)){t.ar_valid_i&=~(1u<<p);arremaining[p]=0;}
  for(auto p:wp){if(awaccept&(1u<<p)){t.aw_valid_i&=~(1u<<p);awremaining[p]=0;}if(waccept&(1u<<p)){t.w_valid_i&=~(1u<<p);ck(wremaining[p]>0,"producer W model underflow");wremaining[p]--;}}
  if(r){ck(!rq.empty(),"R model empty");if(!--rq.front().left)rq.pop_front();rhold=false;t.ds_r_valid_i=0;}
  if(b){ck(!bq.empty(),"B model empty");bq.pop_front();bhold=false;t.ds_b_valid_i=0;}
 }
 t.ds_ar_ready_i=t.ds_aw_ready_i=t.ds_w_ready_i=1;t.r_ready_i=t.b_ready_i=0x1ff;idle(8);
 ck(rq.empty()&&wq.empty()&&bq.empty()&&wbq.empty()&&!t.ar_valid_i&&!t.aw_valid_i&&!t.w_valid_i,"legal model did not drain");
 ck(!t.bad_o&&t.transport_quiet_o,"legal random drain not quiet");
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);ck(argc==3,"need trace and cases paths");trace=fopen(argv[1],"w");cases=fopen(argv[2],"w");ck(trace&&cases,"output open");
 fprintf(trace,"cycle,eval,phase,clk,rst,equal_on,equal_off,buffers_quiet,transport_quiet,read_debt,write_debt,bad,owner_empty,ar_spill,aw_spill,w_spill,r_spill,b_spill,ar_valid,ar_ready,aw_valid,aw_ready,w_valid,w_ready,wlast,r_valid,r_ready,rlast,b_valid,b_ready,unit_read,unit_write,unit_bad,msi_mode,msi_idle,msi_fill,msi_error\n");fprintf(cases,"case,phase,start_cycle\n");
 mark("reset-and-empty");reset();idle();
 mark("unit-net-zero-at-zero");t.unit_read_issue_i=t.unit_read_last_i=t.unit_write_issue_i=t.unit_write_done_i=1;idle(3);ck(!t.unit_bad_o&&t.unit_quiet_o,"net11 atzero faulted");
 mark("unit-read-write-255");t.unit_read_last_i=t.unit_write_done_i=0;idle(255);ck(t.unit_read_o==255&&t.unit_write_o==255&&!t.unit_bad_o,"255 fill failed");
 mark("unit-net-zero-at-255");t.unit_read_last_i=t.unit_write_done_i=1;idle(3);ck(!t.unit_bad_o&&t.unit_read_o==255&&t.unit_write_o==255,"net11 at255 faulted");
 mark("unit-overflow-saturates-sticky");t.unit_read_last_i=t.unit_write_done_i=0;tick();ck(t.unit_read_o==255&&t.unit_write_o==255&&t.unit_bad_o&&!t.unit_quiet_o,"overflow not sticky saturated");
 mark("unit-drain-cannot-clear-bad");t.unit_read_issue_i=t.unit_write_issue_i=0;t.unit_read_last_i=t.unit_write_done_i=1;idle(255);ck(t.unit_read_o==0&&t.unit_write_o==0&&t.unit_bad_o&&!t.unit_quiet_o,"drain cleared bad");
 mark("unit-reset-only-and-underflow");reset();t.unit_read_last_i=t.unit_write_done_i=1;tick();ck(t.unit_bad_o&&t.unit_read_o==0&&t.unit_write_o==0,"underflow not sticky holdzero");t.unit_read_last_i=t.unit_write_done_i=0;idle(3);ck(t.unit_bad_o,"idle clearedbad");reset();
 mark("unit-independent-cross-events");reset();
 t.unit_read_issue_i=1;idle(255);ck(t.unit_read_o==255&&t.unit_write_o==0&&!t.unit_bad_o,"independent read255 failed");
 t.unit_read_last_i=1;t.unit_write_issue_i=1;tick();ck(t.unit_read_o==255&&t.unit_write_o==1&&!t.unit_bad_o,"read net11/writeinc interfered");
 t.unit_read_issue_i=0;t.unit_write_issue_i=0;t.unit_write_done_i=1;tick();ck(t.unit_read_o==254&&t.unit_write_o==0&&!t.unit_bad_o,"independent decrements failed");reset();
 t.unit_write_issue_i=1;idle(255);ck(t.unit_write_o==255&&t.unit_read_o==0&&!t.unit_bad_o,"independent write255 failed");
 t.unit_write_done_i=1;t.unit_read_issue_i=1;tick();ck(t.unit_write_o==255&&t.unit_read_o==1&&!t.unit_bad_o,"write net11/readinc interfered");reset();
 mark("all-producer-current-offers");
 // Hold DS outputs to fill both spill slots. Unsupported offers are current
 // guard inputs only and are cleared before a rising edge.
 for(unsigned p=0;p<9;p++)for(unsigned ch=0;ch<3;ch++){
  if(ch==0)t.ar_valid_i=1u<<p;else if(ch==1)t.aw_valid_i=1u<<p;else t.w_valid_i=1u<<p;
  ev();ck(!t.buffers_quiet_o&&!t.transport_quiet_o,"current offer escaped guard");t.ar_valid_i=t.aw_valid_i=t.w_valid_i=0;ev();
 }
 mark("W-before-AW-owner-WLAST");t.w_valid_i=1u<<6;t.w_last_i&=~(1u<<6);set64(t.w_data_i,6,0x6161);idle(4);ck(!(t.w_ready_o&(1<<6))&&t.owner_empty_o,"empty owner accepted earlyW");
 t.aw_valid_i=1u<<6;setbyte(t.aw_len_i,6,1);set64(t.aw_addr_i,6,0x21610000);
 for(unsigned n=0;t.aw_valid_i||t.w_valid_i;n++){ck(n<30,"earlyW/AW not accepted");ev();bool a=t.aw_valid_i&&(t.aw_ready_o&(1u<<6)),w=t.w_valid_i&&(t.w_ready_o&(1u<<6));tick();if(a)t.aw_valid_i=0;if(w)t.w_valid_i=0;}
 idle(3);ck(!t.owner_empty_o,"nonLAST removedowner");offer_w(6,true,0x2222);ck(t.owner_empty_o&&t.write_debt_o==1&&!t.transport_quiet_o,"WLAST erased B debt");send_b(1);ck(t.transport_quiet_o,"write didnot drain");
 mark("multiple-owners-stalled-Wspill");t.ds_w_ready_i=0;offer_aw(5);offer_aw(4);offer_aw(8);offer_aw(7);
 offer_w(5,true,0x5555);offer_w(4,true,0x4444);ck(t.w_spill_o&&!t.transport_quiet_o,"stalled Wspill guard missing");
 t.ds_w_ready_i=1;idle(3);offer_w(8,true,0x8888);offer_w(7,true,0x7777);send_b(0);send_b(2);send_b(3);send_b(4);ck(t.transport_quiet_o,"owner order didnot drain");
 mark("AR-spill-and-two-beat-debt");t.ds_ar_ready_i=0;offer_ar(0,1);offer_ar(1);ck(t.ar_spill_o&&!t.buffers_quiet_o&&t.read_debt_o==0,"AR spill not guarded before external accept");t.ds_ar_ready_i=1;idle(4);ck(t.read_debt_o==2,"AR debt count wrong");send_r(1,false,0xdead);ck(t.read_debt_o==2,"nonlast retired read debt");send_r(1,true,0xbeef);send_r(2,true,0xcafe);ck(t.transport_quiet_o,"AR read debt didnot drain");
 mark("R-spill-after-external-RLAST");offer_ar(2);t.r_ready_i&=~(1u<<2);send_r(0,true,0x1234);ck(t.read_debt_o==0&&t.r_spill_o&&!t.buffers_quiet_o&&!t.transport_quiet_o,"held R spill lost after external debt retired");t.r_ready_i=0x1ff;idle();ck(t.transport_quiet_o,"R spill didnot drain");
 mark("B-spill-after-external-B");offer_aw(6);offer_w(6,true,0x1234);t.b_ready_i&=~(1u<<6);send_b(1);ck(t.write_debt_o==0&&t.b_spill_o&&!t.transport_quiet_o,"held B spill lost after external debt retired");t.b_ready_i=0x1ff;idle();ck(t.transport_quiet_o,"B spill didnot drain");
 mark("DS-underflow-sticky-original-dataflow");send_b(1);ck(t.bad_o&&!t.transport_quiet_o,"unsolicitedB notsticky");offer_ar(2);send_r(0,true,0xabcdef);offer_aw(6);offer_w(6,true,0x123456);send_b(1);ck(t.bad_o&&!t.transport_quiet_o,"bad cleared or lost data path");reset();
 mark("legal-random-concurrent-backpressure");random_legal();
 mark("DS-read-overflow-preserves-original-dataflow");reset();
 for(unsigned n=0;n<256;n++)offer_ar(0);
 ck(t.read_debt_o==255&&t.bad_o&&!t.transport_quiet_o,"DS read overflow not saturatedsticky");
 for(unsigned n=0;n<256;n++)send_r(1,true,0x510000+n);
 ck(t.read_debt_o==0&&t.bad_o&&t.buffers_quiet_o&&!t.transport_quiet_o,"DS read drain clearedbad");
 mark("DS-write-overflow-preserves-original-dataflow");reset();
 for(unsigned n=0;n<256;n++){offer_aw(6);offer_w(6,true,0x610000+n);}
 ck(t.write_debt_o==255&&t.bad_o&&!t.transport_quiet_o,"DS write overflow not saturatedsticky");
 for(unsigned n=0;n<256;n++)send_b(1);
 ck(t.write_debt_o==0&&t.bad_o&&t.buffers_quiet_o&&!t.transport_quiet_o,"DS write drain clearedbad");
 mark("actual-MSIPTW-two-beat-BT-tail-gap");reset();t.msi_mode_i=1;t.msi_init_i=1;tick();t.msi_init_i=0;
 for(unsigned n=0;t.read_debt_o==0;n++){ck(n<30,"real MSIPTW didnot issueAR");tick();}
 ck(t.ds_ar_len_o==1&&t.ds_ar_id_o==4,"real MSIPTW AR nottwo-beat ID4");
 // Valid Basic-Translate PTE: mode3, V1, reserved0, ppn0x80000.
 unsigned beforefills=fills;send_r(4,false,(uint64_t(0x80000)<<10)|7);
 ck(t.msi_idle_o&&fills==beforefills+1&&!t.msi_error_o,"real first beat didnot finish BT");
 for(unsigned n=0;n<24;n++){ev();ck(t.msi_idle_o&&t.buffers_quiet_o&&!t.ds_ar_valid_o&&!t.ds_r_valid_i&&!t.r_spill_o,"BT gap isnot actual idle/currentVALID0");ck(t.read_debt_o==1&&!t.transport_quiet_o&&!t.bad_o,"tailgap incorrectlyquiet");tick();}
 send_r(4,true,0);ck(t.transport_quiet_o&&!t.read_debt_o&&!t.bad_o,"realMSI tail didnot settlequiet");
 mark("actual-MSIPTW-tail-and-spill-held");
 // The dedicated earlier R-spill case covers downstream hold; real walker has
 // hardwired RREADY1, so this exact walker cannot hold its own tail response.
 ck(t.msi_idle_o&&t.transport_quiet_o,"real walker finalquiet missing");
 fprintf(stdout,"PASS cases=%u cycles=%u evals=%u checks=%u AR=%u AW=%u W=%u RLAST=%u B=%u MSIfill=%u\n",ncases,cycles,evals,checks,arfires,awfires,wfires,rlasts,bfires,fills);
 fclose(trace);fclose(cases);return 0;
}
