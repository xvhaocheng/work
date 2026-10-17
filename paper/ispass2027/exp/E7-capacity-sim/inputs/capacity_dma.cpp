// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Test-only capacity experiment, derived from the frozen invalidation fixture.
#ifndef IOTLB_ENTRIES
#define IOTLB_ENTRIES 16
#endif
// Host PTE writes are model actions; no CPU/Linux execution-time claim.
#include "Vinvalidation_dma_top.h"
#include "verilated.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

double sc_time_stamp() { return 0; }
using U64=uint64_t;
static constexpr U64 PAGE=4096, DDT=0xA0010000ULL, ROOT_B=0xA0020000ULL;
static constexpr U64 ROOT_A=0xA0030000ULL, L1_B=0xA0040000ULL, L0_B=0xA0050000ULL;
static constexpr U64 L1_A=0xA0060000ULL, L0_A=0xA0070000ULL, CQ_BASE=0xA0080000ULL;
static constexpr U64 MARKER=0xA0090000ULL, B_IOVA=0x10000000ULL;
static constexpr U64 DDT_ROOT=0xA00A0000ULL;
static constexpr U64 B_PA=0x80000000ULL, A_PA=0x90000000ULL;
static constexpr U64 REG_NEXT=0x28, REG_DONE=0x30;
static constexpr unsigned CQ_ENTRIES=256, B_PSCID=0x80000;
static constexpr long CYCLE_LIMIT=10000000, STALL_LIMIT=300000;
struct Failure:std::runtime_error {using std::runtime_error::runtime_error;};
#define REQUIRE(c,m) do {if(!(c)) throw Failure(std::string(m)+" at line "+std::to_string(__LINE__));} while(0)
static U64 pattern(unsigned page,unsigned beat) {
    return (((U64(page)<<9)|beat)*0x9E3779B97F4A7C15ULL)^0xA5C3E10D2B4F9678ULL;
}
static std::string quoted(const std::string& s) {
    std::string r="\""; for(unsigned char c:s) {
        if(c=='"'||c=='\\') {r+='\\';r+=char(c);} else if(c=='\n')r+="\\n";
        else if(c<32)r+='?';else r+=char(c);
    } return r+'"';
}
template<class W> static U64 bits(const W& w,unsigned start,unsigned width) {
    U64 value=0;for(unsigned i=0;i<width;++i)value|=U64((w[(start+i)/32]>>((start+i)%32))&1u)<<i;
    return value;
}
struct Options {
    std::string out,mode="m1",domain="different",map_policy="full-api",case_id;
    unsigned b_pages=2,a_jobs=0,b_jobs=4096,a_pages=0;
    bool smoke=false;
    unsigned pscid_a()const{return B_PSCID+(domain=="different");}
    unsigned batch()const{return mode=="m4-16"?16:mode=="m4-64"?64:0;}
};
class Tb {
public:
    Options o;
    std::unique_ptr<Vinvalidation_dma_top> t{new Vinvalidation_dma_top};
    FILE *jsonl=nullptr,*csv=nullptr,*jobs_file=nullptr;
    long cycle=0,t0=-1,measure_end=-1,final_a=-1,last_done[2]={-1,-1};
    std::string era="initial",failure;
    bool measured=false,passed=false,data_ok=false,quiet_ok=false,commands_ok=false;
    bool warm_resident=false,warm_no_walk=false;
    using Key=std::pair<unsigned,U64>;
    std::set<Key> seen,removed;
    struct Counters {U64 hits=0,misses=0,walks=0,inval=0,cold=0,replacement=0,pte_bytes=0;};
    Counters stats[2];
    U64 all_walks=0,all_marker_b=0;
    U64 inv_count=0,fence_count=0,fence_completed=0,decode_count=0,submit_count=0,doorbells=0;
    U64 inval_verified_events=0,inval_verified_ways=0;
    U64 pending_inval_ways=0;
    U64 cq_peak=0,cq_entry_cycles=0,rounds=0,total_waiting=0,map_regions=0,unmap_regions=0;
    U64 measured_issue[2]={0,0},measured_done[2]={0,0};
    unsigned tail=0;
    long last_progress=0;
    std::map<std::string,U64> previous_state;
    std::deque<std::string> recent;
    using Page=std::array<uint8_t,PAGE>;
    std::unordered_map<U64,Page> memory;
    struct Job {
        unsigned lane=0,index=0,pattern_page=0;
        U64 src=0,dst=0,src_pa=0,dst_pa=0,next_id=0;
        long issue=-1,done=-1,poll_done=-1;
        U64 ar=0,aw=0,r=0,w=0,b=0;
        std::array<uint8_t,PAGE> r_seen{},w_seen{};
    };
    std::vector<std::unique_ptr<Job>> jobs;
    Job* active[2]={nullptr,nullptr};
    struct Read {U64 addr=0;unsigned id=0,len=0,beat=0;int latency=16;Job* job=nullptr;};
    struct Write {U64 addr=0;unsigned id=0,len=0,beat=0;Job* job=nullptr;};
    struct B {unsigned id=0;Job* job=nullptr;};
    std::deque<Read> readq;
    std::deque<Write> writeq;
    std::deque<B> bq;
    struct Cfg {
        unsigned target=0,size=3;bool write=false;U64 addr=0,data=0;unsigned strb=0;
        std::function<void(U64)> done;
    };
    std::deque<Cfg> cfgq[3];
    std::unique_ptr<Cfg> cfg;
    bool cfg_release=false;
    unsigned cfg_rr=0;
    long cfg_begin=0;
    struct Lane {
        unsigned lane=0,quota=0,index=0,stage=0;
        bool enabled=false,busy=false,finished=false;
    } lane[2];
    enum class AState {MAP_SRC,MAP_SRC_WAIT,MAP_DST,MAP_DST_WAIT,COPY,COPY_WAIT,
                       UNMAP_SRC,UNMAP_SRC_WAIT,UNMAP_DST,UNMAP_DST_WAIT,BATCH_WAIT,FINISH};
    AState a_state=AState::MAP_SRC;
    unsigned a_index=0,pending_regions=0;
    struct Flush {
        bool busy=false,cfg_busy=false;
        unsigned stage=0,first_goal=0,last_goal=0;
        U64 iova=0;bool wide=false;
        long begin=0;unsigned id=0;
    } flush;
    bool last_fence_decode=false;
    unsigned last_fence_av=0;
    unsigned previous_cq_state=0;

    explicit Tb(Options options):o(std::move(options)) {
        jsonl=std::fopen((o.out+"/events.jsonl").c_str(),"wx");
        csv=std::fopen((o.out+"/events.csv").c_str(),"wx");
        jobs_file=std::fopen((o.out+"/jobs.csv").c_str(),"wx");
        REQUIRE(jsonl&&csv&&jobs_file,"raw output already exists or cannot be opened");
        std::fprintf(csv,"cycle,era,event,lane,job,address,value,aux\n");
        std::fprintf(jobs_file,"era,lane,job,src_iova,dst_iova,src_pa,dst_pa,next_id,issue_cycle,done_cycle,poll_done_cycle,ar,aw,r_beats,w_beats,b\n");
        lane[0].lane=0;lane[1].lane=1;
    }
    ~Tb(){t->final();if(jsonl)std::fclose(jsonl);if(csv)std::fclose(csv);if(jobs_file)std::fclose(jobs_file);}
    void event(const char* name,int l=-1,int j=-1,U64 address=0,U64 value=0,U64 aux=0) {
        std::fprintf(jsonl,"{\"cycle\":%ld,\"era\":%s,\"event\":%s,\"lane\":%d,\"job\":%d,\"address\":%llu,\"value\":%llu,\"aux\":%llu}\n",
            cycle,quoted(era).c_str(),quoted(name).c_str(),l,j,(unsigned long long)address,(unsigned long long)value,(unsigned long long)aux);
        std::fprintf(csv,"%ld,%s,%s,%d,%d,%llu,%llu,%llu\n",cycle,era.c_str(),name,l,j,
            (unsigned long long)address,(unsigned long long)value,(unsigned long long)aux);
        recent.push_back(std::to_string(cycle)+","+era+","+name+","+std::to_string(l)+","+
            std::to_string(j)+","+std::to_string(address)+","+std::to_string(value)+","+std::to_string(aux));
        if(recent.size()>256)recent.pop_front();
    }
    void state(const char* name,U64 value) {
        auto it=previous_state.find(name);if(it==previous_state.end()||it->second!=value){
            previous_state[name]=value;event(name,-1,-1,0,value);
        }
    }
    Page& page(U64 a){return memory.try_emplace(a>>12,Page{}).first->second;}
    void zero_page(U64 a){page(a).fill(0);}
    U64 read64(U64 a) const {
        REQUIRE((a&7)==0,"unaligned memory read");auto it=memory.find(a>>12);
        REQUIRE(it!=memory.end(),"read outside initialized model memory");
        U64 v=0;for(unsigned i=0;i<8;++i)v|=U64(it->second[(a&4095)+i])<<(8*i);return v;
    }
    void write64(U64 a,U64 v,unsigned strb=255) {
        REQUIRE((a&7)==0,"unaligned memory write");Page& p=page(a);
        for(unsigned i=0;i<8;++i)if(strb&(1ULL<<i))p[(a&4095)+i]=uint8_t(v>>(8*i));
    }
    static U64 a_src(unsigned j){return 0x1ffff000ULL-U64(j)*8192;}
    static U64 a_dst(unsigned j){return a_src(j)-PAGE;}
    static U64 a_src_pa(unsigned j){return A_PA+U64(j)*8192;}
    static U64 a_dst_pa(unsigned j){return a_src_pa(j)+PAGE;}
    static U64 pte(U64 pa){return ((pa>>12)<<10)|0xDFULL;}// V/R/W/X/U/A/D; G=0
    static U64 pte_addr(U64 va,bool a){return (a?L0_A:L0_B)+((va>>12)&511)*8;}
    void setup_memory() {
        for(U64 pa:{DDT,DDT_ROOT,ROOT_B,ROOT_A,L1_B,L0_B,L1_A,L0_A,CQ_BASE,MARKER})zero_page(pa);
        // Legal two-level, base-format DDT. Original RTL's mode2 leaf index
        // unconditionally uses 64-byte stride even with MSI disabled. Mode3
        // uses 32-byte leaf entries. Restrict DIDs to 10/11 (<64), for which
        // its first-index [14:6] and the normative [15:7] are both zero.
        write64(DDT_ROOT,((DDT>>12)<<10)|1);
        write64(ROOT_B,((L1_B>>12)<<10)|1);
        write64(L1_B+((B_IOVA>>21)&511)*8,((L0_B>>12)<<10)|1);
        if(o.domain=="same")write64(L1_B+((a_src(0)>>21)&511)*8,((L0_A>>12)<<10)|1);
        else {write64(ROOT_A,((L1_A>>12)<<10)|1);write64(L1_A+((a_src(0)>>21)&511)*8,((L0_A>>12)<<10)|1);}
        for(unsigned l=0;l<2;++l){
            const U64 dc=DDT+U64(10+l)*32;
            write64(dc,1);write64(dc+8,0);// Valid, no ATS/PRI/PDTV; S2 Bare
            write64(dc+16,U64(l?o.pscid_a():B_PSCID)<<12);
            write64(dc+24,(8ULL<<60)|((l&&o.domain=="different"?ROOT_A:ROOT_B)>>12));
        }
        for(unsigned p=0;p<o.b_pages;++p){
            zero_page(B_PA+U64(p)*PAGE);
            write64(pte_addr(B_IOVA+U64(p)*PAGE,false),pte(B_PA+U64(p)*PAGE));
            for(unsigned b=0;b<512;++b)write64(B_PA+U64(p)*PAGE+b*8,p?0xDEAD000000000000ULL^p^b:pattern(0,b));
        }
        for(unsigned j=0;j<o.a_pages;++j){
            zero_page(a_src_pa(j));zero_page(a_dst_pa(j));
            for(unsigned b=0;b<512;++b){write64(a_src_pa(j)+b*8,pattern(j+1,b));write64(a_dst_pa(j)+b*8,0xBAD0000000000000ULL^j^b);}
            if(o.mode=="m0"){write64(pte_addr(a_src(j),true),pte(a_src_pa(j)));write64(pte_addr(a_dst(j),true),pte(a_dst_pa(j)));}
        }
    }
    static int payload_lane(unsigned id){return ((id>>4)&3)==2?int((id>>2)&1):-1;}
    int did_lane(unsigned did)const {REQUIRE(did==10||did==11,"unexpected DMA DID");return did==11;}
    void drive_memory() {
        t->mem_aw_ready=writeq.size()<16;t->mem_w_ready=1;t->mem_ar_ready=readq.size()<16;
        t->mem_b_valid=!bq.empty();t->mem_b_id=bq.empty()?0:bq.front().id;t->mem_b_resp=0;t->mem_b_user=0;
        t->mem_r_valid=0;t->mem_r_id=0;t->mem_r_data=0;t->mem_r_resp=0;t->mem_r_last=0;t->mem_r_user=0;
        if(readq.empty())return;
        Read& h=readq.front();
        // FIFO first-service latency: only the head ages, exactly once per cycle.
        // Sixteen complete low phases with RVALID=0, then one beat per cycle.
        if(h.latency>0){--h.latency;return;}
        t->mem_r_valid=1;t->mem_r_id=h.id;t->mem_r_data=read64(h.addr+U64(h.beat)*8);t->mem_r_last=h.beat==h.len;
    }
    void snapshot_inval() {
        event("invalidation",-1,-1,U64(t->dbg_inval_vpn)<<12,t->dbg_inval_pscid,
              unsigned(t->dbg_inval_av)|(unsigned(t->dbg_inval_pscv)<<1)|(unsigned(t->dbg_inval_gv)<<2));
        REQUIRE(!t->dbg_inval_gv,"experiment requires S2 Bare/GV0");
        std::set<Key> unique;
        for(unsigned i=0;i<IOTLB_ENTRIES;++i){
            const U64 vpn=bits(t->dbg_tags_vpn,i*27,27);const unsigned ps=bits(t->dbg_tags_pscid,i*20,20);
            const unsigned flags=((t->dbg_tags_valid>>i)&1)|(((t->dbg_tags_en1>>i)&1)<<1)|
                (((t->dbg_tags_en2>>i)&1)<<2)|(((t->dbg_tags_global>>i)&1)<<3);
            event("inval_tag_snapshot",-1,int(i),vpn<<12,ps,flags);
            if(!(flags&1)||(flags&4))continue;
            if(t->dbg_inval_pscv&&(ps!=t->dbg_inval_pscid||(flags&8)))continue;
            if(t->dbg_inval_av&&vpn!=t->dbg_inval_vpn)continue;
            pending_inval_ways|=1ULL<<i;
            Key key{ps,vpn};removed.insert(key);
            if(unique.insert(key).second)event("inval_removed_key",vpn>=(B_IOVA>>12)&&vpn<((B_IOVA>>12)+o.b_pages)?0:1,-1,vpn<<12,ps);
        }
    }
    void observe() {
        if(!t->rst_ni)return;
        REQUIRE(!t->dbg_fault,"IOMMU/CQ fault asserted; cause="+std::to_string(t->dbg_fault_cause));
        state("cq_state",t->dbg_cq_state);state("tl_state",t->dbg_tl_state);
        state("ptw_state",t->dbg_ptw_state);
        state("ptw_control",unsigned(t->dbg_ptw_init)|(unsigned(t->dbg_ptw_edge_trigger)<<1));
        if(t->dbg_lookup){
            const int l=did_lane(t->dbg_lookup_did);Key key{t->dbg_lookup_pscid,t->dbg_lookup_vpn};
            const bool hit=t->dbg_lookup_hit;unsigned cause=0;
            if(!hit){if(removed.count(key))cause=1;else cause=seen.count(key)?3:2;}
            // Sample lookup before invalidation: both observe pre-edge tags.
            // A same-edge invalidation then creates a marker for the NEXT access.
            removed.erase(key);seen.insert(key);
            if(measured){Counters& s=stats[l];if(hit)++s.hits;else{++s.misses;if(cause==1)++s.inval;else if(cause==2)++s.cold;else ++s.replacement;}}
            event("lookup",l,active[l]?int(active[l]->index):-1,U64(key.second)<<12,hit,U64(key.first)|(U64(cause)<<20));
            last_progress=cycle;
        }
        if(t->dbg_ptw_start){
            const int l=did_lane(t->dbg_walk_did);++all_walks;if(measured)++stats[l].walks;
            event("walk",l,active[l]?int(active[l]->index):-1,0,all_walks);last_progress=cycle;
        }
        if(t->dbg_ptw_read_fire){
            const int l=did_lane(t->dbg_walk_did);if(measured)stats[l].pte_bytes+=8;
            event("ptw_read",l,active[l]?int(active[l]->index):-1,0,8);
        }
        if(t->dbg_inval)snapshot_inval();
        if(t->dbg_cq_decode){
            event("cq_decode",-1,-1,t->dbg_cq_head,t->dbg_cq_opcode,t->dbg_cq_func3);
            if(measured){++decode_count;if(t->dbg_cq_opcode==1)++inv_count;else if(t->dbg_cq_opcode==2)++fence_count;else REQUIRE(false,"unexpected measured CQ opcode");}
            last_fence_decode=t->dbg_cq_opcode==2;
            last_fence_av=(read64(CQ_BASE+U64((t->dbg_cq_head+CQ_ENTRIES-1)%CQ_ENTRIES)*16)>>10)&1;
        }
        if(previous_cq_state!=0&&t->dbg_cq_state==0&&last_fence_decode){
            event("cq_fence_complete",-1,-1,0,last_fence_av,t->dbg_cq_head);last_fence_decode=false;
            if(measured)++fence_completed;
        }
        previous_cq_state=t->dbg_cq_state;
        if(t->dbg_cq_write_b_fire){++all_marker_b;event("cq_marker_b",-1,-1,MARKER,read64(MARKER));}
        if(measured){const U64 occupancy=(t->dbg_cq_tail+CQ_ENTRIES-t->dbg_cq_head)%CQ_ENTRIES;
            cq_peak=std::max(cq_peak,occupancy);cq_entry_cycles+=occupancy;}
        for(unsigned l=0;l<2;++l){
            if(t->dbg_dma_issue&(1u<<l)){
                Job* p=active[l];REQUIRE(p&&p->issue<0,"DMA ISSUE identity/duplicate");p->issue=cycle;
                if(measured)++measured_issue[l];event("copy_launch",l,p->index,p->src,p->dst,p->next_id);last_progress=cycle;
            }
        }
    }
    void observe_done() {
        for(unsigned l=0;l<2;++l)if(t->dbg_dma_done&(1u<<l)){
            Job* p=active[l];REQUIRE(p&&p->issue>=0&&p->done<0,"DMA DONE identity/duplicate");
            REQUIRE(p->r==512&&p->w==512&&p->b==2,"COPYDONE without full payload/B completion");
            p->done=cycle;if(measured){++measured_done[l];last_done[l]=cycle;}
            event("copy_done",l,p->index,p->dst,p->next_id);last_progress=cycle;
        }
    }
    void tick() {
        REQUIRE(cycle<CYCLE_LIMIT,"configuration cycle watchdog");
        REQUIRE(cycle-last_progress<STALL_LIMIT,"no-progress watchdog (see watchdog.json/recent events)");
        t->cfg_valid=bool(cfg);t->cfg_target=cfg?cfg->target:0;t->cfg_write=cfg?cfg->write:0;
        t->cfg_addr=cfg?cfg->addr:0;t->cfg_wdata=cfg?cfg->data:0;t->cfg_wstrb=cfg?cfg->strb:0;t->cfg_size=cfg?cfg->size:3;
        pending_inval_ways=0;
        t->clk_i=0;drive_memory();t->eval();
        const bool ar=t->mem_ar_valid&&t->mem_ar_ready,aw=t->mem_aw_valid&&t->mem_aw_ready;
        const bool w=t->mem_w_valid&&t->mem_w_ready,b=t->mem_b_valid&&t->mem_b_ready,r=t->mem_r_valid&&t->mem_r_ready;
        const bool sampled_inval=t->rst_ni&&t->dbg_inval;
        const U64 wd=t->mem_w_data;const unsigned ws=t->mem_w_strb;const bool wl=t->mem_w_last;
        Read nr;Write nw;
        if(t->rst_ni){
            observe();
            if(ar){
                REQUIRE(t->mem_ar_size==3&&t->mem_ar_burst==1,"DDR AR requires 64-bit INCR");
                nr.addr=t->mem_ar_addr;nr.id=t->mem_ar_id;nr.len=t->mem_ar_len;
                const int l=payload_lane(nr.id);if(l>=0){nr.job=active[l];REQUIRE(nr.job&&nr.job->issue>=0,"payload AR without issued COPY");++nr.job->ar;}
                event("ddr_ar",l,nr.job?int(nr.job->index):-1,nr.addr,nr.id,nr.len);
            }
            if(aw){
                REQUIRE(t->mem_aw_size==3&&t->mem_aw_burst==1,"DDR AW requires 64-bit INCR");
                nw.addr=t->mem_aw_addr;nw.id=t->mem_aw_id;nw.len=t->mem_aw_len;
                const int l=payload_lane(nw.id);if(l>=0){nw.job=active[l];REQUIRE(nw.job&&nw.job->issue>=0,"payload AW without issued COPY");++nw.job->aw;}
                event("ddr_aw",l,nw.job?int(nw.job->index):-1,nw.addr,nw.id,nw.len);
            }
            if(w){
                REQUIRE(!writeq.empty()||aw,"W without AW");const Write& h=writeq.empty()?nw:writeq.front();
                REQUIRE(wl==(h.beat==h.len),"WLAST/burst mismatch");
                if(h.job){Job& p=*h.job;const U64 a=h.addr+U64(h.beat)*8;
                    REQUIRE(a>=p.dst_pa&&a+8<=p.dst_pa+PAGE&&ws==255,"COPY W address/strobes");
                    const unsigned off=a-p.dst_pa;REQUIRE(wd==pattern(p.pattern_page,off/8),"COPY W data corruption");
                    for(unsigned i=0;i<8;++i)REQUIRE(++p.w_seen[off+i]==1,"duplicate destination byte");++p.w;
                }
                if(h.beat==0||wl)event("ddr_w_edge",h.job?int(h.job->lane):-1,h.job?int(h.job->index):-1,h.addr+U64(h.beat)*8,wd,ws);
            }
            if(r){
                REQUIRE(!readq.empty(),"R without AR");const Read& h=readq.front();
                if(h.job){Job& p=*h.job;const U64 a=h.addr+U64(h.beat)*8;
                    REQUIRE(a>=p.src_pa&&a+8<=p.src_pa+PAGE,"COPY R outside source");const unsigned off=a-p.src_pa;
                    REQUIRE(t->mem_r_data==pattern(p.pattern_page,off/8),"COPY R data corruption");
                    for(unsigned i=0;i<8;++i)REQUIRE(++p.r_seen[off+i]==1,"duplicate source byte");++p.r;
                }
            }
            if(b){REQUIRE(!bq.empty(),"B without AW/W");if(bq.front().job)++bq.front().job->b;
                event("ddr_b",bq.front().job?int(bq.front().job->lane):-1,bq.front().job?int(bq.front().job->index):-1,0,bq.front().id);}
            observe_done();
        }
        t->clk_i=1;t->eval();
        if(sampled_inval){
            REQUIRE((t->dbg_tags_valid&pending_inval_ways)==0,"invalidation predicted way remained valid after rising edge");
            unsigned n=0;for(unsigned i=0;i<IOTLB_ENTRIES;++i)n+=(pending_inval_ways>>i)&1;
            event("inval_post_edge_verified",-1,-1,0,pending_inval_ways,n);
            if(measured){++inval_verified_events;inval_verified_ways+=n;}
        }
        if(ar)readq.push_back(nr);if(aw)writeq.push_back(nw);
        if(w){REQUIRE(!writeq.empty(),"model W queue empty");Write& h=writeq.front();write64(h.addr+U64(h.beat)*8,wd,ws);++h.beat;
            if(wl){bq.push_back(B{h.id,h.job});writeq.pop_front();}}
        if(b)bq.pop_front();
        if(r){if(readq.front().beat==readq.front().len)readq.pop_front();else ++readq.front().beat;}
        t->clk_i=0;t->eval();++cycle;
    }
    bool owner_pending(unsigned owner)const {
        return !cfgq[owner].empty()||(cfg&&cfg->target==owner); // queues map owner=target for this driver
    }
    void request(unsigned target,bool write,U64 addr,U64 data,unsigned size,std::function<void(U64)> cb) {
        REQUIRE(target<3,"bad MMIO target");Cfg c;c.target=target;c.write=write;c.addr=addr;c.size=size;c.done=std::move(cb);
        const unsigned shift=size==2?unsigned(addr&7)*8:0;c.data=data<<shift;c.strb=write?(size==2?0x0fu<<(addr&7):0xffu):0;
        cfgq[target].push_back(std::move(c));
    }
    void step() {
        const bool releasing=cfg_release;
        if(!cfg&&!releasing){for(unsigned i=0;i<3;++i){const unsigned target=(cfg_rr+i)%3;
            if(!cfgq[target].empty()){cfg.reset(new Cfg(std::move(cfgq[target].front())));cfgq[target].pop_front();cfg_rr=(target+1)%3;cfg_begin=cycle;break;}}}
        tick();
        if(releasing){cfg_release=false;return;}
        if(cfg){REQUIRE(cycle-cfg_begin<100000,"MMIO transaction watchdog");if(t->cfg_ready){
            REQUIRE(!t->cfg_error,"real AXI MMIO error");U64 value=t->cfg_rdata;if(cfg->size==2)value=(value>>((cfg->addr&7)*8))&0xffffffffULL;
            auto cb=std::move(cfg->done);event("mmio_complete",cfg->target,-1,cfg->addr,value,cfg->write);
            cfg.reset();cfg_release=true;cb(value);
        }}
    }
    U64 blocking(unsigned target,bool write,U64 addr,U64 data=0,unsigned size=3) {
        bool done=false;U64 value=0;request(target,write,addr,data,size,[&](U64 v){value=v;done=true;});
        while(!done)step();while(cfg_release)step();return value;
    }
    void reset_and_program() {
        t->rst_ni=0;for(unsigned i=0;i<8;++i)step();t->rst_ni=1;for(unsigned i=0;i<4;++i)step();
        const U64 ddtp=(DDT_ROOT>>2)|3;
        blocking(2,true,0x10,uint32_t(ddtp),2);blocking(2,true,0x14,ddtp>>32,2);
        REQUIRE(blocking(2,false,0x10,0,2)==uint32_t(ddtp),"DDTP low readback");
        const U64 cqb=(CQ_BASE>>2)|7;
        blocking(2,true,0x18,uint32_t(cqb),2);blocking(2,true,0x1c,cqb>>32,2);
        tail=blocking(2,false,0x20,0,2);blocking(2,true,0x24,tail,2);blocking(2,true,0x48,1,2);
        for(unsigned i=0;i<100;++i){if(blocking(2,false,0x48,0,2)&0x10000)return;}
        REQUIRE(false,"CQON not visible through real programming MMIO");
    }
    bool quiet()const{return t->dbg_dma_quiet==3&&!t->dbg_iommu_in_flight&&t->dbg_cq_state==0&&readq.empty()&&writeq.empty()&&bq.empty();}
    void drain(){for(unsigned i=0;i<100000;++i){step();if(quiet()&&!cfg&&cfgq[0].empty()&&cfgq[1].empty()&&cfgq[2].empty()&&!cfg_release)return;}REQUIRE(false,"quiet watchdog");}
    Job* make_job(unsigned l,unsigned j) {
        auto p=std::make_unique<Job>();p->lane=l;p->index=j;
        if(l){const unsigned slot=j%o.a_pages;p->src=a_src(slot);p->dst=a_dst(slot);p->src_pa=a_src_pa(slot);p->dst_pa=a_dst_pa(slot);p->pattern_page=slot+1;}
        else {const unsigned dest=1+j%(o.b_pages-1);p->src=B_IOVA;p->dst=B_IOVA+U64(dest)*PAGE;p->src_pa=B_PA;p->dst_pa=B_PA+U64(dest)*PAGE;}
        Job* ptr=p.get();jobs.push_back(std::move(p));return ptr;
    }
    void init_lane(unsigned l,unsigned quota) {lane[l]=Lane{};lane[l].lane=l;lane[l].quota=quota;lane[l].enabled=true;lane[l].finished=quota==0;}
    void finish_job(Job& p) {
        REQUIRE(p.done>=0&&p.issue>=0,"MMIO DONE arrived before true hardware completion");
        REQUIRE(p.ar==2&&p.aw==2&&p.r==512&&p.w==512&&p.b==2,"COPY transfer count mismatch");
        for(unsigned b=0;b<512;++b){REQUIRE(read64(p.dst_pa+b*8)==pattern(p.pattern_page,b),"full destination page mismatch");REQUIRE(read64(p.src_pa+b*8)==pattern(p.pattern_page,b),"source page mutated");}
        for(unsigned i=0;i<PAGE;++i)REQUIRE(p.r_seen[i]==1&&p.w_seen[i]==1,"COPY byte coverage");
        std::fprintf(jobs_file,"%s,%u,%u,%llu,%llu,%llu,%llu,%llu,%ld,%ld,%ld,%llu,%llu,%llu,%llu,%llu\n",era.c_str(),p.lane,p.index,
            (unsigned long long)p.src,(unsigned long long)p.dst,(unsigned long long)p.src_pa,(unsigned long long)p.dst_pa,(unsigned long long)p.next_id,
            p.issue,p.done,p.poll_done,(unsigned long long)p.ar,(unsigned long long)p.aw,(unsigned long long)p.r,(unsigned long long)p.w,(unsigned long long)p.b);
    }
    void service_lane(unsigned l) {
        Lane& s=lane[l];if(!s.enabled||s.finished||s.busy)return;
        if(s.stage==0){REQUIRE(!active[l],"host reused active DMA slot");active[l]=make_job(l,s.index);}
        Job* p=active[l];s.busy=true;
        const U64 addrs[]={0x18,0,8,0x10,REG_NEXT,REG_DONE};
        const U64 vals[]={0,p->src,p->dst,PAGE,0,0};const unsigned stage=s.stage;
        request(l,stage<4,addrs[stage],vals[stage],3,[this,l,p,stage](U64 value){
            Lane& x=lane[l];x.busy=false;
            if(stage==4){p->next_id=value;event("copy_next_id",l,p->index,0,value);x.stage=5;}
            else if(stage==5){if(value==p->next_id){p->poll_done=cycle;event("copy_host_done",l,p->index,p->dst,value);finish_job(*p);active[l]=nullptr;
                    ++x.index;x.stage=0;if(x.index==x.quota)x.finished=true;}
                else x.stage=5;}
            else ++x.stage;
        });
    }
    bool tags_contain(unsigned ps,U64 vpn)const {
        for(unsigned i=0;i<IOTLB_ENTRIES;++i)if(((t->dbg_tags_valid>>i)&1)&&bits(t->dbg_tags_pscid,i*20,20)==ps&&bits(t->dbg_tags_vpn,i*27,27)==vpn)return true;return false;
    }
    void warm() {
        // Prime the actual A footprint too; A1 must be a zero-miss co-run.
        if(o.a_pages){era="warm-a";init_lane(1,256);
            while(!lane[1].finished){service_lane(1);step();}drain();lane[1].enabled=false;}
        era="warm";init_lane(0,64);while(!lane[0].finished){service_lane(0);step();}drain();
        for(unsigned p=0;p<o.b_pages;++p)REQUIRE(tags_contain(B_PSCID,(B_IOVA+U64(p)*PAGE)>>12),"B warm did not leave all pages resident");
        warm_resident=true;event("warm_resident",0,-1,0,o.b_pages);
        const U64 before=all_walks;era="warm-check";init_lane(0,2*(o.b_pages-1));
        while(!lane[0].finished){service_lane(0);step();}drain();
        REQUIRE(all_walks==before,"B-only post-warm validation incurred extra walks");warm_no_walk=true;
        event("warm_zero_extra_walks",0,-1,0,all_walks-before);lane[0].enabled=false;
    }
    void put_command(U64 lo,U64 hi) {
        const unsigned head=t->dbg_cq_head;REQUIRE((tail+1)%CQ_ENTRIES!=head,"command queue overflow");
        const U64 addr=CQ_BASE+U64(tail)*16;write64(addr,lo);write64(addr+8,hi);
        event("cq_submit",-1,-1,addr,lo&127,(lo>>7)&7);if(measured)++submit_count;tail=(tail+1)%CQ_ENTRIES;
    }
    void start_flush(U64 iova,bool force_wide=false) {
        REQUIRE(!flush.busy,"overlapping A flush groups");flush=Flush{};flush.busy=true;flush.iova=iova;flush.wide=force_wide;
        flush.begin=cycle;flush.id=unsigned(rounds);if(measured)++rounds;
        bool scoped=!force_wide&&(o.mode=="m2"||o.mode=="m3"),av=scoped&&o.mode=="m3";
        U64 lo=1|(U64(av)<<10)|(scoped?(U64(o.pscid_a())<<12)|(1ULL<<32):0);
        put_command(lo,av?(iova>>2):0);put_command(2,0);flush.first_goal=tail;
        event("cq_round_begin",1,int(a_index),iova,flush.id,scoped|(unsigned(av)<<1));
    }
    void service_flush() {
        if(!flush.busy||flush.cfg_busy)return;flush.cfg_busy=true;
        if(flush.stage==0||flush.stage==2){
            const unsigned count=flush.stage==0?2:1;
            request(2,true,0x24,tail,2,[this,count](U64){flush.cfg_busy=false;event("cq_doorbell",1,int(a_index),0,tail,count);if(measured)++doorbells;++flush.stage;});
        } else {
            request(2,false,0x20,0,2,[this](U64 head){flush.cfg_busy=false;
                event("cq_head_poll",1,int(a_index),0,head,flush.stage==1?flush.first_goal:flush.last_goal);
                const unsigned goal=flush.stage==1?flush.first_goal:flush.last_goal;if(head!=goal)return;
                if(flush.stage==1){event("cq_first_head_seen",1,int(a_index),flush.iova,flush.id,head);put_command(2,0);flush.last_goal=tail;flush.stage=2;}
                else {event("cq_round_complete",1,int(a_index),flush.iova,flush.id,cycle-flush.begin);if(measured)total_waiting+=cycle-flush.begin;flush.busy=false;}
            });
        }
    }
    void map_region(bool src) {
        U64 va=src?a_src(a_index):a_dst(a_index),pa=src?a_src_pa(a_index):a_dst_pa(a_index);
        write64(pte_addr(va,true),pte(pa));++map_regions;event("map_region",1,int(a_index),va,pa,PAGE);
        if(o.map_policy=="full-api")start_flush(va,o.mode=="m4-16"||o.mode=="m4-64");
    }
    void unmap_region(bool src) {
        U64 va=src?a_src(a_index):a_dst(a_index),pa=src?a_src_pa(a_index):a_dst_pa(a_index);
        write64(pte_addr(va,true),0);++unmap_regions;++pending_regions;event("unmap_region",1,int(a_index),va,pa,PAGE);
        if(!o.batch())start_flush(va);
    }
    void service_a() {
        if(flush.busy){service_flush();return;}
        switch(a_state){
        case AState::MAP_SRC:if(o.mode!="m0")map_region(true);a_state=AState::MAP_SRC_WAIT;break;
        case AState::MAP_SRC_WAIT:a_state=AState::MAP_DST;break;
        case AState::MAP_DST:if(o.mode!="m0")map_region(false);a_state=AState::MAP_DST_WAIT;break;
        case AState::MAP_DST_WAIT:a_state=AState::COPY;break;
        case AState::COPY:init_lane(1,1);lane[1].index=a_index;lane[1].quota=a_index+1;a_state=AState::COPY_WAIT;break;
        case AState::COPY_WAIT:service_lane(1);if(lane[1].finished){lane[1].enabled=false;a_state=AState::UNMAP_SRC;}break;
        case AState::UNMAP_SRC:if(o.mode!="m0")unmap_region(true);a_state=AState::UNMAP_SRC_WAIT;break;
        case AState::UNMAP_SRC_WAIT:a_state=AState::UNMAP_DST;break;
        case AState::UNMAP_DST:if(o.mode!="m0")unmap_region(false);a_state=AState::UNMAP_DST_WAIT;break;
        case AState::UNMAP_DST_WAIT:
            if(o.batch()&&(pending_regions>=o.batch()||a_index+1==o.a_jobs)){start_flush(0,true);pending_regions=0;a_state=AState::BATCH_WAIT;}
            else a_state=AState::BATCH_WAIT;break;
        case AState::BATCH_WAIT:
            ++a_index;if(a_index==o.a_jobs){final_a=cycle;a_state=AState::FINISH;event("a_final_flush_complete",1,-1,0,cycle-t0);}
            else a_state=AState::MAP_SRC;break;
        case AState::FINISH:break;
        }
    }
    void smoke_marker() {
        era="smoke-marker";const U64 token=0x13579BDF;
        // A VMA command, AV1 fence marker, then AV0 sentinel. The marker
        // owns a private 8-byte slot because this RTL writes eight bytes.
        put_command(1,0);put_command(2|(1ULL<<10)|(token<<32),MARKER>>2);put_command(2,0);
        const unsigned goal=tail;blocking(2,true,0x24,tail,2);
        for(unsigned n=0;n<10000;++n){if(blocking(2,false,0x20,0,2)==goal)break;REQUIRE(n+1<10000,"marker/sentinel CQH timeout");}
        REQUIRE(read64(MARKER)==token,"AV1 marker memory mismatch");REQUIRE(all_marker_b==1,"AV1 marker did not complete through CQ B");
        drain();event("smoke_marker_verified",-1,-1,MARKER,token,all_marker_b);
    }
    void verify_counts() {
        REQUIRE(measured_done[0]==o.b_jobs&&measured_done[1]==o.a_jobs,"measured COPY count mismatch");
        REQUIRE(measured_issue[0]==o.b_jobs&&measured_issue[1]==o.a_jobs,"measured ISSUE count mismatch");
        U64 expected_rounds=0;if(o.mode!="m0"){
            const U64 maps=o.map_policy=="full-api"?2*o.a_jobs:0;
            const U64 unmaps=o.batch()?(2*o.a_jobs+o.batch()-1)/o.batch():2*o.a_jobs;
            expected_rounds=maps+unmaps;
            REQUIRE(map_regions==2*o.a_jobs&&unmap_regions==2*o.a_jobs,"map/unmap region count mismatch");
        }else REQUIRE(!map_regions&&!unmap_regions,"M0 persistent mapping changed during measurement");
        REQUIRE(rounds==expected_rounds&&inv_count==expected_rounds&&fence_count==2*expected_rounds&&fence_completed==2*expected_rounds,"CQ opcode/completion counts mismatch");
        REQUIRE(submit_count==3*expected_rounds&&decode_count==submit_count&&doorbells==2*expected_rounds,"CQ submit/decode/doorbell conservation");
        REQUIRE(inval_verified_events==expected_rounds,"invalidation pulse/post-edge verification conservation");
        for(unsigned l=0;l<2;++l)REQUIRE(stats[l].misses==stats[l].inval+stats[l].cold+stats[l].replacement,"miss attribution partition");
        commands_ok=true;
    }
    void run() {
        setup_memory();reset_and_program();drain();warm();
        era="measure";t0=cycle;measured=true;last_progress=cycle;event("measure_begin");
        init_lane(0,o.b_jobs);init_lane(1,o.a_pages?1000000:0);
        while(!lane[0].finished){service_lane(0);service_lane(1);step();}
        // Stop issuing A work after V completes, then finish the one legal
        // in-progress COPY. Report this tail separately from the V window.
        if(active[1]){lane[1].quota=lane[1].index+1;
            while(!lane[1].finished){service_lane(1);step();}}
        else {lane[1].enabled=false;lane[1].finished=true;}
        o.a_jobs=measured_done[1];final_a=cycle;drain();
        quiet_ok=quiet();REQUIRE(quiet_ok,"final model/hardware not quiet");
        REQUIRE(measured_done[0]==o.b_jobs,"victim job conservation");
        REQUIRE(!inv_count&&!fence_count&&!rounds,"capacity run submitted invalidation");
        REQUIRE(!stats[0].inval&&!stats[1].inval,"capacity run has invalidation miss");
        if(o.a_pages<=1)REQUIRE(!stats[0].walks&&!stats[1].walks,"Solo/A1 must have zero post-warm walks");
        data_ok=true;commands_ok=true;measure_end=cycle;
        event("measure_end");measured=false;passed=true;
    }
    void dump_failure() {
        std::FILE* f=std::fopen((o.out+"/watchdog.json").c_str(),"wx");if(!f)return;
        std::fprintf(f,"{\"failure\":%s,\"cycle\":%ld,\"fault_cause\":%u,\"cq_head\":%u,\"cq_tail\":%u,\"cq_csr\":%u,\"cq_state\":%u,\"tl_state\":%u,\"ptw_state\":%u,\"ptw_init\":%u,\"ptw_edge_trigger\":%u,\"lookup\":%u,\"lookup_hit\":%u,\"lookup_vpn\":%u,\"inval\":%u,\"recent\":[",
            quoted(failure).c_str(),cycle,t->dbg_fault_cause,t->dbg_cq_head,t->dbg_cq_tail,t->dbg_cq_csr,t->dbg_cq_state,t->dbg_tl_state,t->dbg_ptw_state,t->dbg_ptw_init,t->dbg_ptw_edge_trigger,
            t->dbg_lookup,t->dbg_lookup_hit,t->dbg_lookup_vpn,t->dbg_inval);
        bool first=true;for(const std::string& s:recent){if(!first)std::fputc(',',f);first=false;std::fputs(quoted(s).c_str(),f);}std::fputs("]}\n",f);std::fclose(f);
    }
    void summary() {
        FILE* f=std::fopen((o.out+"/summary.json").c_str(),"wx");REQUIRE(f,"summary already exists");
        const std::string base=o.domain+"-b"+std::to_string(o.b_pages)+"-m0-full-api";
        std::fprintf(f,"{\"schema_version\":1,\"config\":{\"case_id\":%s,\"domain\":%s,\"b_pages\":%u,\"mode\":%s,\"map_policy\":%s,\"a_jobs\":%u,\"b_jobs\":%u,\"bytes_per_copy\":4096,\"ddt_mode\":3,\"did_b\":10,\"did_a\":11,\"ddr_first_cycles\":16,\"ddr_beat_cycles\":1,\"baseline_case_id\":%s,\"smoke\":%s},\"timing\":{\"t0\":%ld,\"end_cycle\":%ld},\"metrics\":{",
            quoted(o.case_id).c_str(),quoted(o.domain).c_str(),o.b_pages,quoted(o.mode).c_str(),quoted(o.map_policy).c_str(),o.a_jobs,o.b_jobs,quoted(base).c_str(),o.smoke?"true":"false",t0,measure_end);
        bool first=true;auto m=[&](const char* name,long long value){std::fprintf(f,"%s%s:%lld",first?"":",",quoted(name).c_str(),value);first=false;};
        m("T_A",last_done[1]<0?-1:last_done[1]-t0);m("T_B",last_done[0]<0?-1:last_done[0]-t0);
        m("A_final_flush_end",final_a<0?-1:final_a-t0);m("T_end",final_a<0||last_done[0]<0?-1:std::max(final_a,last_done[0])-t0);
        for(unsigned l=0;l<2;++l){std::string p=l?"A_":"B_";const Counters&s=stats[l];
            m((p+"hits").c_str(),s.hits);m((p+"misses").c_str(),s.misses);m((p+"walks").c_str(),s.walks);
            m((p+"invalidation_attributed_misses").c_str(),s.inval);m((p+"cold_misses").c_str(),s.cold);m((p+"replacement_misses").c_str(),s.replacement);
            m((p+"ptw_memory_bytes").c_str(),s.pte_bytes);m((p+"completed").c_str(),measured_done[l]);}
        m("cq_iotinval",inv_count);m("cq_iofence",fence_count);m("cq_decode_count",decode_count);m("cq_submit_commands",submit_count);
        m("cq_fence_completed",fence_completed);m("inval_post_edge_verified_events",inval_verified_events);m("inval_post_edge_verified_ways",inval_verified_ways);
        m("cq_doorbells",doorbells);m("cq_peak",cq_peak);m("cq_entry_cycles",cq_entry_cycles);m("cq_rounds",rounds);m("cq_total_waiting",total_waiting);
        m("map_regions",map_regions);m("unmap_regions",unmap_regions);
        std::fprintf(f,"},\"checks\":{\"passed\":%s,\"data\":%s,\"quiet\":%s,\"commands\":%s,\"warm_resident\":%s,\"warm_no_extra_walks\":%s},\"failure\":%s,\"limitations\":[\"Host PTE writes are direct model memory writes; CPU cost is not counted.\",\"Fixed F5 legacy RTL with disclosed functional repairs; not an unchanged stock execution.\",\"Legal two-level 32-byte DDT with DID10/11 below64; common extra root read avoids original one-level 64-byte stride defect without changing RTL.\",\"M4 models delayed unmap; no claim that Linux C provides this FQ policy.\",\"DDR startup16 at FIFO head, one 64-bit beat per cycle, no read reordering.\"]}\n",
            passed?"true":"false",data_ok?"true":"false",quiet_ok?"true":"false",commands_ok?"true":"false",warm_resident?"true":"false",warm_no_walk?"true":"false",failure.empty()?"null":quoted(failure).c_str());std::fclose(f);
    }
};
int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);Options o;
    for(int i=1;i<argc;++i){std::string k=argv[i];auto val=[&](){REQUIRE(i+1<argc,"missing argument value");return std::string(argv[++i]);};
        if(k=="--out")o.out=val();else if(k=="--mode")o.mode=val();else if(k=="--domain")o.domain=val();
        else if(k=="--b-pages")o.b_pages=std::stoul(val());else if(k=="--map-policy")o.map_policy=val();
        else if(k=="--case-id")o.case_id=val();else if(k=="--a-pages")o.a_pages=std::stoul(val());else if(k=="--v-jobs")o.b_jobs=std::stoul(val());else if(k=="--smoke")o.smoke=true;else throw Failure("unknown option: "+k);}
    REQUIRE(!o.out.empty(),"--out required");REQUIRE(o.domain=="same"||o.domain=="different","domain");
    REQUIRE(o.b_pages==2,"V uses one source and one destination page");REQUIRE(o.a_pages==0||o.a_pages==1||o.a_pages==8||o.a_pages==64,"A footprint");REQUIRE(o.b_jobs>0&&o.b_jobs<=4096,"V job range");REQUIRE(o.map_policy=="full-api"||o.map_policy=="unmap-only","map policy");
    REQUIRE(o.mode=="m0"||o.mode=="m1"||o.mode=="m2"||o.mode=="m3"||o.mode=="m4-16"||o.mode=="m4-64","mode");
    if(o.smoke){o.a_jobs=1;o.b_jobs=8;}
    if(o.case_id.empty())o.case_id=o.domain+"-b"+std::to_string(o.b_pages)+"-"+o.mode+"-"+o.map_policy;
    o.mode="m0";Tb tb(o);try{tb.run();}catch(const std::exception&e){tb.failure=e.what();tb.event("failure",-1,-1,0,0);tb.dump_failure();}
    tb.summary();std::printf("RESULT %s cycle=%ld failure=%s\n",tb.passed?"PASS":"FAIL",tb.cycle,tb.failure.c_str());return tb.passed?0:1;
}
