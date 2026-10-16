// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Private, clock-coupled HERO kernel projection. Not HERO/CVA6 board timing.
class Tb;
static void stream_drive(Tb&);
static void stream_preedge(Tb&);
#define main retained_legacy_main
#include "retained_tb.cpp"
#undef main
#include <array>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <limits>

static constexpr uint64_t SPM_PA=0x100000000ULL, SPM_IOVA=0x30000000ULL;
static constexpr uint64_t HOST_PA=0x80000000ULL, HOST_IOVA=0x10000000ULL;
static constexpr uint64_t BG_SRC=0x90000000ULL, BG_DST=0x90010000ULL;
static constexpr uint32_t SLOT_BYTES=16384, MATRIX_BYTES=262144, SPM_BYTES=98304;
static void require(bool ok,const std::string& msg) { if(!ok) throw TestFailure("stream: "+msg); }
static uint32_t source_a(unsigned row,unsigned k,bool dense) {
    const uint32_t n=row*256u+k;
    return dense ? ((n+1u)*0x9e3779b9u)^0xa53c81e7u : n;
}
static uint32_t source_b(unsigned row,unsigned k,bool dense) {
    const uint32_t n=row*256u+k;
    return dense ? ((n+7u)*0x85ebca6bu)^0x3c6ef372u : (row==k?2u:0u);
}
struct Job {
    char kind; unsigned index,slot,gen; uint64_t src,dst;
    long ready=-1,submitted=-1,issue=-1,done=-1; uint64_t regid=0;
};
struct Tile { long ready=-1,command=-1,done=-1; uint64_t macs=0,reads=0,writes=0; };
struct Burst { unsigned id,len,size,beats=0; uint64_t addr,bytes=0; long cycle; };
struct Hold {
    bool held=false; std::array<uint64_t,6> fields{}; uint64_t stalls=0;
    void check(bool valid,bool ready,std::array<uint64_t,6> f,const char* name) {
        if(held) require(valid && fields==f,std::string(name)+" VALID/payload changed under backpressure");
        held=valid&&!ready; fields=f; if(held) ++stalls;
    }
};
struct Bus {
    std::deque<Burst> aw;
    std::map<unsigned,std::deque<Burst>> ar;
    std::map<unsigned,std::deque<Burst>> b;
    std::array<Hold,5> hold;
    uint64_t read_bytes=0,write_bytes=0,read_bursts=0,write_bursts=0,rreturns=0,breturns=0;
    uint64_t ch_rd[2]{},ch_wr[2]{},ds_reads=0,ds_latency=0,ds_max=0;
    bool empty() const {
        if(!aw.empty()) return false;
        for(const auto& x:ar) if(!x.second.empty()) return false;
        for(const auto& x:b) if(!x.second.empty()) return false;
        return true;
    }
};
struct Stream {
    Tb* tb=nullptr; bool armed=false,dense=false,background=false,startup=false,large=false;
    unsigned tile_target=256,bg_target=0,next_tile=0,bg_issues=0,bg_dones=0;
    long t0=0; int active_job=-1,active_tile=-1; bool command_accepted=false,bg_started=false;
    std::vector<Job> jobs; std::vector<Tile> tiles;
    std::vector<long> bg_issue,bg_done;
    std::array<int,16> aj{},cj{}; std::array<int,256> bj{};
    std::vector<uint8_t> shadow=std::vector<uint8_t>(SPM_BYTES,0);
    std::ofstream events,memtrace,mactrace;
    Bus ddr,spm; uint64_t mem_grants[2]{},mem_reads[2]{},mem_writes[2]{},mem_responses[2]{},macs=0;
    uint64_t tl_req=0,tl_resp=0,lookups=0,hits=0,ptw_cycles=0,compute_dma_overlap=0,spm_contended=0;
    uint32_t accumulator=0;
    bool token=false,token_owner=false,token_we=false; uint64_t token_addr=0,token_data=0;
    long token_cycle=-1;
    uint64_t grant_pause_cycles=0;
    std::string case_id,outdir;
    long now() const {return tb->g_cycle-t0;}
    void event(const char* e,const char* domain,unsigned id,uint64_t addr,unsigned len,unsigned size,uint64_t value,uint64_t aux=0) {
        events<<now()<<','<<e<<','<<domain<<','<<id<<','<<addr<<','<<len<<','<<size<<','<<value<<','<<aux<<'\n';
    }
    void build_jobs() {
        aj.fill(-1);bj.fill(-1);cj.fill(-1); tiles.resize(tile_target);
        unsigned stripes=(tile_target+15)/16;
        for(unsigned s=0;s<stripes;++s) {
            aj[s]=jobs.size(); jobs.push_back({'A',s,s%2,s/2,HOST_IOVA+s*SLOT_BYTES,SPM_IOVA+(s%2)*SLOT_BYTES});
        }
        for(unsigned q=0;q<tile_target;++q) {
            bj[q]=jobs.size(); jobs.push_back({'B',q,q%2,q/2,HOST_IOVA+MATRIX_BYTES+(q%16)*SLOT_BYTES,SPM_IOVA+(2+q%2)*SLOT_BYTES});
        }
        for(unsigned s=0;s<stripes;++s) {
            cj[s]=jobs.size(); jobs.push_back({'C',s,s%2,s/2,SPM_IOVA+(4+s%2)*SLOT_BYTES,HOST_IOVA+2*MATRIX_BYTES+s*SLOT_BYTES});
        }
        for(auto& j:jobs) if((j.kind=='A'&&j.index<2)||(j.kind=='B'&&j.index<2)) j.ready=0;
    }
    void update_ready() {
        for(auto& j:jobs) if(j.ready<0) {
            long rel=-1;
            if(j.kind=='A') rel=tiles[(j.index-2)*16+15].done;
            else if(j.kind=='B') rel=tiles[j.index-2].done;
            else rel=tiles[std::min((j.index+1)*16,tile_target)-1].done;
            if(rel>=0) j.ready=rel;
        }
        if(next_tile<tile_target) {
            unsigned q=next_tile,s=q/16; Tile& t=tiles[q];
            bool prior=(q==0 || tiles[q-1].done>=0);
            bool cfree=(s<2 || jobs[cj[s-2]].done>=0);
            if(t.ready<0 && prior && cfree && jobs[aj[s]].done>=0 && jobs[bj[q]].done>=0) {
                t.ready=std::max(jobs[aj[s]].done,jobs[bj[q]].done);
                if(q) t.ready=std::max(t.ready,tiles[q-1].done);
                if(s>=2) t.ready=std::max(t.ready,jobs[cj[s-2]].done);
            }
        }
    }
    void bus(Bus& b,const char* name,bool av,bool ar,unsigned ai,uint64_t aa,unsigned al,unsigned az,
             bool wv,bool wr,uint64_t wd,unsigned ws,bool wl,bool bv,bool br,unsigned bi,unsigned bp,
             bool rv,bool rr,unsigned ri,uint64_t rd,bool rl,unsigned rp,
             bool qv,bool qr,unsigned qi,uint64_t qa,unsigned ql,unsigned qz) {
        b.hold[0].check(av,ar,{ai,aa,al,az,0,0},"AXI AW");
        b.hold[1].check(wv,wr,{wd,ws,wl,0,0,0},"AXI W");
        b.hold[2].check(bv,br,{bi,bp,0,0,0,0},"AXI B");
        b.hold[3].check(qv,qr,{qi,qa,ql,qz,0,0},"AXI AR");
        b.hold[4].check(rv,rr,{ri,rd,rl,rp,0,0},"AXI R");
        if(av&&ar) {require(az==3,"non64bit AXI AW");b.aw.push_back({ai,al,az,0,aa,0,now()});++b.write_bursts;
            event("AW",name,ai,aa,al,az,0);}
        if(wv&&wr) {
            require(!b.aw.empty(),"AXI W without AW"); auto& x=b.aw.front();
            require(wl==(x.beats==x.len),"AXI WLAST/length mismatch");
            x.bytes+=__builtin_popcount(ws);++x.beats;
            if(wl) {require(x.bytes==(x.len+1u)*8u,"partial DMA write");b.write_bytes+=x.bytes;
                if(((x.id>>4)&3)==2) b.ch_wr[(x.id>>2)&1]+=x.bytes;
                event("WLAST",name,x.id,x.addr,x.len,x.size,x.bytes); b.b[x.id].push_back(x);b.aw.pop_front();}
        }
        if(bv&&br) {require(bp==0,"AXI B error"); require(!b.b[bi].empty(),"B without completed W burst");
            auto x=b.b[bi].front();b.b[bi].pop_front();++b.breturns; event("B",name,bi,x.addr,x.len,x.size,0,bp);}
        if(qv&&qr) {require(qz==3,"non64bit AXI AR");b.ar[qi].push_back({qi,ql,qz,0,qa,0,now()});++b.read_bursts;
            auto bytes=(ql+1u)*8u;b.read_bytes+=bytes;
            if(((qi>>4)&3)==2) b.ch_rd[(qi>>2)&1]+=bytes;
            if(((qi>>4)&3)==3) ++b.ds_reads;
            event("AR",name,qi,qa,ql,qz,0);}
        if(rv&&rr) {require(rp==0,"AXI R error");require(!b.ar[ri].empty(),"R without AR");auto& x=b.ar[ri].front();
            require(rl==(x.beats==x.len),"AXI RLAST/length mismatch");++x.beats;
            if(rl) {++b.rreturns;auto wait=now()-x.cycle;
                if(((ri>>4)&3)==3){b.ds_latency+=wait;b.ds_max=std::max(b.ds_max,(uint64_t)wait);}
                event("RLAST",name,ri,x.addr,x.len,x.size,x.beats,rp); b.ar[ri].pop_front();}
        }
    }
    void observe() {
        auto* p=tb->top; require(p->comp_fault_o==0,"compute fault="+std::to_string(p->comp_fault_o));
        require(now()<=300000000,"total cycle budget exhausted");
        // The previous grant must respond exactly one edge later, including writes.
        require(bool(p->mem_rsp_valid_o)==token,"SPM response missing/spurious");
        if(token) {
            require(now()==token_cycle+1,"SPM latency is not one clock");
            require(p->mem_rsp_owner_o==token_owner && p->mem_rsp_we_o==token_we && p->mem_rsp_addr_o==token_addr,"SPM response owner/address mismatch");
            require(!p->mem_rsp_err_o,"unexpected SPM error");
            if(!token_we) require(p->mem_rsp_rdata_o==token_data,"SPM data mismatch");
            ++mem_responses[token_owner];
            if(dense) memtrace<<now()<<",response,"<<token_owner<<','<<token_we<<','<<token_addr<<','<<uint64_t(p->mem_rsp_rdata_o)<<",0,"<<unsigned(p->mem_rsp_err_o)<<'\n';
        }
        token=false;
        if(p->mem_grant_valid_o) {
            uint64_t a=p->mem_grant_addr_o; require(a>=SPM_PA && a+8<=SPM_PA+SPM_BYTES && !(a&7),"SPM grant range/alignment");
            unsigned off=a-SPM_PA,owner=p->mem_grant_owner_o; uint64_t data=0;
            for(unsigned k=0;k<8;++k)data|=uint64_t(shadow[off+k])<<(8*k);
            token=true;token_owner=owner;token_we=p->mem_grant_we_o;token_addr=a;token_data=data;token_cycle=now();
            ++mem_grants[owner];
            if(p->mem_grant_we_o) {++mem_writes[owner]; for(unsigned k=0;k<8;++k) if(p->mem_grant_strb_o&(1u<<k))shadow[off+k]=(p->mem_grant_wdata_o>>(8*k))&255;}
            else ++mem_reads[owner];
            if(owner) {require(active_tile>=0,"compute memory grant without active tile");auto& t=tiles[active_tile];
                if(p->mem_grant_we_o) ++t.writes;else ++t.reads;}
            if(dense) memtrace<<now()<<",grant,"<<owner<<','<<unsigned(p->mem_grant_we_o)<<','<<a<<','<<uint64_t(p->mem_grant_wdata_o)<<','<<unsigned(p->mem_grant_strb_o)<<",0\n";
        }
        if(p->spm_pause_i) {++grant_pause_cycles;require(!p->mem_grant_valid_o,"grant during integrity pause");}
        if(p->comp_cmd_valid_i&&p->comp_cmd_ready_o) {
            require(active_tile<0&&next_tile<tile_target,"duplicate compute command");active_tile=next_tile;
            auto& t=tiles[active_tile]; require(t.ready>=0&&t.ready<=now(),"compute before dependencies ready");t.command=now();
            unsigned q=active_tile,s=q/16;
            require(p->comp_s_i==s&&p->comp_t_i==q%16&&p->comp_aslot_i==s%2&&p->comp_bslot_i==q%2&&p->comp_cslot_i==s%2,"compute command identity");
            require(p->comp_agen_i==s/2&&p->comp_bgen_i==q/2&&p->comp_cgen_i==s/2,"compute slot generation");
            accumulator=0;command_accepted=true; event("COMMAND","compute",q,0,0,0,0);
        }
        if(p->mac_valid_o) {
            require(active_tile>=0,"MAC without command");auto& t=tiles[active_tile];unsigned n=t.macs;
            unsigned i=n/4096,j=(n/256)%16,k=n%256,s=active_tile/16,tilecol=active_tile%16;
            require(n<65536&&p->mac_s_o==s&&p->mac_t_o==tilecol&&p->mac_i_o==i&&p->mac_j_o==j&&p->mac_k_o==k,"MAC scalar order");
            uint32_t a=source_a(s*16+i,k,dense),b=source_b(tilecol*16+j,k,dense),product=uint32_t(uint64_t(a)*b);
            if(k==0) accumulator=0; accumulator+=product;
            require(p->mac_a_o==a&&p->mac_b_o==b&&p->mac_product_o==product&&p->mac_acc_o==accumulator,"MAC data/uint32 accumulator");
            ++t.macs;++macs;
            if(dense)mactrace<<now()<<','<<s<<','<<tilecol<<','<<i<<','<<j<<','<<k<<','<<a<<','<<b<<','<<product<<','<<accumulator<<'\n';
        }
        if(p->comp_done_o) {
            require(active_tile>=0,"duplicate compute done");auto& t=tiles[active_tile];
            require(p->comp_done_s_o==unsigned(active_tile/16)&&p->comp_done_t_o==unsigned(active_tile%16),"compute done tag");
            require(t.macs==65536&&t.reads==131072&&t.writes==256,"compute operation budget mismatch");
            require(!(token&&token_owner),"compute done before its last response drains");
            t.done=now(); event("TILE_DONE","compute",active_tile,0,0,0,t.macs);active_tile=-1;++next_tile;
        }
        // Completion first permits the next autonomous issue on the same edge.
        if(p->dbg_dma_done&1) {require(active_job>=0,"V done without submitted job");auto& j=jobs[active_job];
            require(j.issue>=0&&j.done<0,"V completion count/order");j.done=now(); event("COPY_DONE","V",active_job,j.dst,0,0,SLOT_BYTES);}
        if(p->dbg_dma_done&2) {require(bg_dones<bg_issues,"BG duplicate completion");bg_done.push_back(now());++bg_dones;event("COPY_DONE","BG",bg_dones-1,0,0,0,4096);}
        if(p->dbg_dma_issue&1) {require(active_job>=0,"V issue without programmed job");auto& j=jobs[active_job];require(j.issue<0&&j.submitted>=0&&j.ready<=now(),"V issue order");j.issue=now();event("COPY_ISSUE","V",active_job,j.src,0,0,SLOT_BYTES);}
        if(p->dbg_dma_issue&2) {require(bg_started&&bg_issues<bg_target&&bg_issues==bg_dones,"BG issue order/budget");bg_issue.push_back(now());++bg_issues;event("COPY_ISSUE","BG",bg_issues-1,0,0,0,4096);}
        bus(ddr,"DDR",p->ddr_aw_valid,p->ddr_aw_ready,p->ddr_aw_id,p->ddr_aw_addr,p->ddr_aw_len,p->ddr_aw_size,
            p->ddr_w_valid,p->ddr_w_ready,p->ddr_w_data,p->ddr_w_strb,p->ddr_w_last,p->ddr_b_valid,p->ddr_b_ready,p->ddr_b_id,p->ddr_b_resp,
            p->ddr_r_valid,p->ddr_r_ready,p->ddr_r_id,p->ddr_r_data,p->ddr_r_last,p->ddr_r_resp,p->ddr_ar_valid,p->ddr_ar_ready,p->ddr_ar_id,p->ddr_ar_addr,p->ddr_ar_len,p->ddr_ar_size);
        bus(spm,"SPM",p->spm_aw_valid_o,p->spm_aw_ready_o,p->spm_aw_id_o,p->spm_aw_addr_o,p->spm_aw_len_o,p->spm_aw_size_o,
            p->spm_w_valid_o,p->spm_w_ready_o,p->spm_w_data_o,p->spm_w_strb_o,p->spm_w_last_o,p->spm_b_valid_o,p->spm_b_ready_o,p->spm_b_id_o,p->spm_b_resp_o,
            p->spm_r_valid_o,p->spm_r_ready_o,p->spm_r_id_o,p->spm_r_data_o,p->spm_r_last_o,p->spm_r_resp_o,p->spm_ar_valid_o,p->spm_ar_ready_o,p->spm_ar_id_o,p->spm_ar_addr_o,p->spm_ar_len_o,p->spm_ar_size_o);
        if(p->dbg_tl_req_valid&&p->dbg_tl_req_ready){++tl_req;event("TL_REQ","TL",p->dbg_main_bank,0,0,0,0);}
        if(p->dbg_tl_resp_valid&&p->dbg_tl_resp_ready){++tl_resp;event("TL_RESP","TL",p->dbg_main_bank,0,0,0,0);}
        if(p->dbg_tl_iotlb_lookup){++lookups;if(p->dbg_tl_iotlb_hit)++hits;event("LOOKUP","TL",p->dbg_main_bank,0,0,0,p->dbg_tl_iotlb_hit);}
        if(p->dbg_tl_ptw_active)++ptw_cycles;
        if(p->dbg_ptw_ar_fire)event("PTW_AR","TL",p->dbg_main_bank,0,0,0,0);
        if(p->dbg_iotlb_fill)event("FILL","TL",p->dbg_main_bank,0,0,0,0);
        if(p->comp_busy_o&&active_job>=0&&jobs[active_job].issue>=0&&jobs[active_job].done<0)++compute_dma_overlap;
        if(p->comp_busy_o&&p->spm_busy_o)++spm_contended; // Occupancy overlap, not an arbiter stall attribution.
        update_ready();
    }
};
static Stream* stream=nullptr;
static void stream_drive(Tb& tb) {
    if(stream&&stream->armed&&stream->tb==&tb) tb.top->spm_pause_i=stream->dense && ((stream->now()%17)==3 || (stream->now()%17)==4);
}
static void stream_preedge(Tb& tb) {if(stream&&stream->armed&&stream->tb==&tb)stream->observe();}

static void initialize_memory(Tb& tb,bool large,bool dense) {
    tb.setup_memory(16);
    for(uint64_t base:{0x20000ULL,0x30000ULL,0x40000ULL,0x50000ULL,0x60000ULL,0x70000ULL,0x80000ULL})
        for(unsigned i=0;i<512;++i)tb.write64(base+i*8,0);
    auto pointer=[](uint64_t pa){return ((pa>>12)<<10)|1ULL;};
    auto leaf=[](uint64_t pa){return ((pa>>12)<<10)|0xdfULL;};
    tb.write64(0x20000,pointer(0x40000));tb.write64(0x30000,pointer(0x60000));
    if(large) {
        tb.write64(0x40400,leaf(HOST_PA));tb.write64(0x40c00,leaf(SPM_PA));tb.write64(0x60800,leaf(BG_SRC));
    } else {
        tb.write64(0x40400,pointer(0x50000));tb.write64(0x40c00,pointer(0x80000));tb.write64(0x60800,pointer(0x70000));
        for(unsigned i=0;i<192;++i)tb.write64(0x50000+i*8,leaf(HOST_PA+4096ULL*i));
        for(unsigned i=0;i<24;++i)tb.write64(0x80000+i*8,leaf(SPM_PA+4096ULL*i));
        for(unsigned i=0;i<32;++i)tb.write64(0x70000+i*8,leaf(BG_SRC+4096ULL*i));
    }
    for(unsigned n=0;n<65536;n+=2) {
        uint64_t a=source_a(n/256,n%256,dense)|(uint64_t(source_a((n+1)/256,(n+1)%256,dense))<<32);
        uint64_t b=source_b(n/256,n%256,dense)|(uint64_t(source_b((n+1)/256,(n+1)%256,dense))<<32);
        tb.write64(HOST_PA+n*4,a);tb.write64(HOST_PA+MATRIX_BYTES+n*4,b);
        tb.write64(HOST_PA+2*MATRIX_BYTES+n*4,0xdeadc0debad0beefULL);
    }
    for(unsigned p=0;p<16;++p)for(unsigned i=0;i<512;++i) {
        tb.write64(BG_SRC+p*4096+i*8,page_word(p+1,i));tb.write64(BG_DST+p*4096+i*8,page_old(p+1,i));
    }
}
static uint32_t read32(Tb& tb,uint64_t addr) {return uint32_t(tb.read64(addr&~7ULL)>>(32*((addr>>2)&1)));}
static void submit_job(Stream& s,int idx) {
    auto& tb=*s.tb;auto& j=s.jobs[idx];s.active_job=idx;j.submitted=s.now();s.event("SUBMIT","V",idx,j.src,0,0,SLOT_BYTES);
    tb.cfg_wr(SEL_V,REG_CONF,0);tb.cfg_wr(SEL_V,REG_SRC,j.src);tb.cfg_wr(SEL_V,REG_DST,j.dst);tb.cfg_wr(SEL_V,REG_NB,SLOT_BYTES);
    j.regid=tb.trigger(SEL_V);require(j.issue>=0,"NEXT_ID returned before hardware issue");
}
static void command_tile(Stream& s) {
    auto& tb=*s.tb; auto* p=tb.top; unsigned q=s.next_tile,row=q/16;
    p->comp_s_i=row;p->comp_t_i=q%16;p->comp_aslot_i=row%2;p->comp_bslot_i=q%2;p->comp_cslot_i=row%2;
    p->comp_agen_i=row/2;p->comp_bgen_i=q/2;p->comp_cgen_i=row/2;p->comp_cmd_valid_i=1;s.command_accepted=false;
    unsigned wait=0;while(!s.command_accepted){tb.tick();require(++wait<200000,"compute command not accepted");}
    p->comp_cmd_valid_i=0;
}
static int choose_job(Stream& s) {
    int best=-1; unsigned use=~0u;
    for(unsigned n=0;n<s.jobs.size();++n) {
        auto& j=s.jobs[n];if(j.ready<0||j.submitted>=0)continue;
        if(j.kind=='C') return n;
        unsigned u=j.kind=='A'?j.index*16:j.index;
        if(u<use||(u==use&&j.kind=='A')){best=n;use=u;}
    }
    return best;
}
static void save_result(Stream& s) {
    auto& tb=*s.tb;
    std::ofstream jobs(s.outdir+"/jobs.csv");jobs<<"job,kind,index,slot,generation,src,dst,bytes,ready,submitted,issue,done,register_id\n";
    for(unsigned n=0;n<s.jobs.size();++n){auto& j=s.jobs[n];jobs<<n<<','<<j.kind<<','<<j.index<<','<<j.slot<<','<<j.gen<<','<<j.src<<','<<j.dst<<','<<SLOT_BYTES<<','<<j.ready<<','<<j.submitted<<','<<j.issue<<','<<j.done<<','<<j.regid<<'\n';}
    std::ofstream tiles(s.outdir+"/tiles.csv");tiles<<"q,s,t,ready,command,done,macs,reads,writes\n";
    for(unsigned q=0;q<s.tiles.size();++q){auto& t=s.tiles[q];tiles<<q<<','<<q/16<<','<<q%16<<','<<t.ready<<','<<t.command<<','<<t.done<<','<<t.macs<<','<<t.reads<<','<<t.writes<<'\n';}
    std::ofstream bg(s.outdir+"/background.csv");bg<<"index,ready,issue,done,bytes\n";
    for(unsigned q=0;q<s.bg_target;++q)bg<<q<<",0,"<<s.bg_issue[q]<<','<<s.bg_done[q]<<",4096\n";
    std::ofstream output(s.outdir+"/output.u32le",std::ios::binary);
    for(unsigned n=0;n<65536;++n) {
        uint32_t val=read32(tb,HOST_PA+2*MATRIX_BYTES+n*4),expect=0;
        if(!s.dense) expect=2u*n;
        else if(n/256<16 && n%256<32)for(unsigned k=0;k<256;++k)expect+=uint32_t(uint64_t(source_a(n/256,k,true))*source_b(n%256,k,true));
        else if(n/256>=16)expect=uint32_t(0xdeadc0debad0beefULL>>(32*(n&1))); // only first C stripe transferred in integrity case
        require(val==expect,"final C mismatch at "+std::to_string(n));
        for(unsigned k=0;k<4;++k)output.put((val>>(k*8))&255);
        require(read32(tb,HOST_PA+n*4)==source_a(n/256,n%256,s.dense),"A source modified");
        require(read32(tb,HOST_PA+MATRIX_BYTES+n*4)==source_b(n/256,n%256,s.dense),"B source modified");
    }
    if(s.background)for(unsigned p=0;p<16;++p)for(unsigned n=0;n<512;++n) {
        require(tb.read64(BG_DST+p*4096+n*8)==page_word(p+1,n),"BG data mismatch");
        require(tb.read64(BG_SRC+p*4096+n*8)==page_word(p+1,n),"BG source modified");
    }
    uint64_t vbytes=uint64_t(s.jobs.size())*SLOT_BYTES, bbytes=uint64_t(s.bg_target)*4096;
    require(s.ddr.ch_rd[0]+s.spm.ch_rd[0]==vbytes&&s.ddr.ch_wr[0]+s.spm.ch_wr[0]==vbytes,"V read/write byte conservation");
    require(s.ddr.ch_rd[1]==bbytes&&s.ddr.ch_wr[1]==bbytes,"BG read/write byte conservation");
    require(s.ddr.read_bursts==s.ddr.rreturns&&s.ddr.write_bursts==s.ddr.breturns&&s.spm.read_bursts==s.spm.rreturns&&s.spm.write_bursts==s.spm.breturns,"AXI response conservation");
    require(s.tl_req==s.tl_resp,"translation request/response mismatch");
    require(s.mem_grants[0]==s.mem_responses[0]&&s.mem_grants[1]==s.mem_responses[1],"SPM response conservation");
    long vfinal=0;for(auto& j:s.jobs){require(j.ready>=0&&j.ready<=j.submitted&&j.submitted<=j.issue&&j.issue<=j.done,"job timeline order");vfinal=std::max(vfinal,j.done);}
    long afinal=s.bg_done.empty()?0:s.bg_done.back();unsigned bgatv=std::upper_bound(s.bg_done.begin(),s.bg_done.end(),vfinal)-s.bg_done.begin();
    std::ofstream out(s.outdir+"/summary.json");
    out<<"{\n\"schema\":\"stream-g0-result-v1\",\"case\":\""<<s.case_id<<"\",\"pass\":true,\"model_only\":true,\n"
       <<"\"mapping\":\""<<(s.large?"2m":"4k")<<"\",\"dense_integrity\":"<<(s.dense?"true":"false")<<",\"startup\":"<<(s.startup?"true":"false")<<",\n"
       <<"\"t0_absolute\":"<<s.t0<<",\"V_final\":"<<vfinal<<",\"BG_final\":"<<afinal<<",\"quiet\":"<<s.now()<<",\"BG_done_at_V\":"<<bgatv<<",\n"
       <<"\"tiles\":"<<s.tile_target<<",\"V_copies\":"<<s.jobs.size()<<",\"V_bytes\":"<<vbytes<<",\"BG_copies\":"<<s.bg_target<<",\"BG_bytes\":"<<bbytes<<",\"MACs\":"<<s.macs<<",\n"
       <<"\"SPM_bytes\":98304,\"SPM_ports\":1,\"multipliers\":1,\"SPM_latency\":1,\"authorized_bytes\":"<<(s.large?6291456:1015808)<<",\n"
       <<"\"TL_req\":"<<s.tl_req<<",\"TL_resp\":"<<s.tl_resp<<",\"lookups\":"<<s.lookups<<",\"hits\":"<<s.hits<<",\"PTW_active_cycles\":"<<s.ptw_cycles<<",\n"
       <<"\"DS_read_bursts\":"<<s.ddr.ds_reads<<",\"DS_latency_sum\":"<<s.ddr.ds_latency<<",\"DS_latency_max\":"<<s.ddr.ds_max<<",\n"
       <<"\"compute_V_DMA_overlap_cycles\":"<<s.compute_dma_overlap<<",\"compute_SPM_occupancy_overlap_cycles\":"<<s.spm_contended<<",\"integrity_pause_cycles\":"<<s.grant_pause_cycles<<",\n"
       <<"\"memory_grants\":["<<s.mem_grants[0]<<','<<s.mem_grants[1]<<"],\"memory_responses\":["<<s.mem_responses[0]<<','<<s.mem_responses[1]<<"],\n"
       <<"\"memory_reads\":["<<s.mem_reads[0]<<','<<s.mem_reads[1]<<"],\"memory_writes\":["<<s.mem_writes[0]<<','<<s.mem_writes[1]<<"],\n"
       <<"\"DDR_stall_cycles\":[";for(unsigned k=0;k<5;++k)out<<(k?",":"")<<s.ddr.hold[k].stalls;
    out<<"],\"SPM_stall_cycles\":[";for(unsigned k=0;k<5;++k)out<<(k?",":"")<<s.spm.hold[k].stalls;
    out<<"],\"quiet_checked\":true,\"final_data_values_checked\":65536,\"source_immutable_checked\":true,\"STOP_used\":false,\"runtime_every_MAC_and_memory_response_checked\":true}\n";
    std::printf("PASS case=%s V=%ld BG=%ld quiet=%ld MACs=%llu copies=%zu BG-at-V=%u\n",s.case_id.c_str(),vfinal,afinal,s.now(),(unsigned long long)s.macs,s.jobs.size(),bgatv);
}
int main(int argc,char** argv) {
    try {
        require(argc==3,"usage: stream_model CASE OUTPUT_DIR");
        Verilated::commandArgs(argc,argv); Stream s; s.case_id=argv[1];s.outdir=argv[2];
        s.dense=s.case_id=="integrity";s.tile_target=s.dense?2:256;
        s.background=s.dense||s.case_id!="solo-p-4k";
        s.startup=s.case_id=="shared-p-4k-startup";s.large=s.case_id=="shared-p-2m";
        require(s.dense||s.case_id=="solo-p-4k"||s.case_id=="shared-p-4k"||s.startup||s.large,"unknown/frozen case");
        s.bg_target=s.background?(s.dense?64:2048):0;
        std::filesystem::create_directories(s.outdir);
        s.events.open(s.outdir+"/events.csv");s.events<<"cycle,event,domain,id,addr,len,size,value,aux\n";
        if(s.dense){s.memtrace.open(s.outdir+"/memory.csv");s.memtrace<<"cycle,event,owner,we,addr,data,strb,error\n";s.mactrace.open(s.outdir+"/mac.csv");s.mactrace<<"cycle,s,t,i,j,k,a,b,product,acc\n";}
        Tb tb; s.tb=&tb;stream=&s;
        tb.top->comp_cmd_valid_i=0;tb.top->spm_pause_i=0;tb.top->comp_s_i=0;tb.top->comp_t_i=0;
        tb.top->comp_aslot_i=0;tb.top->comp_bslot_i=0;tb.top->comp_cslot_i=0;
        tb.top->comp_agen_i=0;tb.top->comp_bgen_i=0;tb.top->comp_cgen_i=0;
        tb.cpu_enable=false;tb.policy_partition=true;tb.owner_only=false;tb.read_admit_mode_val=0;
        tb.ptw_latency=31;tb.warmup=0;tb.a_target=s.bg_target;tb.v_transfers=s.jobs.size();
        initialize_memory(tb,s.large,s.dense);tb.reset(false);
        s.build_jobs();s.t0=tb.g_cycle;tb.scenario_start=s.t0;tb.scenario_budget=300000000;s.armed=true;
        if(s.background) {
            tb.configure_batch(SEL_A,A_IOVA_BASE,A_IOVA_BASE+65536,s.bg_target,16,0);
            if(!s.startup){s.bg_started=true;tb.start_batch(SEL_A);}
        }
        while(true) {
            if(s.active_job>=0&&s.jobs[s.active_job].done>=0) {
                require(tb.cfg_rd(SEL_V,REG_DONE)==s.jobs[s.active_job].regid,"V DONE/NEXT_ID register mismatch");s.active_job=-1;
            }
            s.update_ready();
            if(s.active_tile<0&&s.next_tile<s.tile_target&&s.tiles[s.next_tile].ready>=0) {
                command_tile(s);
                if(s.startup&&!s.bg_started){s.bg_started=true;tb.start_batch(SEL_A);}
            }
            if(s.active_job<0) {int j=choose_job(s);if(j>=0)submit_job(s,j);}
            bool all=true;for(auto& j:s.jobs) if(j.done<0)all=false;
            if(all&&s.active_job<0&&s.next_tile==s.tile_target&&s.bg_dones==s.bg_target)break;
            if(s.active_job>=0&&s.jobs[s.active_job].issue>=0&&s.jobs[s.active_job].done<0)
                if(s.now()-s.jobs[s.active_job].issue>=200000) {tb.dump_stall("stream-V");std::fprintf(stderr,"STREAM state cycle=%ld job=%d kind=%c spmAR=%u/%u spmR=%u/%u last=%u spmB=%u/%u memreqgrant=%u rsp=%u DDRW=%u/%u memoryGrants=%llu/%llu SPMreads=%llu Rlast=%llu\n",s.now(),s.active_job,s.jobs[s.active_job].kind,(unsigned)tb.top->spm_ar_valid_o,(unsigned)tb.top->spm_ar_ready_o,(unsigned)tb.top->spm_r_valid_o,(unsigned)tb.top->spm_r_ready_o,(unsigned)tb.top->spm_r_last_o,(unsigned)tb.top->spm_b_valid_o,(unsigned)tb.top->spm_b_ready_o,(unsigned)tb.top->mem_grant_valid_o,(unsigned)tb.top->mem_rsp_valid_o,(unsigned)tb.top->ddr_w_valid,(unsigned)tb.top->ddr_w_ready,(unsigned long long)s.mem_grants[0],(unsigned long long)s.mem_grants[1],(unsigned long long)s.spm.read_bursts,(unsigned long long)s.spm.rreturns);
#include "diagnostic-flat.inc"
throw TestFailure("active V transfer watchdog");}
            if(s.bg_issues>s.bg_dones)require(s.now()-s.bg_issue.back()<200000,"active BG transfer watchdog");
            tb.tick();
        }
        if(s.background){bool er,co,st,ac,en;tb.read_batch_ctl(SEL_A,er,co,st,ac,en);require(!er&&co&&!st&&!ac,"background did not naturally finish");
            require(tb.cfg_rd(SEL_A,REG_B_DONE)==s.bg_target&&tb.cfg_rd(SEL_A,REG_B_ISSUED)==s.bg_target,"BG batch register totals");}
        tb.check_quiet("stream-final");
        require(!tb.top->comp_busy_o&&!tb.top->spm_busy_o&&!s.token&&s.ddr.empty()&&s.spm.empty(),"compute/SPM/AXI not fully quiet");
        require(s.bg_issues==s.bg_target&&s.bg_dones==s.bg_target,"background tail incomplete");
        s.event("QUIET","SYSTEM",tb.top->dbg_dma_job_quiet,0,0,0,tb.top->dbg_in_flight,
            unsigned(tb.top->spm_busy_o)|(unsigned(tb.top->comp_busy_o)<<1));
        save_result(s);s.armed=false;stream=nullptr;return 0;
    } catch(const std::exception& e) {std::fprintf(stderr,"FAIL %s\n",e.what());stream=nullptr;return 1;}
}
