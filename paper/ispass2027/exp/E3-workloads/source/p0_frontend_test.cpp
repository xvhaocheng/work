#include "Vp0_frontend_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
static void ck(bool v,const char *msg){if(!v)throw std::runtime_error(msg);}
struct Bench {
    Vp0_frontend_top d;
    uint64_t cycle=0,due=0,len=4096; unsigned pages=1,completed=0;
    bool stall=false,legacy=false,batch=true;
    std::vector<uint64_t> offsets;
    void eval(){d.clk_i=0;d.ready_i=!stall;d.complete_i=due&&cycle==due;d.eval();}
    void tick(){
        eval();
        if(legacy)ck(d.unchanged_o,"legacy cycle differs from frozen BTC1 frontend");
        if(d.rst_ni&&d.request_o&&d.ready_i){
            ck(!due,"one task outstanding");
            ck(d.length_o==len&&d.type_o==0,"actual backend descriptor length/type");
            uint64_t off=batch?(offsets.size()*len)%(pages*4096ULL):0;
            ck(d.src_o==0x10000+off&&d.dst_o==0x100000+off,"actual packed-ring addresses");
            if(batch)ck(d.issue_o&&d.issue_id_o==offsets.size()+1,"per-task issue event");
            offsets.push_back(off);due=cycle+7;
        }
        if(d.complete_i){ck(d.done_o,"retirement event");completed++;due=0;}
        d.clk_i=1;d.eval();cycle++;eval();
        if(legacy)ck(d.unchanged_o,"legacy post-clock state differs");
    }
    Bench(){d.rst_ni=0;d.valid_i=d.write_i=0;d.wstrb_i=255;tick();d.rst_ni=1;tick();}
    uint64_t access(unsigned addr,bool write=false,uint64_t value=0,bool error=false){
        d.addr_i=addr;d.write_i=write;d.wdata_i=value;d.valid_i=1;eval();
        ck(d.ready_o&&bool(d.error_o)==error,"MMIO ready/error");
        auto r=d.rdata_o;tick();d.valid_i=0;eval();return r;
    }
    void configure(uint64_t count,unsigned n,uint64_t length){
        len=length;pages=n;batch=true;
        access(0x88,true,0);access(0,true,0x10000);access(8,true,0x100000);
        access(16,true,len);access(24,true,0);
        access(0x90,true,count);access(0x98,true,n);access(0x88,true,1);
    }
    void drain(){for(unsigned i=0;i<200000&&d.batch_active_o;i++)tick();
        ck(!d.batch_active_o&&!due,"finite batch safely drained");}
};
int main(int argc,char **argv){Verilated::commandArgs(argc,argv);try{
    unsigned cases=0;
    for(uint64_t length:{64ULL,1024ULL,4096ULL})for(unsigned pages:{1U,8U}){
        // One task, exactly one ring, and two full rings exercise every wrap edge.
        for(uint64_t count:{1ULL,(pages*4096ULL)/length,2*(pages*4096ULL)/length}){
            Bench b;b.legacy=length==4096;b.configure(count,pages,length);
            ck(b.access(0x80)==0x49444d4142544302ULL,"BTC2 magic");
            auto id=b.access(0x28);ck(id!=0,"functional batch ID");
            b.stall=true;for(int i=0;i<5;i++){b.eval();ck(b.d.request_o,"offered task stays valid");
                ck(b.d.length_o==length&&b.d.src_o==0x10000,"stalled payload stable");b.tick();}
            b.stall=false;b.drain();
            ck(b.offsets.size()==count&&b.completed==count,"exact descriptor count");
            ck(b.access(0xa0)==count&&b.access(0xa8)==count,"issued/completed counters");
            ck(b.access(0x88)==9&&b.access(0x30)==id,"completion state/ID");cases++;
        }
        {Bench b;b.configure(64,pages,length);b.access(0x28);while(b.offsets.size()<3)b.tick();
            b.access(0x88,true,2);b.drain();ck(b.completed==b.offsets.size()&&b.completed==3,"STOP drains current task");
            ck((b.access(0x88)&7)==5,"stopped state");cases++;}
        {Bench b;b.configure(64,pages,length);b.access(0x28);b.stall=true;b.access(0x88,true,2);
            for(int i=0;i<8;i++){ck(b.d.request_o&&b.d.src_o==0x10000&&b.d.length_o==length,
                "STOP cannot withdraw stalled payload");b.tick();}
            b.stall=false;b.drain();ck(b.completed==1,"STOP releases exactly offered task");cases++;}
        for(unsigned addr:{0U,8U,16U,0x90U,0x98U}){
            Bench b;b.configure(8,pages,length);b.access(0x28);b.stall=true;
            auto before=b.access(addr);b.access(addr,true,addr==16?1024:0x90000,addr<0x80);
            ck(b.access(addr)==before,"active configuration immutable");
            b.stall=false;b.drain();ck(b.access(0x88)>>32,"active write raises sticky error");cases++;
        }
    }
    for(uint64_t length:{0ULL,128ULL,256ULL,512ULL,1536ULL,2048ULL,8192ULL,~0ULL}){
        Bench b;b.configure(8,1,length);b.access(0x28,false,0,true);
        ck(!b.d.batch_active_o&&b.offsets.empty(),"unsupported batch length cannot start");cases++;
    }
    {Bench b;b.configure(8,1,64);b.access(0,true,0x10001);b.access(0x28,false,0,true);
        ck(!b.d.batch_active_o&&b.offsets.empty(),"unaligned batch cannot start");cases++;}
    {Bench b;b.configure(8,1,64);b.access(0x98,true,0);b.access(0x28,false,0,true);
        ck(!b.d.batch_active_o&&b.offsets.empty(),"invalid page count cannot start");cases++;}
    {Bench b;b.configure(1ULL<<32,8,64);b.access(0x28);while(b.offsets.size()<3)b.tick();
        b.access(0x88,true,2);b.drain();ck(b.completed==3,"maximum 33-bit target bounded STOP");cases++;}
    {Bench b;b.configure((1ULL<<32)+1,8,64);b.access(0x28,false,0,true);
        ck(!b.d.batch_active_o,"oversized target rejected");cases++;}
    // Non-batch uses arbitrary original register lengths; compare every cycle.
    for(uint64_t length:{64ULL,1024ULL,1500ULL,4096ULL,65536ULL}){
        Bench b;b.legacy=true;b.batch=false;b.len=length;
        b.access(0,true,0x10000);b.access(8,true,0x100000);b.access(16,true,length);b.access(24,true,0);
        auto id=b.access(0x28);for(unsigned i=0;i<100&&b.completed==0;i++)b.tick();
        ck(b.completed==1&&b.offsets.size()==1&&b.access(0x30)==id,"legacy non-batch completion");cases++;
    }
    // Independent real frontend instances run V64/V1024 alongside A4096.
    for(uint64_t length:{64ULL,1024ULL}){
        Bench v,a;v.configure(256,1,length);a.configure(256,8,4096);
        a.access(0x28);v.access(0x28);
        for(unsigned i=0;i<10000&&(v.d.batch_active_o||a.d.batch_active_o);i++){v.tick();a.tick();}
        ck(v.completed==256&&a.completed==256,"independent V/A lengths finish");cases++;
    }
    std::cout<<"PASS "<<cases<<" finite production frontend P0 scenarios; actual length/offset, STOP, locks, BTC1 legacy cycle comparison\n";
    return 0;
}catch(const std::exception &e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
