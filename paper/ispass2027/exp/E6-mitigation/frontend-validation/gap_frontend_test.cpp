#include "Vgap_frontend_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static uint64_t assertions=0, cycles=0, zero_cycles=0;
static void ck(bool v,const char *m){assertions++;if(!v)throw std::runtime_error(m);}
struct Bench {
    Vgap_frontend_top d;
    uint64_t cycle=0,due=0,length=4096,gap_ticks=0;
    unsigned pages=8,done=0;
    bool stalled=false,compare_zero=true,batch=true;
    std::vector<uint64_t> issues, completions;
    void eval(){d.clk_i=0;d.ready_i=!stalled;d.complete_i=due&&cycle==due;d.eval();}
    void check_equal(){ck(d.disabled_equal_o,"default-disabled differs from exact 6c5 P0 frontend");
        if(compare_zero){ck(d.zero_equal_o,"enabled gap0 differs from exact 6c5 P0 frontend");zero_cycles++;}}
    void tick(){
        eval();check_equal();cycles++;
        if(d.rst_ni && d.request_o && d.ready_i){
            ck(!due,"only one actual frontend task outstanding");
            ck(d.length_o==length&&d.type_o==0,"real descriptor length/type");
            const auto offset=batch?(issues.size()*length)%(pages*4096ULL):0;
            ck(d.src_o==0x10000+offset&&d.dst_o==0x100000+offset,"real descriptor packed-ring address");
            if(batch)ck(d.issue_o&&d.issue_id_o==issues.size()+1,"real per-task issue/ID");
            issues.push_back(cycle);due=cycle+7;
        }
        if(d.complete_i){ck(d.done_o,"real per-task done event");done++;completions.push_back(cycle);due=0;}
        if(d.gap_active_o){ck(!d.request_o&&!d.issue_o&&!d.done_o,"GAP offers no backend request/task event");gap_ticks++;}
        d.clk_i=1;d.eval();cycle++;eval();check_equal();
    }
    Bench(){d.rst_ni=0;d.valid_i=d.write_i=0;d.wstrb_i=255;tick();d.rst_ni=1;tick();}
    uint64_t mmio(unsigned address,bool write=false,uint64_t value=0,bool error=false){
        d.addr_i=address;d.write_i=write;d.wdata_i=value;d.valid_i=1;eval();
        ck(d.ready_o&&bool(d.error_o)==error,"MMIO response ready/error");auto result=d.rdata_o;
        tick();d.valid_i=0;eval();return result;
    }
    void configure(uint64_t count=4,uint64_t bytes=4096,unsigned ring=8){
        length=bytes;pages=ring;
        mmio(0x88,true,0);mmio(0,true,0x10000);mmio(8,true,0x100000);
        mmio(16,true,length);mmio(24,true,0);mmio(0x90,true,count);mmio(0x98,true,pages);mmio(0x88,true,1);
        ck(d.target_o==count,"read-only tap is actual 33bit hardware target");
    }
    void gap(uint32_t value){compare_zero=false;mmio(0xe0,true,value);ck(d.gap_cycles_o==value,"actual gap read-only tap");
        ck(mmio(0xe0)==value,"gap32 MMIO readback");}
    void drain(){for(unsigned i=0;i<100000&&d.batch_active_o;i++)tick();ck(!d.batch_active_o&&!due,"bounded natural/STOP drain");}
    void until_gap(){for(unsigned i=0;i<100&& !d.gap_active_o;i++)tick();ck(d.gap_active_o,"reached actual GAP");}
    void report(const char *name){std::cout<<"CASE "<<name<<" cycles="<<cycle<<" tasks="<<issues.size()<<" done="<<done
        <<" gap_cycles="<<gap_ticks<<" configured="<<d.gap_cycles_o<<"\n";}
};

int main(int argc,char **argv){Verilated::commandArgs(argc,argv);try{
    unsigned cases=0;
    // Accepted P0 lengths/targets, every legacy output checked before/after each edge.
    for(uint64_t len:{64ULL,1024ULL,4096ULL})for(unsigned ring:{1U,8U})for(unsigned count:{1U,17U,4096U}){
        Bench b;b.configure(count,len,ring);b.mmio(0x28);b.stalled=true;
        for(int i=0;i<5;i++){ck(b.d.request_o,"gap0 first offer not delayed");b.tick();}
        b.stalled=false;b.drain();ck(b.done==count&&b.issues.size()==count&&b.gap_ticks==0,"gap0 complete exact budget");
        ck(b.mmio(0x88)==9,"gap0 finished state");b.report("gap0-P0-cycle-equivalence");cases++;
    }
    // Non-batch remains cycle exact even for lengths outside P0's batch domain.
    for(uint64_t len:{64ULL,1500ULL,65536ULL}){Bench b;b.batch=false;b.length=len;
        b.mmio(0,true,0x10000);b.mmio(8,true,0x100000);b.mmio(16,true,len);b.mmio(24,true,0);
        b.mmio(0x28);for(unsigned i=0;i<30&&!b.done;i++)b.tick();ck(b.done==1,"legacy nonbatch completes");
        b.report("gap0-nonbatch-cycle-equivalence");cases++;}
    for(uint32_t gap:{1U,2U,1000U,2000U})for(unsigned count:{1U,4U}){
        Bench b;b.configure(count);b.gap(gap);ck(b.mmio(0x80)==0x49444d4142544302ULL,"BTC2 magic unchanged");
        ck(b.mmio(0xe8)==0x49444d4147415001ULL,"independent gap capability");b.mmio(0x28);
        ck(b.d.request_o&&!b.d.gap_active_o,"first task has no leading gap");b.drain();
        ck(b.done==count&&b.issues.size()==count&&b.gap_ticks==uint64_t(count-1)*gap,"exact nonfinal GAP occupancy/no tail gap");
        for(unsigned i=1;i<count;i++)ck(b.issues[i]-b.completions[i-1]==uint64_t(gap)+1,"gap delays next offer by exactly G cycles");
        b.report("nonzero-gap-first-tail-spacing");cases++;
    }
    // Exact same-edge STOP/expiry, plus STOP early in a large gap.
    for(uint32_t gap:{1U,3U,0xffffffffU}){Bench b;b.configure();b.gap(gap);b.mmio(0x28);b.until_gap();
        if(gap<10)while(b.d.countdown_o>1)b.tick();
        else {ck(b.d.countdown_o==0xffffffffU,"32bit max loaded without truncation");b.tick();
            ck(b.d.countdown_o==0xfffffffeU,"32bit max decrements without wrap");}
        b.mmio(0x88,true,2);b.drain();ck(b.done==1&&b.issues.size()==1,"STOP priority creates no extra offer or done");
        ck((b.mmio(0x88)&7)==5,"GAP STOP retires stopped functional batch");b.report("GAP-STOP-expiry-or-max-boundary");cases++;
    }
    {Bench b;b.configure();b.gap(3);b.mmio(0x28);b.stalled=true;b.mmio(0x88,true,2);
        for(int i=0;i<8;i++){ck(b.d.request_o&&b.d.src_o==0x10000&&b.d.length_o==4096,"STOP cannot withdraw offered stalled request");b.tick();}
        b.stalled=false;b.drain();ck(b.done==1&&b.issues.size()==1,"offered STOP drains exactly one task");
        b.report("STOP-offered-backpressure");cases++;}
    for(bool in_gap:{false,true}){Bench b;b.configure();b.gap(4);b.mmio(0x28);
        if(in_gap)b.until_gap();else b.stalled=true;
        b.mmio(0xe0,true,9,true);ck(b.d.gap_cycles_o==4,"active gap configuration immutable");
        b.mmio(0x88,true,2);b.stalled=false;b.drain();ck(b.mmio(0x88)>>32,"active gap write sticky error");
        b.report("active-gap-write-rejected");cases++;}
    for(unsigned kind:{0U,1U,2U}){Bench b;b.configure();b.gap(7);
        if(kind==0)b.mmio(0xe0,true,1ULL<<32,true);
        if(kind==1){b.d.wstrb_i=0x0f;b.mmio(0xe0,true,3,true);b.d.wstrb_i=255;}
        if(kind==2)b.mmio(0xe8,true,0,true);
        ck(b.d.gap_cycles_o==7&&b.mmio(0x88)>>32,"bad width/strobe/capability write rejected/sticky");
        b.mmio(0x88,true,0);ck(!b.d.gap_cycles_o&&b.d.target_o==4,"disable clears gap without forging target");
        ck(b.mmio(0xe0)==0,"disable gap reset readback");b.report("gap-register-negative");cases++;}
    {Bench b;b.compare_zero=false;b.configure(1ULL<<32);b.gap(0xffffffffU);b.mmio(0x28);b.until_gap();
        ck(b.d.target_o==(1ULL<<32),"33bit target endpoint untruncated");b.mmio(0x88,true,2);b.drain();
        b.report("33bit-target-max-bounded-STOP");cases++;}
    std::cout<<"PASS cases="<<cases<<" assertions="<<assertions<<" cycles="<<cycles<<" enabled_gap0_comparison_edges="<<zero_cycles
        <<"; actual frontend only, synthetic completion; max32 gap checked load/decrement/STOP, no 2^32-cycle run\n";return 0;
}catch(const std::exception &e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
