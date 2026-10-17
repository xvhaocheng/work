// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Isolated F5 legacy-IOMMU/real-iDMA behavioral-DDR experiment.
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
    std::string out,mode="ml-strict",domain="different",map_policy="unmap-only",case_id,fence_policy="global";
    unsigned b_pages=4,a_jobs=64,b_jobs=0,b_bytes=4096,b_gap=0;
    bool smoke=false,a_alone=false,b_background=false;
    bool mainline()const{return mode=="ml-strict"||mode=="ml-fq"||mode=="oracle";}
    bool fq()const{return mode=="ml-fq"||mode=="oracle";}
    unsigned pscid_a()const{return B_PSCID+(domain=="different");}
    unsigned batch()const{return mode=="m4-16"?16:mode=="m4-64"?64:0;}
};
class Tb {
public:
    Options o;
    std::unique_ptr<Vinvalidation_dma_top> t{new Vinvalidation_dma_top};
    FILE *jsonl=nullptr,*csv=nullptr,*jobs_file=nullptr;
    FILE *dead_file=nullptr,*latency_file=nullptr,*fences_file=nullptr;
    long cycle=0,t0=-1,measure_end=-1,final_a=-1,last_done[2]={-1,-1};
    std::string era="initial",failure;
    bool measured=false,passed=false,data_ok=false,quiet_ok=false,commands_ok=false;
    bool warm_resident=false,warm_no_walk=false;
    using Key=std::pair<unsigned,U64>;
    std::set<Key> seen,removed;
    std::set<Key> dead,reserved_iovas;
    std::deque<Key> fq_entries;
    static constexpr long FQ_TIMEOUT_CYCLES=500000; // 10ms at declared 50MHz, HZ=1000 equivalence.
    static constexpr unsigned FQ_CAPACITY=255;
    long fq_due=-1,fq_drain_end=-1;
    bool flush_fq=false,fq_waiting_enqueue=false;
    Key waiting_enqueue;
    U64 fq_flushes=0,fq_peak=0,fq_reclaimed=0,fq_timer_flushes=0,fq_full_flushes=0;
    U64 dead_slot_cycles=0,dead_sample_cycles=0,dead_valid_cycles=0,dead_peak=0;
    U64 dead_replacements=0,live_replacements=0,empty_fills=0;
    unsigned previous_dead_mask=~0u,previous_valid_mask=~0u;
    bool oracle_pending=false;
    U64 wall_host=0,wall_flush=0,wall_copy_pte=0,wall_copy_payload_wait=0,wall_copy_other=0;
    U64 flush_cq_fetch=0,flush_cq_decode=0,flush_cq_fence=0,flush_other=0;
    U64 comp_reads=0,comp_writes=0,payload_reads_done=0,payload_writes_done=0;
    U64 fence_read_goal=0,fence_write_goal=0,prpw_fences=0,prpw_wait_cycles=0;
    bool prpw_active=false;
    U64 pte_a_pending=0;
    U64 comp_lane[2][2]={{0,0},{0,0}},done_lane[2][2]={{0,0},{0,0}};
    U64 goals[2][2]={{0,0},{0,0}},done_now[2][2]={{0,0},{0,0}};
    U64 work_comp_base[2][2]={{0,0},{0,0}},work_done_base[2][2]={{0,0},{0,0}};
    U64 b_offered=0,b_overlap_busy=0,b_window_read_bytes=0,b_window_write_bytes=0;
    U64 completed_bytes[2]={0,0},completed_rbeats[2]={0,0},completed_wbeats[2]={0,0};
    U64 completed_ar[2]={0,0},completed_aw[2]={0,0},completed_b[2]={0,0};
    U64 prpw_blocked_cycles=0,cohort_wait[2]={0,0};
    long b_next_offer=0,b_stop=-1;
    struct FenceTrace {
        long submit=-1,inval_decode=-1,inval=-1,fence_submit=-1,decode=-1,complete=-1,idle=-1;
        U64 fetch=0,reg=0,decode_cycles=0,wait=0,blocked=0,other=0,host_samples=0;
        U64 pending[2]={0,0},goal[2][2]={{0,0},{0,0}},snapshot_done[2][2]={{0,0},{0,0}};
        U64 same_ar[2]={0,0},same_aw[2]={0,0},same_rlast[2]={0,0},same_b[2]={0,0};
        U64 inval_lo=0,inval_hi=0,fence_lo=0,fence_hi=0;
        unsigned head_at_decode=0,head_at_idle=0;
    } fence_trace;
    bool trace_open=false;
    struct CmdLatency {long submit=-1,decode=-1,inval=-1;U64 cq_fetch=0,cq_decode=0,cq_fence=0,other=0;};
    std::deque<CmdLatency> cmd_latency;
    U64 command_latency_count=0,command_latency_sum=0,command_latency_min=~U64(0),command_latency_max=0;
    struct Counters {U64 hits=0,misses=0,walks=0,inval=0,cold=0,replacement=0,pte_bytes=0;};
    Counters stats[2];
    U64 all_walks=0,all_marker_b=0;
    U64 inv_count=0,fence_count=0,fence_completed=0,decode_count=0,submit_count=0,doorbells=0;
    U64 inval_verified_events=0,inval_verified_ways=0;
    unsigned pending_inval_ways=0;
    U64 cq_peak=0,cq_entry_cycles=0,rounds=0,total_waiting=0,map_regions=0,unmap_regions=0;
    U64 measured_issue[2]={0,0},measured_done[2]={0,0};
    unsigned tail=0;
    long last_progress=0;
    std::map<std::string,U64> previous_state;
    std::deque<std::string> recent;
    using Page=std::array<uint8_t,PAGE>;
    std::unordered_map<U64,Page> memory;
    struct Job {
        unsigned lane=0,index=0,pattern_page=0,bytes=PAGE;
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
        dead_file=std::fopen((o.out+"/dead-trace.csv").c_str(),"wx");
        latency_file=std::fopen((o.out+"/command-latency.csv").c_str(),"wx");
        fences_file=std::fopen((o.out+"/fences.csv").c_str(),"wx");
        REQUIRE(fences_file,"fences output already exists or cannot be opened");
        std::fprintf(fences_file,"fence,iova,inval_lo,inval_hi,fence_lo,fence_hi,submit,inval_decode,inval_pulse,fence_submit,fence_decode,fence_complete_edge,fence_complete_idle,host_return,fetch_cycles,register_cycles,decode_cycles,wait_prpw_cycles,prpw_blocked_cycles,other_cycles,host_samples,A_pending_cycles,B_pending_cycles,B_read_goal,B_read_done_snapshot,B_write_goal,B_write_done_snapshot,A_read_goal,A_read_done_snapshot,A_write_goal,A_write_done_snapshot,B_same_ar,B_same_aw,B_same_rlast,B_same_b,A_same_ar,A_same_aw,A_same_rlast,A_same_b,head_decode,head_idle,head_host
");
        REQUIRE(jsonl&&csv&&jobs_file&&dead_file&&latency_file,"raw output already exists or cannot be opened");
        std::fprintf(dead_file,"cycle,event,valid_mask,dead_mask,victim_way,victim_dead,victim_valid,victim_pscid,victim_vpn\n");
        std::fprintf(latency_file,"command,submit,inval_decode,inval_pulse,first_fence_complete,submit_to_fence,cq_fetch_register,cq_decode,cq_fence,other\n");
        std::fprintf(csv,"cycle,era,event,lane,job,address,value,aux\n");
        std::fprintf(jobs_file,"era,lane,job,src_iova,dst_iova,src_pa,dst_pa,next_id,issue_cycle,done_cycle,poll_done_cycle,ar,aw,r_beats,w_beats,b,bytes\n");
        lane[0].lane=0;lane[1].lane=1;
    }
    ~Tb(){t->final();if(jsonl)std::fclose(jsonl);if(csv)std::fclose(csv);if(jobs_file)std::fclose(jobs_file);if(dead_file)std::fclose(dead_file);if(latency_file)std::fclose(latency_file);if(fences_file)std::fclose(fences_file);}
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
        for(unsigned i=0;i<8;++i)if(strb&(1u<<i))p[(a&4095)+i]=uint8_t(v>>(8*i));
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
        for(unsigned j=0;j<o.a_jobs;++j){
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
        for(auto it=dead.begin();it!=dead.end();){
            if((!t->dbg_inval_pscv||it->first==t->dbg_inval_pscid)&&
               (!t->dbg_inval_av||it->second==t->dbg_inval_vpn))it=dead.erase(it);else ++it;
        }
        if(!cmd_latency.empty())cmd_latency.front().inval=cycle;
        std::set<Key> unique;
        for(unsigned i=0;i<16;++i){
            const U64 vpn=bits(t->dbg_tags_vpn,i*27,27);const unsigned ps=bits(t->dbg_tags_pscid,i*20,20);
            const unsigned flags=((t->dbg_tags_valid>>i)&1)|(((t->dbg_tags_en1>>i)&1)<<1)|
                (((t->dbg_tags_en2>>i)&1)<<2)|(((t->dbg_tags_global>>i)&1)<<3);
            event("inval_tag_snapshot",-1,int(i),vpn<<12,ps,flags);
            if(!(flags&1)||(flags&4))continue;
            if(t->dbg_inval_pscv&&(ps!=t->dbg_inval_pscid||(flags&8)))continue;
            if(t->dbg_inval_av&&vpn!=t->dbg_inval_vpn)continue;
            pending_inval_ways|=1u<<i;
            Key key{ps,vpn};removed.insert(key);
            if(unique.insert(key).second)event("inval_removed_key",vpn>=(B_IOVA>>12)&&vpn<((B_IOVA>>12)+o.b_pages)?0:1,-1,vpn<<12,ps);
        }
    }
    bool work_window()const {
        return measured&&final_a<0;
    }
    unsigned dead_mask()const {
        unsigned mask=0;
        for(unsigned i=0;i<16;++i)if((t->dbg_tags_valid>>i)&1){
            Key key{unsigned(bits(t->dbg_tags_pscid,i*20,20)),bits(t->dbg_tags_vpn,i*27,27)};
            if(dead.count(key))mask|=1u<<i;
        }
        return mask;
    }
    void observe_dead_and_time() {
        const unsigned mask=dead_mask(),valid=t->dbg_tags_valid;
        const unsigned n=__builtin_popcount(mask),nv=__builtin_popcount(valid);
        if(work_window()){
            dead_slot_cycles+=n;++dead_sample_cycles;dead_valid_cycles+=nv;dead_peak=std::max(dead_peak,U64(n));
            if(mask!=previous_dead_mask||valid!=previous_valid_mask)
                std::fprintf(dead_file,"%ld,occupancy,%u,%u,-1,-1,-1,0,0\n",cycle,valid,mask);
        }
        previous_dead_mask=mask;previous_valid_mask=valid;
        if(t->dbg_iotlb_fill){
            unsigned victims=t->dbg_iotlb_replace_en;
            REQUIRE(__builtin_popcount(victims)==1,"IOTLB update must select exactly one victim");
            unsigned way=__builtin_ctz(victims);bool is_valid=(valid>>way)&1,is_dead=(mask>>way)&1;
            if(work_window()){if(!is_valid)++empty_fills;else if(is_dead)++dead_replacements;else ++live_replacements;}
            std::fprintf(dead_file,"%ld,replacement,%u,%u,%u,%u,%u,%llu,%llu\n",cycle,valid,mask,way,is_dead,is_valid,
                (unsigned long long)bits(t->dbg_tags_pscid,way*20,20),(unsigned long long)bits(t->dbg_tags_vpn,way*27,27));
            event("victim",-1,int(way),bits(t->dbg_tags_vpn,way*27,27)<<12,is_dead,is_valid);
        }
        if(t->dbg_ptw_start)pte_a_pending=t->dbg_walk_did==11;
        else if(t->dbg_ptw_state==0)pte_a_pending=0;
        if(measured&&final_a<0){
            if(flush.busy){
                ++wall_flush;
                if(t->dbg_cq_state==1||t->dbg_cq_state==2)++flush_cq_fetch;
                else if(t->dbg_cq_decode&&t->dbg_cq_opcode==1)++flush_cq_decode;
                else if((t->dbg_cq_decode&&t->dbg_cq_opcode==2)||t->dbg_iofence_wait)++flush_cq_fence;
                else ++flush_other;
            }else if(active[1]&&active[1]->issue>=0&&active[1]->done<0){
                bool a_read_pending=false;
                for(const Read& q:readq)if(q.job&&q.job->lane==1){a_read_pending=true;break;}
                bool a_getting_r=!readq.empty()&&readq.front().job&&readq.front().job->lane==1&&t->mem_r_valid&&t->mem_r_ready;
                if(pte_a_pending)++wall_copy_pte;
                else if(a_read_pending&&!a_getting_r)++wall_copy_payload_wait;
                else ++wall_copy_other;
            }else ++wall_host;
        }
        for(CmdLatency& c:cmd_latency){
            if(t->dbg_cq_state==1||t->dbg_cq_state==2)++c.cq_fetch;
            else if(t->dbg_cq_decode&&t->dbg_cq_opcode==1)++c.cq_decode;
            else if((t->dbg_cq_decode&&t->dbg_cq_opcode==2)||t->dbg_iofence_wait)++c.cq_fence;
            else ++c.other;
        }
    }
    void observe() {
        if(!t->rst_ni)return;
        REQUIRE(!t->dbg_fault,"IOMMU/CQ fault asserted; cause="+std::to_string(t->dbg_fault_cause));
        observe_dead_and_time();
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
        if(t->dbg_inval){snapshot_inval();if(trace_open)fence_trace.inval=cycle;}
        if(trace_open){
            ++fence_trace.host_samples;
            if(t->dbg_cq_state==1)++fence_trace.fetch;
            else if(t->dbg_cq_state==2)++fence_trace.reg;
            else if(t->dbg_cq_state==3)++fence_trace.decode_cycles;
            else if(t->dbg_cq_state==6)++fence_trace.wait;
            else ++fence_trace.other;
        }
        if(measured&&final_a<0&&active[0]&&active[0]->issue>=0&&active[0]->done<0)++b_overlap_busy;
        if(t->dbg_cq_decode){
            event("cq_decode",-1,-1,t->dbg_cq_head,t->dbg_cq_opcode,t->dbg_cq_func3);
            if(measured){++decode_count;if(t->dbg_cq_opcode==1)++inv_count;else if(t->dbg_cq_opcode==2)++fence_count;else REQUIRE(false,"unexpected measured CQ opcode");}
            last_fence_decode=t->dbg_cq_opcode==2;
            if(t->dbg_cq_opcode==1&&!cmd_latency.empty())cmd_latency.front().decode=cycle;
            if(trace_open&&t->dbg_cq_opcode==1)fence_trace.inval_decode=cycle;
            if(trace_open&&t->dbg_cq_opcode==2){
                REQUIRE(fence_trace.decode<0,"duplicate fence decode");fence_trace.decode=cycle;
                fence_trace.head_at_decode=t->dbg_cq_head;
            }
            last_fence_av=(read64(CQ_BASE+U64((t->dbg_cq_head+CQ_ENTRIES-1)%CQ_ENTRIES)*16)>>10)&1;
            // PR/PW test overlay holds CQH at the fence until completion.
            if(o.mainline()&&t->dbg_cq_opcode==2)last_fence_av=0;
        }
        if(previous_cq_state!=0&&t->dbg_cq_state==0&&last_fence_decode){
            event("cq_fence_complete",-1,-1,0,last_fence_av,t->dbg_cq_head);last_fence_decode=false;
            if(measured)++fence_completed;
            if(trace_open){fence_trace.idle=cycle;fence_trace.head_at_idle=t->dbg_cq_head;}
            if(measured&&!cmd_latency.empty()){
                CmdLatency c=cmd_latency.front();cmd_latency.pop_front();
                REQUIRE(c.other>0,"missing terminal idle timing sample");--c.other;
                U64 dt=cycle-c.submit;++command_latency_count;command_latency_sum+=dt;
                REQUIRE(c.cq_fetch+c.cq_decode+c.cq_fence+c.other==dt,"command latency partition does not sum");
                command_latency_min=std::min(command_latency_min,dt);command_latency_max=std::max(command_latency_max,dt);
                std::fprintf(latency_file,"%llu,%ld,%ld,%ld,%ld,%llu,%llu,%llu,%llu,%llu\n",
                    (unsigned long long)command_latency_count,c.submit,c.decode,c.inval,cycle,(unsigned long long)dt,
                    (unsigned long long)c.cq_fetch,(unsigned long long)c.cq_decode,(unsigned long long)c.cq_fence,(unsigned long long)c.other);
            }
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
            const unsigned bursts=(p->bytes+2047)/2048;
            REQUIRE(p->r==p->bytes/8&&p->w==p->bytes/8&&p->b==bursts,"COPYDONE without full payload/B completion");
            p->done=cycle;if(measured){++measured_done[l];last_done[l]=cycle;
                completed_bytes[l]+=p->bytes;completed_rbeats[l]+=p->r;completed_wbeats[l]+=p->w;
                completed_ar[l]+=p->ar;completed_aw[l]+=p->aw;completed_b[l]+=p->b;}
            event("copy_done",l,p->index,p->dst,p->next_id);last_progress=cycle;
        }
    }
    void tick() {
        REQUIRE(cycle<CYCLE_LIMIT,"configuration cycle watchdog");
        REQUIRE(cycle-last_progress<STALL_LIMIT||(o.fq()&&!fq_entries.empty()&&fq_due>=0&&cycle<=fq_due+STALL_LIMIT),"no-progress watchdog (see watchdog.json/recent events)");
        t->cfg_valid=bool(cfg);t->cfg_target=cfg?cfg->target:0;t->cfg_write=cfg?cfg->write:0;
        t->cfg_addr=cfg?cfg->addr:0;t->cfg_wdata=cfg?cfg->data:0;t->cfg_wstrb=cfg?cfg->strb:0;t->cfg_size=cfg?cfg->size:3;
        pending_inval_ways=0;
        t->oracle_remove_i=oracle_pending;
        t->sim_fence_read_done_i=!prpw_active||payload_reads_done>=fence_read_goal;
        t->sim_fence_write_done_i=!prpw_active||payload_writes_done>=fence_write_goal;
        t->clk_i=0;drive_memory();t->eval();
        const bool ar=t->mem_ar_valid&&t->mem_ar_ready,aw=t->mem_aw_valid&&t->mem_aw_ready;
        const bool w=t->mem_w_valid&&t->mem_w_ready,b=t->mem_b_valid&&t->mem_b_ready,r=t->mem_r_valid&&t->mem_r_ready;
        U64 same_ar[2]={0,0},same_aw[2]={0,0},same_rlast[2]={0,0},same_b[2]={0,0};
        if(t->rst_ni){
            if((t->dbg_comp_bus&3)==3)++same_ar[(t->dbg_comp_ar_id>>2)&1];
            if((t->dbg_comp_bus&(3u<<5))==(3u<<5))++same_aw[(t->dbg_comp_aw_id>>2)&1];
            if(r&&t->mem_r_last){int l=payload_lane(t->mem_r_id);if(l>=0)++same_rlast[l];}
            if(b){int l=payload_lane(t->mem_b_id);if(l>=0)++same_b[l];}
        }
        for(unsigned l=0;l<2;++l){done_now[l][0]=done_lane[l][0]+same_rlast[l];done_now[l][1]=done_lane[l][1]+same_b[l];}
        if(t->rst_ni&&t->dbg_iofence_decode){
            REQUIRE(trace_open,"fence decoded without host trace");
            for(unsigned l=0;l<2;++l){
                goals[l][0]=comp_lane[l][0]+same_ar[l];goals[l][1]=comp_lane[l][1]+same_aw[l];
                fence_trace.same_ar[l]=same_ar[l];fence_trace.same_aw[l]=same_aw[l];
                fence_trace.same_rlast[l]=same_rlast[l];fence_trace.same_b[l]=same_b[l];
                for(unsigned k=0;k<2;++k){fence_trace.goal[l][k]=goals[l][k];fence_trace.snapshot_done[l][k]=done_now[l][k];}
                event("fence_cohort_read",l,-1,goals[l][0],done_now[l][0],same_ar[l]|(same_rlast[l]<<1));
                event("fence_cohort_write",l,-1,goals[l][1],done_now[l][1],same_aw[l]|(same_b[l]<<1));
            }
            REQUIRE(done_now[1][0]>=goals[1][0]&&done_now[1][1]>=goals[1][1],"A has pending pre-fence payload after A DONE");
            const bool global=o.fence_policy=="global";
            REQUIRE(bool(t->dbg_iofence_pr)==global&&bool(t->dbg_iofence_pw)==global,"actual fence PR/PW differ from selected policy");
            if(global){
                REQUIRE(!prpw_active,"overlapping fixed fence cohorts");prpw_active=true;
                fence_read_goal=goals[0][0]+goals[1][0];fence_write_goal=goals[0][1]+goals[1][1];
                if(measured)++prpw_fences;event("prpw_begin",-1,-1,0,fence_read_goal,fence_write_goal);
            }
        }
        if(prpw_active){
            t->sim_fence_read_done_i=done_now[0][0]>=goals[0][0]&&done_now[1][0]>=goals[1][0];
            t->sim_fence_write_done_i=done_now[0][1]>=goals[0][1]&&done_now[1][1]>=goals[1][1];
            t->eval();
            bool blocked=false;
            for(unsigned l=0;l<2;++l)if(done_now[l][0]<goals[l][0]||done_now[l][1]<goals[l][1]){
                blocked=true;if(measured){++cohort_wait[l];++fence_trace.pending[l];}
            }
            if(blocked&&measured){++prpw_blocked_cycles;++fence_trace.blocked;}
        }
        const bool prpw_complete=t->rst_ni&&t->dbg_iofence_complete&&prpw_active;
        if(t->rst_ni&&t->dbg_iofence_complete){
            REQUIRE(trace_open,"fence completion without trace");fence_trace.complete=cycle;
            if(prpw_active){
                REQUIRE(t->sim_fence_read_done_i&&t->sim_fence_write_done_i,"fence completed before prior payload cohort");
                event("prpw_complete",-1,-1,0,done_now[0][0]+done_now[1][0],done_now[0][1]+done_now[1][1]);
            }
            event("iofence_complete_edge",-1,-1,0,t->dbg_cq_head,t->dbg_cq_next_state);
        }
        if(t->rst_ni&&t->dbg_iofence_wait&&measured)++prpw_wait_cycles;
        if(t->rst_ni)for(unsigned l=0;l<2;++l){
            if(same_ar[l]){++comp_reads;++comp_lane[l][0];event("comp_ar",l,-1,t->dbg_comp_ar_addr,comp_lane[l][0],t->dbg_comp_ar_id);}
            if(same_aw[l]){++comp_writes;++comp_lane[l][1];event("comp_aw",l,-1,t->dbg_comp_aw_addr,comp_lane[l][1],t->dbg_comp_aw_id);}
        }
        const bool sampled_inval=t->rst_ni&&t->dbg_inval;
        if(oracle_pending&&t->dbg_iotlb_fill)
            REQUIRE(t->dbg_iotlb_update_pscid!=t->oracle_pscid_i||t->dbg_iotlb_update_vpn!=t->oracle_vpn_i,"ORACLE unmap overlaps fill of same key");
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
                    REQUIRE(a>=p.dst_pa&&a+8<=p.dst_pa+p.bytes&&ws==255,"COPY W address/strobes");
                    const unsigned off=a-p.dst_pa;REQUIRE(wd==pattern(p.pattern_page,off/8),"COPY W data corruption");
                    for(unsigned i=0;i<8;++i)REQUIRE(++p.w_seen[off+i]==1,"duplicate destination byte");++p.w;
                    if(measured&&p.lane==0&&final_a<0)b_window_write_bytes+=8;
                }
                if(h.beat==0||wl)event("ddr_w_edge",h.job?int(h.job->lane):-1,h.job?int(h.job->index):-1,h.addr+U64(h.beat)*8,wd,ws);
            }
            if(r){
                REQUIRE(!readq.empty(),"R without AR");const Read& h=readq.front();
                if(h.job){Job& p=*h.job;const U64 a=h.addr+U64(h.beat)*8;
                    REQUIRE(a>=p.src_pa&&a+8<=p.src_pa+p.bytes,"COPY R outside source");const unsigned off=a-p.src_pa;
                    REQUIRE(t->mem_r_data==pattern(p.pattern_page,off/8),"COPY R data corruption");
                    for(unsigned i=0;i<8;++i)REQUIRE(++p.r_seen[off+i]==1,"duplicate source byte");++p.r;
                    if(measured&&p.lane==0&&final_a<0)b_window_read_bytes+=8;
                }
            }
            if(b){REQUIRE(!bq.empty(),"B without AW/W");if(bq.front().job)++bq.front().job->b;
                event("ddr_b",bq.front().job?int(bq.front().job->lane):-1,bq.front().job?int(bq.front().job->index):-1,0,bq.front().id);}
            observe_done();
        }
        t->clk_i=1;t->eval();
        if(prpw_complete)prpw_active=false;
        if(oracle_pending){
            REQUIRE(!tags_contain(o.pscid_a(),t->oracle_vpn_i),"ORACLE left unmapped tag valid after edge");
            event("oracle_remove",1,int(a_index),U64(t->oracle_vpn_i)<<12,o.pscid_a());
            oracle_pending=false;t->oracle_remove_i=0;
        }
        if(sampled_inval){
            REQUIRE((t->dbg_tags_valid&pending_inval_ways)==0,"invalidation predicted way remained valid after rising edge");
            unsigned n=0;for(unsigned i=0;i<16;++i)n+=(pending_inval_ways>>i)&1;
            event("inval_post_edge_verified",-1,-1,0,pending_inval_ways,n);
            if(measured){++inval_verified_events;inval_verified_ways+=n;}
        }
        if(ar)readq.push_back(nr);if(aw)writeq.push_back(nw);
        if(w){REQUIRE(!writeq.empty(),"model W queue empty");Write& h=writeq.front();write64(h.addr+U64(h.beat)*8,wd,ws);++h.beat;
            if(wl){bq.push_back(B{h.id,h.job});writeq.pop_front();}}
        if(b){if(bq.front().job){++payload_writes_done;unsigned l=bq.front().job->lane;++done_lane[l][1];event("ddr_b_complete",l,-1,0,done_lane[l][1]);}bq.pop_front();}
        if(r){if(readq.front().beat==readq.front().len){if(readq.front().job){++payload_reads_done;unsigned l=readq.front().job->lane;++done_lane[l][0];event("ddr_rlast_complete",l,-1,0,done_lane[l][0]);}readq.pop_front();}else ++readq.front().beat;}
        REQUIRE(payload_reads_done<=comp_reads&&payload_writes_done<=comp_writes,"global completions precede processed IOMMU requests");
        for(unsigned l=0;l<2;++l)for(unsigned k=0;k<2;++k)REQUIRE(done_lane[l][k]<=comp_lane[l][k],"lane completions precede processed requests");
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
        auto p=std::make_unique<Job>();p->lane=l;p->index=j;p->bytes=(!l&&measured)?o.b_bytes:PAGE;
        if(l){p->src=a_src(j);p->dst=a_dst(j);p->src_pa=a_src_pa(j);p->dst_pa=a_dst_pa(j);p->pattern_page=j+1;}
        else {const unsigned dest=1+j%(o.b_pages-1);p->src=B_IOVA;p->dst=B_IOVA+U64(dest)*PAGE;p->src_pa=B_PA;p->dst_pa=B_PA+U64(dest)*PAGE;}
        Job* ptr=p.get();jobs.push_back(std::move(p));return ptr;
    }
    void init_lane(unsigned l,unsigned quota) {lane[l]=Lane{};lane[l].lane=l;lane[l].quota=quota;lane[l].enabled=true;lane[l].finished=quota==0;}
    void finish_job(Job& p) {
        REQUIRE(p.done>=0&&p.issue>=0,"MMIO DONE arrived before true hardware completion");
        const unsigned bursts=(p.bytes+2047)/2048;
        REQUIRE(p.ar==bursts&&p.aw==bursts&&p.r==p.bytes/8&&p.w==p.bytes/8&&p.b==bursts,"COPY transfer count mismatch");
        for(unsigned b=0;b<512;++b){
            if(b*8<p.bytes)REQUIRE(read64(p.dst_pa+b*8)==pattern(p.pattern_page,b),"destination payload mismatch");
            REQUIRE(read64(p.src_pa+b*8)==pattern(p.pattern_page,b),"source page mutated");
        }
        for(unsigned i=0;i<PAGE;++i)REQUIRE(p.r_seen[i]==unsigned(i<p.bytes)&&p.w_seen[i]==unsigned(i<p.bytes),"COPY byte coverage including untouched page tail");
        std::fprintf(jobs_file,"%s,%u,%u,%llu,%llu,%llu,%llu,%llu,%ld,%ld,%ld,%llu,%llu,%llu,%llu,%llu,%u\n",era.c_str(),p.lane,p.index,
            (unsigned long long)p.src,(unsigned long long)p.dst,(unsigned long long)p.src_pa,(unsigned long long)p.dst_pa,(unsigned long long)p.next_id,
            p.issue,p.done,p.poll_done,(unsigned long long)p.ar,(unsigned long long)p.aw,(unsigned long long)p.r,(unsigned long long)p.w,(unsigned long long)p.b,p.bytes);
    }
    void service_lane(unsigned l) {
        Lane& s=lane[l];if(!s.enabled||s.finished||s.busy)return;
        if(s.stage==0){
            if(l==0&&measured&&o.b_background){
                if(final_a>=0){s.finished=true;s.enabled=false;if(b_stop<0){b_stop=final_a;event("b_offer_stop",0,-1,0,b_offered);}return;}
                if(cycle<b_next_offer)return;
            }
            REQUIRE(!active[l],"host reused active DMA slot");active[l]=make_job(l,s.index);
            if(l==0&&measured){++b_offered;event("b_job_offer",0,s.index,0,active[l]->bytes,b_next_offer);}
        }
        Job* p=active[l];s.busy=true;
        const U64 addrs[]={0x18,0,8,0x10,REG_NEXT,REG_DONE};
        const U64 vals[]={0,p->src,p->dst,p->bytes,0,0};const unsigned stage=s.stage;
        request(l,stage<4,addrs[stage],vals[stage],3,[this,l,p,stage](U64 value){
            Lane& x=lane[l];x.busy=false;
            if(stage==4){p->next_id=value;event("copy_next_id",l,p->index,0,value);x.stage=5;}
            else if(stage==5){if(value==p->next_id){p->poll_done=cycle;event("copy_host_done",l,p->index,p->dst,value);finish_job(*p);active[l]=nullptr;
                    ++x.index;x.stage=0;if(l==0&&measured)b_next_offer=cycle+o.b_gap;if(x.index==x.quota)x.finished=true;}
                else x.stage=5;}
            else ++x.stage;
        });
    }
    bool tags_contain(unsigned ps,U64 vpn)const {
        for(unsigned i=0;i<16;++i)if(((t->dbg_tags_valid>>i)&1)&&bits(t->dbg_tags_pscid,i*20,20)==ps&&bits(t->dbg_tags_vpn,i*27,27)==vpn)return true;return false;
    }
    void warm() {
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
        if(measured&&(lo&127)==1){CmdLatency c;c.submit=cycle;cmd_latency.push_back(c);}
    }
    void start_flush(U64 iova,bool force_wide=false) {
        REQUIRE(!flush.busy,"overlapping A flush groups");
        if(measured){REQUIRE(!trace_open,"previous fence trace not closed");fence_trace=FenceTrace{};fence_trace.submit=cycle;trace_open=true;}
        flush=Flush{};flush.busy=true;flush.iova=iova;flush.wide=force_wide;
        flush.begin=cycle;flush.id=unsigned(rounds);if(measured)++rounds;
        bool scoped=o.mainline()||(!force_wide&&(o.mode=="m2"||o.mode=="m3"));
        bool av=!force_wide&&(o.mode=="m3"||o.mode=="ml-strict");
        U64 lo=1|(U64(av)<<10)|(scoped?(U64(o.pscid_a())<<12)|(1ULL<<32):0);
        if(trace_open){fence_trace.inval_lo=lo;fence_trace.inval_hi=av?(iova>>2):0;}
        put_command(lo,av?(iova>>2):0);if(!o.mainline())put_command(2,0);flush.first_goal=tail;
        event("cq_round_begin",1,int(a_index),iova,flush.id,scoped|(unsigned(av)<<1));
    }
    void enqueue_fq(Key key) {
        REQUIRE(fq_entries.size()<FQ_CAPACITY,"FQ capacity violated");fq_entries.push_back(key);
        fq_peak=std::max(fq_peak,U64(fq_entries.size()));
        if(fq_due<0)fq_due=cycle+FQ_TIMEOUT_CYCLES;
        event("fq_enqueue",1,int(a_index),key.second<<12,fq_entries.size(),fq_due);
    }
    void complete_flush() {
        event("cq_round_complete",1,int(a_index),flush.iova,flush.id,cycle-flush.begin);
        if(measured)total_waiting+=cycle-flush.begin;
        if(trace_open){
            const FenceTrace& f=fence_trace;
            REQUIRE(f.submit>=0&&f.inval_decode>=f.submit&&f.inval>=f.inval_decode&&f.fence_submit>=f.submit&&f.decode>=f.fence_submit&&f.complete>=f.decode&&f.idle==f.complete+1&&cycle>=f.idle,"fence trace ordering");
            REQUIRE(f.fetch+f.reg+f.decode_cycles+f.wait+f.other==f.host_samples,"fence state partition");
            REQUIRE(f.host_samples==U64(cycle-f.submit),"submit-to-host samples partition");
            REQUIRE(f.goal[1][0]==f.snapshot_done[1][0]&&f.goal[1][1]==f.snapshot_done[1][1]&&f.pending[1]==0,"A pre-fence cohort must already be drained");
            std::vector<U64> values={rounds,flush.iova,f.inval_lo,f.inval_hi,f.fence_lo,f.fence_hi,U64(f.submit),U64(f.inval_decode),U64(f.inval),U64(f.fence_submit),U64(f.decode),U64(f.complete),U64(f.idle),U64(cycle),f.fetch,f.reg,f.decode_cycles,f.wait,f.blocked,f.other,f.host_samples,f.pending[1],f.pending[0],f.goal[0][0],f.snapshot_done[0][0],f.goal[0][1],f.snapshot_done[0][1],f.goal[1][0],f.snapshot_done[1][0],f.goal[1][1],f.snapshot_done[1][1],f.same_ar[0],f.same_aw[0],f.same_rlast[0],f.same_b[0],f.same_ar[1],f.same_aw[1],f.same_rlast[1],f.same_b[1],f.head_at_decode,f.head_at_idle,t->dbg_cq_head};
            for(unsigned i=0;i<values.size();++i)std::fprintf(fences_file,"%s%llu",i?",":"",(unsigned long long)values[i]);std::fputc('\n',fences_file);
            trace_open=false;
        }
        if(flush_fq){
            for(Key key:fq_entries){REQUIRE(reserved_iovas.erase(key)==1,"FQ releases an unreserved IOVA");++fq_reclaimed;event("fq_release",1,-1,key.second<<12,key.first);}
            fq_entries.clear();++fq_flushes;flush_fq=false;
            if(fq_waiting_enqueue){enqueue_fq(waiting_enqueue);fq_waiting_enqueue=false;}
            if(fq_entries.empty())fq_drain_end=cycle;
        }else if(o.mode=="ml-strict"){
            REQUIRE(reserved_iovas.erase(Key{o.pscid_a(),flush.iova>>12})==1,"strict releases an unreserved IOVA");
        }
        flush.busy=false;
    }
    void service_flush() {
        if(!flush.busy||flush.cfg_busy)return;flush.cfg_busy=true;
        if(o.mainline()){
            if(flush.stage==0){
                request(2,true,0x24,tail,2,[this](U64){
                    flush.cfg_busy=false;if(measured)++doorbells;
                    event("cq_doorbell",1,int(a_index),0,tail,1);
                    U64 lo=2|(o.fence_policy=="global"?((1ULL<<12)|(1ULL<<13)):0);
                    if(trace_open){fence_trace.fence_submit=cycle;fence_trace.fence_lo=lo;fence_trace.fence_hi=0;}
                    put_command(lo,0);flush.last_goal=tail;flush.stage=2;
                });
            }else if(flush.stage==2){
                request(2,true,0x24,tail,2,[this](U64){flush.cfg_busy=false;if(measured)++doorbells;
                    event("cq_doorbell",1,int(a_index),0,tail,1);flush.stage=3;});
            }else{
                request(2,false,0x20,0,2,[this](U64 head){flush.cfg_busy=false;
                    event("cq_head_poll",1,int(a_index),0,head,flush.last_goal);
                    if(head!=flush.last_goal)return;
                    REQUIRE(cmd_latency.empty(),"CQH poll returned before actual IOFENCE completion");
                    complete_flush();
                });
            }
            return;
        }
        if(flush.stage==0||flush.stage==2){
            const unsigned count=flush.stage==0?2:1;
            request(2,true,0x24,tail,2,[this,count](U64){flush.cfg_busy=false;event("cq_doorbell",1,int(a_index),0,tail,count);if(measured)++doorbells;++flush.stage;});
        } else {
            request(2,false,0x20,0,2,[this](U64 head){flush.cfg_busy=false;
                event("cq_head_poll",1,int(a_index),0,head,flush.stage==1?flush.first_goal:flush.last_goal);
                const unsigned goal=flush.stage==1?flush.first_goal:flush.last_goal;if(head!=goal)return;
                if(flush.stage==1){event("cq_first_head_seen",1,int(a_index),flush.iova,flush.id,head);put_command(2,0);flush.last_goal=tail;flush.stage=2;}
                else complete_flush();
            });
        }
    }
    void map_region(bool src) {
        U64 va=src?a_src(a_index):a_dst(a_index),pa=src?a_src_pa(a_index):a_dst_pa(a_index);
        if(o.mainline())REQUIRE(reserved_iovas.insert(Key{o.pscid_a(),va>>12}).second,"IOVA allocated before prior FQ flush completed");
        write64(pte_addr(va,true),pte(pa));++map_regions;event("map_region",1,int(a_index),va,pa,PAGE);
        if(!o.mainline()&&o.map_policy=="full-api")start_flush(va,o.mode=="m4-16"||o.mode=="m4-64");
    }
    void unmap_region(bool src) {
        U64 va=src?a_src(a_index):a_dst(a_index),pa=src?a_src_pa(a_index):a_dst_pa(a_index);
        write64(pte_addr(va,true),0);++unmap_regions;++pending_regions;event("unmap_region",1,int(a_index),va,pa,PAGE);
        Key key{o.pscid_a(),va>>12};dead.insert(key);
        if(o.mode=="oracle"){
            REQUIRE(!oracle_pending,"overlapping ORACLE pulse");oracle_pending=true;
            t->oracle_pscid_i=o.pscid_a();t->oracle_vpn_i=va>>12;
        }
        if(o.fq()){
            if(fq_entries.size()==FQ_CAPACITY){
                fq_waiting_enqueue=true;waiting_enqueue=key;flush_fq=true;++fq_full_flushes;start_flush(0,true);
            }else enqueue_fq(key);
        }else if(!o.batch())start_flush(va);
    }
    void service_fq() {
        if(o.fq()&&!flush.busy&&!fq_entries.empty()&&fq_due>=0&&cycle>=fq_due){
            fq_due=-1;last_progress=cycle;flush_fq=true;++fq_timer_flushes;event("fq_timer_expire",1,-1,0,fq_entries.size());start_flush(0,true);
        }
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
        REQUIRE(measured_done[0]==b_offered&&measured_done[1]==o.a_jobs,"measured COPY count mismatch");
        REQUIRE(measured_issue[0]==b_offered&&measured_issue[1]==o.a_jobs,"measured ISSUE count mismatch");
        U64 expected_rounds=0;if(o.mode!="m0"){
            const U64 maps=!o.mainline()&&o.map_policy=="full-api"?2*o.a_jobs:0;
            const U64 unmaps=o.batch()?(2*o.a_jobs+o.batch()-1)/o.batch():2*o.a_jobs;
            expected_rounds=o.fq()?fq_timer_flushes+fq_full_flushes:maps+unmaps;
            REQUIRE(map_regions==2*o.a_jobs&&unmap_regions==2*o.a_jobs,"map/unmap region count mismatch");
        }else REQUIRE(!map_regions&&!unmap_regions,"M0 persistent mapping changed during measurement");
        const U64 fences=(o.mainline()?1:2)*expected_rounds;
        REQUIRE(rounds==expected_rounds&&inv_count==expected_rounds&&fence_count==fences&&fence_completed==fences,"CQ opcode/completion counts mismatch");
        REQUIRE(submit_count==expected_rounds+fences&&decode_count==submit_count&&doorbells==2*expected_rounds,"CQ submit/decode/doorbell conservation");
        REQUIRE(inval_verified_events==expected_rounds,"invalidation pulse/post-edge verification conservation");
        REQUIRE(command_latency_count==expected_rounds&&cmd_latency.empty(),"per-command timing conservation");
        REQUIRE(!prpw_active&&payload_reads_done==comp_reads&&payload_writes_done==comp_writes,"payload/fence cohort not drained");
        REQUIRE(prpw_fences==(o.fence_policy=="global"?expected_rounds:0),"PR/PW fence count mismatch");
        REQUIRE(!trace_open&&cohort_wait[1]==0,"unclosed trace or unexpected A pending cohort");
        for(unsigned l=0;l<2;++l)for(unsigned k=0;k<2;++k)REQUIRE(comp_lane[l][k]==done_lane[l][k],"per-lane completion conservation");
        REQUIRE(completed_bytes[1]==U64(o.a_jobs)*PAGE,"A full COPY bytes mismatch");
        REQUIRE(completed_bytes[0]==b_offered*o.b_bytes,"B all accepted bytes mismatch");
        REQUIRE(wall_host+wall_flush+wall_copy_pte+wall_copy_payload_wait+wall_copy_other==U64(final_a-t0),"A wall-time partition does not sum");
        REQUIRE(flush_cq_fetch+flush_cq_decode+flush_cq_fence+flush_other==wall_flush,"flush partition does not sum");
        if(o.mainline())REQUIRE(reserved_iovas.empty(),"unreclaimed IOVA after full drain");
        if(o.fq())REQUIRE(fq_reclaimed==2*o.a_jobs&&fq_entries.empty(),"FQ page retirement mismatch");
        for(unsigned l=0;l<2;++l)REQUIRE(stats[l].misses==stats[l].inval+stats[l].cold+stats[l].replacement,"miss attribution partition");
        commands_ok=true;
    }
    void run() {
        setup_memory();reset_and_program();drain();if(o.smoke)smoke_marker();
        if(!o.a_alone)warm();else{warm_resident=true;warm_no_walk=true;}
        era="measure";t0=cycle;measured=true;last_progress=cycle;event("measure_begin");
        for(unsigned l=0;l<2;++l)for(unsigned k=0;k<2;++k){work_comp_base[l][k]=comp_lane[l][k];work_done_base[l][k]=done_lane[l][k];}
        b_next_offer=t0;init_lane(0,o.a_alone?0:~0u);lane[1].enabled=false;
        while(!lane[0].finished||a_state!=AState::FINISH||flush.busy){service_a();service_lane(0);step();}
        if(o.a_alone)b_stop=final_a;
        drain();const U64 csr=blocking(2,false,0x48,0,2);
        REQUIRE((csr&0xF00)==0,"CQCSR error status after all work");
        REQUIRE(blocking(2,false,0x20,0,2)==tail,"final CQH/CQT mismatch");drain();
        quiet_ok=quiet();REQUIRE(quiet_ok,"final model/hardware not quiet");data_ok=true;verify_counts();
        measure_end=cycle;event("measure_end");measured=false;passed=true;
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
        std::fprintf(f,"{\"schema_version\":2,\"config\":{\"case_id\":%s,\"domain\":%s,\"b_pages\":%u,\"mode\":%s,\"fence_policy\":%s,\"a_jobs\":%u,\"b_bytes\":%u,\"b_gap\":%u,\"b_background\":%s,\"a_alone\":%s},\"timing\":{\"t0\":%ld,\"end_cycle\":%ld},\"metrics\":{",
            quoted(o.case_id).c_str(),quoted(o.domain).c_str(),o.b_pages,quoted(o.mode).c_str(),quoted(o.fence_policy).c_str(),o.a_jobs,o.b_bytes,o.b_gap,o.b_background?"true":"false",o.a_alone?"true":"false",t0,measure_end);
        bool first=true;auto m=[&](const char* name,long long value){std::fprintf(f,"%s%s:%lld",first?"":",",quoted(name).c_str(),value);first=false;};
        m("T_A",last_done[1]<0?-1:last_done[1]-t0);m("T_B",last_done[0]<0?-1:last_done[0]-t0);
        m("A_final_flush_end",final_a<0?-1:final_a-t0);m("T_end",measure_end<0?-1:measure_end-t0);
        m("B_offer_stop",b_stop<0?-1:b_stop-t0);m("B_offered_jobs",b_offered);
        m("B_concurrent_busy_cycles",b_overlap_busy);m("B_window_read_bytes",b_window_read_bytes);m("B_window_write_bytes",b_window_write_bytes);
        m("A_wall_host",wall_host);m("A_wall_flush",wall_flush);m("A_wall_copy_pte",wall_copy_pte);m("A_wall_copy_payload_wait",wall_copy_payload_wait);m("A_wall_copy_other",wall_copy_other);
        m("A_flush_cq_fetch_register",flush_cq_fetch);m("A_flush_cq_inval_decode",flush_cq_decode);m("A_flush_cq_fence_decode_wait",flush_cq_fence);m("A_flush_other",flush_other);
        m("command_latency_count",command_latency_count);m("command_latency_sum",command_latency_sum);m("command_latency_min",command_latency_count?command_latency_min:0);m("command_latency_max",command_latency_max);
        m("prpw_fences",prpw_fences);m("wait_prpw_state_cycles",prpw_wait_cycles);m("prpw_blocked_cycles",prpw_blocked_cycles);m("A_cohort_pending_cycles",cohort_wait[1]);m("B_cohort_pending_cycles",cohort_wait[0]);
        for(unsigned l=0;l<2;++l){std::string p=l?"A_":"B_";const Counters& c=stats[l];
            m((p+"hits").c_str(),c.hits);m((p+"misses").c_str(),c.misses);m((p+"walks").c_str(),c.walks);m((p+"invalidation_attributed_misses").c_str(),c.inval);m((p+"cold_misses").c_str(),c.cold);m((p+"replacement_misses").c_str(),c.replacement);m((p+"pte_memory_bytes").c_str(),c.pte_bytes);
            m((p+"accepted_jobs").c_str(),measured_issue[l]);m((p+"completed_jobs").c_str(),measured_done[l]);m((p+"payload_bytes_each_direction").c_str(),completed_bytes[l]);
            m((p+"read_beats").c_str(),completed_rbeats[l]);m((p+"write_beats").c_str(),completed_wbeats[l]);m((p+"ar_bursts").c_str(),completed_ar[l]);m((p+"aw_bursts").c_str(),completed_aw[l]);m((p+"b_completions").c_str(),completed_b[l]);
            m((p+"comp_ar_accepted").c_str(),comp_lane[l][0]-work_comp_base[l][0]);m((p+"comp_aw_accepted").c_str(),comp_lane[l][1]-work_comp_base[l][1]);m((p+"ddr_rlast_completed").c_str(),done_lane[l][0]-work_done_base[l][0]);m((p+"ddr_b_completed").c_str(),done_lane[l][1]-work_done_base[l][1]);
        }
        m("cq_iotinval",inv_count);m("cq_iofence",fence_count);m("cq_decode_count",decode_count);m("cq_submit_commands",submit_count);m("cq_fence_completed",fence_completed);m("inval_post_edge_verified_events",inval_verified_events);m("cq_doorbells",doorbells);m("cq_rounds",rounds);m("cq_total_waiting",total_waiting);m("map_regions",map_regions);m("unmap_regions",unmap_regions);
        std::fprintf(f,"},\"checks\":{\"passed\":%s,\"data\":%s,\"quiet\":%s,\"commands\":%s,\"warm_resident\":%s,\"warm_no_extra_walks\":%s},\"failure\":%s,\"limitations\":[\"Corrected original 16-entry PLRU IOMMU baseline with simulation-only global PRPW overlay.\",\"Policy none clears only command PR/PW bits and is a diagnostic, not the mainline default.\",\"Fixed cohort includes comp AR/AW accepted on the decode edge and includes DDR RLAST/B completions on each sampled edge.\",\"Completion at modeled DDR is a conservative test observation point, not an implementation of every architectural ordering property.\",\"A=64 full 4KiB copies, leaf-only strict unmaps; B=4 persistent pages, serial jobs offered after previous host completion plus configured gap.\",\"B new offers stop at A API completion; all already offered jobs complete and quiet is verified. B total useful work varies with A window and is reported separately.\",\"DDR is one FIFO, 16-cycle head startup and one 8-byte beat per cycle; host MMIO/PTE writes do not model native CPU time.\",\"No CPU/Linux/FPGA execution; one deterministic repetition per configuration.\"]}\n",
            passed?"true":"false",data_ok?"true":"false",quiet_ok?"true":"false",commands_ok?"true":"false",warm_resident?"true":"false",warm_no_walk?"true":"false",failure.empty()?"null":quoted(failure).c_str());std::fclose(f);
    }
};
int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);Options o;
    for(int i=1;i<argc;++i){std::string k=argv[i];auto val=[&](){REQUIRE(i+1<argc,"missing argument value");return std::string(argv[++i]);};
        if(k=="--out")o.out=val();else if(k=="--mode")o.mode=val();else if(k=="--domain")o.domain=val();
        else if(k=="--fence-policy")o.fence_policy=val();else if(k=="--b-bytes")o.b_bytes=std::stoul(val());
        else if(k=="--b-gap")o.b_gap=std::stoul(val());else if(k=="--b-background")o.b_background=true;
        else if(k=="--a-alone")o.a_alone=true;else if(k=="--case-id")o.case_id=val();else throw Failure("unknown option: "+k);
    }
    REQUIRE(!o.out.empty(),"--out required");REQUIRE(o.domain=="same"||o.domain=="different","domain");
    REQUIRE(o.mode=="ml-strict","only strict mainline command sequence is supported");
    REQUIRE(o.fence_policy=="global"||o.fence_policy=="none","fence policy");
    REQUIRE(o.b_bytes==512||o.b_bytes==2048||o.b_bytes==4096,"B bytes");REQUIRE(o.b_gap==0||o.b_gap==2048,"B gap");
    REQUIRE(o.a_alone!=o.b_background,"choose exactly one of --a-alone and --b-background");
    if(o.case_id.empty())o.case_id=o.a_alone?"a-alone-"+o.fence_policy:o.domain+"-b"+std::to_string(o.b_bytes)+"-gap"+std::to_string(o.b_gap)+"-"+o.fence_policy;
    Tb tb(o);try{tb.run();}catch(const std::exception&e){tb.failure=e.what();tb.event("failure",-1,-1,0,0);tb.dump_failure();}
    tb.summary();std::printf("RESULT %s cycle=%ld failure=%s\n",tb.passed?"PASS":"FAIL",tb.cycle,tb.failure.c_str());return tb.passed?0:1;
}
