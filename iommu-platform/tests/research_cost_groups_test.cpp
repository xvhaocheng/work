// Standalone regression test for the fullstats cost-group helper.
// Build with Verilator 4.038; no board claim, no formal claim.
#include "Vdma_research_cost_groups.h"
#include "verilated.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

static int checks = 0;
static void check(bool c, const char *m) {
    if (!c) throw std::runtime_error(m);
    checks++;
}

struct Bench {
    Vdma_research_cost_groups d;

    Bench() { reset(); }

    void tick(unsigned n = 1) {
        while (n--) {
            d.clk_i = 0; d.eval();
            d.clk_i = 1; d.eval();
            d.clk_i = 0; d.eval();
        }
    }

    void reset() {
        d.clear_i = 0; d.commit_i = 0;
        d.main_i = 0; d.comp_i = 0; d.dur_i = 0; d.req_i = 0; d.miss_i = 0;
        d.rst_ni = 0; tick(2); d.rst_ni = 1; tick(1);
    }

    void commit(uint32_t main, uint32_t comp, uint32_t dur, uint32_t req, uint32_t miss) {
        d.clear_i = 0;
        d.main_i = main; d.comp_i = comp; d.dur_i = dur; d.req_i = req; d.miss_i = miss;
        d.commit_i = 1;
        tick(1);
        d.commit_i = 0; d.eval();
    }

    void clear() {
        d.clear_i = 1; d.commit_i = 0;
        d.main_i = 0; d.comp_i = 0; d.dur_i = 0; d.req_i = 0; d.miss_i = 0;
        tick(1);
        d.clear_i = 0; d.eval();
    }

    uint64_t count(int g)    { return (uint64_t)d.count_o[g]; }
    uint64_t sum_main(int g) { return (uint64_t)d.sum_main_o[g]; }
    uint64_t sum_comp(int g) { return (uint64_t)d.sum_comp_o[g]; }
    uint64_t sum_dur(int g)  { return (uint64_t)d.sum_dur_o[g]; }
    uint64_t sum_req(int g)  { return (uint64_t)d.sum_req_o[g]; }
    uint64_t sum_miss(int g) { return (uint64_t)d.sum_miss_o[g]; }
    uint64_t min_main(int g) { return (uint64_t)d.min_main_o[g]; }
    uint64_t max_main(int g) { return (uint64_t)d.max_main_o[g]; }
    uint64_t min_comp(int g) { return (uint64_t)d.min_comp_o[g]; }
    uint64_t max_comp(int g) { return (uint64_t)d.max_comp_o[g]; }
    uint64_t min_dur(int g)  { return (uint64_t)d.min_dur_o[g]; }
    uint64_t max_dur(int g)  { return (uint64_t)d.max_dur_o[g]; }
    uint64_t total()         { return (uint64_t)d.total_o; }
    uint16_t err()           { return (uint16_t)d.err_o; }
};

static Bench *g_bench = nullptr;

static void t_all_groups()
{
    Bench &b = *g_bench;
    b.reset();
    for (int g = 0; g <= 4; g++) {
        b.commit(10 + g, 1, 12 + g, 4, g);
    }
    b.commit(7, 0, 7, 0, 0); // group5

    check(b.total() == 6, "total commits");
    check(b.err() == 0, "no error");
    for (int g = 0; g <= 4; g++) {
        check(b.count(g) == 1, "group count");
        check(b.sum_main(g) == (uint64_t)(10 + g), "group sum_main");
        check(b.sum_comp(g) == 1, "group sum_comp");
        check(b.sum_dur(g) == (uint64_t)(12 + g), "group sum_dur");
        check(b.sum_req(g) == 4, "group sum_req");
        check(b.sum_miss(g) == (uint64_t)g, "group sum_miss");
        check(b.min_main(g) == (uint64_t)(10 + g), "group min_main");
        check(b.max_main(g) == (uint64_t)(10 + g), "group max_main");
        check(b.min_comp(g) == 1, "group min_comp");
        check(b.max_comp(g) == 1, "group max_comp");
        check(b.min_dur(g) == (uint64_t)(12 + g), "group min_dur");
        check(b.max_dur(g) == (uint64_t)(12 + g), "group max_dur");
    }
    check(b.count(5) == 1, "group5 count");
    check(b.sum_main(5) == 7, "group5 sum_main");
    check(b.sum_comp(5) == 0, "group5 sum_comp");
    check(b.sum_dur(5) == 7, "group5 sum_dur");
    check(b.sum_req(5) == 0, "group5 sum_req");
    check(b.sum_miss(5) == 0, "group5 sum_miss");
    check(b.min_main(5) == 7 && b.max_main(5) == 7, "group5 main min/max");
    check(b.min_comp(5) == 0 && b.max_comp(5) == 0, "group5 comp min/max");
    check(b.min_dur(5) == 7 && b.max_dur(5) == 7, "group5 dur min/max");
}

static void t_contiguous_commits()
{
    Bench &b = *g_bench;
    b.reset();
    for (int i = 0; i < 5; i++) {
        b.d.main_i = 100 + i; b.d.comp_i = 1; b.d.dur_i = 110 + i;
        b.d.req_i = 4; b.d.miss_i = 0;
        b.d.clear_i = 0;
        b.d.commit_i = 1;
        b.tick(1);
    }
    b.d.commit_i = 0; b.d.eval();

    check(b.total() == 5, "contiguous total");
    check(b.count(0) == 5, "contiguous group0 count");
    check(b.sum_main(0) == 100+101+102+103+104, "contiguous sum_main");
    check(b.sum_dur(0) == 110+111+112+113+114, "contiguous sum_dur");
    check(b.min_main(0) == 100, "contiguous min_main");
    check(b.max_main(0) == 104, "contiguous max_main");
    check(b.err() == 0, "contiguous no error");
}

static void t_illegal_percopy()
{
    Bench &b = *g_bench;
    b.reset();
    b.commit(10, 1, 10, 4, 5);
    check(((b.err() >> 2) & 1) == 1, "miss>req flagged");
    b.commit(10, 10, 15, 4, 0);
    check(((b.err() >> 3) & 1) == 1, "main+comp>dur flagged");
    check(((b.err() >> 2) & 1) == 1, "bit2 sticky");
}

static void t_narrow_overflow_8bit()
{
    Bench &b = *g_bench;
    b.reset();
    for (int i = 0; i < 256; i++) b.commit(0, 0, 0, 0, 0);
    check(b.count(5) == 0, "8-bit count wraps to 0");
    check(b.total() == 0, "8-bit total wraps to 0");
    check(((b.err() >> 0) & 1) == 1, "count overflow sticky");

    b.reset();
    b.commit(300, 0, 300, 4, 0);
    check(((b.err() >> 1) & 1) == 1, "sum overflow flagged");
    check(b.sum_main(0) == (300 & 0xFF), "sum_main wraps");
    check(b.sum_dur(0) == (300 & 0xFF), "sum_dur wraps");

    b.commit(10, 10, 15, 4, 5);
    check(((b.err() >> 2) & 1) == 1, "illegal miss flagged");
    check(((b.err() >> 3) & 1) == 1, "illegal main+comp flagged");

    // 33-bit main+comp overflow: 0xFFFFFFFF + 1 > any 32-bit duration.
    b.reset();
    b.commit(0xFFFFFFFFu, 1u, 0xFFFFFFFFu, 4, 0);
    check(((b.err() >> 3) & 1) == 1, "33-bit main+comp overflow invalid");
}

static void t_empty_reset_and_clear_priority()
{
    Bench &b = *g_bench;
    b.reset();

    // Reset/all-zero: every field of every group is zero.
    for (int g = 0; g < 6; g++) {
        check(b.count(g) == 0, "reset count zero");
        check(b.sum_main(g) == 0, "reset sum_main zero");
        check(b.sum_comp(g) == 0, "reset sum_comp zero");
        check(b.sum_dur(g) == 0, "reset sum_dur zero");
        check(b.sum_req(g) == 0, "reset sum_req zero");
        check(b.sum_miss(g) == 0, "reset sum_miss zero");
        check(b.min_main(g) == 0 && b.max_main(g) == 0, "reset main min/max zero");
        check(b.min_comp(g) == 0 && b.max_comp(g) == 0, "reset comp min/max zero");
        check(b.min_dur(g) == 0 && b.max_dur(g) == 0, "reset dur min/max zero");
    }
    check(b.total() == 0, "reset total zero");
    check(b.err() == 0, "reset err zero");

    // Clear has priority over commit on the same edge.
    b.d.clear_i = 1; b.d.commit_i = 1;
    b.d.main_i = 10; b.d.comp_i = 1; b.d.dur_i = 11;
    b.d.req_i = 4; b.d.miss_i = 0;
    b.tick(1);
    b.d.clear_i = 0; b.d.commit_i = 0; b.d.eval();
    check(b.total() == 0, "clear beats commit total");
    check(b.count(0) == 0, "clear beats commit count");
    check(b.sum_main(0) == 0, "clear beats commit sum_main");
    check(b.err() == 0, "clear beats commit no error");

    // Empty groups remain all-zero after another group commits.
    b.commit(20, 2, 22, 4, 0);
    for (int g = 1; g < 6; g++) {
        check(b.count(g) == 0, "empty group count zero");
        check(b.sum_main(g) == 0, "empty group sum_main zero");
        check(b.sum_comp(g) == 0, "empty group sum_comp zero");
        check(b.sum_dur(g) == 0, "empty group sum_dur zero");
        check(b.sum_req(g) == 0, "empty group sum_req zero");
        check(b.sum_miss(g) == 0, "empty group sum_miss zero");
        check(b.min_main(g) == 0 && b.max_main(g) == 0, "empty group main min/max zero");
        check(b.min_comp(g) == 0 && b.max_comp(g) == 0, "empty group comp min/max zero");
        check(b.min_dur(g) == 0 && b.max_dur(g) == 0, "empty group dur min/max zero");
    }

    // Reset clears a nonempty aggregate.
    b.reset();
    check(b.count(0) == 0, "reset clears count");
    check(b.sum_main(0) == 0, "reset clears sum_main");
    check(b.total() == 0, "reset clears total");
}

static void t_narrow_independent_overflow_accumulated()
{
    Bench &b = *g_bench;

    // sum_main accumulated overflow: 200 + 100 > 255.
    b.reset();
    b.commit(200, 0, 200, 4, 0);
    check(((b.err() >> 1) & 1) == 0, "sum_main first no overflow");
    b.commit(100, 0, 100, 4, 0);
    check(((b.err() >> 1) & 1) == 1, "sum_main accumulated overflow flagged");
    check(b.sum_main(0) == (uint64_t)((200 + 100) & 0xFF), "sum_main accumulated wrap");

    // sum_comp accumulated overflow: 200 + 100 > 255.
    b.reset();
    b.commit(0, 200, 200, 4, 0);
    check(((b.err() >> 1) & 1) == 0, "sum_comp first no overflow");
    b.commit(0, 100, 100, 4, 0);
    check(((b.err() >> 1) & 1) == 1, "sum_comp accumulated overflow flagged");
    check(b.sum_comp(0) == (uint64_t)((200 + 100) & 0xFF), "sum_comp accumulated wrap");

    // sum_req accumulated overflow without count overflow: 255 + 255 > 255.
    b.reset();
    b.commit(0, 0, 0, 255, 0);
    check(((b.err() >> 1) & 1) == 0, "sum_req first no overflow");
    b.commit(0, 0, 0, 255, 0);
    check(((b.err() >> 1) & 1) == 1, "sum_req accumulated overflow flagged");
    check(b.sum_req(5) == (uint64_t)((255 + 255) & 0xFF), "sum_req accumulated wrap");

    // sum_miss accumulated overflow: 255 + 255 > 255.
    b.reset();
    b.commit(0, 0, 0, 255, 255);
    check(((b.err() >> 1) & 1) == 0, "sum_miss first no overflow");
    b.commit(0, 0, 0, 255, 255);
    check(((b.err() >> 1) & 1) == 1, "sum_miss accumulated overflow flagged");
    check(b.sum_miss(5) == (uint64_t)((255 + 255) & 0xFF), "sum_miss accumulated wrap");
}

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    bool small = (argc > 1 && std::string(argv[1]) == "small");
    Bench b;
    g_bench = &b;
    try {
        if (small) {
            t_narrow_overflow_8bit();
            t_narrow_independent_overflow_accumulated();
        } else {
            t_all_groups();
            t_empty_reset_and_clear_priority();
            t_contiguous_commits();
            t_illegal_percopy();
        }
        std::cout << "PASS " << checks << " fullstats cost-group checks (simulation only)" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << checks << " checks: " << e.what() << std::endl;
        return 1;
    }
}
