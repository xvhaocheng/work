// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vm_first_pte_unit.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

struct Test {
    Vm_first_pte_unit dut;
    unsigned cycles = 0, checks = 0, scenarios = 0;
    unsigned arms = 0, p1s = 0, timeouts = 0;
    unsigned accepts = 0, ds = 0, non_ds = 0;
    bool held = false;
    unsigned held_id = 0, held_index = 0;
    std::string phase = "reset";

    void check(bool condition, const std::string &message) {
        ++checks;
        if (!condition) {
            std::cerr << "FAIL cycle=" << cycles << " phase=" << phase
                      << " check=" << message << '\n';
            throw std::runtime_error("finite protocol assertion failed");
        }
    }
    void settle() { dut.eval(); }
    bool pending() const { return dut.state & 1; }
    bool gate() const { return dut.state & 2; }
    bool escape() const { return dut.state & 4; }
    unsigned remain() const { return (dut.state >> 3) & 127; }
    void id(unsigned index, unsigned value) {
        dut.ids = (dut.ids & ~(uint32_t(63) << (6*index))) |
                  (uint32_t(value) << (6*index));
    }
    unsigned id(unsigned index) const { return (dut.ids >> (6*index)) & 63; }
    void tick() {
        dut.clk = 0;
        settle();
        ++cycles;
        if (dut.rst_n) {
            const unsigned grants = dut.grant;
            check((grants & (grants-1)) == 0, "at most one upstream grant");
            check((grants & ~dut.valid) == 0, "no READY grant to invalid producer");
            check(bool(dut.selected_valid && dut.ready) == bool(grants & dut.valid),
                  "selector and producer handshakes conserve requests");
            check(dut.quota <= 4, "common DS quota saturates at four");
            check((dut.allow & 11) == 11, "M never masks another source");
            if (held) {
                check(dut.selected_valid, "held VALID cannot be withdrawn");
                check(dut.selected_id == held_id && dut.selected_index == held_index,
                      "held data/index cannot change under M or DS/F4");
            }
            if (dut.selected_valid && dut.ready) {
                check((dut.valid & (1u << dut.selected_index)) &&
                      dut.selected_id == id(dut.selected_index),
                      "accepted payload retains its real source identity");
                ++accepts;
                if (dut.selected_index == 3) ++ds;
                else ++non_ds;
            }
            arms += dut.arm;
            p1s += dut.p1_commit;
            timeouts += dut.timeout_event;
            std::cout << "TRACE cycle=" << cycles << " phase=" << phase
                << " start=" << unsigned(dut.start) << " active=" << unsigned(dut.active)
                << " pending=" << pending() << " remain=" << remain()
                << " escape=" << escape() << " gate=" << gate()
                << " valid=" << unsigned(dut.valid) << " allow=" << unsigned(dut.allow)
                << " grant=" << grants << " out_valid=" << unsigned(dut.selected_valid)
                << " ready=" << unsigned(dut.ready) << " out_id=" << unsigned(dut.selected_id)
                << " quota=" << unsigned(dut.quota) << " arm=" << unsigned(dut.arm)
                << " p1=" << unsigned(dut.p1_commit)
                << " timeout=" << unsigned(dut.timeout_event) << '\n';
            held = dut.selected_valid && !dut.ready;
            held_id = dut.selected_id;
            held_index = dut.selected_index;
        } else {
            held = false;
        }
        dut.clk = 1;
        settle();
        dut.clk = 0;
        settle();
    }
    void reset_case(const std::string &name) {
        dut.rst_n = 0;
        dut.enable = 1;
        dut.pair_qualified = 1;
        dut.start = 0;
        dut.active = 0;
        dut.bank = 0;
        dut.valid = 0;
        dut.ready = 1;
        dut.ids = 0;
        id(0, 0); id(1, 16); id(2, 36); id(3, 48);
        phase = name;
        tick(); tick();
        dut.rst_n = 1;
        settle();
        check((dut.state & 1023) == 0 && dut.quota == 0,
              "common reset clears the episode state");
        ++scenarios;
    }
    // Producer boundary mirrors IDLE/start -> MEM_ACCESS/active on one edge.
    // The actual PTW/TL event predicate is tested by root integration.
    void fresh_walk() {
        dut.start = 1;
        dut.active = 0;
        dut.bank = 0;
        settle();
        check(dut.arm && !pending(), "fresh qualified occurrence arms without queue");
        tick();
        check(pending() && gate() && remain() == 64,
              "f1 receiver captures at exactly the occurrence edge");
        dut.start = 0;
        dut.active = 1;
        settle();
        check(!(dut.state & (1u << 15)), "new active walk cannot retire fresh record");
    }
    void run() {
        reset_case("qualification");
        dut.start = 1; dut.bank = 1; settle();
        check(!dut.arm, "non-V bank occurrence is ineligible");
        tick(); check(!pending(), "non-V occurrence stores no pending work");
        dut.start = 0; dut.bank = 0; tick();
        dut.pair_qualified = 0; dut.start = 1; settle();
        check(!dut.arm, "unqualified pair occurrence is skipped");
        tick(); check(!pending(), "unqualified occurrence stores no pending work");
        dut.pair_qualified = 1; dut.start = 0; tick();
        check(!pending(), "later qualification cannot replay skipped occurrence");
        dut.enable = 0; dut.start = 1; settle();
        check(!dut.arm, "disabled occurrence is skipped");
        tick(); check(!pending(), "disabled occurrence stores no pending work");

        reset_case("commit_escape");
        fresh_walk();
        dut.valid = 8;
        const auto p1_before = p1s;
        settle();
        check(dut.selected_valid && dut.selected_id == 48 && dut.p1_commit,
              "full-ID48 selector acceptance consumes exact P1");
        tick();
        check(!pending() && !gate() && escape(), "P1 closes record and starts escape");
        check(p1s == p1_before+1, "P1 is counted exactly on acceptance edge");
        check(!dut.p1_commit, "same ID without pending cannot consume second P1");
        tick();
        check(p1s == p1_before+1, "later same-ID acceptance is not another consume");
        dut.valid = 0; dut.start = 1; dut.active = 0; settle();
        check(!dut.arm, "new occurrence skipped during outstanding escape");
        tick(); check(!pending() && escape(), "escape skip is never queued");
        dut.start = 0; dut.valid = 4; settle();
        check((dut.grant & 4) && dut.selected_id == 36,
              "ordinary A acceptance satisfies escape");
        tick();
        check(!escape() && !pending(), "escape clears without replaying skipped start");
        dut.valid = 0;
        tick(); tick(); tick();
        check(!pending(), "later idle cycles contain no implicit replay");
        fresh_walk();

        reset_case("timeout_identity");
        fresh_walk();
        const auto timeout_before = timeouts;
        for (unsigned n = 1; n <= 64; ++n) {
            dut.valid = 4;
            dut.start = n == 7;
            settle();
            check(!dut.arm, "pending occurrence cannot renew timer");
            check(pending() && gate() && remain() == 65-n,
                  "H decrements in real cycles without renewal");
            check(dut.masked && !(dut.allow & 4) && !(dut.grant & 4) && !dut.selected_valid,
                  "new masked A gets no false READY/VALID handshake");
            check(bool(dut.timeout_event) == (n == 64), "timeout is last fixed lease edge");
            tick();
            check(pending() && remain() == 64-n, "timeout never clears P1 association");
            check(gate() == (n != 64), "suppression ends exactly after H64 edges");
        }
        check(timeouts == timeout_before+1, "one timeout event without renewal");
        check(escape() && (dut.allow & 4) && (dut.grant & 4),
              "expired gate permits ordinary A service");
        dut.start = 0; tick();
        check(pending() && !gate() && !escape(), "non-DS escape preserves P1 association");
        dut.valid = 8; id(3, 49); settle();
        check(!dut.p1_commit, "another DS identity cannot stand in for PTW48");
        tick(); check(pending() && remain() == 0, "wrong-ID acceptance leaves passive pending");
        id(3, 48); settle();
        check(dut.p1_commit, "late exact P1 consumes retained association");
        tick(); check(!pending() && escape(), "late P1 retires association once");

        reset_case("true_retire");
        fresh_walk(); tick(); tick();
        dut.active = 0; dut.start = 1; settle();
        check((dut.state & (1u << 15)) && !dut.arm,
              "old retired walk is classified before any new arm");
        tick();
        check(!pending() && remain() == 0 && escape(),
              "true retirement clears pending/timer and requires escape");
        dut.start = 0; dut.valid = 1; tick();
        check(!pending() && !escape(), "retire-time skipped occurrence is not replayed");

        reset_case("held_A_late_intent");
        dut.ready = 0; dut.valid = 4; settle();
        check(dut.selected_valid && dut.selected_id == 36, "A first offer precedes intent");
        tick();
        fresh_walk();
        check(dut.masked && dut.selected_valid && dut.selected_id == 36,
              "late intent cannot withdraw held A");
        dut.valid = 12;
        for (unsigned n = 0; n < 3; ++n) {
            settle();
            check(dut.selected_valid && dut.selected_id == 36 && dut.grant == 0,
                  "M and later DS priority retain stalled held A");
            tick();
        }
        dut.ready = 1; settle();
        check((dut.grant & 4) && !(dut.grant & 8) && dut.selected_id == 36,
              "held A accepts before newly arrived DS");
        tick(); check(pending(), "held non-DS acceptance is not P1 consume");
        dut.valid = 8; settle();
        check((dut.grant & 8) && !(dut.grant & 4) && dut.selected_id == 48 && dut.p1_commit,
              "P1 follows held A without a fake A handshake");
        tick(); check(!pending(), "P1 closes late-intent association");

        reset_case("common_F4");
        dut.enable = 0; dut.valid = 9; id(3, 49);
        for (unsigned n = 1; n <= 4; ++n) {
            settle();
            check(dut.selected_valid && dut.selected_index == 3 && dut.grant == 8,
                  "current-valid DS wins first four accepts");
            check(dut.quota == n-1, "quota counts previous accepted DS only");
            tick(); check(dut.quota == n, "one accepted DS increments quota once");
        }
        check(dut.selected_index == 0 && dut.selected_id == 0 && dut.grant == 1,
              "eligible non-DS selected after four DS accepts");
        dut.ready = 0;
        for (unsigned n = 0; n < 3; ++n) {
            tick();
            check(dut.quota == 4 && dut.selected_id == 0 && dut.selected_valid,
                  "stalled non-DS retains selection without spending quota");
        }
        dut.ready = 1; settle();
        check(dut.grant == 1, "F4 non-DS cannot be preempted by waiting DS");
        tick(); check(dut.quota == 0, "accepted non-DS resets consecutive-DS quota");
        check(dut.selected_index == 3 && dut.grant == 8, "DS resumes after non-DS service");

        reset_case("F4_held_DS");
        dut.enable = 0; dut.valid = 8; id(3, 49);
        for (unsigned n = 0; n < 5; ++n) tick();
        check(dut.quota == 4 && dut.selected_index == 3, "quota4 never blocks lone DS");
        dut.ready = 0; tick();
        dut.valid = 9; settle();
        check(dut.selected_index == 3 && dut.selected_valid && dut.grant == 0,
              "new non-DS at quota4 cannot steal held DS");
        tick();
        dut.ready = 1; settle();
        check(dut.grant == 8, "held DS completes despite quota4");
        tick();
        check(dut.quota == 4 && dut.selected_index == 0 && dut.grant == 1,
              "new arbitration serves non-DS after held DS completes");
        tick(); check(dut.quota == 0, "post-held non-DS resets quota");

        reset_case("soft_disable");
        fresh_walk();
        dut.valid = 4; dut.enable = 0; settle();
        check(pending() && !gate() && (dut.allow & 4) && (dut.grant & 4),
              "soft disable permits A without deleting association");
        tick(); check(pending() && remain() == 0, "soft disable preserves passive pending");
        dut.valid = 8; settle();
        check(dut.p1_commit, "disabled record consumes eventual exact P1");
        tick(); check(!pending(), "disabled association closes normally");

        std::cout << "PASS scenarios=" << scenarios << " checks=" << checks
                  << " cycles=" << cycles << " arms=" << arms << " p1_consumes=" << p1s
                  << " timeouts=" << timeouts << " accepted=" << accepts
                  << " ds=" << ds << " non_ds=" << non_ds << '\n';
    }
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    try { Test test; test.run(); }
    catch (const std::exception &error) { std::cerr << error.what() << '\n'; return 1; }
    return 0;
}
