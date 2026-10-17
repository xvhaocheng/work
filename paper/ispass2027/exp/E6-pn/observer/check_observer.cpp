// Independent absolute-cycle event scoreboard for the actual standalone DUT.
#include "Vdma_pn_observer.h"
#include "verilated.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

static uint64_t checks = 0;
static unsigned scenarios = 0;
static void require(bool condition, const std::string &message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}

struct TB {
    Vdma_pn_observer dut;
    uint64_t cycle = 0;
    int64_t first_a = -1, first_u = -1, last_a = -1, last_u = -1;
    uint64_t ref_n10 = 0, ref_n11 = 0;
    unsigned v = 0, a = 1;
    bool armed = false, has_a = false, reference = true;

    TB() {
        dut.clk_i = 0; dut.rst_ni = 0;
        dut.accepted_arm_i = 0; dut.has_a_i = 0;
        dut.victim_i = 0; dut.aggressor_i = 1;
        clear_inputs(); clock();
        dut.rst_ni = 1; clock();
        require(dut.status_o == 0 && !dut.capture_busy_o, "reset has no active capture");
    }
    void clear_inputs() {
        dut.issue_i = 0; dut.done_i = 0; dut.batch_active_i = 0;
        dut.obs_active_i = 0; dut.v_target_done_i = 0;
        dut.accepted_stop_i = 0; dut.all_quiet_i = 0;
        dut.n_accept_i = 0; dut.n_did_i = 10;
    }
    void clock() {
        dut.clk_i = 0; dut.eval();
        dut.clk_i = 1; dut.eval();
        dut.clk_i = 0; dut.eval();
        ++cycle;
    }
    void arm(bool with_a, unsigned victim = 0, unsigned aggressor = 1) {
        clear_inputs();
        dut.has_a_i = with_a; dut.victim_i = victim; dut.aggressor_i = aggressor;
        dut.accepted_arm_i = 1; dut.all_quiet_i = 1;
        clock(); dut.accepted_arm_i = 0;
        has_a = with_a; v = victim; a = aggressor; armed = true;
        first_a = first_u = last_a = last_u = -1;
        ref_n10 = ref_n11 = 0;
        require(dut.capture_busy_o && !(dut.status_o & 1), "ARM makes capture busy");
        require(!dut.aown_cycles_o && !dut.union_cycles_o && !dut.n_did10_o && !dut.n_did11_o,
                "ARM clears durations and N counts");
    }
    void step(unsigned issue = 0, unsigned done = 0, unsigned batch = 0,
              bool active = false, bool target = false, bool stop = false,
              bool quiet = false, bool n_accept = false, unsigned did = 10) {
        clear_inputs();
        dut.issue_i = issue; dut.done_i = done; dut.batch_active_i = batch;
        dut.obs_active_i = active; dut.v_target_done_i = target;
        dut.accepted_stop_i = stop; dut.all_quiet_i = quiet;
        dut.n_accept_i = n_accept; dut.n_did_i = did;
        const bool live = armed && !(dut.status_o & 1);
        clock();
        if (reference && live) {
            const unsigned selected = (1U << v) | (has_a ? 1U << a : 0);
            if (has_a && (issue & (1U << a)) && first_a < 0) first_a = cycle;
            if ((issue & selected) && first_u < 0) first_u = cycle;
            if (has_a && (done & (1U << a))) last_a = cycle;
            if (done & selected) last_u = cycle;
            if (active && n_accept && did == 10) ++ref_n10;
            if (active && n_accept && did == 11) ++ref_n11;
        }
        if (reference) {
            const uint64_t expected_a = first_a >= 0 && last_a >= 0 ? last_a - first_a : 0;
            const uint64_t expected_u = first_u >= 0 && last_u >= 0 ? last_u - first_u : 0;
            require(dut.aown_cycles_o == expected_a, "A duration from independent absolute edges");
            require(dut.union_cycles_o == expected_u, "union from independent min issue/max done");
            require(dut.n_did10_o == ref_n10 && dut.n_did11_o == ref_n11,
                    "N counts from absolute trace and original active gate");
        }
        require((dut.status_o >> 8) == 0, "199 reserved bits remain zero");
        require(bool(dut.capture_busy_o) == (armed && !(dut.status_o & 1)),
                "capture busy only until freeze");
        if (dut.status_o & 0xf8) require(!(dut.status_o & 6), "any error invalidates both durations");
        std::cout << "{\"event_cycle\":" << cycle << ",\"fixture_only\":true,\"source_board_cases\":0,"
                  << "\"issue\":" << issue << ",\"done\":" << done << ",\"batch\":" << batch
                  << ",\"active\":" << active << ",\"target\":" << target << ",\"stop\":" << stop
                  << ",\"quiet\":" << quiet << ",\"n_accept\":" << n_accept << ",\"did\":" << did
                  << ",\"aown\":" << dut.aown_cycles_o << ",\"union\":" << dut.union_cycles_o
                  << ",\"status\":" << dut.status_o << "}\n";
    }
    void finish(const std::string &name, uint64_t status) {
        require(dut.status_o == status, name + " exact final status");
        require(!dut.capture_busy_o, name + " frozen clears capture busy");
        ++scenarios;
        std::cout << "{\"scenario\":\"" << name << "\",\"fixture_only\":true,"
                  << "\"source_board_cases\":0,\"cycles_driven\":" << cycle
                  << ",\"aown\":" << dut.aown_cycles_o << ",\"union\":" << dut.union_cycles_o
                  << ",\"n10\":" << dut.n_did10_o << ",\"n11\":" << dut.n_did11_o
                  << ",\"status\":" << dut.status_o << "}\n";
    }
#ifndef PN_DISABLED
    // Endpoint injection only: public-flat-rw changes the actual packed state.
    // NumPorts4 layout, low bit offsets from state_t declaration. No test port
    // or timestamp storage is added to production RTL. Finite traces use none.
    void inject(unsigned low, unsigned width, uint64_t value) {
        auto &bits = dut.dma_pn_observer__DOT__gen_enabled__DOT__state_q;
        for (unsigned i = 0; i < width; ++i) {
            const unsigned pos = low + i;
            const uint32_t mask = uint32_t(1) << (pos % 32);
            bits[pos / 32] = (bits[pos / 32] & ~mask) |
                (((value >> i) & 1) ? mask : 0);
        }
        dut.eval();
    }
#endif
};

#ifndef PN_DISABLED
static void successful_traces() {
    {
        TB t;
        t.step(3, 0, 3, true, false, false, false, true, 10);
        t.step(0, 3, 3, true, false, false, false, true, 11);
        require(!t.dut.capture_busy_o && !t.dut.union_cycles_o && !t.dut.n_did10_o,
                "pre-ARM warmup entirely excluded");
        t.arm(true);
        for (unsigned i = 0; i < 5; ++i) t.step(); // HPM/CPU before issue excluded.
        t.step(2, 0, 2, false, false, false, false, true, 10);
        t.step(0, 2, 2);
        t.step(3, 0, 3, false, false, false, false, true, 10); // open edge excluded.
        for (unsigned i = 0; i < 3; ++i) t.step(0, 0, 3, true, false, false, false, true, 10);
        t.step(0, 1, 3, true, false, false, false, true, 11);
        t.step(1, 2, 3, true, false, false, false, false, 99); // cross-lane edges.
        t.step(2, 0, 3, true);
        t.step(0, 1, 3, true, true, false, false, true, 11); // close edge included.
        for (unsigned i = 0; i < 7; ++i) t.step(0, 0, 2, false, false, false, false, true, 10);
        t.step(0, 0, 2, false, false, true);
        t.step(0, 2, 2);
        for (unsigned i = 0; i < 9; ++i) t.step(); // quiet lag is not duration.
        t.step(0, 0, 0, false, false, false, true);
        t.finish("A-first-refill-close-STOP-and-quiet-tail", 7);
        const uint64_t a = t.dut.aown_cycles_o, u = t.dut.union_cycles_o;
        t.step(3, 3, 3, true, true, true, true, true, 99);
        require(t.dut.aown_cycles_o == a && t.dut.union_cycles_o == u && t.dut.status_o == 7,
                "frozen capture ignores later bus noise");
        t.arm(false); // Clear every prior flag/counter, then independent Solo.
        t.step(1, 0, 1);
        t.step(0, 1, 1, true, true);
        t.step(0, 0, 0, false, false, true, true);
        t.finish("ARM-clear-then-Solo", 5);
    }
    {
        TB t; t.arm(true);
        t.step(1, 0, 1);
        for (unsigned i = 0; i < 4; ++i) t.step(0, 0, 1, true);
        t.step(2, 1, 3, true, true); // A first issue at final V done.
        t.step(0, 0, 2, false, false, true);
        for (unsigned i = 0; i < 8; ++i) t.step(0, 0, 2);
        t.step(0, 2, 2);
        t.step(0, 0, 0, false, false, false, true);
        t.finish("A-after-V-cross-lane-same-edge", 7);
        require(t.dut.union_cycles_o > t.dut.aown_cycles_o, "different starts use actual union");
    }
    {
        TB t; t.arm(true);
        t.step(2, 0, 2);
        t.step(1, 2, 3); // Last A done is before the final V done.
        for (unsigned i = 0; i < 10; ++i) t.step(0, 0, 3, true);
        t.step(0, 1, 3, true, true);
        t.step(0, 0, 0, false, false, true, true);
        t.finish("A-last-done-before-V-union-ends-at-V", 7);
        require(t.dut.aown_cycles_o == 1 && t.dut.union_cycles_o == 12,
                "union uses latest lane done, not assumed A last");
    }
    {
        TB t; t.arm(true);
        t.step(3, 0, 3);
        t.step(0, 0, 3, true);
        t.step(0, 3, 3, true, true);
        for (unsigned i = 0; i < 21; ++i) t.step();
        require(t.dut.capture_busy_o && !t.dut.status_o, "quiet without STOP cannot freeze");
        t.step(0, 0, 0, false, false, true, true);
        t.finish("both-lanes-same-edges-delayed-STOP", 7);
        require(t.dut.aown_cycles_o == 2 && t.dut.union_cycles_o == 2, "no CPU tail added");
    }
    {
        TB t; t.arm(false);
        t.step(3, 0, 3); // Unselected A activity is ignored in Solo.
        t.step(0, 3, 3, true, true);
        t.step(0, 0, 0, false, false, true, false);
        require(t.dut.capture_busy_o, "allquiet required after empty batches");
        t.step(0, 0, 0, false, false, false, true);
        t.finish("Solo-union-is-V-and-nonquiet-gate", 5);
    }
    {
        TB t; t.arm(true, 2, 3);
        t.dut.victim_i = 0; t.dut.aggressor_i = 1; t.dut.has_a_i = 0;
        t.step(12, 0, 12);
        t.step(0, 12, 12, true, true);
        t.step(0, 0, 0, false, false, true, true);
        t.finish("ARM-latches-roles-and-hasA", 7);
    }
    {
        TB t; t.arm(false);
        t.step(1, 0, 1);
        t.step(0, 0, 0, false, false, true, true);
        require(t.dut.capture_busy_o && !t.dut.status_o, "pending bit blocks false quiet freeze");
        t.step(0, 1, 1, true, true);
        t.step(0, 0, 1, false, false, false, true);
        require(t.dut.capture_busy_o, "batch active blocks freeze even pending0");
        t.step(0, 0, 0, false, false, false, true);
        t.finish("pending-and-batch-inactive-gates", 5);
    }
    {
        TB t; t.arm(true);
        t.step(3, 0, 3, false, false, false, false, true, 10);
        t.step(0, 0, 3, true, false, false, false, false, 99); // False probe, unknown DID ignored.
        t.step(0, 0, 3, true, false, false, false, true, 10);
        t.step(0, 3, 3, true, true, false, false, true, 11);
        t.step(0, 0, 0, false, false, true, true, true, 11);
        t.finish("N-real-accept-open-excluded-close-included", 7);
        require(t.dut.n_did10_o == 1 && t.dut.n_did11_o == 1, "only exact original V window");
    }
}

static void malformed_traces() {
    for (unsigned kind = 0; kind < 8; ++kind) {
        TB t; t.reference = false; t.arm(false);
        std::string name;
        switch (kind) {
        case 0: name = "zero-events-failed-STOP"; break;
        case 1: name = "same-lane-issue-and-done";
            t.step(1, 1, 1, true, true); break;
        case 2: name = "duplicate-issue";
            t.step(1, 0, 1); t.step(1, 0, 1, true); t.step(0, 1, 1, true, true); break;
        case 3: name = "orphan-done"; t.step(0, 1, 1); break;
        case 4: name = "missing-V-target";
            t.step(1, 0, 1); t.step(0, 1, 1, true); break;
        case 5: name = "forged-target-without-done";
            t.step(0, 0, 0, true, true); break;
        case 6: name = "V-edge-after-target";
            t.step(1, 0, 1); t.step(0, 1, 1, true, true); t.step(0, 1, 1); break;
        case 7: name = "event-with-batch-inactive";
            t.step(1); t.step(0, 1, 1, true, true); break;
        }
        t.step(0, 0, 0, false, false, true, true);
        t.finish(name, 0x21);
    }
    {
        TB t; t.arm(true, 0, 0); t.reference = false;
        require(t.dut.status_o == 0x20, "same roles rejected at ARM");
        t.step(0, 0, 0, false, false, true, true);
        t.finish("invalid-ARM-roles", 0x21);
    }
    {
        TB t; t.arm(false); t.arm(false); t.reference = false;
        require(t.dut.status_o == 0x20, "ARM while capture busy invalidates new record");
        t.step(0, 0, 0, false, false, true, true);
        t.finish("ARM-during-live-capture", 0x21);
    }
    {
        TB t; t.arm(false); t.reference = false;
        t.dut.accepted_arm_i = 1; t.dut.issue_i = 1; t.dut.batch_active_i = 1;
        t.dut.all_quiet_i = 0; t.clock(); t.dut.accepted_arm_i = 0;
        require(t.dut.status_o == 0x20 && !t.dut.union_cycles_o,
                "nonquiet ARM/same-edge issue invalid and not counted");
        t.step(0, 0, 0, false, false, true, true);
        t.finish("nonquiet-ARM-events-excluded", 0x21);
    }
    {
        TB t; t.arm(true);
        t.step(3, 0, 3);
        t.step(0, 3, 3, true, true, false, false, true, 99);
        t.step(0, 0, 0, false, false, true, true);
        t.finish("N-unexpected-DID-invalidates-both", 0x81);
        require(t.dut.aown_cycles_o == 1 && t.dut.union_cycles_o == 1,
                "error retains captured values");
    }
}

static void endpoint_injections() {
    constexpr uint64_t max64 = std::numeric_limits<uint64_t>::max();
    for (unsigned kind = 0; kind < 4; ++kind) {
        TB t; t.arm(true); t.step(3, 0, 3); t.reference = false;
        // state_t packed bit offsets: a_age278, u_age150, n1054, n1122.
        if (kind != 3) t.inject(278, 64, kind == 0 ? max64 - 1 : max64);
        if (kind != 2) t.inject(150, 64, kind == 0 ? max64 - 1 : max64);
        t.step(0, 3, 3, true, true);
        for (unsigned i = 0; i < 7; ++i) t.step(); // Internal age may saturate after lastdone.
        t.step(0, 0, 0, false, false, true, true);
        const uint64_t status[] = {7, 0x19, 9, 0x11};
        const char *names[] = {"injected-MAX-endpoints-valid-tail-irrelevant",
                              "injected-both-MAX-plus1-invalid",
                              "injected-A-MAX-plus1-invalidates-both",
                              "injected-union-MAX-plus1-invalidates-both"};
        t.finish(names[kind], status[kind]);
        if (kind != 3) require(t.dut.aown_cycles_o == max64, "A saturates without wrap");
        if (kind != 2) require(t.dut.union_cycles_o == max64, "union saturates without wrap");
    }
    for (unsigned did = 10; did <= 11; ++did) {
        TB t; t.arm(true); t.step(3, 0, 3); t.reference = false;
        t.inject(did == 10 ? 54 : 22, 32, UINT32_MAX - 1U);
        t.step(0, 0, 3, true, false, false, false, true, did);
        require(!(t.dut.status_o & 0x40), "N exact MAX still representable");
        t.step(0, 0, 3, true, false, false, false, true, did);
        t.step(0, 3, 3, true, true);
        t.step(0, 0, 0, false, false, true, true);
        t.finish(did == 10 ? "injected-N10-saturation" : "injected-N11-saturation", 0x41);
        require((did == 10 ? t.dut.n_did10_o : t.dut.n_did11_o) == UINT32_MAX,
                "N counter saturates without wrap");
        t.arm(false);
        require(!t.dut.status_o, "ARM clears prior N overflow and both-valid errors");
    }
}
#endif

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    try {
#ifdef PN_DISABLED
        TB t;
        for (unsigned i = 0; i < 64; ++i) {
            t.dut.accepted_arm_i = i & 1; t.dut.has_a_i = 1;
            t.dut.issue_i = 15; t.dut.done_i = 15; t.dut.batch_active_i = 15;
            t.dut.obs_active_i = 1; t.dut.v_target_done_i = 1;
            t.dut.accepted_stop_i = 1; t.dut.all_quiet_i = 1;
            t.dut.n_accept_i = 1; t.dut.n_did_i = i;
            t.clock();
            require(!t.dut.status_o && !t.dut.capture_busy_o && !t.dut.aown_cycles_o &&
                    !t.dut.union_cycles_o && !t.dut.n_did10_o && !t.dut.n_did11_o,
                    "Enable default0 outputs all zero for arbitrary inputs");
        }
        scenarios = 1;
#else
        successful_traces(); malformed_traces(); endpoint_injections();
#endif
        std::cout << "{\"summary\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,"
                  << "\"scenarios\":" << scenarios << ",\"checks\":" << checks << "}\n";
    } catch (const std::exception &error) {
        std::cerr << "actual DUT check failed: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
