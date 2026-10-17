#include "Vpn_backend_top.h"
#include <verilated.h>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

double sc_time_stamp(){ return 0; }
static void need(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
struct Burst{uint64_t addr;unsigned beats,index,id;uint64_t ready;};
struct Reply{unsigned id;uint64_t ready;};
struct Job{uint64_t len,src,dst;unsigned type;bool eligible;};
struct Hold{
 bool active=false;uint64_t a=0,b=0,c=0;
 void check(bool v,bool r,uint64_t x,uint64_t y,uint64_t z,const char *name){
  need(!active||(v&&a==x&&b==y&&c==z),std::string(name)+" withdrawn/changed while stalled");
  active=v&&!r;a=x;b=y;c=z;
 }
};
struct Bench{
 VerilatedContext context;
 Vpn_backend_top d{&context};
 std::string name;
 std::vector<uint8_t> mem,expected;
 std::deque<Burst> reads,writes;std::deque<Reply> replies;std::deque<Job> pending;
 Hold har,haw,hw,hr,hb,hctrlr,hctrlb;
 uint64_t cycle=0,compares=0,fires=0,done=0,backend_fires=0,read_bytes=0,write_bytes=0,peak_buffered=0;
 unsigned ar=0,aw=0,rlast=0,b=0,issue=0,obs_done=0,permit_edges=0;
 unsigned stall_ar=0,stall_aw=0,stall_w=0,stall_r=0,stall_b=0,stall_ctrl_r=0,stall_ctrl_b=0;
 bool block_writes=false,block_w_data=false,last_permit=false,requested_nonblock=false;
 uint64_t held_ctrl_data=0;unsigned held_ctrl_resp=0;
 std::vector<uint64_t> copy_issues,copy_dones,launches;
 uint64_t stop_edges=0,frontend_held=0,cooldown_edges=0;
 Bench(std::string n,bool compare,bool nonblock):name(std::move(n)),mem(1<<20),expected(1<<20){
  for(size_t i=0;i<mem.size();i++)mem[i]=uint8_t((i*37)^(i>>3)^(i>>11)^0xa5);
  std::fill(mem.begin()+0x40000,mem.begin()+0x60000,0xcc);expected=mem;
  d.clk_i=0;d.rst_ni=0;d.compare_i=compare;d.nonblock_i=nonblock;
  d.ctrl_aw_valid_i=d.ctrl_w_valid_i=d.ctrl_ar_valid_i=0;
  d.ctrl_b_ready_i=d.ctrl_r_ready_i=1;d.ctrl_w_strb_i=0xff;
  d.ctrl_aw_addr_i=d.ctrl_ar_addr_i=d.ctrl_w_data_i=0;
  d.ar_ready_i=d.aw_ready_i=d.w_ready_i=d.r_valid_i=d.b_valid_i=0;
  d.r_last_i=d.r_data_i=d.r_id_i=d.r_resp_i=d.b_id_i=d.b_resp_i=0;
  for(unsigned i=0;i<4;i++){d.clk_i=0;d.eval();context.timeInc(1);d.clk_i=1;d.eval();context.timeInc(1);}
  // The SV default input generates an initial assignment in this Verilator.
  // Drive the external mode after initialization, then check both sides on
  // every edge. Production reset/mux/certificate behavior is unchanged.
  d.rst_ni=1;requested_nonblock=nonblock;d.nonblock_i=nonblock;
  for(unsigned i=0;i<4;i++)tick();
 }
 uint64_t load(uint64_t p){need(p+8<=mem.size(),"read outside bounded memory");uint64_t x=0;for(unsigned i=0;i<8;i++)x|=uint64_t(mem[p+i])<<(i*8);return x;}
 void compare(){
  need(bool(d.nonblock_i)==requested_nonblock,"public runtime mode not actually driven");
  need(bool(d.select_n_o)==bool(PN_ENABLE&&!TWO_D&&requested_nonblock),"actual backend selector differs from requested compile/runtime mode");
  if(d.compare_i){need(d.equal_o,"independent8aac wrapper+frontend cycle gold mismatch");++compares;}
  need(d.inactive_clean_o,"inactive backend accepted input or generated work");
  const bool stop=TAP_ENABLE&&!TWO_D&&d.reg_valid_o&&d.reg_write_o&&d.reg_addr_o==0x88&&d.reg_strb_o==255&&d.reg_data_o==2;
  const bool fq=TAP_ENABLE&&!TWO_D&&d.frontend_state_o==0&&!d.frontend_offer_o&&!d.frontend_raw_issue_o&&!d.batch_active_o;
  need(bool(d.stop_o)==stop,"actual AXI-to-reg STOP qualifier not passed through");
  need(bool(d.frontend_quiet_o)==fq,"frontend quiet differs from IDLE/no-offer/no-rawlaunch/no-active");
 }
 void select(bool mode){need(d.quiet_o&&d.frontend_idle_o,"external full frontend/backend quiet required before test mode switch");requested_nonblock=mode;d.nonblock_i=mode;}
 void event(const char *what,uint64_t x=0,uint64_t y=0,uint64_t z=0){
  std::printf("{\"event\":\"%s\",\"fixture_only\":true,\"source_board_cases\":0,\"case\":\"%s\",\"cycle\":%llu,\"x\":%llu,\"y\":%llu,\"z\":%llu}\n",what,name.c_str(),(unsigned long long)cycle,(unsigned long long)x,(unsigned long long)y,(unsigned long long)z);
 }
 void tick(){
  need(cycle<160000,"finite case watchdog");d.clk_i=0;
  d.ar_ready_i=cycle%5!=0&&reads.size()<8;
  d.aw_ready_i=!block_writes&&cycle%7!=2&&writes.size()<8;
  d.w_ready_i=!block_writes&&!block_w_data&&!writes.empty()&&cycle%3!=0;
  d.r_valid_i=!reads.empty()&&cycle>=reads.front().ready;
  d.r_data_i=d.r_last_i=d.r_id_i=d.r_resp_i=0;
  if(d.r_valid_i){auto &r=reads.front();d.r_data_i=load(r.addr+8*r.index);d.r_id_i=r.id;d.r_last_i=r.index+1==r.beats;}
  d.b_valid_i=!replies.empty()&&cycle>=replies.front().ready;d.b_id_i=d.b_valid_i?replies.front().id:0;d.b_resp_i=0;
  d.eval();context.timeInc(1);compare();
  har.check(d.ar_valid_o,d.ar_ready_i,d.ar_addr_o,d.ar_len_o|(uint64_t(d.ar_size_o)<<8),d.ar_id_o,"AR");
  haw.check(d.aw_valid_o,d.aw_ready_i,d.aw_addr_o,d.aw_len_o|(uint64_t(d.aw_size_o)<<8),d.aw_id_o,"AW");
  hw.check(d.w_valid_o,d.w_ready_i,d.w_data_o,d.w_strb_o,d.w_last_o,"W");
  hr.check(d.r_valid_i,d.r_ready_o,d.r_data_i,d.r_id_i,d.r_last_i,"R");
  hb.check(d.b_valid_i,d.b_ready_o,d.b_id_i,d.b_resp_i,0,"B");
  hctrlr.check(d.ctrl_r_valid_o,d.ctrl_r_ready_i,d.ctrl_r_data_o,d.ctrl_r_resp_o,0,"control R");
  hctrlb.check(d.ctrl_b_valid_o,d.ctrl_b_ready_i,d.ctrl_b_resp_o,0,0,"control B");
  stall_ar+=d.ar_valid_o&&!d.ar_ready_i;stall_aw+=d.aw_valid_o&&!d.aw_ready_i;
  stall_w+=d.w_valid_o&&!d.w_ready_i;stall_r+=d.r_valid_i&&!d.r_ready_o;stall_b+=d.b_valid_i&&!d.b_ready_o;
  stall_ctrl_r+=d.ctrl_r_valid_o&&!d.ctrl_r_ready_i;stall_ctrl_b+=d.ctrl_b_valid_o&&!d.ctrl_b_ready_i;
  const bool jf=d.job_fire_o,jd=d.job_done_o,frontend_retire=d.frontend_retire_o;
  stop_edges+=d.stop_o;frontend_held+=d.frontend_offer_o&&!d.frontend_ready_o;
  cooldown_edges+=d.frontend_state_o==3;
  if(d.frontend_raw_issue_o){launches.push_back(cycle);event("frontend_launch",d.frontend_state_o,d.gap_o);}
  if(d.obs_issue_o){copy_issues.push_back(cycle);event("copy_issue",copy_issues.size(),d.frontend_state_o,d.remaining_o);}
  if(d.obs_done_o){copy_dones.push_back(cycle);event("copy_done",copy_dones.size(),d.frontend_state_o,d.remaining_o);}
  if(d.stop_o)event("stop_tap",stop_edges,d.frontend_state_o,d.remaining_o);
  Job job{d.job_length_o,d.job_src_o,d.job_dst_o,unsigned(d.ttype_o),false};
  job.eligible=PN_ENABLE&&d.nonblock_i&&job.type==0&&job.len==4096&&(job.src%4096)==0&&(job.dst%4096)==0;
  const bool tr=d.r_valid_i&&d.r_ready_o,tb=d.b_valid_i&&d.b_ready_o;
  const bool tw=d.w_valid_o&&d.w_ready_i,tar=d.ar_valid_o&&d.ar_ready_i,taw=d.aw_valid_o&&d.aw_ready_i;
  const uint64_t araddr=d.ar_addr_o,awaddr=d.aw_addr_o,wd=d.w_data_o;
  const unsigned arlen=d.ar_len_o,awlen=d.aw_len_o,arid=d.ar_id_o,awid=d.aw_id_o,ws=d.w_strb_o,wl=d.w_last_o;
  if(tar)need(d.ar_size_o==3&&d.ar_burst_o==1&&d.ar_sid_o==10&&araddr%8==0&&(araddr%2048)+(arlen+1)*8<=2048,"AR geometry/identity");
  if(taw)need(d.aw_size_o==3&&d.aw_burst_o==1&&d.aw_sid_o==10&&awaddr%8==0&&(awaddr%2048)+(awlen+1)*8<=2048,"AW geometry/identity");
  if(d.backend_req_fire_o)++backend_fires;
  issue+=d.obs_issue_o;obs_done+=d.obs_done_o;
  d.clk_i=1;d.eval();context.timeInc(1);compare();
  if(frontend_retire)
   need(d.frontend_state_o==0,"last COPY has no extra tail cooldown");
  if(jd){need(!pending.empty(),"retire without real accepted frontend job");pending.pop_front();++done;event("retire",done);}
  if(jf){need(pending.empty(),"serial frontend unexpectedly overlapped jobs");pending.push_back(job);++fires;
   need(job.src+job.len<mem.size()&&job.dst+job.len<mem.size(),"descriptor outside finite memory");
   for(uint64_t i=0;i<job.len;i++){
    if(job.type==0)expected[job.dst+i]=mem[job.src+i];
    if(job.type==2)expected[job.dst+i]=uint8_t(0x8877665544332211ULL>>(8*(i%8)));
   }
   event("job_fifo_fire",job.len,job.src,job.dst);
  }
  need(!d.certificate_fault_o,"real serial trace generated certificate fault");
  need(bool(d.certificate_pending_o)==bool(PN_ENABLE&&!pending.empty()),"certificate pending not bound to real jobFIFO fire/retire");
  const bool expected_permit=PN_ENABLE&&d.nonblock_i&&!pending.empty()&&pending.front().eligible&&d.ttype_o==0;
  need(bool(d.permit_o)==expected_permit,"permit not causally bound to actual full-capacity eligible job");
  if(bool(d.permit_o)!=last_permit){last_permit=d.permit_o;++permit_edges;event("permit",last_permit,fires,done);}
  if(tr){auto &r=reads.front();event("R",r.addr+8*r.index,load(r.addr+8*r.index),r.index+1==r.beats);read_bytes+=8;if(++r.index==r.beats){++rlast;reads.pop_front();}}
  if(tb){++b;event("B",replies.front().id);replies.pop_front();}
  if(tw){need(!writes.empty(),"W without accepted AW");auto &w=writes.front();if(wl!=(w.index+1==w.beats)){event("unexpected_WLAST",wl,w.index+1==w.beats,compares);std::fflush(stdout);}need(wl==(w.index+1==w.beats),"WLAST mismatch");
   need(w.addr+8*w.index+8<=mem.size(),"W outside model");for(unsigned i=0;i<8;i++)if(ws&(1u<<i)){mem[w.addr+8*w.index+i]=uint8_t(wd>>(8*i));++write_bytes;}
   event("W",w.addr+8*w.index,wd,ws|(uint64_t(wl)<<8));
   if(++w.index==w.beats){replies.push_back({w.id,cycle+13});writes.pop_front();}
  }
  if(tar){reads.push_back({araddr,arlen+1,0,arid,cycle+3});++ar;event("AR",araddr,arlen,arid);}
  if(taw){writes.push_back({awaddr,awlen+1,0,awid,cycle});++aw;event("AW",awaddr,awlen,awid);}
  peak_buffered=std::max(peak_buffered,read_bytes>=write_bytes?read_bytes-write_bytes:0);
  ++cycle;
 }
 void idle(unsigned n){for(unsigned i=0;i<n;i++)tick();}
 void write(uint64_t addr,uint64_t value,unsigned hold=0){
  d.ctrl_aw_addr_i=addr;d.ctrl_w_data_i=value;d.ctrl_aw_valid_i=d.ctrl_w_valid_i=1;d.ctrl_b_ready_i=0;
  bool awok=false,wok=false;unsigned guard=0;
  while(!awok||!wok){tick_low();bool a=d.ctrl_aw_ready_o&&d.ctrl_aw_valid_i,w=d.ctrl_w_ready_o&&d.ctrl_w_valid_i;tick();
   awok|=a;wok|=w;d.ctrl_aw_valid_i=!awok;d.ctrl_w_valid_i=!wok;need(++guard<100,"control AW/W timeout");}
  while(true){tick_low();if(d.ctrl_b_valid_o)break;tick();need(++guard<200,"control B timeout");}
  need(d.ctrl_b_resp_o==0,"control write rejected");idle(hold);d.ctrl_b_ready_i=1;tick();d.ctrl_aw_valid_i=d.ctrl_w_valid_i=0;
 }
 void tick_low(){d.clk_i=0;d.eval();}
 uint64_t read(uint64_t addr,unsigned hold=0){
  d.ctrl_ar_addr_i=addr;d.ctrl_ar_valid_i=1;d.ctrl_r_ready_i=0;unsigned guard=0;
  while(true){tick_low();bool a=d.ctrl_ar_ready_o;tick();if(a)break;need(++guard<200,"control AR timeout");}
  d.ctrl_ar_valid_i=0;while(true){tick_low();if(d.ctrl_r_valid_o)break;tick();need(++guard<300,"control R timeout");}
  need(d.ctrl_r_resp_o==0,"control read rejected");uint64_t value=d.ctrl_r_data_o;idle(hold);d.ctrl_r_ready_i=1;tick();return value;
 }
 void configure(uint64_t len=4096,uint64_t src=0x10000,uint64_t dst=0x40000,unsigned type=0){
  write(0x88,0);write(0,src);write(8,dst);write(0x10,len);write(0x18,type<<4);write(0x48,1);write(0x40,0x8877665544332211ULL);
 }
 void start_batch(unsigned count,unsigned pages){write(0x90,count);write(0x98,pages);write(0x88,1);read(0x28,2);}
 void drain(){unsigned stable=0;while(stable<8){tick();bool q=d.quiet_o&&d.frontend_idle_o&&pending.empty()&&reads.empty()&&writes.empty()&&replies.empty();stable=q?stable+1:0;}need(fires==done&&ar==rlast&&aw==b,"retire/burst/drain conservation");need(mem==expected,"actual bytes or untouched guard mismatch");}
 void summary(){
  need(!d.permit_o&&!d.certificate_pending_o&&!d.certificate_fault_o,"final certificate revoked without reset");
  event("new_tap_accounting",stop_edges,frontend_held,cooldown_edges);
  std::printf("{\"event\":\"case_summary\",\"case\":\"%s\",\"status\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"enable\":%u,\"cycles\":%llu,\"gold_comparisons\":%llu,\"job_fifo_fires\":%llu,\"backend_fires\":%llu,\"retires\":%llu,\"ar\":%u,\"aw\":%u,\"rlast\":%u,\"b\":%u,\"read_bytes\":%llu,\"write_bytes\":%llu,\"peak_buffered_bytes\":%llu,\"permit_edges\":%u,\"stalls\":[%u,%u,%u,%u,%u,%u,%u],\"drained\":true,\"data_and_guards_equal\":true}\n",name.c_str(),PN_ENABLE,(unsigned long long)cycle,(unsigned long long)compares,(unsigned long long)fires,(unsigned long long)backend_fires,(unsigned long long)done,ar,aw,rlast,b,(unsigned long long)read_bytes,(unsigned long long)write_bytes,(unsigned long long)peak_buffered,permit_edges,stall_ar,stall_aw,stall_w,stall_r,stall_b,stall_ctrl_r,stall_ctrl_b);
 }
};
int main(int argc,char **argv){Verilated::commandArgs(argc,argv);try{
 if(TWO_D){
  Bench x("twoD_idle_conservative_outputs",false,true);x.idle(12);x.read(0);x.write(0,0);x.idle(8);
  need(!x.d.stop_o&&!x.d.frontend_quiet_o&&!x.d.select_n_o&&!x.d.permit_o&&!x.d.quiet_o,"2D conservative new outputs");
  need(x.fires==0&&x.ar==0&&x.aw==0,"2D idle probe launched no job");
  std::puts("{\"event\":\"twoD_idle_passed\",\"fixture_only\":true,\"source_board_cases\":0,\"scope\":\"actual 2D idle/control only, no 2D DMA workload\"}");
  return 0;
 }
 {
  Bench x("S_reset_gap0_independent_gold",true,false);x.configure();
  x.write(0x70,0x11223344,3);need(x.read(0x78,3)==0x12345678,"old observer cfg read-through");
  x.start_batch(3,1);x.drain();need(x.fires==3&&x.copy_issues.size()==3,"S actual three-copy batch");
  need(x.launches.size()==1&&x.copy_issues[0]==x.launches[0]+1,"no first-copy gap");
  for(unsigned i=0;i<2;++i)need(x.copy_issues[i+1]==x.copy_dones[i]+1,"reset gap0 refill is original cycle");
  need(x.stop_edges==0&&x.cooldown_edges==0,"reset default no STOP/cooldown");x.summary();
 }
 {
  Bench x("S_real_MMIO_STOP_held_AXI_gold",true,false);x.configure();x.block_writes=true;
  x.start_batch(100,1);while(!x.fires)x.tick();x.idle(40);
  need(x.read(0x88)&1,"batch read is not a STOP");need(x.stop_edges==0,"read never pulses STOP");
  x.d.ctrl_w_strb_i=0x7f;x.write(0x88,2);x.d.ctrl_w_strb_i=255;
  need(x.stop_edges==0,"partial MMIO strobe never pulses STOP");
  x.write(0x88,2,4);need(x.stop_edges==(TAP_ENABLE?1:0),"one actual accepted full-strobe STOP");
  x.block_writes=false;x.drain();need(x.fires==1&&x.done==1,"STOP held data work drains naturally");
  x.write(0x88,2);need(x.stop_edges==(TAP_ENABLE?2:0),"repeated idle STOP is accepted event");x.summary();
 }
 if(GAP_ENABLE){
  for(unsigned gap:{1U,1000U,4000U}){
   Bench x("gap_"+std::to_string(gap),false,PN_ENABLE);x.write(0xb0,gap);
   need(x.read(0xb8)==0x49444d4147415001ULL&&x.read(0xb0)==gap,"real MMIO gap config/cap readback");
   x.configure();need(x.d.gap_o==gap,"batch disable/configure preserves gap");
   x.start_batch(3,1);x.drain();need(x.copy_issues.size()==3&&x.copy_dones.size()==3,"three natural copies");
   need(x.launches.size()==1&&x.copy_issues[0]==x.launches[0]+1,"configured gap adds no first delay");
   for(unsigned i=0;i<2;++i){need(x.copy_issues[i+1]-x.copy_dones[i]==uint64_t(gap)+1,"exact refill gap cycles");
    x.event("refill_checked",x.copy_dones[i],x.copy_issues[i+1],gap);}
   need(x.cooldown_edges==uint64_t(gap)*2,"exact two cooldowns, no first/tail extra gap");x.summary();
  }
  {
   Bench x("MMIO_STOP_during_cooldown",false,PN_ENABLE);x.configure();x.write(0xb0,4000);x.start_batch(10,1);
   while(x.copy_dones.empty())x.tick();need(x.d.frontend_state_o==3&&!x.d.frontend_quiet_o,"cooldown locally not quiet");
   x.write(0x88,2);x.drain();need(x.fires==1&&x.stop_edges==(TAP_ENABLE?1:0),"cooldown STOP creates no second COPY");x.summary();
  }
 }
 if(PN_ENABLE){
  Bench x("N4096_capacity_and_natural_certificate_revoke",false,true);x.configure();x.block_w_data=true;x.read(0x28);
  for(unsigned i=0;i<4000&&x.read_bytes<4096;++i)x.tick();
  x.event("capacity_probe",x.read_bytes,x.d.large_byte_count_o,x.d.permit_o);
  need(x.read_bytes==4096&&x.write_bytes==0&&x.d.large_byte_count_o==512&&x.d.permit_o,"true N full4096 with AW accepted W held");
  x.block_w_data=false;x.drain();need(x.fires==1&&x.done==1&&x.permit_edges==2,"natural N retirement revokes certificate");x.summary();
 }
 std::printf("{\"event\":\"all_passed\",\"enable\":%u,\"taps\":%u,\"gap\":%u,\"fixture_only\":true,\"source_board_cases\":0}\n",PN_ENABLE,TAP_ENABLE,GAP_ENABLE);
 return 0;
 }catch(const std::exception&e){std::fflush(stdout);std::fprintf(stderr,"FAIL %s\n",e.what());return 1;}}
