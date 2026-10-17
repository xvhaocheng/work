#include "Vpn_peripheral_top.h"
#include <verilated.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <stdexcept>
#include <string>
#include <unordered_map>

double sc_time_stamp(){return 0;}
static void need(bool x,const std::string& why){if(!x)throw std::runtime_error(why);}
struct Burst{uint64_t address;unsigned beats,index,id,size;uint64_t ready;};
struct Reply{unsigned id;uint64_t ready;};
struct Held{
 bool active=false;uint64_t a=0,b=0,c=0;
 void check(bool v,bool ready,uint64_t x,uint64_t y,uint64_t z){
  need(!active||(v&&a==x&&b==y&&c==z),"AXI held offer changed");
  active=v&&!ready;a=x;b=y;c=z;
 }
};
struct Port{
 std::deque<Burst> reads,writes;std::deque<Reply> replies;
 Held ar,aw,w,r,b;uint64_t ar_count=0,r_last=0,aw_count=0,b_count=0;
};
struct Bench{
 VerilatedContext context;Vpn_peripheral_top d{&context};
 std::unordered_map<uint64_t,uint8_t> memory;
 std::array<Port,2> ports;
 uint64_t cycle=0,clear_edges=0,n10=0,n11=0;
 bool blocked_w=false,recording=false,has_a=false;
 uint64_t a_first=UINT64_MAX,u_first=UINT64_MAX,a_last=0,u_last=0;
 unsigned issues[4]={},dones[4]={};
 Bench(){
  d.clk_i=0;d.rst_ni=0;d.cfg_iommu_i=0;
  d.ctrl_aw_valid_i=d.ctrl_w_valid_i=d.ctrl_ar_valid_i=0;
  d.ctrl_b_ready_i=d.ctrl_r_ready_i=1;d.ctrl_w_strb_i=255;
  d.ctrl_aw_addr_i=d.ctrl_ar_addr_i=d.ctrl_w_data_i=0;
  for(unsigned p=0;p<2;p++){
   d.mem_ar_ready_i[p]=d.mem_aw_ready_i[p]=d.mem_w_ready_i[p]=0;
   d.mem_r_valid_i[p]=d.mem_b_valid_i[p]=d.mem_r_last_i[p]=0;
   d.mem_r_data_i[p]=d.mem_r_id_i[p]=d.mem_b_id_i[p]=d.mem_r_resp_i[p]=d.mem_b_resp_i[p]=0;
  }
  for(unsigned i=0;i<4;i++){d.clk_i=0;d.eval();context.timeInc(1);d.clk_i=1;d.eval();context.timeInc(1);}
  d.rst_ni=1;idle(8);
 }
 uint64_t load(uint64_t address){
  uint64_t value=0;for(unsigned i=0;i<8;i++)value|=uint64_t(memory[address+i])<<(8*i);return value;
 }
 void store(uint64_t address,uint64_t value,unsigned strobe=255){
  for(unsigned i=0;i<8;i++)if(strobe&(1u<<i))memory[address+i]=uint8_t(value>>(8*i));
 }
 void low(){d.clk_i=0;d.eval();}
 void tick(){
  need(cycle<400000,"bounded peripheral watchdog");d.clk_i=0;
  for(unsigned p=0;p<2;p++){
   auto& q=ports[p];
   d.mem_ar_ready_i[p]=cycle%5!=p&&q.reads.size()<16;
   d.mem_aw_ready_i[p]=cycle%7!=p&&q.writes.size()<16;
   d.mem_w_ready_i[p]=!blocked_w&&!q.writes.empty()&&cycle%3!=p;
   d.mem_r_valid_i[p]=!q.reads.empty()&&cycle>=q.reads.front().ready;
   d.mem_r_data_i[p]=d.mem_r_id_i[p]=d.mem_r_last_i[p]=d.mem_r_resp_i[p]=0;
   if(d.mem_r_valid_i[p]){
    const auto& b=q.reads.front();d.mem_r_data_i[p]=load((b.address+(uint64_t(b.index)<<b.size))&~uint64_t(7));
    d.mem_r_id_i[p]=b.id;d.mem_r_last_i[p]=b.index+1==b.beats;
   }
   d.mem_b_valid_i[p]=!q.replies.empty()&&cycle>=q.replies.front().ready;
   d.mem_b_id_i[p]=d.mem_b_valid_i[p]?q.replies.front().id:0;d.mem_b_resp_i[p]=0;
  }
  d.eval();context.timeInc(1);
  need(!(d.permit_o&12),"DMA2/3 gained fullbuffer permit");
  need(!d.certificate_fault_o,"unexpected real certificate fault");
  if(d.ctrl_b_valid_o)need(d.ctrl_b_id_o==45,"full control B ID6");
  if(d.ctrl_r_valid_o)need(d.ctrl_r_id_o==53,"full control R ID6");
  clear_edges+=d.policy_clear_o;
  if(d.arm_o){recording=true;has_a=d.arm_has_a_o;a_first=u_first=UINT64_MAX;a_last=u_last=0;n10=n11=0;}
  else if(recording){
   unsigned role_mask=has_a?3:1;
   if(d.issue_o&role_mask){if(u_first==UINT64_MAX)u_first=cycle;}
   if(d.done_o&role_mask)u_last=cycle;
   if(has_a&&(d.issue_o&2)&&a_first==UINT64_MAX)a_first=cycle;
   if(has_a&&(d.done_o&2))a_last=cycle;
   if(d.obs_active_o&&d.n_fire_o){
    need(d.n_did_o==10||d.n_did_o==11,"N actual accept unexpected DID");
    if(d.n_did_o==10)++n10;else ++n11;
   }
  }
  for(unsigned lane=0;lane<4;lane++){issues[lane]+=bool(d.issue_o&(1u<<lane));dones[lane]+=bool(d.done_o&(1u<<lane));}
  struct Edge{bool ar,aw,w,r,b;uint64_t aa,wa,wd;unsigned al,wl,ai,wi,as,ws,strb,last;};
  std::array<Edge,2> edge;
  for(unsigned p=0;p<2;p++){
   auto& q=ports[p];
   q.ar.check(d.mem_ar_valid_o[p],d.mem_ar_ready_i[p],d.mem_ar_addr_o[p],d.mem_ar_len_o[p],d.mem_ar_id_o[p]);
   q.aw.check(d.mem_aw_valid_o[p],d.mem_aw_ready_i[p],d.mem_aw_addr_o[p],d.mem_aw_len_o[p],d.mem_aw_id_o[p]);
   q.w.check(d.mem_w_valid_o[p],d.mem_w_ready_i[p],d.mem_w_data_o[p],d.mem_w_strb_o[p],d.mem_w_last_o[p]);
   q.r.check(d.mem_r_valid_i[p],d.mem_r_ready_o[p],d.mem_r_data_i[p],d.mem_r_id_i[p],d.mem_r_last_i[p]);
   q.b.check(d.mem_b_valid_i[p],d.mem_b_ready_o[p],d.mem_b_id_i[p],0,0);
   edge[p]={bool(d.mem_ar_valid_o[p]&&d.mem_ar_ready_i[p]),bool(d.mem_aw_valid_o[p]&&d.mem_aw_ready_i[p]),
    bool(d.mem_w_valid_o[p]&&d.mem_w_ready_i[p]),bool(d.mem_r_valid_i[p]&&d.mem_r_ready_o[p]),bool(d.mem_b_valid_i[p]&&d.mem_b_ready_o[p]),
    d.mem_ar_addr_o[p],d.mem_aw_addr_o[p],d.mem_w_data_o[p],d.mem_ar_len_o[p],d.mem_aw_len_o[p],
    d.mem_ar_id_o[p],d.mem_aw_id_o[p],d.mem_ar_size_o[p],d.mem_aw_size_o[p],d.mem_w_strb_o[p],d.mem_w_last_o[p]};
  }
  d.clk_i=1;d.eval();context.timeInc(1);
  for(unsigned p=0;p<2;p++){
   auto& q=ports[p];const auto& e=edge[p];
   if(e.r){auto& b=q.reads.front();if(++b.index==b.beats){++q.r_last;q.reads.pop_front();}}
   if(e.b){q.replies.pop_front();++q.b_count;}
   if(e.w){need(!q.writes.empty(),"W without accepted AW");auto& b=q.writes.front();
    need(e.last==(b.index+1==b.beats),"WLAST mismatch");
    store((b.address+(uint64_t(b.index)<<b.size))&~uint64_t(7),e.wd,e.strb);
    if(++b.index==b.beats){q.replies.push_back({b.id,cycle+11});q.writes.pop_front();}}
   if(e.ar){need(e.as<=3,"unsupported memory read size");q.reads.push_back({e.aa,e.al+1,0,e.ai,e.as,cycle+7});++q.ar_count;}
   if(e.aw){need(e.ws<=3,"unsupported memory write size");q.writes.push_back({e.wa,e.wl+1,0,e.wi,e.ws,cycle});++q.aw_count;}
  }
  ++cycle;
 }
 void idle(unsigned n){for(unsigned i=0;i<n;i++)tick();}
 void write(bool iommu,uint64_t address,uint64_t value,unsigned strobe=255){
  d.cfg_iommu_i=iommu;d.ctrl_aw_addr_i=address;d.ctrl_w_data_i=value;d.ctrl_w_strb_i=strobe;
  d.ctrl_aw_valid_i=d.ctrl_w_valid_i=1;d.ctrl_b_ready_i=0;
  bool aw=false,w=false;unsigned guard=0;
  while(!aw||!w){low();bool af=d.ctrl_aw_ready_o&&d.ctrl_aw_valid_i,wf=d.ctrl_w_ready_o&&d.ctrl_w_valid_i;tick();
   aw|=af;w|=wf;d.ctrl_aw_valid_i=!aw;d.ctrl_w_valid_i=!w;need(++guard<500,"MMIO AW/W timeout");}
  for(;;){low();if(d.ctrl_b_valid_o)break;tick();need(++guard<1000,"MMIO B timeout");}
  need(!d.ctrl_b_resp_o,"MMIO write bus error");d.ctrl_b_ready_i=1;tick();
 }
 uint64_t read(bool iommu,uint64_t address){
  d.cfg_iommu_i=iommu;d.ctrl_ar_addr_i=address;d.ctrl_ar_valid_i=1;d.ctrl_r_ready_i=0;unsigned guard=0;
  for(;;){low();bool accept=d.ctrl_ar_ready_o;tick();if(accept)break;need(++guard<500,"MMIO AR timeout");}
  d.ctrl_ar_valid_i=0;
  for(;;){low();if(d.ctrl_r_valid_o)break;tick();need(++guard<1000,"MMIO R timeout");}
  need(!d.ctrl_r_resp_o,"MMIO read bus error at "+std::to_string(address));auto value=d.ctrl_r_data_o;d.ctrl_r_ready_i=1;tick();return value;
 }
 uint64_t pn(unsigned index){write(false,0x70,index);return read(false,0x78);}
 void mode(unsigned value){write(false,0x70,193);write(false,0x78,value);}
 void drain(){
  unsigned stable=0;for(unsigned guard=0;stable<8;guard++){
   tick();bool empty=true;for(const auto& p:ports)empty&=p.reads.empty()&&p.writes.empty()&&p.replies.empty();
   stable=d.dataflow_quiet_o&&empty?stable+1:0;need(guard<80000,"real full-domain drain timeout");
  }
  for(const auto& p:ports)need(p.ar_count==p.r_last&&p.aw_count==p.b_count,"memory transaction conservation");
  need(!d.certificate_pending_o&&!d.permit_o,"final real certificates revoked");
 }
 void configure(unsigned lane,uint64_t src,uint64_t dst,unsigned copies=1,unsigned pages=1){
  uint64_t b=lane*0x1000;
  write(false,b+0x88,0);write(false,b,src);write(false,b+8,dst);write(false,b+0x10,4096);
  write(false,b+0x18,0);write(false,b+0x48,1);write(false,b+0x90,copies);write(false,b+0x98,pages);write(false,b+0x88,1);
 }
 void start(unsigned lane){(void)read(false,lane*0x1000+0x28);}
 void initialize_copy(uint64_t src,uint64_t dst){
  for(unsigned i=0;i<4096;i++){memory[src+i]=uint8_t((i*37)^(i>>3)^0xa5);memory[dst+i]=0xcc;}
  for(unsigned i=1;i<=16;i++){memory[dst-i]=0x35;memory[dst+4095+i]=0x7c;}
 }
 void check_copy(uint64_t src,uint64_t dst){
  for(unsigned i=0;i<4096;i++)need(memory[src+i]==memory[dst+i],"actual full-copy bytes differ");
  for(unsigned i=1;i<=16;i++)need(memory[dst-i]==0x35&&memory[dst+4095+i]==0x7c,"copy guard overwritten");
 }
 void report(const char* name){std::printf("{\"case\":\"%s\",\"status\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"cycle\":%llu}\n",name,(unsigned long long)cycle);std::fflush(stdout);}
 void setup_s1(){
  // Reuse A1's actual tl-quiet/tl_test.cpp setup() context and page-table words.
  // The full production program bridge receives DDTP, not a TL test input.
  for(unsigned did=10;did<=11;did++){
   uint64_t base=0x10000+64*did;for(unsigned j=0;j<8;j++)store(base+8*j,0);
   store(base,1);store(base+16,uint64_t(did)<<12);
   store(base+24,0x8000000000000000ULL+(did==10?0x20:0x30));
   store(base+32,0x1000000000000080ULL);store(base+40,255);store(base+48,0x24000);
  }
  store(0x20000,0x10001);store(0x40400,0x14001);store(0x30000,0x18001);store(0x60800,0x1c001);
  for(unsigned j=0;j<64;j++){
   store(0x50000+8*j,((0x80000ULL+j)<<10)|0xdf);
   store(0x70000+8*j,((0x90000ULL+j)<<10)|0xdf);
  }
  write(true,0x10,(0x10<<10)|2);need((read(true,0x10)&15)==2,"real MMIO one-level DDT");
 }
};

int main(){
 try{
  Bench b;
  need(b.pn(192)==0x49444d41504e0001ULL,"PN cap through real DMA0 window");
  need(b.pn(193)==0&&b.read(false,0xb0)==0,"actual reset S/gap0");
  // True MMIO DDTP Bare permits the first S data-path test.
  b.write(true,0x10,1);need((b.read(true,0x10)&15)==1,"actual DDTP Bare readback");b.drain();
  auto edges=b.clear_edges;b.mode(2);need(b.pn(193)==2&&b.clear_edges==edges+1,"atomic P MODE/clear binding");
  b.mode(6);need(b.pn(193)==6&&b.d.mode_word_o==6,"P_shared raw6 stays N0");
  b.mode(0);b.report("real_MMIO_mode_history_clear");
  b.initialize_copy(0x10000,0x40000);b.configure(0,0x10000,0x40000);b.start(0);
  b.mode(2);need(b.pn(193)==0&&(b.pn(194)&2),"busy MODE rejected without changing S");
  b.drain();need(b.read(false,0xa0)==1&&b.read(false,0xa8)==1,"batch actual completion/issue");
  b.check_copy(0x10000,0x40000);b.report("S_copy_real_mux_IOMMU_completion_and_drain");
  b.initialize_copy(0x20000,0x50000);b.configure(2,0x20000,0x50000);b.start(2);
  b.mode(2);need(b.pn(193)==0&&(b.pn(194)&2),"DMA2 real work blocks MODE");
  b.drain();b.check_copy(0x20000,0x50000);b.mode(2);need(b.pn(193)==2,"MODE accepts after natural DMA2 drain");
  b.report("four_port_quiet_DMA2_no_N_certificate");
  b.setup_s1();b.drain();b.mode(1);need(b.pn(193)==1,"real N selection through MODE");
  b.initialize_copy(0x80000000,0x80010000);
  for(unsigned j=0;j<8*4096;j++){b.memory[0x90000000+j]=uint8_t((j*29)^(j>>5)^0x6b);b.memory[0x90010000+j]=0xcc;}
  for(unsigned j=1;j<=16;j++){b.memory[0x90010000-j]=0x35;b.memory[0x90010000+8*4096-1+j]=0x7c;}
  b.configure(0,0x10000000,0x10010000);b.start(0);b.drain();
  b.configure(1,0x20000000,0x20010000);b.start(1);b.drain();
  b.check_copy(0x80000000,0x80010000);b.report("N_real_S1_warmup_and_natural_certificates");
  b.configure(0,0x10000000,0x10010000,4,1);b.configure(1,0x20000000,0x20010000,4096,8);
  b.write(false,0x68,4);b.write(false,0x60,1|(1ULL<<16)|(1ULL<<24)|(1ULL<<25)|(1ULL<<26));
  need(b.d.capture_busy_o,"actual accepted ARM reaches new capture");
  const unsigned v_done_before=b.dones[0];b.start(1);b.start(0);
  for(unsigned guard=0;b.dones[0]-v_done_before<4;guard++){b.tick();need(guard<100000,"V target completion timeout");}
  (void)b.read(false,0x30);b.idle(25);b.write(false,0x1088,2);b.drain();
  need(!b.d.capture_busy_o,"capture freezes on actual STOP/natural full-domain quiet");
  const auto flags=b.pn(199),aown=b.pn(197),whole=b.pn(198);
  need(flags==7,"capture frozen/Avalid/unionvalid without error flags");
  need(b.a_first!=UINT64_MAX&&b.u_first!=UINT64_MAX,"actual issue endpoints seen");
  need(aown==b.a_last-b.a_first&&whole==b.u_last-b.u_first,"hardware durations match actual issue/done edges");
  auto hardware_n10=b.pn(195),hardware_n11=b.pn(196);
  need(hardware_n10==b.n10&&hardware_n11==b.n11,
       "N count binding hardware="+std::to_string(hardware_n10)+","+std::to_string(hardware_n11)+
       " actual-window="+std::to_string(b.n10)+","+std::to_string(b.n11));
  need((b.read(false,0x60)>>32)==0,"original OBS/PATH validity preserved in real system");
  need(b.read(false,0xa0)==4&&b.read(false,0xa8)==4,"complete V work");
  auto a_completed=b.read(false,0x10a0),a_issued=b.read(false,0x10a8);
  need(a_completed>0&&a_completed==a_issued,"all real A work naturally completes after STOP");
  b.check_copy(0x80000000,0x80010000);
  for(unsigned page=0;page<8;page++)for(unsigned j=0;j<4096;j++)
   need(b.memory[0x90010000+page*4096+j]==(page<a_completed?b.memory[0x90000000+page*4096+j]:uint8_t(0xcc)),"A full-work page bytes");
  for(unsigned j=1;j<=16;j++)need(b.memory[0x90010000-j]==0x35&&b.memory[0x90010000+8*4096-1+j]==0x7c,"A region guards");
  b.idle(60);need(b.pn(197)==aown&&b.pn(198)==whole,"CPU quiet/read tail excluded from locked latest-done durations");
  b.report("actual_N_window_counters_and_Aown_union_STOP_freeze");
  std::printf("{\"status\":\"passed_targeted_peripheral_finite\",\"fixture_only\":true,\"source_board_cases\":0,\"N10\":%llu,\"N11\":%llu,\"Acompleted\":%llu,\"Aown\":%llu,\"union\":%llu}\n",(unsigned long long)b.n10,(unsigned long long)b.n11,(unsigned long long)a_completed,(unsigned long long)aown,(unsigned long long)whole);
  return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"FAIL: %s\n",e.what());return 1;}
}
