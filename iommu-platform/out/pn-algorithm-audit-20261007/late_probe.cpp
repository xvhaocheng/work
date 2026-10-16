// Audit-only temporal sweep. Reuse the production TL testbench and RTL model.
#define main original_composition_test_main
#include "../../tests/composition_tl_test.cpp"
#undef main

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    std::puts("n,cold_v,offer_cycle,leaf_release_cycle,a_response_cycle,v_response_cycle,v_fast,a_fills,v_fills");
    for (int cold = 0; cold < 2; ++cold) {
        for (int offer = 0; offer <= 12; ++offer) {
            for (int n = 0; n < 2; ++n) {
                Tb t;
                t.setup_memory();
                t.reset(n, true, 10);
                t.warm_leaf(10, 0x10000000ULL, 0x80000000ULL, 0);
                t.warm_leaf(11, 0x20001000ULL, 0x90001000ULL, 1);
                t.drain_quiet();
                t.fills.clear();
                t.start_cold_a_and_hold(0x20000000ULL, 1);
                t.hold_ptw_r = false;
                bool at_leaf = false;
                for (int k = 0; k < 1000; ++k) {
                    t.tick();
                    if (t.p_ar_fire_last && t.ptw_rd.addr == 0x70000ULL) {
                        at_leaf = true;
                        break;
                    }
                }
                CHECK_AT(at_leaf, "A final-level PTE request not reached");
                int ar = -1, vr = -1, fast = 0, accepts = 0;
                bool finished = false;
                for (int cycle = 0; cycle < 1000; ++cycle) {
                    t.hold_ptw_r = cycle < 4;
                    if (cycle == offer)
                        t.set_req(10, cold ? 0x10003000ULL : 0x10000000ULL,
                                  0x2, 0, true);
                    t.tick();
                    if (t.req_fire) {
                        ++accepts;
                        fast += t.req_fire_fast;
                        t.set_req(10, 0, 0, 0, false);
                    }
                    while (!t.resp_q.empty()) {
                        Resp r = t.resp_q.front();
                        t.resp_q.pop_front();
                        CHECK_AT(!r.error, "unexpected fault");
                        if (r.bank == 1) {
                            CHECK_AT(ar < 0 && r.spaddr == 0x90000000ULL,
                                     "duplicate or incorrect A response");
                            ar = cycle;
                        } else {
                            CHECK_AT(vr < 0 && r.spaddr ==
                                     (cold ? 0x80002000ULL : 0x80000000ULL),
                                     "duplicate or incorrect V response");
                            vr = cycle;
                        }
                    }
                    if (ar >= 0 && vr >= 0 && t.quiet_s) {
                        finished = true;
                        break;
                    }
                }
                CHECK_AT(finished && accepts == 1, "lost request/response or nonquiescent TL");
                CHECK_AT(t.count_fills(11) == 1, "duplicate A PTW/fill");
                CHECK_AT(t.count_fills(10) == cold, "unexpected V PTW/fill count");
                t.check_all_fills(true);
                std::printf("%d,%d,%d,4,%d,%d,%d,%d,%d\n", n, cold,
                            offer, ar, vr, fast, t.count_fills(11), t.count_fills(10));
            }
        }
    }
    return 0;
}
