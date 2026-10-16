#include <cstdint>
class Tb;
static void workflow_preedge(Tb&);
#define main retained_legacy_main
#include "workflow_base.cpp"
#undef main

// Isolated measurement adapter. The only alteration to workflow_base.cpp is
// a call to workflow_preedge(*this) immediately before its rising edge.
static const uint64_t ROW_SRC_IOVA = 0x10000000ULL;
static const uint64_t ROW_DST_IOVA = 0x20000000ULL;
static const uint64_t ROW_SRC_PA = 0x80000000ULL;
static const uint64_t ROW_DST_PA = 0x80200000ULL;
static const uint64_t BG_SRC_IOVA = 0x20000000ULL;
static const uint64_t BG_DST_IOVA = 0x20100000ULL;
static const unsigned ROW_SIZE = 640, ROW_COUNT = 480, BG_COUNT = 2048;
static const long JOB_WATCHDOG = 200000, TOTAL_BUDGET = 20000000;
static long release_at(unsigned row) { return ((long)(row - 3) * 31250 + 8) / 9; }
static long deadline_at(unsigned row) { return (long)(row - 2) * 31250 / 9; }

struct Job {
    uint64_t src = 0, dst = 0, size = 0;
    long ready = 0, issue = -1, first_w = -1, done = -1, progress = 0;
    uint64_t ar = 0, aw = 0, rd_bytes = 0, r_bytes = 0, wr_bytes = 0, b = 0;
};
struct WriteBurst {
    unsigned ch, job, id, len, got;
    uint64_t addr;
    uint64_t burst_strobed_bytes = 0;
};
struct Workflow {
    bool armed = false, steady = false, large = false;
    long init_start = 0, init_end = 0, t0 = 0, quiet = 0;
    unsigned next_v = 0, issued[2] = {0, 0}, completed[2] = {0, 0};
    int active[2] = {-1, -1}, owner[4] = {-1, -1, -1, -1};
    unsigned next_release = 3, next_deadline = 3;
    unsigned consumer_events = 0, consumer_blocked = 0, deadline_events = 0, deadline_failed = 0;
    std::vector<Job> rows = std::vector<Job>(ROW_COUNT);
    std::vector<Job> background = std::vector<Job>(BG_COUNT);
    std::deque<WriteBurst> writes, responses;
    FILE* trace = nullptr;
    Job& job(unsigned c, unsigned j) { return c ? background.at(j) : rows.at(j); }
    Job& current(unsigned c) {
        CHECK(active[c] >= 0, "payload event without an active copy");
        return job(c, (unsigned)active[c]);
    }
    void event(Tb& t, const char* type, unsigned ch, unsigned j,
               unsigned id = 0, uint64_t addr = 0, uint64_t value = 0) {
        std::fprintf(trace, "%ld,%s,%ld,%s,%u,%u,%u,%llu,%llu\n", t.g_cycle,
            steady ? "steady" : "initialization", steady ? t.g_cycle-t0 : t.g_cycle-init_start,
            type, ch, j, id, (unsigned long long)addr, (unsigned long long)value);
    }
    void check_data(Tb& t, const Job& r) {
        for (uint64_t i = 0; i < r.size; ++i)
            CHECK(t.mem.at(r.src+i) == t.mem.at(r.dst+i), "copy destination differs from immutable source");
    }
    void finish(Tb& t, unsigned c) {
        Job& r = current(c);
        CHECK(r.issue >= 0 && r.done < 0 && r.first_w >= r.issue,
              "missing issue/write or duplicate DONE");
        CHECK(r.rd_bytes == r.size && r.r_bytes == r.size && r.wr_bytes == r.size,
              "per-copy AR/R/W payload byte conservation failed");
        CHECK(r.ar > 0 && r.aw > 0 && r.aw == r.b,
              "per-copy AW/B conservation failed");
        check_data(t, r);
        r.done = t.g_cycle; r.progress = t.g_cycle;
        event(t, "dma_done", c, (unsigned)active[c]);
        ++completed[c]; active[c] = -1;
    }
    void start(Tb& t, unsigned c) {
        CHECK(active[c] < 0, "second same-channel copy issued before DONE");
        unsigned j = c ? issued[c] : next_v;
        CHECK(j < (c ? BG_COUNT : ROW_COUNT), "ISSUE beyond finite target");
        CHECK(j == issued[c], "foreground row order skipped or duplicated");
        Job& r = job(c, j);
        r.ready = c ? (j ? background[j-1].done : t0)
                    : (j < 3 ? init_start : t0 + release_at(j));
        CHECK(r.ready <= t.g_cycle, "copy issued before logical ready/release");
        r.issue = t.g_cycle; r.progress = t.g_cycle;
        active[c] = (int)j; ++issued[c]; event(t, "dma_issue", c, j);
    }
    void range(const Job& r, uint64_t addr, unsigned len, bool write) {
        const uint64_t base = write ? r.dst : r.src;
        const uint64_t bytes = (uint64_t)(len+1)*8;
        CHECK(addr >= base && bytes <= r.size && addr-base <= r.size-bytes,
              "physical payload burst outside its copy");
        CHECK((addr >> 12) == ((addr+bytes-1) >> 12), "AXI burst crosses 4KiB boundary");
    }
} workflow;

static void workflow_preedge(Tb& t) {
    auto& w = workflow;
    if (!w.armed) return;
    auto* p = t.top;
    CHECK(!t.cpu_enable && !p->dbg_cpu_active && p->dbg_cpu_issued == 0 &&
          p->dbg_cpu_completed == 0 && p->dbg_cpu_wr_issued == 0 &&
          p->dbg_cpu_amo_issued == 0, "synthetic CPU traffic in CPU-off run");
    CHECK(!t.full_batch && !t.full_era && !t.win_open && !t.win_armed &&
          !t.ds_priority && !t.owner_only && t.read_admit_mode_val == 0,
          "legacy measurement or diagnostic intervention enabled");
    if (w.steady) {
        CHECK(t.g_cycle-w.t0 <= TOTAL_BUDGET, "finite workflow total budget exhausted");
        CHECK(w.issued[1] || t.g_cycle-w.t0 <= JOB_WATCHDOG,
              "background initial issue supply watchdog");
        while (w.next_release < ROW_COUNT && t.g_cycle-w.t0 >= release_at(w.next_release)) {
            const unsigned row = w.next_release++, slot = row % 4;
            CHECK(t.g_cycle-w.t0 == release_at(row), "consumer release was not sampled at exact cycle");
            const int old = w.owner[slot];
            if (old >= 0) {
                if (w.rows[(unsigned)old].done < 0) ++w.consumer_blocked;
                else w.check_data(t, w.rows[(unsigned)old]);
            }
            ++w.consumer_events; w.event(t, "consumer_release", 0, row, 0, slot,
                old < 0 ? 0 : (uint64_t)old+1);
        }
    } else CHECK(t.g_cycle-w.init_start <= TOTAL_BUDGET, "initialization budget exhausted");

    if (p->ddr_ar_valid && p->ddr_ar_ready) {
        const unsigned id = p->ddr_ar_id, port = (id >> 4) & 3;
        CHECK(port == 2 || port == 3, "unexpected DDR master while CPU disabled");
        if (port == 2) {
            const unsigned c = (id >> 2) & 1; Job& r = w.current(c);
            CHECK(p->ddr_ar_size == 3, "non-64-bit payload read");
            w.range(r, p->ddr_ar_addr, p->ddr_ar_len, false);
            ++r.ar; r.rd_bytes += ((uint64_t)p->ddr_ar_len+1)*8; r.progress=t.g_cycle;
            w.event(t, "payload_ar", c, (unsigned)w.active[c], id, p->ddr_ar_addr, p->ddr_ar_len);
        } else w.event(t, "ds_ar", 3, 0, id, p->ddr_ar_addr, p->ddr_ar_len);
    }
    if (p->ddr_aw_valid && p->ddr_aw_ready) {
        const unsigned id = p->ddr_aw_id;
        CHECK(((id >> 4) & 3) == 2, "unexpected DDR write master");
        const unsigned c=(id >> 2)&1; Job& r=w.current(c);
        CHECK(p->ddr_aw_size == 3, "non-64-bit payload write");
        w.range(r, p->ddr_aw_addr, p->ddr_aw_len, true);
        ++r.aw; r.progress=t.g_cycle;
        w.writes.push_back({c,(unsigned)w.active[c],id,(unsigned)p->ddr_aw_len,0,p->ddr_aw_addr});
        w.event(t, "payload_aw", c, (unsigned)w.active[c], id, p->ddr_aw_addr, p->ddr_aw_len);
    }
    if (p->ddr_w_valid && p->ddr_w_ready) {
        CHECK(!w.writes.empty(), "W without recorded AW");
        WriteBurst& b=w.writes.front(); Job& r=w.job(b.ch,b.job);
        const uint64_t addr=b.addr+(uint64_t)b.got*8;
        CHECK(b.got <= b.len && ((b.got == b.len) == (p->ddr_w_last != 0)), "W/LAST length mismatch");
        if (r.first_w < 0) {
            r.first_w=t.g_cycle;
            if (b.ch == 0) {
                const unsigned slot=b.job%4; const int old=w.owner[slot];
                CHECK(b.job < 3 || r.first_w >= w.t0+release_at(b.job), "payload write before slot release");
                if (old >= 0) {
                    CHECK(w.rows[(unsigned)old].done >= 0 && w.rows[(unsigned)old].done <= r.first_w,
                          "slot overwritten before old copy DONE");
                    w.check_data(t,w.rows[(unsigned)old]);
                }
                w.owner[slot]=(int)b.job;
            }
            w.event(t,"payload_first_w",b.ch,b.job,b.id,addr,p->ddr_w_strb);
        }
        for (unsigned i=0;i<8;++i) if (p->ddr_w_strb&(1u<<i)) {
            CHECK(addr+i >= r.dst && addr+i-r.dst < r.size,"strobed write outside copy");
            CHECK((uint8_t)(p->ddr_w_data>>(i*8)) == t.mem.at(r.src+addr+i-r.dst),"payload W data mismatch");
            ++r.wr_bytes; ++b.burst_strobed_bytes;
        }
        r.progress=t.g_cycle; ++b.got;
        if(p->ddr_w_last){
            w.event(t,"payload_w_last",b.ch,b.job,b.id,b.addr,b.burst_strobed_bytes);
            w.responses.push_back(b);w.writes.pop_front();
        }
    }
    if (p->ddr_b_valid && p->ddr_b_ready) {
        CHECK(!w.responses.empty(), "B without completed W burst");
        WriteBurst b=w.responses.front(); w.responses.pop_front();
        CHECK((unsigned)p->ddr_b_id == b.id && p->ddr_b_resp == 0,"B id/error mismatch");
        Job& r=w.job(b.ch,b.job); ++r.b; r.progress=t.g_cycle;
        w.event(t,"payload_b",b.ch,b.job,b.id);
    }
    if (p->ddr_r_valid && p->ddr_r_ready) {
        CHECK(!t.mem_q.empty(),"R without accepted AR"); const auto& b=t.mem_q.front();
        CHECK((unsigned)p->ddr_r_id == b.id && p->ddr_r_resp == 0 &&
              ((b.beat == b.len) == (p->ddr_r_last != 0)),"R id/response/LAST mismatch");
        if (((b.id>>4)&3) == 2) {
            const unsigned c=(b.id>>2)&1;Job& r=w.current(c);
            w.range(r,b.addr,b.len,false);r.r_bytes+=8;r.progress=t.g_cycle;
            CHECK(p->ddr_r_data == t.read64(b.addr+(uint64_t)b.beat*8),"payload R data mismatch");
            if(p->ddr_r_last)
                w.event(t,"payload_r_last",c,(unsigned)w.active[c],b.id,b.addr,(uint64_t)b.beat+1);
        } else if(p->ddr_r_last) w.event(t,"ds_r_last",3,0,b.id,b.addr,b.len);
    }
    const unsigned done=p->dbg_dma_done, issue=p->dbg_dma_issue;
    for(unsigned c=0;c<2;++c) if(done&(1u<<c)) w.finish(t,c);
    for(unsigned c=0;c<2;++c) if(issue&(1u<<c)) w.start(t,c);
    // This is a scheduled prefetch consumer observation, not actual Sobel
    // execution. A missed deadline remains a scientific outcome: no release
    // is shifted and the copy workload continues to natural completion.
    if(w.steady) while(w.next_deadline<ROW_COUNT &&
        t.g_cycle-w.t0>=deadline_at(w.next_deadline)) {
        const unsigned row=w.next_deadline++;
        CHECK(t.g_cycle-w.t0==deadline_at(row),"deadline not observed at exact cycle");
        const Job& r=w.rows[row];
        const bool readable=r.done>=0 && r.done<=t.g_cycle && w.owner[row%4]==(int)row;
        if(readable) w.check_data(t,r); else ++w.deadline_failed;
        ++w.deadline_events;w.event(t,readable?"consumer_deadline_ready":"consumer_deadline_miss",0,row);
    }
    for(unsigned c=0;c<2;++c) if(w.active[c]>=0)
        CHECK(t.g_cycle-w.current(c).progress <= JOB_WATCHDOG,"independent channel progress watchdog");
}

static uint64_t leaf(uint64_t pa) { return ((pa>>12)<<10)|0xDFULL; }
static uint64_t pointer_pte(uint64_t pa) { return ((pa>>12)<<10)|1ULL; }
static void setup_workflow_memory(Tb& t,bool large) {
    t.setup_memory(16);
    // Every PTE that could be read is initialized, including unmapped entries.
    for(uint64_t base : {0x20000ULL,0x30000ULL,0x40000ULL,0x50000ULL,0x60000ULL,0x70000ULL,0x80000ULL})
        for(unsigned i=0;i<512;++i) t.write64(base+(uint64_t)i*8,0);
    t.write64(0x20000,pointer_pte(0x40000));
    t.write64(0x30000,pointer_pte(0x60000));
    if(large){
        t.write64(0x40400,leaf(ROW_SRC_PA));
        t.write64(0x40800,leaf(ROW_DST_PA));
        t.write64(0x60800,leaf(A_SRC_PA_BASE));
    }else{
        t.write64(0x40400,pointer_pte(0x50000));
        t.write64(0x40800,pointer_pte(0x80000));
        for(unsigned p=0;p<75;++p)t.write64(0x50000+(uint64_t)p*8,leaf(ROW_SRC_PA+(uint64_t)p*4096));
        t.write64(0x80000,leaf(ROW_DST_PA));
        t.write64(0x60800,pointer_pte(0x70000));
        for(unsigned p=0;p<16;++p){
            t.write64(0x70000+(uint64_t)p*8,leaf(A_SRC_PA_BASE+(uint64_t)p*4096));
            t.write64(0x70800+(uint64_t)p*8,leaf(A_DST_PA_BASE+(uint64_t)p*4096));
        }
    }
    for(unsigned i=0;i<307200/8;++i)t.write64(ROW_SRC_PA+(uint64_t)i*8,make_test_word(i));
    for(unsigned i=0;i<4096/8;++i)t.write64(ROW_DST_PA+(uint64_t)i*8,page_old(100,i));
}
static void configure_row(Tb& t,unsigned row) {
    t.cfg_wr(SEL_V,REG_CONF,0);t.cfg_wr(SEL_V,REG_SRC,ROW_SRC_IOVA+(uint64_t)row*ROW_SIZE);
    t.cfg_wr(SEL_V,REG_DST,ROW_DST_IOVA+(uint64_t)(row%4)*ROW_SIZE);t.cfg_wr(SEL_V,REG_NB,ROW_SIZE);
    workflow.next_v=row;
}
static void wait_row(Tb& t,unsigned row,uint64_t id) {
    const long start=t.g_cycle;
    while(workflow.rows[row].done<0){
        t.tick();CHECK(t.g_cycle-start <= JOB_WATCHDOG,"V DONE supply watchdog");
    }
    CHECK(t.cfg_rd(SEL_V,REG_DONE)==id,"functional DONE id does not match NEXT_ID");
}
static void dump_job(FILE* f,const Job& r,unsigned j,long origin,const char* phase) {
    std::fprintf(f,"{\"job\":%u,\"phase\":\"%s\",\"src_pa\":%llu,\"dst_pa\":%llu,\"bytes\":%llu,"
        "\"ready_cycle\":%ld,\"issue_cycle\":%ld,\"payload_write_start_cycle\":%ld,\"done_cycle\":%ld,"
        "\"ar\":%llu,\"aw\":%llu,\"rd_bytes\":%llu,\"r_bytes\":%llu,\"wr_bytes\":%llu,\"b\":%llu,"
        "\"ready_absolute\":%ld,\"issue_absolute\":%ld,\"first_w_absolute\":%ld,\"done_absolute\":%ld",
        j,phase,(unsigned long long)r.src,(unsigned long long)r.dst,(unsigned long long)r.size,
        r.ready-origin,r.issue-origin,r.first_w-origin,r.done-origin,(unsigned long long)r.ar,
        (unsigned long long)r.aw,(unsigned long long)r.rd_bytes,(unsigned long long)r.r_bytes,
        (unsigned long long)r.wr_bytes,(unsigned long long)r.b,r.ready,r.issue,r.first_w,r.done);
    if(r.size==ROW_SIZE && j>=3)
        std::fprintf(f,",\"release_cycle\":%ld,\"deadline_cycle\":%ld,\"release_absolute\":%ld,\"deadline_absolute\":%ld",
            release_at(j),deadline_at(j),origin+release_at(j),origin+deadline_at(j));
    std::fputc('}',f);
}
int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);
    if(argc!=4 || (std::string(argv[1])!="p" && std::string(argv[1])!="s") ||
       (std::string(argv[2])!="4k" && std::string(argv[2])!="2m")){
        std::fprintf(stderr,"usage: row_driver p|s 4k|2m OUTPUT_PREFIX\n");return 2;
    }
    try{
        Tb t;auto& w=workflow; w.large=std::string(argv[2])=="2m";
        t.cpu_enable=false;t.cpu_writes=false;t.ds_priority=false;t.policy_partition=std::string(argv[1])=="p";
        t.full_batch=false;t.owner_only=false;t.read_admit_mode_val=0;t.ptw_latency=31;
        t.a_pages=16;t.a_target=BG_COUNT;t.v_transfers=477;t.warmup=0;t.v_gap_cycles=0;
        setup_workflow_memory(t,w.large);t.reset(false);
        CHECK(t.top->dbg_policy_elaborated,"adapter requires known policy-enabled SYSTEM archive");
        const std::string prefix=argv[3];w.trace=std::fopen((prefix+".csv").c_str(),"w");CHECK(w.trace,"cannot open trace");
        std::fprintf(w.trace,"absolute_cycle,phase,phase_cycle,event,channel,job,id,address,value\n");
        w.init_start=t.g_cycle;w.armed=true;
        for(unsigned r=0;r<ROW_COUNT;++r){w.rows[r].src=ROW_SRC_PA+(uint64_t)r*ROW_SIZE;w.rows[r].dst=ROW_DST_PA+(r%4)*ROW_SIZE;w.rows[r].size=ROW_SIZE;}
        for(unsigned j=0;j<BG_COUNT;++j){w.background[j].src=A_SRC_PA_BASE+(j%16)*4096;w.background[j].dst=A_DST_PA_BASE+(j%16)*4096;w.background[j].size=4096;}
        for(unsigned r=0;r<3;++r){configure_row(t,r);const uint64_t id=t.trigger(SEL_V);wait_row(t,r,id);}
        t.check_quiet("row-initialization");w.init_end=t.g_cycle;
        CHECK(w.writes.empty() && w.responses.empty(),"initialization observer queues not empty");
        w.event(t,"initialization_quiet",0,0,(unsigned)t.top->dbg_dma_job_quiet,
            (uint64_t)t.top->dbg_in_flight,
            (uint64_t)(!t.mem_q.empty())+(!t.caw_q.empty())+(!t.cb_q.empty()));
        t.configure_batch(SEL_A,BG_SRC_IOVA,BG_DST_IOVA,BG_COUNT,16,0);
        configure_row(t,3);w.t0=t.g_cycle;w.steady=true;t.measurement_phase=true;
        t.start_batch(SEL_A);
        for(unsigned r=3;r<ROW_COUNT;++r){
            if(r!=3)configure_row(t,r);
            while(t.g_cycle-w.t0<release_at(r))t.tick();
            const uint64_t id=t.trigger(SEL_V);wait_row(t,r,id);
        }
        while(t.g_cycle-w.t0<=deadline_at(ROW_COUNT-1))t.tick();
        const long a_wait=t.g_cycle;
        while(w.completed[1]<BG_COUNT){t.tick();CHECK(t.g_cycle-a_wait<=TOTAL_BUDGET,"background natural completion budget");}
        bool err,complete,stopped,active,enabled;
        t.read_batch_ctl(SEL_A,err,complete,stopped,active,enabled);
        CHECK(!err && complete && !stopped && !active && enabled,"A finite batch flags invalid");
        CHECK(t.cfg_rd(SEL_A,REG_B_DONE)==BG_COUNT && t.cfg_rd(SEL_A,REG_B_ISSUED)==BG_COUNT,"A register counts differ");
        t.check_quiet("row-workflow");w.quiet=t.g_cycle;
        CHECK(w.issued[0]==ROW_COUNT && w.completed[0]==ROW_COUNT && w.issued[1]==BG_COUNT && w.completed[1]==BG_COUNT,"finite copy counts disagree");
        CHECK(w.writes.empty() && w.responses.empty() && w.consumer_events==477 &&
              w.deadline_events==477,"observer queues/releases/deadlines incomplete");
        CHECK(t.top->dbg_in_flight==0 && t.top->dbg_dma_job_quiet==3 &&
              t.top->dbg_cpu_quiet && t.top->dbg_cpu_wr_quiet &&
              t.mem_q.empty() && t.caw_q.empty() && t.cb_q.empty(),"final hardware/model quiet readback failed");
        w.event(t,"final_quiet",0,0,(unsigned)t.top->dbg_dma_job_quiet,
            (uint64_t)t.top->dbg_in_flight,
            (uint64_t)(!t.mem_q.empty())+(!t.caw_q.empty())+(!t.cb_q.empty()));
        for(unsigned i=0;i<307200/8;++i)CHECK(t.read64(ROW_SRC_PA+(uint64_t)i*8)==make_test_word(i),"frame source changed");
        for(unsigned p=0;p<16;++p)for(unsigned i=0;i<PAGE_BEATS;++i){
            CHECK(t.read64(A_SRC_PA_BASE+(uint64_t)p*4096+i*8)==page_word(p+1,i),"background source changed");
            CHECK(t.read64(A_DST_PA_BASE+(uint64_t)p*4096+i*8)==page_word(p+1,i),"background final page mismatch");
        }
        for(unsigned i=2560;i<4096;++i)CHECK(t.mem.at(ROW_DST_PA+i)==(uint8_t)(page_old(100,i/8)>>((i%8)*8)),"destination guard bytes changed");
        w.event(t,"guard_checks_pass",0,0);
        FILE* f=std::fopen((prefix+".json").c_str(),"w");CHECK(f,"cannot open result");
        unsigned misses=0;for(unsigned r=3;r<ROW_COUNT;++r)if(w.rows[r].done-w.t0>deadline_at(r))++misses;
        std::fprintf(f,"{\"schema\":\"actual-dma-row-workflow-v1\",\"mode\":\"%s\",\"mapping\":\"%s\",\"cpu_enable\":0,"
            "\"ds_priority\":false,\"init_start_absolute\":%ld,\"init_end_absolute\":%ld,\"initialization_cycles\":%ld,"
            "\"prepared_t0_absolute\":%ld,\"preparation_cycles\":%ld,\"quiet_cycle\":%ld,\"deadline_misses\":%u,"
            "\"consumer_blocked_at_release\":%u,\"consumer_deadline_failures\":%u,\"consumer_deadline_events\":%u,"
            "\"v_count\":480,\"v_steady_count\":477,\"a_count\":2048,\"quiet\":true,\"rows\":[",
            argv[1],argv[2],w.init_start,w.init_end,w.init_end-w.init_start,w.t0,w.t0-w.init_end,w.quiet-w.t0,misses,w.consumer_blocked,w.deadline_failed,w.deadline_events);
        for(unsigned r=0;r<ROW_COUNT;++r){if(r)std::fputc(',',f);dump_job(f,w.rows[r],r,r<3?w.init_start:w.t0,r<3?"initialization":"steady");}
        std::fprintf(f,"],\"background\":[");for(unsigned j=0;j<BG_COUNT;++j){if(j)std::fputc(',',f);dump_job(f,w.background[j],j,w.t0,"steady");}
        std::fprintf(f,"]}\n");std::fclose(f);
        f=std::fopen((prefix+".completions.json").c_str(),"w");CHECK(f,"cannot open completions");std::fputc('[',f);
        for(unsigned r=3;r<ROW_COUNT;++r){if(r!=3)std::fputc(',',f);std::fprintf(f,"{\"row\":%u,\"payload_write_start_cycle\":%ld,\"done_cycle\":%ld}",r,w.rows[r].first_w-w.t0,w.rows[r].done-w.t0);}std::fprintf(f,"]\n");std::fclose(f);
        std::fclose(w.trace);w.trace=nullptr;
        std::printf("ROW-WORKFLOW-INTEGRITY: PASS mode=%s mapping=%s deadline_misses=%u consumer_blocked=%u init_cycles=%ld preparation_cycles=%ld\n",argv[1],argv[2],misses,w.consumer_blocked,w.init_end-w.init_start,w.t0-w.init_end);
        return 0;
    }catch(const std::exception& e){if(workflow.trace){std::fclose(workflow.trace);workflow.trace=nullptr;}
        std::fprintf(stderr,"ROW-WORKFLOW-INTEGRITY: FAIL %s\n",e.what());return 1;}
}
