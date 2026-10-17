#include "Vstage_top.h"
#include <verilated.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <stdexcept>

static unsigned bucket(uint32_t age) {
    if (!age) return 0;
    unsigned result = 0;
    uint64_t upper = 1;
    while (upper <= age && result < 31) { ++result; upper <<= 1; }
    return result;
}
struct Birth { uint32_t time; bool active; };
struct Bench {
    Vstage_top d;
    std::deque<Birth> pending;
    std::array<uint32_t,32> expected{};
    uint64_t entered=0, full=0, carry=0, sum=0, max=0, area=0;
    unsigned opening=0, closing=0, steps=0;
    bool bad=false;
    Bench() { d.rst_ni=0; clock(); d.rst_ni=1; clear(); }
    void require(bool condition, const char *message) {
        if (!condition) { std::fprintf(stderr,"step=%u %s\n",steps,message); throw std::runtime_error(message); }
    }
    void clock() { d.clk_i=0; d.eval(); d.clk_i=1; d.eval(); }
    void clear() {
        d.enter_i=d.leave_i=d.open_i=d.close_i=d.path_zero_i=d.path_arm_i=0;
        d.clear_i=1; clock(); d.clear_i=0; d.run_i=d.active_i=1;
        pending.clear(); expected.fill(0); entered=full=carry=sum=max=area=opening=closing=0; bad=false;
        d.eval(); require(!d.error_o && !d.overflow_o && !d.small_overflow_o && !d.path_error_o,"clear flags");
        for (unsigned i=0;i<32;i++) require(d.hist_o[i]==0 && d.small_hist_o[i]==0,"clear bins");
    }
    void send(uint32_t now, bool enter, bool leave, bool active=true, bool open=false, bool close=false, bool run=true) {
        d.time_i=now; d.enter_i=enter; d.leave_i=leave; d.active_i=active;
        d.open_i=open; d.close_i=close; d.run_i=run;
        if (run) {
            const size_t before=pending.size();
            const bool matched=leave && (before || enter);
            const bool invalid=(leave && !matched) || (enter && before==4 && !leave);
            if (invalid) bad=true;
            else {
                const uint32_t age=before ? now-pending.front().time : 0;
                const bool whole=before ? pending.front().active : active;
                if (active) {
                    area+=before;
                    if (enter) ++entered;
                    if (matched) {
                        if (whole) { ++full; sum+=age; max=std::max<uint64_t>(max,age); ++expected[bucket(age)]; }
                        else ++carry;
                    }
                }
                if (leave && before) pending.pop_front();
                if (enter && !(leave && !before)) pending.push_back({now,active});
                if (open) opening=pending.size();
                if (close) closing=pending.size();
            }
        }
        clock(); ++steps;
        require(bool(d.error_o)==bad,"legacy stage validity differs from logical transaction model");
        const uint64_t values[8]={entered,full,carry,sum,max,opening,closing,area};
        for (unsigned field=0;field<8;field++) require(d.data_o[field]==values[field],"legacy stage statistic mismatch");
        uint64_t count=0;
        for (unsigned i=0;i<32;i++) {
            if (d.hist_o[i]!=expected[i]) {
                std::fprintf(stderr,"bin=%u actual=%u expected=%u\n",i,unsigned(d.hist_o[i]),expected[i]);
                require(false,"histogram differs from logical completed transaction ages");
            }
            count+=d.hist_o[i];
        }
        require(count==full && !d.overflow_o,"production-width bin/full conservation");
    }
};
int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    try {
        Bench b;
        // External stage clock stamps permit full 32-bit age boundaries in bounded cycles.
        for (uint32_t age: {0u,1u,2u,3u,0xffffffffu}) {
            b.clear(); b.send(0,true,false); b.send(age,false,true);
            std::printf("BOUNDARY age=%u bin=%u actual=%u\n",age,bucket(age),b.d.hist_o[bucket(age)]);
        }
        for (unsigned bit=1;bit<32;bit++) for (int delta=-1;delta<=1;delta++) {
            const uint32_t age=(uint32_t(1)<<bit)+delta;
            b.clear(); b.send(0,true,false); b.send(age,false,true);
            std::printf("BOUNDARY age=%u bin=%u actual=%u\n",age,bucket(age),b.d.hist_o[bucket(age)]);
        }
        // Empty simultaneous enter/leave, all bins, wrap-safe timestamp subtraction.
        b.clear(); b.send(10,true,true); b.send(0xfffffff0u,true,false); b.send(0x10u,false,true);
        // A full queue may pop and push in the same cycle. Then drain accepted work.
        b.clear(); for (unsigned i=0;i<4;i++) b.send(i,true,false);
        b.send(4,true,true); for (unsigned i=0;i<4;i++) b.send(5+i,false,true);
        // Carry-in/out and boundary-cycle inclusion are checked against birth membership.
        b.clear(); b.send(0,true,false,false); b.send(1,false,false,true,true);
        b.send(2,false,true); b.send(3,true,false); b.send(4,false,false,true,false,true);
        b.send(5,false,true,false); b.send(6,true,true,false); b.send(7,true,true,true,false,false,false);
        // Queue capacity and unmatched-return invalidation are sticky, and clear recovers.
        b.clear(); for (unsigned i=0;i<5;i++) b.send(i,true,false);
        b.send(5,false,true); b.clear(); b.send(0,false,true); b.send(1,true,true);
        // Narrow test-only counter width forces a real saturated bin; full count does not overflow.
        b.clear(); for (unsigned i=0;i<8;i++) b.send(i,true,true);
        b.require(b.d.small_hist_o[0]==7 && b.d.small_overflow_o && b.d.data_o[1]==8,"saturation and overflow");
        b.send(8,true,true); b.require(b.d.small_hist_o[0]==7 && b.d.small_overflow_o,"sticky saturation");
        // Actual observer reduction propagates the four read-stage overflow bits to PATH error.
        b.clear(); b.d.path_arm_i=1; b.clock(); b.d.path_arm_i=0; b.d.path_zero_i=1;
        for (unsigned i=0;i<8;i++) b.clock();
        b.require(b.d.path_overflow_o==0xf && b.d.path_error_o,"overflow mask and PATH invalidation");
        b.d.path_zero_i=0; b.clock(); b.require(b.d.path_error_o,"sticky PATH error");
        // A reduced 8-bit observer timer wraps in 256 cycles, with no traffic or histogram error.
        b.clear(); b.d.path_arm_i=1; b.clock(); b.d.path_arm_i=0;
        for (unsigned i=0;i<255;i++) b.clock();
        b.require(!b.d.path_error_o,"timer before wrap"); b.clock();
        b.require(b.d.path_error_o && !b.d.path_overflow_o,"timer wrap invalidation");
        b.clear();
        std::printf("PASS actual stage/path boundaries, conservation, capacity, carry, saturation, timer; modeled_steps=%u\n",b.steps);
    } catch (const std::exception &e) { std::fprintf(stderr,"FAIL %s\n",e.what()); return 1; }
    return 0;
}
