#include "Vfull_frontend_top.h"
#include "verilated.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

// Only the real register frontend is modeled. Completion is synthetic, seven
// cycles after COPY acceptance; this is not an AXI/data/observer test.
static void ck(bool ok,const char *msg){if(!ok)throw std::runtime_error(msg);}
static std::ofstream events;
static unsigned cases=0;
struct Geometry {
    uint64_t length=4096; unsigned pages=1,kind=0,rows=1,pitch=0,matrix=0,columns=0;
    uint64_t span()const{return uint64_t(pages)*4096;}
    uint64_t ring()const{
        if(kind==2)return uint64_t(matrix)*columns;
        if(kind==1)return uint64_t(pages)*(4096/length);
        return span()/length;
    }
    // Closed form independent of the DUT's incremental row/column cursors.
    uint64_t offset(uint64_t k)const{
        if(kind==2){auto op=(k%ring())/rows;auto row=k%rows;
            return ((op/columns)*rows+row)*pitch+(op%columns)*length;}
        if(kind==1)return ((k/(4096/length))%pages)*4096+(k%(4096/length))*length;
        return (k*length)%span();
    }
};
static Geometry linear(uint64_t n,unsigned p){return {n,p,0,1,0,0,0};}
static Geometry packed(uint64_t n,unsigned p){return {n,p,1,1,0,0,0};}
static Geometry tiled(uint64_t bytes){auto n=std::min<uint64_t>(bytes,1024);
    return {n,256,2,unsigned(bytes/n),16384,64,unsigned(16384/n)};}

struct Bench {
    Vfull_frontend_top d;
    Geometry g;
    std::string name;
    uint64_t cycle=0,due=0,src_base=0x10000,dst_base=0x100000;
    uint64_t accepted=0,completed=0;
    bool stall=false,gold2=false,gold1=false,batch=true;
    std::set<uint64_t> seen_slots;
    explicit Bench(std::string n):name(std::move(n)){
        d.rst_ni=0;d.valid_i=d.write_i=0;d.wstrb_i=255;tick();d.rst_ni=1;tick();}
    void compare(){
        if(gold2)ck(d.unchanged_o,"cycle differs from frozen BTC2 frontend");
        if(gold1)ck(d.unchanged1_o,"cycle differs from frozen BTC1 frontend");
    }
    void eval(){d.clk_i=0;d.ready_i=!stall;d.complete_i=due&&cycle==due;d.eval();}
    void tick(){
        eval();compare();
        if(d.rst_ni&&d.request_o&&d.ready_i){
            ck(!due,"single COPY outstanding");
            ck(d.length_o==g.length&&d.type_o==0,"actual length/type");
            auto off=batch?g.offset(accepted):0;
            ck(d.src_o==src_base+off&&d.dst_o==dst_base+off,"actual geometry address");
            if(batch){
                ck(off+g.length<=g.span(),"COPY remains in allocated ring");
                ck(d.issue_o&&d.issue_id_o==accepted+1,"per-COPY issue event");
            }
            events<<"{\"case\":\""<<name<<"\",\"index\":"<<accepted
                <<",\"src\":"<<d.src_o<<",\"dst\":"<<d.dst_o
                <<",\"length\":"<<d.length_o<<"}\n";
            seen_slots.insert(off);accepted++;due=cycle+7;
        }
        if(d.complete_i){ck(d.done_o,"per-COPY retirement event");completed++;due=0;}
        d.clk_i=1;d.eval();cycle++;eval();compare();
    }
    uint64_t access(unsigned a,bool wr=false,uint64_t v=0,bool err=false){
        d.addr_i=a;d.write_i=wr;d.wdata_i=v;d.valid_i=1;eval();
        ck(d.ready_o&&bool(d.error_o)==err,"MMIO ready/error");
        auto r=d.rdata_o;tick();d.valid_i=0;eval();return r;
    }
    void configure(uint64_t n,const Geometry &geom,bool new_regs=true){
        g=geom;batch=true;
        access(0x88,true,0);access(0,true,src_base);access(8,true,dst_base);
        access(16,true,g.length);access(24,true,0);access(0x90,true,n);access(0x98,true,g.pages);
        if(new_regs){
            access(0xb0,true,g.kind);access(0xb8,true,g.rows);access(0xc0,true,g.pitch);
            access(0xc8,true,g.matrix);access(0xd0,true,g.columns);
        }
        access(0x88,true,1);
    }
    void drain(){for(uint64_t i=0;i<3000000&&d.batch_active_o;i++)tick();
        ck(!d.batch_active_o&&!due,"finite batch safely drained");}
    void finish(){
        ck(accepted==completed,"all accepted COPYs retired");
        std::cout<<"CASE {\"name\":\""<<name<<"\",\"geometry\":"<<g.kind
            <<",\"length\":"<<g.length<<",\"pages\":"<<g.pages
            <<",\"rows\":"<<g.rows<<",\"pitch\":"<<g.pitch
            <<",\"matrix\":"<<g.matrix<<",\"columns\":"<<g.columns
            <<",\"batch\":"<<(batch?"true":"false")<<",\"accepted\":"<<accepted
            <<",\"completed\":"<<completed<<",\"distinct_offsets\":"<<seen_slots.size()
            <<",\"gold_btc2\":"<<(gold2?"true":"false")
            <<",\"gold_btc1\":"<<(gold1?"true":"false")<<"}\n";cases++;
    }
};
static void complete(const Geometry &g,uint64_t count,const std::string &name,bool gold=false){
    Bench b(name);b.gold2=gold;b.gold1=gold&&g.length==4096;
    b.configure(count,g,!gold);ck(b.access(0x80)==0x49444d4142544303ULL,"BTC3 magic");
    auto id=b.access(0x28);ck(id!=0,"functional batch ID");
    b.stall=true;
    for(unsigned i=0;i<5;i++){
        b.eval();ck(b.d.request_o&&b.d.length_o==g.length&&b.d.src_o==b.src_base,
            "initial offered payload held stable");b.tick();
    }
    b.stall=false;b.drain();
    ck(b.accepted==count,"exact target COPY count");
    ck(b.access(0xa0)==count&&b.access(0xa8)==count,"completed and issued counters");
    ck(b.access(0x88)==9&&b.access(0x30)==id,"completion state and functional ID");b.finish();
}
static void refuse(const std::string &n,const Geometry &g,unsigned addr=0,uint64_t val=0){
    Bench b(n);b.configure(8,g);if(addr)b.access(addr,true,val);
    b.access(0x28,false,0,true);ck(!b.d.batch_active_o&&!b.accepted,"invalid config never issues COPY");b.finish();
}
int main(int argc,char **argv){Verilated::commandArgs(argc,argv);try{
    ck(argc==2,"raw event output argument");events.open(argv[1]);ck(events.good(),"raw event file opened");
    // Frozen BTC2 P0 trace, and independently BTC1 for old 4KiB. Include old
    // multi-device non-power-of-two page counts: ABI3 same-total uses 3/6 pages.
    for(uint64_t len:{64ULL,1024ULL,4096ULL})for(unsigned pages:{1U,3U,6U,8U}){
        auto g=linear(len,pages);
        unsigned variant=0;
        for(uint64_t n:std::vector<uint64_t>{1,g.ring(),2*g.ring()})
            complete(g,n,"legacy_"+std::to_string(len)+"_"+std::to_string(pages)+"_"+std::to_string(n)+"_"+std::to_string(variant++),true);
    }
    for(uint64_t len:{64ULL,1024ULL,4096ULL})for(unsigned pages:{1U,8U})for(bool offered:{false,true}){
        Bench b("legacy_stop_"+std::to_string(len)+"_"+std::to_string(pages)+"_"+std::to_string(offered));
        b.gold2=true;b.gold1=len==4096;b.configure(64,linear(len,pages),false);b.access(0x28);
        while(b.accepted<3)b.tick();
        if(offered){while(b.due)b.tick();b.stall=true;}
        b.access(0x88,true,2);if(offered)for(unsigned i=0;i<8;i++)b.tick();
        b.stall=false;b.drain();ck(b.completed==(offered?4U:3U),"old STOP trace and count");b.finish();
    }
    for(uint64_t len:{64ULL,1024ULL,4096ULL})for(unsigned addr:{0U,8U,16U,0x90U,0x98U}){
        Bench b("legacy_lock_"+std::to_string(len)+"_"+std::to_string(addr));
        b.gold2=true;b.gold1=len==4096;b.configure(8,linear(len,8),false);b.access(0x28);b.stall=true;
        auto before=b.access(addr);b.access(addr,true,0x90000,addr<0x80);
        ck(b.access(addr)==before,"old active configuration immutable");b.stall=false;b.drain();
        ck(b.access(0x88)>>32,"old active error trace");b.finish();
    }
    // All six real linear COPY sizes, minimal rings and full storage rings.
    for(uint64_t len:{64ULL,256ULL,1024ULL,4096ULL,16384ULL,65536ULL}){
        auto g=linear(len,std::max<uint64_t>(1,len/4096));
        complete(g,2*g.ring(),"linear_min_"+std::to_string(len));
        g=linear(len,256);complete(g,2*g.ring()+1,"linear_storage_"+std::to_string(len));
        auto t=tiled(len);complete(t,2*t.ring()+1,"tile_"+std::to_string(len));
    }
    //1536B uses two page-local slots. Two rings exercise padding and wrap.
    for(uint64_t len:{64ULL,256ULL,1024ULL,1536ULL,4096ULL})for(unsigned pages:{1U,8U}){
        auto g=packed(len,pages);complete(g,2*g.ring()+1,
            "pagepack_"+std::to_string(len)+"_"+std::to_string(pages));
    }
    // STOP in WAIT, and STOP while COUNT offers a held row. It does not
    // manufacture a completed tile or withdraw an already offered COPY.
    for(auto g:{linear(65536,256),packed(1536,8),tiled(16384),tiled(65536)}){
        for(bool offered:{false,true}){
            Bench b("stop_"+std::to_string(g.length)+"_"+std::to_string(g.rows)+"_"+std::to_string(offered));
            b.configure(200,g);b.access(0x28);
            while(b.accepted<3)b.tick();
            if(offered){while(b.due)b.tick();b.stall=true;b.eval();ck(b.d.request_o,"next row offered");}
            auto addr=b.d.src_o;auto bytes=b.d.length_o;
            b.access(0x88,true,2);
            if(offered)for(unsigned i=0;i<9;i++){
                ck(b.d.request_o&&b.d.src_o==addr&&b.d.length_o==bytes,"STOP preserves held row");b.tick();}
            b.stall=false;b.drain();ck(b.completed==(offered?4U:3U),"STOP exact partial-row drain");
            ck((b.access(0x88)&7)==5,"stopped status");b.finish();
        }
    }
    // All batch configuration registers are immutable while active, including
    // enable, geometry, and high-address writes. Existing error paths remain.
    for(unsigned addr:{0U,8U,16U,24U,0x88U,0x90U,0x98U,0xb0U,0xb8U,0xc0U,0xc8U,0xd0U}){
        Bench b("active_lock_"+std::to_string(addr));b.configure(8,tiled(65536));b.access(0x28);b.stall=true;
        auto before=b.access(addr);b.access(addr,true,0x90000,addr<0x80);
        auto after=b.access(addr);if(addr==0x88)after&=~(1ULL<<32);
        ck(after==before,"active register unchanged");b.stall=false;b.drain();
        ck(b.access(0x88)>>32,"active write sticky error");b.finish();
    }
    for(uint64_t len:{0ULL,128ULL,512ULL,1500ULL,1536ULL,2048ULL,8192ULL,131072ULL,~0ULL})
        refuse("bad_linear_len_"+std::to_string(len),linear(len,256));
    for(uint64_t len:{0ULL,128ULL,512ULL,1500ULL,2048ULL,16384ULL,65536ULL,~0ULL})
        refuse("bad_packed_len_"+std::to_string(len),packed(len,8));
    refuse("span_too_short",linear(65536,8));
    refuse("span_not_divisible",linear(16384,6));
    for(unsigned addr:{0U,8U}){
        Bench b("unaligned_"+std::to_string(addr));b.configure(8,linear(64,1));b.access(addr,true,0x10001);
        b.access(0x28,false,0,true);ck(!b.accepted&&!b.d.batch_active_o,"unaligned base rejected");b.finish();
    }
    for(uint64_t conf:{1ULL,2ULL,3ULL,16ULL,32ULL,48ULL})
        refuse("bad_conf_"+std::to_string(conf),linear(64,1),24,conf);
    for(unsigned addr:{0xb0U,0xb8U,0xc0U,0xc8U,0xd0U,0x98U,0x90U}){
        refuse("field_max_"+std::to_string(addr),linear(64,1),addr,~0ULL);
        if(addr!=0xb0&&addr!=0xc0&&addr!=0xc8&&addr!=0xd0)
            refuse("field_zero_"+std::to_string(addr),linear(64,1),addr,0);
    }
    for(auto pair:std::vector<std::pair<unsigned,uint64_t>>{{0xb0,3},{0xb8,2},{0xc0,8192},{0xc8,32},{0xd0,32}})
        refuse("bad_field_"+std::to_string(pair.first),linear(64,1),pair.first,pair.second);
    // Legal individual register values can still form an illegal combination.
    for(auto pair:std::vector<std::pair<unsigned,uint64_t>>{{0x98,128},{0xc0,0},{0xc8,0},{0xd0,64},{16,4096}})
        refuse("bad_tile_combo_"+std::to_string(pair.first),tiled(65536),pair.first,pair.second);
    for(auto pair:std::vector<std::pair<unsigned,uint64_t>>{{0xb8,4},{0xc0,16384},{0xc8,64},{0xd0,16}})
        refuse("bad_linear_combo_"+std::to_string(pair.first),linear(1024,256),pair.first,pair.second);
    for(unsigned addr:{0xd8U,0xe0U,0xe8U,0xf0U,0xf8U}){
        Bench b("reserved_"+std::to_string(addr));b.configure(8,linear(64,1));
        ck(b.access(addr)==0,"reserved read zero");b.access(addr,true,0);
        b.access(0x28,false,0,true);ck(!b.accepted,"reserved writes rejected");b.finish();
    }
    for(unsigned addr:{0x88U,0x90U,0x98U,0xb0U,0xb8U,0xc0U,0xc8U,0xd0U}){
        Bench b("partial_strobe_"+std::to_string(addr));b.configure(8,linear(64,1));
        b.d.wstrb_i=0x0f;b.access(addr,true,0);b.d.wstrb_i=0xff;
        b.access(0x28,false,0,true);ck(!b.accepted,"partial strobe rejected");b.finish();
    }
    for(unsigned addr:{0U,8U}){
        Bench b("address_overflow_"+std::to_string(addr));b.configure(8,linear(64,1));
        b.access(addr,true,~uint64_t(4095));b.access(0x28,false,0,true);
        ck(!b.accepted,"exclusive ring end must not overflow u64");b.finish();
    }
    {Bench b("highest_nonoverflow_page");b.src_base=~uint64_t(8191);b.dst_base=b.src_base;
        b.configure(64,linear(64,1));b.access(0x28);b.drain();ck(b.completed==64,"highest allowed base");b.finish();}
    {Bench b("max_target_stop");b.configure(1ULL<<32,linear(64,8));b.access(0x28);
        while(b.accepted<3)b.tick();b.access(0x88,true,2);b.drain();ck(b.completed==3,"33-bit count STOP");b.finish();}
    {Bench b("oversized_target");b.configure((1ULL<<32)+1,linear(64,8));
        b.access(0x28,false,0,true);ck(!b.d.batch_active_o,"oversized count rejected");b.finish();}
    // CTRL=0 clears prior geometry and errors so the old register sequence can
    // follow a completed tile. The old gold traces are checked separately above.
    {Bench b("tile_before_legacy_reset");b.configure(9,tiled(65536));b.access(0x28);b.drain();b.finish();
        b.access(0x88,true,0);ck(b.access(0xb0)==0&&b.access(0xb8)==1&&b.access(0xc0)==0&&
            b.access(0xc8)==0&&b.access(0xd0)==0,"CTRL zero restores legacy geometry");
        b.accepted=b.completed=0;b.seen_slots.clear();b.name="legacy_after_tile_reset";b.configure(16,linear(4096,8),false);
        b.access(0x28);b.drain();ck(b.completed==16,"old ABI after tile");b.finish();}
    for(unsigned v=0;v<3;v++)for(unsigned a=0;a<3;a++){
        std::vector<Geometry> geometry{packed(1536,8),linear(65536,256),tiled(65536)};
        auto tag=std::to_string(v)+"_"+std::to_string(a);
        Bench victim("independent_v_"+tag),aggressor("independent_a_"+tag);
        victim.configure(129,geometry[v]);aggressor.configure(257,geometry[a]);
        aggressor.access(0x28);victim.access(0x28);
        for(unsigned i=0;i<10000&&(victim.d.batch_active_o||aggressor.d.batch_active_o);i++){
            victim.tick();aggressor.tick();
        }
        ck(victim.completed==129&&aggressor.completed==257,"independent V/A shape and length");
        victim.finish();aggressor.finish();
    }
    for(uint64_t len:{64ULL,256ULL,1024ULL,1500ULL,4096ULL,65536ULL}){
        Bench b("nonbatch_"+std::to_string(len));b.gold2=b.gold1=true;b.batch=false;b.g.length=len;
        b.access(0,true,b.src_base);b.access(8,true,b.dst_base);b.access(16,true,len);b.access(24,true,0);
        auto id=b.access(0x28);for(unsigned i=0;i<100&&!b.completed;i++)b.tick();
        ck(b.completed==1&&b.accepted==1&&b.access(0x30)==id,"nonbatch old completion");b.finish();
    }
    events.close();std::cout<<"PASS "<<cases<<" finite actual frontend scenarios; synthetic completion; no AXI/board qualification\n";
    return 0;
}catch(const std::exception &e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
