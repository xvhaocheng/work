// Directed Verilator test for dma_research_e2_timing.sv.
// Simulation only; no board claim and no execution claim.
#include "Vdma_research_e2_timing.h"
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

struct TB {
    Vdma_research_e2_timing d;

    void tick(unsigned n = 1) {
        while (n--) {
            d.clk_i = 0; d.eval();
            d.clk_i = 1; d.eval();
            d.clk_i = 0; d.eval();
        }
    }

    void reset() {
        d.clear_i = 0; d.probe_accept_i = 0; d.phase_i = 0; d.epoch_i = 0;
        d.issue_v_i = 0; d.done_v_i = 0; d.select_v_i = 0;
        d.ctl_v_i = 0; d.ctl_state_i = 0;
        d.lookup_v_i = 0; d.lookup_hit_i = 0;
        d.response_error_i = 0; d.unsupported_i = 0;
        d.rst_ni = 0; tick(2); d.rst_ni = 1; tick(1);
    }

    void clear() { d.clear_i = 1; tick(1); d.clear_i = 0; d.eval(); }
    void probe() { d.probe_accept_i = 1; tick(1); d.probe_accept_i = 0; d.eval(); }
};

static void t_basic_edges() {
    TB t; t.reset(); t.clear(); t.probe();

    // Issue edge at epoch 100, V owns REQ_TRANS.
    t.d.epoch_i = 100; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1;
    t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();

    // Occupancy cycles 101, 102; select at 102.
    t.d.epoch_i = 101; t.tick(1);
    t.d.epoch_i = 102; t.d.select_v_i = 1; t.tick(1);
    t.d.select_v_i = 0; t.d.eval();

    // Hit then miss in the active interval.
    t.d.lookup_v_i = 1; t.d.lookup_hit_i = 1; t.d.epoch_i = 103; t.tick(1);
    t.d.lookup_hit_i = 0; t.d.epoch_i = 104; t.tick(1);
    t.d.lookup_v_i = 0; t.d.eval();

    // Completion edge at 110, V owns COMPLETE.
    t.d.epoch_i = 110; t.d.done_v_i = 1; t.d.ctl_state_i = 4; t.tick(1);
    t.d.done_v_i = 0; t.d.eval();

    check(t.d.flags_o == ((1ULL << 0) | (1ULL << 1) | (1ULL << 2)), "flags issue/select/done");
    check(t.d.issue_epoch_o == 100, "issue epoch");
    check(t.d.first_select_epoch_o == 102, "first select epoch");
    check(t.d.done_epoch_o == 110, "done epoch");
    check(t.d.done_issue_cycles_o == 10, "done-issue cycles");
    check(t.d.first_select_issue_cycles_o == 2, "first select-issue cycles");
    check(t.d.v_main_cycles_o == 5, "V main occupancy");
    check(t.d.v_comp_cycles_o == 1, "V complete cycles");
    check(t.d.v_select_count_o == 1, "V select count");
    check(t.d.v_hit_count_o == 1, "V hit count");
    check(t.d.v_miss_count_o == 1, "V miss count");
}

static void t_repeats_and_clear() {
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 10; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.epoch_i = 20; t.d.done_v_i = 1; t.d.ctl_state_i = 4; t.tick(1);
    t.d.done_v_i = 0; t.d.eval();

    // Duplicate issue and duplicate done are sticky errors.
    t.d.epoch_i = 21; t.d.issue_v_i = 1; t.tick(1); t.d.issue_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 1)) != 0, "duplicate issue error");
    t.d.epoch_i = 22; t.d.done_v_i = 1; t.tick(1); t.d.done_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 3)) != 0, "duplicate done error");

    t.clear();
    check(t.d.error_o == 0, "clear error");
    check(t.d.flags_o == 0, "clear flags");
    check(t.d.done_issue_cycles_o == 0, "clear done cycles");
    check(t.d.v_main_cycles_o == 0, "clear V main");
}

static void t_done_without_issue_and_faults() {
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 30; t.d.done_v_i = 1; t.tick(1); t.d.done_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 2)) != 0, "done without issue error");

    t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 40; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    t.d.response_error_i = 1; t.tick(1); t.d.response_error_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 4)) != 0, "response error during active interval");
    t.d.unsupported_i = 1; t.tick(1); t.d.unsupported_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 5)) != 0, "unsupported during active interval");
}

static void t_overflow_small() {
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 1; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    for (int i = 0; i < 300; i++) { t.d.epoch_i++; t.tick(1); }
    check((t.d.error_o & 1u) != 0, "counter overflow sticky small variant");
    check(t.d.v_main_cycles_o == (301ULL & 0xFFULL), "8-bit V main counter wrapped");
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    bool small = (argc > 1 && std::string(argv[1]) == "small");
    try {
        if (small) {
            t_overflow_small();
        } else {
            t_basic_edges();
            t_repeats_and_clear();
            t_done_without_issue_and_faults();
        }
        std::cout << "PASS " << checks << " E2 timing checks (simulation only)" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << checks << " checks: " << e.what() << std::endl;
        return 1;
    }
}
