#include "Vbackend_top.h"
#include "verilated.h"
#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct Job {
    uint64_t length, src, dst;
    unsigned expected_bursts, ar=0, aw=0;
    uint64_t read_bytes=0, write_bytes=0;
};
struct Burst {
    uint64_t addr;
    unsigned beats, index, id, job;
    uint64_t ready_cycle;
    bool error;
};
struct Reply { unsigned id, resp, job; uint64_t ready_cycle; };
struct Hold {
    bool active=false;
    uint64_t a=0, b=0, c=0;
    void check(bool valid, bool ready, uint64_t x, uint64_t y, uint64_t z, const std::string& name) {
        if (active && (!valid || x!=a || y!=b || z!=c))
            throw std::runtime_error(name+" changed while stalled");
        active=valid && !ready; a=x; b=y; c=z;
    }
};

static void need(bool condition, const std::string& text) {
    if (!condition) throw std::runtime_error(text);
}
static uint64_t load64(const std::vector<uint8_t>& memory, uint64_t address) {
    need(address+8<=memory.size(), "read outside memory model");
    uint64_t data=0;
    for (unsigned i=0; i<8; ++i) data|=uint64_t(memory[address+i])<<(8*i);
    return data;
}
static unsigned owner(const std::vector<Job>& jobs, uint64_t address, bool write) {
    for (unsigned i=0; i<jobs.size(); ++i) {
        uint64_t base=write ? jobs[i].dst : jobs[i].src;
        if (address>=base && address<base+jobs[i].length) return i;
    }
    throw std::runtime_error("AXI address outside every supplied descriptor");
}
static void clock_reset(Vbackend_top& d) {
    d.clk_i=0; d.rst_ni=0; d.req_valid_i=0; d.rsp_ready_i=1;
    d.length_i=0; d.src_i=0; d.dst_i=0;
    d.ar_ready_i=0; d.aw_ready_i=0; d.w_ready_i=0;
    d.r_valid_i=0; d.r_last_i=0; d.r_data_i=0; d.r_id_i=0; d.r_resp_i=0;
    d.b_valid_i=0; d.b_id_i=0; d.b_resp_i=0;
    for (unsigned i=0; i<8; ++i) { d.clk_i=0; d.eval(); d.clk_i=1; d.eval(); }
    d.clk_i=0; d.rst_ni=1; d.eval();
}

static void run_case(const std::string& name, std::vector<Job> jobs, bool inject=false, bool stop=false) {
    auto model=std::make_unique<Vbackend_top>();
    auto& d=*model;
    clock_reset(d);
    std::vector<uint8_t> memory(1<<20);
    for (uint64_t p=0; p<memory.size(); ++p)
        memory[p]=uint8_t((p*37)^(p>>3)^(p>>11)^(p>>17)^0xa5);
    std::fill(memory.begin()+0x20000, memory.begin()+0x40000, 0xcc);
    auto expected=memory;
    for (const auto& job:jobs) {
        need(job.src+job.length<=memory.size() && job.dst+job.length<=memory.size(), "invalid test descriptor");
        for (uint64_t i=0; i<job.length; ++i) expected[job.dst+i]=memory[job.src+i];
    }
    std::deque<Burst> reads, writes;
    std::deque<Reply> replies;
    std::deque<unsigned> accepted_jobs;
    Hold hold_ar, hold_aw, hold_w, hold_req, hold_rsp;
    unsigned submitted=0, completed=0, ar_count=0, aw_count=0, rlast_count=0, b_count=0;
    unsigned read_errors=0, write_errors=0, response_errors=0;
    uint64_t read_bytes=0, write_bytes=0, w_beats=0, r_beats=0;
    bool b_error_scheduled=false;
    unsigned peak_r=0,peak_w=0;
    unsigned quiet=0;
    uint64_t cycle=0;
    for (; cycle<50000; ++cycle) {
        d.clk_i=0;
        d.req_valid_i=submitted<jobs.size();
        if (submitted<jobs.size()) {
            const auto& job=jobs[submitted];
            d.length_i=job.length; d.src_i=job.src; d.dst_i=job.dst;
        } else { d.length_i=64; d.src_i=0; d.dst_i=0; }
        d.rsp_ready_i=(cycle%11)!=0;
        d.ar_ready_i=(cycle%5)!=0 && reads.size()<128;
        d.aw_ready_i=(cycle%7)!=2 && writes.size()<128;
        d.w_ready_i=!writes.empty() && (cycle%3)!=0;
        d.r_valid_i=!reads.empty() && cycle>=400 && cycle>=reads.front().ready_cycle;
        d.r_data_i=0; d.r_last_i=0; d.r_id_i=0; d.r_resp_i=0;
        if (d.r_valid_i) {
            const auto& burst=reads.front();
            d.r_data_i=load64(memory,burst.addr+8*burst.index);
            d.r_last_i=burst.index+1==burst.beats;
            d.r_id_i=burst.id;
            d.r_resp_i=burst.error && burst.index==0 ? 2 : 0;
        }
        d.b_valid_i=!replies.empty() && cycle>=12000 && cycle>=replies.front().ready_cycle;
        d.b_id_i=d.b_valid_i ? replies.front().id : 0;
        d.b_resp_i=d.b_valid_i ? replies.front().resp : 0;
        d.eval();

        hold_req.check(d.req_valid_i,d.req_ready_o,d.length_i,d.src_i,d.dst_i,"descriptor");
        hold_ar.check(d.ar_valid_o,d.ar_ready_i,d.ar_addr_o,
                      uint64_t(d.ar_len_o)|(uint64_t(d.ar_size_o)<<8)|(uint64_t(d.ar_burst_o)<<16),
                      uint64_t(d.ar_id_o)|(uint64_t(d.ar_sid_o)<<8),"AR");
        hold_aw.check(d.aw_valid_o,d.aw_ready_i,d.aw_addr_o,
                      uint64_t(d.aw_len_o)|(uint64_t(d.aw_size_o)<<8)|(uint64_t(d.aw_burst_o)<<16),
                      uint64_t(d.aw_id_o)|(uint64_t(d.aw_sid_o)<<8),"AW");
        hold_w.check(d.w_valid_o,d.w_ready_i,d.w_data_o,d.w_strb_o,d.w_last_o,"W");
        hold_rsp.check(d.rsp_valid_o,d.rsp_ready_i,d.rsp_error_o,d.rsp_last_o,0,"backend response");
        bool take_req=d.req_valid_i&&d.req_ready_o, take_rsp=d.rsp_valid_o&&d.rsp_ready_i;
        bool take_ar=d.ar_valid_o&&d.ar_ready_i, take_aw=d.aw_valid_o&&d.aw_ready_i;
        bool take_r=d.r_valid_i&&d.r_ready_o, take_w=d.w_valid_o&&d.w_ready_i;
        bool take_b=d.b_valid_i&&d.b_ready_o;
        uint64_t ar_addr=d.ar_addr_o, aw_addr=d.aw_addr_o, w_data=d.w_data_o;
        unsigned ar_len=d.ar_len_o, aw_len=d.aw_len_o, ar_id=d.ar_id_o, aw_id=d.aw_id_o;
        unsigned ar_size=d.ar_size_o, aw_size=d.aw_size_o, ar_burst=d.ar_burst_o, aw_burst=d.aw_burst_o;
        unsigned w_strb=d.w_strb_o, w_last=d.w_last_o, rsp_error=d.rsp_error_o, rsp_last=d.rsp_last_o;
        unsigned r_resp=d.r_resp_i, b_resp=d.b_resp_i;
        d.clk_i=1; d.eval();

        if (take_req) {
            std::cout<<"{\"event\":\"descriptor\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"index\":"<<submitted<<",\"length\":"<<jobs[submitted].length
                     <<",\"src\":"<<jobs[submitted].src<<",\"dst\":"<<jobs[submitted].dst<<"}\n";
            accepted_jobs.push_back(submitted++);
        }
        if (take_rsp) {
            need(!accepted_jobs.empty(), "unsolicited backend response");
            unsigned index=accepted_jobs.front(); accepted_jobs.pop_front();
            need(rsp_error==(jobs[index].length==0), "response error does not match fixed NO_ERROR_HANDLING/zero-length behavior");
            need(jobs[index].length==0 || rsp_last, "final descriptor response lacks last flag");
            response_errors+=rsp_error; ++completed;
            std::cout<<"{\"event\":\"response\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"index\":"<<index<<",\"error\":"<<rsp_error<<",\"last\":"<<rsp_last<<"}\n";
        }
        if (take_r) {
            auto& burst=reads.front();
            ++r_beats; read_bytes+=8; jobs[burst.job].read_bytes+=8;
            if (r_resp) ++read_errors;
            std::cout<<"{\"event\":\"R\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"descriptor\":"<<burst.job<<",\"address\":"<<burst.addr+8*burst.index
                     <<",\"data\":"<<load64(memory,burst.addr+8*burst.index)<<",\"resp\":"<<r_resp
                     <<",\"last\":"<<(burst.index+1==burst.beats)<<"}\n";
            if (++burst.index==burst.beats) { ++rlast_count; reads.pop_front(); }
        }
        if (take_b) {
            ++b_count; if (b_resp) ++write_errors;
            std::cout<<"{\"event\":\"B\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"descriptor\":"<<replies.front().job<<",\"resp\":"<<b_resp<<"}\n";
            replies.pop_front();
        }
        if (take_w) {
            need(!writes.empty(), "write beat without AW");
            auto& burst=writes.front();
            need(w_last==(burst.index+1==burst.beats), "WLAST disagrees with accepted AWLEN");
            need(burst.addr+8*burst.index+8<=memory.size(), "write outside memory model");
            unsigned bytes=0;
            for (unsigned lane=0; lane<8; ++lane) if (w_strb&(1u<<lane)) {
                memory[burst.addr+8*burst.index+lane]=uint8_t(w_data>>(8*lane)); ++bytes;
            }
            write_bytes+=bytes; jobs[burst.job].write_bytes+=bytes; ++w_beats;
            std::cout<<"{\"event\":\"W\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"descriptor\":"<<burst.job<<",\"address\":"<<burst.addr+8*burst.index
                     <<",\"data\":"<<w_data<<",\"strb\":"<<w_strb<<",\"last\":"<<w_last<<"}\n";
            if (++burst.index==burst.beats) {
                bool error=inject&&!b_error_scheduled;
                b_error_scheduled|=error;
                replies.push_back({burst.id,unsigned(error?2:0),burst.job,cycle+4});
                writes.pop_front();
            }
        }
        if (take_ar) {
            need(ar_size==3 && ar_burst==1 && d.ar_sid_o==10, "unexpected AR size/burst/stream ID");
            need(ar_addr%8==0 && (ar_addr%2048)+(ar_len+1)*8<=2048, "AR violates actual 2048-byte legalizer boundary");
            unsigned index=owner(jobs,ar_addr,false); ++jobs[index].ar;
            reads.push_back({ar_addr,ar_len+1,0,ar_id,index,cycle+2,inject&&ar_count==0}); ++ar_count;
            std::cout<<"{\"event\":\"AR\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"descriptor\":"<<index<<",\"addr\":"<<ar_addr<<",\"len\":"<<ar_len
                     <<",\"size\":"<<ar_size<<",\"id\":"<<ar_id<<"}\n";
        }
        if (take_aw) {
            need(aw_size==3 && aw_burst==1 && d.aw_sid_o==10, "unexpected AW size/burst/stream ID");
            need(aw_addr%8==0 && (aw_addr%2048)+(aw_len+1)*8<=2048, "AW violates actual 2048-byte legalizer boundary");
            unsigned index=owner(jobs,aw_addr,true); ++jobs[index].aw;
            writes.push_back({aw_addr,aw_len+1,0,aw_id,index,cycle, false}); ++aw_count;
            std::cout<<"{\"event\":\"AW\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                     <<",\"descriptor\":"<<index<<",\"addr\":"<<aw_addr<<",\"len\":"<<aw_len
                     <<",\"size\":"<<aw_size<<",\"id\":"<<aw_id<<"}\n";
        }
        peak_r=std::max(peak_r,ar_count-rlast_count);
        peak_w=std::max(peak_w,aw_count-b_count);
        bool drained=submitted==jobs.size() && completed==jobs.size() && accepted_jobs.empty() &&
                     reads.empty() && writes.empty() && replies.empty() && d.busy_o==0;
        quiet=drained ? quiet+1 : 0;
        if (quiet==8) break;
    }
    need(cycle<50000, "finite 50000-cycle watchdog expired");
    need(!d.path16_r_error_o && !d.path16_w_error_o,"Depth16 passive PATH overflow");
    need(d.path16_r_full_o==ar_count && d.path16_w_full_o==aw_count,"Depth16 PATH full count mismatch");
    need(bool(d.path4_r_error_o)==(peak_r>4) && bool(d.path4_w_error_o)==(peak_w>4),"Depth4 validity/inventory mismatch");
    std::cout<<"{\"event\":\"path_capacity\",\"case\":\""<<name<<"\",\"peak_read_bursts\":"<<peak_r<<",\"peak_write_bursts\":"<<peak_w<<",\"depth4_read_error\":"<<unsigned(d.path4_r_error_o)<<",\"depth4_write_error\":"<<unsigned(d.path4_w_error_o)<<",\"depth16_read_error\":"<<unsigned(d.path16_r_error_o)<<",\"depth16_write_error\":"<<unsigned(d.path16_w_error_o)<<",\"depth16_read_full\":"<<d.path16_r_full_o<<",\"depth16_write_full\":"<<d.path16_w_full_o<<"}\n";
    need(memory==expected, "contents or bytes outside requested destination differ");
    need(ar_count==rlast_count && aw_count==b_count, "AXI burst/completion conservation");
    uint64_t expected_bytes=0;
    for (const auto& job:jobs) {
        expected_bytes+=job.length;
        need(job.ar==job.expected_bursts && job.aw==job.expected_bursts, "actual per-descriptor AR/AW count mismatch");
        need(job.read_bytes==job.length && job.write_bytes==job.length, "actual per-descriptor byte mismatch");
    }
    need(read_bytes==expected_bytes && write_bytes==expected_bytes, "total physical bytes mismatch");
    need(!inject || (read_errors==1 && write_errors==1), "injected R/B SLVERR not observed on handshakes");
    std::cout<<"{\"event\":\"case_summary\",\"case\":\""<<name<<"\",\"status\":\"passed\",\"cycles\":"<<cycle
             <<",\"submitted\":"<<submitted<<",\"completed\":"<<completed<<",\"ar\":"<<ar_count<<",\"aw\":"<<aw_count
             <<",\"rlast\":"<<rlast_count<<",\"b\":"<<b_count<<",\"r_beats\":"<<r_beats<<",\"w_beats\":"<<w_beats
             <<",\"read_bytes\":"<<read_bytes<<",\"write_bytes\":"<<write_bytes<<",\"r_slverr\":"<<read_errors
             <<",\"b_slverr\":"<<write_errors<<",\"backend_error_responses\":"<<response_errors
             <<",\"stop_supply_after_last_accepted\":"<<(stop?"true":"false")
             <<",\"contents_and_guards_equal\":true,\"drained\":true,\"descriptors\":[";
    for (unsigned i=0; i<jobs.size(); ++i) {
        if (i) std::cout<<",";
        const auto& job=jobs[i];
        std::cout<<"{\"length\":"<<job.length<<",\"src\":"<<job.src<<",\"dst\":"<<job.dst
                 <<",\"src_page_offset\":"<<(job.src%4096)<<",\"dst_page_offset\":"<<(job.dst%4096)
                 <<",\"ar\":"<<job.ar<<",\"aw\":"<<job.aw<<",\"read_bytes\":"<<job.read_bytes
                 <<",\"write_bytes\":"<<job.write_bytes<<"}";
    }
    std::cout<<"]}\n";
    d.final();
}

static std::vector<Job> packed(uint64_t length, unsigned count, unsigned bursts, uint64_t offset=0) {
    std::vector<Job> jobs;
    for (unsigned i=0; i<count; ++i) jobs.push_back({length,0x10000+offset+i*length,0x20000+offset+i*length,bursts});
    return jobs;
}
int main(int argc, char** argv) {
    Verilated::commandArgs(argc,argv);
    try {
        run_case("control_4096",packed(4096,1,2));
        run_case("long_16384",packed(16384,1,8));
        run_case("long_65536",packed(65536,1,32));
        run_case("long_65536_offset64",packed(65536,1,33,64));
        std::cout<<"{\"event\":\"all_passed\",\"cases\":4}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr<<"FAIL: "<<error.what()<<"\n";return 1;
    }
}
