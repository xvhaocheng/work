// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Private stage106 transfer adapter. The retained DDR/Tb implementation is unchanged.
#define main retained_diagnostic_main
#include "retained_tb.cpp"
#undef main
#include <array>
#include <fstream>
#include <sstream>
#include <set>
#include <limits>

namespace mandatory_transfer {
constexpr uint64_t SLOT = 10ULL * 1024 * 1024;
constexpr uint64_t BACKING = 48ULL * 1024 * 1024;
constexpr uint64_t IOVA = 0x40000000ULL, PA = 0x84000000ULL;
constexpr uint64_t USEFUL = 9447936, EXTENT = 9457664;
constexpr long NO_PROGRESS = 200000;
const uint64_t tensor_offset[6] = {0,4718592,4726784,9445376,9449472,9453568};
const uint64_t tensor_bytes[6] = {4718592,6144,4718592,1536,1536,1536};
const char* tensor_name[6] = {"fc1.weight","fc1.bias","fc2.weight","fc2.bias",
                            "final_layer_norm.weight","final_layer_norm.bias"};

struct Case {
    std::string id, dispatch;
    uint64_t page_bytes=0;
    bool offered[2]={false,false};
    bool single=false;
    uint64_t useful() const { return USEFUL * (unsigned(offered[0])+unsigned(offered[1])); }
};
struct Stats {
    uint64_t issue=0,done=0,ar=0,aw=0,r=0,w=0,b=0;
    uint64_t ar_bytes=0,r_bytes=0,aw_bytes=0,w_bytes=0,errors=0;
    uint64_t tl_accept=0,tl_resp=0;
};
struct Config {
    uint64_t writes=0,reads=0,write_cycles=0,read_cycles=0,next_reads=0,done_reads=0;
};
struct Job {
    uint64_t id=0,src=0,dst=0,bytes=0;
    unsigned role=0,channel=0,tensor=0;
    int64_t hw_id=-1,done_id=-1;
    long ready=0,channel_ready=0,config_start=-1,config_program_end=-1;
    long issue=-1,done=-1,next_cycle=-1,done_reg_cycle=-1;
    Config cfg;
    Stats bus;
};
struct Burst {
    uint64_t addr=0;
    uint32_t id=0,beats=0,got=0;
    int channel=-1;
    Job* job=nullptr;
};
struct Held {
    bool held=false;
    std::array<uint64_t,8> data{};
    void check(bool valid,bool ready,std::array<uint64_t,8> now) {
        CHECK(!held || (valid && now==data),"DDR VALID/payload changed while held");
        held=valid && !ready;
        if(held) data=now;
    }
};
static uint64_t to_pa(uint64_t a) {
    CHECK(a>=IOVA && a<IOVA+4*SLOT,"IOVA outside declared four slots");
    return PA+(a-IOVA);
}
static uint8_t& dense(Tb& tb,uint64_t offset) {
    CHECK(offset<BACKING,"dense backing offset outside reserve");
    return tb.transfer_dense[offset/(16ULL*1024*1024)][offset%(16ULL*1024*1024)];
}
static unsigned pop8(unsigned x) {
    unsigned n=0;for(unsigned i=0;i<8;++i)n+=(x>>i)&1u;return n;
}
static void add(Stats& a,const Stats& b) {
    a.issue+=b.issue;a.done+=b.done;a.ar+=b.ar;a.aw+=b.aw;a.r+=b.r;a.w+=b.w;a.b+=b.b;
    a.ar_bytes+=b.ar_bytes;a.r_bytes+=b.r_bytes;a.aw_bytes+=b.aw_bytes;a.w_bytes+=b.w_bytes;
    a.errors+=b.errors;a.tl_accept+=b.tl_accept;a.tl_resp+=b.tl_resp;
}
static void stat_json(std::ostream& f,const Stats& s) {
    f<<"{\"issue\":"<<s.issue<<",\"done\":"<<s.done<<",\"ar\":"<<s.ar
     <<",\"aw\":"<<s.aw<<",\"r\":"<<s.r<<",\"w\":"<<s.w<<",\"b\":"<<s.b
     <<",\"ar_bytes\":"<<s.ar_bytes<<",\"r_bytes\":"<<s.r_bytes
     <<",\"aw_bytes\":"<<s.aw_bytes<<",\"w_bytes\":"<<s.w_bytes<<",\"errors\":"<<s.errors
     <<",\"tl_accept\":"<<s.tl_accept<<",\"tl_resp\":"<<s.tl_resp<<"}";
}
static void config_json(std::ostream& f,const Config& c) {
    f<<"{\"writes\":"<<c.writes<<",\"reads\":"<<c.reads
     <<",\"write_cycles\":"<<c.write_cycles<<",\"read_cycles\":"<<c.read_cycles
     <<",\"next_reads\":"<<c.next_reads<<",\"done_reads\":"<<c.done_reads<<"}";
}

class Observer {
public:
    Tb& tb;
    std::vector<Job>& jobs;
    const Case& c;
    Stats channels[2],all_ddr;
    // Observer ownership is only a checker; the controller owns a separate running[].
    Job* active[2]={nullptr,nullptr};
    long t0=0,natural_quiet=-1,validation_end=-1,last_true_progress[2]={0,0};
    long role_done[2]={-1,-1},role_done_reg[2]={-1,-1};
    long all_hardware_done=-1;
    uint64_t role_completed[2]={0,0},role_completed_bytes[2]={0,0};
    uint64_t other_bytes_at_role_done[2]={0,0};
    int first_complete_role=-1;
    uint64_t other_bytes_at_first_complete=0;
    uint64_t total_budget=0,stability_samples=0,launch_errors=0;
    uint64_t metadata_ar=0,metadata_ar_bytes=0,metadata_r_bytes=0;
    std::deque<Burst> rq,wq,bq;
    Held arheld,awheld,wheld,rheld,bheld;
    // Debug interface exposes bank but not the complete TL payload. Check only
    // VALID/bank stability here; do not pretend unexposed payload was observed.
    bool tl_req_held=false,tl_resp_held=false;
    unsigned tl_req_bank=0,tl_resp_bank=0;
    std::ofstream events;
    static Observer* current;

    Observer(Tb& t,std::vector<Job>& js,const Case& cs,const std::string& path):
        tb(t),jobs(js),c(cs),total_budget(200000+64*cs.useful()/8),events(path) {
        CHECK(events.good(),"cannot open events.csv");
        events<<"cycle,event,job,role,channel,hw_id,src_iova,dst_iova,bytes,tensor,ready,value,aux\n";
    }
    void event(const char* kind,const Job* j,long cyc,uint64_t value=0,uint64_t aux=0) {
        events<<cyc<<','<<kind<<',';
        if(j)events<<j->id<<','<<j->role<<','<<j->channel<<','<<j->hw_id<<','<<j->src
                   <<','<<j->dst<<','<<j->bytes<<','<<j->tensor<<','<<j->ready;
        else events<<"-1,-1,-1,-1,0,0,0,-1,0";
        events<<','<<value<<','<<aux<<'\n';
        CHECK(events.good(),"events.csv write failed");
    }
    static void hook(Tb& t) {CHECK(current && &current->tb==&t,"observer owner mismatch");current->sample();}
    Job* payload_job(uint32_t id) {
        const unsigned port=(id>>4)&3u;
        if(port!=2){CHECK(port==3,"CPU/debug DDR traffic while CPU disabled");return nullptr;}
        const unsigned ch=(id>>2)&1u;
        CHECK(active[ch],"payload bus traffic without a scheduled job");
        return active[ch];
    }
    bool quiet() const {
        auto* p=tb.top;
        return p->dbg_dma_job_quiet==3 && p->dbg_in_flight==0 && p->dbg_cpu_quiet && p->dbg_cpu_wr_quiet
            && !p->ddr_ar_valid && !p->ddr_aw_valid && !p->ddr_w_valid && !p->ddr_r_valid && !p->ddr_b_valid
            && tb.mem_q.empty() && tb.caw_q.empty() && tb.cb_q.empty() && rq.empty() && wq.empty() && bq.empty();
    }
    void sample() {
        auto* p=tb.top;CHECK(p->rst_ni,"observer invoked during reset");
        const long cyc=tb.g_cycle-t0;bool progress[2]={false,false};
        arheld.check(p->ddr_ar_valid,p->ddr_ar_ready,{p->ddr_ar_id,p->ddr_ar_addr,p->ddr_ar_len,p->ddr_ar_size,p->ddr_ar_burst,p->ddr_ar_prot,p->ddr_ar_user,0});
        awheld.check(p->ddr_aw_valid,p->ddr_aw_ready,{p->ddr_aw_id,p->ddr_aw_addr,p->ddr_aw_len,p->ddr_aw_size,p->ddr_aw_burst,p->ddr_aw_prot,p->ddr_aw_user,0});
        wheld.check(p->ddr_w_valid,p->ddr_w_ready,{p->ddr_w_data,p->ddr_w_strb,p->ddr_w_last,p->ddr_w_user,0,0,0,0});
        rheld.check(p->ddr_r_valid,p->ddr_r_ready,{p->ddr_r_id,p->ddr_r_data,p->ddr_r_resp,p->ddr_r_last,0,0,0,0});
        bheld.check(p->ddr_b_valid,p->ddr_b_ready,{p->ddr_b_id,p->ddr_b_resp,0,0,0,0,0,0});
        CHECK(!tl_req_held || (p->dbg_tl_req_valid && unsigned(p->dbg_tl_req_bank)==tl_req_bank),"TL request VALID/bank changed while held");
        CHECK(!tl_resp_held || (p->dbg_tl_resp_valid && unsigned(p->dbg_tl_resp_bank)==tl_resp_bank),"TL response VALID/bank changed while held");
        tl_req_held=p->dbg_tl_req_valid && !p->dbg_tl_req_ready;tl_req_bank=p->dbg_tl_req_bank;
        tl_resp_held=p->dbg_tl_resp_valid && !p->dbg_tl_resp_ready;tl_resp_bank=p->dbg_tl_resp_bank;
        ++stability_samples;launch_errors+=pop8(p->dbg_dma_launch_error);
        CHECK(p->dbg_dma_launch_error==0,"real DMA launch error");
        if(p->dbg_tl_req_valid && p->dbg_tl_req_ready) {
            unsigned ch=p->dbg_tl_req_bank;CHECK(ch<2 && active[ch],"TL accept without active job");
            ++channels[ch].tl_accept;++active[ch]->bus.tl_accept;progress[ch]=true;
            event("tl_accept",active[ch],cyc,active[ch]->bus.tl_accept,ch);
        }
        if(p->dbg_tl_resp_valid && p->dbg_tl_resp_ready) {
            unsigned ch=p->dbg_tl_resp_bank;CHECK(ch<2 && active[ch],"TL response without active job");
            CHECK(active[ch]->bus.tl_resp<active[ch]->bus.tl_accept,"TL response without matching accepted work");
            ++channels[ch].tl_resp;++active[ch]->bus.tl_resp;progress[ch]=true;
            event("tl_resp",active[ch],cyc,active[ch]->bus.tl_resp,ch);
        }
        if(p->ddr_ar_valid && p->ddr_ar_ready) {
            CHECK(p->ddr_ar_size==3 && p->ddr_ar_burst==1,"unsupported DDR read size/burst");
            Burst q;q.addr=p->ddr_ar_addr;q.id=p->ddr_ar_id;q.beats=uint32_t(p->ddr_ar_len)+1;
            q.job=payload_job(q.id);q.channel=q.job?int(q.job->channel):-1;
            const uint64_t n=uint64_t(q.beats)*8;++all_ddr.ar;all_ddr.ar_bytes+=n;
            if(q.job) {
                CHECK(q.addr>=to_pa(q.job->src) && q.addr+n<=to_pa(q.job->src)+q.job->bytes,"payload AR outside active source");
                ++channels[q.channel].ar;channels[q.channel].ar_bytes+=n;
                ++q.job->bus.ar;q.job->bus.ar_bytes+=n;progress[q.channel]=true;
                event("ddr_ar",q.job,cyc,n,q.addr);
            } else {++metadata_ar;metadata_ar_bytes+=n;event("metadata_ar",nullptr,cyc,n,q.addr);}
            rq.push_back(q);
        }
        if(p->ddr_aw_valid && p->ddr_aw_ready) {
            CHECK(p->ddr_aw_size==3 && p->ddr_aw_burst==1,"unsupported DDR write size/burst");
            Burst q;q.addr=p->ddr_aw_addr;q.id=p->ddr_aw_id;q.beats=uint32_t(p->ddr_aw_len)+1;
            q.job=payload_job(q.id);CHECK(q.job,"unexpected metadata DDR write");q.channel=q.job->channel;
            const uint64_t n=uint64_t(q.beats)*8;
            CHECK(q.addr>=to_pa(q.job->dst) && q.addr+n<=to_pa(q.job->dst)+q.job->bytes,"payload AW outside active destination");
            ++all_ddr.aw;all_ddr.aw_bytes+=n;++channels[q.channel].aw;channels[q.channel].aw_bytes+=n;
            ++q.job->bus.aw;q.job->bus.aw_bytes+=n;progress[q.channel]=true;
            event("ddr_aw",q.job,cyc,n,q.addr);wq.push_back(q);
        }
        if(p->ddr_w_valid && p->ddr_w_ready) {
            CHECK(!wq.empty(),"observed W without AW");Burst& q=wq.front();
            CHECK(q.got<q.beats && bool(p->ddr_w_last)==(q.got+1==q.beats),"observed W LAST mismatch");
            CHECK(p->ddr_w_strb==0xff,"aligned whole-tensor W strobe is not full width");
            const unsigned n=pop8(p->ddr_w_strb);
            ++all_ddr.w;all_ddr.w_bytes+=n;++channels[q.channel].w;channels[q.channel].w_bytes+=n;
            ++q.job->bus.w;q.job->bus.w_bytes+=n;progress[q.channel]=true;
            ++q.got;if(q.got==q.beats){bq.push_back(q);wq.pop_front();}
        }
        if(p->ddr_b_valid && p->ddr_b_ready) {
            CHECK(!bq.empty(),"observed B without complete W burst");Burst q=bq.front();
            CHECK(p->ddr_b_id==q.id,"observed B ID mismatch");
            ++all_ddr.b;++channels[q.channel].b;++q.job->bus.b;progress[q.channel]=true;
            if(p->ddr_b_resp){++all_ddr.errors;++channels[q.channel].errors;++q.job->bus.errors;}
            event("ddr_b",q.job,cyc,p->ddr_b_resp,q.id);
            CHECK(p->ddr_b_resp==0,"DDR B error");bq.pop_front();
        }
        if(p->ddr_r_valid && p->ddr_r_ready) {
            CHECK(!rq.empty(),"observed R without AR");Burst& q=rq.front();
            CHECK(p->ddr_r_id==q.id && bool(p->ddr_r_last)==(q.got+1==q.beats),"observed R ID/LAST mismatch");
            ++all_ddr.r;all_ddr.r_bytes+=8;
            if(q.job) {
                ++channels[q.channel].r;channels[q.channel].r_bytes+=8;
                ++q.job->bus.r;q.job->bus.r_bytes+=8;progress[q.channel]=true;
                if(p->ddr_r_resp){++channels[q.channel].errors;++q.job->bus.errors;}
            } else metadata_r_bytes+=8;
            if(p->ddr_r_resp)++all_ddr.errors;
            CHECK(p->ddr_r_resp==0,"DDR R error");++q.got;if(q.got==q.beats)rq.pop_front();
        }
        // Process same-edge bus traffic before the corresponding completion.
        bool newly_complete[2]={false,false};Job* complete_job[2]={nullptr,nullptr};
        for(unsigned ch=0;ch<2;++ch) {
            if((p->dbg_dma_issue>>ch)&1u) {
                Job* j=active[ch];CHECK(j && j->issue<0 && j->done<0,"unexpected/duplicate hardware issue");
                j->issue=cyc;++j->bus.issue;++channels[ch].issue;progress[ch]=true;event("issue",j,cyc);
            }
            if((p->dbg_dma_done>>ch)&1u) {
                Job* j=active[ch];CHECK(j && j->issue>=0 && j->done<0,"hardware done without unique issue");
                CHECK(j->bus.ar_bytes==j->bytes && j->bus.r_bytes==j->bytes && j->bus.w_bytes==j->bytes,"done before exact payload bytes");
                CHECK(j->bus.aw==j->bus.b && j->bus.aw_bytes==j->bytes && !j->bus.errors,"done before write-response conservation");
                CHECK(j->bus.tl_accept==j->bus.tl_resp,"done before TL request/response conservation");
                j->done=cyc;++j->bus.done;++channels[ch].done;progress[ch]=true;event("done",j,cyc);
                const unsigned r=j->role;++role_completed[r];role_completed_bytes[r]+=j->bytes;
                CHECK(role_completed[r]<=6 && role_completed_bytes[r]<=USEFUL,"role completion overflow");
                if(role_completed[r]==6){CHECK(role_completed_bytes[r]==USEFUL,"whole role byte mismatch");role_done[r]=cyc;newly_complete[r]=true;complete_job[r]=j;}
            }
        }
        // Both same-edge completions are included in the other-role snapshot.
        for(unsigned r=0;r<2;++r)if(newly_complete[r]) {
            other_bytes_at_role_done[r]=role_completed_bytes[1-r];
            event("role_complete",complete_job[r],cyc,USEFUL,6);
            event("other_role_completed",complete_job[r],cyc,role_completed_bytes[1-r],1-r);
        }
        if(first_complete_role<0 && (newly_complete[0] || newly_complete[1])) {
            first_complete_role=newly_complete[0]?0:1;
            other_bytes_at_first_complete=role_completed_bytes[1-first_complete_role];
        }
        if(all_hardware_done<0 && role_completed[0]+role_completed[1]==jobs.size()) {
            all_hardware_done=cyc;event("all_hardware_done",nullptr,cyc,c.useful(),jobs.size());
        }
        if(all_hardware_done>=0 && natural_quiet<0 && quiet()) {
            natural_quiet=cyc;event("natural_quiet",nullptr,cyc,3,0);
        }
        for(unsigned ch=0;ch<2;++ch) {
            if(progress[ch])last_true_progress[ch]=cyc;
            if(active[ch] && cyc-last_true_progress[ch]>NO_PROGRESS) {
                std::fprintf(stderr,"STALL cycle=%ld ch=%u last_progress=%ld R_valid=%u R_ready=%u R_id=%u R_last=%u W_valid=%u W_ready=%u TL_req_valid=%u TL_req_ready=%u TL_req_bank=%u TL_resp_valid=%u TL_resp_ready=%u TL_resp_bank=%u metadata_AR=%llu mem_q=%zu caw_q=%zu cb_q=%zu\n",cyc,ch,last_true_progress[ch],unsigned(p->ddr_r_valid),unsigned(p->ddr_r_ready),unsigned(p->ddr_r_id),unsigned(p->ddr_r_last),unsigned(p->ddr_w_valid),unsigned(p->ddr_w_ready),unsigned(p->dbg_tl_req_valid),unsigned(p->dbg_tl_req_ready),unsigned(p->dbg_tl_req_bank),unsigned(p->dbg_tl_resp_valid),unsigned(p->dbg_tl_resp_ready),unsigned(p->dbg_tl_resp_bank),(unsigned long long)metadata_ar,tb.mem_q.size(),tb.caw_q.size(),tb.cb_q.size());
                for(const auto& q:tb.mem_q)std::fprintf(stderr,"R_QUEUE id=%u addr=%llx beat=%u len=%u latency=%d accepted=%ld\n",unsigned(q.id),(unsigned long long)q.addr,q.beat,unsigned(q.len),q.latency,q.accepted_cycle-t0);
                for(const auto& q:tb.caw_q)std::fprintf(stderr,"AW_QUEUE id=%u addr=%llx got=%u len=%u\n",q.id,(unsigned long long)q.addr,q.got,unsigned(q.len));
                for(unsigned k=0;k<2;++k)std::fprintf(stderr,"CHANNEL %u ARbytes=%llu Rbytes=%llu AWbytes=%llu Wbytes=%llu AW=%llu B=%llu TLaccept=%llu TLresp=%llu\n",k,(unsigned long long)channels[k].ar_bytes,(unsigned long long)channels[k].r_bytes,(unsigned long long)channels[k].aw_bytes,(unsigned long long)channels[k].w_bytes,(unsigned long long)channels[k].aw,(unsigned long long)channels[k].b,(unsigned long long)channels[k].tl_accept,(unsigned long long)channels[k].tl_resp);
                tb.dump_stall("stage106-long-job-no-progress");
                CHECK(false,"active channel lacks true bus/TL/issue/done progress");
            }
        }
        CHECK(cyc>=0 && uint64_t(cyc)<=total_budget,"whole offered-work hang budget exhausted");
    }
};
Observer* Observer::current=nullptr;

static Case parse_case(const std::string& id) {
    Case c;c.id=id;
    if(id.compare(0,3,"4k-")==0)c.page_bytes=4096;
    else if(id.compare(0,3,"2m-")==0)c.page_bytes=2097152;
    else CHECK(false,"case ID must use frozen 4k-/2m- prefix");
    c.dispatch=id.substr(3);
    if(c.dispatch=="solo0")c.offered[0]=true;
    else if(c.dispatch=="solo1")c.offered[1]=true;
    else if(c.dispatch=="parallel" || c.dispatch=="single")c.offered[0]=c.offered[1]=true;
    else CHECK(false,"dispatch is not a registered case");
    c.single=c.dispatch=="single";return c;
}
static std::vector<Job> load_jobs(const std::string& path,const Case& c) {
    std::ifstream f(path);CHECK(f.good(),"cannot open commands TSV");std::string line;
    CHECK(bool(std::getline(f,line)),"missing TSV header");
    std::istringstream hs(line);std::vector<std::string> header;std::string part;
    while(hs>>part)header.push_back(part);
    CHECK(header==std::vector<std::string>({"id","role","channel","src_iova","dst_iova","bytes","tensor","ready"}),"TSV header mismatch");
    std::vector<Job> jobs;std::set<uint64_t> ids;unsigned seen[2]={0,0};
    while(std::getline(f,line)) {
        if(line.empty())continue;Job j;std::string extra;std::istringstream s(line);
        CHECK(bool(s>>j.id>>j.role>>j.channel>>j.src>>j.dst>>j.bytes>>j.tensor>>j.ready) && !(s>>extra),"invalid TSV row");
        CHECK(j.role<2 && c.offered[j.role] && j.channel<2 && j.tensor<6,"TSV role/channel/tensor mismatch");
        CHECK(j.channel==(c.single?0:j.role),"logical role physical-channel mismatch");
        CHECK(j.ready==0 && j.tensor==seen[j.role]++,"ready must stay zero and tensors must be full FIFO");
        CHECK(j.id==j.role*6+j.tensor && ids.insert(j.id).second,"TSV job ID mismatch/duplicate");
        CHECK(j.bytes==tensor_bytes[j.tensor] && j.src==IOVA+(2*j.role)*SLOT+tensor_offset[j.tensor]
              && j.dst==IOVA+(2*j.role+1)*SLOT+tensor_offset[j.tensor],"TSV complete tensor address/size mismatch");
        CHECK(j.bytes%8==0 && j.src%8==0 && j.dst%8==0,"unaligned full tensor");
        if(c.single)CHECK(j.role==jobs.size()/6,"single must finish role0 before role1");
        jobs.push_back(j);
    }
    CHECK(f.eof() && !f.bad(),"TSV read error");
    for(unsigned r=0;r<2;++r)CHECK(seen[r]==(c.offered[r]?6u:0u),"TSV omitted full role work");
    CHECK(jobs.size()==6*(unsigned(c.offered[0])+unsigned(c.offered[1])),"offered job count mismatch");
    return jobs;
}
static std::vector<uint8_t> load_payload(const std::string& path,unsigned role) {
    std::ifstream f(path,std::ios::binary);CHECK(f.good(),"cannot open rank payload");
    f.seekg(0,std::ios::end);CHECK(f.tellg()==std::streamoff(EXTENT),"payload extent mismatch");f.seekg(0);
    std::vector<uint8_t> bytes(EXTENT);f.read(reinterpret_cast<char*>(bytes.data()),bytes.size());
    CHECK(f.good(),"payload read failed");std::vector<uint8_t> expected(EXTENT,0x3c);
    for(unsigned t=0;t<6;++t)for(uint64_t w=0;w<tensor_bytes[t]/8;++w) {
        const uint64_t v=(uint64_t(role+1)<<60)^(uint64_t(t)<<52)^w;
        for(unsigned b=0;b<8;++b)expected[tensor_offset[t]+8*w+b]=uint8_t(v>>(8*b));
    }
    CHECK(bytes==expected,"rank/tensor-distinct fixture or source-hole bytes mismatch");return bytes;
}

struct MetadataPage {uint64_t pa;std::string kind;int role;};
struct Mapping {
    uint64_t pte_pages=0,leaf_ptes=0,nonleaf_ptes=0,pte_write64=0,ddt_write64=0;
    uint64_t next=0x100000;
    std::vector<MetadataPage> pages;
    std::map<uint64_t,uint8_t> initialized;
};
static void pte_write(Tb& tb,Mapping& m,uint64_t a,uint64_t v) {tb.write64(a,v);++m.pte_write64;}
static uint64_t page(Tb& tb,Mapping& m,const char* kind,int role,uint64_t fixed=0) {
    const uint64_t p=fixed?fixed:m.next;if(!fixed)m.next+=4096;
    CHECK(p<0x01000000 && p%4096==0,"diagnostic PT allocator escaped declared low PA");
    ++m.pte_pages;m.pages.push_back({p,kind,role});
    for(uint64_t i=0;i<4096;i+=8)pte_write(tb,m,p+i,0);return p;
}
static Mapping setup_tables(Tb& tb,const Case& c) {
    Mapping m;m.pages.push_back({0x10000,"ddt",-1});
    for(uint64_t i=0;i<4096;i+=8){tb.write64(0x10000+i,0);++m.ddt_write64;}
    for(unsigned ch=0;ch<(c.single?1u:2u);++ch) {
        const uint64_t root=page(tb,m,"root",ch,ch?0x30000:0x20000);
        const uint64_t dc=0x10000+(10+ch)*64;
        tb.write64(dc,1);tb.write64(dc+16,ch?0xB000:0xA000);
        tb.write64(dc+24,0x8000000000000000ULL|(root>>12));m.ddt_write64+=3;
        const uint64_t l1=page(tb,m,"l1",ch);
        pte_write(tb,m,root+((IOVA>>30)&511)*8,((l1>>12)<<10)|1);++m.nonleaf_ptes;
        const unsigned first=c.single?0:2*ch,last=c.single?4:2*ch+2;
        for(unsigned slot=first;slot<last;++slot)for(unsigned seg=0;seg<5;++seg) {
            const uint64_t va=IOVA+slot*SLOT+seg*0x200000ULL;
            const uint64_t pa=PA+slot*SLOT+seg*0x200000ULL;
            const uint64_t flags=(slot&1)?0xD7ULL:0xD3ULL; // V,U,A,D plus R or RW, no X.
            if(c.page_bytes==2097152) {
                pte_write(tb,m,l1+((va>>21)&511)*8,((pa>>12)<<10)|flags);++m.leaf_ptes;
            } else {
                const uint64_t l0=page(tb,m,"l0",ch);
                pte_write(tb,m,l1+((va>>21)&511)*8,((l0>>12)<<10)|1);++m.nonleaf_ptes;
                for(uint64_t i=0;i<512;++i){pte_write(tb,m,l0+i*8,(((pa+i*4096)>>12)<<10)|flags);++m.leaf_ptes;}
            }
        }
    }
    CHECK(m.pte_pages==(c.page_bytes==4096?(c.single?22u:24u):(c.single?2u:4u)),"actual allocated PTE page count mismatch");
    CHECK(m.leaf_ptes==(c.page_bytes==4096?10240u:20u),"actual complete leaf count mismatch");
    CHECK(m.nonleaf_ptes==(c.page_bytes==4096?(c.single?21u:22u):(c.single?1u:2u)),"actual nonleaf count mismatch");
    CHECK(tb.mem.size()==(m.pte_pages+1)*4096,"initialized metadata extent mismatch");
    m.initialized=tb.mem;return m;
}

static void terminal_check(Observer& o) {
    CHECK(o.all_hardware_done>=0,"terminal check before all offered work completed");
    while(o.natural_quiet<0)o.tb.tick();
    CHECK(o.quiet(),"natural quiet disappeared before terminal validation");
    for(unsigned i=0;i<64;++i){o.tb.tick();CHECK(o.quiet(),"activity reappeared during quiet confirmation");}
    CHECK(o.tb.top->dbg_cpu_issued==0 && o.tb.top->dbg_cpu_completed==0,"CPU-off violated");
    CHECK(o.all_ddr.ar_bytes==o.all_ddr.r_bytes && o.all_ddr.aw==o.all_ddr.b
          && o.all_ddr.aw_bytes==o.all_ddr.w_bytes && !o.all_ddr.errors,"all-DDR conservation/error mismatch");
    CHECK(o.metadata_ar_bytes==o.metadata_r_bytes,"metadata request/return bytes differ");
    uint64_t offered_read=0;
    for(unsigned ch=0;ch<2;++ch) {
        const Stats& s=o.channels[ch];offered_read+=s.ar_bytes;
        CHECK(!o.active[ch] && s.issue==s.done && s.ar_bytes==s.r_bytes && s.ar_bytes==s.w_bytes
              && s.aw_bytes==s.w_bytes && s.aw==s.b && !s.errors,"channel conservation/error mismatch");
        CHECK(s.tl_accept==s.tl_resp,"terminal TL accept/response mismatch");
    }
    CHECK(offered_read==o.c.useful(),"complete offered useful bytes not transferred exactly once");
    o.validation_end=o.tb.g_cycle-o.t0;o.event("validation_end",nullptr,o.validation_end,64,0);
}

static void export_metadata(Tb& tb,const Mapping& m,const std::string& path) {
    CHECK(tb.mem==m.initialized,"actual DDT/PTE metadata changed during copies");
    std::ofstream f(path,std::ios::binary);CHECK(f.good(),"cannot open metadata.bin");
    for(const auto& p:m.pages)for(uint64_t off=0;off<4096;off+=8) {
        const uint64_t word=tb.read64(p.pa+off);
        for(unsigned b=0;b<8;++b)f.put(char(uint8_t(word>>(8*b))));
    }
    f.flush();CHECK(f.good(),"metadata.bin write failed");
}
static void write_summary(const std::string& path,const Observer& o,const Mapping& m,
                          const Config channels_cfg[2]) {
    std::ofstream f(path);CHECK(f.good(),"cannot open summary.json");
    f<<"{\"schema\":\"ai-mandatory-transfer-g0-v1\",\"pass\":true,\"stage\":106"
     <<",\"scope\":\"cold_independent_operand_transfer_stage_FPGA_projection_only\""
     <<",\"case_id\":\""<<o.c.id<<"\",\"dispatch\":\""<<o.c.dispatch<<"\",\"page_bytes\":"<<o.c.page_bytes
     <<",\"offered_roles\":[";bool first=true;
    for(unsigned r=0;r<2;++r)if(o.c.offered[r]){if(!first)f<<',';f<<r;first=false;}
    f<<"],\"useful_bytes\":"<<o.c.useful()<<",\"payload_extent_bytes\":"<<EXTENT
     <<",\"common_t0_absolute_cycle\":"<<o.t0<<",\"all_hardware_done_cycle\":"<<o.all_hardware_done
     <<",\"natural_quiet_cycle\":"<<o.natural_quiet<<",\"quiet_cycle\":"<<o.natural_quiet
     <<",\"validation_end_cycle\":"<<o.validation_end<<",\"first_complete_role\":"<<o.first_complete_role
     <<",\"other_role_completed_useful_bytes_at_first_complete\":"<<o.other_bytes_at_first_complete
     <<",\"policy_enable\":0,\"cpu_enable\":0,\"nonblock_enable\":0,\"ptw_latency\":1,\"model_only_metadata\":true"
     <<",\"priority\":false,\"read_admit\":0,\"warmup\":0,\"STOP_used\":false"
     <<",\"watchdog\":{\"no_progress_cycles\":"<<NO_PROGRESS<<",\"total_cycle_budget\":"<<o.total_budget
     <<",\"per_active_physical_channel\":true,\"cfg_counts_as_progress\":false}"
     <<",\"mapping\":{\"diagnostic_metadata_only\":true,\"ddt_base\":65536,\"page_bytes\":"<<o.c.page_bytes
     <<",\"logical_mapped_bytes\":"<<4*SLOT<<",\"payload_reserved_bytes\":"<<BACKING
     <<",\"pte_pages\":"<<m.pte_pages<<",\"pte_reserved_bytes\":"<<m.pte_pages*4096
     <<",\"leaf_pte_count\":"<<m.leaf_ptes<<",\"nonleaf_pte_count\":"<<m.nonleaf_ptes
     <<",\"pte_init_write64_count\":"<<m.pte_write64<<",\"ddt_init_write64_count\":"<<m.ddt_write64
     <<",\"init_write64_count\":"<<m.pte_write64+m.ddt_write64<<",\"ddt_reserved_bytes\":4096"
     <<",\"metadata_reserved_bytes\":"<<(m.pte_pages+1)*4096<<",\"metadata_initialized_unique_bytes\":"<<o.tb.mem.size()
     <<",\"metadata_dump\":\"metadata.bin\",\"metadata_pages\":[";
    for(size_t i=0;i<m.pages.size();++i) {
        if(i)f<<',';const auto& p=m.pages[i];
        f<<"{\"pa\":"<<p.pa<<",\"kind\":\""<<p.kind<<"\",\"role\":"<<p.role
         <<",\"file_offset\":"<<i*4096<<",\"bytes\":4096}";
    }
    f<<"],\"slots\":[";
    for(unsigned s=0;s<4;++s) {
        if(s)f<<',';
        f<<"{\"slot\":"<<s<<",\"role\":"<<s/2<<",\"kind\":\""<<((s&1)?"destination":"source")
         <<"\",\"iova\":"<<IOVA+s*SLOT<<",\"pa\":"<<PA+s*SLOT<<",\"bytes\":"<<SLOT
         <<",\"did\":"<<(o.c.single?10:10+s/2)<<",\"pte_flags\":"<<((s&1)?0xD7:0xD3)<<"}";
    }
    f<<"],\"did11_valid\":"<<(o.c.single?"false":"true")<<"}"
     <<",\"stability_samples\":"<<o.stability_samples<<",\"launch_errors\":"<<o.launch_errors
     <<",\"TL_payload_visibility\":\"debug_VALID_bank_only_not_complete_payload\",\"jobs\":[";
    for(size_t i=0;i<o.jobs.size();++i) {
        if(i)f<<',';const auto& j=o.jobs[i];
        f<<"{\"id\":"<<j.id<<",\"role\":"<<j.role<<",\"channel\":"<<j.channel<<",\"did\":"<<10+j.channel
         <<",\"src_iova\":"<<j.src<<",\"dst_iova\":"<<j.dst<<",\"bytes\":"<<j.bytes<<",\"tensor\":"<<j.tensor
         <<",\"ready\":"<<j.ready<<",\"channel_ready\":"<<j.channel_ready<<",\"config_start\":"<<j.config_start
         <<",\"config_program_end\":"<<j.config_program_end<<",\"issue\":"<<j.issue<<",\"done\":"<<j.done
         <<",\"next_id\":"<<j.hw_id<<",\"next_cycle\":"<<j.next_cycle<<",\"done_id\":"<<j.done_id
         <<",\"done_reg_cycle\":"<<j.done_reg_cycle<<",\"ready_to_issue_cycles\":"<<j.issue-j.ready
         <<",\"issue_to_hardware_done_cycles\":"<<j.done-j.issue
         <<",\"hardware_done_to_polled_done_cycles\":"<<j.done_reg_cycle-j.done<<",\"cfg\":";
        config_json(f,j.cfg);f<<",\"bus\":";stat_json(f,j.bus);f<<'}';
    }
    f<<"],\"roles\":[";
    for(unsigned r=0;r<2;++r) {
        if(r)f<<',';Stats total;for(const auto& j:o.jobs)if(j.role==r)add(total,j.bus);
        f<<"{\"role\":"<<r<<",\"offered\":"<<(o.c.offered[r]?"true":"false")
         <<",\"completed_tensors\":"<<o.role_completed[r]<<",\"completed_useful_bytes\":"<<o.role_completed_bytes[r]
         <<",\"hardware_done_cycle\":"<<o.role_done[r]<<",\"polled_done_cycle\":"<<o.role_done_reg[r]
         <<",\"other_role_completed_useful_bytes_at_hardware_done\":"<<o.other_bytes_at_role_done[r]<<",\"tensors\":[";
        for(unsigned t=0;t<6;++t) {
            if(t)f<<',';long done=-1,polled=-1;for(const auto& j:o.jobs)if(j.role==r && j.tensor==t){done=j.done;polled=j.done_reg_cycle;}
            f<<"{\"tensor\":"<<t<<",\"name\":\""<<tensor_name[t]<<"\",\"offset\":"<<tensor_offset[t]
             <<",\"bytes\":"<<tensor_bytes[t]<<",\"done\":"<<done<<",\"done_reg_cycle\":"<<polled<<"}";
        }
        f<<"],\"bus\":";stat_json(f,total);f<<'}';
    }
    Config total_cfg;
    f<<"],\"channels\":[";
    for(unsigned ch=0;ch<2;++ch) {
        if(ch)f<<',';const auto& cc=channels_cfg[ch];
        total_cfg.writes+=cc.writes;total_cfg.reads+=cc.reads;total_cfg.write_cycles+=cc.write_cycles;
        total_cfg.read_cycles+=cc.read_cycles;total_cfg.next_reads+=cc.next_reads;total_cfg.done_reads+=cc.done_reads;
        f<<"{\"channel\":"<<ch<<",\"did\":"<<10+ch<<",\"last_true_progress\":"<<o.last_true_progress[ch]<<",\"bus\":";
        stat_json(f,o.channels[ch]);f<<",\"cfg\":";config_json(f,cc);f<<'}';
    }
    f<<"],\"cfg\":";config_json(f,total_cfg);
    f<<",\"disable_writes\":2,\"all_ddr\":";stat_json(f,o.all_ddr);
    f<<",\"metadata_traffic\":{\"ar\":"<<o.metadata_ar<<",\"ar_bytes\":"<<o.metadata_ar_bytes
     <<",\"r_bytes\":"<<o.metadata_r_bytes<<"},\"terminal\":{\"dma_job_quiet\":"<<unsigned(o.tb.top->dbg_dma_job_quiet)
     <<",\"tl_in_flight\":"<<unsigned(o.tb.top->dbg_in_flight)<<",\"cpu_quiet\":"<<unsigned(o.tb.top->dbg_cpu_quiet)
     <<",\"cpu_wr_quiet\":"<<unsigned(o.tb.top->dbg_cpu_wr_quiet)<<",\"cpu_issued\":"<<o.tb.top->dbg_cpu_issued
     <<",\"cpu_completed\":"<<o.tb.top->dbg_cpu_completed<<",\"memory_r_queue\":"<<o.tb.mem_q.size()
     <<",\"memory_aw_queue\":"<<o.tb.caw_q.size()<<",\"memory_b_queue\":"<<o.tb.cb_q.size()
     <<",\"observer_r_queue\":"<<o.rq.size()<<",\"observer_aw_queue\":"<<o.wq.size()
     <<",\"observer_b_queue\":"<<o.bq.size()<<",\"ddr_valid\":0}"
     <<",\"source_match\":true,\"destination_match\":true,\"padding_preserved\":true"
     <<",\"inactive_target_preserved\":true,\"inactive_destination_preserved\":true"
     <<",\"slot_tail_guard_preserved\":true,\"backing_guard_preserved\":true"
     <<",\"unused_backing_preserved\":true,\"metadata_unchanged\":true"
     <<",\"guards_checked_bytes\":"<<BACKING<<",\"output_includes_extent_holes\":true"
     <<",\"board_costs_measured\":false,\"trained_weights\":false,\"GPU_inference_run\":false}\n";
    f.flush();CHECK(f.good(),"summary.json write failed");
}

static int run(int argc,char** argv) {
    CHECK(argc==6,"usage: executable CASE_ID TSV PAYLOAD0 PAYLOAD1 OUTDIR");
    const Case c=parse_case(argv[1]);const std::string out=argv[5];
    auto jobs=load_jobs(argv[2],c);
    const std::vector<uint8_t> payloads[2]={load_payload(argv[3],0),load_payload(argv[4],1)};
    std::vector<uint8_t> touched(EXTENT,0);
    for(unsigned t=0;t<6;++t)std::fill(touched.begin()+tensor_offset[t],touched.begin()+tensor_offset[t]+tensor_bytes[t],1);
    Tb tb;tb.cpu_enable=false;tb.cpu_writes=false;tb.policy_partition=false;tb.owner_only=false;
    tb.ds_priority=false;tb.read_admit_mode_val=0;tb.read_admit_measure_only=false;tb.full_batch=false;
    tb.ptw_latency=1;tb.write_tail_pause=0;tb.read_latency_overlap=false;tb.warmup=0;
    const Mapping m=setup_tables(tb,c);
    for(auto& backing:tb.transfer_dense)std::fill(backing.begin(),backing.end(),0xa7);
    for(unsigned r=0;r<2;++r)for(uint64_t i=0;i<SLOT;++i) {
        dense(tb,2*r*SLOT+i)=i<EXTENT?payloads[r][i]:0x3c;
        dense(tb,(2*r+1)*SLOT+i)=0xd9;
    }
    tb.reset(false);
    Observer o(tb,jobs,c,out+"/events.csv");o.t0=tb.g_cycle;Observer::current=&o;tb.observe_pre_edge=&Observer::hook;
    o.event("common_start",nullptr,0,c.useful(),jobs.size());
    for(const auto& j:jobs)o.event("ready",&j,0);
    Config channels_cfg[2];
    auto cfg_write=[&](unsigned ch,uint64_t address,uint64_t value,Job* job) {
        const long begin=tb.g_cycle;tb.cfg_wr(ch==1,address,value);const uint64_t elapsed=tb.g_cycle-begin;
        ++channels_cfg[ch].writes;channels_cfg[ch].write_cycles+=elapsed;
        if(job){++job->cfg.writes;job->cfg.write_cycles+=elapsed;}
    };
    auto cfg_read=[&](unsigned ch,uint64_t address,Job& job) {
        const long begin=tb.g_cycle;const uint64_t value=tb.cfg_rd(ch==1,address);const uint64_t elapsed=tb.g_cycle-begin;
        ++channels_cfg[ch].reads;channels_cfg[ch].read_cycles+=elapsed;++job.cfg.reads;job.cfg.read_cycles+=elapsed;
        if(address==REG_NEXT){++channels_cfg[ch].next_reads;++job.cfg.next_reads;}
        if(address==REG_DONE){++channels_cfg[ch].done_reads;++job.cfg.done_reads;}
        return value;
    };
    cfg_write(0,REG_B_CTL,0,nullptr);cfg_write(1,REG_B_CTL,0,nullptr);
    std::deque<Job*> pending[2];for(auto& j:jobs)pending[j.channel].push_back(&j);
    // Admission and capacity are controller-owned and updated solely by real DONE reads.
    Job* running[2]={nullptr,nullptr};uint64_t retired_id[2]={1,1};long channel_available[2]={0,0};
    auto submit=[&](unsigned ch) {
        CHECK(!running[ch] && !pending[ch].empty(),"submit without controller free channel/queued job");
        Job* j=pending[ch].front();pending[ch].pop_front();running[ch]=j;
        CHECK(!o.active[ch],"observer ownership still active at legal submission");o.active[ch]=j;
        j->channel_ready=channel_available[ch];j->config_start=tb.g_cycle-o.t0;
        CHECK(j->ready==0 && j->config_start>=j->channel_ready,"config precedes immutable readiness or polled capacity");
        o.event("config_start",j,j->config_start);
        cfg_write(ch,REG_CONF,0,j);cfg_write(ch,REG_SRC,j->src,j);cfg_write(ch,REG_DST,j->dst,j);cfg_write(ch,REG_NB,j->bytes,j);
        j->config_program_end=tb.g_cycle-o.t0;o.event("config_program_end",j,j->config_program_end);
        const uint64_t next=cfg_read(ch,REG_NEXT,*j);
        CHECK(next>0 && next<=uint64_t(std::numeric_limits<int64_t>::max()) && next==retired_id[ch]+1,"NEXT_ID not next unique functional ID");
        j->hw_id=int64_t(next);j->next_cycle=tb.g_cycle-o.t0;o.event("next_id",j,j->next_cycle,next);
    };
    for(unsigned ch=0;ch<2;++ch)if(!pending[ch].empty())submit(ch);
    size_t retired=0;
    while(retired<jobs.size()) {
        for(unsigned ch=0;ch<2;++ch)if(running[ch]) {
            Job* j=running[ch];const uint64_t done=cfg_read(ch,REG_DONE,*j);
            CHECK(done==retired_id[ch] || done==uint64_t(j->hw_id),"DONE register returned unowned/out-of-order ID");
            if(done==uint64_t(j->hw_id)) {
                j->done_id=int64_t(done);j->done_reg_cycle=tb.g_cycle-o.t0;
                CHECK(j->issue>=j->ready && j->issue>=j->channel_ready && j->done>=j->issue
                      && j->bus.issue==1 && j->bus.done==1,"real register DONE lacks unique hardware issue/done");
                CHECK(j->done_reg_cycle>j->done && j->done_id==j->hw_id,"polled DONE does not match earlier hardware completion/NEXT_ID");
                o.event("done_reg",j,j->done_reg_cycle,done);
                o.role_done_reg[j->role]=j->done_reg_cycle;
                retired_id[ch]=done;channel_available[ch]=j->done_reg_cycle;running[ch]=nullptr;o.active[ch]=nullptr;++retired;
                // Refill immediately on this actual polled completion, before polling the next lane.
                if(!pending[ch].empty())submit(ch);
            }
        }
        tb.tick();
    }
    terminal_check(o);
    for(unsigned r=0;r<2;++r)for(uint64_t i=0;i<SLOT;++i) {
        CHECK(dense(tb,2*r*SLOT+i)==(i<EXTENT?payloads[r][i]:0x3c),"source slot or source padding damaged");
        const bool useful=i<EXTENT && touched[i];
        const uint8_t expected=c.offered[r] && useful?payloads[r][i]:0xd9;
        CHECK(dense(tb,(2*r+1)*SLOT+i)==expected,"destination tensor, hole, tail or inactive target damaged");
    }
    for(uint64_t i=4*SLOT;i<BACKING;++i)CHECK(dense(tb,i)==0xa7,"unused 8MiB backing damaged");
    export_metadata(tb,m,out+"/metadata.bin");
    for(unsigned r=0;r<2;++r) {
        std::vector<uint8_t> result(EXTENT);for(uint64_t i=0;i<EXTENT;++i)result[i]=dense(tb,(2*r+1)*SLOT+i);
        std::ofstream bin(out+"/rank"+std::to_string(r)+"-output.bin",std::ios::binary);CHECK(bin.good(),"cannot open rank output");
        bin.write(reinterpret_cast<const char*>(result.data()),result.size());bin.flush();CHECK(bin.good(),"rank output write failed");
    }
    write_summary(out+"/summary.json",o,m,channels_cfg);o.events.flush();CHECK(o.events.good(),"events flush failed");
    tb.observe_pre_edge=nullptr;Observer::current=nullptr;
    std::printf("PASS %s complete finite transfer-stage projection: bytes=%llu jobs=%zu natural-quiet=%ld validation-end=%ld\n",
                c.id.c_str(),(unsigned long long)c.useful(),jobs.size(),o.natural_quiet,o.validation_end);
    return 0;
}
} // namespace mandatory_transfer

int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);
    try{return mandatory_transfer::run(argc,argv);}
    catch(const std::exception& e){std::fprintf(stderr,"FAIL independent finite transfer: %s\n",e.what());return 1;}
}
