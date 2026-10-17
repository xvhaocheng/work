// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vn_tl_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <vector>
static Vn_tl_top t;
static FILE *trace,*records;
static unsigned cycle=0,checks=0,cases=0,ptw_reads=0,fills=0,accepts=0,faults=0,probes=0;
static const char* phase="reset";
static std::map<uint64_t,uint64_t> mem;
static void check(bool ok,const char*s){checks++;if(!ok){std::fprintf(stderr,"FAIL cycle=%u phase=%s %s\n",cycle,phase,s);std::exit(1);}}
struct Read{bool pending=false;uint64_t addr=0;unsigned id=0,len=0,beat=0,delay=0;};static Read d,p;
struct Resp{unsigned bank;bool error;uint64_t addr;unsigned code;};static std::vector<Resp> responses;
static bool request_fire=false,hold_ptw=false,hold_resp=false,hold_fault=false;
static uint64_t word(uint64_t a){auto it=mem.find(a);check(it!=mem.end(),"unmapped DS read");return it->second;}
static void evaluate(){t.eval();if(t.rst_ni){check(t.off_equal_o,"compile-disabled TL differs from raw85b gold");if(t.compare_i)check(t.gold_equal_o,"runtime N0/fullmask differs from raw85b gold");}}
static void tick(){
 t.ddt_ar_ready_i=!d.pending&&cycle%5!=0;t.ptw_ar_ready_i=!p.pending&&cycle%7!=0;
 t.ddt_r_valid_i=d.pending&&!d.delay;t.ddt_r_data_i=t.ddt_r_valid_i?word(d.addr+8*d.beat):0;t.ddt_r_id_i=d.id;t.ddt_r_last_i=d.beat==d.len;
 t.ptw_r_valid_i=p.pending&&!p.delay&&!hold_ptw;t.ptw_r_data_i=t.ptw_r_valid_i?word(p.addr+8*p.beat):0;t.ptw_r_id_i=p.id;t.ptw_r_last_i=p.beat==p.len;
 t.resp_ready_i=!hold_resp&&cycle%6!=0;t.fault_ready_i=!hold_fault;
 t.clk_i=0;evaluate();
 bool da=t.ddt_ar_valid_o&&t.ddt_ar_ready_i,pa=t.ptw_ar_valid_o&&t.ptw_ar_ready_i,dr=t.ddt_r_valid_i&&t.ddt_r_ready_o,pr=t.ptw_r_valid_i&&t.ptw_r_ready_o;
 uint64_t dad=t.ddt_ar_addr_o,pad=t.ptw_ar_addr_o;unsigned dil=t.ddt_ar_len_o,pil=t.ptw_ar_len_o,did=t.ddt_ar_id_o,pid=t.ptw_ar_id_o;
 request_fire=t.req_valid_i&&t.req_ready_o;
 if(t.resp_valid_o&&t.resp_ready_i)responses.push_back({t.resp_bank_o,bool(t.resp_error_o),t.resp_addr_o,t.fault_code_o});
 if(t.rst_ni){check(!t.msi_walk_o&&!t.msi_fill_o,"restricted normal test unexpectedly started MSI walk/fill");
  if(t.n_probe_o){probes++;check(!t.ddwalk_init_o,"N cache probe started DDT walker");if(!t.n_accept_o&&!t.fill_o)check(t.probe_no_touch_o,"unaccepted N probe changed real cache PLRU");}
  check(t.n_touch_o==t.n_accept_o,"cache touch differs from true N accept");if(t.n_accept_o){accepts++;check(request_fire,"N accept without actual handshake");check(t.n_did_o==t.did_i,"N DID not actual accepted candidate");}
  if(t.fill_o)fills++;if(t.fault_valid_o&&t.fault_ready_i)faults++;
 }
 std::fprintf(trace,"%u,%s,%u,%u,%u,%u,%u,%u,%06x,%016llx,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%014llx,%u,%u,%u,%u,%u,%u\n",cycle,phase,t.rst_ni,t.compare_i,t.n_enable_i,t.req_valid_i,t.req_bank_i,t.type_i,t.did_i,(unsigned long long)t.iova_i,t.req_ready_o,t.n_window_o,t.n_state_o,t.n_probe_o,t.n_accept_o,t.n_touch_o,t.probe_no_touch_o,t.fill_o,t.resp_valid_o,t.resp_ready_i,t.resp_bank_o,(unsigned long long)t.resp_addr_o,t.resp_error_o,t.fault_code_o,t.fault_valid_o,t.fault_cause_o,pa,pr);
 t.clk_i=1;evaluate();t.clk_i=0;evaluate();
 if(!t.rst_ni){d=Read{};p=Read{};}else{if(d.delay)d.delay--;if(p.delay)p.delay--;if(dr){if(d.beat==d.len)d.pending=false;else d.beat++;}if(pr){if(p.beat==p.len)p.pending=false;else p.beat++;}if(da){check(!d.pending,"DDT overlap");d={true,dad,did,dil,0,2};}if(pa){check(!p.pending,"PTW overlap");p={true,pad,pid,pil,0,3};ptw_reads++;}}
 cycle++;
}
static void reset(bool n){t.req_valid_i=0;t.req_bank_i=0;t.req_pid_i=t.req_priv_i=t.req_debug_i=0;t.clear_i=t.inval_i=t.dc_inval_i=0;t.ddt_mode_i=2;t.partition_i=0;t.n_enable_i=n;t.restricted_i=n;t.compare_i=!n;t.rst_ni=0;hold_ptw=hold_resp=hold_fault=false;tick();tick();t.rst_ni=1;for(int i=0;i<5;i++)tick();responses.clear();}
static void offer(unsigned did,uint64_t a,unsigned bank=0,unsigned type=2){t.did_i=did;t.iova_i=a;t.req_bank_i=bank;t.type_i=type;t.req_valid_i=1;}
static void send(unsigned did,uint64_t a,unsigned bank=0,unsigned type=2){offer(did,a,bank,type);for(unsigned i=0;;i++){check(i<1000,"request accept timeout");tick();if(request_fire)break;}t.req_valid_i=0;}
static void wait_responses(unsigned n){for(unsigned i=0;responses.size()<n;i++){check(i<1000,"response timeout");tick();}}
static void response(unsigned index,unsigned bank,uint64_t addr,bool error=false,unsigned code=0){check(responses.size()>index,"response missing");auto r=responses[index];check(r.bank==bank,"wrong response bank");check(r.error==error,"wrong response error");if(error)check(r.code==code,"wrong fault code");else check(r.addr==addr,"wrong translated address");}
static void record(const char*label,unsigned ac,unsigned reads,unsigned fs){cases++;std::fprintf(records,"%u,%s,%s,%u,%u,%u,%u,%u\n",cases,phase,label,cycle,accepts-ac,ptw_reads-reads,faults-fs,probes);}
static void translate(unsigned did,uint64_t a,uint64_t expected,unsigned bank=0,unsigned type=2){unsigned n=responses.size(),ac=accepts,rd=ptw_reads,fs=faults;send(did,a,bank,type);wait_responses(n+1);response(n,bank,expected);record("normal",ac,rd,fs);for(int i=0;i<3;i++)tick();}
static void setup(){for(unsigned did=10;did<=11;did++){uint64_t b=0x10000+64*did;for(unsigned i=0;i<8;i++)mem[b+8*i]=0;mem[b]=1;mem[b+16]=uint64_t(did)<<12;mem[b+24]=0x8000000000000000ULL+(did==10?0x20:0x30);mem[b+32]=0x1000000000000080ULL;mem[b+40]=255;mem[b+48]=0x24000;}
 mem[0x20000]=0x10001;mem[0x40400]=0x14001;mem[0x30000]=0x18001;mem[0x60800]=0x1c001;
 for(unsigned i=0;i<64;i++){mem[0x50000+8*i]=((0x80000ULL+i)<<10)|0xdf;mem[0x70000+8*i]=((0x90000ULL+i)<<10)|0xdf;}
 mem[0x50010]=(0x80002ULL<<10)|0xd3;mem[0x40c00]=(0xa0000ULL<<10)|0xdf;mem[0x20008]=(0xc0000ULL<<10)|0xdf;
}
static void warm(){translate(10,0x10000000,0x80000000);translate(10,0x10001000,0x80001000);translate(10,0x10002000,0x80002000);translate(10,0x30000000,0xa0000000);translate(11,0x20000000,0x90000000,1);responses.clear();}
static void cold_walk(){hold_ptw=true;send(11,0x20003000,1);for(unsigned i=0;!p.pending;i++){check(i<100,"PTW start timeout");tick();}for(int i=0;i<3;i++)tick();}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);check(argc==3,"need trace and records paths");trace=std::fopen(argv[1],"w");records=std::fopen(argv[2],"w");check(trace&&records,"output open");std::fprintf(trace,"cycle,phase,rst,compare,n_enable,valid,bank,type,did,iova,ready,window,n_state,probe,n_accept,touch,no_probe_touch,fill,resp_valid,resp_ready,resp_bank,spaddr,error,code,fq_valid,fq_cause,ptw_ar_fire,ptw_r_fire\n");std::fprintf(records,"case,phase,label,end,n_accept,ptw_ar,fq,probe_total\n");setup();reset(false);phase="N0-gold";
 for(unsigned i=0;i<24;i++){translate(11,0x20000000ULL+4096*(i%16),0x90000000ULL+4096*(i%16));translate(10,0x10000000ULL+(i%2)*4096,0x80000000ULL+(i%2)*4096);}
 phase="N0-prefetch-spill-gold";reset(false);hold_ptw=true;send(11,0x20003000);for(unsigned i=0;!p.pending;i++){check(i<100,"N0 PTW start");tick();}unsigned before=cycle;send(10,0x10000000);check(cycle-before<5,"N0 original spill prefetch serialized");check(responses.empty(),"held PTW unexpectedly completed");hold_ptw=false;wait_responses(2);response(0,0,0x90003000);response(1,0,0x80000000);record("prefetched-while-PTW",accepts,ptw_reads,faults);
 phase="N-real-hit-held-response";reset(true);warm();cold_walk();unsigned ac=accepts,rd=ptw_reads,fs=faults;hold_resp=true;send(10,0x10000000);check(accepts==ac+1,"hot request not true N accept");for(int i=0;i<12;i++){tick();check(t.resp_valid_o&&!t.resp_error_o&&t.resp_addr_o==0x80000000&&t.resp_bank_o==0,"held N response changed");}check(responses.empty(),"held response consumed");hold_resp=false;wait_responses(1);response(0,0,0x80000000);check(p.pending,"A PTW not held during V hit");hold_ptw=false;wait_responses(2);response(1,1,0x90003000);record("true-hit-no-probe-touch-held",ac,rd,fs);
 phase="N-two-consecutive-hits";reset(true);warm();cold_walk();ac=accepts;rd=ptw_reads;fs=faults;send(10,0x10000000);wait_responses(1);send(10,0x10001000);wait_responses(2);check(accepts==ac+2,"two hot requests must accept exactly twice");response(0,0,0x80000000);response(1,0,0x80001000);hold_ptw=false;wait_responses(3);response(2,1,0x90003000);record("two-true-hits",ac,rd,fs);
 // Permission, context, large-leaf and candidate exclusions are observed against real caches.
 for(unsigned kind=0;kind<8;kind++){phase="N-real-exclusion";reset(true);warm();cold_walk();ac=accepts;rd=ptw_reads;fs=faults;
  uint64_t a=kind==0?0x10002000:kind==1?0x30000000:kind==2?0x10004000:0x10000000;
  offer(10,a,0,kind==0?3:2);if(kind==3)t.req_pid_i=1;if(kind==4)t.req_priv_i=1;if(kind==5)t.req_debug_i=1;
  for(unsigned i=0;i<8;i++){if(kind==6&&i==1)t.iova_i=0x10001000;if(kind==7&&i==1)t.dc_inval_i=1;tick();if(kind==7)t.dc_inval_i=0;check(!request_fire&&accepts==ac,"excluded request accepted as N");}
  t.req_valid_i=0;t.req_pid_i=t.req_priv_i=t.req_debug_i=0;for(int i=0;i<2;i++)tick();hold_ptw=false;wait_responses(1);response(0,1,0x90003000);record("cache-exclusion-unaccepted",ac,rd,fs);
 }
 // Runtime restricted MSI rejection uses the real TL ERROR/FQ path, not a mocked response.
 for(unsigned dtf=0;dtf<2;dtf++){phase=dtf?"N-MSI-DTF1":"N-MSI-DTF0";mem[0x10000+64*10]=1+(dtf?16:0);mem[0x50018]=(0x24000ULL<<10)|0xdf;reset(true);unsigned ac0=accepts,rd0=ptw_reads,fs0=faults,f0=fills;hold_fault=true;send(10,0x10003000,0,3);
  if(!dtf){for(unsigned i=0;!t.fault_valid_o;i++){check(i<100,"MSI fault did not reach FQ");tick();}for(int i=0;i<9;i++){check(t.fault_valid_o&&t.fault_cause_o==260&&t.fault_did_o==10&&t.fault_iova_o==0x10003000,"held MSI FQ record changed");tick();check(responses.empty(),"error response bypassed backpressured FQ");}}
  hold_fault=false;wait_responses(1);response(0,0,0,true,260);check(faults-fs0==(dtf?0:1),"DTF FQ count mismatch");check(ptw_reads==rd0+3&&fills==f0&&accepts==ac0,"restricted MSI unexpected S1 walk count/fill/N accept");record("real-error-FQ-no-walk-no-fill",ac0,rd0,fs0);
 }
 check(!d.pending&&!p.pending,"walk responses not drained");std::fclose(trace);std::fclose(records);std::printf("PASS n_tl cycles=%u cases=%u checks=%u true_accepts=%u probes=%u fills=%u ptw_ar=%u faults=%u; real TL/cache/walkers; no banked/backend/certificate/board\n",cycle,cases,checks,accepts,probes,fills,ptw_reads,faults);t.final();}
