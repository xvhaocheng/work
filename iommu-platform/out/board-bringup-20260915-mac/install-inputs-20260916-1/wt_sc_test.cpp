#include "Vwt_sc_test_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <initializer_list>

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    Vwt_sc_test_top d;
    auto tick = [&]() { d.clk=0; d.eval(); d.clk=1; d.eval(); d.clk=0; d.eval(); };
    d.rst_n=0; d.req=0; d.aw_ready=0; d.w_ready=0; d.b_valid=0;
    tick(); tick(); d.rst_n=1; tick();
    int total=0, failed=0;
    // Consecutive transactions deliberately change offset; do not reset between cases.
    for (int delay : {0, 3, 17}) for (int shape=0; shape<3; ++shape)
    for (int success=0; success<2; ++success) for (int split=0; split<3; ++split) {
        int offset = shape==1 ? 4 : 0;
        d.offset=offset; d.size=shape==2 ? 3 : 2; d.exokay=success;
        d.req=1; d.b_valid=0;
        bool accepted=false, aw=false, w=false, b=false, got=false;
        int ready_at=-1;
        uint64_t actual=0;
        for (int cycle=0; cycle<150 && !got; ++cycle) {
            d.aw_ready = cycle >= (split==1 ? 7 : 0);
            d.w_ready = cycle >= (split==2 ? 7 : 0);
            d.b_valid = ready_at>=0 && cycle>=ready_at && !b;
            d.eval();
            bool rq_hs=d.req_ack;
            bool aw_hs=d.aw_valid && d.aw_ready;
            bool w_hs=d.w_valid && d.w_ready;
            bool b_hs=d.b_valid && d.b_ready;
            if (aw_hs) { aw=true; d.b_id=d.aw_id; }
            if (w_hs) w=true;
            if (aw && w && ready_at<0) ready_at=cycle+1+delay;
            if (d.atomic_valid) { got=true; actual=d.result; }
            tick();
            if (rq_hs) { accepted=true; d.req=0; }
            if (b_hs) b=true;
        }
        uint64_t expected=success ? 0 : (UINT64_C(1) << (offset*8));
        bool pass=accepted && got && actual==expected;
        ++total;
        if (!pass) {
            ++failed;
            std::printf("FAIL size=%d offset=%d success=%d Bdelay=%d split=%d got=%d result=%016llx expected=%016llx\n",
                int(d.size),offset,success,delay,split,got,
                (unsigned long long)actual,(unsigned long long)expected);
        }
        d.req=0; d.b_valid=0; tick(); tick();
    }
    std::printf("SC response regression: %d cases, %d failures\n",total,failed);
    return failed ? 1 : 0;
}
