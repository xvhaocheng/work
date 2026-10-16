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

    // Observer E2 phase codes (mirror the observer localparams).
    enum : unsigned { PH_IDLE = 0, PH_ARM_WARM = 1, PH_ARM_A = 2,
                      PH_PROBE = 3, PH_FREEZE = 4, PH_ABORT = 5, PH_READY = 6 };

    void set_phase(unsigned p) { d.phase_i = p; d.eval(); }

    // Legal PROBE acceptance: the observer sits in READY on the accept edge and
    // moves to PROBE (the only measuring phase) on the following cycle.
    void probe() {
        d.phase_i = PH_READY; d.eval();
        d.probe_accept_i = 1; tick(1); d.probe_accept_i = 0;
        d.phase_i = PH_PROBE; d.eval();
    }
};

static void t_basic_edges() {
    TB t; t.reset(); t.clear(); t.probe();

    // Issue edge at epoch 100, V owns REQ_TRANS.  The interval is (issue, done],
    // so the issue edge itself is not an occupancy cycle.
    t.d.epoch_i = 100; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1;
    t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    check(t.d.v_main_cycles_o == 0, "issue edge excluded from V main");
    check(t.d.v_comp_cycles_o == 0, "issue edge excluded from V complete");

    // Occupancy cycles 101, 102; select at 102.
    t.d.epoch_i = 101; t.tick(1);
    t.d.epoch_i = 102; t.d.select_v_i = 1; t.tick(1);
    t.d.select_v_i = 0; t.d.eval();

    // Hit then miss in the active interval.
    t.d.lookup_v_i = 1; t.d.lookup_hit_i = 1; t.d.epoch_i = 103; t.tick(1);
    t.d.lookup_hit_i = 0; t.d.epoch_i = 104; t.tick(1);
    t.d.lookup_v_i = 0; t.d.eval();

    // Completion edge at 110, V owns COMPLETE.  The done edge is inside the
    // interval, the issue edge is not.
    t.d.epoch_i = 110; t.d.done_v_i = 1; t.d.ctl_state_i = 4; t.tick(1);
    t.d.done_v_i = 0; t.d.eval();

    check(t.d.flags_o == ((1ULL << 0) | (1ULL << 1) | (1ULL << 2)), "flags issue/select/done");
    check(t.d.issue_epoch_o == 100, "issue epoch");
    check(t.d.first_select_epoch_o == 102, "first select epoch");
    check(t.d.done_epoch_o == 110, "done epoch");
    check(t.d.done_issue_cycles_o == 10, "done-issue cycles");
    check(t.d.first_select_issue_cycles_o == 2, "first select-issue cycles");
    check(t.d.v_main_cycles_o == 4, "V main occupancy excludes the issue edge");
    check(t.d.v_comp_cycles_o == 1, "V complete cycles include the done edge");
    check(t.d.v_select_count_o == 1, "V select count");
    check(t.d.v_hit_count_o == 1, "V hit count");
    check(t.d.v_miss_count_o == 1, "V miss count");
    check(t.d.error_o == 0, "no error on the clean interval");
}

static void t_repeats_and_clear() {
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 10; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.epoch_i = 20; t.d.done_v_i = 1; t.d.ctl_state_i = 4; t.tick(1);
    t.d.done_v_i = 0; t.d.eval();

    // Duplicate issue and duplicate done are sticky errors while PROBE measures.
    t.d.epoch_i = 21; t.d.issue_v_i = 1; t.tick(1); t.d.issue_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 1)) != 0, "duplicate issue error");
    t.d.epoch_i = 22; t.d.done_v_i = 1; t.tick(1); t.d.done_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 3)) != 0, "duplicate done error");
    // ... but they must not rewrite the recorded interval.
    check(t.d.issue_epoch_o == 10, "duplicate issue keeps the issue epoch");
    check(t.d.done_epoch_o == 20, "duplicate done keeps the done epoch");
    check(t.d.done_issue_cycles_o == 10, "duplicate done keeps the duration");
    check(t.d.v_main_cycles_o == 0, "late ctl traffic is not counted after done");

    t.clear();
    check(t.d.error_o == 0, "clear error");
    check(t.d.flags_o == 0, "clear flags");
    check(t.d.done_issue_cycles_o == 0, "clear done cycles");
    check(t.d.v_main_cycles_o == 0, "clear V main");

    // A cleared block accepts a fresh session.
    t.probe();
    check(t.d.flags_o == 0 && t.d.error_o == 0, "fresh probe after CLEAR starts clean");
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

    // Same faults outside the E2_PROBE window are ignored entirely.
    t.reset(); t.clear(); t.probe();
    t.set_phase(TB::PH_ARM_A);
    t.d.response_error_i = 1; t.d.unsupported_i = 1; t.tick(1);
    t.d.response_error_i = 0; t.d.unsupported_i = 0; t.d.eval();
    check(t.d.error_o == 0, "faults outside E2_PROBE are not attributed to the interval");
}

static void t_overflow_small() {
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 1; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    for (int i = 0; i < 300; i++) { t.d.epoch_i++; t.tick(1); }
    check((t.d.error_o & 1u) != 0, "counter overflow sticky small variant");
    // 300 counted cycles: the issue edge is excluded from the interval.
    check(t.d.v_main_cycles_o == (300ULL & 0xFFULL), "8-bit V main counter wrapped");
}

// ---------------------------------------------------------------- snapshots
// Full output snapshot, so FREEZE/ABORT can be checked field by field.
struct E2TSnap {
    uint64_t err, flags, issue_ep, sel_ep, done_ep, done_issue, sel_issue;
    uint64_t vmain, vcomp, vsel, vhit, vmiss;
};

static E2TSnap snap(TB &t) {
    E2TSnap s;
    s.err        = t.d.error_o;
    s.flags      = t.d.flags_o;
    s.issue_ep   = t.d.issue_epoch_o;
    s.sel_ep     = t.d.first_select_epoch_o;
    s.done_ep    = t.d.done_epoch_o;
    s.done_issue = t.d.done_issue_cycles_o;
    s.sel_issue  = t.d.first_select_issue_cycles_o;
    s.vmain      = t.d.v_main_cycles_o;
    s.vcomp      = t.d.v_comp_cycles_o;
    s.vsel       = t.d.v_select_count_o;
    s.vhit       = t.d.v_hit_count_o;
    s.vmiss      = t.d.v_miss_count_o;
    return s;
}

static void check_unchanged(TB &t, const E2TSnap &s, const char *m) {
    check(t.d.error_o == s.err, m);
    check(t.d.flags_o == s.flags, m);
    check(t.d.issue_epoch_o == s.issue_ep, m);
    check(t.d.first_select_epoch_o == s.sel_ep, m);
    check(t.d.done_epoch_o == s.done_ep, m);
    check(t.d.done_issue_cycles_o == s.done_issue, m);
    check(t.d.first_select_issue_cycles_o == s.sel_issue, m);
    check(t.d.v_main_cycles_o == s.vmain, m);
    check(t.d.v_comp_cycles_o == s.vcomp, m);
    check(t.d.v_select_count_o == s.vsel, m);
    check(t.d.v_hit_count_o == s.vhit, m);
    check(t.d.v_miss_count_o == s.vmiss, m);
}

// (issue, done] convention, phase window and out-of-window silence.
static void t_interval_edges() {
    // (a) issue edge excluded, done edge included
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 100; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1;
    t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    check(t.d.v_main_cycles_o == 0, "issue edge is not a V main cycle");
    check(t.d.v_comp_cycles_o == 0, "issue edge is not a V complete cycle");
    t.d.epoch_i = 101; t.d.ctl_state_i = 4; t.d.done_v_i = 1; t.tick(1);
    t.d.done_v_i = 0; t.d.eval();
    check(t.d.v_main_cycles_o == 0, "done edge is not a V main cycle");
    check(t.d.v_comp_cycles_o == 1, "done edge is a V complete cycle");
    check(t.d.done_issue_cycles_o == 1, "one-cycle interval measures 1");

    // (b) issue and done on the same edge: empty (issue, done] interval
    t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 200; t.d.issue_v_i = 1; t.d.done_v_i = 1;
    t.d.ctl_v_i = 1; t.d.ctl_state_i = 4;
    t.tick(1);
    t.d.issue_v_i = 0; t.d.done_v_i = 0; t.d.eval();
    check((t.d.flags_o & 7) == 5, "same-edge issue+done records both flags");
    check(t.d.done_issue_cycles_o == 0, "same-edge issue+done duration is zero");
    check(t.d.v_main_cycles_o == 0 && t.d.v_comp_cycles_o == 0,
          "same-edge issue+done has no occupancy cycle");
    check(t.d.error_o == 0, "same-edge issue+done is not an error");

    // (c) select/lookup on the issue edge are recorded, main/complete are not
    t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 300; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1;
    t.d.select_v_i = 1; t.d.lookup_v_i = 1; t.d.lookup_hit_i = 1;
    t.tick(1);
    t.d.issue_v_i = 0; t.d.select_v_i = 0; t.d.lookup_v_i = 0; t.d.lookup_hit_i = 0;
    t.d.eval();
    check(t.d.v_select_count_o == 1, "select on the issue edge is counted");
    check(t.d.v_hit_count_o == 1, "lookup hit on the issue edge is counted");
    check(t.d.first_select_epoch_o == 300, "select on the issue edge records its epoch");
    check(t.d.first_select_issue_cycles_o == 0, "select on the issue edge has zero delta");
    check(t.d.v_main_cycles_o == 0, "main occupancy still excludes the issue edge");

    // (d) outside E2_PROBE nothing is measured and no error is raised
    t.reset(); t.clear(); t.probe();
    t.set_phase(TB::PH_ARM_A);
    t.d.epoch_i = 400; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1;
    t.d.select_v_i = 1; t.d.lookup_v_i = 1; t.d.lookup_hit_i = 1;
    t.tick(2);
    t.d.issue_v_i = 0; t.d.select_v_i = 0; t.d.lookup_v_i = 0; t.d.lookup_hit_i = 0;
    t.d.eval();
    check(t.d.flags_o == 0, "no flags outside E2_PROBE");
    check(t.d.v_main_cycles_o == 0 && t.d.v_select_count_o == 0 && t.d.v_hit_count_o == 0,
          "no counting outside E2_PROBE");
    check(t.d.error_o == 0, "no error outside E2_PROBE");
}

// FREEZE and ABORT hold every field, error_o included, until the legal CLEAR.
static void t_phase_freeze_and_abort_hold() {
    TB t;

    // ---- build one complete interval inside E2_PROBE ----------------------
    t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 50; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    t.d.epoch_i = 51; t.tick(1);
    t.d.epoch_i = 52; t.d.select_v_i = 1; t.tick(1);
    t.d.select_v_i = 0; t.d.eval();
    t.d.epoch_i = 53; t.d.ctl_state_i = 4; t.d.done_v_i = 1; t.tick(1);
    t.d.done_v_i = 0; t.d.ctl_v_i = 0; t.d.ctl_state_i = 0; t.d.eval();

    check((t.d.flags_o & 7) == 7, "issue/select/done recorded before freeze");
    check(t.d.v_main_cycles_o == 2, "V main cycles before freeze");
    check(t.d.v_comp_cycles_o == 1, "V complete cycles before freeze");
    check(t.d.v_select_count_o == 1, "V select count before freeze");
    check(t.d.done_issue_cycles_o == 3, "interval before freeze");
    check(t.d.v_main_cycles_o + t.d.v_comp_cycles_o == t.d.done_issue_cycles_o,
          "(issue, done] cycles equal the epoch delta");
    check(t.d.error_o == 0, "clean before freeze");

    // ---- FREEZE holds every field, also under unrelated V activity --------
    t.set_phase(TB::PH_FREEZE);
    const E2TSnap frozen = snap(t);

    t.d.epoch_i = 60; t.d.issue_v_i = 1; t.d.ctl_v_i = 1; t.d.ctl_state_i = 1;
    t.d.select_v_i = 1; t.d.lookup_v_i = 1; t.d.lookup_hit_i = 1;
    t.tick(3);
    check_unchanged(t, frozen, "FREEZE holds every field under V traffic");

    t.d.issue_v_i = 0; t.d.select_v_i = 0; t.d.lookup_v_i = 0; t.d.lookup_hit_i = 0;
    t.d.done_v_i = 1; t.d.ctl_state_i = 4; t.d.epoch_i = 61; t.tick(1);
    t.d.done_v_i = 0; t.d.eval();
    check_unchanged(t, frozen, "FREEZE holds every field under a duplicate V done");

    t.d.response_error_i = 1; t.d.unsupported_i = 1; t.tick(1);
    t.d.response_error_i = 0; t.d.unsupported_i = 0;
    t.d.ctl_v_i = 0; t.d.ctl_state_i = 0; t.d.eval();
    check_unchanged(t, frozen, "FREEZE holds every field under faults");

    // ---- the legal CLEAR releases the freeze ------------------------------
    t.clear();
    check(t.d.error_o == 0 && t.d.flags_o == 0, "CLEAR after FREEZE zeroes error and flags");
    check(t.d.v_main_cycles_o == 0 && t.d.v_comp_cycles_o == 0 &&
          t.d.v_select_count_o == 0, "CLEAR after FREEZE zeroes counters");

    // ---- ABORT holds exactly like FREEZE ----------------------------------
    t.probe();
    t.d.epoch_i = 70; t.d.issue_v_i = 1; t.tick(1); t.d.issue_v_i = 0; t.d.eval();
    t.d.epoch_i = 71; t.d.done_v_i = 1; t.tick(1); t.d.done_v_i = 0; t.d.eval();
    t.set_phase(TB::PH_ABORT);
    const E2TSnap aborted = snap(t);

    t.d.epoch_i = 72; t.d.issue_v_i = 1; t.d.done_v_i = 1;
    t.d.select_v_i = 1; t.d.lookup_v_i = 1;
    t.tick(2);
    t.d.issue_v_i = 0; t.d.done_v_i = 0; t.d.select_v_i = 0; t.d.lookup_v_i = 0;
    t.d.eval();
    check_unchanged(t, aborted, "ABORT holds every field under V traffic");

    t.clear();
    check(t.d.error_o == 0 && t.d.flags_o == 0, "CLEAR after ABORT zeroes the record");
    check(t.d.done_issue_cycles_o == 0, "CLEAR after ABORT zeroes the duration");
    // A duplicate probe_accept must not overwrite recorded data (error bit 7).
    t.probe();
    t.d.epoch_i = 80; t.d.issue_v_i = 1; t.tick(1); t.d.issue_v_i = 0; t.d.eval();
    uint64_t keep_issue = t.d.issue_epoch_o;
    t.d.probe_accept_i = 1; t.tick(1); t.d.probe_accept_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 7)) != 0, "duplicate probe_accept flagged");
    check(t.d.issue_epoch_o == keep_issue && t.d.v_main_cycles_o == 0,
          "duplicate probe_accept keeps old data");
}

// A backwards or wrapping epoch must be flagged, never silently wrapped.
static void t_epoch_reverse() {
    // (a) done < issue
    TB t; t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 100; t.d.issue_v_i = 1; t.tick(1); t.d.issue_v_i = 0; t.d.eval();
    t.d.epoch_i = 50; t.d.done_v_i = 1; t.tick(1); t.d.done_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 6)) != 0, "done < issue sets epoch error bit 6");
    check(t.d.done_issue_cycles_o == 0, "done < issue stores no wrapped duration");
    check(t.d.issue_epoch_o == 100 && t.d.done_epoch_o == 50,
          "raw epochs stay observable after a reverse");

    // (b) first_select < issue
    t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 200; t.d.issue_v_i = 1; t.tick(1); t.d.issue_v_i = 0; t.d.eval();
    t.d.epoch_i = 150; t.d.select_v_i = 1; t.tick(1); t.d.select_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 6)) != 0, "first_select < issue sets epoch error bit 6");
    check(t.d.first_select_issue_cycles_o == 0, "first_select < issue stores no wrapped delta");
    check(t.d.first_select_epoch_o == 150, "raw select epoch stays observable");

    // (c) 64-bit epoch wrap
    t.reset(); t.clear(); t.probe();
    t.d.epoch_i = 0xFFFFFFFFFFFFFFFCULL; t.d.issue_v_i = 1; t.tick(1);
    t.d.issue_v_i = 0; t.d.eval();
    t.d.epoch_i = 3; t.d.done_v_i = 1; t.tick(1); t.d.done_v_i = 0; t.d.eval();
    check((t.d.error_o & (1u << 6)) != 0, "epoch wrap sets epoch error bit 6");
    check(t.d.done_issue_cycles_o == 0, "epoch wrap stores no wrapped duration");
    check((t.d.error_o & 1u) == 0, "no counter overflow in the epoch-reverse cases");
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
            t_interval_edges();
            t_phase_freeze_and_abort_hold();
            t_epoch_reverse();
        }
        std::cout << "PASS " << checks << " E2 timing checks (simulation only)" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << checks << " checks: " << e.what() << std::endl;
        return 1;
    }
}
