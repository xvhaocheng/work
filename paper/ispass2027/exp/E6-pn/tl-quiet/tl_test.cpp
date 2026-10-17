// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vtl_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <deque>
#include <vector>
static Vtl_top t;static FILE*trace,*records;static unsigned cyc=0,evs=0,checks=0,cases=0;
static const char*phase="reset";
static std::map<uint64_t,uint64_t> mem;
struct Read{uint64_t addr;unsigned id,len,beat,delay;};static std::deque<Read>q;
struct Result{bool error,ignore;uint64_t addr;unsigned code;};static std::vector<Result>rsp;
struct Debug{bool error;uint64_t ppn;};static std::vector<Debug>dbg;
static bool reqfire=false,dbgfire=false,hold_pt=false,hold_pd=false,hold_dd=false,hold_tail=false;
static unsigned ar_count[16]={},fq=0,msfills=0,mrfills=0,accepts=0,pdfills=0;
static void ck(bool x,const char*s){checks++;if(!x){fprintf(stderr,"FAIL cycle=%u phase=%s %s\n",cyc,phase,s);exit(1);}}
static uint64_t word(uint64_t a){auto it=mem.find(a);if(it==mem.end()){fprintf(stderr,"unmapped DS addr=%llx\n",(unsigned long long)a);ck(false,"unmapped DS model");}return it->second;}
static void ev(){t.eval();evs++;ck(t.equal_off_o,"Pn0/Pn1 legacy complete interfaces differ");ck(t.equal_old_o,"old6082 chosen original-source reference differs");if(t.compare85_i)ck(t.equal85_o,"N0 raw85b legacy complete interfaces differ");ck(!t.off_idle_o&&!t.branches_off_o,"defaultoff quiet not0");if(t.rst_ni){ck(t.branches_idle_o&&t.bt_only_idle_o,"noPC/noMSI/BTonly factor notidle");ck(!t.ds_bad_o,"actual DS debt fault");if(t.pn_idle_o)ck(t.policy_quiet_o&&!t.dd_active_o&&!t.pd_active_o&&!t.pt_active_o&&!t.n_busy_o&&!t.n_accept_o&&!t.msi_init_o&&!t.ddt_update_o&&!t.pdt_update_o&&!t.iotlb_update_o&&!t.mrif_update_o&&t.msi_idle_o,"pn quiet with actual walker/update stock");}
 fprintf(trace,"%u,%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%llx,%llx\n",cyc,evs,phase,t.clk_i,t.rst_ni,t.n_enable_i,t.restricted_i,t.gold_shared_debug_i,t.compare85_i,t.equal_old_o,t.equal_off_o,t.equal85_o,t.pn_idle_o,t.policy_quiet_o,t.ds_quiet_o,t.ds_buffers_o,t.ds_debt_o,t.msi_idle_o,t.dd_active_o,t.pd_active_o,t.pt_active_o,t.n_busy_o,t.n_accept_o,t.ddt_update_o,t.pdt_update_o,t.iotlb_update_o,t.mrif_update_o,t.msi_init_o,t.state_o,t.bt_state_o,t.mrif_state_o,t.stable_debug_o,t.stable_type_o,t.restricted_msi_o,t.stable_ats_o,t.stable_msg_o,t.req_valid_i,t.req_debug_i,t.type_i,t.req_ready_o,t.resp_valid_o,t.resp_ready_i,t.resp_error_o,t.resp_code_o,t.debug_req_valid_o,t.debug_req_ready_o,t.debug_valid_o,t.debug_error_o,t.fault_valid_o,t.fault_ready_i,t.fault_code_o,t.ds_ar_valid_o,t.ds_ar_ready_i,t.ds_ar_id_o,t.ds_ar_len_o,t.ds_r_valid_i,t.ds_r_last_i,(unsigned long long)t.ds_ar_addr_o,(unsigned long long)t.ds_r_data_i);
}
static void tick(){
 t.ds_ar_ready_i=q.size()<4&&cyc%5!=0;t.ds_r_valid_i=0;
 if(!q.empty()){
  auto&r=q.front();bool held=(hold_dd&&r.id==1)||(hold_pd&&r.id==2)||(hold_pt&&r.id==0)||(hold_tail&&r.id==4&&r.beat==r.len&&r.len>0);
  if(!r.delay&&!held){t.ds_r_valid_i=1;t.ds_r_id_i=r.id;t.ds_r_last_i=r.beat==r.len;t.ds_r_data_i=word(r.addr+8*r.beat);t.ds_r_resp_i=0;}
 }
 t.clk_i=0;ev();bool a=t.ds_ar_valid_o&&t.ds_ar_ready_i,r=t.ds_r_valid_i&&t.ds_r_ready_o;uint64_t ad=t.ds_ar_addr_o;unsigned id=t.ds_ar_id_o,len=t.ds_ar_len_o;
 reqfire=t.req_valid_i&&t.req_ready_o;dbgfire=t.debug_req_valid_o&&t.debug_req_ready_o;
 if(t.rst_ni){if(t.resp_valid_o&&t.resp_ready_i)rsp.push_back({bool(t.resp_error_o),bool(t.resp_ignore_o),t.resp_addr_o,t.resp_code_o});if(t.debug_valid_o)dbg.push_back({bool(t.debug_error_o),t.debug_ppn_o});if(t.fault_valid_o&&t.fault_ready_i)fq++;msfills+=t.msi_iotlb_fill_o;mrfills+=t.mrif_update_o;pdfills+=t.pdt_update_o;accepts+=t.n_accept_o;}
 t.clk_i=1;ev();t.clk_i=0;ev();
 if(!t.rst_ni)q.clear();else{for(auto&x:q)if(x.delay)x.delay--;if(r){if(q.front().beat==q.front().len)q.pop_front();else q.front().beat++;}if(a){q.push_back({ad,id,len,0,3});ar_count[id]++;}}
 cyc++;
}
static void idle(unsigned n=5){while(n--)tick();}
template<class F>static void until(F f,const char*s){for(unsigned n=0;!f();n++){ck(n<1200,s);tick();}}
static void mark(const char*s){phase=s;cases++;fprintf(records,"%u,%s,%u\n",cases,s,cyc);}
static void reset(bool n=false,bool shared=false){t.rst_ni=0;t.n_enable_i=n;t.restricted_i=n;t.gold_shared_debug_i=shared;t.compare85_i=!n;t.req_valid_i=t.req_bank_i=t.req_pid_i=t.req_priv_i=t.req_debug_i=0;t.pid_i=0;t.did_i=10;t.iova_i=0;t.type_i=2;t.resp_ready_i=t.fault_ready_i=1;t.dbg_go_i=t.dbg_read_i=0;t.dbg_iova_i=0x24000000;t.dbg_did_i=10;t.inval_i=t.dc_inval_i=t.clear_i=0;t.ds_r_valid_i=0;t.ds_r_last_i=0;t.ds_r_id_i=0;t.ds_r_data_i=0;t.ds_r_resp_i=0;hold_pt=hold_pd=hold_dd=hold_tail=false;idle(2);t.rst_ni=1;idle(5);rsp.clear();dbg.clear();ck(t.pn_idle_o&&t.ds_quiet_o,"reset completequiet missing");}
static void send(unsigned did,uint64_t iova,unsigned type=2,unsigned bank=0){t.did_i=did;t.iova_i=iova;t.type_i=type;t.req_bank_i=bank;t.req_valid_i=1;for(unsigned n=0;;n++){ck(n<1000,"TRrequest timeout");tick();if(reqfire)break;}t.req_valid_i=0;}
static void sendDebug(){t.dbg_go_i=1;for(unsigned n=0;;n++){ck(n<1000,"realdebugGO accept timeout");tick();if(dbgfire)break;}t.dbg_go_i=0;}
static void drain(){until([](){return t.pn_idle_o&&t.ds_quiet_o&&q.empty();},"complete transport/lifecycle drain timeout");idle(3);}
static void setup(){mem.clear();for(unsigned did=10;did<=11;did++){uint64_t b=0x10000+64*did;for(unsigned i=0;i<8;i++)mem[b+8*i]=0;mem[b]=1;mem[b+16]=uint64_t(did)<<12;mem[b+24]=0x8000000000000000ULL+(did==10?0x20:0x30);mem[b+32]=0x1000000000000080ULL;mem[b+40]=255;mem[b+48]=0x24000;}
 mem[0x20000]=0x10001;mem[0x40400]=0x14001;mem[0x30000]=0x18001;mem[0x60800]=0x1c001;for(unsigned i=0;i<64;i++){mem[0x50000+8*i]=((0x80000ULL+i)<<10)|0xdf;mem[0x70000+8*i]=((0x90000ULL+i)<<10)|0xdf;}}
static void msiSetup(unsigned dtf=0,unsigned kind=0){setup();uint64_t b=0x10000+64*10;mem[b]=1+(dtf?16:0);mem[b+8]=0x8000000000000040ULL;mem[b+24]=0;mem[0x80000]=kind==0?((0xb0000ULL<<10)|7):kind==1?((0x400ULL<<7)|3):0;mem[0x80008]=(0xc0000ULL<<10)|27;}
static void result(unsigned n,uint64_t addr){ck(rsp.size()>n&&!rsp[n].error&&rsp[n].addr==addr,"normal translation result");}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);ck(argc==3,"trace/cases args");trace=fopen(argv[1],"w");records=fopen(argv[2],"w");ck(trace&&records,"outputopen");fprintf(trace,"cycle,eval,phase,clk,rst,n_enable,restricted,gold_shared,compare85,equal_old,equal_off,equal85,pn_idle,policy_quiet,ds_quiet,ds_buffers,ds_debt,msi_idle,dd_active,pd_active,pt_active,n_busy,n_accept,dd_update,pd_update,io_update,mrif_update,msi_init,state,bt_state,mrif_state,stable_debug,stable_type,restricted_msi,stable_ats,stable_msg,reqvalid,current_debug,current_type,ready,respvalid,respready,resperror,respcode,dbg_reqvalid,dbg_reqready,dbg_valid,dbg_error,fqvalid,fqready,fqcode,arvalid,arready,arid,arlen,rvalid,rlast,araddr,rdata\n");fprintf(records,"case,phase,start_cycle\n");
 mark("N0-original85b-DDT-PTW");setup();reset();send(10,0x10000000);until([](){return rsp.size()==1;},"normalrsp");result(0,0x80000000);drain();send(11,0x20001000);until([](){return rsp.size()==2;},"normalrsp2");result(1,0x90001000);drain();
 mark("N0-original85b-prefetch-and-held-response");setup();reset();hold_pt=true;send(11,0x20003000);until([](){return t.pt_active_o&&!q.empty()&&q.front().id==0;},"realPTW start");unsigned begin=cyc;send(10,0x10000000);ck(cyc-begin<5,"N0 original prefetch serialized");idle(16);ck(!t.pn_idle_o,"PTW quiet whileheld");t.resp_ready_i=0;hold_pt=false;until([](){return t.resp_valid_o;},"heldnormalresponse");idle(12);ck(!t.pn_idle_o,"heldresponsequiet");t.resp_ready_i=1;until([](){return rsp.size()==2;},"two prefetched results");result(0,0x90003000);result(1,0x80000000);drain();
 mark("real-PDT-active-original-no-update-nonquiet");setup();mem[0x10000+640]=33;mem[0x10000+640+24]=0x10000000000000a0ULL;mem[0xa0030]=(12ull<<12)|1;mem[0xa0038]=0x8000000000000020ULL;reset();t.req_pid_i=1;t.pid_i=3;hold_pd=true;send(10,0x10000000);until([](){return t.pd_active_o;},"realPDTactive");idle(16);ck(!t.pn_idle_o,"PDTactivegap quiet");unsigned pd0=pdfills;hold_pd=false;until([](){return !t.pd_active_o&&q.empty()&&t.ds_quiet_o;},"originalPDT returned memory");idle(12);ck(rsp.empty()&&pdfills==pd0&&!t.pn_idle_o&&t.state_o==2,"baseline no-update PDT behavior changed");
 mark("N1-real-helper-held-response-quiet");setup();reset(true);send(10,0x10000000);until([](){return rsp.size()==1;},"warmV");drain();send(11,0x20000000,2,1);until([](){return rsp.size()==2;},"warmA");drain();hold_pt=true;send(11,0x20003000,2,1);until([](){return t.pt_active_o&&!q.empty()&&q.front().id==0;},"coldA PTW");unsigned acc=accepts;t.resp_ready_i=0;send(10,0x10000000);until([](){return t.resp_valid_o;},"actualNfastresponse");idle(16);ck(accepts==acc+1&&t.n_busy_o&&!t.pn_idle_o,"N trueaccept/heldbusy notguarded");t.resp_ready_i=1;idle(5);hold_pt=false;until([](){return rsp.size()==4;},"NandAresults");result(2,0x80000000);result(3,0x90003000);drain();
 mark("N1-real-debug-GO-BT-stable-source-and-tailgap");msiSetup();reset(true,true);hold_tail=true;unsigned ms0=msfills;sendDebug();until([](){return dbg.size()==1;},"realdebugBT result");ck(!dbg[0].error&&dbg[0].ppn==0xb0000&&msfills==ms0+1,"debugBT bypassed original MSI translation");until([](){return t.pn_idle_o&&t.ds_buffers_o;},"debugBT localquiet");ck(q.size()==1&&q.front().id==4&&q.front().beat==1,"actualMSIPTW two-beat tail notpending");
 for(unsigned n=0;n<24;n++){t.req_debug_i=n&1;t.type_i=n%3==0?8:3;ck(t.stable_debug_o&&t.msi_idle_o&&t.pn_idle_o&&!t.restricted_msi_o&&t.ds_debt_o==1&&!t.ds_quiet_o,"stabledebug/localidle vs DS tail debt");tick();}t.req_debug_i=0;t.type_i=2;hold_tail=false;drain();
 mark("N1-debug-MRIF-original-fault-semantics");msiSetup(0,1);reset(true,true);t.fault_ready_i=0;unsigned mf0=mrfills;sendDebug();until([](){return t.fault_valid_o;},"debugMRIF originalfault");idle(12);ck(t.fault_code_o==260&&!t.pn_idle_o&&mrfills==mf0+1,"debugMRIF original ERROR notpreserved");t.fault_ready_i=1;until([](){return dbg.size()==1;},"debugMRIFresult");ck(dbg[0].error,"debugMRIF incorrectlysucceeded");drain();
 mark("N0-MRIF-unaligned-ignore-localidle-tail-debt");msiSetup(0,1);reset();hold_tail=true;unsigned mf1=mrfills;send(10,0x24000004,3);until([](){return rsp.size()==1;},"realMRIFignore");ck(!rsp[0].error&&rsp[0].ignore&&mrfills==mf1,"MRIF unaligned notoriginalignore");until([](){return t.pn_idle_o&&t.ds_buffers_o;},"ignorelocalidle");for(unsigned n=0;n<24;n++){ck(t.ds_debt_o==1&&!t.ds_quiet_o,"ignoredMRIF lost taildebt");tick();}hold_tail=false;drain();
 mark("N0-MSI-invalid-PTE-error-waits-tail-and-held-fault");msiSetup(0,2);reset();hold_tail=true;t.fault_ready_i=0;send(10,0x24000000,3);until([](){return t.bt_state_o==3;},"realBTerror");idle(20);ck(!t.pn_idle_o&&rsp.empty()&&!t.fault_valid_o,"BTerror didnotwaittail");hold_tail=false;until([](){return t.fault_valid_o;},"realMSIinvalidfault");idle(12);ck(t.fault_code_o==262&&!t.pn_idle_o,"MSIPTEinvalid fault/quiet");t.fault_ready_i=1;until([](){return rsp.size()==1;},"invalidresult");ck(rsp[0].error&&rsp[0].code==262,"invalidPTEresult");drain();
 for(unsigned dtf=0;dtf<2;dtf++){
  mark(dtf?"N1-normal-restricted-MSI-DTF1-candidate-fake-debug":"N1-normal-restricted-MSI-DTF0-candidate-fake-debug");msiSetup(dtf);reset(true);t.fault_ready_i=0;unsigned a0=ar_count[4],m0=msfills,f0=fq;send(10,0x24000000,3);t.req_debug_i=1;t.type_i=8;
  if(!dtf){until([](){return t.fault_valid_o;},"restricted FQ");for(unsigned n=0;n<16;n++){ck(t.fault_code_o==260&&t.fault_did_o==10&&!t.stable_debug_o&&t.stable_type_o==3&&t.restricted_msi_o&&!t.msi_init_o,"current fake debug exempted savednormal");tick();}}
  else until([](){return t.resp_valid_o;},"DTF1errorresult");
  t.fault_ready_i=1;until([](){return rsp.size()==1;},"restricted normalresponse");ck(rsp[0].error&&rsp[0].code==260&&ar_count[4]==a0&&msfills==m0&&fq-f0==(dtf?0:1),"normal restricted MSI error/FQ/no walk/fill contract");t.req_debug_i=0;t.type_i=2;drain();
 }
 for(unsigned ty: {8u,9u}){
  mark(ty==8?"ATS0-ttype8-stable-origin-fixture-only":"ATS0-ttype9-stable-origin-fixture-only");setup();reset(true,true);send(10,0x10000000,ty);bool seen=false;for(unsigned n=0;n<300;n++){if(t.stable_type_o==ty&&t.state_o){seen=true;ck(!t.restricted_msi_o,"stable ATS/message source incorrectlyrestricted");}tick();if(n>10&&t.pn_idle_o&&t.ds_quiet_o)break;}ck(seen&&t.pn_idle_o&&t.ds_quiet_o,"ATS enumfixture notobserved/drained");
 }
 mark("policy-clear-does-not-self-disqualify");setup();reset(true);t.clear_i=1;idle(2);ck(t.pn_idle_o&&t.policy_quiet_o,"policyclear self-blocked quiet");t.clear_i=0;drain();
 fprintf(stdout,"PASS tl cases=%u cycles=%u evals=%u checks=%u DDT_AR=%u PDT_AR=%u PTW_AR=%u MSI_AR=%u MSI_fill=%u MRIF_fill=%u PDT_fill=%u Naccept=%u fault=%u; actual TL/walkers/DS/debug source; ATS fixtures0board; no system/FPGA/board\n",cases,cyc,evs,checks,ar_count[1],ar_count[2],ar_count[0],ar_count[4],msfills,mrfills,pdfills,accepts,fq);fclose(trace);fclose(records);return 0;
}
