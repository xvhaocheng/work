// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vp_tl_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <map>

static Vp_tl_top t;
static FILE *trace,*records;
static unsigned cycle=0,checks=0,cases=0,ptw_reads=0,fills=0;
static const char* phase="reset";
static std::map<uint64_t,uint64_t> mem;
static void check(bool ok,const char* s){checks++;if(!ok){std::fprintf(stderr,"FAIL cycle=%u phase=%s %s\n",cycle,phase,s);std::exit(1);}}
struct Read {bool pending=false;uint64_t addr=0;unsigned id=0,len=0,beat=0,delay=0;};
static Read d,p;
static bool request_fire=false,response_fire=false;
static uint64_t response_addr=0;
static bool response_error=false;
static unsigned response_code=0;
static uint64_t word(uint64_t addr){auto it=mem.find(addr);check(it!=mem.end(),"unmapped DS read");return it->second;}
static void evaluate(){
    t.eval();
    if(t.rst_ni){check(t.off_equal_o,"disabled P TL differs from byte-frozen 85b gold");
        if(t.compare_i)check(t.gold_equal_o,"runtime full-mask TL differs from 85b gold");}
}
static void tick(){
    t.ddt_ar_ready_i=!d.pending && cycle%5!=0;
    t.ptw_ar_ready_i=!p.pending && cycle%7!=0;
    t.ddt_r_valid_i=d.pending && !d.delay;t.ddt_r_data_i=t.ddt_r_valid_i?word(d.addr+8*d.beat):0;
    t.ddt_r_id_i=d.id;t.ddt_r_last_i=d.beat==d.len;
    t.ptw_r_valid_i=p.pending && !p.delay;t.ptw_r_data_i=t.ptw_r_valid_i?word(p.addr+8*p.beat):0;
    t.ptw_r_id_i=p.id;t.ptw_r_last_i=p.beat==p.len;
    t.resp_ready_i=cycle%6!=0;
    t.clk_i=0;evaluate();
    bool da=t.ddt_ar_valid_o&&t.ddt_ar_ready_i, pa=t.ptw_ar_valid_o&&t.ptw_ar_ready_i;
    bool dr=t.ddt_r_valid_i&&t.ddt_r_ready_o, pr=t.ptw_r_valid_i&&t.ptw_r_ready_o;
    uint64_t dad=t.ddt_ar_addr_o,pad=t.ptw_ar_addr_o;
    unsigned dil=t.ddt_ar_len_o,pil=t.ptw_ar_len_o,did=t.ddt_ar_id_o,pid=t.ptw_ar_id_o;
    request_fire=t.req_valid_i&&t.req_ready_o;
    response_fire=t.resp_valid_o&&t.resp_ready_i;
    response_addr=t.resp_addr_o;response_error=t.resp_error_o;response_code=t.fault_code_o;
    if(t.rst_ni&&t.fill_o){fills++;if(t.partition_i){unsigned expected=t.fill_did_o==10?3:0xfffc;
        check(t.fill_mask_o==expected,"TL fill mask not stable-main-DID owned");
        unsigned v=t.fill_victim_o;check(v&&!(v&(v-1))&&!(v&~expected),"TL fill escaped fixed partition");}}
    std::fprintf(trace,"%u,%s,%u,%u,%u,%u,%u,%06x,%016llx,%u,%u,%u,%014llx,%u,%u,%016llx,%u,%u,%016llx,%u,%u,%06x,%04x,%04x,%04x,%u,%u\n",
      cycle,phase,t.rst_ni,t.compare_i,t.partition_i,t.clear_i,t.req_valid_i,t.did_i,(unsigned long long)t.iova_i,
      t.req_ready_o,t.resp_valid_o,t.resp_error_o,(unsigned long long)t.resp_addr_o,da,dr,(unsigned long long)dad,pa,pr,
      (unsigned long long)pad,t.fill_o,t.gold_equal_o,t.fill_did_o,t.fill_mask_o,t.fill_victim_o,t.valid_o,t.plru_o,t.off_equal_o);
    t.clk_i=1;evaluate();t.clk_i=0;evaluate();
    if(!t.rst_ni){d=Read{};p=Read{};}
    else {
        if(d.delay)d.delay--;if(p.delay)p.delay--;
        if(dr){if(d.beat==d.len)d.pending=false;else d.beat++;}
        if(pr){if(p.beat==p.len)p.pending=false;else p.beat++;}
        if(da){check(!d.pending,"DDT overlap");d={true,dad,did,dil,0,2};}
        if(pa){check(!p.pending,"PTW overlap");p={true,pad,pid,pil,0,3};ptw_reads++;}
    }
    cycle++;
}
static void reset(bool partition){t.req_valid_i=0;t.clear_i=0;t.inval_i=0;t.partition_i=partition;
    t.compare_i=!partition;t.rst_ni=0;tick();tick();t.rst_ni=1;for(int i=0;i<5;i++)tick();}
static void translate(unsigned did,uint64_t addr,uint64_t expected,bool error=false,unsigned type=2){
    t.did_i=did;t.iova_i=addr;t.type_i=type;t.req_valid_i=1;
    unsigned start=cycle,ptw_before=ptw_reads,fill_before=fills;
    for(unsigned i=0;;i++){check(i<1000,"request accept timeout");tick();if(request_fire)break;}
    t.req_valid_i=0;
    for(unsigned i=0;;i++){check(i<1000,"response timeout");tick();if(response_fire)break;}
    check(response_error==error,"unexpected response error");
    if(!error)check(response_addr==expected,"wrong translated address");
    cases++;std::fprintf(records,"%u,%s,%u,%u,%016llx,%u,%u,%014llx,%u,%u,%u,%u\n",cases,phase,start,cycle,
        (unsigned long long)addr,did,type,(unsigned long long)response_addr,response_error,response_code,ptw_reads-ptw_before,fills-fill_before);
    for(unsigned i=0;i<3;i++)tick();
}
static void setup(){
    for(unsigned did=10;did<=11;did++){
        uint64_t base=0x10000+64*did;
        for(unsigned i=0;i<8;i++)mem[base+8*i]=0;
        mem[base]=1;mem[base+16]=uint64_t(did)<<12;
        mem[base+24]=0x8000000000000000ULL+(did==10?0x20:0x30);
        mem[base+32]=0x1000000000000080ULL; // Actual flat MSI context; ordinary payload outside IMSIC.
        mem[base+40]=255;mem[base+48]=0x24000;
    }
    mem[0x20000]=0x10001;mem[0x40400]=0x14001;
    mem[0x30000]=0x18001;mem[0x60800]=0x1c001;
    for(unsigned i=0;i<64;i++){
        mem[0x50000+8*i]=((0x80000ULL+i)<<10)|0xdf;
        mem[0x70000+8*i]=((0x90000ULL+i)<<10)|0xdf;
    }
    mem[0x50010]=(0x80002ULL<<10)|0xd3; // Read-only V page.
    mem[0x40c00]=(0xa0000ULL<<10)|0xdf; // Valid 2 MiB leaf.
    mem[0x20008]=(0xc0000ULL<<10)|0xdf; // Valid 1 GiB leaf.
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);check(argc==3,"need trace and records paths");
    trace=std::fopen(argv[1],"w");records=std::fopen(argv[2],"w");check(trace&&records,"output open");
    std::fprintf(trace,"cycle,phase,rst,compare,partition,clear,req_valid,did,iova,ready,resp_valid,error,resp_addr,ddt_ar_fire,ddt_r_fire,ddt_addr,ptw_ar_fire,ptw_r_fire,ptw_addr,fill,gold_equal,fill_did,mask,victim,valid,plru,off_equal\n");
    std::fprintf(records,"case,phase,start,end,iova,did,type,spaddr,error,fault_code,ptw_ar,fills\n");
    setup();reset(false);phase="S-fullmask-gold";
    for(unsigned i=0;i<48;i++){
        unsigned k=i%16;translate(11,0x20000000ULL+4096*k,0x90000000ULL+4096*k,false,i%2?2:3);
        translate(10,0x10000000ULL+(i%2)*4096,0x80000000ULL+(i%2)*4096);
        if(i%13==0){t.inval_i=1;tick();t.inval_i=0;}
    }
    translate(10,0x30001238ULL,0xa0001238ULL);
    translate(10,0x40001238ULL,0xc0001238ULL);
    translate(10,0x10002000ULL,0,true,3);
    reset(true);phase="P-stable-fill";
    translate(10,0x10000000ULL,0x80000000ULL);
    translate(10,0x10001000ULL,0x80001000ULL);
    for(unsigned i=0;i<48;i++){
        unsigned k=i%32;translate(11,0x20000000ULL+4096*k,0x90000000ULL+4096*k);
        unsigned before=ptw_reads;
        translate(10,0x10000000ULL+(i%2)*4096,0x80000000ULL+(i%2)*4096);
        check(ptw_reads==before,"A churn evicted protected V page");
    }
    phase="P-quiet-clear";t.clear_i=1;tick();t.clear_i=0;tick();
    check(t.valid_o==0&&t.plru_o==0,"P clear did not reset tags/tree");
    unsigned before=ptw_reads;translate(10,0x10000000ULL,0x80000000ULL);
    check(ptw_reads>before,"P clear did not force real rewalk");
    translate(10,0x30001238ULL,0xa0001238ULL);
    translate(10,0x10002000ULL,0,true,3);
    check(!d.pending&&!p.pending,"DS reads not drained");
    std::fclose(trace);std::fclose(records);
    std::printf("PASS p_tl cycles=%u cases=%u checks=%u fills=%u ptw_ar=%u MSI_BT_MRIF flat-context; actual TL/DDTW/PTW/IOTLB; no backend/board\n",cycle,cases,checks,fills,ptw_reads);
    t.final();return 0;
}
