#include "Vstage_compare.h"
#include <verilated.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <stdexcept>

static vluint64_t ticks = 0;
double sc_time_stamp() { return ticks; }
static constexpr unsigned Depth = QUEUE_DEPTH;
struct Bench {
    Vstage_compare d;
    uint64_t cycles=0, comparisons=0, writes=0, held_words=0, digest=1469598103934665603ULL;
    unsigned cases=0, stale_differences=0;
    const char *scenario="initial";
    void require(bool ok, const char *why) {
        if (!ok) {
            std::fprintf(stderr,"FAIL case=%s cycle=%llu depth=%u hist=%u %s\n",scenario,
                         (unsigned long long)cycles,Depth,HIST_ENABLE,why);
            throw std::runtime_error(why);
        }
    }
    void compare() {
        require(d.equal_o==7,"actual candidate/gold outputs, metadata or histograms differ");
        ++comparisons;
        for (unsigned f=0;f<8;f++) { digest^=d.data_o[f]; digest*=1099511628211ULL; }
        digest^=d.error_o|(d.overflow_o<<1)|(d.small_error_o<<2)|(d.small_overflow_o<<3);
        digest*=1099511628211ULL;
    }
    Bench() {
        d.clk_i=0; d.rst_ni=0; d.clear_i=d.run_i=d.active_i=d.open_i=d.close_i=0;
        d.enter_i=d.leave_i=d.path_arm_i=d.path_zero_i=d.path_abort_i=0; d.time_i=0;
        d.eval(); ++ticks; d.clk_i=1; d.eval(); ++ticks;
        for(unsigned n=0;n<Depth;n++) stale_differences+=d.memory_o[n]!=d.gold_memory_o[n];
        d.rst_ni=1;
        clear();
    }
    void clock() {
        d.clk_i=0; d.eval(); ++ticks; compare();
        std::array<uint64_t,Depth> before{},gold_before{};
        for(unsigned n=0;n<Depth;n++) { before[n]=d.memory_o[n]; gold_before[n]=d.gold_memory_o[n]; }
        const bool write=d.rst_ni&&!d.clear_i&&d.run_i&&!d.bad_o&&d.enter_i;
        const unsigned tail=d.tail_o;
        const uint64_t word=(uint64_t(d.active_i)<<32)|d.time_i;
        d.clk_i=1; d.eval(); ++ticks; ++cycles; compare();
        for(unsigned n=0;n<Depth;n++) {
            const bool changed=write&&tail==n;
            require(d.memory_o[n]==(changed?word:before[n]),"candidate storage write-enable/address/value mismatch");
            require(d.gold_memory_o[n]==(changed?word:gold_before[n]),"gold storage write-enable/address/value mismatch");
            held_words+=!changed;
        }
        writes+=write;
    }
    void neutral() {
        d.enter_i=d.leave_i=d.open_i=d.close_i=d.path_arm_i=d.path_zero_i=d.path_abort_i=0;
        d.run_i=d.active_i=1;
    }
    void clear() { neutral(); d.clear_i=1; clock(); d.clear_i=0; d.eval(); }
    void start(const char *name) { scenario=name; clear(); ++cases; }
    void send(uint32_t now,bool enter=false,bool leave=false,bool active=true,
              bool open=false,bool close=false,bool run=true) {
        d.time_i=now; d.enter_i=enter; d.leave_i=leave; d.active_i=active;
        d.open_i=open; d.close_i=close; d.run_i=run; clock();
    }
    void event(const char *name) {
        std::printf("CASE {\"name\":\"%s\",\"depth\":%u,\"hist\":%u,\"cycle\":%llu,"
                    "\"entered\":%llu,\"full\":%llu,\"carry\":%llu,\"sum\":%llu,"
                    "\"opening\":%llu,\"closing\":%llu,\"count\":%u,\"error\":%u,"
                    "\"small_error\":%u,\"small_overflow\":%u}\n",name,Depth,HIST_ENABLE,
                    (unsigned long long)cycles,(unsigned long long)d.data_o[0],
                    (unsigned long long)d.data_o[1],(unsigned long long)d.data_o[2],
                    (unsigned long long)d.data_o[3],(unsigned long long)d.data_o[5],
                    (unsigned long long)d.data_o[6],unsigned(d.count_o),unsigned(d.error_o),
                    unsigned(d.small_error_o),unsigned(d.small_overflow_o));
    }
};
int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    Verilated::randReset(2); // Independent arbitrary initial words, not a zero-memory proof.
    try {
        Bench b;
        b.require(b.stale_differences>0,"test must include independently initialized stale memory");
        b.start("empty-simultaneous"); b.send(123,true,true);
        b.require(b.d.count_o==0&&b.d.data_o[1]==1&&b.d.data_o[3]==0,"empty enter+leave age0");
        b.send(124,true,true,false); b.require(b.d.data_o[1]==1,"inactive empty completion excluded");
        b.event(b.scenario);

        b.start("full-old-head-and-wrap");
        for(unsigned i=0;i<Depth;i++) b.send(i,true);
        b.require(b.d.count_o==Depth&&b.d.head_o==b.d.tail_o,"full equal head/tail");
        b.send(10000,true,true);
        b.require(b.d.data_o[1]==1&&b.d.data_o[3]==10000,"full pop+push consumes OLD head timestamp");
        for(unsigned i=0;i<Depth;i++) b.send(10001+i,false,true);
        b.require(!b.d.count_o&&!b.d.error_o,"full wave drains");
        for(unsigned wave=0;wave<4;wave++) {
            for(unsigned i=0;i<Depth;i++) b.send(11000+wave*200+i,true,false,(i%2)==0);
            for(unsigned i=0;i<Depth;i++) b.send(11100+wave*200+i,false,true);
        }
        b.require(b.d.data_o[2]==4*(Depth/2),"born flag survives wrap and carry"); b.event(b.scenario);

        b.start("opening-closing-carry"); b.send(0,true,false,false);
        b.send(1,true,false,true,true); b.send(2,false,true);
        b.send(3,true,false); b.send(4,false,true,true,false,true);
        b.require(b.d.data_o[2]==1&&b.d.data_o[5]==2&&b.d.data_o[6]==1,"boundary includes current edge");
        b.send(5,false,true,false); b.require(b.d.count_o==0,"inactive drain"); b.event(b.scenario);

        b.start("run0-ignores-all-inputs"); b.send(0,true);
        const auto count=b.d.count_o; const auto entered=b.d.data_o[0];
        for(unsigned i=0;i<4;i++) b.send(100+i,true,true,false,true,true,false);
        b.require(b.d.count_o==count&&b.d.data_o[0]==entered,"run0 preserves valid metadata");
        b.send(200,false,true); b.event(b.scenario);

        b.start("overflow-and-sticky-write");
        for(unsigned i=0;i<Depth;i++) b.send(i,true);
        const auto tail=b.d.tail_o; b.send(100,true);
        b.require(b.d.error_o&&b.d.count_o==Depth&&b.d.tail_o==tail,"full invalid enter suppressed");
        b.send(101,false,true); b.send(102,true); b.send(103,false,true);
        b.require(b.d.error_o&&b.d.count_o==Depth-1,"sticky error does not gate later legal writes"); b.event(b.scenario);

        b.start("unmatched-and-sticky-write"); b.send(1,false,true);
        b.require(b.d.error_o&&!b.d.count_o,"unmatched return rejected");
        b.send(10,true); b.send(25,false,true);
        b.require(b.d.error_o&&b.d.data_o[3]==15,"legal write after unmatched error"); b.event(b.scenario);

        b.start("clear-reset-priority-and-stale-words");
        for(unsigned i=0;i<Depth;i++) b.send(0x8abcdef0u+i,true,false,(i%2)==0);
        b.d.clear_i=1; b.send(123,true,true,true,true,true);
        b.require(!b.d.count_o&&!b.d.error_o&&!b.d.data_o[0],"clear beats enter/leave");
        b.d.clear_i=0; b.send(999,true,true);
        b.require(b.d.data_o[3]==0,"stale word ignored while empty after clear");
        b.send(1000,true); b.d.rst_ni=0; b.send(2000,true,true,true,true,true);
        b.require(!b.d.count_o&&!b.d.data_o[0]&&!b.d.error_o,"reset beats enter/leave");
        b.d.rst_ni=1; b.send(3000,true,true);
        b.require(b.d.data_o[3]==0,"stale word ignored while empty after reset"); b.event(b.scenario);

        b.start("time-wrap-and-all-bin-edges");
        b.send(0xfffffff0u,true); b.send(0x10,false,true);
        b.require(b.d.data_o[3]==32,"32-bit wrap subtraction");
        for(unsigned bit=0;bit<32;bit++) {
            b.clear(); b.send(0,true); b.send(uint32_t(1)<<bit,false,true);
            b.require(b.d.data_o[3]==(uint32_t(1)<<bit),"32-bit timestamp retained");
        }
        b.clear(); b.send(0,true); b.send(0xffffffffu,false,true); b.event(b.scenario);

        b.start("hist-and-counter-sticky-write");
        for(unsigned i=0;i<9;i++) b.send(i,true,true);
        b.require(b.d.small_error_o,"real narrow entered/full counter overflow");
        if(HIST_ENABLE) b.require(b.d.small_overflow_o&&b.d.small_hist_o[0]==7,"real histogram saturation");
        b.send(40,true); b.send(41,false,true);
        if(HIST_ENABLE) b.require(b.d.small_hist_o[1]==1,"legal memory writes continue after histogram/counter error");
        else b.require(!b.d.small_overflow_o&&!b.d.hist_o[0],"hist0 constant outputs");
        b.event(b.scenario);

        b.start("sum-area-overflow-sticky-write");
        b.send(0,true); b.send(200,false,true); b.send(201,true); b.send(255,false,true);
        b.send(0,true); b.send(2,false,true); // 200+54+2 overflows narrow sum.
        b.require(b.d.small_error_o,"real narrow sum overflow");
        b.send(3,true); b.send(4,false,true);
        b.clear(); for(unsigned i=0;i<300;i++) b.send(i,i==0,false);
        b.require(b.d.small_error_o,"real narrow area overflow"); b.send(301,false,true); b.event(b.scenario);

        b.start("actual-path-abort-close-and-timer"); b.neutral(); b.d.path_arm_i=1; b.clock();
        b.d.path_arm_i=0; b.d.path_zero_i=1;
        for(unsigned i=0;i<3;i++) b.clock();
        b.d.path_abort_i=1; b.clock(); b.d.path_abort_i=0;
        const auto frozen=b.d.path_bin_o; for(unsigned i=0;i<10;i++) b.clock();
        b.require(b.d.path_bin_o==frozen&&!b.d.path_error_o,"actual abort freezes after current edge");
        b.clear(); b.d.path_arm_i=1; b.clock(); b.d.path_arm_i=0; b.d.path_zero_i=1;
        for(unsigned i=0;i<3;i++) b.clock(); b.d.close_i=1; b.clock(); b.d.close_i=0;
        const auto closed=b.d.path_bin_o; for(unsigned i=0;i<5;i++) b.clock();
        b.require(b.d.path_bin_o==closed,"actual close freezes");
        b.clear(); b.d.path_arm_i=1; b.clock(); b.d.path_arm_i=0;
        for(unsigned i=0;i<255;i++) b.clock();
        b.require(!b.d.path_error_o,"timer valid before wrap"); b.clock();
        b.require(b.d.path_error_o&&!b.d.path_overflow_o,"timer wrap invalidates");
        b.d.path_zero_i=1; b.clock(); b.d.path_zero_i=0;
        if(HIST_ENABLE) b.require(b.d.path_bin_o==1,"timer sticky error does not gate stage histogram");
        b.event(b.scenario);

        b.start("randomized-boundaries"); std::mt19937 rng(0x31a501u);
        for(unsigned i=0;i<12000;i++) {
            b.d.clear_i=(rng()%113)==0; b.d.rst_ni=(rng()%211)!=0;
            b.d.path_arm_i=(rng()%127)==0; b.d.path_abort_i=(rng()%131)==0;
            b.d.path_zero_i=(rng()%5)==0;
            b.send(rng(),rng()%2,rng()%2,rng()%2,(rng()%19)==0,(rng()%23)==0,(rng()%7)!=0);
        }
        b.d.rst_ni=1; b.clear(); b.event(b.scenario);
        std::printf("PASS {\"cases\":%u,\"cycles\":%llu,\"gold_comparisons\":%llu,"
                    "\"memory_writes\":%llu,\"memory_hold_word_checks\":%llu,"
                    "\"initial_stale_differences\":%u,\"digest\":\"%016llx\"}\n",b.cases,
                    (unsigned long long)b.cycles,(unsigned long long)b.comparisons,
                    (unsigned long long)b.writes,(unsigned long long)b.held_words,b.stale_differences,
                    (unsigned long long)b.digest);
    } catch(const std::exception&) { return 1; }
    return 0;
}
