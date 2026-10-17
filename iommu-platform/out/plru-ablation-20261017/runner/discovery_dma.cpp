// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// DISCOVERY-REUSE-1: isolated real dual-iDMA / IOMMU behavioral-DDR test.
// No CPU/Linux/FPGA time claim. One base elaboration: PolicyEnable=1,
// PairPolicy=0, CommonDsFirstEnable=1 (F4); runtime P or PN is reset-stable.
// Only the A destination reuse ring changes. No warm/clear/remap/poison,
// autonomous batch, M/O action, return reordering or queued startup overlap.
#ifdef ACTUAL_DMA_SYSTEM
#include "Vnonblock_actual_dma_system_top.h"
using TopT = Vnonblock_actual_dma_system_top;
#else
#error "The discovery fixture requires ACTUAL_DMA_SYSTEM"
#endif
#include "verilated.h"
#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

double sc_time_stamp() { return 0; }
static constexpr uint64_t V_SRC_IOVA=0x10000000ULL, V_DST_IOVA=0x10001000ULL;
static constexpr uint64_t V_SRC_PA=0x80000000ULL, V_DST_PA=0x80001000ULL;
static constexpr uint64_t A_SRC_IOVA=0x20000000ULL, A_DST_IOVA=0x20100000ULL;
static constexpr uint64_t A_SRC_PA=0x90000000ULL, A_DST_PA=0x90100000ULL;
static constexpr unsigned PAGE_BYTES=4096, PAGE_BEATS=512, PARENTS=64;
static constexpr uint64_t REG_SRC=0, REG_DST=8, REG_NB=0x10, REG_CONF=0x18;
static constexpr uint64_t REG_NEXT=0x28, REG_DONE=0x30, REG_B_CTL=0x88;
static constexpr long WATCHDOG=200000L, TICK_LIMIT=1000000L, CFG_TIMEOUT=100000L;

struct TestFailure : std::runtime_error { using std::runtime_error::runtime_error; };
static void fail(const char* msg,const char* file,int line) {
    throw TestFailure(std::string(msg)+" ("+file+":"+std::to_string(line)+")");
}
#define CHECK(c,m) do { if(!(c)) fail((m),__FILE__,__LINE__); } while(0)
static uint64_t page_word(unsigned page,unsigned beat) {
    const uint64_t i=(uint64_t(page)<<9)|beat;
    return (i*0x9E3779B97F4A7C15ULL)^0xA5C3E10D2B4F9678ULL;
}
static uint64_t page_old(unsigned page,unsigned beat) {
    return 0xD000000000000000ULL^(uint64_t(page+1)<<40)^beat;
}

class Tb {
public:
    TopT* top=new TopT;
    FILE* event_trace=nullptr;
    FILE* parents_file=nullptr;
    std::string mode="p", a_leaf="4k";
    unsigned ring=1;
    long g_cycle=0, t0=-1, quiet_cycle=-1, initial_cycles=0;
    bool era=false, final_quiet=false;
    // Model-only memory is sparse; arena reservation is a synthetic contract,
    // not a host/OS physical allocation measurement.
    std::map<uint64_t,uint8_t> mem;
    struct Parent {
        unsigned lane=0,j=0;
        uint64_t src_iova=0,dst_iova=0,src_pa=0,dst_pa=0,next_id=0;
        bool id_known=false, issued=false, backend_active=false, retired=false;
        long ready=-1,cfg_begin=-1,cfg_end=-1,next_begin=-1,next_end=-1;
        long poll_begin=-1,poll_end=-1,first_poll=-1,issue=-1,backend=-1;
        long first_r=-1,source_complete=-1,last_b=-1,done=-1;
        uint64_t ar=0,aw=0,r_beats=0,w_beats=0,b=0,rd_bytes=0,wr_bytes=0,err=0;
        uint64_t local_ar=0,local_r=0,local_outstanding=0,polls=0;
        // Exact byte coverage detects duplicated/omitted beats even with a
        // fixed source and repeated writes to the same destination.
        std::vector<uint8_t> r_seen=std::vector<uint8_t>(PAGE_BYTES,0);
        std::vector<uint8_t> w_seen=std::vector<uint8_t>(PAGE_BYTES,0);
    };
    std::vector<Parent> parents[2];
    int current[2]={-1,-1};
    uint64_t hw_issue[2]={0,0},hw_done[2]={0,0};
    long last_progress[2]={0,0};
    uint64_t overlap_ticks=0,tl_accept=0,tl_resp=0;
    uint64_t normal_accept=0,n_accept=0,normal_lookup_first=0,normal_lookup_hit=0;
    uint64_t n_probes=0,n_probe_hits=0,ptw_starts=0,ptw_active_ticks=0;
    uint64_t ddtw_ar=0,ptw_ar=0,ddr_ar=0,ddr_aw=0,ddr_r=0,ddr_w=0,ddr_b=0;
    uint64_t next_read_occ=0,next_write_occ=0;
    struct MemRead {
        uint64_t addr=0,occ=0;
        unsigned len=0,id=0,beat=0;
        int latency=31;
        long accepted_cycle=0;
        int lane=-1,parent=-1;
    };
    std::deque<MemRead> mem_q;  // one FIFO R-return queue; no ID reordering
    struct CawEnt {
        unsigned id=0,len=0,got=0;
        uint64_t addr=0,occ=0;
        int lane=-1,parent=-1;
    };
    struct BEnt { unsigned id=0; uint64_t occ=0; int lane=-1,parent=-1; };
    std::deque<CawEnt> caw_q;
    std::deque<BEnt> cb_q;
    bool cfg_valid=false,cfg_write=false,cfg_sel=false;
    uint64_t cfg_addr=0,cfg_wdata=0;
    unsigned cfg_wstrb=0;

    // Held VALID shadows. These are observations, never scheduling inputs.
    struct Held { bool held=false; uint64_t addr=0; unsigned id=0,len=0,attrs=0; };
    Held selector,ra_up,ra_down,local_ar_shadow[2],ptw_local,ds_merge,ds_delayed;
    std::map<std::string,uint64_t> states;
    bool ddr_setup_wait=false;
    bool observed_ptw_active=false;

    ~Tb() {
        if(event_trace) std::fclose(event_trace);
        if(parents_file) std::fclose(parents_file);
        top->final(); delete top;
    }
    uint64_t read64(uint64_t addr) const {
        uint64_t data=0;
        for(unsigned i=0;i<8;++i) {
            auto p=mem.find(addr+i);
            CHECK(p!=mem.end(),"unmapped model read");
            data|=uint64_t(p->second)<<(8*i);
        }
        return data;
    }
    void write64(uint64_t addr,uint64_t data) {
        for(unsigned i=0;i<8;++i) mem[addr+i]=uint8_t(data>>(8*i));
    }
    void write_beat(uint64_t addr,uint64_t data,unsigned strb) {
        for(unsigned i=0;i<8;++i) if(strb&(1u<<i)) mem[addr+i]=uint8_t(data>>(8*i));
    }
    void event(const char* name,unsigned tag=0,uint64_t addr=0,uint64_t value=0) {
        if(era&&event_trace) std::fprintf(event_trace,"%ld,%s,%u,%llu,%llu\n",
            g_cycle-t0,name,tag,(unsigned long long)addr,(unsigned long long)value);
    }
    void state(const char* name,uint64_t value,unsigned tag=0,uint64_t addr=0) {
        const std::string key=std::string(name)+":"+std::to_string(tag);
        auto it=states.find(key);
        if(it==states.end()||it->second!=value) {
            states[key]=value; event(name,tag,addr,value);
        }
    }
    void stable(Held& h,bool valid,bool ready,unsigned id,uint64_t addr,unsigned len,
                unsigned attrs,const char* why) {
        if(h.held) CHECK(valid&&id==h.id&&addr==h.addr&&len==h.len&&attrs==h.attrs,why);
        h.held=valid&&!ready;
        if(h.held) {h.addr=addr;h.id=id;h.len=len;h.attrs=attrs;}
    }
    Parent& active(unsigned lane) {
        CHECK(lane<2&&current[lane]>=0,"payload/observer outside supplied parent");
        return parents[lane].at(unsigned(current[lane]));
    }
    Parent& identified(int lane,int parent) {
        CHECK(lane>=0&&lane<2&&parent>=0,"queued payload missing parent identity");
        return parents[lane].at(unsigned(parent));
    }
    static int payload_lane(unsigned id) { return ((id>>4)&3u)==2u?int((id>>2)&1u):-1; }
    void progress(unsigned lane) {last_progress[lane]=g_cycle;}
    static unsigned parent_tag(const Parent& p) {return (p.j<<1)|p.lane;}

    void setup_memory() {
        CHECK(ring==1||ring==8||ring==16,"unsupported A ring");
        CHECK(!era,"memory initialization after t0");
        mem.clear();
        // Seven fixed 4KiB metadata pages reserved in both leaf conditions.
        for(unsigned page=1;page<=7;++page)
            for(unsigned off=0;off<PAGE_BYTES;off+=8)
                write64(0xA0000000ULL+page*0x10000ULL+off,0);
        write64(0xA0010280,1); write64(0xA0010290,0xA000);
        write64(0xA0010298,0x80000000000A0020ULL);
        write64(0xA00102C0,1); write64(0xA00102D0,0xB000);
        write64(0xA00102D8,0x80000000000A0030ULL);
        write64(0xA0020000,0x28010001ULL);
        write64(0xA0040400,0x28014001ULL);
        write64(0xA0050000,((V_SRC_PA>>12)<<10)|0xDFULL);
        write64(0xA0050008,((V_DST_PA>>12)<<10)|0xDFULL);
        write64(0xA0030000,0x28018001ULL);
        if(a_leaf=="2m") {
            CHECK((A_SRC_IOVA&0x1fffff)==0&&(A_SRC_PA&0x1fffff)==0,"unaligned A superpage");
            // The SAME exclusive A [90000000,90200000) arena is authorized
            // in every condition. This ordinary Sv39 L1 leaf has A/D preset.
            write64(0xA0060800,((A_SRC_PA>>12)<<10)|0xDFULL);
        } else {
            write64(0xA0060800,0x2801C001ULL);
            write64(0xA0070000,((A_SRC_PA>>12)<<10)|0xDFULL);
            for(unsigned p=0;p<ring;++p)
                write64(0xA0070000ULL+(256+p)*8ULL,(((A_DST_PA+p*4096ULL)>>12)<<10)|0xDFULL);
        }
        for(unsigned i=0;i<PAGE_BEATS;++i) {
            write64(V_SRC_PA+i*8ULL,page_word(0,i));
            write64(V_DST_PA+i*8ULL,page_old(0,i));
            write64(A_SRC_PA+i*8ULL,page_word(1,i));
            for(unsigned p=0;p<ring;++p) write64(A_DST_PA+p*4096ULL+i*8ULL,page_old(1+p,i));
        }
        // Preserve the old test system's disabled CPU scratch initialization.
        write64(0x81000000ULL,0xE000000000000000ULL);
        write64(0x81000008ULL,0xE000000000000001ULL);
        for(unsigned i=0;i<8;++i) write64(0x81001000ULL+i*8ULL,0xD000000000000000ULL+i);
    }

    // Frozen behavior of the original drive_ddr() with all optional controls
    // false: FIFO head only ages once/tick; each queued request pays startup31
    // only on reaching the head; independent writes/B and R; AR cap=16.
    // No observer calls this function a second time in the same clock tick.
    void drive_ddr() {
        ddr_setup_wait=false;
        top->ddr_aw_ready=1;
        top->ddr_w_ready=1;
        top->ddr_ar_ready=(mem_q.size()<16)?1:0;
        if(!cb_q.empty()) {
            top->ddr_b_valid=1; top->ddr_b_id=cb_q.front().id; top->ddr_b_resp=0;
        } else {
            top->ddr_b_valid=0; top->ddr_b_id=0; top->ddr_b_resp=0;
        }
        top->ddr_r_valid=0;
        if(mem_q.empty()) return;
        MemRead& h=mem_q.front();
        if(h.latency>0) {--h.latency;ddr_setup_wait=true;return;}
        top->ddr_r_valid=1;
        top->ddr_r_data=read64(h.addr+uint64_t(h.beat)*8);
        top->ddr_r_id=h.id;
        top->ddr_r_resp=0;
        top->ddr_r_last=(h.beat==h.len)?1:0;
    }

    void guards() {
        CHECK(top->dbg_ra_tag_fault==0,"queued read translation tag underflow/overflow");
        CHECK(top->dbg_e2_fault==0,"E2 completion pool fault");
        CHECK(top->dbg_e2_mode==0||top->dbg_dma_fullbuffer==0,"unexpected fullbuffer in E2 mode");
        CHECK(top->dbg_e2_mode<4||top->dbg_e2_queued<=16,"metadata queue exceeds sixteen entries");
        CHECK(top->dbg_dma_launch_error==0,"DMA normal launch guard failed");
        CHECK(top->dbg_dma_batch_active==0&&top->dbg_dma_batch_gap==0,"batch activity in batch-OFF experiment");
        CHECK(!top->dbg_m_arm&&!top->dbg_m_commit&&!top->dbg_m_timeout&&
              !top->dbg_m_masked&&!top->dbg_o_wait&&!top->dbg_o_done&&!top->dbg_o_l1_offer,
              "M/O controls active in PairPolicy0 experiment");
        CHECK(top->dbg_cpu_issued==0&&top->dbg_cpu_completed==0&&top->dbg_cpu_wr_issued==0&&
              top->dbg_cpu_amo_issued==0,"disabled synthetic CPU generated traffic");
        stable(ra_up,top->dbg_ra_up_valid,top->dbg_ra_up_ready,top->dbg_ra_up_id,top->dbg_ra_up_addr,
               top->dbg_ra_up_len,top->dbg_ra_up_attrs,"read admission upstream changed held AR");
        stable(ra_down,top->dbg_ra_down_valid,top->dbg_ra_down_ready,top->dbg_ra_down_id,top->dbg_ra_down_addr,
               top->dbg_ra_down_len,top->dbg_ra_down_attrs,"read admission downstream changed held AR");
    }

    // Observation-only endpoint offers. first is emitted on each new offer;
    // held VALID must remain stable until the endpoint accepts it.
    void observe_offer(Held& h,const char* first,const char* accept,bool valid,bool ready,
                       unsigned id,uint64_t addr,unsigned len) {
        const bool was_held=h.held;
        stable(h,valid,ready,id,addr,len,0,"endpoint changed held AR");
        if(valid&&!was_held) event(first,id,addr,len);
        if(valid&&ready) event(accept,id,addr,len);
    }
    uint64_t walk_seq=0;
    unsigned walk_bank=0;
    uint64_t walk_iova=0;
    void observe_translation() {
        if(top->dbg_tl_req_valid&&top->dbg_tl_req_ready) ++tl_accept;
        if(top->dbg_tl_resp_valid&&top->dbg_tl_resp_ready) {
            ++tl_resp; event("tl_response",top->dbg_tl_resp_bank,0,0);
        }
        if(top->dbg_fast_resp_valid&&top->dbg_fast_resp_ready)
            event("n_response",top->dbg_fast_resp_bank,0,0);
        if(top->dbg_disc_normal_accept||top->dbg_disc_n_accept) {
            const bool n=top->dbg_disc_n_accept;
            CHECK(!(n&&top->dbg_disc_normal_accept),"one shared request accepted on both normal/N paths");
            if(n) ++n_accept; else ++normal_accept;
            const unsigned bank=top->dbg_disc_req_bank;
            Parent& p=active(bank);
            CHECK(p.issued&&!p.retired,"translation accepted outside active parent");
            event(n?"n_accept":"normal_accept",bank,top->dbg_disc_req_iova,
                  uint64_t(top->dbg_disc_req_did)|(uint64_t(top->dbg_disc_req_type)<<24));
            progress(bank);
        }
        if(top->dbg_disc_lu_valid&&top->dbg_disc_lu_first) {
            const bool n=top->dbg_disc_lu_is_n,hit=top->dbg_disc_lu_hit;
            if(n) {++n_probes;if(hit)++n_probe_hits;}
            else {++normal_lookup_first;if(hit)++normal_lookup_hit;}
            // tag bit0=bank, bit1=N probe, bit2=physical hit, bit3=first.
            event("lookup",unsigned(top->dbg_disc_lu_bank)|(unsigned(n)<<1)|(unsigned(hit)<<2)|8u,
                  top->dbg_disc_lu_iova,
                  uint64_t(top->dbg_disc_lu_did)|(uint64_t(top->dbg_disc_lu_type)<<24));
        }
        const uint64_t main_value=uint64_t(top->dbg_disc_main_valid)|
            (uint64_t(top->dbg_disc_main_state)<<1)|(uint64_t(top->dbg_disc_main_bank)<<6)|
            (uint64_t(top->dbg_disc_main_did)<<7)|(uint64_t(top->dbg_disc_main_type)<<31);
        state("main_state",main_value,0,top->dbg_disc_main_iova);
        // IOVA may change while all packed fields stay unchanged.
        state("main_iova",top->dbg_disc_main_iova,top->dbg_disc_main_bank,main_value);
        if(top->dbg_disc_ptw_start) {
            ++ptw_starts;walk_seq=ptw_starts;
            walk_bank=top->dbg_disc_main_bank;walk_iova=top->dbg_disc_main_iova;
            event("ptw_start",walk_bank,walk_iova,walk_seq);
            event("ptw_owner",unsigned(walk_seq),walk_iova,
                uint64_t(top->dbg_disc_main_did)|(uint64_t(top->dbg_disc_main_type)<<24)|
                (uint64_t(walk_bank)<<30));
        }
        if(top->dbg_tl_ptw_active) ++ptw_active_ticks;
        if(observed_ptw_active&&!top->dbg_tl_ptw_active)
            event("ptw_end",walk_bank,walk_iova,walk_seq);
        observed_ptw_active=top->dbg_tl_ptw_active;
        state("ptw_state",uint64_t(top->dbg_disc_ptw_state)|(uint64_t(top->dbg_disc_ptw_level)<<8),
              unsigned(walk_seq),top->dbg_disc_ptw_addr);
        observe_offer(ptw_local,"pte_local_first","pte_local_accept",top->dbg_ptw_local_valid,
            top->dbg_ptw_local_ready,top->dbg_disc_ptw_id,top->dbg_disc_ptw_addr,0);
        observe_offer(ds_merge,"ds_merge_first","ds_merge_accept",top->dbg_disc_ds_merge_valid,
            top->dbg_disc_ds_merge_ready,top->dbg_disc_ds_merge_id,top->dbg_disc_ds_merge_addr,
            top->dbg_disc_ds_merge_len);
        observe_offer(ds_delayed,"ds_first","ds_accept",top->dbg_ds_ar_valid,top->dbg_ds_ar_ready,
            top->dbg_disc_ds_ar_id,top->dbg_disc_ds_ar_addr,top->dbg_disc_ds_ar_len);
        state("ptw_r_channel",unsigned(top->dbg_disc_ptw_r_valid)|(unsigned(top->dbg_disc_ptw_r_ready)<<1)|
              (unsigned(top->dbg_disc_ptw_r_last)<<2),top->dbg_disc_ptw_r_id);
        if(top->dbg_disc_ptw_r_valid&&top->dbg_disc_ptw_r_ready)
            event("pte_local_return",top->dbg_disc_ptw_r_id,0,top->dbg_disc_ptw_r_last);
        state("ds_r_channel",unsigned(top->dbg_ds_r_valid)|(unsigned(top->dbg_ds_r_ready)<<1)|
              (unsigned(top->dbg_disc_ds_r_last)<<2),top->dbg_disc_ds_r_id);
        if(top->dbg_ds_r_valid&&top->dbg_ds_r_ready)
            event("ds_return",top->dbg_disc_ds_r_id,0,top->dbg_disc_ds_r_last);
        if(top->dbg_iotlb_fill) event("fill",top->dbg_main_bank,0,0);
        if(top->dbg_ddtw_ar_fire) {++ddtw_ar;event("ddtw_ar",0);}
        if(top->dbg_ptw_ar_fire) {++ptw_ar;event("ptw_ar",walk_bank);}
        // No precedence is assigned to simultaneous flags.
        const unsigned aw=unsigned(top->dbg_tr_aw_stall)|(unsigned(top->dbg_tr_aw_bank)<<1)|
            (unsigned(top->dbg_aw_wait_flags)<<2);
        state("aw_wait_change",aw);
        state("source_drained_state",top->dbg_src_drained);
    }
    void observe_parents_begin() {
        for(unsigned c=0;c<2;++c) {
            const unsigned bit=1u<<c;
            if(top->dbg_dma_issue&bit) {
                Parent& p=active(c);
                CHECK(!p.issued&&!p.retired,"duplicate normal ISSUE");
                CHECK(hw_issue[c]==hw_done[c]&&hw_issue[c]<PARENTS,"single-active ISSUE guard");
                CHECK(p.j==hw_issue[c],"hardware ISSUE parent order");
                p.issued=true;p.issue=g_cycle;++hw_issue[c];progress(c);
                event("issue",c,p.src_iova,p.j);
            }
            if(top->dbg_backend_fire&bit) {
                Parent& p=active(c);
                CHECK(p.issued&&!p.retired&&p.backend<0,"backend parent overlap/identity");
                p.backend=g_cycle;p.backend_active=true;progress(c);
                event("backend_accept",c,p.src_iova,p.j);
            }
            const bool av=top->dbg_local_ar_valid&bit,ar=top->dbg_local_ar_ready&bit;
            if(av) {
                Parent& p=active(c);
                CHECK(p.backend_active,"local AR outside accepted backend parent");
                CHECK(top->dbg_local_ar_len[c]==255,"ordinary COPY read split changed");
                const uint64_t a=top->dbg_local_ar_addr[c];
                CHECK(a>=p.src_iova&&a+2048<=p.src_iova+PAGE_BYTES,"local AR outside fixed source");
                if(!local_ar_shadow[c].held) event("dma_ar_first",c,a,top->dbg_local_ar_len[c]);
                if(ar) {++p.local_ar;++p.local_outstanding;progress(c);event("dma_ar_accept",c,a,top->dbg_local_ar_len[c]);}
            }
            stable(local_ar_shadow[c],av,ar,0,top->dbg_local_ar_addr[c],top->dbg_local_ar_len[c],0,
                   "local DMA changed held source AR");
            if(top->dbg_local_r_fire&bit) {
                Parent& p=active(c);
                CHECK(p.backend_active&&p.local_r<PAGE_BEATS,"local R outside accepted parent or overflow");
                ++p.local_r;if(p.first_r<0)p.first_r=g_cycle;progress(c);
                event("dma_r",c,p.src_iova,p.local_r);
                if(top->dbg_local_r_last&bit) {
                    CHECK(p.local_outstanding>0,"local RLAST without AR");
                    --p.local_outstanding;event("dma_rlast",c,p.src_iova,p.local_r);
                }
                if(p.local_r==PAGE_BEATS) {
                    CHECK(p.local_outstanding==0,"source512 reached before all local RLAST");
                    p.source_complete=g_cycle;event("source_complete",c,p.src_iova,p.j);
                }
            }
        }
        if(hw_issue[0]>hw_done[0]&&hw_issue[1]>hw_done[1]) ++overlap_ticks;
    }
    void observe_parent_done() {
        for(unsigned c=0;c<2;++c) if(top->dbg_dma_done&(1u<<c)) {
            Parent& p=active(c);
            CHECK(p.issued&&p.backend_active&&!p.retired,"DONE without active backend parent");
            CHECK(hw_done[c]<hw_issue[c]&&hw_done[c]<PARENTS,"extra hardware DONE");
            CHECK(p.local_r==PAGE_BEATS&&p.local_ar==2&&p.local_outstanding==0,
                  "parent DONE without all local source data");
            CHECK(p.ar==2&&p.aw==2&&p.r_beats==PAGE_BEATS&&p.w_beats==PAGE_BEATS&&p.b==2,
                  "parent R/W/B transaction conservation failed");
            CHECK(p.rd_bytes==PAGE_BYTES&&p.wr_bytes==PAGE_BYTES&&p.err==0,
                  "parent byte/error conservation failed");
            CHECK(std::all_of(p.r_seen.begin(),p.r_seen.end(),[](uint8_t x){return x==1;})&&
                  std::all_of(p.w_seen.begin(),p.w_seen.end(),[](uint8_t x){return x==1;}),
                  "parent byte coverage failed");
            CHECK(p.last_b>=0&&p.last_b<=g_cycle&&p.source_complete<=g_cycle,"DONE preceded required source/B endpoint");
            p.done=g_cycle;p.retired=true;p.backend_active=false;++hw_done[c];progress(c);
            event("parent_done",c,p.dst_iova,p.j);
            if(p.j+1<PARENTS) event("logical_ready",c,0,p.j+1);
        }
    }

    MemRead accepted_read() {
        const uint64_t addr=top->ddr_ar_addr;
        const unsigned len=top->ddr_ar_len,id=top->ddr_ar_id;
        CHECK(addr>=0x80000000ULL&&addr<0xc0000000ULL,"AR outside frozen DRAM decode");
        CHECK(top->ddr_ar_size==3&&top->ddr_ar_burst==1,"DDR AR must retain 64bit INCR");
        CHECK(addr+(uint64_t(len)+1)*8<=0xc0000000ULL,"AR extends outside DRAM decode");
        MemRead r;r.addr=addr;r.len=len;r.id=id;r.occ=++next_read_occ;
        r.accepted_cycle=g_cycle;r.beat=0;r.latency=31;
        const int lane=payload_lane(id);
        if(lane>=0) {
            Parent& p=active(unsigned(lane));
            CHECK(p.issued&&!p.retired&&p.backend_active,"payload DDR AR outside parent");
            CHECK(len==255&&addr>=p.src_pa&&addr+2048<=p.src_pa+PAGE_BYTES,
                  "payload AR outside fixed source or ordinary split");
            CHECK((addr>>12)==((addr+2047)>>12),"payload AR crossed 4KiB");
            r.lane=lane;r.parent=current[lane];++p.ar;p.rd_bytes+=(uint64_t(len)+1)*8;
            CHECK(p.ar<=2&&p.rd_bytes<=PAGE_BYTES,"parent payload AR overflow");progress(lane);
        }
        ++ddr_ar;event("full_ddr_ar",id,addr,len);event("ddr_ar_occ",id,addr,r.occ);
        return r;
    }
    CawEnt accepted_write() {
        const uint64_t addr=top->ddr_aw_addr;
        const unsigned len=top->ddr_aw_len,id=top->ddr_aw_id;
        CHECK(addr>=0x80000000ULL&&addr<0xc0000000ULL,"AW outside frozen DRAM decode");
        CHECK(top->ddr_aw_size==3&&top->ddr_aw_burst==1&&len==255,
              "COPY AW must retain 256beat 64bit INCR");
        const int lane=payload_lane(id);
        CHECK(lane>=0,"unexpected non-DMA write with CPU disabled");
        Parent& p=active(unsigned(lane));
        CHECK(p.issued&&!p.retired&&p.backend_active,"payload DDR AW outside parent");
        CHECK(addr>=p.dst_pa&&addr+2048<=p.dst_pa+PAGE_BYTES,"AW outside current parent destination");
        CHECK((addr>>12)==((addr+2047)>>12),"payload AW crossed 4KiB");
        CawEnt w;w.id=id;w.addr=addr;w.len=len;w.lane=lane;w.parent=current[lane];w.occ=++next_write_occ;
        ++p.aw;CHECK(p.aw<=2,"parent AW overflow");progress(lane);
        ++ddr_aw;event("full_ddr_aw",id,addr,len);event("ddr_aw_occ",id,addr,w.occ);
        return w;
    }
    void returned_r() {
        CHECK(!mem_q.empty(),"DDR R with empty queue");
        const MemRead& r=mem_q.front();
        CHECK(top->ddr_r_id==r.id&&bool(top->ddr_r_last)==(r.beat==r.len),
              "DDR R ID/LAST differs from FIFO head");
        CHECK(r.beat<=r.len,"DDR R beat overflow");
        ++ddr_r;event("full_ddr_r",r.id,r.addr,r.beat);
        if(r.beat==0) event("ddr_r_first",r.id,r.addr,r.occ);
        if(r.beat==r.len) event("ddr_r_last",r.id,r.addr,r.occ);
        if(r.lane>=0) {
            Parent& p=identified(r.lane,r.parent);
            CHECK(p.issued&&!p.retired,"DDR source R outside original parent");
            const uint64_t addr=r.addr+uint64_t(r.beat)*8;
            CHECK(addr>=p.src_pa&&addr+8<=p.src_pa+PAGE_BYTES,"returned R outside fixed source");
            const unsigned offset=unsigned(addr-p.src_pa);
            CHECK(top->ddr_r_data==page_word(unsigned(r.lane),offset/8),"DDR source R data corrupted");
            for(unsigned i=0;i<8;++i) {CHECK(p.r_seen[offset+i]==0,"duplicate source R byte");++p.r_seen[offset+i];}
            ++p.r_beats;CHECK(p.r_beats<=PAGE_BEATS,"parent R overflow");
            if(top->ddr_r_resp!=0) ++p.err;progress(r.lane);
        }
    }
    void written_w(const CawEnt& w) {
        CHECK(w.got<=w.len,"W beyond AW burst");
        CHECK(bool(top->ddr_w_last)==(w.got==w.len),"W LAST/burst length mismatch");
        Parent& p=identified(w.lane,w.parent);
        CHECK(p.issued&&!p.retired,"W outside original parent");
        const uint64_t addr=w.addr+uint64_t(w.got)*8;
        CHECK(addr>=p.dst_pa&&addr+8<=p.dst_pa+PAGE_BYTES,"W outside original parent destination");
        const unsigned offset=unsigned(addr-p.dst_pa);
        const unsigned strb=top->ddr_w_strb;
        CHECK(strb==0xff,"aligned normal4096 COPY must write all eight strobe bytes");
        const uint64_t expected=page_word(unsigned(w.lane),offset/8);
        for(unsigned i=0;i<8;++i) if(strb&(1u<<i)) {
            CHECK(uint8_t(top->ddr_w_data>>(8*i))==uint8_t(expected>>(8*i)),"COPY W strobe data mismatch");
            CHECK(p.w_seen[offset+i]==0,"duplicate destination W byte");++p.w_seen[offset+i];++p.wr_bytes;
        }
        ++p.w_beats;CHECK(p.w_beats<=PAGE_BEATS&&p.wr_bytes<=PAGE_BYTES,"parent W overflow");
        progress(w.lane);++ddr_w;
        // Preserve old raw W encoding: tag=LAST, addr=WDATA, value=WSTRB.
        event("full_ddr_w",top->ddr_w_last,top->ddr_w_data,strb);
        if(w.got==0||w.got==w.len) event("ddr_w_position",w.id,addr,w.occ);
    }
    void returned_b() {
        CHECK(!cb_q.empty(),"DDR B with empty queue");
        const BEnt& b=cb_q.front();
        CHECK(top->ddr_b_id==b.id,"DDR B ID differs from original AW");
        Parent& p=identified(b.lane,b.parent);
        CHECK(p.issued&&!p.retired,"B outside original parent");
        ++p.b;CHECK(p.b<=2,"parent B overflow");p.last_b=g_cycle;
        if(top->ddr_b_resp!=0) ++p.err;
        progress(b.lane);++ddr_b;event("full_ddr_b",b.id,0,0);event("ddr_b_occ",b.id,0,b.occ);
    }
    void observe_ddr_model() {
        // drive_ddr already performed this tick's one head-latency decrement.
        // These are model truth, unavailable to DUT/control logic.
        state("ddr_model_empty",mem_q.empty());
        unsigned id=0;uint64_t occ=0,addr=0;
        if(!mem_q.empty()) {
            const MemRead& h=mem_q.front();id=h.id;occ=h.occ;addr=h.addr;
            state("ddr_head",occ,id,addr);
            state("ddr_head_progress",uint64_t(h.beat)|(uint64_t(unsigned(h.latency))<<16),unsigned(occ),addr);
            state("ddr_head_attrs",uint64_t(id)|(uint64_t(h.len)<<8),unsigned(occ),addr);
        }
        state("r_channel_change",unsigned(top->ddr_r_valid)|(unsigned(top->ddr_r_ready)<<1)|
              (unsigned(top->ddr_r_last&&top->ddr_r_valid)<<2)|(unsigned(ddr_setup_wait)<<3)|
              (unsigned(mem_q.empty())<<4),unsigned(occ),addr);
        state("ddr_aw_channel",unsigned(top->ddr_aw_valid)|(unsigned(top->ddr_aw_ready)<<1));
        state("ddr_w_channel",unsigned(top->ddr_w_valid)|(unsigned(top->ddr_w_ready)<<1));
        state("ddr_b_channel",unsigned(top->ddr_b_valid)|(unsigned(top->ddr_b_ready)<<1));
    }

    void watchdog() {
        CHECK(g_cycle<TICK_LIMIT,"total 1000000-tick hard ceiling reached");
        if(!era) return;
        for(unsigned c=0;c<2;++c) if(hw_done[c]<PARENTS&&g_cycle-last_progress[c]>WATCHDOG) {
            event("watchdog",c,hw_issue[c],hw_done[c]);
            dump_stall("independent lane watchdog");
            throw TestFailure(std::string(c?"A":"V")+" lane made no progress for 200000 ticks");
        }
    }
    void tick() {
        watchdog();
        top->nonblock_enable_i=(mode=="pn");top->policy_partition_i=1;
        top->owner_only_i=0;top->cpu_enable_i=0;top->read_admit_mode_i=0;
        top->read_budget_i=256;top->read_period_i=256;
        top->cfg_sel=cfg_sel;top->cfg_valid=cfg_valid;top->cfg_write=cfg_write;
        top->cfg_addr=cfg_addr;top->cfg_wdata=cfg_wdata;top->cfg_wstrb=cfg_wstrb;
        top->clk_i=0;drive_ddr();top->eval();
        // Every handshake, payload and debug pulse below is sampled BEFORE
        // the rising edge. Model queue mutation is AFTER it. No post-edge
        // READY/pulse/response can be mistaken for this edge's acceptance.
        const bool ar=top->ddr_ar_valid&&top->ddr_ar_ready;
        const bool aw=top->ddr_aw_valid&&top->ddr_aw_ready;
        const bool w=top->ddr_w_valid&&top->ddr_w_ready;
        const bool b=top->ddr_b_valid&&top->ddr_b_ready;
        const bool r=top->ddr_r_valid&&top->ddr_r_ready;
        const uint64_t wd=top->ddr_w_data;
        const unsigned ws=top->ddr_w_strb;
        const bool wl=top->ddr_w_last;
        MemRead new_ar;CawEnt new_aw;
        if(top->rst_ni) {
            guards();
            observe_offer(selector,"sel_first","sel_accept",top->dbg_sel_valid,top->dbg_sel_ready,
                          top->dbg_sel_id,top->dbg_sel_addr,top->dbg_sel_len);
            if(era) {
                observe_parents_begin();observe_translation();observe_ddr_model();
            }
            if(ar) new_ar=accepted_read();
            if(aw) new_aw=accepted_write();
            if(w) {
                CHECK(!caw_q.empty()||aw,"W without pending/same-edge AW");
                written_w(caw_q.empty()?new_aw:caw_q.front());
            }
            if(b) returned_b();
            if(r) returned_r();
            if(era) observe_parent_done();
        }
        top->clk_i=1;top->eval();
        // Exact original service order: enqueue AR/AW, write W and enqueue B,
        // dequeue old B, then advance the old FIFO R head. All use snapshots.
        if(ar) mem_q.push_back(new_ar);
        if(aw) caw_q.push_back(new_aw);
        if(w) {
            CHECK(!caw_q.empty(),"model W queue empty");
            CawEnt& c=caw_q.front();write_beat(c.addr+uint64_t(c.got)*8,wd,ws);++c.got;
            if(wl) {
                CHECK(c.got==c.len+1,"model W burst length mismatch");
                BEnt q;q.id=c.id;q.occ=c.occ;q.lane=c.lane;q.parent=c.parent;cb_q.push_back(q);caw_q.pop_front();
            }
        }
        if(b) {CHECK(!cb_q.empty(),"model B queue empty");cb_q.pop_front();}
        if(r) {
            CHECK(!mem_q.empty(),"model R queue empty");
            if(mem_q.front().beat==mem_q.front().len) mem_q.pop_front();else ++mem_q.front().beat;
        }
        top->clk_i=0;top->eval();++g_cycle;
    }

    uint64_t cfg_xact(bool sel,bool write,uint64_t addr,uint64_t data,unsigned strb) {
        cfg_sel=sel;cfg_write=write;cfg_addr=addr;cfg_wdata=data;cfg_wstrb=strb;cfg_valid=true;
        for(long n=0;n<CFG_TIMEOUT;++n) {
            tick();
            if(top->cfg_ready) {
                // cfg_ready/rdata/error are deliberately read AFTER the edge:
                // C_HOLD captures the real register R/B response at that edge.
                const uint64_t value=top->cfg_rdata;
                const bool error=top->cfg_error;
                cfg_valid=false;tick();  // C_HOLD -> IDLE, same as old driver
                CHECK(!error,"configuration AXI returned an error");
                return value;
            }
        }
        cfg_valid=false;dump_stall("cfg timeout");throw TestFailure("configuration transaction timeout");
    }
    void cfg_wr(unsigned lane,uint64_t addr,uint64_t data) {cfg_xact(lane==1,true,addr,data,0xff);}
    uint64_t cfg_rd(unsigned lane,uint64_t addr) {return cfg_xact(lane==1,false,addr,0,0);}
    void reset() {
        CHECK(!era,"reset during discovery era");
        top->clk_i=0;top->rst_ni=0;cfg_valid=false;
        mem_q.clear();caw_q.clear();cb_q.clear();
        for(unsigned i=0;i<8;++i) tick();
        top->rst_ni=1;
        // Read-only clock anchor: local IOTLB counter zero is this first released edge.
        std::printf("IOTLB_CYCLE_ANCHOR,%ld,0\n",g_cycle);
        for(unsigned i=0;i<4;++i) tick();
        // Same two ordinary debug baseline reads as the previous fixture.
        (void)cfg_rd(0,REG_DONE);(void)cfg_rd(1,REG_DONE);
        CHECK(top->dbg_policy_elaborated,"PolicyEnable base required");
    }
    bool quiet_now() const {
        return top->dbg_in_flight==0&&top->dbg_dma_job_quiet==3&&
               top->dbg_cpu_quiet&&top->dbg_cpu_wr_quiet;
    }
    void check_quiet() {
        for(long i=0;i<WATCHDOG;++i) {
            tick();
            if(quiet_now()) {
                CHECK(mem_q.empty()&&caw_q.empty()&&cb_q.empty(),"hardware quiet with pending DDR/AW/B queues");
                return;
            }
        }
        dump_stall("quiet timeout");throw TestFailure("final quiet not reached");
    }
    void prepare(unsigned lane,unsigned j) {
        CHECK(j<PARENTS&&parents[lane].size()==j,"software parent quota/order");
        if(j) {
            Parent& prev=parents[lane].back();
            CHECK(prev.retired&&prev.poll_end>=0,"refill before exact preceding DONE observation");
        }
        Parent p;p.lane=lane;p.j=j;
        p.src_iova=lane?A_SRC_IOVA:V_SRC_IOVA;p.src_pa=lane?A_SRC_PA:V_SRC_PA;
        p.dst_iova=lane?A_DST_IOVA+(j%ring)*4096ULL:V_DST_IOVA;
        p.dst_pa=lane?A_DST_PA+(j%ring)*4096ULL:V_DST_PA;
        p.ready=j?parents[lane].back().done:t0;
        parents[lane].push_back(std::move(p));current[lane]=int(j);
        Parent& cur=active(lane);cur.cfg_begin=g_cycle;event("cfg_begin",lane,cur.dst_iova,j);
        cfg_wr(lane,REG_CONF,0);cfg_wr(lane,REG_SRC,cur.src_iova);
        cfg_wr(lane,REG_DST,cur.dst_iova);cfg_wr(lane,REG_NB,PAGE_BYTES);
        cur.cfg_end=g_cycle;event("cfg_end",lane,cur.dst_iova,j);
    }
    void launch(unsigned lane) {
        Parent& p=active(lane);
        CHECK(!p.id_known&&!p.issued,"duplicate software NEXT");
        p.next_begin=g_cycle;event("next_begin",lane,p.dst_iova,p.j);
        p.next_id=cfg_rd(lane,REG_NEXT);p.id_known=true;p.next_end=g_cycle;
        if(p.j) CHECK(p.next_id!=parents[lane][p.j-1].next_id,"NEXT returned previous parent ID");
        event("next_end",lane,p.next_id,p.j);
    }
    bool poll(unsigned lane) {
        Parent& p=active(lane);CHECK(p.id_known,"poll without exact NEXT ID");
        const long begin=g_cycle;
        if(p.first_poll<0)p.first_poll=begin;
        ++p.polls;event("poll_begin",lane,p.next_id,p.j);
        const uint64_t id=cfg_rd(lane,REG_DONE);
        event("poll_end",lane,id,p.j);
        if(id!=p.next_id) return false;
        CHECK(p.retired&&p.done<=g_cycle,"frontend exact DONE observed before hardware DONE");
        p.poll_begin=begin;p.poll_end=g_cycle;event("poll_exact_done",lane,id,p.j);return true;
    }
    void verify_payload() const {
        for(unsigned i=0;i<PAGE_BEATS;++i) {
            CHECK(read64(V_SRC_PA+i*8ULL)==page_word(0,i),"V source mutated");
            CHECK(read64(A_SRC_PA+i*8ULL)==page_word(1,i),"A fixed source mutated");
            CHECK(read64(V_DST_PA+i*8ULL)==page_word(0,i),"V final destination mismatch");
            for(unsigned p=0;p<ring;++p)
                CHECK(read64(A_DST_PA+p*4096ULL+i*8ULL)==page_word(1,i),"A ring final destination mismatch");
        }
    }
    void run() {
        parents[0].reserve(PARENTS);parents[1].reserve(PARENTS);
        setup_memory();reset();check_quiet();
        CHECK((cfg_rd(0,REG_B_CTL)&1)==0&&(cfg_rd(1,REG_B_CTL)&1)==0,"batch enabled after reset");
        check_quiet();
        initial_cycles=g_cycle;t0=g_cycle;era=true;
        last_progress[0]=last_progress[1]=t0;
        event("t0",0,0,0);event("logical_ready",1,0,0);event("logical_ready",0,0,0);
        // Both configurations finish before either trigger; fixed A, V order.
        prepare(1,0);prepare(0,0);launch(1);launch(0);
        unsigned observed[2]={0,0};
        while(observed[0]<PARENTS||observed[1]<PARENTS) {
            // This fixed closed-loop order is the complete external program.
            // A completed lane stops polling/refilling; the other still drains
            // to its own finite quota, without STOP or batch/autonomous work.
            for(unsigned lane:{0u,1u}) if(observed[lane]<PARENTS&&poll(lane)) {
                ++observed[lane];
                if(observed[lane]<PARENTS) {prepare(lane,observed[lane]);launch(lane);}
            }
            tick();
        }
        for(unsigned c=0;c<2;++c) {
            CHECK(hw_issue[c]==PARENTS&&hw_done[c]==PARENTS,"lane did not finish exactly64 hardware parents");
            CHECK(cfg_rd(c,REG_DONE)==parents[c].back().next_id,"final exact frontend DONE mismatch");
            CHECK((cfg_rd(c,REG_B_CTL)&1)==0,"autonomous batch enabled");
        }
        check_quiet();quiet_cycle=g_cycle;final_quiet=true;
        event("final_quiet",0,0,0);era=false;
        verify_payload();
        CHECK(tl_accept==tl_resp,"all-era translation request/response conservation");
        CHECK(normal_accept+n_accept==tl_accept,"normal/N accept partition does not match shared endpoint");
        for(unsigned c=0;c<2;++c) {
            uint64_t ar=0,aw=0,rr=0,ww=0,b=0,rd=0,wr=0;
            for(const Parent& p:parents[c]) {
                CHECK(p.retired&&p.id_known&&p.poll_end>=0,"unretired/unobserved parent at final quiet");
                CHECK(p.ready<=p.cfg_begin&&p.cfg_begin<=p.cfg_end&&p.cfg_end<=p.next_begin&&
                      p.next_begin<=p.issue&&p.issue<=p.backend&&p.backend<=p.first_r&&
                      p.first_r<=p.source_complete&&p.source_complete<=p.done&&p.done<=p.poll_end,
                      "parent dependency timestamps out of order");
                CHECK(p.last_b<=p.done,"parent hardware DONE preceded B");
                ar+=p.ar;aw+=p.aw;rr+=p.r_beats;ww+=p.w_beats;b+=p.b;rd+=p.rd_bytes;wr+=p.wr_bytes;
            }
            CHECK(ar==128&&aw==128&&rr==32768&&ww==32768&&b==128&&rd==262144&&wr==262144,
                  "lane full workload conservation failed");
        }
    }
    long rel(long x) const {return x<0||t0<0?-1:x-t0;}
    void write_parents() {
        if(!parents_file)return;
        std::fprintf(parents_file,"lane,j,src_iova,dst_iova,src_pa,dst_pa,next_id,ready,cfg_begin,cfg_end,next_begin,next_end,poll_begin,poll_end,issue,backend,first_r,source_complete,last_b,done,ar,aw,r_beats,w_beats,b,rd_bytes,wr_bytes,err,local_ar,local_r,first_poll,polls,id_known,retired\n");
        for(unsigned c=0;c<2;++c) for(const Parent& p:parents[c]) {
            std::fprintf(parents_file,"%u,%u,%llu,%llu,%llu,%llu,%llu",c,p.j,
                (unsigned long long)p.src_iova,(unsigned long long)p.dst_iova,
                (unsigned long long)p.src_pa,(unsigned long long)p.dst_pa,(unsigned long long)p.next_id);
            for(long x:{p.ready,p.cfg_begin,p.cfg_end,p.next_begin,p.next_end,p.poll_begin,p.poll_end,
                        p.issue,p.backend,p.first_r,p.source_complete,p.last_b,p.done})
                std::fprintf(parents_file,",%ld",rel(x));
            for(uint64_t x:{p.ar,p.aw,p.r_beats,p.w_beats,p.b,p.rd_bytes,p.wr_bytes,p.err,p.local_ar,p.local_r})
                std::fprintf(parents_file,",%llu",(unsigned long long)x);
            std::fprintf(parents_file,",%ld,%llu,%u,%u\n",rel(p.first_poll),(unsigned long long)p.polls,
                         unsigned(p.id_known),unsigned(p.retired));
        }
        CHECK(std::fflush(parents_file)==0,"parents CSV flush failed");
    }
    static std::string json_quote(const std::string& s) {
        std::string out="\"";
        for(unsigned char c:s) {
            if(c=='"'||c=='\\') {out+='\\';out+=char(c);}
            else if(c=='\n')out+="\\n";
            else if(c=='\r')out+="\\r";
            else if(c=='\t')out+="\\t";
            else if(c<32) {char b[7];std::snprintf(b,sizeof(b),"\\u%04x",c);out+=b;}
            else out+=char(c);
        }
        return out+'"';
    }
    void report(const char* status,const std::string& error="") const {
        const long tv=parents[0].empty()?-1:rel(parents[0].back().done);
        const long ta=parents[1].empty()?-1:rel(parents[1].back().done);
        std::printf("RESULT_JSON {\"schema\":\"discovery-reuse-v1\",\"status\":%s,\"error\":%s,\"mode\":%s,\"ring\":%u,\"a_leaf\":%s,\"history\":\"cold\",\"policy_enable\":1,\"pair_policy\":0,\"partition\":true,\"nonblock\":%s,\"common_ds_first_f4\":true,\"ddr_startup\":31,\"ds_delay\":[50,50],\"cpu\":false,\"batch\":false,\"M\":false,\"O\":false,\"return_reorder\":false,\"queued_startup_overlap\":false,\"launch_order\":\"av\",\"poll_order\":\"va\",\"t0\":%ld,\"initialization_ticks\":%ld,\"ticks\":%ld,\"hard_tick_limit\":1000000,\"lane_watchdog\":200000,\"T_V\":%ld,\"T_A\":%ld,\"T_data\":%ld,\"T_return\":%ld,\"quiet\":%s,\"both_pending_ticks\":%llu,\"arena_reserved_bytes\":2097152,\"effective_payload_pages\":%u,\"metadata_reserved_bytes\":28672,\"sparse_initialized_bytes\":%zu,\"channels\":[",
            json_quote(status).c_str(),json_quote(error).c_str(),json_quote(mode).c_str(),ring,json_quote(a_leaf).c_str(),
            mode=="pn"?"true":"false",t0,initial_cycles,g_cycle,tv,ta,tv<0||ta<0?-1:std::max(tv,ta),
            rel(quiet_cycle),final_quiet?"true":"false",(unsigned long long)overlap_ticks,ring+3,mem.size());
        for(unsigned c=0;c<2;++c) {
            uint64_t ar=0,aw=0,rr=0,ww=0,b=0,rd=0,wr=0,err=0,polls=0;
            for(const Parent& p:parents[c]) {ar+=p.ar;aw+=p.aw;rr+=p.r_beats;ww+=p.w_beats;b+=p.b;
                rd+=p.rd_bytes;wr+=p.wr_bytes;err+=p.err;polls+=p.polls;}
            std::printf("%s{\"id\":\"%s\",\"issue\":%llu,\"done\":%llu,\"parents_supplied\":%zu,\"ar\":%llu,\"aw\":%llu,\"r_beats\":%llu,\"w_beats\":%llu,\"b\":%llu,\"rd_bytes\":%llu,\"wr_bytes\":%llu,\"err\":%llu,\"polls\":%llu}",c?",":"",c?"A":"V",
                (unsigned long long)hw_issue[c],(unsigned long long)hw_done[c],parents[c].size(),
                (unsigned long long)ar,(unsigned long long)aw,(unsigned long long)rr,(unsigned long long)ww,
                (unsigned long long)b,(unsigned long long)rd,(unsigned long long)wr,(unsigned long long)err,(unsigned long long)polls);
        }
        std::printf("],\"raw\":{\"tl_accept\":%llu,\"tl_resp\":%llu,\"normal_accept\":%llu,\"n_accept\":%llu,\"normal_lookup_first\":%llu,\"normal_lookup_hit\":%llu,\"n_probes\":%llu,\"n_probe_hits\":%llu,\"ptw_starts\":%llu,\"ptw_active_ticks\":%llu,\"ddtw_ar\":%llu,\"ptw_ar\":%llu,\"ddr_ar\":%llu,\"ddr_aw\":%llu,\"ddr_r\":%llu,\"ddr_w\":%llu,\"ddr_b\":%llu}}\n",
            (unsigned long long)tl_accept,(unsigned long long)tl_resp,(unsigned long long)normal_accept,
            (unsigned long long)n_accept,(unsigned long long)normal_lookup_first,(unsigned long long)normal_lookup_hit,
            (unsigned long long)n_probes,(unsigned long long)n_probe_hits,(unsigned long long)ptw_starts,
            (unsigned long long)ptw_active_ticks,(unsigned long long)ddtw_ar,(unsigned long long)ptw_ar,
            (unsigned long long)ddr_ar,(unsigned long long)ddr_aw,(unsigned long long)ddr_r,
            (unsigned long long)ddr_w,(unsigned long long)ddr_b);
    }
    void dump_stall(const char* reason) const {
        std::fprintf(stderr,"DISCOVERY_STALL %s cycle=%ld relative=%ld cfg_state=%u inflight=%u quiet=%u\n",
            reason,g_cycle,t0<0?-1:g_cycle-t0,unsigned(top->dbg_cfg_state),unsigned(top->dbg_in_flight),unsigned(top->dbg_dma_job_quiet));
        for(unsigned c=0;c<2;++c) std::fprintf(stderr," lane=%s current=%d issue=%llu done=%llu since_progress=%ld\n",
            c?"A":"V",current[c],(unsigned long long)hw_issue[c],(unsigned long long)hw_done[c],g_cycle-last_progress[c]);
        if(!mem_q.empty()) {const MemRead& h=mem_q.front();std::fprintf(stderr,
            " DDR head occ=%llu id=%u addr=0x%llx len=%u beat=%u remaining_startup=%d\n",
            (unsigned long long)h.occ,h.id,(unsigned long long)h.addr,h.len,h.beat,h.latency);}
    }
};

int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);
    std::unique_ptr<Tb> tb;
    try {
        tb.reset(new Tb);
        std::string trace,parents;
        for(int i=1;i<argc;++i) {
            const std::string arg=argv[i];
            CHECK(i+1<argc,"option missing value");const std::string value=argv[++i];
            if(arg=="--mode")tb->mode=value;
            else if(arg=="--ring") {CHECK(value=="1"||value=="8"||value=="16","ring must be 1,8,16");tb->ring=unsigned(std::stoul(value));}
            else if(arg=="--a-leaf")tb->a_leaf=value;
            else if(arg=="--event-trace")trace=value;
            else if(arg=="--parents")parents=value;
            else throw TestFailure("unsupported discovery option: "+arg);
        }
        CHECK(tb->mode=="p"||tb->mode=="pn","mode must be p or pn");
        CHECK(tb->a_leaf=="4k"||tb->a_leaf=="2m","A leaf must be 4k or 2m");
        CHECK(!trace.empty()&&!parents.empty(),"event trace and parents outputs are required");
        CHECK(trace!=parents,"event trace and parents outputs must differ");
        // Exclusive creation preserves all earlier successful or failed traces.
        tb->event_trace=std::fopen(trace.c_str(),"wx");CHECK(tb->event_trace,"cannot create new event trace");
        tb->parents_file=std::fopen(parents.c_str(),"wx");CHECK(tb->parents_file,"cannot create new parents CSV");
        std::fprintf(tb->event_trace,"cycle,event,tag,addr,value\n");
        tb->run();tb->write_parents();
        CHECK(std::fflush(tb->event_trace)==0,"event trace flush failed");tb->report("PASS");
        std::puts("DISCOVERY_PASS");return 0;
    } catch(const std::exception& e) {
        std::fprintf(stderr,"DISCOVERY_FAIL: %s\n",e.what());
        if(tb) {
            tb->event("failure",0,0,0);
            try {tb->write_parents();}catch(const std::exception& save){std::fprintf(stderr,"partial parents save failed: %s\n",save.what());}
            if(tb->event_trace)std::fflush(tb->event_trace);
            tb->report("FAIL",e.what());
        }
        return 1;
    }
}
