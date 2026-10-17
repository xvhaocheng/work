// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vhistory_n_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <vector>
static Vhistory_n_top t;
static FILE *trace,*records;
static unsigned cycle=0,checks=0,cases=0,ptw_reads=0,fills=0,accepts=0,faults=0,probes=0;
static int scenario=-1;
static const char* phase="reset";
static std::map<uint64_t,uint64_t> mem;
static void check(bool ok,const char*s){checks++;if(!ok){std::fprintf(stderr,"FAIL cycle=%u phase=%s %s\n",cycle,phase,s);std::exit(1);}}
struct Read{bool pending=false;uint64_t addr=0;unsigned id=0,len=0,beat=0,delay=0;};static Read d,p;
struct Resp{unsigned bank;bool error;uint64_t addr;unsigned code;};static std::vector<Resp> responses;
static bool request_fire=false,hold_ptw=false,hold_resp=false,hold_fault=false;
static uint64_t word(uint64_t a){auto it=mem.find(a);check(it!=mem.end(),"unmapped DS read");return it->second;}
static void evaluate(){t.eval();if(t.rst_ni){check(t.history_isolate_i?t.gold_equal_o:t.off_equal_o,"new TL history path differs from actual old755 local/shared TL");}}
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
 if(t.rst_ni){check(!t.msi_walk_o&&!t.msi_fill_o&&!t.resp_mrif_o,"restricted normal test unexpectedly started MSI walk/fill");
  if(t.n_probe_o){probes++;check(!t.ddwalk_init_o,"N cache probe started DDT walker");if(!t.n_accept_o&&!t.fill_o)check(t.probe_no_touch_o,"unaccepted N probe changed real cache PLRU");}
  check(t.n_touch_o==t.n_accept_o,"cache touch differs from true N accept");if(t.n_accept_o){accepts++;check(request_fire,"N accept without actual handshake");check(t.n_did_o==t.did_i,"N DID not actual accepted candidate");}
  if(t.fill_o){fills++;if(t.partition_i){unsigned mask=t.fill_did_o==10?3:0xfffc;check(t.fill_mask_o==mask,"TL P fill mask changed with history control");check(t.fill_victim_o&&!(t.fill_victim_o&(t.fill_victim_o-1))&&!(t.fill_victim_o&~mask),"TL P victim escaped partition");}}if(t.fault_valid_o&&t.fault_ready_i)faults++;
 }
 std::fprintf(trace,"%u,%s,%u,%u,%u,%u,%u,%u,%06x,%016llx,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%014llx,%u,%u,%u,%u,%u,%u",cycle,phase,t.rst_ni,t.compare_i,t.n_enable_i,t.req_valid_i,t.req_bank_i,t.type_i,t.did_i,(unsigned long long)t.iova_i,t.req_ready_o,t.n_window_o,t.n_state_o,t.n_probe_o,t.n_accept_o,t.n_touch_o,t.probe_no_touch_o,t.fill_o,t.resp_valid_o,t.resp_ready_i,t.resp_bank_o,(unsigned long long)t.resp_addr_o,t.resp_error_o,t.fault_code_o,t.fault_valid_o,t.fault_cause_o,pa,pr);
 std::fprintf(trace,",%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%016llx,%016llx,%u\n",scenario,t.req_pid_i,t.req_priv_i,t.req_debug_i,t.inval_i,t.dc_inval_i,t.ddt_mode_i,t.msi_walk_o,t.msi_fill_o,t.resp_mrif_o,da,dr,(unsigned long long)dad,(unsigned long long)pad,t.history_isolate_i);
 t.clk_i=1;evaluate();t.clk_i=0;evaluate();
 if(!t.rst_ni){d=Read{};p=Read{};}else{if(d.delay)d.delay--;if(p.delay)p.delay--;if(dr){if(d.beat==d.len)d.pending=false;else d.beat++;}if(pr){if(p.beat==p.len)p.pending=false;else p.beat++;}if(da){check(!d.pending,"DDT overlap");d={true,dad,did,dil,0,2};}if(pa){check(!p.pending,"PTW overlap");p={true,pad,pid,pil,0,3};ptw_reads++;}}
 cycle++;
}
static void reset(bool n){t.req_valid_i=0;t.req_bank_i=0;t.req_pid_i=t.req_priv_i=t.req_debug_i=0;t.clear_i=t.inval_i=t.dc_inval_i=0;t.ddt_mode_i=2;t.partition_i=0;t.n_enable_i=n;t.restricted_i=n;t.compare_i=!n;t.rst_ni=0;hold_ptw=hold_resp=hold_fault=false;tick();tick();t.n_enable_i=n;t.restricted_i=n;t.rst_ni=1;for(int i=0;i<5;i++)tick();responses.clear();}
static void offer(unsigned did,uint64_t a,unsigned bank=0,unsigned type=2){t.did_i=did;t.iova_i=a;t.req_bank_i=bank;t.type_i=type;t.req_valid_i=1;}
static void send(unsigned did,uint64_t a,unsigned bank=0,unsigned type=2){offer(did,a,bank,type);for(unsigned i=0;;i++){check(i<1000,"request accept timeout");tick();if(request_fire)break;}t.req_valid_i=0;}
static void wait_responses(unsigned n){for(unsigned i=0;responses.size()<n;i++){check(i<1000,"response timeout");tick();}}
static void response(unsigned index,unsigned bank,uint64_t addr,bool error=false,unsigned code=0){check(responses.size()>index,"response missing");auto r=responses[index];check(r.bank==bank,"wrong response bank");check(r.error==error,"wrong response error");if(error)check(r.code==code,"wrong fault code");else check(r.addr==addr,"wrong translated address");}
static void record(const char*label,unsigned ac,unsigned reads,unsigned fs){cases++;std::fprintf(records,"%u,%s,%s,%u,%u,%u,%u,%u,%d\n",cases,phase,label,cycle,accepts-ac,ptw_reads-reads,faults-fs,probes,scenario);}
static void translate(unsigned did,uint64_t a,uint64_t expected,unsigned bank=0,unsigned type=2){unsigned n=responses.size(),ac=accepts,rd=ptw_reads,fs=faults;send(did,a,bank,type);wait_responses(n+1);response(n,bank,expected);record("normal",ac,rd,fs);for(int i=0;i<3;i++)tick();}
static void setup(){for(unsigned did=10;did<=11;did++){uint64_t b=0x10000+64*did;for(unsigned i=0;i<8;i++)mem[b+8*i]=0;mem[b]=1;mem[b+16]=uint64_t(did)<<12;mem[b+24]=0x8000000000000000ULL+(did==10?0x20:0x30);mem[b+32]=0x1000000000000080ULL;mem[b+40]=255;mem[b+48]=0x24000;}
 mem[0x20000]=0x10001;mem[0x40400]=0x14001;mem[0x30000]=0x18001;mem[0x60800]=0x1c001;
 for(unsigned i=0;i<64;i++){mem[0x50000+8*i]=((0x80000ULL+i)<<10)|0xdf;mem[0x70000+8*i]=((0x90000ULL+i)<<10)|0xdf;}
 mem[0x50010]=(0x80002ULL<<10)|0xd3;mem[0x40c00]=(0xa0000ULL<<10)|0xdf;mem[0x20008]=(0xc0000ULL<<10)|0xdf;
}
static void warm(){translate(10,0x10000000,0x80000000);translate(10,0x10001000,0x80001000);translate(10,0x10002000,0x80002000);translate(10,0x30000000,0xa0000000);translate(11,0x20000000,0x90000000,1);responses.clear();}
static void cold_walk(){hold_ptw=true;send(11,0x20003000,1);for(unsigned i=0;!p.pending;i++){check(i<100,"PTW start timeout");tick();}for(int i=0;i<3;i++)tick();}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);check(argc==3,"need trace and records paths");trace=std::fopen(argv[1],"w");records=std::fopen(argv[2],"w");check(trace&&records,"output open");
 std::fprintf(trace,"cycle,phase,rst,compare,n_enable,valid,bank,type,did,iova,ready,window,n_state,probe,n_accept,touch,no_probe_touch,fill,resp_valid,resp_ready,resp_bank,spaddr,error,code,fq_valid,fq_cause,ptw_ar_fire,ptw_r_fire,scenario,pid_valid,priv,debug,iotinval,ddtc_inval,ddt_mode,msi_init_ar,msi_fill,mrif,ddt_ar_fire,ddt_r_fire,ddt_ar_addr,ptw_ar_addr,history_isolate\n");
 std::fprintf(records,"case,phase,label,end,n_accept,ptw_ar,fq,probe_total,scenario\n");
 for(unsigned iso=0;iso<2;iso++) {scenario=iso;phase=iso?"P-local-N-touch-old-gold":"P-shared-N-touch-old-whole-gold";setup();t.history_isolate_i=iso;reset(true);t.history_isolate_i=iso;t.partition_i=1;check(t.n_enable_i==1&&t.history_isolate_i==iso,"test controls lost after Verilator initialization");
  translate(10,0x10000000,0x80000000);translate(10,0x10000000,0x80000000);translate(10,0x10001000,0x80001000);translate(11,0x20000000,0x90000000,1);responses.clear();
  cold_walk();unsigned ac=accepts,rd=ptw_reads,fs=faults;hold_resp=true;send(10,0x10000000);check(accepts==ac+1,"true N hit not accepted under P history");hold_ptw=false;
  for(int i=0;i<50;i++){tick();check(t.resp_valid_o&&!t.resp_error_o&&t.resp_addr_o==0x80000000&&t.resp_bank_o==0,"held N response changed with slow completion");}
  hold_resp=false;wait_responses(2);response(0,0,0x80000000);response(1,1,0x90003000);record("true-accept-touch-and-held-response",ac,rd,fs);
  check(!d.pending&&!p.pending,"walker not drained");
  // Same mode remains fixed through each run; switching follows reset/clear.
 }
 scenario=6;phase="P-shared-runtime-N0-old-whole-gold";setup();reset(false);t.history_isolate_i=0;t.partition_i=1;
 unsigned ac0=accepts;translate(10,0x10000000,0x80000000);translate(10,0x10000000,0x80000000);translate(10,0x10001000,0x80001000);translate(11,0x20000000,0x90000000,1);check(accepts==ac0,"P_shared runtimeN0 activated fast path");
 std::fclose(trace);std::fclose(records);std::printf("PASS history_n cycles=%u cases=%u checks=%u actual_n_accepts=%u probes=%u; actual TL/P/N cache touch local/shared vs old755; no backend/board\n",cycle,cases,checks,accepts,probes);t.final();}
