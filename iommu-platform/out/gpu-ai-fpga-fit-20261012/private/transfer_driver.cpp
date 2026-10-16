// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Finite source-derived byte-transfer functional seed, not a performance run.
// The retained RTL wrapper and DDR protocol remain unchanged.
#define main retained_diagnostic_main
#include "retained_tb.cpp"
#undef main
#include <array>
#include <fstream>
#include <sstream>
#include <set>
#include <limits>

namespace transfer_seed {
constexpr uint64_t REGION = 16ULL * 1024 * 1024;
constexpr uint64_t SRC = 0x40000000ULL, RELAY = 0x50000000ULL, DST = 0x60000000ULL;
constexpr uint64_t SRC_PA = 0x84000000ULL, RELAY_PA = 0x85000000ULL, DST_PA = 0x86000000ULL;
constexpr uint64_t USEFUL = 9447936, PAYLOAD = 9457664;
const uint64_t tensor_offset[6] = {0,4718592,4726784,9445376,9449472,9453568};
const uint64_t tensor_bytes[6] = {4718592,6144,4718592,1536,1536,1536};
const char* tensor_name[6] = {"fc1.weight","fc1.bias","fc2.weight","fc2.bias",
                            "final_layer_norm.weight","final_layer_norm.bias"};

struct Stats {
    uint64_t issue=0,done=0,ar=0,aw=0,r=0,w=0,b=0;
    uint64_t ar_bytes=0,r_bytes=0,aw_bytes=0,w_bytes=0,errors=0;
    uint64_t tl_accept=0,tl_resp=0;
};
struct Job {
    uint64_t id=0,src=0,dst=0,bytes=0,chunk=0;
    int channel=0,tensor=0;
    int64_t dep=-1,hw_id=-1,done_id=-1;
    long ready=0,channel_ready=0,config_start=-1,issue=-1,done=-1,next_cycle=-1,done_reg_cycle=-1;
    Stats bus;
};
struct Tensor {
    uint64_t expected_fill_count=0,expected_fill_bytes=0;
    uint64_t completed_fill_count=0,completed_fill_bytes=0;
    long barrier_cycle=-1;
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
        if (held) data=now;
    }
};

static uint64_t to_pa(uint64_t a) {
    if (a>=SRC && a<SRC+REGION) return SRC_PA+(a-SRC);
    if (a>=RELAY && a<RELAY+REGION) return RELAY_PA+(a-RELAY);
    if (a>=DST && a<DST+REGION) return DST_PA+(a-DST);
    CHECK(false,"IOVA outside declared transfer mappings"); return 0;
}
static unsigned pop8(unsigned x) {
    unsigned n=0; for (unsigned i=0;i<8;++i) n+=(x>>i)&1u; return n;
}
static void stat_json(std::ostream& f,const Stats& s) {
    f<<"{\"issue\":"<<s.issue<<",\"done\":"<<s.done
     <<",\"ar\":"<<s.ar<<",\"aw\":"<<s.aw<<",\"r\":"<<s.r
     <<",\"w\":"<<s.w<<",\"b\":"<<s.b<<",\"ar_bytes\":"<<s.ar_bytes
     <<",\"r_bytes\":"<<s.r_bytes<<",\"aw_bytes\":"<<s.aw_bytes
     <<",\"w_bytes\":"<<s.w_bytes<<",\"errors\":"<<s.errors
     <<",\"tl_accept\":"<<s.tl_accept<<",\"tl_resp\":"<<s.tl_resp<<"}";
}

class Observer {
public:
    Tb& tb;
    std::vector<Job>& jobs;
    Tensor tensors[6];
    Stats channels[2],all_ddr;
    Job* active[2]={nullptr,nullptr};
    long last_done[2]={0,0};
    long t0=0,quiet_cycle=-1;
    uint64_t stability_samples=0,launch_errors=0;
    std::deque<Burst> rq,wq,bq;
    Held arheld,awheld,wheld,rheld,bheld;
    std::ofstream events;
    static Observer* current;

    Observer(Tb& t,std::vector<Job>& js,const std::string& path):tb(t),jobs(js),events(path) {
        CHECK(events.good(),"cannot open events.csv");
        events<<"cycle,event,job,channel,hw_id,src_iova,dst_iova,bytes,dep,tensor,chunk,value,aux\n";
        for (const auto& j:jobs) if(j.channel==0) {
            ++tensors[j.tensor].expected_fill_count;
            tensors[j.tensor].expected_fill_bytes+=j.bytes;
        }
    }
    void event(const char* kind,const Job* j,long cyc,uint64_t value=0,uint64_t aux=0) {
        events<<cyc<<','<<kind<<',';
        if(j) events<<j->id<<','<<j->channel<<','<<j->hw_id<<','<<j->src<<','<<j->dst
                    <<','<<j->bytes<<','<<j->dep<<','<<j->tensor<<','<<j->chunk;
        else events<<"-1,-1,-1,0,0,0,-1,-1,-1";
        events<<','<<value<<','<<aux<<'\n';
        CHECK(events.good(),"events.csv write failed");
    }
    static void hook(Tb& t) { CHECK(current && &current->tb==&t,"observer owner mismatch"); current->sample(); }
    Job* payload_job(uint32_t id) {
        const unsigned port=(id>>4)&3u;
        if(port!=2) { CHECK(port==3,"CPU/debug DDR traffic in CPU-off seed"); return nullptr; }
        const unsigned ch=(id>>2)&1u;
        CHECK(active[ch],"payload bus traffic without a scheduled job");
        return active[ch];
    }
    void sample() {
        auto* p=tb.top;
        CHECK(p->rst_ni,"observer invoked during reset");
        const long cyc=tb.g_cycle-t0;
        arheld.check(p->ddr_ar_valid,p->ddr_ar_ready,{p->ddr_ar_id,p->ddr_ar_addr,p->ddr_ar_len,p->ddr_ar_size,p->ddr_ar_burst,p->ddr_ar_prot,p->ddr_ar_user,0});
        awheld.check(p->ddr_aw_valid,p->ddr_aw_ready,{p->ddr_aw_id,p->ddr_aw_addr,p->ddr_aw_len,p->ddr_aw_size,p->ddr_aw_burst,p->ddr_aw_prot,p->ddr_aw_user,0});
        wheld.check(p->ddr_w_valid,p->ddr_w_ready,{p->ddr_w_data,p->ddr_w_strb,p->ddr_w_last,p->ddr_w_user,0,0,0,0});
        rheld.check(p->ddr_r_valid,p->ddr_r_ready,{p->ddr_r_id,p->ddr_r_data,p->ddr_r_resp,p->ddr_r_last,0,0,0,0});
        bheld.check(p->ddr_b_valid,p->ddr_b_ready,{p->ddr_b_id,p->ddr_b_resp,0,0,0,0,0,0});
        ++stability_samples;
        launch_errors+=pop8(p->dbg_dma_launch_error);
        CHECK(p->dbg_dma_launch_error==0,"real DMA launch error");
        if(p->dbg_tl_req_valid && p->dbg_tl_req_ready) {
            unsigned ch=p->dbg_tl_req_bank; CHECK(ch<2 && active[ch],"TL accept without active job");
            ++channels[ch].tl_accept; ++active[ch]->bus.tl_accept;
        }
        if(p->dbg_tl_resp_valid && p->dbg_tl_resp_ready) {
            unsigned ch=p->dbg_tl_resp_bank; CHECK(ch<2 && active[ch],"TL response without active job");
            ++channels[ch].tl_resp; ++active[ch]->bus.tl_resp;
        }
        if(p->ddr_ar_valid && p->ddr_ar_ready) {
            CHECK(p->ddr_ar_size==3 && p->ddr_ar_burst==1,"unsupported DDR read size/burst");
            Burst q; q.addr=p->ddr_ar_addr;q.id=p->ddr_ar_id;q.beats=uint32_t(p->ddr_ar_len)+1;
            q.job=payload_job(q.id); q.channel=q.job?q.job->channel:-1;
            const uint64_t n=uint64_t(q.beats)*8;
            ++all_ddr.ar;all_ddr.ar_bytes+=n;
            if(q.job) {
                CHECK(q.addr>=to_pa(q.job->src) && q.addr+n<=to_pa(q.job->src)+q.job->bytes,"payload AR outside active job source");
                ++channels[q.channel].ar;channels[q.channel].ar_bytes+=n;
                ++q.job->bus.ar;q.job->bus.ar_bytes+=n;
                event("ddr_ar",q.job,cyc,n,q.addr);
            }
            rq.push_back(q);
        }
        if(p->ddr_aw_valid && p->ddr_aw_ready) {
            CHECK(p->ddr_aw_size==3 && p->ddr_aw_burst==1,"unsupported DDR write size/burst");
            Burst q;q.addr=p->ddr_aw_addr;q.id=p->ddr_aw_id;q.beats=uint32_t(p->ddr_aw_len)+1;
            q.job=payload_job(q.id);CHECK(q.job,"unexpected non-payload DDR write");q.channel=q.job->channel;
            const uint64_t n=uint64_t(q.beats)*8;
            CHECK(q.addr>=to_pa(q.job->dst) && q.addr+n<=to_pa(q.job->dst)+q.job->bytes,"payload AW outside active job destination");
            ++all_ddr.aw;all_ddr.aw_bytes+=n;
            ++channels[q.channel].aw;channels[q.channel].aw_bytes+=n;
            ++q.job->bus.aw;q.job->bus.aw_bytes+=n;
            event("ddr_aw",q.job,cyc,n,q.addr);
            wq.push_back(q);
        }
        if(p->ddr_w_valid && p->ddr_w_ready) {
            CHECK(!wq.empty(),"observed W without AW"); Burst& q=wq.front();
            CHECK(q.got<q.beats && bool(p->ddr_w_last)==(q.got+1==q.beats),"observed W LAST mismatch");
            const unsigned n=pop8(p->ddr_w_strb);
            ++all_ddr.w;all_ddr.w_bytes+=n;
            ++channels[q.channel].w;channels[q.channel].w_bytes+=n;
            ++q.job->bus.w;q.job->bus.w_bytes+=n;
            ++q.got;if(q.got==q.beats) {bq.push_back(q);wq.pop_front();}
        }
        if(p->ddr_b_valid && p->ddr_b_ready) {
            CHECK(!bq.empty(),"observed B without complete W burst");Burst q=bq.front();
            CHECK(p->ddr_b_id==q.id,"observed B ID mismatch");
            ++all_ddr.b;++channels[q.channel].b;++q.job->bus.b;
            if(p->ddr_b_resp) {++all_ddr.errors;++channels[q.channel].errors;++q.job->bus.errors;}
            event("ddr_b",q.job,cyc,p->ddr_b_resp,q.id);
            CHECK(p->ddr_b_resp==0,"DDR B error");bq.pop_front();
        }
        if(p->ddr_r_valid && p->ddr_r_ready) {
            CHECK(!rq.empty(),"observed R without AR");Burst& q=rq.front();
            CHECK(p->ddr_r_id==q.id && bool(p->ddr_r_last)==(q.got+1==q.beats),"observed R ID/LAST mismatch");
            ++all_ddr.r;all_ddr.r_bytes+=8;
            if(q.job) {
                ++channels[q.channel].r;channels[q.channel].r_bytes+=8;
                ++q.job->bus.r;q.job->bus.r_bytes+=8;
                if(p->ddr_r_resp) {++channels[q.channel].errors;++q.job->bus.errors;}
            }
            if(p->ddr_r_resp) ++all_ddr.errors;
            CHECK(p->ddr_r_resp==0,"DDR R error");++q.got;
            if(q.got==q.beats) rq.pop_front();
        }
        // Finish this same pre-edge bus sample before accepting completion;
        // a legal last response and DMA completion can share an edge.
        for(unsigned ch=0;ch<2;++ch) {
            if((p->dbg_dma_issue>>ch)&1u) {
                Job* j=active[ch]; CHECK(j && j->issue<0 && j->done<0,"unexpected or duplicate hardware issue");
                j->issue=cyc; ++j->bus.issue; ++channels[ch].issue;
                event("issue",j,cyc);
            }
            if((p->dbg_dma_done>>ch)&1u) {
                Job* j=active[ch]; CHECK(j && j->issue>=0 && j->done<0,"hardware done without unique issue");
                CHECK(j->bus.ar_bytes==j->bytes && j->bus.r_bytes==j->bytes && j->bus.w_bytes==j->bytes,
                      "hardware done before exact payload bytes");
                CHECK(j->bus.aw==j->bus.b && j->bus.aw_bytes==j->bytes && !j->bus.errors,
                      "hardware done before write response conservation");
                j->done=cyc; ++j->bus.done; ++channels[ch].done; last_done[ch]=cyc;
                event("done",j,cyc);
                if(ch==0) {
                    Tensor& t=tensors[j->tensor]; ++t.completed_fill_count; t.completed_fill_bytes+=j->bytes;
                    CHECK(t.completed_fill_count<=t.expected_fill_count && t.completed_fill_bytes<=t.expected_fill_bytes,"tensor fill overflow");
                    if(t.completed_fill_count==t.expected_fill_count) {
                        CHECK(t.completed_fill_bytes==tensor_bytes[j->tensor],"tensor barrier byte mismatch");
                        t.barrier_cycle=cyc;
                        event("barrier_ready",j,cyc,t.completed_fill_count,t.completed_fill_bytes);
                    }
                }
            }
        }
    }
};
Observer* Observer::current=nullptr;

static std::vector<Job> load_jobs(const std::string& path,bool two_hop) {
    std::ifstream f(path);CHECK(f.good(),"cannot open commands TSV");std::string line;
    std::getline(f,line);CHECK(line=="id channel src_iova dst_iova bytes dep tensor chunk","TSV header mismatch");
    std::vector<Job> js;std::set<uint64_t> ids;
    while(std::getline(f,line)) {
        if(line.empty()) continue;
        Job j;std::istringstream s(line);std::string extra;
        CHECK(bool(s>>j.id>>j.channel>>j.src>>j.dst>>j.bytes>>j.dep>>j.tensor>>j.chunk) && !(s>>extra),"invalid TSV row");
        CHECK(j.channel>=0 && j.channel<2 && j.tensor>=0 && j.tensor<6,"TSV role/tensor outside contract");
        CHECK(j.bytes>0 && j.bytes<=4096 && j.bytes%8==0 && j.src%8==0 && j.dst%8==0,"TSV alignment/size mismatch");
        CHECK(ids.insert(j.id).second,"duplicate TSV job ID");
        const uint64_t source=two_hop && j.channel==1?RELAY:SRC;
        const uint64_t dest=two_hop && j.channel==0?RELAY:DST;
        CHECK(j.src>=source && j.src<source+REGION && j.dst>=dest && j.dst<dest+REGION,"TSV address role mismatch");
        const uint64_t off=j.src-source;
        CHECK(j.dst-dest==off && off>=tensor_offset[j.tensor] && off+j.bytes<=tensor_offset[j.tensor]+tensor_bytes[j.tensor],"TSV tensor byte range mismatch");
        CHECK((j.src>>12)==((j.src+j.bytes-1)>>12) && (j.dst>>12)==((j.dst+j.bytes-1)>>12),"seed chunk crosses 4KiB boundary");
        if(!two_hop) CHECK(j.channel==1 && j.dep==-1,"resident direct role/dependency mismatch");
        else if(j.channel==0) CHECK(j.dep==-1,"fill must be source-ready");
        else CHECK(j.dep>=0,"copy must depend on tensor fill");
        js.push_back(j);
    }
    CHECK(f.eof(),"TSV read error");CHECK(js.size()==(two_hop?4618u:2309u),"full package command count mismatch");
    uint64_t cursor[2][6]{};uint64_t bytes[2]={0,0};int64_t final_fill[6]={-1,-1,-1,-1,-1,-1};
    for(const auto& j:js) {
        uint64_t off=j.src-(two_hop && j.channel==1?RELAY:SRC);
        CHECK(off==tensor_offset[j.tensor]+cursor[j.channel][j.tensor],"TSV tensor coverage out of order/duplicate/hole");
        cursor[j.channel][j.tensor]+=j.bytes;bytes[j.channel]+=j.bytes;
        if(two_hop && j.channel==0) final_fill[j.tensor]=int64_t(j.id);
    }
    for(const auto& j:js) if(two_hop && j.channel==1) CHECK(j.dep==final_fill[j.tensor],"copy.dep is not final fill ID of its full tensor");
    CHECK(bytes[1]==USEFUL && bytes[0]==(two_hop?USEFUL:0),"TSV full useful-byte total mismatch");
    for(unsigned t=0;t<6;++t) {
        CHECK(cursor[1][t]==tensor_bytes[t],"destination tensor incomplete");
        CHECK(cursor[0][t]==(two_hop?tensor_bytes[t]:0),"fill tensor incomplete");
    }
    return js;
}

static uint64_t pt_pages=2,pt_init_write64=0,leaf_ptes=0,nonleaf_ptes=0;
static void pt_write(Tb& tb,uint64_t addr,uint64_t value) {
    tb.write64(addr,value);++pt_init_write64;
}
static uint64_t alloc_pt(Tb& tb,uint64_t& next) {
    uint64_t p=next;next+=4096;CHECK(next<0x01000000ULL,"page table allocator escaped low PA metadata");
    ++pt_pages;
    for(uint64_t i=0;i<4096;i+=8) pt_write(tb,p+i,0);return p;
}
static void setup_tables(Tb& tb) {
    uint64_t next=0x100000ULL;
    for(unsigned ch=0;ch<2;++ch) {
        const uint64_t root=ch?0x30000:0x20000, dc=0x10000+(10+ch)*64;
        for(uint64_t i=0;i<4096;i+=8) pt_write(tb,root+i,0);
        for(uint64_t i=0;i<64;i+=8) pt_write(tb,dc+i,0);
        pt_write(tb,dc,1);pt_write(tb,dc+16,ch?0xB000:0xA000);
        pt_write(tb,dc+24,0x8000000000000000ULL|(root>>12));
        const uint64_t l1=alloc_pt(tb,next);
        pt_write(tb,root+8,((l1>>12)<<10)|1);++nonleaf_ptes;
        for(unsigned region=0;region<3;++region) {
            const uint64_t iova[3]={SRC,RELAY,DST}, pa[3]={SRC_PA,RELAY_PA,DST_PA};
            for(uint64_t segment=0;segment<8;++segment) {
                const uint64_t l0=alloc_pt(tb,next), va=iova[region]+segment*0x200000ULL;
                pt_write(tb,l1+((va>>21)&511)*8,((l0>>12)<<10)|1);++nonleaf_ptes;
                for(uint64_t page=0;page<512;++page) {
                    const uint64_t phys=pa[region]+segment*0x200000ULL+page*4096;
                    pt_write(tb,l0+page*8,((phys>>12)<<10)|0xDFULL);++leaf_ptes;
                }
            }
        }
    }
}
static std::vector<uint8_t> load_payload(const std::string& path) {
    std::ifstream f(path,std::ios::binary);CHECK(f.good(),"cannot open payload.bin");
    f.seekg(0,std::ios::end);CHECK(f.tellg()==std::streamoff(PAYLOAD),"payload full size mismatch");f.seekg(0);
    std::vector<uint8_t> b(PAYLOAD);f.read(reinterpret_cast<char*>(b.data()),b.size());
    CHECK(f.good(),"payload read failed");return b;
}
static void terminal_check(Observer& o) {
    Tb& tb=o.tb;tb.check_quiet("finite-transfer-seed");
    for(unsigned i=0;i<64;++i) tb.tick();
    CHECK(tb.top->dbg_dma_job_quiet==3 && tb.top->dbg_in_flight==0,"terminal DMA/TL not quiet");
    CHECK(!tb.top->ddr_ar_valid && !tb.top->ddr_aw_valid && !tb.top->ddr_w_valid && !tb.top->ddr_r_valid && !tb.top->ddr_b_valid,"terminal DDR VALID pending");
    CHECK(tb.mem_q.empty() && tb.caw_q.empty() && tb.cb_q.empty() && o.rq.empty() && o.wq.empty() && o.bq.empty(),"terminal observer/model queue not empty");
    CHECK(tb.top->dbg_cpu_issued==0 && tb.top->dbg_cpu_completed==0 && tb.top->dbg_cpu_quiet && tb.top->dbg_cpu_wr_quiet,"CPU-off violated");
    CHECK(o.all_ddr.ar_bytes==o.all_ddr.r_bytes && o.all_ddr.aw==o.all_ddr.b && o.all_ddr.aw_bytes==o.all_ddr.w_bytes && !o.all_ddr.errors,"terminal all-DDR conservation/error mismatch");
    for(unsigned ch=0;ch<2;++ch) {
        const Stats& s=o.channels[ch];
        CHECK(s.issue==s.done && s.ar_bytes==s.r_bytes && s.ar_bytes==s.w_bytes && s.aw_bytes==s.w_bytes && s.aw==s.b && !s.errors,"terminal channel conservation/error mismatch");
        CHECK(s.tl_accept==s.tl_resp,"terminal TL accept/response mismatch");
    }
    o.quiet_cycle=tb.g_cycle-o.t0;o.event("quiet",nullptr,o.quiet_cycle,3,0);
}
static void write_summary(const std::string& path,const std::string& mode,const Observer& o) {
    std::ofstream f(path);CHECK(f.good(),"cannot open summary.json");
    f<<"{\"schema\":\"ai-fpga-transfer-functional-v1\",\"pass\":true,\"scope\":\"functional_serial_seed_only\",\"source_scope\":\"one_complete_OPT125m_MLP_weight_package_byte_fixture\",\"model_only_metadata\":true,\"mode\":\""<<mode
     <<"\",\"common_t0_absolute_cycle\":"<<o.t0<<",\"quiet_cycle\":"<<o.quiet_cycle
     <<",\"useful_bytes\":"<<USEFUL<<",\"payload_bytes_including_holes\":"<<PAYLOAD
     <<",\"policy_enable\":0,\"cpu_enable\":0,\"nonblock_enable\":0,\"ptw_latency\":"<<o.tb.ptw_latency
     <<",\"mapping\":{\"diagnostic_metadata_only\":true,\"ddt_base\":65536,\"roots\":[131072,196608],\"page_bytes\":4096,\"region_bytes\":"<<REGION
     <<",\"payload_reserved_bytes\":"<<3*REGION<<",\"pte_pages\":"<<pt_pages<<",\"pte_reserved_bytes\":"<<pt_pages*4096
     <<",\"leaf_pte_count\":"<<leaf_ptes<<",\"nonleaf_pte_count\":"<<nonleaf_ptes<<",\"init_write64_count\":"<<pt_init_write64
     <<",\"ddt_initialized_bytes\":128,\"ddt_reserved_bytes\":4096,\"metadata_reserved_bytes\":"<<pt_pages*4096+4096
     <<",\"fallback_metadata_entries\":"<<o.tb.mem.size()<<"}"
     <<",\"stability_samples\":"<<o.stability_samples<<",\"launch_errors\":"<<o.launch_errors<<",\"jobs\":[";
    for(size_t i=0;i<o.jobs.size();++i) {
        const Job& j=o.jobs[i];if(i)f<<',';
        f<<"{\"id\":"<<j.id<<",\"channel\":"<<j.channel<<",\"did\":"<<10+j.channel
         <<",\"src_iova\":"<<j.src<<",\"dst_iova\":"<<j.dst<<",\"bytes\":"<<j.bytes
         <<",\"dep\":"<<j.dep<<",\"tensor\":"<<j.tensor<<",\"chunk\":"<<j.chunk
         <<",\"ready\":"<<j.ready<<",\"channel_ready\":"<<j.channel_ready<<",\"config_start\":"<<j.config_start
         <<",\"issue\":"<<j.issue<<",\"done\":"<<j.done<<",\"next_id\":"<<j.hw_id
         <<",\"next_cycle\":"<<j.next_cycle<<",\"done_id\":"<<j.done_id<<",\"done_reg_cycle\":"<<j.done_reg_cycle<<",\"bus\":";
        stat_json(f,j.bus);f<<'}';
    }
    f<<"],\"tensors\":[";
    for(unsigned t=0;t<6;++t) {
        if(t)f<<',';const Tensor& z=o.tensors[t];
        f<<"{\"tensor\":"<<t<<",\"name\":\""<<tensor_name[t]<<"\",\"offset\":"<<tensor_offset[t]<<",\"bytes\":"<<tensor_bytes[t]
         <<",\"expected_fill_count\":"<<z.expected_fill_count<<",\"expected_fill_bytes\":"<<z.expected_fill_bytes
         <<",\"completed_fill_count\":"<<z.completed_fill_count<<",\"completed_fill_bytes\":"<<z.completed_fill_bytes
         <<",\"barrier_cycle\":"<<z.barrier_cycle<<",\"payload_match\":true}";
    }
    f<<"],\"channels\":[";stat_json(f,o.channels[0]);f<<',';stat_json(f,o.channels[1]);
    f<<"],\"all_ddr\":";stat_json(f,o.all_ddr);
    f<<",\"terminal\":{\"dma_job_quiet\":"<<unsigned(o.tb.top->dbg_dma_job_quiet)
     <<",\"tl_in_flight\":"<<unsigned(o.tb.top->dbg_in_flight)
     <<",\"cpu_quiet\":"<<unsigned(o.tb.top->dbg_cpu_quiet)
     <<",\"cpu_wr_quiet\":"<<unsigned(o.tb.top->dbg_cpu_wr_quiet)
     <<",\"cpu_issued\":"<<o.tb.top->dbg_cpu_issued<<",\"cpu_completed\":"<<o.tb.top->dbg_cpu_completed
     <<",\"memory_r_queue\":"<<o.tb.mem_q.size()<<",\"memory_aw_queue\":"<<o.tb.caw_q.size()
     <<",\"memory_b_queue\":"<<o.tb.cb_q.size()<<",\"observer_r_queue\":"<<o.rq.size()
     <<",\"observer_aw_queue\":"<<o.wq.size()<<",\"observer_b_queue\":"<<o.bq.size()
     <<",\"ddr_valid\":0},\"source_match\":true,\"relay_match\":true,\"destination_match\":true,\"padding_preserved\":true}\n";
    f.flush();CHECK(f.good(),"summary.json write failed");
}

static int run(int argc,char** argv) {
    CHECK(argc==5,"usage: executable two-hop|resident-direct commands.tsv payload.bin existing-output-directory");
    const std::string mode=argv[1],out=argv[4];
    CHECK(mode=="two-hop" || mode=="resident-direct","unknown finite seed mode");
    const bool two_hop=mode=="two-hop";
    auto jobs=load_jobs(argv[2],two_hop);auto payload=load_payload(argv[3]);
    std::vector<uint8_t> touched(PAYLOAD,0);
    for(unsigned t=0;t<6;++t) std::fill(touched.begin()+tensor_offset[t],touched.begin()+tensor_offset[t]+tensor_bytes[t],1);
    Tb tb;tb.cpu_enable=false;tb.cpu_writes=false;tb.policy_partition=false;tb.owner_only=false;
    tb.ds_priority=false;tb.read_admit_mode_val=0;tb.full_batch=false;tb.ptw_latency=1;
    setup_tables(tb);
    std::fill(tb.transfer_dense[0].begin(),tb.transfer_dense[0].end(),0x3c);
    std::copy(payload.begin(),payload.end(),tb.transfer_dense[0].begin());
    std::fill(tb.transfer_dense[1].begin(),tb.transfer_dense[1].end(),0xa7);
    std::fill(tb.transfer_dense[2].begin(),tb.transfer_dense[2].end(),0xd9);
    tb.reset(false);
    Observer observer(tb,jobs,out+"/events.csv");observer.t0=tb.g_cycle;
    Observer::current=&observer;tb.observe_pre_edge=&Observer::hook;
    observer.event("common_start",nullptr,0,PAYLOAD,USEFUL);
    tb.cfg_wr(SEL_V,REG_B_CTL,0);tb.cfg_wr(SEL_A,REG_B_CTL,0);
    std::map<uint64_t,long> completed;
    for(auto& j:jobs) {
        CHECK(!observer.active[j.channel],"channel has more than one active job");
        if(two_hop && j.channel==1) {
            const Tensor& t=observer.tensors[j.tensor];
            CHECK(completed.count(uint64_t(j.dep)) && t.completed_fill_count==t.expected_fill_count &&
                  t.completed_fill_bytes==tensor_bytes[j.tensor] && t.barrier_cycle>=0,"copy issued before whole tensor fill barrier");
            j.ready=t.barrier_cycle;
        }
        j.channel_ready=observer.last_done[j.channel];j.config_start=tb.g_cycle-observer.t0;
        CHECK(j.config_start>=j.ready && j.config_start>=j.channel_ready,"configuration precedes logical/capacity readiness");
        observer.event("ready",&j,j.ready);observer.event("config_start",&j,j.config_start);
        observer.active[j.channel]=&j;
        const bool sel=j.channel==1;
        tb.cfg_wr(sel,REG_CONF,0);tb.cfg_wr(sel,REG_SRC,j.src);tb.cfg_wr(sel,REG_DST,j.dst);tb.cfg_wr(sel,REG_NB,j.bytes);
        j.hw_id=int64_t(tb.trigger(sel));j.next_cycle=tb.g_cycle-observer.t0;
        observer.event("next_id",&j,j.next_cycle,uint64_t(j.hw_id));
        CHECK(j.hw_id>0,"NEXT_ID invalid");
        const long limit=tb.g_cycle+Tb::WATCHDOG_CYCLES;
        while(true) {
            const uint64_t d=tb.cfg_rd(sel,REG_DONE);
            CHECK(tb.g_cycle<=limit,"bounded functional job watchdog expired");
            if(d==uint64_t(j.hw_id)) {j.done_id=int64_t(d);j.done_reg_cycle=tb.g_cycle-observer.t0;break;}
            tb.tick();
        }
        CHECK(j.issue>=j.ready && j.issue>=j.channel_ready && j.done>=j.issue && j.bus.issue==1 && j.bus.done==1,
              "NEXT/DONE register reached without exactly one real issue/done pair");
        CHECK(j.done_reg_cycle>j.done && j.done_id==j.hw_id,"DONE register does not match hardware completion/NEXT_ID");
        observer.event("done_reg",&j,j.done_reg_cycle,uint64_t(j.done_id));
        completed[j.id]=j.done;observer.active[j.channel]=nullptr;
        if(j.id%256==0) std::fprintf(stderr,"functional seed completed job %llu of %zu\n",(unsigned long long)j.id,jobs.size());
    }
    terminal_check(observer);
    CHECK(observer.channels[0].ar_bytes==(two_hop?USEFUL:0) && observer.channels[1].ar_bytes==USEFUL,"final useful hop-byte mismatch");
    for(uint64_t i=0;i<REGION;++i) {
        const uint8_t expected_source=i<PAYLOAD?payload[i]:0x3c;
        const bool useful=i<PAYLOAD && touched[i];
        CHECK(tb.transfer_dense[0][i]==expected_source,"source buffer damaged");
        CHECK(tb.transfer_dense[1][i]==(two_hop && useful?payload[i]:0xa7),"relay useful byte/padding damaged");
        CHECK(tb.transfer_dense[2][i]==(useful?payload[i]:0xd9),"target useful byte/padding damaged");
    }
    std::ofstream bin(out+"/output.bin",std::ios::binary);CHECK(bin.good(),"cannot open output.bin");
    bin.write(reinterpret_cast<const char*>(tb.transfer_dense[2].data()),PAYLOAD);bin.flush();CHECK(bin.good(),"output.bin write failed");
    write_summary(out+"/summary.json",mode,observer);observer.events.flush();CHECK(observer.events.good(),"events flush failed");
    tb.observe_pre_edge=nullptr;Observer::current=nullptr;
    std::printf("PASS finite %s packet: useful=%llu, jobs=%zu, model cycles=%ld; functional only\n",
                mode.c_str(),(unsigned long long)USEFUL,jobs.size(),observer.quiet_cycle);
    return 0;
}
} // namespace transfer_seed

int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);
    try {return transfer_seed::run(argc,argv);}
    catch(const std::exception& e) {
        std::fprintf(stderr,"FAIL finite source transfer seed: %s\n",e.what());return 1;
    }
}
